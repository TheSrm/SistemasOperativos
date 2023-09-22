#include "entradas.h"
#include <stdbool.h>

// Ao printf non lle gusta este símbolo
// #define PROMPT_SYMBOL ♦

void imprPrompt(){
    printf("$\t");
}

int main(){
    bool rematado = false;
    char* entrada = malloc(100);

    while (!rematado){
        imprPrompt();
        lerEntrada(entrada);
        procesarEntrada(entrada);
    }

    return 0;
}
