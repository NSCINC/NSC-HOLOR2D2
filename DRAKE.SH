#!/bin/bash

# Configurações do banco de dados
DB_HOST="seu_host_do_banco"
DB_USER="seu_usuario"
DB_PASS="sua_senha"
DB_NAME="seu_banco"

# Função para executar comandos SQL
execute_sql() {
    PGPASSWORD="$DB_PASS" psql -h "$DB_HOST" -U "$DB_USER" -d "$DB_NAME" -c "$1"
}

# Processamento de dados usando Drake Unix
drake -f Makefile

# Exemplo de chamada SQL para buscar todas as planilhas
execute_sql "SELECT * FROM planilhas ORDER BY data_criacao DESC;"

# Exemplo de chamada SQL para inserir uma nova planilha
execute_sql "INSERT INTO planilhas (nome, dados) VALUES ('Vendas Q2', '{\"vendas\": [{\"produto\": \"Produto C\", \"quantidade\": 150, \"preco\": 20.0}]}');"

echo "Processamento de dados concluído."
