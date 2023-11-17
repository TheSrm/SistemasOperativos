#include "entradas.h"

//Función encargada de cerrar a shell, cando o usuario o indique, cambiando o valor de rematado
void shutDown (bool* rematado){
    *rematado = true;
}

//Función encargada de ler a entrada do usuario
void lerEntrada(char *entrada, historial* h) {
    fgets(entrada, ENTRADA_MAX, stdin);//Conseguimos a entrada
    if(!insertHist(entrada, h))//Se non a podemos insertar no historial por erro na función createnode
        printf("Erro ao insertar o comando no historial");//Indicamos o erro e seguimos
}
//Repetimos un comando cuxa posición na lista sexa a indicada
void repetirComando(char **argumentos, historial *h, taboaFicheiros *t, listaBloques *l){
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
    procesarEntrada(hAux->comando, h, false, t, l);//Se ningunha das condicions anteriores fallou, repetimos o comando indicado
}

//Troceamos o comando en argPpal, donde se garda o comando e argumentos[], donde gardaremos os argumentos de cada comando
void procesarEntrada(char *entrada, historial* h, bool* rematado, taboaFicheiros *t, listaBloques *l) {
    int i;//Creamos as variables necesarias para o correcto funcionamento da función
    char *argPpal, *argumentos[MAXARGS];

    argPpal = strtok(entrada, " \n\t");//Dividimos o comando dos argumentos no primeiro espazo, ou senon hai argumentos, no \n ou \t final
    for (i = 0; i < MAXARGS; ++i) {//Dividimos cada un dos argumentos do seguinte no primeiro espazo,  ou senon hai mais argumentos, no \n ou \t final
        argumentos[i] = strtok(NULL, " \n\t");
    }
    //Sempre que exista un argumento principal, pasamolo por este conxunto de ifs, que comparará con cada un dos comandos existentes
    // e executará o correcto, nalguns casos, diferentes argumentos teñen diferentes funcións, polo que tamen compararemos con argumentos
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
            borrarHist(h);
            fflush(stdin);
            free(*h);
            pecharTodoFicheiro(t);
            free(*t);
            pecharTodoBloque(l);
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
            repetirComando(argumentos, h, t, l);
        else if(strcmp(argPpal,"delete")==0)
            borrarFicheiros(argumentos,false);
        else if(strcmp(argPpal,"deltree")==0)
            borrarFicheiros(argumentos,true);
        else if (strcmp(argPpal, "create") == 0)
            create(argumentos);
        else if (strcmp(argPpal, "list") == 0)
            listarFicheiros(argumentos, 0, 0);
        else if (strcmp(argPpal, "stat") == 0)
            stats(argumentos);
        else if (strcmp(argPpal,"malloc")==0)
            memAlloc(l,argumentos);
        else if (strcmp(argPpal,"recurse")==0)
            recurse(argumentos);
        else if (strcmp(argPpal,"read")==0)
            CmdRead(argumentos);
        else if (strcmp(argPpal,"memfill")==0)
            memfill(argumentos);
        else if (strcmp(argPpal,"mem")==0)
            mem(*l);
        else if (strcmp(argPpal,"write")==0)
            CmdWrite(argumentos);
        /*else if (strcmp(argPpal,"shared")==0)
            shared(999,*l); // 999 provisional
        else if (strcmp(argPpal,"mmap")==0)
            mmap(argumentos[0],*l);
        else if (strcmp(argPpal,"read")==0)
            readFile(argumentos[0],*l);
        //
        else if (strcmp(argPpal,"memdump")==0)
            memdump(*l);
        */
        else
            printf("Comando invalido\n");//Se non é ningun dos comandos anteriores é que non existe, polo que mostramos comando inválido e voltamos ao bucle
    }

}