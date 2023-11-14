#include "entradas.h"

// Autor1: Sergio Regueiro Martínez
// Autor2: Daniel Ruibal Balboa

void imprPrompt(){
    printf("$:- ");
}

int main(){
    bool rematado = false;
    char* entrada = malloc(256);
    historial h = crearHistBaleiro();
    taboaFicheiros t;
    listaBloques l = crearTaboaBloques();

    crearTaboaFich(&t);

    while (!rematado){
        imprPrompt();
        lerEntrada(entrada, &h);
        procesarEntrada(entrada, &h, &rematado, &t,l);
    }
    return 0;
}
