#include <signal.h>
#include <time.h>
#include <unistd.h>
#include <sys/resource.h>
#include "procesos.h"

void crearListaProcesos(listaProcesos *lista){
    *lista=NULL;
}

extern char **environ;

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
    bool Senalado;
} EstadoProceso;

EstadoProceso obtenerEstadoProceso(Proceso *l) {
    EstadoProceso resultado = {0, 0};
    int status;



    // Esperar a que el proceso hijo termine (modo no bloqueante)
    pid_t result = waitpid(l->pid, &status, WNOHANG | WUNTRACED | WCONTINUED);

    if (l->state==2 && !WIFCONTINUED(status)) {
        resultado.senal =l->senal;
        resultado.estado=l->state;
        return resultado;
    }
    if (l->state==2 && WIFCONTINUED(status)) {
        resultado.senal=0;
        resultado.estado=-1;
        return resultado;
    }



    if (result == 0) {
        // El proceso hijo aún no ha terminado
        resultado.estado = -1;  // Mantenemos el valor original
        return resultado;
    } else if (result == -1) {
        resultado.estado = l->state;
        resultado.senal = l->senal;
        return resultado;
    }

    resultado.estado = status;

    // Conservar la señal anterior si la señal obtenida es nula


    // Verificar el estado del proceso hijo
    if (WIFEXITED(status)) {
        resultado.senal = WEXITSTATUS(status);
        l->senal = resultado.senal;
    } else if (WIFSIGNALED(status)) {
        resultado.estado = 1;  // Estado diferente de cero para indicar que se terminó por señal
        resultado.senal = WTERMSIG(status);
        l->senal = resultado.senal;
    } else if (WIFSTOPPED(status)) {
        resultado.estado = 2;
        resultado.senal = WSTOPSIG(status);
        l->senal = resultado.senal;
    } else {
        resultado.estado = EXIT_FAILURE;
    }

    if (resultado.senal == 0 || strcmp(NombreSenal(resultado.senal), "SIGUNKNOWN") == 0) {
        resultado.senal = l->senal;
        l->senal = resultado.senal;
    }



    return resultado;
}


void MostrarEstadoPantalla(Proceso *l) {
    EstadoProceso estadoProceso = obtenerEstadoProceso(l);
    int state = estadoProceso.estado;
    l->state = state;
    uid_t uid = getuid();

    struct passwd *user_info = getpwuid(uid);
    if (user_info != NULL) {
        printf("\t%d\t%s\tp=%d \t%d/%d/%d %02d:%02d:%02d", l->pid, user_info->pw_name,
               getpriority(PRIO_PROCESS, l->pid), localtime(&l->data)->tm_mday,
               localtime(&l->data)->tm_mon, localtime(&l->data)->tm_year + 1900,
               localtime(&l->data)->tm_hour, localtime(&l->data)->tm_min,
               localtime(&l->data)->tm_sec);
    } else {
        // Manejar el caso en el que no se puede obtener la información del usuario
        fprintf(stderr, "Error al obtener información del usuario.\n");
        return;
    }


    switch (estadoProceso.estado) {
        case 1:
            printf(" SIGNALED (%s) %s", NombreSenal(estadoProceso.senal), l->commans_line);
            break;
        case 0:
            printf(" TERMINADO (000) %s", l->commans_line);
            break;
        case 2:
            printf(" STOPPED (0%d) %s", estadoProceso.senal, l->commans_line);
            break;
        case -1:
            printf(" ACTIVO (000) %s", l->commans_line);
            break;
        default:
            printf(" TERMINADO (255)");
            l->state=10;
            break;
    }

    printf("\n");
}



void MostrarJobs(listaProcesos listaProcesos1){
 Proceso *l;
    for (l= listaProcesos1; l != NULL  ; l=l->next) {
        MostrarEstadoPantalla(l);

    }
}

void VaciarListaProcesos(listaProcesos *cabeza) {
    Proceso *actual = *cabeza;
    Proceso *siguiente = NULL;  // Cambié el nombre de 'anterior' a 'siguiente'

    while (actual != NULL) {
        siguiente = actual->next;
        free(actual);
        actual = siguiente;
    }

    *cabeza = NULL;  // Después de vaciar la lista, la cabeza debe ser NULL
}

