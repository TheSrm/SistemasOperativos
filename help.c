#include "help.h"
#include "definicionsBase.h"

//É moi posible que o que estea facendo sexa un sacrilexio, pero agora non atopo outra forma
void axudaComando(char* cmd){
    int i;
    listaComandos lc = {authorsCmd,pidCmd,chdirCmd,timeCmd,dateCmd,histCmd,comandCmd,
                        openCmd,closeCmd,dupCmd,listopenCmd,infosysCmd,helpCmd,quitCmd,
                        exitCmd,byeCmd};
    if(cmd==NULL)
        for(i=0; i<NUMCOMANDOS; i++)
            printf("Comando %s\n-----\n%s", lc[i].nome, lc[i].descricion);
    else {
        for (i = 0; i < NUMCOMANDOS && strcmp(cmd,lc[i].nome) != 0; i++);
        if(i>=NUMCOMANDOS)
            printf("Non existe comando co nome %s\n",cmd);
        else
            printf("Comando %s\n-----\n%s", lc[i].nome, lc[i].descricion);
    }
}

