#!/bin/bash

# Verifica se o arquivo ativas.txt existe
if [ ! -f "ativas.txt" ]; then
    echo "Erro: O arquivo ativas.txt não foi encontrado!"
    exit 1
fi

echo "Lendo as pastas ativas do arquivo ativas.txt..."

# Limpa a configuração anterior do sparse-checkout
git sparse-checkout disable

# Reativa o sparse-checkout no modo cone
git sparse-checkout init --cone

# Lê o arquivo ativas.txt linha por linha e define as pastas ativas
echo "Configurando as pastas no Git..."
while IFS= read -r pasta || [ -n "$pasta" ]; do
    # Ignora linhas em branco ou comentários
    [[ -z "$pasta" || "$pasta" =~ ^# ]] && continue
    
    git sparse-checkout add "$pasta"
done < ativas.txt

# Atualiza os arquivos no disco de acordo com a seleção
echo "Atualizando os arquivos locais..."
git checkout main

echo "Pronto! Suas pastas ativas foram atualizadas com sucesso."