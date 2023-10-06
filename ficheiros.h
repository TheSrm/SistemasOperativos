#ifndef FICHEIROS_H
#define FICHEIROS_H

#include "definicionsBase.h"
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>

#define NULLFICH NULL
#define MAXNAME 30

typedef struct ficheiro{
    unsigned int descriptor;
    int modo;
    char *nome;
    struct ficheiro *next;
}ficheiro;

typedef ficheiro *taboaFicheiros;

void crearTaboaFich(taboaFicheiros *t);
void Cmd_open (char * argumentos[], taboaFicheiros *t);
void Cmd_close (char *argumentos[], taboaFicheiros *t);
void Cmd_dup (char * argumentos[], taboaFicheiros *t);
void listarAbertos(taboaFicheiros t);

void listarFicheiros();
void borrarFicheiros(char *argumentos[]);

int cambiarDirectorio(char *argumentos[]);
void pecharTodoFicheiro(taboaFicheiros *t);

#endif
