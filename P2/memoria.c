#include <time.h>
#include "memoria.h"

/*malloc allocates (or deallocates) a block malloc memory. Updates the list of memory blocks
shared allocates (or deallocates) a block shared memory. Updates the list of memory blocks
mmap maps (or unmaps) a file in memory. Updates the list of memory blocks
read reads a file into memory
write writes to a file the contents at a memory address
memdump dumps the contents of memory to the screen
memfill fills the memory with one character
mem shows information on the memory of the process
recurse executes a recursive function*/


listaBloques crearTaboaBloques() {
    return NULL;
}

void insertarBloqueEnTaboa(listaBloques *l, double tam, void *dir, time_t data, char *tipo) {
    listaBloques tAux, tCnt;
    tAux = malloc(sizeof(listaBloques));

    tAux->tipoAsignacion=tipo;

    tAux->dataCreacion=data;
    tAux->tamanoBloque=tam;
    tAux->direccion=dir;
    if(*l==NULL){
        tAux->next=NULL;
        *l = tAux;
    } else {
        for (tCnt = *l; tCnt->next != NULL; tCnt = tCnt->next);
        tCnt->next = tAux;
    }
}



void memAlloc(listaBloques l, char *argumentos[MAXARGS]) {
    char **strAux;
    double P;

    if (argumentos[0] == NULL) {
        if (l == NULL)
            printf("Non hai ningún bloque asignado no momento\n");
        else {
            printf("Lista de bloques asignados para o proceso %d\n", getpid());
            for (; l != NULL; l = l->next)
                printf("%p\t|\t%9f\t|\t%ld\t|\t%s\t%d/%d/%02d:%02d\n",
                       l->direccion, l->tamanoBloque, l->dataCreacion,
                       l->tipoAsignacion, localtime(&l->dataCreacion)->tm_mday,
                       localtime(&l->dataCreacion)->tm_mon + 1,
                       localtime(&l->dataCreacion)->tm_hour,
                       localtime(&l->dataCreacion)->tm_min);
        }
        return;
    }

    if (strcmp(argumentos[0], "-free") != 0) {
        char *fin;
        errno = 0;
        P = strtod(argumentos[0], &fin);

        if (errno != 0) {
            perror("Error en la conversión");
            return;
        }

        void *A = malloc(P);
        time_t tempo;
        tempo = time(NULL);

        if (A == NULL) {
            perror("Non hai memoria suficiente para realizar a asignacion");
            return;
        }

        insertarBloqueEnTaboa(&l, P, A, tempo, "malloc\0");

        printf("Insertado bloque de tamano %.0f bytes en 0x%x\n", P, A);
    } else if (l == NULL) {
        printf("Non hai ningún bloque asignado\n");
    } else {
        char *fin;
        errno = 0;
        P = strtod(argumentos[0], &fin);

        if (errno != 0) {
            perror("Error en la conversión");
            return;
        }

        for (; l != NULL; l = l->next)
            if (l->tamanoBloque == P) {
                return;
            }
    }

    if (l == NULL)
        printf("Non hai bloque dese tamano asignado con malloc\n");
}

void intRecurse (int n) {
    char automatico[TAMANO];
    static char estatico[TAMANO];

    printf ("parametro:%3d(%p) array %p, arr estatico %p\n",n,&n,automatico, estatico);

    if (n>0)
        intRecurse(n-1);
}

void recurse (char* argumentos[MAXARGS]){
    int n;
    char* strAux[1];

    n = strtol(argumentos[0],strAux,10);
    if(*strAux!=argumentos[0] && argumentos[0][0]!=0)
        intRecurse(n);
    //free(strAux[0]);
}