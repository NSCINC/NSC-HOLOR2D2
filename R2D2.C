#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h> // Para simular operações de I/O com sleep

#define NUM_THREADS 4  // Definimos 4 threads para processamento simultâneo

// Estrutura para definir a tarefa de processamento (simulação de SSP - separação de dados)
typedef struct {
    char storage_provider[50];
    char path[256];
    int task_id;
} ProcessingTask;

// Função que cada thread irá executar
void *process_storage_task(void *arg) {
    ProcessingTask *task = (ProcessingTask *)arg;
    printf("Thread %d: Processando dados em %s no caminho %s\n", task->task_id, task->storage_provider, task->path);
    
    // Simular uma tarefa de processamento como I/O, consulta, etc.
    sleep(2);  // Simula o tempo de processamento
    printf("Thread %d: Finalizou o processamento em %s\n", task->task_id, task->storage_provider);
    
    pthread_exit(NULL);
}

int main() {
    pthread_t threads[NUM_THREADS];   // Array de threads
    ProcessingTask tasks[NUM_THREADS]; // Definir as tarefas a serem processadas
    
    // Configurar diferentes tarefas para diferentes provedores de armazenamento
    strcpy(tasks[0].storage_provider, "AWS");
    strcpy(tasks[0].path, "s3://my-bucket/data");
    tasks[0].task_id = 0;
    
    strcpy(tasks[1].storage_provider, "Azure");
    strcpy(tasks[1].path, "azure://my-container/data");
    tasks[1].task_id = 1;
    
    strcpy(tasks[2].storage_provider, "Google Cloud");
    strcpy(tasks[2].path, "gcs://my-bucket/data");
    tasks[2].task_id = 2;
    
    strcpy(tasks[3].storage_provider, "Local");
    strcpy(tasks[3].path, "/local/disk/path");
    tasks[3].task_id = 3;

    // Criar threads para processar as tarefas em paralelo
    for (int i = 0; i < NUM_THREADS; i++) {
        printf("Main: Criando thread %d\n", i);
        int rc = pthread_create(&threads[i], NULL, process_storage_task, (void *)&tasks[i]);
        if (rc) {
            printf("ERRO: Não foi possível criar a thread %d\n", i);
            exit(-1);
        }
    }

    // Esperar que todas as threads finalizem o trabalho
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(threads[i], NULL);
    }

    printf("Main: Finalizou todas as threads.\n");
    pthread_exit(NULL);
}
