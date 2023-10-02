#ifndef HELP_H
#define HELP_H

#define NUMCOMANDOS 16

typedef struct descrComando{
    char* nome;
    char* descricion;
}descrComando;

void axudaComando(char* cmd);

#endif
