#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define NUM_CORES 4  // Definir número de núcleos (cores) para o kernel gerenciar

// Estrutura de Tarefa do Kernel
typedef struct {
    char provider[50]; // Provedor de armazenamento (AWS, Azure, Google, Local)
    char path[256];    // Caminho no sistema de arquivos
    int task_id;       // ID da tarefa
} KernelTask;

// Estrutura do Kernel Index para Holord2
typedef struct {
    KernelTask *tasks;
    int num_tasks;     // Número de tarefas
    int current_task;  // Tarefa atual sendo processada
    pthread_mutex_t mutex; // Mutex para sincronização
} KernelIndex;

// Função de processamento de tarefa do kernel
void *process_kernel_task(void *arg) {
    KernelIndex *kernel_index = (KernelIndex *)arg;
    
    // Cada thread pega e processa uma tarefa do KernelIndex
    while (1) {
        pthread_mutex_lock(&kernel_index->mutex); // Bloquear a seção crítica

        if (kernel_index->current_task >= kernel_index->num_tasks) {
            pthread_mutex_unlock(&kernel_index->mutex); // Desbloquear antes de sair
            break; // Nenhuma tarefa restante
        }

        int task_index = kernel_index->current_task;
        kernel_index->current_task++;
        pthread_mutex_unlock(&kernel_index->mutex); // Desbloquear após pegar tarefa

        KernelTask task = kernel_index->tasks[task_index];

        // Simular processamento
        printf("Kernel: Processando tarefa %d em %s - caminho: %s\n", 
               task.task_id, task.provider, task.path);
        sleep(1); // Simulação de tempo de processamento

        printf("Kernel: Finalizou tarefa %d\n", task.task_id);
    }

    pthread_exit(NULL);
}

// Função principal do kernel
void run_kernel(KernelTask *tasks, int num_tasks) {
    pthread_t threads[NUM_CORES]; // Threads de processamento
    KernelIndex kernel_index;     // Estrutura de indexação do kernel

    kernel_index.tasks = tasks;
    kernel_index.num_tasks = num_tasks;
    kernel_index.current_task = 0;
    pthread_mutex_init(&kernel_index.mutex, NULL); // Inicializar mutex

    // Criar threads para processar as tarefas
    for (int i = 0; i < NUM_CORES; i++) {
        pthread_create(&threads[i], NULL, process_kernel_task, (void *)&kernel_index);
    }

    // Aguardar que todas as threads terminem
    for (int i = 0; i < NUM_CORES; i++) {
        pthread_join(threads[i], NULL);
    }

    pthread_mutex_destroy(&kernel_index.mutex); // Destruir mutex
    printf("Kernel: Todas as tarefas foram processadas.\n");
}

int main() {
    // Definir tarefas para serem processadas pelo kernel
    KernelTask tasks[8] = {
        {"AWS", "s3://bucket1/data", 1},
        {"Azure", "azure://container/data", 2},
        {"Google", "gcs://bucket2/data", 3},
        {"Local", "/local/disk1/data", 4},
        {"AWS", "s3://bucket2/data", 5},
        {"Azure", "azure://container2/data", 6},
        {"Google", "gcs://bucket3/data", 7},
        {"Local", "/local/disk2/data", 8}
    };

    // Rodar o kernel com as tarefas definidas
    run_kernel(tasks, 8);

    return 0;
}
