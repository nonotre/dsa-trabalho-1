#!/bin/bash
# Compila o programa do menu (fontes em src/ e a versao de arquivo unico
# em entrega/) e compara a saida com os casos de teste oficiais (*.in/*.out).
# Uso (de qualquer pasta): ./testes/rodar_testes.sh

cd "$(dirname "$0")/.."

gcc -Wall -Wextra -std=c99 -Isrc -o trabalho1 src/trabalho1.c src/algoritmos.c
gcc -Wall -Wextra -std=c99 -o trabalho1_runcodes entrega/trabalho1_runcodes.c

falhas=0
for bin in trabalho1 trabalho1_runcodes; do
    for entrada in testes/*.in; do
        caso=$(basename "$entrada" .in)
        if cmp -s <("./$bin" < "$entrada") "testes/$caso.out"; then
            echo "$bin  caso $caso: OK"
        else
            echo "$bin  caso $caso: FALHOU"
            falhas=$((falhas + 1))
        fi
    done
done

[ "$falhas" -eq 0 ] && echo "Todos os casos passaram." || { echo "$falhas falha(s)."; exit 1; }
