#include "entradas.h"

void leerEntrada(char *entrada, historial h) {
    fgets(entrada, ENTRADA_MAX, stdin);
    insertHist(&h);
}

void procesarEntrada(char *entrada) {
    int i;
    char* argumentos[MAXARGS];
    char* argPpal;
    argPpal = strtok(entrada, " ");
    for (i = 0; i < MAXARGS; ++i) {
        argumentos[i] = strtok(NULL, " ");
    }
    if(strcmp(argPpal, "time") == 0)
        imprHora();
    else if(strcmp(argPpal, "date") == 0)
        imprData();
}