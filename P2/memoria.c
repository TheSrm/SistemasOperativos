#include <time.h>
#include "memoria.h"
#include "ficheiros.h"
#include "sys/mman.h"


#include <sys/ipc.h>

#include <sys/shm.h>


/*malloc allocates (or deallocates) a block malloc memory. Updates the list of memory blocks
shared allocates (or deallocates) a block shared memory. Updates the list of memory blocks
mmap maps (or unmaps) a file in memory. Updates the list of memory blocks
read reads a file into memory
write writes to a file the contents at a memory address
memdump dumps the contents of memory to the screen
memfill fills the memory with one character
mem shows information on the memory of the process
recurse executes a recursive function*/


void liberarBloqueMemoriaMalloc(listaBloques bloque) {

    free(bloque->nombreDocumento);
    free(bloque->direccion);
    free(bloque);
}

void crearTaboaBloques(listaBloques *lista){
    *lista=NULL;
}

void MostrarListaMemoria(listaBloques lista,int Mode) {

    if (lista == NULL) {
        printf("No se han asignado memoria en el proceso en cuestion\n");
        return;
    } else {
        printf("Lista de bloques asignados para el proceso %d\n", getpid());
        if (Mode == 0) {
            for (bloquesMemoria *l = lista; l != NULL; l = (bloquesMemoria *) l->next) {
                if (strcmp(l->tipoAsignacion, "malloc") == 0) {
                    if (l->dataCreacion!=(time_t) NULL) {
                        printf("%p \t %d/%d/%d %02d:%02d  %lu %s\n",
                               l->direccion, localtime(&l->dataCreacion)->tm_mday,
                               localtime(&l->dataCreacion)->tm_mon, localtime(&l->dataCreacion)->tm_year + 1900,
                               localtime(&l->dataCreacion)->tm_hour,
                               localtime(&l->dataCreacion)->tm_min, l->tamanoBloque, l->tipoAsignacion);
                    }
                }
            }
        }

        if (Mode == 1) {
            for (bloquesMemoria *l = lista; l != NULL; l = (bloquesMemoria *) l->next) {
                if (strcmp(l->tipoAsignacion, "shared") == 0) {
                    if (l->dataCreacion!=(time_t) NULL) {
                        printf("%p \t %d/%d/%d %02d:%02d  %lu %s ( key %d)\n",
                               l->direccion, localtime(&l->dataCreacion)->tm_mday,
                               localtime(&l->dataCreacion)->tm_mon, localtime(&l->dataCreacion)->tm_year + 1900,
                               localtime(&l->dataCreacion)->tm_hour,
                               localtime(&l->dataCreacion)->tm_min, l->tamanoBloque, l->tipoAsignacion, l->key);
                    }
                }
            }
        }

        if (Mode == 2) {
            for (bloquesMemoria *l = lista; l != NULL; l = (bloquesMemoria *) l->next) {
                if (strcmp(l->tipoAsignacion, "file") == 0) {
                    if (l->dataCreacion!=(time_t) NULL) {
                        printf("%p \t %d/%d/%d %02d:%02d  %lu %s ( descriptor %d)\n",
                               l->direccion, localtime(&l->dataCreacion)->tm_mday,
                               localtime(&l->dataCreacion)->tm_mon, localtime(&l->dataCreacion)->tm_year + 1900,
                               localtime(&l->dataCreacion)->tm_hour,
                               localtime(&l->dataCreacion)->tm_min, l->tamanoBloque, l->nombreDocumento, l->key);
                    }
                }
            }
        }

        if (Mode == 3) {
            MostrarListaMemoria(lista, 0);
            MostrarListaMemoria(lista, 1);
            MostrarListaMemoria(lista, 2);

        }
    }
}


