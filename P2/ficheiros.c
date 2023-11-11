#include "ficheiros.h"
//Devolve o tipo de ficheiro
char LetraTF (mode_t m)
{
    switch (m&S_IFMT) { /*and bit a bit con los bits de formato,0170000 */
        case S_IFSOCK: return 's'; /*socket */
        case S_IFLNK: return 'l'; /*symbolic link*/
        case S_IFREG: return '-'; /* fichero normal*/
        case S_IFBLK: return 'b'; /*block device*/
        case S_IFDIR: return 'd'; /*directorio */
        case S_IFCHR: return 'c'; /*char device*/
        case S_IFIFO: return 'p'; /*pipe*/
        default: return '?'; /*desconocido, no deberia aparecer*/
    }
}
//Da os permisos do ficheiro
char * ConvierteModo2 (mode_t m){
    static char permisos[12];
    strcpy (permisos,"---------- ");

    permisos[0]=LetraTF(m);
    if (m&S_IRUSR) permisos[1]='r';    /*propietario*/
    if (m&S_IWUSR) permisos[2]='w';
    if (m&S_IXUSR) permisos[3]='x';
    if (m&S_IRGRP) permisos[4]='r';    /*grupo*/
    if (m&S_IWGRP) permisos[5]='w';
    if (m&S_IXGRP) permisos[6]='x';
    if (m&S_IROTH) permisos[7]='r';    /*resto*/
    if (m&S_IWOTH) permisos[8]='w';
    if (m&S_IXOTH) permisos[9]='x';
    if (m&S_ISUID) permisos[3]='s';    /*setuid, setgid y stickybit*/
    if (m&S_ISGID) permisos[6]='s';
    if (m&S_ISVTX) permisos[9]='t';

    return permisos;
}

void nomeDendeDireccion(char* direccion, char* nome){
    int l = strlen(direccion), i;
    for(i = l; direccion[i-1]!='/'; i--);
    strcpy(nome,&direccion[i]);
}

//Funcion auxiliar para imprimir os datos dun arquivo segundo os parametros que nos dan
void ImprimirDatos(char *argumentos[], int numDir, int numOpcions, bool HaiLong, bool HaiAcc, bool HaiLink) {
    struct stat datosArchivo;//Struct para almacenar datos de ficheiro
    char nome[256];

    if (numDir == 0) {//Se non hai directorios, mostro a carpeta onde estou
        char s[MAX_PATHSTRING];
        getcwd(s, MAX_PATHSTRING);
        printf("%s\n", s);
        return;
    }
        //Se es un archvo normal no pasa nada, pero si tengo un link, tengo que darme los datos del link no del fichero al que apunta

    else {
        for (int i = numOpcions; i <= numDir + numOpcions - 1; ++i) {
            if (lstat(argumentos[i], &datosArchivo) < 0) {
                perror("Erro ao obter informacion do ficheiro");
                return;
            }
            char L = LetraTF(datosArchivo.st_mode);

            if(argumentos[i][0]=='/')
                nomeDendeDireccion(argumentos[i],nome);
            else strcpy(nome,argumentos[i]);

            if (L == 'l') {//Si ponen -link, pero no es un link simbólico, entonces no muestro a donde apunta el link, pues no apunta a nada
                char *path = argumentos[i]; // Ruta al enlace simbólico
                char Destino[100]; // Buffer para almacenar el destino del enlace

                ssize_t len = readlink(path, Destino, sizeof(Destino) - 1);
                Destino[len] = 0;

                if (len < 0) {
                    perror("Erro ao ler o enlace simbolico");
                    return;
                }

                if (!HaiLong) {
                    printf("%9lu  %s\n", strlen(Destino), argumentos[i]);
                } else {
                    struct tm *timeinfo;//Struct para almacenar o tempo, terá tempo de ultimo acceso, ou de modificación segundo o introducido
                    if (HaiAcc)
                        timeinfo = localtime(&datosArchivo.st_atime);
                    else
                        timeinfo = localtime(&datosArchivo.st_mtime);

                    printf("%02d-%02d-%d %02d:%02d   %lu (%ld) %s %s %s",timeinfo->tm_mday, timeinfo->tm_mon + 1,
                           timeinfo->tm_year + 1900, timeinfo->tm_hour,timeinfo->tm_min, datosArchivo.st_nlink,
                           datosArchivo.st_ino,getpwuid(datosArchivo.st_uid)->pw_name,getgrgid(datosArchivo.st_gid)->gr_name,
                           ConvierteModo2(datosArchivo.st_mode));
                    if(HaiLink) {
                        //Mostro o tempo,numero de links, numero de inodo, usuario e grupo e permisos de archivo,
                        printf(" %9lu  %s -> %s\n", strlen(Destino), nome, Destino);
                    }else
                        printf("%9lu  %s\n",strlen(Destino), nome);

                }
            } else {

                if (stat(argumentos[i], &datosArchivo) < 0) {
                    perror("Erro ao obter informacion do ficheiro");
                    return;
                }


                if (!HaiLong)
                    printf("%9ld  %s\n", datosArchivo.st_size, nome);
                else {
                    struct tm *timeinfo;//Struct para almacenar o tempo, terá tempo de ultimo acceso, ou de modificación segundo o introducido
                    if (HaiAcc)
                        timeinfo = localtime(&datosArchivo.st_atime);
                    else
                        timeinfo = localtime(&datosArchivo.st_mtime);
                    //Mostro o tempo,numero de links, numero de inodo, usuario e grupo e permisos de archivo,
                    printf("%02d-%02d-%d %02d:%02d   %lu (%ld) %s %s %s %9ld  %s\n",
                           timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour,
                           timeinfo->tm_min, datosArchivo.st_nlink, datosArchivo.st_ino,
                           getpwuid(datosArchivo.st_uid)->pw_name,
                           getgrgid(datosArchivo.st_gid)->gr_name, ConvierteModo2(datosArchivo.st_mode),
                           datosArchivo.st_size, nome);

                }
            }
        }
    }
}

