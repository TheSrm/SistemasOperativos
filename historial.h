#ifndef HISTORIAL_H
#define HISTORIAL_H

#include <stdlib.h>
#include <string.h>

#define ENTRADA_MAX 50
#define HISTNULL NULL

typedef struct hNode{
    char comando[ENTRADA_MAX];
    struct hNode* next;
};

typedef struct hNode* historial;

historial crearHistBaleiro();
void borrarHist(historial* h);
void insertHist(char* entrada, historial* h);

#endif