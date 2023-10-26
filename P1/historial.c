#include "historial.h"

historial crearHistBaleiro(){
    return HISTNULL;
}

bool crearNodo(histNode **Pnodo){
    *Pnodo = malloc(sizeof(histNode));
    return (*Pnodo!=NULL);
}

bool insertHist(char* entrada, historial* h){
    historial hCnt, hAux;
    if(!crearNodo(&hAux))
        return false;
    strcpy(hAux->comando,entrada);
    hAux->nextCmd=HISTNULL;
    if(*h==HISTNULL)
        *h = hAux;
    else{
        for (hCnt = *h; hCnt->nextCmd!=HISTNULL; hCnt=hCnt->nextCmd);
        hCnt->nextCmd=hAux;
    }
    return true;
}

void borrarHist(historial* h){
    historial hAux;
    while(*h != HISTNULL){
        hAux = *h;
        *h = hAux->nextCmd;
        free(hAux);
    }
}

void imprimirHistorial(historial h){
    int cmdCnt=0;
    for(; h != HISTNULL; h=h->nextCmd, cmdCnt++)
        printf("Comando %d: %s\n",cmdCnt,h->comando);
}

int intPotencia(int base, int exp){
    if(exp == 0)
        return 1;
    else if(exp == 1)
        return base;
    else
        return intPotencia(base, exp / 2) * intPotencia(base, exp / 2) * intPotencia(base, exp%2);
}

int enteiroDeComando(char* s){
    int enteiro = 0, i, nInts; // se o enteiroProba dá ben, usalo
    char* sAux;
    sAux = strtok(s,"-");
    for(i = 0; sAux[i]!='\0'; i++);
    --i;
    nInts=i;
    for(; i>=0; i--){
        if(sAux[i]<48 || sAux[i]>57)
            return -1;
        else
            enteiro += ((sAux[i]-48) * intPotencia(10, i - nInts));
    }
    return enteiro;
}

void imprimirNComandos(historial h, char* numCmds){
    int cmdCnt=0, N = enteiroDeComando(numCmds)-1;
    if(N<=-1)
        printf("Argumento inválido\n");
    else {
        for (; h != HISTNULL && cmdCnt <= N; h = h->nextCmd, cmdCnt++)
            printf("Comando %d: %s\n", cmdCnt, h->comando);
        if (cmdCnt < N)
            printf("O historial non ten tantos comandos coma os indicados\n");
    }
}