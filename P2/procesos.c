#include <signal.h>
#include <time.h>
#include <sys/resource.h>



#include "procesos.h"

void crearListaProcesos(listaProcesos *lista){
    *lista=NULL;
}


static  struct SEN sigstrnum[] = {
        {"HUP", SIGHUP},
        {"INT", SIGINT},
        {"QUIT", SIGQUIT},
        {"ILL", SIGILL},
        {"TRAP", SIGTRAP},
        {"ABRT", SIGABRT},
        {"IOT", SIGIOT},
        {"BUS", SIGBUS},
        {"FPE", SIGFPE},
        {"KILL", SIGKILL},
        {"USR1", SIGUSR1},
        {"SEGV", SIGSEGV},
        {"USR2", SIGUSR2},
        {"PIPE", SIGPIPE},
        {"ALRM", SIGALRM},
        {"TERM", SIGTERM},
        {"CHLD", SIGCHLD},
        {"CONT", SIGCONT},
        {"STOP", SIGSTOP},
        {"TSTP", SIGTSTP},
        {"TTIN", SIGTTIN},
        {"TTOU", SIGTTOU},
        {"URG", SIGURG},
        {"XCPU", SIGXCPU},
        {"XFSZ", SIGXFSZ},
        {"VTALRM", SIGVTALRM},
        {"PROF", SIGPROF},
        {"WINCH", SIGWINCH},
        {"IO", SIGIO},
        {"SYS", SIGSYS},
/*senales que no hay en todas partes*/
#ifdef SIGPOLL
        {"POLL", SIGPOLL},
#endif
#ifdef SIGPWR
        {"PWR", SIGPWR},
#endif
#ifdef SIGEMT
        {"EMT", SIGEMT},
#endif
#ifdef SIGINFO
        {"INFO", SIGINFO},
#endif
#ifdef SIGSTKFLT
        {"STKFLT", SIGSTKFLT},
#endif
#ifdef SIGCLD
        {"CLD", SIGCLD},
#endif
#ifdef SIGLOST
        {"LOST", SIGLOST},
#endif
#ifdef SIGCANCEL
        {"CANCEL", SIGCANCEL},
#endif
#ifdef SIGTHAW
        {"THAW", SIGTHAW},
#endif
#ifdef SIGFREEZE
        {"FREEZE", SIGFREEZE},
#endif
#ifdef SIGLWP
        {"LWP", SIGLWP},
#endif
#ifdef SIGWAITING
        {"WAITING", SIGWAITING},
#endif
        {NULL,-1},
};    /*fin array sigstrnum */


int ValorSenal(char * sen)  /*devuelve el numero de senial a partir del nombre*/
{
    int i;
    for (i=0; sigstrnum[i].nombre!=NULL; i++)
        if (!strcmp(sen, sigstrnum[i].nombre))
            return sigstrnum[i].senal;
    return -1;
}


char *NombreSenal(int sen)  /*devuelve el nombre senal a partir de la senal*/
{
    int i;
    for (i=0; sigstrnum[i].nombre!=NULL; i++)
        if (sen==sigstrnum[i].senal)
            return sigstrnum[i].nombre;
    return ("SIGUNKNOWN");
}

typedef struct {
    int estado;
    int senal;
} EstadoProceso;

EstadoProceso obtenerEstadoProceso(Proceso *l) {
    EstadoProceso resultado = {0, 0};
    int status;

    // Esperar a que el proceso hijo termine (modo no bloqueante)
    pid_t result = waitpid(l->pid, &status, WNOHANG);

    if (result == 0) {
        // El proceso hijo aún no ha terminado
        resultado.estado = -1;
        return resultado;
    } else if (result == -1) {
        resultado.estado = l->state;
        resultado.senal = l->senal;
        return resultado;
    }

    resultado.estado = status;

    // Verificar el estado del proceso hijo
    if (WIFEXITED(status)) {
        resultado.senal = WEXITSTATUS(status);
        l->senal=resultado.senal;
    } else if (WIFSIGNALED(status)) {
        resultado.estado = 1;  // Estado diferente de cero para indicar que se terminó por señal
        resultado.senal = WTERMSIG(status);
        l->senal=resultado.senal;
    } else if (WIFSTOPPED(status)) {
        resultado.estado = 2;
        resultado.senal = WSTOPSIG(status);
        l->senal=resultado.senal;// Estado diferente de cero para indicar que se detuvo por señal
    } else {
        resultado.estado = EXIT_FAILURE;
    }

   // Conservar la señal anterior si la señal obtenida es nula
    if (resultado.senal == 0 || strcmp(NombreSenal(resultado.senal),"SIGUNKNOWN")==0) {
        resultado.senal = l->senal;
        l->senal=resultado.senal;
    }
    return resultado;
}