int crearComandoRecursivo(char **arg, char *path, bool hid, bool longo, bool link, bool acc){
    int modo = hid + 2 * longo + 4 * link + 8 * acc, pathPos;
    switch (modo) {
        case 1:
            arg[0]="-hid"; pathPos=1;
            break;
        case 2:
            arg[0]="-long"; pathPos=1;
            break;
        case 4:
            arg[0]="-link"; pathPos=1;
            break;
        case 8:
            arg[0]="-acc"; pathPos=1;
            break;
        case 3:
            arg[0]="-hid"; arg[1]="-long"; pathPos=2;
            break;
        case 5:
            arg[0]="-hid"; arg[1]="-link"; pathPos=2;
            break;
        case 9:
            arg[0]="-hid"; arg[1]="-acc"; pathPos=2;
            break;
        case 6:
            arg[0]="-long"; arg[1]="-link"; pathPos=2;
            break;
        case 10:
            arg[0]="-long"; arg[1]="-acc"; pathPos=2;
            break;
        case 12:
            arg[0]="-link"; arg[1]="-acc"; pathPos=2;
            break;
        case 7:
            arg[0]="-hid"; arg[1]="-long"; arg[2]="-link"; pathPos=3;
            break;
        case 11:
            arg[0]="-hid"; arg[1]="-long"; arg[2]="-acc"; pathPos=3;
            break;
        case 13:
            arg[0]="-hid"; arg[1]="-link"; arg[2]="-acc"; pathPos=3;
            break;
        case 14:
            arg[0]="-long"; arg[1]="-link"; arg[2]="-acc"; pathPos=3;
            break;
        case 15:
            arg[0]="-hid"; arg[1]="-long"; arg[2]="-link"; arg[3]="-acc"; pathPos=4;
        default:
            pathPos=0;
            break;
    }
    arg[pathPos]=path;
    arg[pathPos+1]=NULL;
    return pathPos;
}

