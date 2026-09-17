#!/usr/bin/env bash
# Testa somente a parte 3, sem depender da conclusao das outras buscas.
set -eu
cd "$(dirname "$0")"
pasta_testes=$(mktemp -d)
trap 'rm -rf "$pasta_testes"' EXIT
gcc -Wall -Wextra -Werror -std=c99 trabalho1.c algoritmos.c -o "$pasta_testes/trabalho1"
"$pasta_testes/trabalho1" < "Casos de Teste - TRABALHO 1/3.in" > "$pasta_testes/3.out"
diff -u "Casos de Teste - TRABALHO 1/3.out" "$pasta_testes/3.out"
echo "Caso oficial 3: OK"
gcc -Wall -Wextra -Werror -std=c99 testes/testar_binaria_iterativa.c algoritmos.c -o "$pasta_testes/testar"
"$pasta_testes/testar"
