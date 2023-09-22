#include "historial.h"

historial crearHistBaleiro(){
    return HISTNULL;
}

void insertHist(char* entrada, historial* h){
    historial hAux = malloc(sizeof(historial)), hCnt;
    strcpy(hAux->comando,entrada);
    hAux->next=HISTNULL;
    if(h==HISTNULL)
        *h = hAux;
    else{
        for (hCnt = *h; hCnt->next!=HISTNULL ; hCnt=hCnt->next);
        hCnt->next=hAux;
    }
}

void borrarHist(historial* h){

}