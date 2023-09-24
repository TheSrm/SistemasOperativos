#ifndef HISTORIAL_H
#define HISTORIAL_H

#include "definicionsBase.h"

#define ENTRADA_MAX 50
#define HISTNULL NULL

typedef struct hNode{
    char comando[ENTRADA_MAX];
    struct hNode* nextCmd;
}histNode;

typedef histNode* historial;

historial crearHistBaleiro();
void borrarHist(historial* h);
bool insertHist(char* entrada, historial* h);
void imprimirHistorial(historial h);
void imprimirNComandos(historial h, char* numCmds);

#endif