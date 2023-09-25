#include "entradas.h"

void shutDown (bool* rematado){
    *rematado = true;
}

void lerEntrada(char *entrada, historial* h) { // poñer hist despois, tamén no .h
    fgets(entrada, ENTRADA_MAX, stdin);
    insertHist(entrada, h);
}

void procesarEntrada(char *entrada, historial* h, bool* rematado) {
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
    else if(strcmp(argPpal, "pid") == 0)
        pid(argumentos);
    else if(strcmp(argPpal,"quit") == 0 || strcmp(argPpal,"exit") == 0 || strcmp(argPpal,"bye") == 0) {
        shutDown(rematado);
        free(entrada);
    } else if(strcmp(argPpal,"hist") == 0){
        if(argumentos[0]==NULL)
            imprimirHistorial(*h);
        else if(strcmp(argumentos[0],"-c") == 0)
            borrarHist(h);
        else
            imprimirNComandos(*h, argumentos[0]);
    } else if(strcmp(argPpal,"infosys")==0)
        infosys();
    else
        printf("Comando invalido\n"); // implementar erros?
}