void insertarElemento(listaBloques *lista, void *direccion, long tamanoBloque, const char *tipoAsignacion, key_t key, char* nombre) {
    // Crear un nuevo bloque de memoria
    bloquesMemoria *nuevoBloque = (bloquesMemoria *) malloc(sizeof(bloquesMemoria));


    if (nuevoBloque == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el nuevo bloque.\n");
        return;
    }

    // Configurar los valores del nuevo bloque
    nuevoBloque->direccion = direccion;
    nuevoBloque->dataCreacion = time(NULL);
    nuevoBloque->tamanoBloque = tamanoBloque;
    nuevoBloque->tipoAsignacion = tipoAsignacion;
    nuevoBloque->nombreDocumento=NULL;
    if (nombre != NULL) {
        nuevoBloque->nombreDocumento = strdup(nombre);
        if (nuevoBloque->nombreDocumento == NULL) {
            perror("Error al duplicar la cadena");
            free(nuevoBloque);
            return;
        }
    }


    nuevoBloque->next = NULL;

    if (strcmp(tipoAsignacion,"shared")!=0) {
        nuevoBloque->key = key;


    }else if ( key == 0) {
            printf("Non asignamos bloques de clave 0 ");
            return;
        }else {nuevoBloque->key = key;
        nuevoBloque->nombreDocumento = "shared";}


        bloquesMemoria *temp2 = *lista;
        // Si la lista está vacía, insertar al principio
        if (*lista == NULL) {
            *lista = nuevoBloque;
        } else {
            // Encontrar el final de la lista
            for (; temp2->next != NULL; temp2 = (bloquesMemoria *) temp2->next);
            // Enlazar el nuevo bloque al final de la lista
            temp2->next = (struct bloqueMemoria *) nuevoBloque;
        }

}



void * ObtenerMemoriaShmget (key_t clave, size_t tam,listaBloques *L)
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
    insertarElemento(L,p,s.shm_segsz,"shared",clave,"shared");
    /** Guardar en la lista, p.e.  InsertarNodoShared (&L, p, s.shm_segsz, clave); */
    return (p);
}


void SharedCreate (char *tr[],listaBloques *l){
    key_t cl;
    size_t tam;
    void *p;



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


void do_AllocateCreateshared (char *tr[],listaBloques *L)
{
    key_t cl;
    size_t tam;
    void *p;

    if (tr[0]==NULL || tr[1]==NULL) {
        MostrarListaMemoria(*L,2);
        return;
    }


    cl=(key_t)  strtoul(tr[0],NULL,10);
    tam=(size_t) strtoul(tr[1],NULL,10);
    if (tam==0) {
        printf ("No se asignan bloques de 0 bytes\n");
        return;
    }
    if ((p=ObtenerMemoriaShmget(cl,tam,L))!=NULL)
        printf ("Asignados %lu bytes en %p\n",(unsigned long) tam, p);
    else
        printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
}

void eliminarDocumento(int key, listaBloques *lista) {
    bloquesMemoria *actual = *lista;
    bloquesMemoria *anterior = NULL;

    // Buscar el nodo con el descriptor de archivo key
    while (actual != NULL && actual->key != key) {
        anterior = actual;
        actual = (bloquesMemoria *) actual->next;
    }

    if (actual == NULL) {
        fprintf(stderr, "No se encontró el documento en la lista.\n");
        return;
    }

    // Cerrar el archivo
    if (close(actual->key) == -1) {
        perror("Error al cerrar el archivo");
        return;
    }

    // Liberar memoria del campo nombreDocumento
    free(actual->nombreDocumento);

    // Eliminar el nodo de la lista
    if (anterior == NULL) {
        *lista = (listaBloques) actual->next;
    } else {
        anterior->next = actual->next;
    }


    // Liberar memoria del nodo actual
    free(actual);
}

void * MapearFichero (char * fichero, int protection, listaBloques *l)
{
    int df, map=MAP_PRIVATE,modo=O_RDONLY;
    struct stat s;
    void *p;

    if (protection&PROT_WRITE)
        modo=O_RDWR;
    if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
        return NULL;
    if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED)
        return NULL;
    insertarElemento(l,p,s.st_size,"file",df,fichero);
    return p;
}



void do_AllocateMmap(char *arg[],listaBloques *L)
{
    char *perm;
    void *p;
    int protection=0;

    if (arg[0]==NULL){
        MostrarListaMemoria(*L,2);
        return;
    }

    if ((perm=arg[1])!=NULL && strlen(perm)<4) {
        if (strchr(perm,'r')!=NULL) protection|=PROT_READ;
        if (strchr(perm,'w')!=NULL) protection|=PROT_WRITE;
        if (strchr(perm,'x')!=NULL) protection|=PROT_EXEC;
    }
    if ((p=MapearFichero(arg[0],protection,L))==NULL)
        perror ("Imposible mapear fichero");
    else
        printf ("fichero %s mapeado en %p\n", arg[0], p);
}

