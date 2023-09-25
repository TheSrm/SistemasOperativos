#include "procesos.h"

void pid(char *argumentos[MAXARGS]) {
    pid_t process_id=getpid();
    //pid_t p_process_id=getppid();

    if (argumentos[0]==NULL) {
        printf("Process id: %lli\n", process_id);
    }
    //else
   // printf("Parent's process id: %lli\n",p_process_id);

}

void infosys(){
        struct utsname Datos_del_ordenador;

        if (uname(&Datos_del_ordenador) != 0) {
            perror("Error retrieving system information");
            return;
        }

        printf("System Information:\n");
        printf("  - Operating System: %s\n", Datos_del_ordenador.sysname);
        printf("  - Node Name: %s\n", Datos_del_ordenador.nodename);
        printf("  - Release: %s\n", Datos_del_ordenador.release);
        printf("  - Version: %s\n", Datos_del_ordenador.version);
        printf("  - Machine: %s\n", Datos_del_ordenador.machine);

        long procesadores = sysconf(_SC_NPROCESSORS_ONLN);
        if (procesadores <0) {
            printf("Number of Processors: %ld\n", procesadores);
        } else {
            perror("Error retrieving number of processors");
        }
}