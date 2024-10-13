#!/bin/bash

# Define variáveis
DATA_LAKE_DIR="/path/para/seu/data/lake"
OUTPUT_DIR="/path/para/seu/saida"
DB_HOST="seu_host_do_banco"
DB_USER="seu_usuario"
DB_PASSWORD="sua_senha"
DB_NAME="seu_banco"

# Função para executar uma consulta SQL
function executar_consulta() {
    local consulta=$1
    echo "Executando consulta: $consulta"
    PGPASSWORD=$DB_PASSWORD psql -h $DB_HOST -U $DB_USER -d $DB_NAME -c "$consulta"
}

# Chamada do Drake
drake -d $DATA_LAKE_DIR -o $OUTPUT_DIR

# Executar consultas SQL
executar_consulta "SELECT * FROM planilhas WHERE nome = 'Vendas Q1';"
executar_consulta "SELECT dados->'vendas'->0->>'quantidade' AS quantidade_produto_a FROM planilhas WHERE nome = 'Vendas Q1';"

# Processamento adicional (exemplo: mover arquivos processados)
echo "Movendo arquivos processados para o diretório de saída..."
mv "$DATA_LAKE_DIR/arquivos_processados/*" "$OUTPUT_DIR/"

echo "Processo concluído!"
