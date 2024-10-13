#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

#define MAX_INVESTORS 100 // Definindo um limite para o número de investidores

typedef struct {
    char *investors[MAX_INVESTORS];
    int authorizedInvestors[MAX_INVESTORS]; // 1 para autorizado, 0 para não autorizado
    long balances[MAX_INVESTORS]; // Saldo de cada investidor
    long investedAmount[MAX_INVESTORS]; // Quantidade investida por cada investidor
    int investorCount; // Contagem de investidores
} InvestmentContract;

// Callback para manipular resultados da consulta SQL
static int callback(void *data, int argc, char **argv, char **azColName) {
    InvestmentContract *contract = (InvestmentContract *)data;
    if (contract->investorCount >= MAX_INVESTORS) {
        printf("Max investors reached.\n");
        return 0; // Para não exceder o limite
    }
    // Armazenar dados do investidor
    contract->investors[contract->investorCount] = strdup(argv[0]); // Nome do investidor
    contract->authorizedInvestors[contract->investorCount] = atoi(argv[1]); // Autorização
    contract->balances[contract->investorCount] = atol(argv[2]); // Saldo
    contract->investedAmount[contract->investorCount] = 0; // Investimento inicial
    contract->investorCount++;
    return 0;
}

// Função para conectar ao banco de dados e inicializar investidores
void initializeContract(InvestmentContract *contract) {
    contract->investorCount = 0;

    sqlite3 *db;
    char *errMsg = 0;
    
    // Abrir (ou criar) o banco de dados
    if (sqlite3_open("investors.db", &db)) {
        fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
        return;
    }

    // Criar a tabela de investidores se não existir
    const char *sqlCreateTable = "CREATE TABLE IF NOT EXISTS Investors (Name TEXT, Authorized INT, Balance LONG);";
    if (sqlite3_exec(db, sqlCreateTable, 0, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    // Inserir investidores fictícios
    const char *sqlInsert = "INSERT INTO Investors (Name, Authorized, Balance) VALUES ('Alice', 1, 1000), ('Bob', 0, 500), ('Charlie', 1, 1500);";
    if (sqlite3_exec(db, sqlInsert, 0, 0, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    // Consultar investidores do banco de dados
    const char *sqlSelect = "SELECT Name, Authorized, Balance FROM Investors;";
    if (sqlite3_exec(db, sqlSelect, callback, contract, &errMsg) != SQLITE_OK) {
        fprintf(stderr, "SQL error: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    // Fechar o banco de dados
    sqlite3_close(db);
}

// Função para investir
void invest(InvestmentContract *contract, const char *investor, long amount) {
    if (amount <= 0) {
        printf("Investment amount must be greater than zero\n");
        return;
    }

    int investorIndex = -1;

    // Verificar se o investidor está na lista de investidores
    for (int i = 0; i < contract->investorCount; i++) {
        if (strcmp(contract->investors[i], investor) == 0) {
            investorIndex = i;
            break;
        }
    }

    if (investorIndex == -1) {
        printf("Investor '%s' not found in the contract\n", investor);
        return;
    }

    // Verificar se o investidor está autorizado
    if (!contract->authorizedInvestors[investorIndex]) {
        printf("Investor '%s' is not authorized to invest\n", investor);
        return;
    }

    // Verificar se o saldo é suficiente
    if (amount > contract->balances[investorIndex]) {
        printf("Insufficient balance for investor '%s'. Current balance: %ld\n", investor, contract->balances[investorIndex]);
        return;
    }

    // Atualizar saldos e valores investidos
    contract->balances[investorIndex] -= amount;
    contract->investedAmount[investorIndex] += amount;
    printf("Investment of %ld made by '%s'\n", amount, investor);
}

// Função para liberar a memória alocada para investidores
void freeContract(InvestmentContract *contract) {
    for (int i = 0; i < contract->investorCount; i++) {
        free(contract->investors[i]);
    }
}

int main() {
    InvestmentContract contract;
    initializeContract(&contract);

    // Testando a função de investimento
    invest(&contract, "Alice", 200);   // Investimento válido
    invest(&contract, "Bob", 100);     // Investidor não autorizado
    invest(&contract, "Charlie", 2000); // Saldo insuficiente
    invest(&contract, "Dave", 100);    // Investidor não encontrado

    // Liberar a memória alocada
    freeContract(&contract);
    
    return 0;
}
