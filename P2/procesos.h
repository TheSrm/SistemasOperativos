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
    int estado;
    int prioridade;
    pid_t commans_line;
    struct proceso *next;
} Proceso;

typedef Proceso *listaProcesos;


void crearListaPorcesos(listaProcesos *t);
void pid(char* argumentos[MAXARGS]);
void infosys();
void uid(char *argumentos[]);

#endif