void desmapearSegmento(int clave, listaBloques *lista) {
    listaBloques actual = *lista;

    // Recorrer la lista para encontrar el nodo con la clave
    while (actual != NULL) {
        if (actual->key == clave) {
            // Obtener el identificador del segmento de memoria compartida
            int shmid = shmget((key_t) actual->key, 0, 0);
            if (shmid == -1) {
                perror("Error al obtener el identificador del segmento de memoria compartida");
                exit(EXIT_FAILURE);
            }

            // Desmapear el segmento de memoria compartida
            if (shmdt(actual->direccion) == -1) {
                perror("Error al desmapear el segmento de memoria compartida");
                exit(EXIT_FAILURE);
            }
            free(actual->nombreDocumento);
            printf("Segmento de memoria compartida con clave %d desmapeado con éxito.\n", actual->key);


            return;  // Salir del bucle después de desmapear el segmento
        }

        actual = (listaBloques) actual->next;

    }
//Si no salí en el bucle anterior, no encontré la clave
    printf("Clave %d no encontrada en la lista.\n", clave);
}

void MemoryMap (char* argumentos[MAXARGS],listaBloques *l){
    if (argumentos[0]==NULL || strcmp(argumentos[0],"-free")!=0){
        do_AllocateMmap(argumentos,l);
        return;
    }
    if (strcmp(argumentos[0],"-free")==0){
        bloquesMemoria *L;
        for ( L = *l; l != NULL; L = (bloquesMemoria *) L->next) {
            if (strcmp(L->nombreDocumento,argumentos[1])==0)
                break;
        }
        if (L==NULL){
            printf("No se encuentra ese bloque en la lista");
            return;
        }
        else
            desmapearSegmento(L->key,l);
        return;
    }
    printf("No has introducido una opción válida, usa help mmap para ver las opciones disponibles.\n");


        return;
}

void eliminarClave2(int clave, listaBloques *lista) {
    bloquesMemoria *l;
    bool Hechoalgo=false;

    for (l = (bloquesMemoria *) lista; l != NULL ||  l->key!=clave; l = (bloquesMemoria *) l->next);


    if(l==NULL){
        printf("No se encuentra la clave en la lista ");
        return;
    }

    l->dataCreacion= (time_t) NULL;

}

void eliminarClave(int clave, listaBloques *lista) {
    bloquesMemoria *l = *lista;
    bloquesMemoria *anterior = NULL;

    while (l != NULL && l->key != clave) {
        anterior = l;
        l = l->next;
    }

    if (l == NULL) {
        printf("No se encuentra la clave en la lista.\n");
        return;
    }

    // Obtener el identificador del segmento de memoria compartida
    int shmid = shmget((key_t) l->key, l->tamanoBloque, 0);
    if (shmid == -1) {
        perror("Error al obtener el identificador del segmento de memoria compartida");
        exit(EXIT_FAILURE);
    }

    // Eliminar el segmento de memoria compartida
    if (shmctl(shmid, IPC_RMID, NULL) == -1) {
        perror("Error al eliminar el segmento de memoria compartida");
        exit(EXIT_FAILURE);
    }

    // Liberar la memoria del nodo actual
    if (anterior == NULL) {
        *lista = l->next;
    } else {
        anterior->next = l->next;
    }

    free(l);
}






void pecharTodoBloque(listaBloques *lista) {
    bloquesMemoria *actual = *lista;
    bloquesMemoria *siguiente;

    while (actual != NULL) {
        siguiente = (bloquesMemoria *) actual->next;
        if (strcmp(actual->tipoAsignacion, "malloc") == 0)
            liberarBloqueMemoriaMalloc(actual);
        else if (strcmp(actual->tipoAsignacion, "shared") == 0) {
            eliminarClave(actual->key, lista);
        }else if (strcmp(actual->tipoAsignacion, "file") == 0) {
            eliminarDocumento(actual->key,lista);
        }

        actual = siguiente;
    }
}


void sharedMemory ( char *argumentos[MAXARGS],listaBloques *lista){
    key_t cl;
    char* key = argumentos[1];
    if (argumentos[0] ==NULL) {
        if (*lista == NULL) {
            printf("Non hai ningún bloque asignado no momento\n");
        } else {

            MostrarListaMemoria(*lista,1);

        }
        return;
    }
    if (strcmp(argumentos[0],"-create")==0){
        SharedCreate(argumentos,lista);
        return;
    } else {
        if (strcmp(argumentos[0], "-free") == 0) {
            if (argumentos[1] != NULL) {

                cl = (key_t) strtoul(argumentos[1], NULL, 10);
                eliminarClave2(cl, lista);
            } else printf("No se ha introducido una clave para desmapear");
            return;

        } else {
            if (strcmp(argumentos[0], "-delkey") == 0) {
                if (argumentos[1] != NULL) {
                    cl = (key_t) strtoul(argumentos[1], NULL, 10);
                    desmapearSegmento(cl, lista);
                } else printf("No se ha introducido una clave para eliminar");

                return;
            }
            }
        }
    printf("No se ha inyroducido una opcion valida, consulta la ayuda con help shared\n");
}


