#ifndef HELP_H
#define HELP_H

#define NUMCOMANDOS 40

typedef struct descrComando{
    char* nome;
    char* descricion;
}descrComando;

void axudaComando(char* cmd);

#endif
