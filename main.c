#include "entradas.h"

// Autor1: Sergio Regueiro Martínez
// Autor2: Daniel Ruibal Balboa

void imprPrompt(){
    printf("$\t");
}

int main(){
    bool rematado = false;
    char* entrada = malloc(100);
    historial h = crearHistBaleiro();
    taboaFicheiros t;

    crearTaboaFich(&t);

    while (!rematado){
        imprPrompt();
        lerEntrada(entrada, &h);
        procesarEntrada(entrada, &h, &rematado, &t);
    }
    return 0;
}
