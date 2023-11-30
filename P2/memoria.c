#include <time.h>
#include "memoria.h"
#include "ficheiros.h"
#include "sys/mman.h"


#include <sys/ipc.h>

#include <sys/shm.h>

double globIni1 = 0, globIni2 = 0.5, globIni3 = 9123876.912836;
int glob1, glob2, glob3;

/*malloc allocates (or deallocates) a block malloc memory. Updates the list of memory blocks
shared allocates (or deallocates) a block shared memory. Updates the list of memory blocks
mmap maps (or unmaps) a file in memory. Updates the list of memory blocks
read reads a file into memory
write writes to a file the contents at a memory address
memdump dumps the contents of memory to the screen
memfill fills the memory with one character
mem shows information on the memory of the process
recurse executes a recursive function*/

//LIberamos el bloque de memoria reservado con el free así como lo eliminamos de la lista
void liberarBloqueMemoriaMalloc(listaBloques bloque) {


    free(bloque->direccion);
    free(bloque);
}
//Creamos la tabla para guardar los segmentos reservados de memoria
void crearTaboaBloques(listaBloques *lista){
    *lista=NULL;
}
//Segun nos pidan mostramos mallocs, memoria shared, mmap o todos a la vez
void MostrarListaMemoria(listaBloques lista,int Mode, bool FT) {

    if (FT)
        printf("Lista de bloques asignados para el proceso %d\n", getpid());
    if (lista == NULL) {
        return;
    } else {

        if (Mode == 0) {
            for (bloquesMemoria *l = lista; l != NULL; l = (bloquesMemoria *) l->next) {
                if (strcmp(l->tipoAsignacion, "malloc") == 0) {
                    if (l->key!=-1) {
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
                    if (l->key!=-1) {
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
                    if (l->key!=-1) {
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
            MostrarListaMemoria(lista, 0,false);
            MostrarListaMemoria(lista, 1,false);
            MostrarListaMemoria(lista, 2,false);

        }
    }
}

//INsertamos un bloque reservado en la lista
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


//COmprobamos si un bloque se ha asignado correctamente y obtenemos bloques creados por otros procesos a partir de su clave
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

//Hacemos el crear del shared, y llamamos a la función anterior para comprobar si ha sido creado correctamente
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


void do_AllocateCreateshared (char *tr[],listaBloques *l)
{
    key_t cl;
    size_t tam;
    void *p;

    if (tr[0]==NULL || tr[1]==NULL) {
        MostrarListaMemoria(*l,2,true);
        return;
    }

    cl=(key_t)  strtoul(tr[0],NULL,10);
    tam=(size_t) strtoul(tr[1],NULL,10);
    if (tam==0) {
        printf ("No se asignan bloques de 0 bytes\n");
        return;
    }
    if ((p=ObtenerMemoriaShmget(cl,tam,l))!=NULL)
        printf ("Asignados %lu bytes en %p\n",(unsigned long) tam, p);
    else
        printf ("Imposible asignar memoria compartida clave %lu:%s\n",(unsigned long) cl,strerror(errno));
}


//Eliminamos un documento de la lista
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

//mapeamos un fichero a memoria, y despois insertamos este na taboa
void * MapearFichero (char * fichero, int protection, listaBloques *l)
{
    int df, map=MAP_PRIVATE,modo=O_RDONLY;
    struct stat s;
    void *p;

    if (protection&PROT_WRITE)
        modo=O_RDWR;
    if (stat(fichero,&s)==-1 || (df=open(fichero, modo))==-1)
        return NULL;
    if ((p=mmap (NULL,s.st_size, protection,map,df,0))==MAP_FAILED)//Función que realmente fai o traballo
        return NULL;
    insertarElemento(l,p,s.st_size,"file",df,fichero);
    return p;
}


//transmforma os argumentos pasados a argumentos lexibles pola funcion anterior
void do_AllocateMmap(char *arg[],listaBloques *L)
{
    char *perm;
    void *p;
    int protection=0;

    if (arg[0]==NULL){
        MostrarListaMemoria(*L,2,true);
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
//Mapea ficheros en memoria
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

//Usada para desmapear unah clave
void eliminarClave2(int clave, listaBloques *lista) {
    bloquesMemoria *l = *lista;
    bool Hechoalgo=false;
    bloquesMemoria *anterior = NULL;

    while (l != NULL && l->key != clave) {
        anterior = l;
        l = (bloquesMemoria *) l->next;
    }

    if (l == NULL) {
        printf("No se encuentra la clave en la lista.\n");
        return;
    }


    l->key= -1;

}
//LIbera clave e elimina da lista, usada cando cerramos o programa
void eliminarClave(int clave, listaBloques *lista) {
    bloquesMemoria *l = *lista;
    bloquesMemoria *anterior = NULL;

    while (l != NULL && l->key != clave) {
        anterior = l;
        l =(bloquesMemoria *) l->next;
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
        *lista = (bloquesMemoria *) l->next;
    } else {
        anterior->next = l->next;
    }

    free(l);
}





//FUnción encargada de cerrar todo cando saimos, non era necesaria,pero fai que de ningunha meneira teñamos memory leaks
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

//Crea, desmapea, elimina memoria compartida seun o qwue lle mandemos
void sharedMemory ( char *argumentos[MAXARGS],listaBloques *lista){
    key_t cl;
    if (argumentos[0] ==NULL) { //Se non hay argumentos, mostro a lista
        if (*lista == NULL) {
            printf("Non hai ningún bloque asignado no momento\n");
        } else {

            MostrarListaMemoria(*lista,1,true);

        }
        return;
    }
    if (strcmp(argumentos[0],"-create")==0){
        if(argumentos[1]!=NULL && argumentos[2]!=NULL) {
            SharedCreate(argumentos, lista);
        }else MostrarListaMemoria(*lista,1,true);//Se non me poñen os 2 tamaños, non imvento, mostro a lista
        return;
    } else {
        if (strcmp(argumentos[0], "-free") == 0) {
            if (argumentos[1] != NULL) {

                cl = (key_t) strtoul(argumentos[1], NULL, 10);
                eliminarClave2(cl, lista);//Desmapeo
            } else printf("No se ha introducido una clave para desmapear");
            return;

        } else {
            if (strcmp(argumentos[0], "-delkey") == 0) {
                if (argumentos[1] != NULL) {
                    cl = (key_t) strtoul(argumentos[1], NULL, 10);
                    desmapearSegmento(cl, lista);//Elimino a clave
                } else printf("No se ha introducido una clave para eliminar");

                return;
            } else {
                cl = atoi(argumentos[0]);
                ObtenerMemoriaShmget( cl, 0, lista);
            }
        }
    }
}

//Asigno, ou libero memoria segundo me pidan
void memAlloc(listaBloques *lista, char *argumentos[MAXARGS]) {
    if (argumentos[0] == NULL) {//Se non hai argumentos, mostro a lista, se hai
        if (*lista == NULL) {
            printf("Non hai ningún bloque asignado no momento\n");
        } else {
                MostrarListaMemoria(*lista, 0,true);
        }
        return;
    }

    int n;
    char *strAux = NULL;

    if (strcmp(argumentos[0], "-free") != 0) {
        //Se non é un free, comprobo que o tamaño é un numero maior que 0 e fago o malloc
        n = strtol(argumentos[0], &strAux, 10);

        if (n <= 0 || *strAux != '\0' && *strAux != '\n') {
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
        //Se é un free, comprobo que introduciron un tamaño adecuado e libero 
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
//Busco o bloque se este existe, libero, se non non podo facer nada
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
//MOstro as direccións das variable local automatico e da estática estatico
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

//Comando para leer ficheiros
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
        //función que fai realmente o traballo, o resto son comprobacións
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

void CmdRead (char *ar[]){
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
void memfill(char *argumentos[]){
    if(argumentos[2] != NULL)
        LlenarMemoria(cadtop(argumentos[0]),
                      strtol(argumentos[1],NULL,10),
                      (char) strtol(argumentos[2],NULL,10));
    else
        perror("Numero de argumentos non valido");
}

void Do_MemPmap (void) /*sin argumentos*/
{ pid_t pid;       /*hace el pmap (o equivalente) del proceso actual*/
    char elpid[32];
    char *argv[4]={"pmap",elpid,NULL};

    sprintf (elpid,"%d", (int) getpid());
    if ((pid=fork())==-1){
        perror ("Imposible crear proceso");
        return;
    }
    if (pid==0){ /*proceso hijo*/
        if (execvp(argv[0],argv)==-1)
            perror("cannot execute pmap (linux, solaris)");

        argv[0]="vmmap"; argv[1]="-interleave"; argv[2]=elpid;argv[3]=NULL;
        if (execvp(argv[0],argv)==-1) /*probamos vmmap Mac-OS*/
            perror("cannot execute vmmap (Mac-OS)");

        argv[0]="procstat"; argv[1]="vm"; argv[2]=elpid; argv[3]=NULL;
        if (execvp(argv[0],argv)==-1)/*No hay pmap, probamos procstat FreeBSD */
            perror("cannot execute procstat (FreeBSD)");

        argv[0]="procmap",argv[1]=elpid;argv[2]=NULL;
        if (execvp(argv[0],argv)==-1)  /*probamos procmap OpenBSD*/
            perror("cannot execute procmap (OpenBSD)");

        exit(1);
    }
    waitpid (pid,NULL,0);
}
//MSegundo me pida, mositrarei as direccións de 3 variables locales, globales, estáticas, funcións, fundions de programa e libería, bloques de memoria reservados, ou unha representación do espazo de memoria virtual do proceso con pmap
void mem(char *argumentos[], listaBloques l){
    long loc1, loc2, loc3;
    static float st1, st2, st3;
    static char stIni1='a', stIni2='Z', stIni3=0;
    if(argumentos[0]==NULL || strcmp(argumentos[0],"-all")==0) {
        printf("Variables locais: %p, %p, %p\n",&loc1, &loc2, &loc3);
        printf("Variables globais: %p, %p, %p\n", &globIni1, &globIni2, &globIni3);
        printf("Variables estaticas: %p, %p, %p\n", &stIni1, &stIni2, &stIni3);
        printf("Variables globais non inicializadas: %p, %p, %p\n", &glob1, &glob2, &glob3);
        printf("Variables estaticas non inicializadas: %p, %p, %p\n", &st1, &st2, &st3);
        printf("Funcions programa: %p, %p, %p\n",memfill,cadtop,recurse);
        printf("Funcions libraria: %p, %p, %p\n",printf,time,strtol);
        MostrarListaMemoria(l, 3, true);
    }else if(strcmp(argumentos[0],"-blocks")==0)
        MostrarListaMemoria(l,3, true);
    else if(strcmp(argumentos[0],"-funcs")==0){
        printf("Funcions programa: %p, %p, %p\n",memfill,cadtop,recurse);
        printf("Funcions libraria: %p, %p, %p\n",printf,time,strtol);
    }
    else if(strcmp(argumentos[0],"-vars")==0) {
        printf("Variables locais: %p, %p, %p\n",&loc1, &loc2, &loc3);
        printf("Variables globais: %p, %p, %p\n", &globIni1, &globIni2, &globIni3);
        printf("Variables estaticas: %p, %p, %p\n", &stIni1, &stIni2, &stIni3);
        printf("Variables globais non inicializadas: %p, %p, %p\n", &glob1, &glob2, &glob3);
        printf("Variables estaticas non inicializadas: %p, %p, %p\n", &st1, &st2, &st3);
    }
    else if(strcmp(argumentos[0],"-pmap")==0)
        Do_MemPmap();
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

//Escribo nun ficheiro os argumentos que me pasen 
void CmdWrite(char *ar[]){ //falta probalo
    if(ar[3]==NULL && ar[2]!=NULL)
        EscribirFichero(ar[0], cadtop(ar[1]),strtol(ar[2],NULL,10),0);
    else if(strcmp(ar[0],"-o")==0 && ar[3]!=NULL)
        EscribirFichero(ar[1], cadtop(ar[2]),strtol(ar[3],NULL,10),1);
    else
        perror("Argumentos invalidos");
}

int min(int a, int b){ if(a<b) return a; else return b; }


//Mostramos por pantalla os contidos de certa dicerrción de memoria, se non lle pasamos ata que contido, facemolo ata un valor por defecto
void CmdMemdump(char* ar[]){
    void *dir = cadtop(ar[0]);
    int i, j, tam;

    if(ar[1]==NULL) tam = POR_DEFECTO;
    else tam = strtol(ar[1],NULL,10);
    for(i=0; i<tam; i+=20) {
        for (j = i; j < min(i + 20, tam); j++)
            printf("%02X ",*(char *)(dir+j));
        printf("\n");
        for (j = i; j < min(i + 20, tam); j++)
            printf("%2c ",*(char *)(dir+j));
        printf("\n");
    }
}
