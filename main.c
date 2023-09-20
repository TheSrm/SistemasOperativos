#include <stdio.h>
#include "entradas.h"

// Ao printf non lle gusta este símbolo
// #define PROMPT_SYMBOL ♦

//top lonxitude max de comando: 14

void imprPrompt(){
    printf("$\t");
}

int main(){
    char* entrada;
    //while (1==1){
    imprPrompt();
    lerEntrada(entrada);
    procesarEntrada(entrada);


    return 0;
}
