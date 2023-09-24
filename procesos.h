#ifndef PROCESOS_H
#define PROCESOS_H

#include "definicionsBase.h"
#include <unistd.h>
#include"sys/utsname.h"


void pid(char* argumentos[MAXARGS]);
void Cmd_open(char *argumentos[MAXARGS]);
void Cmd_dup(char *argumentos[MAXARGS]);
void Cmd_close(char *argumentos[MAXARGS]);
void infosys();

#endif
