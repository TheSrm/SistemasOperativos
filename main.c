#include "entradas.h"

// Ao printf non lle gusta este símbolo
// #define PROMPT_SYMBOL ♦

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
