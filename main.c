#include "entradas.h"

// Ao printf non lle gusta este símbolo
// #define PROMPT_SYMBOL ♦

void imprPrompt(){
    printf("$\t");
}

int main(){
    char* entrada = malloc(100);

    //while (1==1){
    imprPrompt();
    lerEntrada(entrada);
    procesarEntrada(entrada);


    return 0;
}
