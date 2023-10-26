#ifndef FICHEIROS_H
#define FICHEIROS_H

#include "definicionsBase.h"
#include <dirent.h>
#include <unistd.h>
#include <fcntl.h>
#include <ftw.h>
#include <sys/stat.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>

#define NULLFICH NULL
#define MAXNAME 30
#define MAX_PATHSTRING 256
#define MAXDIR 100

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

void listarFicheiros(char *argumentos[], short int modoRec, int numrec);
void borrarFicheiros(char *argumentos[], bool recursivo);

int cambiarDirectorio(char *argumentos[]);
void pecharTodoFicheiro(taboaFicheiros *t);

void create (char *argumentos[]);
void stats (char *argumentos[]);

#endif
