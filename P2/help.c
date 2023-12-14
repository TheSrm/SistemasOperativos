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
    else if (strcmp(cmd,"create")==0)
        printf("Crea un directorio ou un archivo con -f, en ambos casos crearase co nome introducido \n");
    else if (strcmp(cmd,"stat")==0)
        printf("Ofrece as datos sobre un ficheiro, con -long  da mais datos, -long -acc da a fecha de modificación e -long -link, mostra qe ficheiro apunta un link simbolico\n");
    else if (strcmp(cmd,"delete")==0)
        printf("Borra ficheiros ou directorios baleiros indicados polo usuario \n");
    else if (strcmp(cmd,"list")==0)
        printf("Lista os contidos dun directorio\n -hid mostra os ocultos, -recb é recursivo antes, - reca é recursivo despois, - acc, mostra a data de acceso -long mostra datos coma numero de inodo, tamaño, usuario e grupo..., -link mostra a onde apunta o link simbólico \n");
    else if (strcmp(cmd,"deltree")==0)
        printf("Borra directorios o ficheros no vacíos recursivamente\n");
    else if (strcmp(cmd,"mem")==0)
        printf("mem [-blocks|-funcs|-vars|-all|-pmap] ..\tMostra detalles da memoria do proceso\n"
               "\t\t-blocks: os bloques de memoria asignados\n"
               "\t\t-funcs: as direccions de las funcions\n"
               "\t\t-vars: as direccions de las variables\n"
               "\t\t:-all: todo\n"
               "\t\t-pmap: mostra a saida similar á do comando pmap\n");
    else if (strcmp(cmd,"malloc")==0)
        printf("malloc [-free] [tam]\tasigna un bloque memoria de tamano tam con malloc\n"
               "\t-free: desasigna un bloque de memoria de tamano tam asignado con malloc\n");
    else if (strcmp(cmd,"shared")==0)
        printf("hared [-free|-create|-delkey] cl [tam]\tasigna memoria compartida coa clave cl no programa\n"
               "\t-create cl tam: asigna (creando) o bloque de memoria compartida con clave cl e tamano tam\n"
               "\t-free cl: desmapea o bloque de memoria compartida de clave cl\n"
               "\t-delkey cl: elimina del sistema (sin desmapear) a clave de memoria cl\n");
    else if (strcmp(cmd,"mmap")==0)
        printf("mmap [-free] fich prm\tmapea o ficheiro fich con permisos prm\n"
               "\t-free fich: desmapea o ficheiro fich\n");
    else if (strcmp(cmd,"read")==0)
        printf("read fiche addr cont \tLe cont bytes desde fich a direccion addr\n");
    else if (strcmp(cmd,"write")==0)
        printf("write [-o] fiche addr cont \tEscribe cont bytes dende a  direccion addr a fich (-o sobreescribe)\n");
    else if (strcmp(cmd,"memdump")==0)
        printf("memdump addr cont \tMostra na pantallas os contidos (cont bytes) da posicion de memoria addr\n");
    else if (strcmp(cmd,"memfil")==0)
        printf("memfill addr cont byte \tENche a memoria a partir de addr (cont bytes) da posicion de memoria addr\n");
    else if (strcmp(cmd,"recurse")==0)
        printf("recurse [n]\tInvoca a funcion recursiva n veces\n");
    else if (strcmp(cmd,"uid")==0)
        printf("uid [-get|-set] [-l] [id] \t Accede ás credenciais do proceso que executa o shell\n"
               "\t-get: mostra as credenciais"
               "\t-set id: establece a credencial ao valor numerico id\n"
               "\t-set -l id: establece a credencial a login id\n");
    else if (strcmp(cmd,"showvar")==0)
        printf("Mostra o valor dunha variable de entorno e as direccións\n");
    else if (strcmp(cmd,"changevar")==0)
        printf("changevar [-a|-e|-p] var valor\tCambia o valor dunha variable de entorno\n"
               "\t-a: accede polo  terceiro arg de main\n"
               "\t-e: accede mediante environ\n"
               "\t-p: accede mediante putenv\n");
    else if (strcmp(cmd,"subsvar")==0)
        printf("subsvar [-a|-e] var1 var2 valor\tSustitue a variable de entorno var1\n"
               "\tcon var2=valor\n"
               "\t-a: accede polo terceiro arg de main\n"
               "\t-e: accede mediante environ\n");
    else if (strcmp(cmd,"showenv")==0)
        printf("showenv [-environ|-addr] \t Mostra o entorno do proceso\n"
               "\t-environ: accede usando environ (no lugar do terceiro arg de main)\n"
               "\t-addr: mostra o  valor e donde se almacenan environ e o 3er arg main\n ");
    else if (strcmp(cmd,"fork")==0)
        printf("Crea un novo proceso e queda esperando que acabe\n");
    else if (strcmp(cmd,"exec")==0)
        printf("Executa un programa sen crear un novo proceso\n");
    else if (strcmp(cmd,"job")==0)
        printf("Mostra información dun proceso. con -fg pasa a primeiro plano\n");
    else if (strcmp(cmd,"jobs")==0)
        printf("Mostra a informacion dos procesos en segundo plano\n");
    else if (strcmp(cmd,"deljobs")==0)
        printf("Borra da lista de procesos os procesos terminados con -term ou sinalados con -sig\n");
    else
        printf("Ese comando non existe\n");
}

void axudaComando(char* cmd){
    int i;
    char* nomesComandos[NUMCOMANDOS]={"authors","pid","chdir","date","time","hist","comand",
                                      "open","close","dup","listopen","infosys","help","quit",
                                      "exit","bye","create","stat","list","delete","deltree",
                                      "malloc","shared","mmap","read","write","memdump",
                                      "memfill","mem","recurse", "uid","showvar","changevar","subsvar","showenv",
                                      "fork","exec","job","jobs","deljobs"};

    if(cmd==NULL)
        for(i=0; i<NUMCOMANDOS; i++)
            printf("Comando %d: %s\n", i, nomesComandos[i]);
    else {
        imprimirComando(cmd);
    }
}
