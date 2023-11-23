#ifndef MEMORIA_H
#define MEMORIA_H

#include "definicionsBase.h"
#include "procesos.h"
#include <sys/ipc.h>
#include "ficheiros.h"
#include "sys/mman.h"
#include <sys/shm.h>
#include <sys/wait.h>

#define TAMANO 2048
#define POR_DEFECTO 25

typedef u_long tam_t;
typedef int key_t;

typedef struct bloquesMemoria{
    void *direccion;
    time_t dataCreacion;
    char* nombreDocumento;
    tam_t tamanoBloque;
    char* tipoAsignacion;
    struct bloqueMemoria *next;
    int key;
}bloquesMemoria;

typedef bloquesMemoria *listaBloques;

void crearTaboaBloques(listaBloques *l);

void pecharTodoBloque(listaBloques *l);
void memAlloc( listaBloques *l, char *argumentos[MAXARGS]);
void sharedMemory(char *argumentos[MAXARGS], listaBloques *l);
void MemoryMap(char* argumentos[MAXARGS], listaBloques *l);
void CmdRead(char* ar[]);
void CmdWrite(char* ar[]);
void CmdMemdump(char* ar[]);
void memfill(char* argumentos[MAXARGS]);
void mem(char *argumentos[], listaBloques l);
void recurse(char* argumentos[MAXARGS]);


#endif
