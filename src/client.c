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
        char time[20];                            // Da print ao conteudo da mensagem mandada pelo cliente ao servidor 
        sprintf(time, "%d", msg.tempoExp);
        strcat(msgprint,time);
        strcat(msgprint," ");
        strcat(msgprint,msg.tasks.programa);
        for(int j = 0; j < TAMANHO_ARG; j++) {
            strcat(msgprint," ");
            strcat(msgprint,msg.tasks.args[j]);
        }
        strcat(msgprint,"\n");
        write(1,msgprint,sizeof(msgprint));
        strcpy(msgprint,"");
    }
}

int main(int argc, char *argv[]) 
{
    
    
    if(strcmp(argv[1],"execute") == 0) // argv[1] should be used instead of argv[0]
    {
        TASK tarefa;
        tarefa.programa = argv[2]; 
        int i;
        for (i = 3; i < argc && i < TAMANHO_ARG + 3; i++) {
            tarefa.args[i - 3] = argv[i];                   // Vai preencher os argumentos associados ao programa da tarefa em questao
        }
        tarefa.args[i - 3] = NULL;

        for(int i = 0; tarefa.args[i] != NULL; i++) {
            printf("%s\n", tarefa.args[i]);
        }

        tarefa.tipo = 1;

        MSG mensagem;
        mensagem.tasks = tarefa;
        mensagem.client_id = getpid();               // Vai buscar o id do processo que sera usado como id do cliente.
        mensagem.tempoExp = atoi(argv[1]);
        mensagem.tipodepedido = 1;
           
        int fifo_client_orchestrator = open("orchestrator", O_WRONLY); // WRONLY significa que so tem permissao de escrita

        if (fifo_client_orchestrator < 0)
        {
            perror("Erro ao abrir o fifo");
            return 1;
        }

        // Use mensagem.tasks->programa instead of mensagem.tasks
        write(fifo_client_orchestrator,&mensagem,sizeof(mensagem));

        close(fifo_client_orchestrator);

        int fifo_client = open("client_id", O_RDONLY);
    
        char resposta[320];
        read(fifo_client,resposta,320);
        printf("%d",mensagem.tipodepedido);
    }
    else if(strcmp(argv[1],"status") == 0) // argv[1] should be used instead of argv[0] and compare with 0
    {
        STATUS status;
        int fifo_client = open("client_id", O_RDONLY);
        read(fifo_client,&status,sizeof(status));
        char waiting[20] = "Waiting:\n";                            // Vai dar display do estado das tarefas
        char running[20] = "Running:\n";
        char completed[20] = "Completed:\n";
        write(1,waiting,strlen(waiting)); // Use strlen instead of sizeof
        printMsgs(status.waiting_size,status.waiting);
        write(1,running,strlen(running)); // Use strlen instead of sizeof
        printMsgs(status.running_size,status.running);
        write(1,completed,strlen(completed)); // Use strlen instead of sizeof
        printMsgs(status.completed_size,status.completed);
    }
    else
    {
        printf("Comando invalido\n");
    }

    return 0;
}