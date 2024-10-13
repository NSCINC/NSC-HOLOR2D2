#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>

// Estrutura para armazenar a resposta HTTP
struct MemoryStruct {
    char *memory;
    size_t size;
};

// Função de callback para gravar dados da resposta
static size_t WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t realsize = size * nmemb;
    struct MemoryStruct *mem = (struct MemoryStruct *)userp;

    char *ptr = realloc(mem->memory, mem->size + realsize + 1);
    if (ptr == NULL) {
        printf("Não há memória suficiente (realloc falhou)\n");
        return 0;
    }

    mem->memory = ptr;
    memcpy(&(mem->memory[mem->size]), contents, realsize);
    mem->size += realsize;
    mem->memory[mem->size] = 0;

    return realsize;
}

// Função para enviar a requisição HTTP POST
void send_request(const char *message) {
    CURL *curl;
    CURLcode res;

    struct MemoryStruct chunk;
    chunk.memory = malloc(1);
    chunk.size = 0;

    curl_global_init(CURL_GLOBAL_DEFAULT);
    curl = curl_easy_init();

    if (curl) {
        const char *url = "http://localhost:8080/ask";
        curl_easy_setopt(curl, CURLOPT_URL, url);

        // Configurações do POST
        curl_easy_setopt(curl, CURLOPT_POST, 1L);

        // Cabeçalho da requisição
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        // Corpo da requisição
        char json_data[256];
        snprintf(json_data, sizeof(json_data), "{\"message\":\"%s\"}", message);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);

        // Função para capturar a resposta
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        // Enviar a requisição
        res = curl_easy_perform(curl);

        if (res != CURLE_OK) {
            fprintf(stderr, "Erro no cURL: %s\n", curl_easy_strerror(res));
        } else {
            printf("Resposta do servidor: %s\n", chunk.memory);
        }

        // Limpeza
        curl_easy_cleanup(curl);
        curl_slist_free_all(headers);
    }

    curl_global_cleanup();
    free(chunk.memory);
}

int main() {
    char message[100];

    // Simulação da entrada do usuário
    printf("Digite a mensagem a ser enviada: ");
    fgets(message, sizeof(message), stdin);
    message[strcspn(message, "\n")] = 0; // Remover a nova linha

    // Enviar a mensagem
    send_request(message);

    return 0;
}
