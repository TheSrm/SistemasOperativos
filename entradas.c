#include "entradas.h"

void lerEntrada(char **entrada) { // poñer hist despois, tamén no .h
    char buffer[ENTRADA_MAX];
    *entrada = fgets(buffer, ENTRADA_MAX, stdin);
   // insertHist(&h);
}

void procesarEntrada(char *entrada) {
    int i;
    char* argumentos[MAXARGS];
    char* argPpal;
    argPpal = strtok(entrada, " ,\n");
    for (i = 0; i < MAXARGS; ++i) {
        argumentos[i] = strtok(NULL, " ");
    }
    if(strcmp(argPpal, "time") == 0)
        imprHora();
    else if(strcmp(argPpal, "date") == 0)
        imprData();
}