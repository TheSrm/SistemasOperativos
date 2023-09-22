#include "funcionsTemporais.h"

void imprData(void){
    time_t tempo;
    struct tm *tempoStruct;
    tempo = time(NULL); // asigna á variable tempo ao tempo actual
    tempoStruct = localtime(&tempo); // transforma o tempo time_t nunha struct coa que é máis fácil traballar
    printf("%d/%d/%d\n",tempoStruct->tm_mday,tempoStruct->tm_mon,tempoStruct->tm_year+1900); // imprime a data collendo os parámetros necesarios da struct
}

void imprHora(){
    time_t now;

    now = time(NULL); // a variable toma o valor do tempo actual

    struct tm *local = localtime(&now); // convírtese o tempo time_t a unha struct cos datos separados

    printf("%02d:%02d:%02d \n", local->tm_hour, local->tm_min, local->tm_sec);
}
