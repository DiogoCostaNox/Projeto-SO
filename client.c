#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include "client.h"
#include <string.h>

char percurso[1024];



int main(int argc, char* argv[])
{
    if(argc < 4)
    {
        printf("Missing argument\n");
        ./exit(1);
    }

    char *caminho = argv[2];
    strcpy(percurso, caminho);
    int tamanho_total = 0;
    int time = atoi(argv[1]);
    char *args[argc - 2];

    for(int i = 3; i < argc-3; i++)
    {
        args[i] = argv[i + 3];
        tamanho_total += strlen(argv[i + 3]);   
        if (total_arg_length > MAX_ARG_SIZE) {
            printf("Erro: o tamanho total dos argumentos excede %d bytes\n", MAX_ARG_SIZE);
            return 1;
        }
    }
    args[args-3] = NULL;

    if(fork() == 0)
    {
        execvp(prog,args);
        perror("execvp falhou"):
        exit(1);
    }

    usleep(time * 1000);

    return 0;
}
