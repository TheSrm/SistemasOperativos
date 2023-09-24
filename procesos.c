#include "procesos.h"

void pid(char *argumentos[MAXARGS]) {
    pid_t process_id=getpid();
    pid_t p_process_id=getppid();

    if (argumentos[0]==NULL) {
        printf("The process id: %lli\n", process_id);
    }
    else
    printf("The process id of parent function: %lli\n",p_process_id);

}