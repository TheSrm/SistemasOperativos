#include "procesos.h"

void pid(char *argumentos[MAXARGS]) {//Importante, en Windows non funciona, pero en Linux vai ben se quito os comentarios
    pid_t process_id=getpid();
//    pid_t p_process_id=getppid();

    if (argumentos[0]==NULL) {
        printf("The process id: %lli\n", process_id);
    }
    //else
        //   printf("The process id of parent function: %lli\n",p_process_id);

}

void Cmd_open (char * argumentos[MAXARGS]){
    int i,df, mode=0;

    if (argumentos[0]==NULL) { /*no hay parametro*/
        ..............ListarFicherosAbiertos...............
        return;
    }
    for (i=1; argumentos[i]!=NULL; i++)
        if (!strcmp(argumentos[i],"cr")) mode|=O_CREAT;
        else if (!strcmp(argumentos[i],"ex")) mode|=O_EXCL;
        else if (!strcmp(argumentos[i],"ro")) mode|=O_RDONLY;
        else if (!strcmp(argumentos[i],"wo")) mode|=O_WRONLY;
        else if (!strcmp(argumentos[i],"rw")) mode|=O_RDWR;
        else if (!strcmp(argumentos[i],"ap")) mode|=O_APPEND;
        else if (!strcmp(argumentos[i],"tr")) mode|=O_TRUNC;
        else break;

    if ((df=open(argumentos[0],mode,0777))==-1)
        perror ("Imposible abrir fichero");
    else{
        ...........AnadirAFicherosAbiertos (descriptor...modo...nombre....)....
        printf ("Anadida entrada a la tabla ficheros abiertos..................",......);
    }
