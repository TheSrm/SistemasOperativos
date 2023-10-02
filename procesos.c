#include "procesos.h"

void pid(char *argumentos[MAXARGS]) {
    pid_t process_id=getpid();
    pid_t p_process_id=getppid();

    if (argumentos[0]==NULL) {
        printf("Process id: %lli\n", process_id);
    }
    else if(strcmp(argumentos[0],"-p")==0)
        printf("Parent's process id: %i\n",p_process_id);
}
//
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