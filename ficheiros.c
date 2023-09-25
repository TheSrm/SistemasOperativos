#include "ficheiros.h"

void listarFicheiros(){
    struct dirent **ficheiros;
    int n, i;

    n = scandir(".", &ficheiros,NULL,alphasort);
    if(n==-1){
        printf("Erro na lectura do directorio actual\n");
        return;
    }
    for(i=0; i<=n; i++){
        printf("%s\n", ficheiros[i]->d_name);
        free(ficheiros[i]);
    }
    free(ficheiros);
}
/*
void Cmd_open (char * argumentos[]) {
    int i, df, mode = 0;

    if (argumentos[0] == NULL) { //no hay parametro
        //ListarFicherosAbiertos
        return;
    }
    for (i = 1; argumentos[i] != NULL; i++)
        if (!strcmp(argumentos[i], "cr")) mode |= O_CREAT;
        else if (!strcmp(argumentos[i], "ex")) mode |= O_EXCL;
        else if (!strcmp(argumentos[i], "ro")) mode |= O_RDONLY;
        else if (!strcmp(argumentos[i], "wo")) mode |= O_WRONLY;
        else if (!strcmp(argumentos[i], "rw")) mode |= O_RDWR;
        else if (!strcmp(argumentos[i], "ap")) mode |= O_APPEND;
        else if (!strcmp(argumentos[i], "tr")) mode |= O_TRUNC;
        else break;

    if ((df = open(argumentos[0], mode, 0777)) == -1)
        perror("Imposible abrir fichero");
    else {
        //AnadirAFicherosAbiertos(descriptor...modo...nombre....)
        //printf("Anadida entrada a la tabla ficheros abiertos..................", ......);
    }
}

void Cmd_close (char *argumentos[]){
    int df;

    if (argumentos[0]==NULL || (df=atoi(argumentos[0]))<0) { //no hay parametro
        // istarFicherosAbiertos............... //o el descriptor es menor que 0
        return;
    }

    if (close(df)==-1)
        perror("Imposible cerrar descriptor");
    else
        //........EliminarDeFicherosAbiertos......
}

void Cmd_dup (char * argumentos[]){
    int df, duplicado;
    char aux[],*p;

    if (argumentos[0]==NULL || (df=atoi(argumentos[0]))<0) { //no hay parametro
        ListOpenFiles(-1);   //Supongo qeu es una definicón estática, adaptar a nuestr lista       //o el descriptor es menor que 0
        return;
    }


    //p=.....NombreFicheroDescriptor(df).......;
    sprintf (aux,"dup %d (%s)",df, p);
    //.......AnadirAFicherosAbiertos......duplicado......aux.....fcntl(duplicado,F_GETFL).....;
}*/