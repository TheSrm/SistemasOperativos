#include "entradas.h"

// Autor1: Sergio Regueiro Martínez
// Autor2: Daniel Ruibal Balboa

void imprPrompt(){
    printf("$:- ");
}

int main(int argc, char *argv[], char *envp[]){
    bool rematado = false;
    char* entrada = malloc(256);
    historial h = crearHistBaleiro();
    taboaFicheiros t;
    listaBloques l;
    listaProcesos p;

    crearListaProcesos(&p);
    crearTaboaFich(&t);
    crearTaboaBloques(&l);

    while (!rematado){
        imprPrompt();
        lerEntrada(entrada, &h);
        procesarEntrada(entrada, &h, &rematado, &t,&l,&p,envp);
    }
    return 0;
}
