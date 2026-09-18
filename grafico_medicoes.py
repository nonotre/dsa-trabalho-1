#!/usr/bin/env python3
"""
Trabalho 1 - Estruturas de Dados I - ICMC-USP - 2026

Le resultados/tempos_detalhados.csv (as 100 medicoes individuais de
tempo de cada algoritmo, por tamanho de vetor) e gera, para cada
algoritmo implementado, um grafico com um painel por tamanho mostrando
as 100 medicoes e a media:

    graficos/<algoritmo>_100_medicoes.png

Algoritmos ainda nao implementados (stub, 0 comparacoes em
resultados/resultado_completo.csv) sao pulados.

Uso:
    python3 grafico_medicoes.py

Dependencia: matplotlib (pip install matplotlib)
"""

import csv
import sys
from collections import defaultdict
from pathlib import Path

import matplotlib.pyplot as plt

CSV_DETALHADO = "resultados/tempos_detalhados.csv"
CSV_RESUMO = "resultados/resultado_completo.csv"
PASTA_SAIDA = "graficos"
LIMITE_OUTLIER = 3.0  # pontos acima de 3x a mediana sao anotados fora da escala


def algoritmos_implementados(caminho_resumo):
    """Conjunto de algoritmos com pelo menos 1 comparacao contada (nao sao stub)."""
    comparacoes = defaultdict(int)
    with open(caminho_resumo, newline="") as f:
        for linha in csv.DictReader(f):
            comparacoes[linha["algoritmo"]] += int(linha["comparacoes_pior_caso"])
    return {alg for alg, total in comparacoes.items() if total > 0}


def ler_tempos(caminho_csv):
    """Retorna {algoritmo: {tamanho: [(repeticao, tempo_s), ...]}} ordenado por repeticao."""
    dados = defaultdict(lambda: defaultdict(list))

    with open(caminho_csv, newline="") as f:
        for linha in csv.DictReader(f):
            dados[linha["algoritmo"]][int(linha["tamanho"])].append(
                (int(linha["repeticao"]), float(linha["tempo_s"]))
            )

    for por_tamanho in dados.values():
        for pares in por_tamanho.values():
            pares.sort(key=lambda par: par[0])

    return dados


def plotar_algoritmo(alg, por_tamanho):
    tamanhos = sorted(por_tamanho.keys())

    fig, eixos = plt.subplots(1, len(tamanhos), figsize=(4 * len(tamanhos), 4), sharey=False)
    if len(tamanhos) == 1:
        eixos = [eixos]

    for eixo, tamanho in zip(eixos, tamanhos):
        pares = por_tamanho[tamanho]
        repeticoes = [p[0] for p in pares]
        tempos = [p[1] * 1e6 for p in pares]  # segundos -> microssegundos
        media = sum(tempos) / len(tempos)
        mediana = sorted(tempos)[len(tempos) // 2]

        # Um unico pico muito alto (ex: uma interrupcao do SO de varios
        # microssegundos numa chamada de 40 ns) achataria todo o painel.
        # Limita o eixo y a LIMITE_OUTLIER x a mediana e anota quantos
        # pontos ficaram fora da escala, com o valor do maior deles.
        teto = LIMITE_OUTLIER * mediana
        fora = [t for t in tempos if t > teto]
        dentro = [(r, t) for r, t in zip(repeticoes, tempos) if t <= teto]

        eixo.scatter([r for r, _ in dentro], [t for _, t in dentro], s=12, alpha=0.6, label="medicao")
        eixo.axhline(media, color="red", linestyle="--", linewidth=1, label=f"media = {media:.3f} us")
        eixo.axhline(mediana, color="gray", linestyle=":", linewidth=1, label=f"mediana = {mediana:.3f} us")
        if fora:
            eixo.scatter([r for r, t in zip(repeticoes, tempos) if t > teto], [teto] * len(fora),
                         marker="^", color="red", s=30, label=f"{len(fora)} fora da escala (max {max(fora):.2f} us)")
            eixo.set_ylim(top=teto * 1.05)
        eixo.set_title(f"n = {tamanho:,}".replace(",", "."))
        eixo.set_xlabel(f"Execucao (1 a {len(pares)})")
        eixo.grid(True, linestyle="--", alpha=0.4)
        eixo.legend(fontsize=8)

    eixos[0].set_ylabel("Tempo (us)")
    fig.suptitle(f"{alg} — {len(por_tamanho[tamanhos[0]])} medicoes de tempo por tamanho de entrada")
    fig.tight_layout()

    Path(PASTA_SAIDA).mkdir(exist_ok=True)
    caminho = Path(PASTA_SAIDA) / f"{alg}_100_medicoes.png"
    fig.savefig(caminho, dpi=150)
    plt.close(fig)
    print(f"Salvo: {caminho}")


def main():
    for caminho in (CSV_DETALHADO, CSV_RESUMO):
        if not Path(caminho).exists():
            print(f"Nao encontrei {caminho}. Rode ./rodar_benchmark.sh primeiro.")
            sys.exit(1)

    implementados = algoritmos_implementados(CSV_RESUMO)
    dados = ler_tempos(CSV_DETALHADO)

    gerou = False
    for alg in sorted(dados):
        if alg not in implementados:
            print(f"Pulando '{alg}' (ainda nao implementado).")
            continue
        plotar_algoritmo(alg, dados[alg])
        gerou = True

    if not gerou:
        print("Nenhum algoritmo implementado ainda - nada para plotar.")


if __name__ == "__main__":
    main()
