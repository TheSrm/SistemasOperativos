#include "entradas.h"

void shutDown (bool* rematado){
    *rematado = true;
}

void lerEntrada(char *entrada) { // poñer hist despois, tamén no .h
    char buffer[ENTRADA_MAX];
    fgets(entrada, ENTRADA_MAX, stdin);
   // insertHist(&h);
}

void procesarEntrada(char *entrada, bool* rematado) {
    int i;
    char* argPpal, *argumentos[MAXARGS];


    argPpal = strtok(entrada, " \n\t");

    for (i = 0; i < MAXARGS; ++i) {
        argumentos[i] = strtok(NULL, " \n\t");
    }
    if(strcmp(argPpal, "time") == 0)
        imprHora();
    else if(strcmp(argPpal, "date") == 0)
        imprData();
    else if(strcmp(argPpal, "authors") == 0)
        authors(argumentos);
    else if(strcmp(argPpal,"quit") + strcmp(argPpal,"exit") + strcmp(argPpal,"bye") == 0) {
        shutDown(rematado);
        free(entrada);
    }
    else{
        printf("Comando invalido\n"); // implementar erros?
    }
}