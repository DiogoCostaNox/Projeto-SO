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
        char buffer[1024];
        sprintf(buffer, "%d %s ", mensagem.tempoExp, mensagem.tasks);
        write(fifo_client_orchastrator,buffer,strlen(buffer));

        close(fifo_client_orchastrator);

        fifo_client = open(client_id, O_RDONLY);
        read(fifo_client,resposta,...);
    }
    else if(strcmp(argv[0],"status"))
    {
        Status status[1024];
        
        fifo_client = open(client_id, O_RDONLY);
        read(fifo_client,status,...);
        Waiting *waiting = status->waiting();
        Exec *exec = status->exec();
        Finish *finish = status->finish();
        print("Waiting\n");
        for(int i = 0; i < waiting->size; i++)
            print(....)
        print("Executing\n");
        for(int i = 0; i < exec->size; i++)
            print(....)
        print("Finish\n");
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