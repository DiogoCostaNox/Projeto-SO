#define TAMANHO_ARG 300
#define SERVIDOR 1024
#define MAX 100

typedef struct task {
    char* programa; 
    char* args[TAMANHO_ARG];
} TASK;

typedef struct msg {
    TASK* tasks; 
    int client_id; 
    int id;
    int tempoExp;
} MSG;

typedef struct waitqueue
{
    TASK* tarefa;
    struct waitqueue *next;
} *WAITQUEUE;


typedef struct msgResposta {
    long timeRes;
} MSGRESPOSTA;

typedef struct fila{
    MSG* tarefaZada;
    int inicio, fim;
} FILA;

typedef struct status
{
    MSG waiting[1024];
    MSG running[1024];
    MSG completed[1024];
    int waiting_size;
    int running_size;
    int completed_size;
} STATUS;


