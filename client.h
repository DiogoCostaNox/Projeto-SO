#define TAMANHO_ARG 300
#define SERVIDOR 1024
#define FIFO_FILE "FIFO_CLIENT_ORCHESTRATOR"

struct Process {
    int id; // Identificador da tarefa
    int exec_time; // Tempo de execução da tarefa
};
