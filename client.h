#define TAMANHO_ARG 300
#define SERVIDOR 1024
#define MAX 100

typedef struct msg {
    TASK* tasks; 
    char* client_id; 
    int id;
    int tempoExp;
} MSG;

typedef struct waitqueue
{
    TASK* tarefa;
    struct waitqueue *next;
} *WAITQUEUE;

typedef struct task {
    char* programa; 
    char* args[TAMANHO_ARG];
} TASK;

typedef struct msgResposta {
    long timeRes;
} MSGRESPOSTA;

typedef struct {
    TASK* tarefaZada;
    int inicio, fim;
} Fila;


