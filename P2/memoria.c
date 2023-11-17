#include <time.h>
#include <float.h>
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

void pecharTodoBloque(listaBloques *l){
    listaBloques tAux;
    void *df;



    if(*l != NULL)
        for (tAux = *l; tAux->next != NULL; tAux = (listaBloques) tAux->next) {
            df = tAux->direccion;
            free(df);
            char cadena[20];
            sprintf(cadena, "%f", tAux->tamanoBloque);
            char *args[2]={"free",cadena};
            memAlloc(l,args);

        }
}

void crearTaboaBloques(listaBloques *lista){
    *lista=NULL;
}





void insertarElemento(listaBloques *lista, void *direccion, double tamanoBloque, const char *tipoAsignacion) {
    // Crear un nuevo bloque de memoria
    bloquesMemoria *nuevoBloque = (bloquesMemoria *)malloc(sizeof(bloquesMemoria));

    if (nuevoBloque == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el nuevo bloque.\n");
        exit(EXIT_FAILURE);
    }

    // Configurar los valores del nuevo bloque
    nuevoBloque->direccion = direccion;
    nuevoBloque->dataCreacion = time(NULL);
    nuevoBloque->tamanoBloque = tamanoBloque;
   nuevoBloque->tipoAsignacion=tipoAsignacion;

    nuevoBloque->next = NULL;

    bloquesMemoria *temp = *lista;
    // Si la lista está vacía, insertar al principio
    if (*lista == NULL) {
        *lista = nuevoBloque;
    } else {
        // Encontrar el final de la lista
        for (; temp->next != NULL; temp=temp->next);
        // Enlazar el nuevo bloque al final de la lista
        temp->next = nuevoBloque;
    }
}

void memAlloc(listaBloques *lista, char *argumentos[MAXARGS]) {
    if (argumentos[0] == NULL) {
        if (*lista == NULL) {
            printf("Non hai ningún bloque asignado no momento\n");
        } else {
            printf("Lista de bloques asignados para o proceso %d\n", getpid());
            for (bloquesMemoria *l = *lista; l != NULL; l = (bloquesMemoria *) l->next) {
                       printf("0x%x \t %d/%d/ %02d:%02d  %.0f\n",
                              l->direccion,localtime(&l->dataCreacion)->tm_mday,
                              localtime(&l->dataCreacion)->tm_mon,localtime(&l->dataCreacion)->tm_hour,
                              localtime(&l->dataCreacion)->tm_min,l->tamanoBloque);
            }
        }
        return;
    }

    int n;
    char *strAux = NULL;

    if (strcmp(argumentos[0], "-free") != 0) {
        n = strtol(argumentos[0], &strAux, 10);

        if (n < 0 || *strAux != '\0' && *strAux != '\n') {
            perror("Non se introduciu un tamaño adecuado\n");
            return;
        }

        void *A = malloc(n);

        if (A == NULL) {
        }
        insertarElemento(lista, A, n, "malloc");
        printf("Se ha asignado memoria correspondiente a %d bytes en 0x%x\n", n, A);
    } else {
        if (argumentos[1] == NULL) {
            printf("Error: Debe proporcionar el tamaño a liberar.\n");
            return;
        }

        n = strtol(argumentos[1], &strAux, 10);

        if (n < 0 || *strAux != '\0' && *strAux != '\n') {
            perror("Non se introduciu un tamaño adecuado\n");
            return;
        }

        bloquesMemoria *l = *lista;
        bloquesMemoria *anterior = NULL;

        while (l != NULL) {
            if (l->tamanoBloque == n) {
                // Eliminar el bloque encontrado
                if (anterior == NULL) {
                    *lista = l->next;
                } else {
                    anterior->next = l->next;
                }

                // Imprimir y liberar memoria
                printf("Bloque liberado: 0x%x\n", l->direccion);
                free(l);

                return;
            } else {
                anterior = l;
                l = l->next;
            }
        }

        printf("Non hai bloque dese tamano asignado con malloc\n");
    }
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