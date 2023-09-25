#ifndef FICHEIROS_H
#define FICHEIROS_H

#include "definicionsBase.h"
#include <dirent.h>

void Cmd_open (char * argumentos[]);
void Cmd_close (char *argumentos[]);
void Cmd_dup (char * argumentos[]);
void listarFicheiros(); //está provisionalmente, só para probar

#endif
