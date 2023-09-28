#ifndef HELP_H
#define HELP_H

#define NUMCOMANDOS 16

typedef struct comando{
    char* sintaxe;
    char* descricion;
}comando;

comando authors = {.sintaxe = "", .descricion = ""};
comando pid = {.sintaxe = "", .descricion = ""};
comando chdir = {.sintaxe = "", .descricion = ""};
comando date = {.sintaxe = "", .descricion = ""};
comando time = {.sintaxe = "", .descricion = ""};
comando hist = {.sintaxe = "", .descricion = ""};
comando comand = {.sintaxe = "", .descricion = ""};
comando open = {.sintaxe = "", .descricion = ""};
comando close = {.sintaxe = "", .descricion = ""};
comando dup = {.sintaxe = "", .descricion = ""};
comando listopen = {.sintaxe = "", .descricion = ""};
comando infosys = {.sintaxe = "", .descricion = ""};
comando help = {.sintaxe = "", .descricion = ""};
comando quit = {.sintaxe = "quit", .descricion = "Sae da consola"};
comando exitCmd = {.sintaxe = "exit", .descricion = "Sae da consola"};
comando bye = {.sintaxe = "bye", .descricion = "Sae da consola"};

#endif
