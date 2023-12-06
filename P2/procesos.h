#ifndef PROCESOS_H
#define PROCESOS_H

#include "definicionsBase.h"
#include <unistd.h>
#include"sys/utsname.h"
#include <pwd.h>
#include <sys/wait.h>

#define MAXVAR 5000

typedef struct SEN{
    char *nombre;
    int senal;
};

typedef struct proceso{
    pid_t pid;
    time_t data;
    int prioridade;
    char *commans_line;
    struct proceso *next;
} Proceso;

typedef Proceso *listaProcesos;


void crearListaProcesos(listaProcesos *t);

void pid(char* argumentos[MAXARGS]);
void infosys();
void uid(char *argumentos[]);
void ComandoNonConocido(char* ArgPpal , char* arguentos[MAXARGS], listaProcesos *l);
void MostrarJobs(listaProcesos listaProcesos1);

#endif
