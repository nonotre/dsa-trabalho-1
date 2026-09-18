#!/usr/bin/env python3
"""
Trabalho 1 - Estruturas de Dados I - ICMC-USP - 2026

Le resultados/resultado_completo.csv (gerado por auxiliar/rodar_benchmark.sh)
e gera os graficos de COMPARACAO ENTRE ALGORITMOS (secao de conclusao
do relatorio):

  - graficos/tempo_medio.png        tempo medio x tamanho de entrada
  - graficos/comparacoes.png        comparacoes (pior caso) x tamanho
  - graficos/atribuicoes.png        atribuicoes (pior caso) x tamanho

Os tres graficos usam escala log-log: as buscas binarias (dezenas de
operacoes, ~dezenas de ns) e os algoritmos lineares (milhoes de
operacoes, centenas de us) nao cabem juntos em escala linear.

Algoritmos ainda nao implementados (stub, sempre 0 comparacoes) sao
automaticamente OMITIDOS dos graficos, para nao confundir o grupo com
uma "curva" de tempo ~0 que na verdade e so a funcao retornando na
hora sem fazer nada.

Uso (de qualquer pasta):
    python3 auxiliar/grafico.py
    python3 grafico.py caminho/para/outro_resultado.csv

Dependencia: matplotlib (pip install matplotlib)
"""

import csv
import os
import sys
from collections import defaultdict
from pathlib import Path

import matplotlib.pyplot as plt
from matplotlib.ticker import FixedLocator, FuncFormatter, NullLocator

CSV_PADRAO = "resultados/resultado_completo.csv"
PASTA_SAIDA = "graficos"


def formatar_milhar(valor):
    """1000000 -> '1.000.000' (separador de milhar em portugues)."""
    return f"{int(valor):,}".replace(",", ".")


def ler_resultados(caminho_csv):
    """Retorna {algoritmo: {"tamanhos": [...], "tempos": [...], "comparacoes": [...], "atribuicoes": [...]}}."""
    dados = defaultdict(lambda: {"tamanhos": [], "tempos": [], "comparacoes": [], "atribuicoes": []})

    with open(caminho_csv, newline="") as f:
        leitor = csv.DictReader(f)
        for linha in leitor:
            alg = linha["algoritmo"]
            dados[alg]["tamanhos"].append(int(linha["tamanho"]))
            dados[alg]["tempos"].append(float(linha["tempo_medio_s"]))
            dados[alg]["comparacoes"].append(int(linha["comparacoes_pior_caso"]))
            dados[alg]["atribuicoes"].append(int(linha["atribuicoes_pior_caso"]))

    # garante que cada serie esta ordenada por tamanho
    for serie in dados.values():
        ordem = sorted(range(len(serie["tamanhos"])), key=lambda i: serie["tamanhos"][i])
        for chave in ("tamanhos", "tempos", "comparacoes", "atribuicoes"):
            serie[chave] = [serie[chave][i] for i in ordem]

    return dados


def eh_stub(serie):
    """Um algoritmo ainda nao implementado sempre conta 0 comparacoes."""
    return all(c == 0 for c in serie["comparacoes"])


def remover_stubs(dados):
    implementados = {}
    for alg, serie in dados.items():
        if eh_stub(serie):
            print(f"Pulando '{alg}' nos graficos (ainda nao implementado).")
        else:
            implementados[alg] = serie
    return implementados


def plotar(dados, chave, titulo, eixo_y, nome_arquivo):
    plt.figure(figsize=(8, 5))
    todos_tamanhos = set()
    for alg, serie in sorted(dados.items()):
        plt.plot(serie["tamanhos"], serie[chave], marker="o", label=alg)
        todos_tamanhos.update(serie["tamanhos"])

    eixo = plt.gca()
    eixo.set_xscale("log")
    eixo.set_yscale("log")

    # ticks do eixo x exatamente nos 5 tamanhos exigidos, com separador de milhar
    tamanhos = sorted(todos_tamanhos)
    eixo.xaxis.set_major_locator(FixedLocator(tamanhos))
    eixo.xaxis.set_minor_locator(NullLocator())
    eixo.xaxis.set_major_formatter(FuncFormatter(lambda v, _: formatar_milhar(v)))
    plt.xticks(rotation=30)

    plt.title(titulo)
    plt.xlabel("Tamanho da entrada (n)")
    plt.ylabel(eixo_y)
    plt.grid(True, which="both", linestyle="--", alpha=0.4)
    plt.legend()
    plt.tight_layout()

    Path(PASTA_SAIDA).mkdir(exist_ok=True)
    caminho = Path(PASTA_SAIDA) / nome_arquivo
    plt.savefig(caminho, dpi=150)
    plt.close()
    print(f"Salvo: {caminho}")


def main():
    # caminhos (resultados/, graficos/) sao relativos a raiz do repositorio
    os.chdir(Path(__file__).resolve().parent.parent)

    caminho_csv = sys.argv[1] if len(sys.argv) > 1 else CSV_PADRAO

    if not Path(caminho_csv).exists():
        print(f"Nao encontrei {caminho_csv}. Rode ./auxiliar/rodar_benchmark.sh primeiro.")
        sys.exit(1)

    dados = remover_stubs(ler_resultados(caminho_csv))

    if len(dados) == 0:
        print("Nenhum algoritmo implementado ainda - nada para plotar.")
        sys.exit(0)

    plotar(dados, "tempos", "Tempo medio de execucao (100 execucoes)", "Tempo (s, escala log)", "tempo_medio.png")
    plotar(dados, "comparacoes", "Comparacoes no pior caso", "Comparacoes (escala log)", "comparacoes.png")
    plotar(dados, "atribuicoes", "Atribuicoes no pior caso", "Atribuicoes (escala log)", "atribuicoes.png")


if __name__ == "__main__":
    main()
