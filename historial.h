#ifndef HISTORIAL_H
#define HISTORIAL_H

#include <stdlib.h>

#define ENTRADA_MAX 50
#define HISTNULL NULL


typedef struct historial* histP;

typedef struct historial{
    char comando[ENTRADA_MAX];
    histP prev;
} hist;



void createHist(histP h){
    h = HISTNULL;
}

#endif