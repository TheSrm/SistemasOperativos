#include <string.h>
#include "historial.h"

void leerEntrada(char *entrada, historial h) {
    fgets(entrada, ENTRADA_MAX, stdin);
    insertHist(&h);
}

void procesarEntrada(char *entrada) {
    //char *Trozos[3];
    //if ((TrocearCadena(entrada,  Trozos)) == 0) {
    if (strcmp(entrada, "time") == 0) {
        imprHora();
    }
    else {
        imprData();
    }
}