//Función para listar ficheiros nun directorio
void listarFicheiros(char *argumentos[], short int modoRec, int numrec){
    struct dirent **ficheiros;
    char s[MAX_PATHSTRING], *nomeFich, nomeAux[MAX_PATHSTRING], separacionRec[64] = "",
            path[MAX_PATHSTRING], *pathRec[6];
    int n, i, j, k, numOps;
    short int recursivo = modoRec; // 0 = nada, 1 = reca, 2 = recb
    bool fichOcultos=false, soNomes=false, listarLongo=false, darLinks=false, datasAcceso=false;
    if(argumentos == NULL || *argumentos==NULL) {
        printf("%s\n", getcwd(s, 100)); return;
    }
    for(i=0; argumentos[i]!=NULL; i++){
        if(argumentos[i][0]=='-' && !soNomes) {
            if (strcmp(argumentos[i], "-reca") == 0 && recursivo == 0) recursivo = 1;
            else if (strcmp(argumentos[i], "-recb") == 0 && recursivo == 0) recursivo = 2;
            else if(strcmp(argumentos[i], "-hid") == 0) fichOcultos = true;
            else if(strcmp(argumentos[i], "-long") == 0) listarLongo = true;
            else if(strcmp(argumentos[i], "-link")==0) darLinks = true;
            else if(strcmp(argumentos[i], "-acc")==0) datasAcceso = true;
        } else {
            soNomes=true; // indica a partir de que argumento só se collen nomes de ficheiros
            printf("Directorio: %s\n",argumentos[i]);
            n = scandir(argumentos[i], &ficheiros,NULL,alphasort);
            if(n==-1){
                perror("Erro na lectura do directorio actual"); return;
            } else if (n <= 2) {
                printf("(Directorio baleiro)\n");
                free(ficheiros[0]); free(ficheiros[1]);
            } else {
                for(j=0; j<n; ++j) { // percorremos os ficheiros do directorio i
                    nomeFich = ficheiros[j]->d_name;
                    if (strcmp(nomeFich, ".") != 0 && strcmp(nomeFich, "..") != 0){
                        realpath(argumentos[i], path);
                        strcat(path, "/");
                        strcat(path, nomeFich);
                        numOps = crearComandoRecursivo(pathRec,path,fichOcultos,
                                                       listarLongo,darLinks,datasAcceso);
                        if (recursivo == 2 && ficheiros[j]->d_type == 4) { // recb
                            listarFicheiros(pathRec, 2, numrec + 1);
                        }
                        // decídese se imprime en función de se empeza por punto (= fich oculto) ou non
                        if (fichOcultos || (!fichOcultos && nomeFich[0] != '.')
                            || (!fichOcultos && strcmp(nomeFich, "..") == 0 && numrec==0) ){
                            ImprimirDatos(pathRec, 1, numOps, listarLongo, datasAcceso, darLinks);
                            if (j == n - 1) {
                                for (k = 0; k <= numrec; k++)
                                    strcat(separacionRec, "--");
                                printf("%s\n", separacionRec);
                            }
                        }
                        if (recursivo == 1 && ficheiros[j]->d_type == 4) { // reca
                            listarFicheiros(pathRec, 1, numrec + 1);
                        }
                    }
                    free(ficheiros[j]);
                }
            }
            free(ficheiros);
        }
    }
}

// borra o ficheiro ou ficheiros pasador polo array de strings "argumentos"
void borrarFicheiros(char *argumentos[], bool recursivo) {
    int i, j, n;
    struct stat st;
    struct dirent **ficheiros;
    char *s[2], path[MAX_PATHSTRING], nomeFich[MAX_PATHSTRING], pathAux[MAX_PATHSTRING];

    if (argumentos == NULL || *argumentos == NULL) {
        strerror(EINVAL);
        perror("Non se introduciu ningun ficheiro a borrar");
    } else {
        for (i = 0; argumentos[i] != NULL; i++) {
            realpath(argumentos[i],path);
            if (recursivo) {
                n = scandir(argumentos[i], &ficheiros, NULL, alphasort);
                stat(argumentos[i], &st);
                if (n == -1) {
                    perror("Erro na lectura do directorio actual");
                    return;
                } else {
                    free(ficheiros[0]);
                    free(ficheiros[1]);
                    for (j = n-1; j >= 2; j--) {
                        strcpy(nomeFich, ficheiros[j]->d_name);
                        if (lstat(path, &st) == -1) { // non existe o ficheiro ou os datos non son válidos
                            perror("Erro na lectura dos datos do directorio");
                            continue;
                        }
                        strcpy(pathAux,path);
                        strcat(pathAux,"/"); strcat(pathAux,nomeFich);
                        s[0] = pathAux;
                        s[1] = NULL;
                        // borra o ficheiro j, recursivamente se é directorio
                        borrarFicheiros(s, ficheiros[j]->d_type==4);
                        free(ficheiros[j]);
                    }
                    if (remove(path) == -1)
                        perror("Imposible borrar o ficheiro");
                } free(ficheiros);
            } else if (remove(path) == -1)
                perror("Imposible borrar o ficheiro");
        }
    }
}


//Función encargada de cambiar de directorio de traballo
int cambiarDirectorio(char *argumentos[]) {
    char s[100];//Inicializamos as variables necesarias

    if (argumentos[0]!=NULL){//Se se lle pasou un argumento
        if(chdir(argumentos[0])==-1)//Se non se introduce un directorio válido, non fai o cambio
            printf("O directorio introducido non existe. O directorio non foi cambiado\n");//Mostrando unha mensaxe de erro
    }
    else
        printf("%s\n", getcwd(s, 100));//Se non se lle pasou argumeto, mostra o directorio de traballo actual
    return 0;
}

