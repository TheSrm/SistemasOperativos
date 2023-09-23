#include "autores.h"

void authors (char* argumentos[MAXARGS]){

    if ((argumentos[0]) == NULL) {
        printf("Name: Daniel Ruibal Balboa\tLogin: daniel.ruibal.balboa@udc.es\n");
        printf("Name: Sergio Regueiro Martínez\tLogin: sergio.regueiro.martinez@udc.es\n");
    }
    else if (strcmp(argumentos[0], "-l") == 0) {
        printf("Login: daniel.ruibal.balboa@udc.es\n");
        printf("Login: sergio.regueiro.martinez@udc.es");
    }
    else if (strcmp(argumentos[0], "-n") == 0) {
        printf("Name: Daniel Ruibal Balboa\n");
        printf("Name: Sergio Regueiro Martínez\n");
    }
}