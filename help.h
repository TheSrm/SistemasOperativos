#ifndef HELP_H
#define HELP_H

#define NUMCOMANDOS 16

typedef const struct comando{
    char* nome;
    char* descricion;
}comando;

const comando authorsCmd = {.nome = "authors", .descricion = "Sintaxe: authors [-l|-n]\n"
                                                                      "Se non se lle pasa argumento devolve nomes completos e correos de ambos autores.\n"
                                                                      "Con -l só se devolven correos e con -n só nomes\n"};
const comando pidCmd = {.nome = "pid", .descricion = "Sintaxe: pid [-p]\n"
                                                              "Imprime o ID de proceso desta shell. Con -p devolve o do pai do proceso\n"};
const comando chdirCmd = {.nome = "chdir", .descricion = "Sintaxe: chdir [dir]\n"
                                                                  "Cambia o directorio actual ao introducido (dir) se é correcto.\n"
                                                                  "Se non se lle pasa argumento devolve o directorio actual\n"};
const comando dateCmd = {.nome = "date", .descricion = "Sintaxe: date\nDevolve a data actual en formato DD/MM/AAAA\n"};
const comando timeCmd = {.nome = "time", .descricion = "Sintaxe: time\nDevolve o tempo actual en formato HH:MM\n"};
const comando histCmd = {.nome = "hist", .descricion = "Sintaxe: hist [-c|-N]\n"
                                                                "Sen argumentos devolve o historial de comandos desta shell.\n"
                                                                "Co argumento -N, sendo N un enteiro, imprime os primeiros N comandos do historial.\n"
                                                                "Co argumento -c borra o historial. A numeración do historial comeza en 0\n"};
const comando comandCmd = {.nome = "comand", .descricion = "Sintaxe: comand N\n"
                                                           "Repite o comando número N do historial\n"};
const comando openCmd = {.nome = "open", .descricion = "Sintaxe: open [file] mode\n"
                                                       "Abre o ficheiro file no modo dado e inserta nunha lista de abertos. Modos dispoñibles:\n"
                                                       "cr:crear | ap:adxuntar | ex:crear con acceso exclusivo | ro:so lectura\n" // ex é raro, mirar máis tarde
                                                       "rw:lectura e escritura | wo: so escritura | tr: truncar\n"};
const comando closeCmd = {.nome = "close", .descricion = "Sintaxe: close [df]\n"
                                                         "Pecha o ficheiro co descriptor df e eliminao da lista de ficheiros abertos\n"};
const comando dupCmd = {.nome = "dup", .descricion = "Sintaxe: dup [df]\n"
                                                     "Duplica o ficheiro co descriptor df, creando unha nova entrada na lista\n"};
const comando listopenCmd = {.nome = "listopen", .descricion = "Sintaxe: listopen\nImprime unha lista cos ficheiros que a shell ten abertos, o seu modo e descriptor\n"};
const comando infosysCmd = {.nome = "infosys", .descricion = "Sintaxe: infosys\n"
                                                             "Devolve información do sistema no que se executa a shell\n"};
const comando helpCmd = {.nome = "help", .descricion = "Sintaxe: help [cmd]\n"
                                                       "Devolve unha pequena descricion sobre o uso do comando cmd.\n"
                                                       "Se non se pasa nada en cmd, faino para tódolos comandos dispoñibles\n"};
const comando quitCmd = {.nome = "quit", .descricion = "Sintaxe: quit\nSae da consola"};
const comando exitCmd = {.nome = "exit", .descricion = "Sintaxe: exit\nSae da consola"};
const comando byeCmd = {.nome = "bye", .descricion = "Sintaxe: bye\nSae da consola"};

typedef comando listaComandos[NUMCOMANDOS];

void axudaComando(char* cmd);

#endif