void memAlloc(listaBloques *lista, char *argumentos[MAXARGS]) {
    if (argumentos[0] == NULL) {
        if (*lista == NULL) {
            printf("Non hai ningún bloque asignado no momento\n");
        } else {
                MostrarListaMemoria(*lista, 0);
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
            printf("No se ha podido realizar la reserva de memoria");
        }

        insertarElemento(lista, A, n, "malloc", 0,"malloc");
        printf("Se ha asignado memoria correspondiente a %d bytes en %p\n", n, A);
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
                    *lista = (listaBloques) l->next;
                } else {
                    anterior->next = l->next;
                }

                // Imprimir y liberar memoria
                printf("Bloque liberado: %p\n", l->direccion);
                liberarBloqueMemoriaMalloc(l);

                return;
            } else {
                anterior = l;
                l = (bloquesMemoria *) l->next;
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

ssize_t LeerFichero (char *f, void *p, size_t cont)
{
    struct stat s;
    ssize_t  n;
    int df,aux;

    if (stat (f,&s)==-1 || (df=open(f,O_RDONLY))==-1)
        return -1;
    if (cont==-1)   /* si pasamos -1 como bytes a leer lo leemos entero*/
        cont=s.st_size;
    if ((n=read(df,p,cont))==-1){
        aux=errno;
        close(df);
        errno=aux;
        return -1;
    }
    close (df);
    return n;
}

void *cadtop(char *cad){
    return (void *) strtol(cad, NULL, 16);
}

void CmdRead (char *ar[]){ // comprobar se furrula ben
    void *p;
    size_t cont=-1;  /* -1 indica leer todo el fichero*/
    ssize_t n;
    if (ar[0]==NULL || ar[1]==NULL){
        printf ("faltan parametros\n");
        return;
    }
    p=cadtop(ar[1]);  /*convertimos de cadena a puntero*/
    if (ar[2]!=NULL)
        cont=(size_t) atoll(ar[2]);

    if ((n=LeerFichero(ar[0],p,cont))==-1)
        perror ("Imposible leer fichero");
    else
        printf ("leidos %lld bytes de %s en %p\n",(long long) n,ar[0],p);
}


void LlenarMemoria (void *p, size_t cont, unsigned char byte){
    unsigned char *arr=(unsigned char *) p;
    size_t i;

    for (i=0; i<cont;i++)
        arr[i]=byte;
}
void memfill(char *argumentos[]){ // comprobar se furrula ben
    if(argumentos[2] != NULL && argumentos[3] == NULL)
        LlenarMemoria(cadtop(argumentos[0]),
                      strtol(argumentos[1],NULL,10),
                      argumentos[2][0]);
    else
        perror("Numero de argumentos non valido");
}

void mem(listaBloques l){ //comprobar se furrula
    MostrarListaMemoria(l,3);
}



ssize_t EscribirFichero (char *f, void *p, size_t cont,int overwrite)
{
    ssize_t  n;
    int df,aux, flags=O_CREAT | O_EXCL | O_WRONLY;

    if (overwrite)
        flags=O_CREAT | O_WRONLY | O_TRUNC;

    if ((df=open(f,flags,0777))==-1)
        return -1;

    if ((n=write(df,p,cont))==-1){
        aux=errno;
        close(df);
        errno=aux;
        return -1;
    }
    close (df);
    return n;
}

void CmdWrite(char *ar[]){ //falta probalo
    if(ar[3]==NULL && ar[2]!=NULL)
        EscribirFichero(ar[0], cadtop(ar[1]),strtol(ar[2],NULL,10),0);
    else if(strcmp(ar[0],"-o")==0 && ar[3]!=NULL &&ar[4]==NULL)
        EscribirFichero(ar[1], cadtop(ar[2]),strtol(ar[3],NULL,10),1);
    else
        perror("Argumentos invalidos");
}
