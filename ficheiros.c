#include "ficheiros.h"
/* Eu rompéndome a cabeza e ao final non serve
 * Gardámola de todas formas porque é posible que teñamos que facer algo tipo ls
void listarFicheiros(){
    struct dirent **ficheiros;
    int n, i;

    n = scandir(".", &ficheiros,NULL,alphasort);
    if(n==-1){
        printf("Erro na lectura do directorio actual\n");
        return;
    }
    for(i=0; i<n; ++i){
        printf("%s\n", ficheiros[i]->d_name);
        free(ficheiros[i]);
    }
    free(ficheiros);
}
 */
int cambiarDirectorio(char *argumentos[]) {
    char s[100];

    if (argumentos[0]!=NULL){
        if(chdir(argumentos[0])==-1)
            printf("O directorio introducido non existe. O directorio non foi cambiado\n");
    }
    else
        printf("%s\n", getcwd(s, 100));
    return 0;
}

// Crea unha táboa de ficheiros baleira, neste caso un punteiro a NULL
void crearTaboaFich(taboaFicheiros *t) {
    *t = NULLFICH;
}

char* nomeFichSegundoDescriptor(int descr, taboaFicheiros t){
    for(t; t!=NULLFICH && t->descriptor!=descr;t=t->next);
    if(t==NULLFICH)
        return NULL;
    return t->nome;
}

// Inserta un ficheiro f na táboa de ficheiros t
void insertarFicheiroEnTaboa(int modo, char* nomefich, unsigned int desc, taboaFicheiros *t) {
    taboaFicheiros tAux, tCnt;
    tAux = malloc(sizeof(taboaFicheiros));
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

void eliminarFicheiroDeTaboa(int descr, taboaFicheiros *t) {
    taboaFicheiros tElim, tAux;

    if(*t==NULLFICH)
        perror("Non hai ficheiros que eliminar, a taboa esta baleira");
    else {
        for(tElim = *t; tElim->next!=NULLFICH && tElim->descriptor!=descr; tAux=tElim, tElim=tElim->next);
        if(tElim->next==NULLFICH)
            if(tElim->descriptor==descr)
                tAux->next=NULLFICH;
            else
                perror("Arquivo co descriptor dado non atopado");
        else
            tAux->next = tElim->next;
        free(tElim->nome);
        free(tElim);
    }
}

// Imprime por pantalla os ficherios que constan abertos na táboa t
void listarAbertos(taboaFicheiros t) {
    if(t==NULLFICH)
        printf("Non hai ningún ficheiro aberto no momento\n");
    else {
        printf("Descriptor\t|\tModo\t\t|\tNome\n");
        for (t; t != NULL; t = t->next)
            printf("%d\t\t\t|\t\t%d\t\t|\t%s\n",t->descriptor,t->modo,t->nome); // o ideal sería pasar o modo coma string, xa se verá
    }
}

void Cmd_open (char * tr[], taboaFicheiros *t) {
    int i, df, mode = 0;

    if (tr[0] == NULL) { // non hai parámetro
        return;
    }
    for (i = 1; tr[i] != NULL; i++)
        if (!strcmp(tr[i], "cr")) mode |= O_CREAT;
        else if (!strcmp(tr[i], "ex")) mode |= O_EXCL;
        else if (!strcmp(tr[i], "ro")) mode |= O_RDONLY;
        else if (!strcmp(tr[i], "wo")) mode |= O_WRONLY;
        else if (!strcmp(tr[i], "rw")) mode |= O_RDWR;
        else if (!strcmp(tr[i], "ap")) mode |= O_APPEND;
        else if (!strcmp(tr[i], "tr")) mode |= O_TRUNC;
        else break;

    if ((df = open(tr[0], mode, 0777)) == -1)
        perror("Imposible engadir ficheiro");
    else {
        insertarFicheiroEnTaboa(mode, tr[0],df, t);
        printf("Engadida entrada a taboa ficheiros abertos %s\n", tr[0]);
    }
}
void Cmd_close (char *tr[], taboaFicheiros *t){
    int df;

    if (tr[0]==NULL || (df=atoi(tr[0]))<0) { // sen parámetro ou é negativo
        listarAbertos(*t);
        return;
    }
    if (close(df)==-1)
        printf("Imposible pechar descriptor\n");
    else
        eliminarFicheiroDeTaboa(df,t);
}

    // función incompleta
void Cmd_dup (char * tr[], taboaFicheiros *t){
    int df, duplicado;
    char aux[MAXNAME],*p;

    if (tr[0]==NULL || (df=atoi(tr[0]))<0) { // sen parámetro ou é 0
        listarAbertos(*t);
        return;
    }
    p=nomeFichSegundoDescriptor(df, *t);
    if(p==NULL){
        printf("Non existe ficheiro con tal descriptor\n");
        return;
    }
    sprintf (aux,"dup %d (%s)",df, p);
    duplicado = dup(df);

    insertarFicheiroEnTaboa(fcntl(df,F_GETFD),aux,duplicado,t);
    printf("Ficheiro duplicado correctamente\n");
}

