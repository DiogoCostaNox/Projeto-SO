#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "client.h"

#define MAX_PROGRAMA 100
#define MAX_ARGS 300

void printMsgs(int size, MSG *msgs) {
    char msgprint[1000] = "";
    for(int i = 0; i < size; i++) {
        MSG msg = msgs[i];
        char time[20];                            // Da print ao conteudo da mensagem mandada pelo cliente ao servidor 
        sprintf(time, "%d", msg.tempoExp);
        strcat(msgprint,time);
        strcat(msgprint," ");
        strcat(msgprint,msg.tasks.programa);
        for(int j = 0; j < TAMANHO_ARG; j++) {
            strcat(msgprint," ");
            strcat(msgprint,&msg.tasks.args[j]);
        }
        strcat(msgprint,"\n");
        write(1,msgprint,sizeof(msgprint));
        strcpy(msgprint,"");
    }
}

int main(int argc, char *argv[]) 
{
    if(strcmp(argv[1],"execute") == 0) 
    {
        TASK tarefa;
        strncpy(tarefa.programa, argv[2], MAX_PROGRAMA);
        tarefa.programa[MAX_PROGRAMA - 1] = '\0'; 

        char args[MAX_ARGS] = "";
        for (int i = 3; i < argc && i < TAMANHO_ARG + 3; i++) {
            strcat(args, argv[i]);
            strcat(args, " ");
        }
        
        args[strlen(args) - 1] = '\0'; 
        strncpy(tarefa.args, args, MAX_ARGS);
        tarefa.args[MAX_ARGS - 1] = '\0';

        tarefa.tipo = 1;

        MSG mensagem;
        mensagem.tasks = tarefa;
        mensagem.client_id = getpid();               
        mensagem.tempoExp = atoi(argv[1]);
        mensagem.tipodepedido = 1;
           
        int fifo_client_orchestrator = open("orchestrator", O_RDWR); 

        if (fifo_client_orchestrator < 0)
        {
            perror("Erro ao abrir o fifo");
            return 1;
        }

        write(fifo_client_orchestrator,&mensagem,sizeof(mensagem));

        close(fifo_client_orchestrator);

        int fifo_client = open("client_id", O_RDONLY);
    
        char resposta[320];
        read(fifo_client,resposta,320);
        printf("%d",mensagem.tipodepedido);
    }
    else if(strcmp(argv[1],"status") == 0) 
    {
        STATUS status;
        int fifo_client = open("client_id", O_RDONLY);
        read(fifo_client,&status,sizeof(status));
        char waiting[20] = "Waiting:\n";                            
        char running[20] = "Running:\n";
        char completed[20] = "Completed:\n";
        write(1,waiting,strlen(waiting)); 
        printMsgs(status.waiting_size,status.waiting);
        write(1,running,strlen(running)); 
        printMsgs(status.running_size,status.running);
        write(1,completed,strlen(completed)); 
        printMsgs(status.completed_size,status.completed);
    }
    else
    {
        printf("Comando invalido\n");
    }

    return 0;
}