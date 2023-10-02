#include "entradas.h"

void shutDown (bool* rematado){
    *rematado = true;
}

void lerEntrada(char *entrada, historial* h) { // poñer hist despois, tamén no .h
    fgets(entrada, ENTRADA_MAX, stdin);
    if(!insertHist(entrada, h))
        perror("Erro ao insertar o comando no historial");
}
//Repetimos un comando cuxa posición na lista sexa a indicada
void repetirComando(char **argumentos, historial *h, taboaFicheiros *t){
    int repe = atoi(argumentos[0]), i;//Inicializamos variables para comprobar funcionamento correcto
    historial hAux; // hAux: copia estática do historial actual

    if (argumentos[0] == NULL){//Se non hai numero indicado non o podemos facer
        printf("Non hai argumentos suficientes para conseguir o comando pedido\n");//Imprimimos mensaxe de erro e voltamos ao bucle
        return;
    }
    for(hAux=*h, i=0; i<repe && hAux!=HISTNULL; i++, hAux=hAux->nextCmd);//Contamos o numero de comandos que hai no historial, e comprobamos que nunca sexan menos que o numero dado
    if(hAux==HISTNULL){//Se o número dado é maior que o numero de comandos, enton non o podemos realizar
        printf("Non existe comando con tal numero\n");//Imprimimos mensaxe de erro e voltamos ao bulce
        return;
    }
    if(strcmp(hAux->comando, "comand") == 0){//Se o comando cuxo numero corresponde co indicado non debemos facelo, para evitar bucles infinitos
        printf("Para evitar bucles infinitos non se pode chamar a un 'comand' con outro\n");//Imprimimos unha mensaxe indicandoo e voltamos ao bucle
        return;
    }
    procesarEntrada(hAux->comando, h, false, t);//Se ningunha das condicions anteriores fallou, repetimos o comando indicado
}

//Troceamos o comando en argPpal, donde se garda o comando e argumentos[], donde gardaremos os argumentos de cada comando
void procesarEntrada(char *entrada, historial* h, bool* rematado, taboaFicheiros *t) {
    int i;
    char *argPpal, *argumentos[MAXARGS];

    argPpal = strtok(entrada, " \n\t");
    for (i = 0; i < MAXARGS; ++i) {
        argumentos[i] = strtok(NULL, " \n\t");
    }
    if (argPpal != NULL) {
        if (strcmp(argPpal, "time") == 0)
            imprHora();
        else if (strcmp(argPpal, "date") == 0)
            imprData();
        else if (strcmp(argPpal, "authors") == 0)
            authors(argumentos);
        else if (strcmp(argPpal, "pid") == 0)
            pid(argumentos);
        else if (strcmp(argPpal, "quit") == 0 || strcmp(argPpal, "exit") == 0 || strcmp(argPpal, "bye") == 0) {
            shutDown(rematado);
            free(entrada);
            fflush(stdin);
            free(*h);
            pecharTodoFicheiro(t);
        } else if (strcmp(argPpal, "hist") == 0) {
            if (argumentos[0] == NULL)
                imprimirHistorial(*h);
            else if (strcmp(argumentos[0], "-c") == 0)
                borrarHist(h);
            else
                imprimirNComandos(*h, argumentos[0]);
        } else if (strcmp(argPpal, "infosys") == 0)
            infosys();
        else if (strcmp(argPpal, "chdir") == 0)
            cambiarDirectorio(argumentos);
        else if (strcmp(argPpal, "listopen") == 0)
            listarAbertos(*t);
        else if (strcmp(argPpal, "open") == 0)
            Cmd_open(argumentos, t);
        else if (strcmp(argPpal, "close") == 0)
            Cmd_close(argumentos, t);
        else if (strcmp(argPpal, "dup") == 0)
            Cmd_dup(argumentos, t);
        else if (strcmp(argPpal,"help") == 0)
            axudaComando(argumentos[0]);
        else if(strcmp(argPpal,"comand")== 0)
            repetirComando(argumentos, h, t);
        else
            printf("Comando invalido\n");
    }
}
