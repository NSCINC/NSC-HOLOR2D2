#!/bin/bash

# Definir o nome do arquivo fonte e do programa de saída
SOURCE_FILE="master.c"
OUTPUT_PROGRAM="master_program"

# Compilar o programa
gcc "$SOURCE_FILE" -o "$OUTPUT_PROGRAM" -lm

# Checar se o binário foi gerado
if [ -f "./$OUTPUT_PROGRAM" ]; then
    echo "Compilação bem-sucedida."

    # Executar o programa
    ./"$OUTPUT_PROGRAM" # Alterado para executar diretamente o binário
else
    echo "Falha na compilação."
    exit 1
fi
