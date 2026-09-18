# Trabalho 1 — Estruturas de Dados I (ICMC-USP, 2026)

Inversão de vetor, busca sequencial, busca binária iterativa e recursiva:
tempo de execução, contagem de operações e análise assintótica.
**Entrega: 18/09** (código no Run.Codes, relatório + auxiliares no e-disciplinas).

## Quem faz o quê

| Função | Responsável | Status |
|---|---|---|
| `inverter` | João Pedro Oliveira | feito |
| `buscaSequencial` | Matheus Amaral Alves | feito |
| `buscaBinariaIterativa` | Lorenzo Vittorio Rudnik Spelta | feito |
| `buscaBinariaRecursiva` | Gustavo Cestari Cassaro Mendes | feito |

**Estado: as quatro funções estão integradas em `master`, os 5 casos
oficiais passam e o relatório está com a rodada final de medições.**
O que segue é o fluxo que foi usado para chegar aqui.

## 1. Clonar e criar sua branch

```bash
git clone https://github.com/nonotre/dsa-trabalho-1.git
cd dsa-trabalho-1
git checkout -b busca-sequencial      # ou busca-binaria-iterativa / busca-binaria-recursiva
```

## 2. Implementar

Abra `algoritmos.c`, ache sua função (o comentário traz o pseudocódigo de
referência e o T(n) esperado). Regras:

- C puro, sem bibliotecas externas.
- Siga **exatamente** o pseudocódigo do comentário — o relatório já
  foi escrito com base nele.
- Conte operações com os contadores globais (convenção completa em
  `algoritmos.h`; exemplo pronto: `inverter`):
  - `g_comparacoes++` em toda condição de `while`/`if` (inclusive a
    última do laço, que dá falso).
  - `g_atribuicoes++` em toda atribuição, de vetor **ou de índice**
    (`i = 0`, `i++`, `meio = …`, `ini = …`, `fim = …`). Parâmetro de
    função não conta.
  - Nunca zere os contadores dentro da função.
- Coloque seu nome e NUSP no cabeçalho de `algoritmos.c` e `trabalho1.c`.

Sua função está certa se, no pior caso (`x = v[n-1] + 1`), o benchmark
mostrar exatamente isto:

| n | sequencial comp / atrib | binária comp (iter. e rec.) | bin. iter. atrib | bin. rec. atrib |
|---|---|---|---|---|
| 1.000 | 2.001 / 1.001 | 31 | 22 | 10 |
| 50.000 | 100.001 / 50.001 | 49 | 34 | 16 |
| 100.000 | 200.001 / 100.001 | 52 | 36 | 17 |
| 500.000 | 1.000.001 / 500.001 | 58 | 40 | 19 |
| 1.000.000 | 2.000.001 / 1.000.001 | 61 | 42 | 20 |

## 3. Testar

```bash
gcc -Wall -Wextra -std=c99 -o trabalho1 trabalho1.c algoritmos.c
for i in 1 2 3 4 5; do
  diff <(./trabalho1 < "Casos de Teste - TRABALHO 1/$i.in") "Casos de Teste - TRABALHO 1/$i.out" \
    && echo "teste $i: OK" || echo "teste $i: FALHOU"
done
```

Teste 1 = inversão, 2 = sequencial, 3 = binária iterativa, 4 = binária
recursiva, 5 = tudo junto. O seu tem que passar.

## 4. Benchmark e gráficos

```bash
./rodar_benchmark.sh          # gera dados/ se faltar, compila, mede (~2 s)
python3 grafico.py            # graficos/tempo_medio.png, comparacoes.png, atribuicoes.png
python3 grafico_medicoes.py   # graficos/<sua_funcao>_100_medicoes.png
```

`rodar_benchmark.sh` coloca o perfil de energia em `performance` durante
a medição (e restaura depois) e fixa o processo em um núcleo; sem isso os
tempos variam até 2,7× entre rodadas na mesma máquina.

(`pip install --user matplotlib` uma vez.) Confira em
`resultados/resultado_completo.csv` se as contagens bateram com a tabela
acima. Funções ainda não implementadas aparecem com 0 e são ignoradas
nos gráficos.

## 5. Commit, push e Pull Request

```bash
git add algoritmos.c trabalho1.c resultados/ graficos/
git commit -m "Implementa buscaSequencial com contagem de operações"
git push -u origin busca-sequencial
```

Depois abra o PR no GitHub (link que aparece no `push`, ou
`gh pr create --fill`) para a branch `master`. Antes de pedir revisão:
teste oficial passando, contagens batendo com a tabela.

Não commite os binários (`trabalho1`, `gerador`, `benchmark` já estão no
`.gitignore`) nem `dados/` (14 MB; é regenerado com seed fixa). Não crie
benchmark/scripts paralelos — o `benchmark.c` e os dois `grafico*.py` já
medem e plotam as quatro funções; se faltar algo, altere eles.

Os números de tempo do relatório final vêm de **uma rodada só, numa
única máquina**, feita depois do merge de todas as funções (tempos de
máquinas diferentes não são comparáveis). As suas contagens de
operações, sim, precisam bater com a tabela antes do PR. Se editar o
`relatorio.md` no Windows, salve com fim de linha LF, não CRLF.

## 6. Relatório

`relatorio.md` está completo (metodologia, análise empírica das quatro
funções, análise teórica, conclusão e contribuições). Se alguma função
mudar, refaça `./rodar_benchmark.sh` + os dois `grafico*.py` e atualize
as tabelas das seções 2.x e 4.1 com o `resultado_completo.csv` — os
números de tempo do relatório têm que vir todos da mesma rodada.

## Estrutura

```
algoritmos.h / .c     protótipos, convenção de contagem e as 4 funções (SEM main)
trabalho1.c           menu de E/S (usa algoritmos.c)
trabalho1_runcodes.c  arquivo ÚNICO para o Run.Codes (algoritmos + menu)
gerador.c             gera dados/dados_<n>.txt (seed 42)
benchmark.c           100 medições de tempo + contagem no pior caso → resultados/*.csv
rodar_benchmark.sh    gera dados (se faltar), compila e roda o benchmark
grafico.py            gráficos de comparação (log-log)
grafico_medicoes.py   100 medições por algoritmo, um painel por tamanho
relatorio.md          relatório final
Casos de Teste - TRABALHO 1/   casos oficiais do professor
```

**Entrega no Run.Codes:** enviar `trabalho1_runcodes.c` (arquivo único
com algoritmos + menu, gerado a partir de `algoritmos.h`/`algoritmos.c`/
`trabalho1.c`; passa nos 5 casos oficiais byte a byte). `trabalho1.c`
sozinho não compila lá, pois depende de `algoritmos.c`.
