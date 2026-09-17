#!/usr/bin/env python3
"""Graficos adicionais da parte 3, gerados apenas dos CSVs do benchmark."""
import csv
import sys
from collections import defaultdict
from pathlib import Path

import matplotlib
matplotlib.use("Agg")
import matplotlib.pyplot as plt
from matplotlib.ticker import FixedLocator, FuncFormatter

ALG = "buscaBinariaIterativa"
ROOT = Path(__file__).resolve().parent
sys.dont_write_bytecode = True


def main():
    with (ROOT / "resultados/resultado_binaria_iterativa.csv").open() as f:
        rows = sorted((r for r in csv.DictReader(f) if r["algoritmo"] == ALG),
                      key=lambda r: int(r["tamanho"]))
    ns = [int(r["tamanho"]) for r in rows]
    if ns != [1000, 50000, 100000, 500000, 1000000]:
        raise ValueError("Faltam tamanhos exigidos no resultado do benchmark.")
    tempos = defaultdict(list)
    with (ROOT / "resultados/tempos_binaria_iterativa.csv").open() as f:
        for r in csv.DictReader(f):
            if r["algoritmo"] == ALG:
                tempos[int(r["tamanho"])].append((int(r["repeticao"]), float(r["tempo_s"]) * 1e9))
    for n in ns:
        tempos[n].sort()
        if [r for r, _ in tempos[n]] != list(range(1, 101)):
            raise ValueError(f"Esperadas 100 medicoes para n={n}.")

    plt.rcParams.update({"font.size": 10, "axes.spines.top": False,
                         "axes.spines.right": False, "savefig.facecolor": "white"})
    cor, verde = "#174c78", "#178477"
    fig, axs = plt.subplots(2, 1, figsize=(7.1, 6.0), layout="constrained")
    medias = [float(r["tempo_medio_s"]) * 1e9 for r in rows]
    axs[0].plot(ns, medias, "o-", color=cor, linewidth=2)
    for n, y in zip(ns, medias):
        axs[0].annotate(f"{y:.0f}", (n, y), xytext=(0, 8), textcoords="offset points", ha="center")
    axs[0].set_title("Tempo medio de 100 execucoes", loc="left", weight="bold")
    axs[0].set_ylabel("Tempo (ns)")
    axs[0].set_ylim(0, max(medias) * 1.3)
    axs[1].plot(ns, [int(r["comparacoes_pior_caso"]) for r in rows], "o-", color=cor, label="Comparacoes")
    axs[1].plot(ns, [int(r["atribuicoes_pior_caso"]) for r in rows], "s-", color=verde, label="Atribuicoes")
    axs[1].set_title("Operacoes no pior caso: medido = teorico", loc="left", weight="bold")
    axs[1].set_ylabel("Numero de operacoes")
    axs[1].set_ylim(0, 75)
    axs[1].legend(loc="upper left", frameon=False)
    for ax in axs:
        ax.set_xscale("log")
        ax.xaxis.set_major_locator(FixedLocator(ns))
        ax.xaxis.set_major_formatter(FuncFormatter(lambda x, _: f"{int(x):,}".replace(",", ".")))
        ax.tick_params(axis="x", labelrotation=25)
        ax.minorticks_off()
        ax.set_xlabel("Tamanho do vetor (escala logaritmica)")
        ax.grid(axis="y", alpha=.2)
    out = ROOT / "graficos"
    out.mkdir(exist_ok=True)
    fig.savefig(out / "buscaBinariaIterativa_resumo.png", dpi=180)
    plt.close(fig)

    # Mesmo formato horizontal de cinco paineis utilizado pelo grupo.
    import grafico_medicoes
    grafico_medicoes.plotar_algoritmo(ALG, {n: [(r, t / 1e9) for r, t in tempos[n]] for n in ns})

    fig, axs = plt.subplots(3, 2, figsize=(7.1, 6.8), sharex=True, sharey=True, layout="constrained")
    ymax = max(t for values in tempos.values() for _, t in values) * 1.18
    for ax, n in zip(axs.flat, ns):
        reps, ts = zip(*tempos[n])
        media = sum(ts) / len(ts)
        ax.scatter(reps, ts, s=10, alpha=.6, color=cor)
        ax.axhline(media, color=verde, linestyle="--", linewidth=1.2)
        ax.set_title(f"n = {n:,}".replace(",", ".") + f" | media {media:.2f} ns")
        ax.set_ylim(0, ymax)
        ax.set_xlim(0, 101)
        ax.set_xlabel("Execucao")
        ax.set_ylabel("Tempo (ns)")
        ax.tick_params(labelbottom=True)
        ax.grid(alpha=.18)
    axs.flat[5].axis("off")
    axs.flat[5].text(0.05, .85,
        "500 medicoes reais\n\n100 por tamanho\nPontos: chamadas individuais\nTracejado: media do painel\nMesma escala vertical\n\nAlvo: v[n - 1] + 1\nContadores incluidos no tempo", va="top", fontsize=11,
        transform=axs.flat[5].transAxes, linespacing=1.55)
    fig.savefig(out / "buscaBinariaIterativa_detalhes.png", dpi=180)
    plt.close(fig)
    print("Graficos da busca binaria iterativa atualizados.")


if __name__ == "__main__":
    main()
