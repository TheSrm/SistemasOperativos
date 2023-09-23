#include "procesos.h"

void pid(char *argumentos[MAXARGS]) {
    pid_t getppid();
    if ((argumentos[0]) == NULL) {
        printf("Process id: %d\n", getppid());
    } else {
        printf("%lli", getppid());
    }
}