// Crea unha táboa de ficheiros baleira, neste caso un punteiro a NULL
void crearTaboaFich(taboaFicheiros *t) {
    *t = NULLFICH;
}

//Obtén o nome dun ficheiro segundo o seu descriptor
char* nomeFichSegundoDescriptor(int descr, taboaFicheiros t){
    for(; t!=NULLFICH && t->descriptor!=descr;t=t->next);//Crea un bucle que non para ata chegar ao final ou ata que os nomes coincidan
    if(t==NULLFICH)//Se chegou ao final, non hai ficheiro con ese descriptor, polo que non se pode devolver nome ningún, polo que devolvemos nulo
        return NULL;
    return t->nome;
}

// Inserta un ficheiro f na táboa de ficheiros t
void insertarFicheiroEnTaboa(int modo, char* nomefich, unsigned int desc, taboaFicheiros *t) {
    taboaFicheiros tAux, tCnt;
    tAux = malloc(sizeof(ficheiro));

    strcpy(tAux->nome,nomefich);
    tAux->modo=modo;
    tAux->descriptor=desc;
    tAux->next=NULLFICH;
    if(*t==NULLFICH)
        *t = tAux;
    else {
        for (tCnt = *t; tCnt->next != NULLFICH; tCnt = tCnt->next);
        tCnt->next = tAux;
    }
}

//Elimina o ficheiro cuxo descriptor coincida con indiado
void eliminarFicheiroDeTaboa(int descr, taboaFicheiros *t) {
    taboaFicheiros tElim=*t, tAux; //tElim indica o ficheiro a eliminar e tAux o anteior para enlazar se é necesario

    if(tElim==NULLFICH) {//Se non hai ficheiros na taboa enton non podemos eliminar
        printf("Non hai ficheiros que eliminar, a taboa esta baleira");
        return;
    }
    else if(tElim->next==NULLFICH){ //Comprobamos se a táboa ten un único elemento
        if(tElim->descriptor!=descr){
            printf("Ficheiro co descriptor dado non atopado");
            return;
        } else{ // pechamos o ficheiro e liberamos memoria
            free(tElim);
            *t = NULL;
        }
    } else{
        for(; tElim->next!=NULLFICH && tElim->descriptor!=descr; tAux=tElim, tElim=tElim->next);//Se hai ficheiros percorremos a taboa ata o final ata atopalo
        if(tElim->next==NULLFICH)// Se é o ultimo
            if(tElim->descriptor==descr)//E o descriptor coincide
                tAux->next=NULLFICH; //tAux pasará a ser o último elemento da táboa
            else {//Se non atopamos o archivo co descriptor indicado
                printf("Ficheiro co descriptor dado non atopado");
                return;
            }
        else {
            tAux->next = tElim->next;
            tElim->next = NULL;
            free(tElim);//Liberamos a memoria
        }
    }
}

void pecharTodoFicheiro(taboaFicheiros *t){
    taboaFicheiros tAux;
    int df;

    if(*t != NULL)
        for (tAux = *t; tAux->next != NULL; tAux = tAux->next) {
            df = tAux->descriptor;
            close(df);
            eliminarFicheiroDeTaboa(df, t);
        }
}

// Imprime por pantalla os ficherios que constan abertos na táboa t
void listarAbertos(taboaFicheiros t) {
    if(t==NULLFICH)
        printf("Non hai ningún ficheiro aberto no momento\n");//Se non hai lista, enton non podemos listar nada
    else {
        printf("Descriptor\t|\tModo\t\t|\tNome\n");//Se hai lista
        for (; t != NULL; t = t->next)//Percorremola
            printf("%d\t\t\t|\t\t%d\t\t|\t%s\n",t->descriptor,t->modo,t->nome);//Imprimindo cada un dos ficheiros actualemte abertos
    }
}

