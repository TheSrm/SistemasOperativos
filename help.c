#include "help.h"
#include "definicionsBase.h"

void imprimirComando(char* cmd){
    if (strcmp(cmd, "time") == 0)
        printf("Sintaxe: time\nDevolve o tempo actual en formato HH:MM\n");
    else if (strcmp(cmd, "date") == 0)
        printf("Sintaxe: date\nDevolve a data actual en formato DD/MM/AAAA\n");
    else if (strcmp(cmd, "authors") == 0)
        printf("Sintaxe: authors [-l|-n]\nSe non se lle pasa argumento devolve nomes completos e correos de ambos autores.\nCon -l só se imprimen correos e con -n só nomes\n");
    else if (strcmp(cmd, "pid") == 0)
        printf("Sintaxe: pid [-p]\nImprime o ID de proceso desta shell. Con -p devolve o do pai do proceso\n");
    else if (strcmp(cmd, "quit") == 0 || strcmp(cmd, "exit") == 0 || strcmp(cmd, "bye") == 0)
        printf("Sintaxe: quit\t|\texit\t|\tbye\nSae da consola\n");
    else if (strcmp(cmd, "hist") == 0)
        printf("Sintaxe: hist [-c|-N]\nSen argumentos devolve o historial de comandos desta shell.\nCo argumento -N, sendo N un enteiro, imprime os primeiros N comandos do historial.\nCo argumento -c borra o historial. A numeración do historial comeza en 0\n");
    else if (strcmp(cmd, "infosys") == 0)
        printf("Sintaxe: infosys\nDevolve información do sistema no que se executa a shell\n");
    else if (strcmp(cmd, "chdir") == 0)
        printf("Sintaxe: chdir [dir]\nCambia o directorio actual ao introducido (dir) se é correcto.\nSe non se lle pasa argumento devolve o directorio actual\n");
    else if (strcmp(cmd, "listopen") == 0)
        printf("Sintaxe: listopen\nImprime unha lista cos ficheiros que a shell ten abertos, o seu modo e descriptor\n");
    else if (strcmp(cmd, "open") == 0)
        printf("Sintaxe: open [file] mode\nAbre o ficheiro file no modo dado e inserta nunha lista de abertos. Modos dispoñibles:\ncr:crear | ap:adxuntar | ex:acceso exclusivo | ro:so lectura\nrw:lectura e escritura | wo: so escritura | tr: truncar\n");
    else if (strcmp(cmd, "close") == 0)
        printf("Sintaxe: close [df]\nPecha o ficheiro co descriptor df e eliminao da lista de ficheiros abertos\n");
    else if (strcmp(cmd, "dup") == 0)
        printf("Sintaxe: dup [df]\nDuplica o ficheiro co descriptor df, creando unha nova entrada na lista\n");
    else if (strcmp(cmd,"help") == 0)
        printf("Sintaxe: help [cmd]\nDevolve unha pequena descricion sobre o uso do comando cmd.\nSe non se pasa parámetro cmd, devolve unha lista con tódolos comandos dispoñibles\n");
    else if(strcmp(cmd,"comand")==0)
        printf("Sintaxe: comand N\nRepite o comando número N do historial\n");
    else
        printf("Ese comando non existe\n");
}

void axudaComando(char* cmd){
    int i;
    char* nomesComandos[NUMCOMANDOS]={"authors","pid","chdir","date","time","hist","comand",
                                   "open","close","dup","listopen","infosys","help","quit","exit","bye"};
    if(cmd==NULL)
        for(i=0; i<NUMCOMANDOS; i++)
            printf("Comando %d: %s\n", i, nomesComandos[i]);
    else {
        imprimirComando(cmd);
    }
}

