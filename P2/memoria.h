#ifndef MEMORIA_H
#define MEMORIA_H

#include "definicionsBase.h"
#include "procesos.h"

#define TAMANO 2048

typedef u_long tam_t;
typedef int key_t;

typedef struct bloquesMemoria{
    void *direccion;
    time_t dataCreacion;
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
void mmap(char* rutaFich, listaBloques l);
void CmdRead(char* ar[]);
void CmdWrite(char* ar[]);
void memdump(listaBloques l);
void memfill(char* argumentos[MAXARGS]);
void mem(listaBloques l);
void recurse(char* argumentos[MAXARGS]);


#endif
