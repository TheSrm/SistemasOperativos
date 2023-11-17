#ifndef MEMORIA_H
#define MEMORIA_H

#include "definicionsBase.h"
#include "procesos.h"

#define TAMANO 2048

typedef struct bloquesMemoria{
    void *direccion;
    time_t dataCreacion;
    double tamanoBloque;
    char* tipoAsignacion;
    struct bloqueMemoria *next;
}bloquesMemoria;

typedef bloquesMemoria *listaBloques;

void crearTaboaBloques(listaBloques *l);

void pecharTodoBloque(listaBloques *l);
void memAlloc( listaBloques *l, char *argumentos[MAXARGS]);
void shared(int size, listaBloques l);
void mmap(char* rutaFich, listaBloques l);
void readFile(char* rutaFich, listaBloques l);
void memdump(listaBloques l);
void memfill(listaBloques l);
void mem();
void recurse(char* argumentos[MAXARGS]);


#endif
