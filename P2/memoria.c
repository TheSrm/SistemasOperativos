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


void liberarBloqueMemoria(listaBloques bloque) {
    free(bloque->direccion);
    // Otros pasos de liberación si es necesario
    free(bloque);
}


void pecharTodoBloque(listaBloques *lista) {
     bloquesMemoria *actual = *lista;
    bloquesMemoria *siguiente;

    while (actual != NULL) {
        siguiente = (bloquesMemoria *) actual->next;
        liberarBloqueMemoria(actual);
        actual = siguiente;
    }
}
void crearTaboaBloques(listaBloques *lista){
    *lista=NULL;
}

void MostrarListaMemoria(listaBloques lista,int Mode){
    if (Mode==0) {
        for (bloquesMemoria *l = lista; l != NULL; l = (bloquesMemoria *) l->next) {
            if (strcmp(l->tipoAsignacion, "malloc") == 0) {
                printf("%p \t %d/%d %02d:%02d  %lu %s\n",
                       l->direccion, localtime(&l->dataCreacion)->tm_mday,
                       localtime(&l->dataCreacion)->tm_mon, localtime(&l->dataCreacion)->tm_hour,
                       localtime(&l->dataCreacion)->tm_min, l->tamanoBloque, l->tipoAsignacion);
            }
        }
    }

    if (Mode==1) {
        for (bloquesMemoria *l = lista; l != NULL; l = (bloquesMemoria *) l->next) {
            if (strcmp(l->tipoAsignacion, "shared") == 0) {
                printf("%p \t %d/%d/ %02d:%02d  %lu %s ( key %d)\n",
                       l->direccion, localtime(&l->dataCreacion)->tm_mday,
                       localtime(&l->dataCreacion)->tm_mon, localtime(&l->dataCreacion)->tm_hour,
                       localtime(&l->dataCreacion)->tm_min, l->tamanoBloque, l->tipoAsignacion, l->key);
            }
        }
    }

    if (Mode==2) {
        for (bloquesMemoria *l = lista; l != NULL; l = (bloquesMemoria *) l->next) {
            if (strcmp(l->tipoAsignacion, "shared") == 0) {
                printf("%p \t %d/%d/ %02d:%02d  %lu %s ( key %d)\n",
                       l->direccion, localtime(&l->dataCreacion)->tm_mday,
                       localtime(&l->dataCreacion)->tm_mon, localtime(&l->dataCreacion)->tm_hour,
                       localtime(&l->dataCreacion)->tm_min, l->tamanoBloque, l->tipoAsignacion, l->key);
            }
            if (strcmp(l->tipoAsignacion, "malloc") == 0)
                printf("%p \t %d/%d/ %02d:%02d  %lu %s\n",
                       l->direccion, localtime(&l->dataCreacion)->tm_mday,
                       localtime(&l->dataCreacion)->tm_mon, localtime(&l->dataCreacion)->tm_hour,
                       localtime(&l->dataCreacion)->tm_min, l->tamanoBloque, l->tipoAsignacion);

        }
    }
}






void insertarElemento(listaBloques *lista, void *direccion, long tamanoBloque, const char *tipoAsignacion, key_t key) {
    // Crear un nuevo bloque de memoria
    bloquesMemoria *nuevoBloque = (bloquesMemoria *) malloc(sizeof(bloquesMemoria));

    if (nuevoBloque == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el nuevo bloque.\n");
        exit(EXIT_FAILURE);
    }

    // Configurar los valores del nuevo bloque
    nuevoBloque->direccion = direccion;
    nuevoBloque->dataCreacion = time(NULL);
    nuevoBloque->tamanoBloque = tamanoBloque;
    nuevoBloque->tipoAsignacion = tipoAsignacion;

    nuevoBloque->next = NULL;

    if (strcmp(tipoAsignacion, "malloc") == 0) {
        nuevoBloque->key = 0;
    } else {
        if (key == 0) {
            printf("Non asignamos bloques de clave 0 ");
        } else nuevoBloque->key = key;
    }
        bloquesMemoria *temp = *lista;
        // Si la lista está vacía, insertar al principio
        if (*lista == NULL) {
            *lista = nuevoBloque;
        } else {
            // Encontrar el final de la lista
            for (; temp->next != NULL; temp = temp->next);
            // Enlazar el nuevo bloque al final de la lista
            temp->next = nuevoBloque;
        }

}


void * ObtenerMemoriaShmget (key_t clave, size_t tam,listaBloques L)
{
    void * p;
    int aux,id,flags=0777;
    struct shmid_ds s;

    if (tam)     /*tam distito de 0 indica crear */
        flags=flags | IPC_CREAT | IPC_EXCL;
    if (clave==IPC_PRIVATE)  /*no nos vale*/
    {errno=EINVAL; return NULL;}
    if ((id=shmget(clave, tam, flags))==-1)
        return (NULL);
    if ((p=shmat(id,NULL,0))==(void*) -1){
        aux=errno;
        if (tam)
            shmctl(id,IPC_RMID,NULL);
        errno=aux;
        return (NULL);
    }
    shmctl (id,IPC_STAT,&s);
    insertarElemento(&L,p,s.shm_segsz,"shared",clave);
    /** Guardar en la lista, p.e.  InsertarNodoShared (&L, p, s.shm_segsz, clave); */
    return (p);
}


void SharedCreate (char *tr[],listaBloques *l){
    key_t cl;
    size_t tam;
    void *p;

    if (tr[1]==NULL || tr[2]==NULL) {
        MostrarListaMemoria(l,1);
        return;
    }

    cl=(key_t)  strtoul(tr[1],NULL,10);
    tam=(size_t) strtoul(tr[2],NULL,10);
    if (tam==0) {
        printf ("No se asignan bloques de 0 bytes\n");
        return;
    }
    if ((p=ObtenerMemoriaShmget(cl,tam,l))!=NULL)
        printf ("Asignados %lu bytes en %p\n",(unsigned long) tam, p);
    else
        printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
}

void sharedMem (listaBloques *lista , char *argumentos[MAXARGS]){

    if (argumentos[0] == NULL) {
        if (*lista == NULL) {
            printf("Non hai ningún bloque asignado no momento\n");
        } else {
            printf("Lista de bloques asignados para o proceso %d\n", getpid());
            MostrarListaMemoria(*lista,1);

        }
        return;
    }

    if (strcmp(argumentos[0],"-create")==0){
        SharedCreate(argumentos,lista);
    } else if (strcmp(argumentos[0],"-")==0){

    }


}

void memAlloc(listaBloques *lista, char *argumentos[MAXARGS]) {
    if (argumentos[0] == NULL) {
        if (*lista == NULL) {
            printf("Non hai ningún bloque asignado no momento\n");
        } else {
            printf("Lista de bloques asignados para o proceso %d\n", getpid());
            for (bloquesMemoria *l = *lista; l != NULL; l = (bloquesMemoria *) l->next) {
                MostrarListaMemoria(*lista,0);
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
                printf("Bloque liberado: %p\n", l->direccion);
                liberarBloqueMemoria(l);

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