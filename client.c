#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "client.h"

int main(int argc, char *argv[]) 
{
    char* client_id = "";
    mkfifo(client_id,0600);
    if(strcmp(argv[0],"execute"))
    {
        /*
        Cria tarefa
        Task task;
        task->id = id;
        task->id_client = client_id;
        task->pedido = pedido;
        task->estado = "Espera";
        */
        if (argc < 3) 
        {
            printf("Uso: %s <tempo> <programa> [args]\n", argv[0]);
            return 1;
        }

        int fifo_client_orchestrator = open("orchestrator",0600);

        if (fifo_client_orchestrator < 0)
        {
            perror("Erro ao abrir o fifo");
            return 1;
        }

        int pid = getpid();
        char buffer[1024];
        sprintf(buffer, "%d %s %s", tempo_exec, programa, args);
        msg->client_id = client_id;
        write(fifo_client_orchastrator,buffer,strlen(buffer));

        close(fifo_client_orchastrator);

        fifo_client = open(client_id, O_RDONLY);
        read(fifo_client,resposta,...);
    }
    else if(strcmp(argv[0],"status"))
    {
        Status *status = ...;
        
        fifo_client = open(client_id, O_RDONLY);
        read(fifo_client,status,...);
        Waiting *waiting = status->waiting();
        Exec *exec = status->exec();
        Finish *finish = status->finish();
        print("Waiting\n");
        for(int i = 0; i < waiting->size; i++)
            print(....)
        print("Waiting\n");
        for(int i = 0; i < exec->size; i++)
            print(....)
        print("Waiting\n");
        for(int i = 0; i < finish->size; i++)
            print(....)

        
    }

    return 0;
}
/**
orchestrator
clientes -> servidor

0:
servidor -> client 0
1:
servidor -> client 1
2:
servidor -> client 2
3:
servidor -> client 3


*/