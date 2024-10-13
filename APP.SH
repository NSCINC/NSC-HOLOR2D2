#!/bin/bash

# Verifica se o arquivo de rede neural está presente
if [ ! -f neural_network.py ]; then
    echo "O arquivo neural_network.py não foi encontrado!"
    exit 1
fi

# Executa o script da rede neural com Python
echo "Executando a rede neural..."
python3 neural_network.py

# Verifica se a execução foi bem-sucedida
if [ $? -eq 0 ]; then
    echo "A rede neural foi executada com sucesso!"
else
    echo "Erro ao executar a rede neural."
    exit 1
fi