//Abre un ficueiro indicado
void Cmd_open (char * tr[], taboaFicheiros *t) {
    int i, df, mode = 0;//Creación das variables iniciais

    if (tr[0] == NULL) { //Se  non hai parámetro, non sabemos como abrir,
        return;
    }
    for (i = 1; tr[i] != NULL; i++)//Vamos buscando con uqe sintaxe se correponde o argumento dado
        if (!strcmp(tr[i], "cr")) mode |= O_CREAT;
        else if (!strcmp(tr[i], "ex")) mode |= O_EXCL;
        else if (!strcmp(tr[i], "ro")) mode |= O_RDONLY;
        else if (!strcmp(tr[i], "wo")) mode |= O_WRONLY;
        else if (!strcmp(tr[i], "rw")) mode |= O_RDWR;
        else if (!strcmp(tr[i], "ap")) mode |= O_APPEND;
        else if (!strcmp(tr[i], "tr")) mode |= O_TRUNC;
        else break;

    if ((df = open(tr[0], mode, 0777)) == -1)//Se da un erro o comando open
        perror("Imposible engadir ficheiro");//Mostramos a mensaxe e
    else {
        insertarFicheiroEnTaboa(mode, tr[0],df, t);//Se non insertamos e mostramos un mensaxe de o insertado
        printf("Engadida entrada a taboa ficheiros abertos %s\n", tr[0]);
    }
}

//Cerramos un ficheiro cuxo file descriptor correposnda co dado
void Cmd_close (char *tr[], taboaFicheiros *t){
    int df;

    if (tr[0]==NULL || (df=atoi(tr[0]))<0) { //Se non hai parametros,
        listarAbertos(*t);//Listo os ficheiros abertos e retorno
        return;
    }
    if (close(df)==-1)//Se falla o close, non podemos pechar o ficheiro
        printf("Imposible pechar descriptor\n");// Enviamos unha mensaxe
    else
        eliminarFicheiroDeTaboa(df,t); //Eliminamos o ficheiro indicado
}

// Duplicamos un archivo polo seu file descriptor
void Cmd_dup (char * tr[], taboaFicheiros *t){
    int df, duplicado, mododup;//Variables iniciais
    char aux[MAXNAME],*p;

    if (tr[0]==NULL || (df=atoi(tr[0]))<0) { // sen parámetro ou é 0
        listarAbertos(*t);//Listamos os archivos abertos e volvemos ao bucle
        return;
    }
    p=nomeFichSegundoDescriptor(df, *t);//Encontramos o ficheiro a duplicar
    if(p==NULL){//Se non existe,
        printf("Non existe ficheiro con tal descriptor\n");// mostramolo por pantalla e saimos
        return;
    }
    sprintf (aux,"dup %d (%s)",df, p);
    duplicado = dup(df);

    mododup=fcntl(df,F_GETFL)-32768;
    insertarFicheiroEnTaboa(mododup,aux,duplicado,t);//Senon insertamos o ficheiro en taboa e
    printf("Ficheiro duplicado correctamente\n");//Mostramos o mensaxe de proceso correcto.
}



//Crea un ficheiro co nombe indicado
void create (char *argumentos[]) {

    if (argumentos[0] == NULL)//Se non hai argumentos, non é un nome válido
        printf("No se introduciu un nome de ficheiro valido\n");
    else if(strcmp(argumentos[0],"-f")==0) {//SI ponen -f creo un ficheiro, sino un directorio
        if (open(argumentos[1], O_CREAT, 0777) < 0)
            perror("Non puido crearse o ficheiro");
    }else if( mkdir(argumentos[0],0777) < 0)
        perror("Non puido crearse o directorio");
}

//Proporciona os stats dun stat dun ou mais dun  ficheiro dado
void stats (char *argumentos[]){
    int numComandos=0, numDir=0;
    bool HaiLong=false,HaiAcc=false,HaiLink=false;//Booleanos, para que funcione correctamente

    if ((*argumentos)==NULL){//Se non hai argumentos, poño a dirección
        char s[MAX_PATHSTRING];
        getcwd(s, MAX_PATHSTRING);
        printf("%s\n", s);
        return;
    }
//Se o argumento é baleiro,  non se pode dar a infomración dese ficheiro
    if (strcmp(argumentos[0],"")==0) {
        printf("No ejecutado, no such file or directory");
        return;
    }
//Contamos na entrada proporcionada cantos ficheiros se introduciron, así como que opcions de stat están presentes no comando introducido
    for (int j = 0; argumentos[j]!=NULL; ++j) {
        if (strcmp(argumentos[j],"-long")==0){
            numComandos++;
            HaiLong=true;

        }else if (strcmp(argumentos[j],"-acc")==0){
            numComandos++;
            HaiAcc=true;
        }else if(strcmp(argumentos[j],"-link")==0){
            numComandos++;
            HaiLink=true;

        }else {
            numDir++;
        }

    }
//Funcion auxiliar para mostrar por pantalla segundo os datos introducidos.
    ImprimirDatos(argumentos,numDir,numComandos,HaiLong,HaiAcc,HaiLink);
}


