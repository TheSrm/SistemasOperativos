#include "ficheiros.h"

//Función para listar ficheiros nun directorio
void listarFicheiros(char *argumentos[]){
    struct dirent **ficheiros;
    struct stat st;
    char s[100];
    int n, i;

    if(argumentos==NULL) {
        printf("%s\n", getcwd(s, 100)); return;
    }
    /* for (i=0; argumentos[i]!=NULL; ++i) {
         if(strcmp)
     }*/
    n = scandir(".", &ficheiros,NULL,alphasort);
    stat(".",&st);
    if(n==-1){
        printf("Erro na lectura do directorio actual\n");
        return;
    }
    for(i=0; i<n; ++i){ // sen argumentos lista, por defecto, tamaño e nome
        printf("%ld\t%s\n", st.st_size, ficheiros[i]->d_name);
        free(ficheiros[i]);
    }
    free(ficheiros);
}

// borra o ficheiro ou ficheiros pasador polo array de strings "argumentos"
void borrarFicheiros(char *argumentos[], bool recursivo) {
    int i, j, n;
    struct stat st;
    struct dirent **ficheiros;
    char* s[1];

    if (argumentos == NULL || *argumentos == NULL) {
        strerror(EINVAL);
        perror("Non se introduciu ningun ficheiro a borrar");
    } else {
        for (i = 0; argumentos[i] != NULL; i++) {
            if (recursivo) {
                n = scandir(argumentos[i], &ficheiros, NULL, alphasort);
                stat(argumentos[i], &st);
                if (n == -1) {
                    perror("Erro na lectura do directorio actual");
                    return;
                } else {
                    free(ficheiros[0]);
                    free(ficheiros[1]);
                    for (j = 2; j < n; j++) { //directorios: tipo 4
                        s[0] = ficheiros[j]->d_name;
                        borrarFicheiros(s, ficheiros[j]->d_type==4);
                        free(ficheiros[j]);
                    }
                    if (remove(argumentos[i]) == -1)
                        perror("Imposible borrar o ficheiro");
                }
            } else if (remove(argumentos[i]) == -1)

                perror("Imposible borrar o ficheiro");
        }
        free(ficheiros);
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
    tAux->nome = malloc(sizeof(char)*50);

    strcpy(tAux->nome,nomefich);
    tAux->modo=modo;
    tAux->descriptor=desc;
    if(*t==NULLFICH){
        tAux->next=NULLFICH;
        *t = tAux;
    } else {
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
            printf("Arquivo co descriptor dado non atopado");
            return;
        } else{ // pechamos o ficheiro e liberamos memoria
            free(tElim->nome);
            free(tElim);
            *t = NULL;
        }
    } else{
        for(; tElim->next!=NULLFICH && tElim->descriptor!=descr; tAux=tElim, tElim=tElim->next);//Se hai ficheiros percorremos a taboa ata o final ata atopalo
        if(tElim->next==NULLFICH)// Se é o ultimo
            if(tElim->descriptor==descr)//E o descriptor coincide
                tAux->next=NULLFICH; //tAux pasará a ser o último elemento da táboa
            else {//Se non atopamos o archivo co descriptor indicado
                printf("Arquivo co descriptor dado non atopado");
                return;
            }
        else {
            tAux->next = tElim->next; // Illamos o elemento a eliminar
            free(tElim->nome);//Liberamos a memoria
            free(tElim);
        }
    }
}

void pecharTodoFicheiro(taboaFicheiros *t){
    taboaFicheiros tAux = *t;
    for (; tAux != NULL; tAux = tAux->next) {
        close(tAux->descriptor);
        eliminarFicheiroDeTaboa(tAux->descriptor, t);
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
        printf("Imposible engadir ficheiro");//Mostramos a mensaxe e
    else {
        insertarFicheiroEnTaboa(mode, tr[0],df, t);//Se non insertamos e mostramos un mensaxe de o insertado
        printf("Engadida entrada a taboa ficheiros abertos %s\n", tr[0]);
    }
}

//Cerramos un ficheiro cuxo file descriptor correposnda co dado
void Cmd_close (char *tr[], taboaFicheiros *t){
    int df;

    if (tr[0]==NULL || (df=atoi(tr[0]))<0) { //Se non hai parametros,
        listarAbertos(*t);//Listo os archivos abertos e retorno
        return;
    }
    if (close(df)==-1)//Se falla o close, non podemos cerrar o archivo
        printf("Imposible pechar descriptor\n");// Enviamos unha mensaxe e
    else
        eliminarFicheiroDeTaboa(df,t);//Eliminamos o ficheiro indicado
}

// Duplicamos un archivo polo seu file descriptor
void Cmd_dup (char * tr[], taboaFicheiros *t){
    int df, duplicado;//Variables iniciais
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

    insertarFicheiroEnTaboa(fcntl(df,F_GETFD),aux,duplicado,t);//Senon insertamos o ficheiro en taboa e
    printf("Ficheiro duplicado correctamente\n");//Mostramos o mensaxe de proceso correcto.
}

