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

void printMsgs(char* label, int size, MSG *msgs) {
    write(1,label,strlen(label)); 
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

void sendMsg(MSG *mensagem) {
    int fifo_client_orchestrator = open("orchestrator", O_RDWR); 
    write(fifo_client_orchestrator,mensagem,sizeof(*mensagem));
}

void createMsg(MSG *mensagem) {
    mensagem->client_id = getpid();  
    mensagem->tipodepedido = 2;
}

void makeFifo(MSG *mensagem,char *filename) {
    sprintf(filename, "%d", mensagem->client_id);
    mkfifo(filename,0666);
}

void readStatus(STATUS *status,char *filename) {
    int fifo_client = open(filename, O_RDONLY);
    ssize_t bytes_read = read(fifo_client,status,sizeof(STATUS));
    printf("Bytes total: %d\n",bytes_read);
    close(fifo_client);
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
        char filename[20] = " ";
        MSG mensagem;
        STATUS status;
        createMsg(&mensagem);
        makeFifo(&mensagem,filename);
        sendMsg(&mensagem);
        readStatus(&status,filename);
        printf("Recebido: %d %d %d %d\n",status.waiting_size, status.running_size, status.completed_size,sizeof(STATUS));
        printMsgs("Waiting:\n",status.waiting_size,status.waiting);
        printMsgs("Running:\n",status.running_size,status.running);
        printMsgs("Completed:\n",status.completed_size,status.completed);
    }
    else
    {
        printf("Comando invalido\n");
    }

    return 0;
}