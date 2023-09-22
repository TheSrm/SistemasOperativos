#include "entradas.h"

void lerEntrada(char *entrada) { // poñer hist despois, tamén no .h
    char buffer[ENTRADA_MAX];
    fgets(entrada, ENTRADA_MAX, stdin);
   // insertHist(&h);
}

void procesarEntrada(char *entrada) {
    int i;
    char* argumentos[MAXARGS];
    char* argPpal;

    argPpal = strtok(entrada, " \n\t");

    for (i = 0; i < MAXARGS; ++i) {
        argumentos[i] = strtok(NULL, " \n\t");
    }
    if(strcmp(argPpal, "time") == 0)
        imprHora();
    else if(strcmp(argPpal, "date") == 0)
        imprData();
    else{
        printf("Comando non valido\n"); // implementar erros?
    }
}