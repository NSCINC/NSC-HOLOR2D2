#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

const char* COMPILE_COMMAND = "gcc"; // Comando de compilação
const char* OUTPUT_NAME = "output_program"; // Nome do programa final
const char* TERMINAL_COMMAND = "abc"; // Comando fictício para executar o programa

// Função para compilar todos os arquivos .c no diretório atual
void compileAllCFiles() {
    char compileCmd[1024] = ""; // Buffer para o comando de compilação
    snprintf(compileCmd, sizeof(compileCmd), "%s -o %s ", COMPILE_COMMAND, OUTPUT_NAME);

    // Obter o diretório atual
    DIR* dir;
    struct dirent* entry;

    dir = opendir("."); // Abrir o diretório atual
    if (dir == NULL) {
        perror("Error accessing directory");
        return;
    }

    // Listar todos os arquivos no diretório atual
    while ((entry = readdir(dir)) != NULL) {
        // Adicionar arquivos .c ao comando de compilação
        if (strstr(entry->d_name, ".c") != NULL) {
            strcat(compileCmd, entry->d_name);
            strcat(compileCmd, " ");
        }
    }
    closedir(dir);

    // Verificar se há arquivos para compilar
    if (strcmp(compileCmd, "") == 0) {
        printf("Nenhum arquivo .c encontrado para compilar.\n");
        return;
    }

    // Executar o comando de compilação
    printf("Compiling with command: %s\n", compileCmd);
    int compileResult = system(compileCmd);

    if (compileResult != 0) {
        printf("Compilation failed.\n");
        return;
    }

    printf("Compilation successful!\n");
}

// Função para executar o programa compilado usando o terminal fictício "abc"
void runProgramWithABC() {
    char runCmd[1024];
    snprintf(runCmd, sizeof(runCmd), "%s ./%s", TERMINAL_COMMAND, OUTPUT_NAME);

    // Executar o programa
    printf("Executing the program with terminal '%s': %s\n", TERMINAL_COMMAND, runCmd);
    int runResult = system(runCmd);

    if (runResult != 0) {
        printf("Execution failed.\n");
    }
}

// Função principal
int main() {
    compileAllCFiles();  // Compilar todos os arquivos .c
    runProgramWithABC(); // Executar o programa com o comando fictício "abc"
    return 0;
}
