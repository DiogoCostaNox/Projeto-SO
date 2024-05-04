<#include <stdio.h>
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

void criarFila(FILA* f) {
    f->inicio = f->fim = 0;
}

void addFila(FILA* f, TASK t) {
    if((f->fim + 1) % MAX == f->inicio) {
        printf("Fila cheia!\n");
        return;
    }
    f->tarefas[f->fim] = t;
    f->fim = (f->fim + 1) % MAX;
}

void removeFila(FILA* f) {
    if(f->inicio == f->fim) {
        printf("Fila vazia!\n");
        exit(1); // Fila vazia
    }
    TASK t = f->tarefas[f->inicio];
    f->inicio = (f->inicio + 1) % MAX;
    return t;
}

void print(char *id, time initial_time, time final_time) {
    char msg[70] = "";
    int aux= final_time - initial_time;
    char sub[20];
    sprintf(sub, "%d", aux);
    int file = open("endtasks","r");
    strcat(msg, id);
    strcat(msg, ":");
    strcat(msg, sub);
    strcat(msg, "\n");
    // id1: final_time1 - initial_time1
    // id2: final_time2 - initial_time2
    // id3: final_time3 - initial_time3
    write(file,msg,sizeof(msg));
    close(file);
}

int trata_pedido(MSG *msg, Status *status) {

    if(se pedido Ã© tarefas) {
        char filename[20];
        sprintf(filename, "%d", msg->client_id);
        int output = open(filename,"w");
        dup2(1,output);

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

        MSGRESPOSTA *msgresposta = malloc(...);
        read(pipes[number_tasks-1],mensagem_resposta,sizeof(mensagem_resposta));
        time final_time = gettimeofday();
        msgresposta->time = final_time - initial_time;

        for(int i = 0;  i < number_tasks; i++)
        {
            close(pipes[i]);
        }
        int fifo_client = open(filename, O_WRONLY);
        write(fifo_client,mensagem_resposta,sizeof(mensagem_resposta));
        
        int fd = open("orchestrator", O_WRONLY);
        write(fd,acabei_pedido(msg->id),sizeof(msg->id));
        print(filename,initial_time,final_time);
        close(output);
    }
    else 
    {
        char *client_id = msg->client_id;
        int fifo_client = open(client_id, O_WRONLY);
        write(fifo_client,status,sizeof(status));
    }
}

void add_task(STATUS status,MSG msg) {
    status.waiting[status.waiting_size] = msg;
    status.waiting_size++;
}

// removes a mensagem da fila de espera
void exec_task(STATUS status,int msg_id) {
    int i = 0;
    for(; i < status.waiting_size; i++) {
        if(status.waiting[i].client_id = msg_id)
            break;
    }
    status.running[i] = status.waiting[i];
    for(; i < status.waiting_size-1; i++) {
        status.waiting[i] = status.waiting[i+1];
    }
    status.waiting_size--;
    status.running_size++;
}

void end_task(STATUS status,int msg_id) {
    int i = 0;
    for(; i < status.running_size; i++) {
        if(status.running[i].client_id = msg_id)
            break;
    }
    status.completed[i] = status.running[i];
    for(; i < status.running_size-1; i++) {
        status.running[i] = status.running[i+1];
    }
    status.running_size--;
    status.completed_size++;
}

int executar_pedido(STATUS status,MSG buff) {
    exec_task(status,buff);
    if (fork() == 0)
        trata_pedido(msg,status);
}

int main()
{
    int limite = 0; // ler dos argumentos
    STATUS status;
    status.waiting_size = 0;
    status.running_size = 0;
    status.completed_size = 0;
    mkfifo("orchestrator",0600);
    int fd = open("orchestrator", O_RDONLY);
    printf("Pipe criada\n");
    while(1)
    {
        MSG buff;
        read (fd,&buff,sizeof(buff));
        if(mensagem do client) {
            add_task(status,buff);
            addFila(...)
            if(status.running_size < limite) 
                executar_pedido(status,buff);
        }
        else {
            end_task(status,buff->id); // tarefa acabou
            MSG remove = removeFila(fila);
            executar_pedido(status,remove);
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

=============================
cria processo
$ ps -a
- pid    runnning
depois de ele acabar
$ ps -a
- pid    zombie
wait()
$ ps -a
- pid não aparece
*/