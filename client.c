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
    if(strcmp(argv[0],"execute"))
    {
        if (argc < 3) 
        {
            printf("Uso: %s <tempo> <programa> [args]\n", argv[0]);
            return 1;
        }

        int tempo_exec = atoi(argv[1]);
        char *programa = argv[2];
        char *args = "";

        for (int i = 3; i < argc; i++) {
            strcat(args, argv[i]);
            strcat(args, " ");
        }

        mkfifo(FIFO_FILE, 0666);

        int fifo_client_orchestrator = open(FIFO_FILE)

        if (fifo_client_orchatstrator < 0)
        {
            perror("Erro ao abrir o fifo");
            return 1;
        }

        char buffer[1024];
        sprintf(buffer, "%d %s %s", tempo_exec, programa, args);
        write(fifo_client_orchastrator,buffer,strlen(buffer));

        close(fifo_client_orchastrator);

        fifo_client_orchastrator = open(FIFO_FILE, O_RDONLY);
        if (fifo_client_orchastrator < 0) {
            perror("Erro ao abrir o pipe com nome");
            return 1;
        }

        char id[256];
        read(fifo_client_orchastrator, id, 256);
        printf("Identificador da tarefa: %s\n", id);

        close(fifo_client_orchastrator);
    }

    return 0;
}