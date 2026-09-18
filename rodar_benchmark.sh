#!/bin/bash
# Trabalho 1 - Estruturas de Dados I - ICMC-USP - 2026
#
# Gera os dados (se ainda nao existirem), compila e roda o benchmark
# para os 5 tamanhos, SEQUENCIALMENTE, em um unico processo.
#
# Saidas:
#   resultados/resultado_completo.csv   medias e operacoes (pior caso), todos os algoritmos
#   resultados/tempos_detalhados.csv    as 100 medicoes individuais de cada (algoritmo, tamanho)
#
# Uso: ./rodar_benchmark.sh

set -e

mkdir -p dados resultados

if [ ! -f dados/dados_1000.txt ]; then
    echo "Dados nao encontrados, gerando..."
    gcc -Wall -Wextra -std=c99 -o gerador gerador.c
    ./gerador
fi

gcc -Wall -Wextra -O2 -std=c99 -o benchmark benchmark.c algoritmos.c

# Fixa o processo em um unico nucleo (CPU 1, um P-core na maquina de
# referencia): em processadores hibridos (P-cores + E-cores) o
# escalonador pode migrar o processo entre nucleos de velocidades
# diferentes no meio das 100 medicoes, o que distorce as medias.
# Se "taskset" nao existir, roda sem fixar.
#
# Perfil de energia: em notebooks, o perfil "power-saver" limita a
# frequencia e deixou os tempos ~2,7x maiores que em "performance" na
# maquina de referencia. Para as medicoes serem comparaveis entre
# rodadas, o script muda para "performance" durante o benchmark e
# restaura o perfil anterior ao final (se powerprofilesctl existir).
PERFIL_ANTERIOR=""
if command -v powerprofilesctl >/dev/null 2>&1; then
    PERFIL_ANTERIOR=$(powerprofilesctl get)
    powerprofilesctl set performance && echo "Perfil de energia: performance (antes: $PERFIL_ANTERIOR)"
fi
restaurar_perfil() {
    if [ -n "$PERFIL_ANTERIOR" ]; then
        powerprofilesctl set "$PERFIL_ANTERIOR"
        echo "Perfil de energia restaurado: $PERFIL_ANTERIOR"
    fi
}
trap restaurar_perfil EXIT

if command -v taskset >/dev/null 2>&1; then
    echo "Rodando benchmark (sequencial, fixado na CPU 1)..."
    taskset -c 1 ./benchmark
else
    echo "Rodando benchmark (sequencial)..."
    ./benchmark
fi

echo "Concluido."
echo "  - resultados/resultado_completo.csv"
echo "  - resultados/tempos_detalhados.csv"
