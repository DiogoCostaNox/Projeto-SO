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
    char filename[20] = "output";
    sprintf(filename, "%d", msg->client_id);
    int fifo_client = open(filename, O_WRONLY);

    if(msg->tipodepedido == 1) 
    {
        printf("Pedido reconhecido como tipo 1\n");
        int output = open(filename, O_WRONLY | O_CREAT, 0644);
        if (output < 0) {
            perror("open");
            return -1;
        }
        dup2(output, 1);
        
        
        
        time_t initial_time = time(NULL);

        if(fork() == 0) {
            TASK tarefa = msg->tasks;
            printf("execvp(%s)",tarefa.programa);
            execvp(tarefa.programa,tarefa.args);
            exit(0);
        }

        int status;
        wait(&status);

        MSGRESPOSTA msgresposta;
        read(output,&msgresposta,sizeof(msgresposta));
        time_t final_time = time(NULL);
        msgresposta.timeRes = final_time - initial_time;

        write(fifo_client,&msgresposta,sizeof(msgresposta));

        int fd = open("orchestrator", O_WRONLY);
        write(fd,"pedido finalizado",sizeof(msg->id));
        print(filename,initial_time,final_time);
    
        close(output);
    }
    else {
        write(fifo_client,status,sizeof(*status));
    }
    
    close(fifo_client);
    return 0;
}

void add_task(STATUS *status,MSG msg) {
    status->waiting[status->waiting_size] = msg;
    status->waiting_size++;
}

// removes a mensagem da fila de espera
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
}

void executar_pedido(STATUS *status,MSG buff) {
    exec_task(status,buff.client_id);
    if (fork() == 0)
    {
        trata_pedido(&buff,status);
    }
        
}

int main()
{
     // ler dos argumentos
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
         // le a mensagem com a tarefa
        if(buff.tipodepedido != 0) {
            add_task(&status,buff);
            printf("Pedido adicionado\n");
            executar_pedido(&status,buff);
            end_task(&status,buff.client_id);
            
                
        }
        close(fd);
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