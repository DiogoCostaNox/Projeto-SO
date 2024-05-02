#define TAMANHO_ARG 300
#define SERVIDOR 1024
#define FIFO_FILE "FIFO_CLIENT_ORCHESTRATOR"

struct Msg {
    Task* tasks; 
    char* client_id; 
    int id;
    int tempoExp;
};

struct Task {
    char* programa; 
    char* args[TAMANHO_ARG];
};

struct MsgResposta {
    long timeRes;
};

struct Status {
    void add_task(int task_id) {   
    }

    void exec_task(int task_id) {
    }

    void remove_task(int task_id) {
    }
};

struct Waiting {
    Task* tasks; 
    int size; 
};

struct Exec {
    Task* tasks; 
    int size;
};

struct Finish {
    Task* tasks; 
    int size; 
};