void MostrarEstadoPantalla(Proceso *l) {
    EstadoProceso estadoProceso = obtenerEstadoProceso(l);
    int state = estadoProceso.estado;
    int beforestate = l->state;
    l->state = state;  // Se asume que esta asignación es necesaria, ajusta según tus necesidades.
    uid_t uid = getuid();

    struct passwd *user_info = getpwuid(uid);
    if (user_info != NULL) {
        printf("\t%d\t%s\tp=%d\t%d/%d/%d %02d:%02d:%02d", l->pid, user_info->pw_name,
               getpriority(PRIO_PROCESS, l->pid), localtime(&l->data)->tm_mday,
               localtime(&l->data)->tm_mon, localtime(&l->data)->tm_year + 1900,
               localtime(&l->data)->tm_hour, localtime(&l->data)->tm_min,
               localtime(&l->data)->tm_sec);
    } else {
        // Manejar el caso en el que no se puede obtener la información del usuario
        fprintf(stderr, "Error al obtener información del usuario.\n");
        return;  // O manejar el error de otra manera según tus necesidades.
    }

    if (beforestate == 0 && state == 1) {
        l->state = beforestate;
    }

    switch (estadoProceso.estado) {
        case -1:
            printf(" ACTIVO (000) %s", l->commans_line);
            break;
        case 0:
            printf(" TERMINADO (000) %s", l->commans_line);
            break;
        case 1:
            printf(" SIGNALED (%s) %s", NombreSenal(estadoProceso.senal), l->commans_line);
            break;
        case 2:
            printf(" STOPPED (%s) %s", NombreSenal(estadoProceso.senal), l->commans_line);
            break;
        default:
            printf(" Error inesperado al obtener el estado del proceso %d", l->pid);
    }

    printf("\n");
}



void MostrarJobs(listaProcesos listaProcesos1){
 Proceso *l;
    for (l= listaProcesos1; l != NULL  ; l=l->next) {
        MostrarEstadoPantalla(l);

    }
}

void VaciarListaProcesos(listaProcesos *listaProcesos1){
    if ( listaProcesos1 != NULL) {
        Proceso *l;
        for (l = (Proceso *) listaProcesos1; l != NULL; l = l->next) {
            free(l);
        }
        free(listaProcesos1);
    }
}

void eliminarElementos(listaProcesos *cabeza, int condicion) {
    Proceso *actual = *cabeza;
    Proceso *anterior = NULL;  // Inicializar anterior a NULL

    while (actual != NULL) {
        int beforestate = actual->state;
        int actualP = obtenerEstadoProceso(actual).estado;

        if (beforestate == 1 && actualP == 0) {
            actualP = beforestate;
        }

        if (actualP == condicion) {
            // El elemento cumple con la condición, se elimina
            if (anterior == NULL) {
                *cabeza = actual->next;
                free(actual);
                actual = *cabeza;
            } else {
                anterior->next = actual->next;
                free(actual);
                actual = anterior->next;
            }
        } else {
            // El elemento no cumple con la condición, se avanza
            anterior = actual;
            actual = actual->next;
        }
    }
}




void insertarListaProcesos(listaProcesos *lista, pid_t pid, int prioridade, char *commans_line[]) {
    Proceso *nuevoBloque = (Proceso *)malloc(sizeof(Proceso));
    Proceso *temp2 = *lista;

    if (nuevoBloque == NULL) {
        fprintf(stderr, "Error: No se pudo asignar memoria para el nuevo proceso.\n");
        return;
    }
    nuevoBloque->state=1;
    nuevoBloque->pid = pid;
    nuevoBloque->senal=0;
    nuevoBloque->data = time(NULL);


    // Concatenar los elementos del array en una sola cadena
    int total_length = 0;
    for (int i = 0; commans_line[i] != NULL; i++) {
        total_length += strlen(commans_line[i]) + 1; // +1 para el espacio entre palabras
    }

    nuevoBloque->commans_line = (char *)malloc(total_length);
    nuevoBloque->commans_line[0] = '\0'; // Inicializar la cadena

    for (int i = 0; commans_line[i] != NULL; i++) {
        strcat(nuevoBloque->commans_line, commans_line[i]);
        strcat(nuevoBloque->commans_line, " ");
    }

    nuevoBloque->next = NULL;

    if (*lista == NULL) {
        *lista = nuevoBloque;
    } else {
        // Encontrar el final de la lista
        for (; temp2->next != NULL; temp2 = temp2->next);
        // Enlazar el nuevo bloque al final de la lista
        temp2->next = nuevoBloque;
    }
}

void pid(char *argumentos[MAXARGS]) {
    pid_t process_id=getpid();
    pid_t p_process_id=getppid();

    if (argumentos[0]==NULL) {
        printf("Process id: %i\n", process_id);
    }
    else if(strcmp(argumentos[0],"-p")==0)
        printf("Parent's process id: %i\n",p_process_id);
}

