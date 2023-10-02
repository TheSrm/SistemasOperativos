#include "autores.h"

//Esta función recibe o segundo argumento introducido e segundo esto mostra unha cousa ou outra
void authors (char* argumentos[MAXARGS]){

    //Se non recibiu argumento ningún, mostra tanto nosos nomes coma logins
    if ((argumentos[0]) == NULL) {
        printf("Name: Daniel Ruibal Balboa\tLogin: daniel.ruibal.balboa@udc.es\n");
        printf("Name: Sergio Regueiro Martínez\tLogin: sergio.regueiro.martinez@udc.es\n");
    }
    //Se recibiu  -l, mostra so os logins
    else if (strcmp(argumentos[0], "-l") == 0) {
        printf("Login: daniel.ruibal.balboa@udc.es\n");
        printf("Login: sergio.regueiro.martinez@udc.es");
    }
    //Se recibiu -n, mostra so os nomes
    else if (strcmp(argumentos[0], "-n") == 0) {
        printf("Name: Daniel Ruibal Balboa\n");
        printf("Name: Sergio Regueiro Martínez\n");
    }
}