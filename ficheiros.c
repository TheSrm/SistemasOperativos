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

// Crea unha táboa de ficheiros baleira, neste caso un punteiro a NULL
void crearTaboaFich(taboaFicheiros *t) {
    *t = NULLFICH;
}

// Inserta un ficheiro f na táboa de ficheiros t (ordeada polos descriptores?)
void insertarEnTaboa(taboaFicheiros *t, ficheiro f) {
    *t = malloc(sizeof(taboaFicheiros));
}

// Imprime por pantalla os ficherios que constan abertos na táboa t
void listarAbertos(taboaFicheiros t) {
    if(t==NULLFICH)
        printf("Non hai ningún ficheiro aberto no momento\n");
    else {
        printf("Descriptor\t|\tModo\t|\tNome");
        for (t; t->next != NULL; t = t->next)
            printf("%d\t|\t%d\t|\t%s",t->descriptor,t->modo,t->nome); // o ideal sería pasar o modo coma string, xa se verá
    }
}

void Cmd_open (char * tr[]) {
    int i, df, mode = 0;

    if (tr[0] == NULL) { /*no hay parametro*/
        listarAbertos();
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
        ...........AnadirAFicherosAbiertos(descriptor...
        modo...nombre....)....
        printf("Engadida entrada a taboa ficheiros abertos %s", );
    }
}
void Cmd_close (char *tr[])
    {
        int df;

        if (tr[0]==NULL || (df=atoi(tr[0]))<0) { /*no hay parametro*/
            ..............ListarFicherosAbiertos............... /*o el descriptor es menor que 0*/
            return;
        }


        if (close(df)==-1)
            perror("Inposible pechar descriptor");
        else
        ........EliminarDeFicherosAbiertos......
    }
}
void Cmd_dup (char * tr[])
    {
        int df, duplicado;
        char aux[MAXNAME],*p;

        if (tr[0]==NULL || (df=atoi(tr[0]))<0) { /*no hay parametro. */
            listarAbertos();
            return;
        }


        p=.....NombreFicheroDescriptor(df).......;
        sprintf (aux,"dup %d (%s)",df, p);
        .......AnadirAFicherosAbiertos......duplicado......aux.....fcntl(duplicado,F_GETFL).....;
    }
}