void uid(char *argumentos[]){
    uid_t uid = getuid(), euid = geteuid();
    if(argumentos[0]!=NULL && strcmp(argumentos[0],"-set")==0 && argumentos[1]!=NULL)
        setuid(strtol(argumentos[1],NULL,10));
    else
        printf("Credencial real: %d (%s)\n"
                     "Credencial gardada: %d (%s)\n"
                     ,uid, getpwuid(uid)->pw_name, euid, getpwuid(euid)->pw_name);
}

void infosys(){
        struct utsname Datos_del_ordenador;

        if (uname(&Datos_del_ordenador) != 0) {
            printf("Erro ao obter información do sistema\n");
            return;
        }

        printf("Informacion do sistema:\n");
        printf("  - Sistema operativo: %s\n", Datos_del_ordenador.sysname);
        printf("  - Nome do nodo: %s\n", Datos_del_ordenador.nodename);
        printf("  - Release: %s\n", Datos_del_ordenador.release);
        printf("  - Version: %s\n", Datos_del_ordenador.version);
        printf("  - Maquina: %s\n", Datos_del_ordenador.machine);

        long procesadores = sysconf(_SC_NPROCESSORS_ONLN);
        if (procesadores <0) {
            printf("Numero de procesadores: %ld\n", procesadores);
        } else {
            printf("Erro ao obter o numero de procesadores\n");
        }
}

void Cmd_fork (char *tr[],listaProcesos *listaProcesos1){
    pid_t pid;

    if ((pid=fork())==0){
        VaciarListaProcesos(listaProcesos1);
        printf ("ejecutando proceso %d\n", getpid());
    }
    else if (pid!=-1)
        waitpid (pid,NULL,0);
}
int BuscarVariable (char * var, char *e[])  /*busca una variable en el entorno que se le pasa como parámetro*/
{
    int pos=0;
    char aux[MAXVAR];

    strcpy (aux,var);
    strcat (aux,"=");

    while (e[pos]!=NULL)
        if (!strncmp(e[pos],aux,strlen(aux)))
            return (pos);
        else
            pos++;
    errno=ENOENT;   /*no hay tal variable*/
    return(-1);
}

int CambiarVariable(char * var, char * valor, char *e[]) /*cambia una variable en el entorno que se le pasa como parámetro*/
{                                                        /*lo hace directamente, no usa putenv*/
    int pos;
    char *aux;

    if ((pos=BuscarVariable(var,e))==-1)
        return(-1);

    if ((aux=(char *)malloc(strlen(var)+strlen(valor)+2))==NULL)
        return -1;
    strcpy(aux,var);
    strcat(aux,"=");
    strcat(aux,valor);
    e[pos]=aux;
    return (pos);
}

/*las siguientes funciones nos permiten obtener el nombre de una senal a partir
del número y viceversa */



bool CombinarArrays(char *Destino[MAXARGS], char *String, char *Origen[MAXARGS]) {
    int i;
    bool ESSP=false;

    if (String == NULL) {
        printf("No aceptamos comandos de este tipo\n");
        return ESSP;
    }

    // Agregar String a Destino
    Destino[0] = String;

    // Copiar elementos de Origen a Destino
    for (i = 0; Origen[i] != NULL && i < MAXARGS - 1; ++i) {
        if (strcmp(Origen[i], "&") == 0){
            ESSP = true;
        break;
    }
            Destino[i + 1] = Origen[i];

    }

    // Asegurarse de que la última posición del array sea NULL
    Destino[i + 1] = NULL;
    return ESSP;
}


void ComandoNonConocido(char *comando, char *argumentos[], listaProcesos *l) {
    char* ComandoCorrecto[MAXARGS];
    bool insertar = CombinarArrays(ComandoCorrecto, comando, argumentos);

    pid_t pid = fork();

    if (pid == -1) {
        perror("Error en fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Proceso hijo
        execvp(comando, ComandoCorrecto);
        // Si execvp tiene éxito, no se ejecutará esta línea
        perror("Error ejecutando el comando");
        exit(EXIT_FAILURE);

    } else {  // Proceso padre
        int status;
        waitpid(pid, &status, WNOHANG);
        // Verificar si el proceso hijo terminó correctamente
        if (WIFEXITED(status) && WEXITSTATUS(status) == 0) {
            if (insertar) {
                insertarListaProcesos(l, pid, getpriority(PRIO_PROCESS, getpid()), ComandoCorrecto);
            }
        }
    }
}


void EliminarJobs (char *argumentos[], listaProcesos *listaProcesos1){
    if (argumentos[0]==NULL)
        MostrarJobs(*listaProcesos1);

    if (argumentos[0]!=NULL && *listaProcesos1!=NULL) {
        if (strcmp(argumentos[0], "-term")==0) {
            eliminarElementos(listaProcesos1, 0);
            MostrarJobs(*listaProcesos1);

        }
        else if (strcmp(argumentos[0], "-sig")==0) {
            eliminarElementos( listaProcesos1, 1);
        }
        MostrarJobs(*listaProcesos1);
    }

}


