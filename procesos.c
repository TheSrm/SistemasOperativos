#include "procesos.h"

void pid(char *argumentos[MAXARGS]) {
    pid_t process_id=getpid();
    pid_t p_process_id=getppid();

    if (argumentos[0]==NULL) {
        printf("Process id: %lli\n", process_id);
    }
    else
    printf("Parent's process id: %lli\n",p_process_id);

}