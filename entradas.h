#ifndef ENTRADAS_H
#define ENTRADAS_H

#include "funcionsTemporais.h"
#include "historial.h"
#include "procesos.h"
#include "autores.h"
#include "ficheiros.h"

void lerEntrada(char* entrada, historial* h);
void procesarEntrada(char* entrada, historial* h, bool* rematado);

#endif