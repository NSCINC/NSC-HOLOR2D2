#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

// Estrutura para armazenar dados de resposta do chat
typedef struct {
    int id;
    char question[256];
    char answer[256];
} ChatResponse;

// Função para inicializar o banco de dados e criar a tabela Responses
int initializeDatabase(sqlite3 **db) {
    int rc = sqlite3_open("ApolloHoloFi.sqlite", db);
    if (rc) {
        fprintf(stderr, "Erro ao abrir o banco de dados: %s\n", sqlite3_errmsg(*db));
        return rc;
    }

    const char *createTableString = 
        "CREATE TABLE IF NOT EXISTS Responses ("
        "id INTEGER PRIMARY KEY AUTOINCREMENT,"
        "question TEXT NOT NULL,"
        "answer TEXT NOT NULL);";

    char *errMsg;
    rc = sqlite3_exec(*db, createTableString, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Erro ao criar a tabela: %s\n", errMsg);
        sqlite3_free(errMsg);
        return rc;
    }

    printf("Tabela Responses criada ou já existe.\n");
    return SQLITE_OK;
}

// Função para inserir uma resposta de chat no banco de dados
void insertResponse(sqlite3 *db, ChatResponse response) {
    const char *insertStatementString = 
        "INSERT INTO Responses (question, answer) VALUES (?, ?);";
    sqlite3_stmt *stmt;

    if (sqlite3_prepare_v2(db, insertStatementString, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "Erro ao preparar a declaração: %s\n", sqlite3_errmsg(db));
        return;
    }

    sqlite3_bind_text(stmt, 1, response.question, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, response.answer, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_DONE) {
        printf("Resposta inserida com sucesso.\n");
    } else {
        printf("Erro ao inserir resposta: %s\n", sqlite3_errmsg(db));
    }

    sqlite3_finalize(stmt);
}

// Função para ler uma resposta com base na pergunta
char* getResponse(sqlite3 *db, const char *question) {
    const char *queryStatementString = "SELECT answer FROM Responses WHERE question = ?;";
    sqlite3_stmt *stmt;
    char *answer = NULL;

    if (sqlite3_prepare_v2(db, queryStatementString, -1, &stmt, 0) != SQLITE_OK) {
        fprintf(stderr, "Erro ao preparar a declaração: %s\n", sqlite3_errmsg(db));
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, question, -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) == SQLITE_ROW) {
        answer = strdup((const char *)sqlite3_column_text(stmt, 0));
    } else {
        printf("Nenhuma resposta encontrada para a pergunta.\n");
    }

    sqlite3_finalize(stmt);
    return answer;
}

// Função principal para executar a aplicação
int main() {
    sqlite3 *db;
    if (initializeDatabase(&db) != SQLITE_OK) {
        return EXIT_FAILURE;
    }

    // Inserir respostas de exemplo
    ChatResponse response1 = {0, "What is HoloFi?", "HoloFi is a blockchain-based platform."};
    ChatResponse response2 = {0, "How does it work?", "It uses smart contracts for transactions."};
    insertResponse(db, response1);
    insertResponse(db, response2);

    // Exemplo de consulta a uma resposta
    char *response = getResponse(db, "What is HoloFi?");
    if (response != NULL) {
        printf("Chatbot: %s\n", response);
        free(response); // Libere a memória alocada
    } else {
        printf("Chatbot: I don't know the answer to that.\n");
    }

    // Fechar o banco de dados
    sqlite3_close(db);
    return EXIT_SUCCESS;
}
