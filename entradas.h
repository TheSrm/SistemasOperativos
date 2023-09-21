#ifndef ENTRADAS_H
#define ENTRADAS_H

#define MAXARGS 3

#include "funcionsTemporais.h"
#include "historial.h"
#include <string.h>

void lerEntrada(char* entrada, historial h);
void procesarEntrada(char* entrada);

#endif