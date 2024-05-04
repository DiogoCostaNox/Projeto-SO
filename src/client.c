#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "client.h"

void printMsgs(int size, MSG *msgs) {
    char msgprint[1000] = "";
    for(int i = 0; i < size; i++) {
        MSG msg = msgs[i];
        char time[20];
        sprintf(time, "%d", msg.tempoExp);
        msg.tempoExp;
        strcat(msgprint,time);
        strcat(msgprint," ");
        strcat(msgprint,msg.tasks->programa);
        for(int j = 0; j < TAMANHO_ARG; j++) {
            strcat(msgprint," ");
            strcat(msgprint,msg.tasks->args[j]);
        }
        strcat("\n");
        write(1,msgprint,sizeof(msgprint));
        msgprint = ""
    }
}

int main(int argc, char *argv[]) 
{
    char* client_id = "";
    mkfifo(client_id,0666);

    if(strcmp(argv[0],"execute"))   
    {
        struct Task tarefa;
        tarefa.programa = argv[2]; 
        for (int i = 3; i < argc && i < TAMANHO_ARG + 3; i++) {
            tarefa.args[i - 3] = argv[i];
        }
        tarefa.args[i - 3] = NULL; 

        struct Msg mensagem;
        mensagem.tasks = tarefa;
        mensagem.client_id = "id_do_cliente";
        mensagem.id = 1;
        mensagem.tempoExp = argv[1];
           
        int fifo_client_orchestrator = open("orchestrator",0600);

        if (fifo_client_orchestrator < 0)
        {
            perror("Erro ao abrir o fifo");
            return 1;
        }

        int pid = getpid();
        char buffer[320];
        sprintf(buffer, "%d %d %s ", mensagem.tempoExp, mensagem.id, mensagem.tasks);
        write(fifo_client_orchestrator,buffer,strlen(buffer));

        close(fifo_client_orchestrator);

        fifo_client = open(client_id, O_RDONLY);

        read(fifo_client,resposta,...);
    }
    else if(strcmp(argv[0],"status"))
    {
        STATUS status;
        
        fifo_client = open(client_id, O_RDONLY);
        read(fifo_client,status,...);
        char waiting[20] = "Waiting:\n";
        char running[20] = "Running:\n";
        char completed[20] = "Completed:\n";
        write(1,waiting,sizeof(waiting))
        printMsgs(status.waiting_size,status.waiting);
        write(1,waiting,sizeof(running))
        printMsgs(status.running_size,status.running);
        write(1,waiting,sizeof(completed))
        printMsgs(status.completed_size,status.completed);
    }

    return 0;
}