void eliminarElementos(listaProcesos *cabeza, int condicion) {
    Proceso *actual = *cabeza;
    Proceso *anterior = NULL;  // Inicializar anterior a NULL

    while (actual != NULL) {
        int actualP = obtenerEstadoProceso(actual).estado;


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


void CmdShowenv(char *argumentos[], char *env[]){
    int i;
    if(argumentos == NULL || argumentos[0]==NULL || (strcmp(argumentos[0],"-addr")!=0 && strcmp(argumentos[0],"-environ")!=0))
        for(i = 0; env[i] != NULL; i++)
            printf("%s\n",env[i]);
    else if (strcmp(argumentos[0],"-environ")==0)
        for(i = 0; env[i] != NULL; i++)
            printf("%s\n",environ[i]);
    else
        printf("Direccion arg3: %p\n"
               "Direccion environ: %p\n",
               &env,&environ);
}

void CmdShowvar(char *argumentos[], char *env[]){
    int n_env, n_environ;
    char *g_env;
    if(argumentos[0]==NULL)
        CmdShowenv(NULL, env);
    else{
        n_env = BuscarVariable(argumentos[0], env),
                n_environ = BuscarVariable(argumentos[0], environ);
        g_env = getenv(argumentos[0]);
        if (n_env != -1 && n_environ != -1 && g_env != NULL)
            printf("Environ: %s\narg3: %s\ngetenv: %s\n", environ[n_environ], env[n_env], g_env);
    }
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

void CmdChangevar(char *argumentos[], char *env[]){
    char* e_var;
    if(argumentos[0]!=NULL && argumentos[2]!=NULL) {
        if (strcmp(argumentos[0], "-a") == 0) {
            if (CambiarVariable(argumentos[1], argumentos[2], env) == -1)
                perror("Erro ao cambiar o valor da variable");
        }
        else if (strcmp(argumentos[0], "-e") == 0){
            if(CambiarVariable(argumentos[1], argumentos[2], environ)==-1)
                perror("Erro ao cambiar o valor da variable");

        }
        else if (strcmp(argumentos[0], "-p") == 0){
            e_var = malloc(strlen(argumentos[1])+ strlen(argumentos[2])+2);
            sprintf(e_var, "%s=%s", argumentos[1], argumentos[2]);
            if(putenv(e_var)!=0)
                perror("Erro ao cambiar o valor da variable");
        } else printf("Uso: changevar [-a|-e|-p] var valor\n");
    }
    else
        printf("Uso: changevar [-a|-e|-p] var valor\n");
}

void CmdSubsvar(char *argumentos[], char *env[]){
    char *e_var;
    int pos;
    if(argumentos[3]!=NULL){
        e_var = malloc(strlen(argumentos[2])+ strlen(argumentos[3])+2);
        sprintf(e_var, "%s=%s", argumentos[2], argumentos[3]);
        if(strcmp(argumentos[0],"-a")==0){
            if((pos=BuscarVariable(argumentos[1],env))==-1)
                perror("Variable non atopada");
            else
                env[pos] = e_var;
        } else if(strcmp(argumentos[0],"-a")==0){
            if((pos=BuscarVariable(argumentos[1],environ))==-1)
                perror("Variable non atopada");
            else
                environ[pos] = e_var;
        } else printf("Uso: subsvar [-a|-e] var1 var2 valor\n");
    } else
        printf("Uso: subsvar [-a|-e] var1 var2 valor\n");
}

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

void ComandoNonConocido(char *comando, char *argumentos[],listaProcesos *l) {
    char* ComandoCorrecto[MAXARGS];
    bool insertar=CombinarArrays(ComandoCorrecto,comando,argumentos);//Necesitamos poñer o comando 2 veces en Comando correcto, por lagunha razón que debe ser así

    pid_t pid = fork();

    if (pid == -1) {
        perror("Error en fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) {  // Proceso hijo
        execvp(comando, ComandoCorrecto);
        perror("Error ejecutando el comando");
        exit(EXIT_FAILURE);

    } else {  // Proceso padre

        if (!insertar)//Se non temos que insertalo na lista, é un proceso en primeiro plano, teño qeu esperar por el,
            waitpid(pid, NULL, 0);
        else
            insertarListaProcesos(l, pid, getpriority(PRIO_PROCESS,getpid()), ComandoCorrecto);


    }

}


void EliminarJobs (char *argumentos[], listaProcesos *listaProcesos1){
    if (argumentos[0]==NULL)
        MostrarJobs(*listaProcesos1);

    MostrarJobs(*listaProcesos1);

    if (argumentos[0]!=NULL && *listaProcesos1!=NULL) {
        if (strcmp(argumentos[0], "-term")==0) {
            eliminarElementos(listaProcesos1, 0);
            eliminarElementos(listaProcesos1, 10);

        }
        else if (strcmp(argumentos[0], "-sig")==0) {
            eliminarElementos( listaProcesos1, 1);
        }

    }

}

void eliminarBloquePorPID(listaProcesos *cabeza, int pidAEliminar) {
    Proceso *actual = *cabeza;
    Proceso *anterior = NULL;

    while (actual != NULL && actual->pid != pidAEliminar) {
        anterior = actual;
        actual = actual->next;
    }

    if (actual != NULL) {
        if (anterior != NULL) {
            anterior->next = actual->next;
        } else {
            *cabeza = actual->next;
        }
        free(actual);
    }
}
void job (char *argumentos[], listaProcesos *listaProcesos1){
    if (argumentos[0]==NULL)
        MostrarJobs(*listaProcesos1);
    else{

        if (strcmp(argumentos[0],"-fg")==0) {
            int *State;
            pid_t PID=strtol(argumentos[1], NULL, 10);
            eliminarBloquePorPID(listaProcesos1,PID);
             waitpid(PID, State, 0);

             if(State==NULL) {
                 State = 0;
             }

            printf("Proceso %d terminado. Valor %d devuelto\n", PID, *State);

        }else{
            Proceso *l;
            long PID=strtol(argumentos[0], NULL, 10);
            for (l= *listaProcesos1; l != NULL && l->pid!=PID  ; l=l->next);
            if (l==NULL)
                printf("Non se meteu un pid dun proceso adecuado\n");
            else
                MostrarEstadoPantalla(l);
        }
    }
}

 void Exec ( char *argumentos[MAXARGS]){
     execvp(argumentos[0],argumentos);
     perror("Error ejecutando el comando");
     exit(EXIT_FAILURE);
 }






