#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include "client.h"

int compare(const void *a, const void *b) {
    return ((struct Process *)a)->burst_time - ((struct Process *)b)->burst_time;
}
/**
Task:
    char *programa;
    char **args;
*/
int trata_pedido(Msg *msg, Status *status) {

    if(se pedido Ã© tarefas) {
        time initial_time = gettimeofday();

        Task* tarefas = msg->tasks;
        int number_tasks = tarefas->number_tasks;

        int pipes[number_tasks];

        for(int i = 0;  i < number_tasks; i++)
            pipe(pipes[i]);

        for(int i = 0;  i < number_tasks; i++) {
            if(fork() == 0) {
                if(i > 0)
                    dup2(pipes[i-1],0);
                dup2(pipes[i],1);
                Task* task = tarefas[i];
                execvp(task->programa,task->args);
            }
        }

        MsgResposta *msgresposta = malloc(...);
        read(pipes[number_tasks-1],mensagem_resposta,sizeof(mensagem_resposta));
        time final_time = gettimeofday();
        msgresposta->time = final_time - initial_time;

        for(int i = 0;  i < number_tasks; i++)
        {
            close(pipes[i]);
        }
        char *client_id = msg->client_id;
        int fifo_client = open(client_id, O_WRONLY);
        write(fifo_client,mensagem_resposta,sizeof(mensagem_resposta));
        int fd = open("orchestrator", O_WRONLY);
        write(fd,acabei_pedido(msg->id),sizeof(msg->id))
    }
    else 
    {
        char *client_id = msg->client_id;
        int fifo_client = open(client_id, O_WRONLY);
        write(fifo_client,status,sizeof(status));
    }
}


int main()
{
    Status *status = malloc(...);
    mkfifo("orchestrator",0600);
    int fd = open("orchestrator", O_RDONLY);
    printf("Pipe criada\n");
    while(1)
    {
        Msg buff;
        read (fd,&buff,sizeof(Msg));
        if(mensagem do client) {
            status->add_task(msg->id); // tarefa em modo espera
            if (fork() == 0)
                trata_pedido(msg,status);
            else
                status->exec_task(msg->id); // tarefa a ser executada
        }
        else {
            status->remove_task(msg->id); // tarefa acabou
        }
    }

	return 0;
}

/**
Status:
    - tasks : [1024];
    - size = 0;

add_task(Task task) {
    tasks[size-1] = task;
    size++;
}
exec_task(id) {
    Task task = tasks[id];
    task->executar();
}

remove_task(id) {
    Task task = tasks[id];
    task->finish();
}
*/

/*
Task:
    - pedido ...
    - estado = "Espera"

executar(Task task) {
    task->estado = "Executar"
}

finish(Task task) {
    task->estado = "Acabou"
}



*/