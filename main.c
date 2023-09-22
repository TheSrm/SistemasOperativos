#include <stdio.h>
#include "entradas.h"

// Ao printf non lle gusta este símbolo
// #define PROMPT_SYMBOL ♦

void imprPrompt(){
    printf("$\t");
}

int main(){
    char* entrada;

    //while (1==1){
    imprPrompt();
    lerEntrada(&entrada);
    procesarEntrada(entrada);


    return 0;
}
