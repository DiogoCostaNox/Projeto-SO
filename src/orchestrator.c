#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <unistd.h>
#include "client.h"

#define MAX_PROGRAMA 100
#define MAX_ARGS 300

// int compare(const void *a, const void *b) {
//     return ((struct Process*)a)->burst_time - ((struct Process*)b)->burst_time;
// }

// void criarFila(FILA* f) {
//     f->inicio = f->fim = 0;
// }

// void addFila(FILA* f, TASK t) {
//     if((f->fim + 1) % MAX == f->inicio) {
//         printf("Fila cheia!\n");
//         return;
//     }
//     f->tarefaZada->tasks[f->fim] = t;
//     f->fim = (f->fim + 1) % MAX;
// }

// TASK removeFila(FILA* f) {
//     if(f->inicio == f->fim) {
//         printf("Fila vazia!\n");
//         exit(1); // Fila vazia
//     }
//     TASK t = f->tarefaZada->tasks[f->inicio];
//     f->inicio = (f->inicio + 1) % MAX;
//     return t;
// }

void print(char *id, time_t initial_time, time_t final_time) {
    char msg[70] = "";
    int aux = final_time - initial_time;
    char sub[20];
    sprintf(sub, "%d", aux);
    int file = open("endtasks", O_WRONLY | O_APPEND | O_CREAT, 0644);
    if (file < 0) {
        perror("open");
        return;
    }
    strcat(msg, id);
    strcat(msg, ":");
    strcat(msg, sub);
    strcat(msg, "\n");
    if (write(file, msg, strlen(msg)) < 0) {
        perror("write");
    }
    close(file);
    // id1: final_time1 - initial_time1
    // id2: final_time2 - initial_time2
    // id3: final_time3 - initial_time3
}

int trata_pedido(MSG *msg, STATUS *status) {
    char filename[20] = " ";
    sprintf(filename, "%d", msg->client_id);
    TASK tarefa = msg->tasks;

    int fifo_client = open(filename, O_WRONLY);

    if(msg->tipodepedido == 1) 
    {
        printf("Pedido reconhecido como tipo 1\n");
        
        // if (fifo_client < 0) {
        //     perror("open");
        //     printf("open error\n"); 
        //     return -1;
        // }
        // dup2(fifo_client, 1);
        
        time_t initial_time = time(NULL);

        if(fork() == 0) {
            printf("Programa: %s\n",tarefa.programa);
            printf("execvp(%s)\n",tarefa.programa);
            int arg_count = 1;  // Start at 1 for the program name
            for (char *p = tarefa.args; *p != '\0'; p++)
                if (*p == ' ') arg_count++;
            // Allocate the args array
            char **args = malloc((arg_count + 2) * sizeof(char *));  // +2 for the program name and the NULL terminator
            if (args == NULL) {
                perror("malloc failed");
                exit(1);
            }
            // Split tarefa.args into separate arguments
            args[0] = tarefa.programa;
            char *token = strtok(tarefa.args, " ");
            for (int i = 1; i <= arg_count; i++) {
                args[i] = token;
                token = strtok(NULL, " ");
            }
            args[arg_count + 1] = NULL;
            // Execute the program
            execvp(tarefa.programa, args);
            // Free the args array
            free(args);
            exit(0);
        }

        int status;
        wait(&status);
        time_t final_time = time(NULL);
        print(filename,initial_time,final_time);
    }
    else {
        printf("status: %d %d %d %d\n",status->waiting_size, status->running_size, status->completed_size,sizeof(STATUS));
        ssize_t bytes_read = write(fifo_client,status,sizeof(STATUS));
        write(1,"Mandou\n",strlen("Mandou\n"));
    }
    close(fifo_client);
    int fd = open("orchestrator", O_WRONLY);
    msg->tipodepedido=3;
    write(fd,msg,sizeof(*msg));
    close(fd);
    return 0;
}

void add_task(STATUS *status,MSG msg) {
    status->waiting[status->waiting_size] = msg;
    status->waiting_size++;
    printf("1: %d %d %d\n",status->waiting_size, status->running_size, status->completed_size);
}

void exec_task(STATUS *status,int msg_id) {
    int i = 0;
    for(; i < status->waiting_size; i++) {
        if(status->waiting[i].client_id == msg_id)
            break;
    }
    status->running[i] = status->waiting[i];
    for(; i < status->waiting_size-1; i++) {
        status->waiting[i] = status->waiting[i+1];
    }
    status->waiting_size--;
    status->running_size++;
    printf("2: %d %d %d\n",status->waiting_size, status->running_size, status->completed_size);
}

void end_task(STATUS *status,int msg_id) {
    int i = 0;
    for(; i < status->running_size; i++) {
        if(status->running[i].client_id == msg_id)
            break;
    }
    status->completed[i] = status->running[i];
    for(; i < status->running_size-1; i++) {
        status->running[i] = status->running[i+1];
    }
    status->running_size--;
    status->completed_size++;
    printf("3: %d %d %d\n",status->waiting_size, status->running_size, status->completed_size);
}

void executar_pedido(STATUS *status,MSG buff) {
    exec_task(status,buff.client_id);
    if (fork() == 0)
    {
        TASK tarefa = buff.tasks;
        printf("Executando pedido\n");
        printf("%d",tarefa.tipo);
        trata_pedido(&buff,status);
    }
        
}

int main(int argc, char *argv[])
{
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
        buff.tipodepedido = 0;
        read (fd,&buff,sizeof(buff));
        if(buff.tipodepedido != 0) {
            if(buff.tipodepedido == 3)
                end_task(&status,buff.client_id);
            else {
                add_task(&status,buff);
                printf("Pedido adicionado\n");
                executar_pedido(&status,buff);
            }
        }
    }
    close(fd);
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