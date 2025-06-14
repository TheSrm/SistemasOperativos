#ifndef PROCESOS_H
#define PROCESOS_H

#include "definicionsBase.h"
#include"sys/utsname.h"
#include "historial.h"
#include "ficheiros.h"
#include <pwd.h>
#include <sys/wait.h>

#include "memoria.h"

#define MAXVAR 5000

 struct SEN{
    char *nombre;
    int senal;
};

typedef struct proceso{
    pid_t pid;
    time_t data;
    int state;
    int senal;
    char *commans_line;
    struct proceso *next;
} Proceso;

typedef Proceso *listaProcesos;

void Exec (char *argumentos[]);
void job (char *argumentos[], listaProcesos *listaProcesos1);
void crearListaProcesos(listaProcesos *t);
void EliminarJobs (char *argumentos[], listaProcesos *listaProcesos1);
void VaciarListaProcesos(listaProcesos *listaProcesos1);
void CmdShowvar(char *argumentos[], char *env[]);
void CmdShowenv(char *argumentos[], char *env[]);
void CmdChangevar(char *argumentos[], char *env[]);
void CmdSubsvar(char *argumentos[], char *env[]);
void Cmd_fork (char *argumentos[],listaProcesos *listaprocesos1);
void pid(char* argumentos[MAXARGS]);
void infosys();
void uid(char *argumentos[]);
void ComandoNonConocido(char* ArgPpal , char* argumentos[MAXARGS], listaProcesos *l);
void MostrarJobs(listaProcesos listaProcesos1);

#endif
