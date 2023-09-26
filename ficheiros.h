#ifndef FICHEIROS_H
#define FICHEIROS_H

#define NULLFICH NULL
#define O_CREAT 0b1000000
#define O_EXCL 0b0100000
#define O_RDONLY 0b0010000
#define O_WRONLY 0b0001000
#define O_RDWR 0b0000100
#define O_APPEND 0b0000010
#define O_TRUNC 0b0000001

#include "definicionsBase.h"
//#include <dirent.h>

void Cmd_open (char * argumentos[]);
void Cmd_close (char *argumentos[]);
void Cmd_dup (char * argumentos[]);

typedef struct ficheiro{
    unsigned int descriptor;
    short int modo;
    char *nome;
    struct ficheiro *next;
}ficheiro;

typedef ficheiro *taboaFicheiros;

void crearTaboaFich(taboaFicheiros *t);

#endif
