# Trabalho 1 — Estruturas de Dados I (ICMC-USP, 2026)

Inversão de vetor, busca sequencial, busca binária iterativa e recursiva:
tempo de execução, contagem de operações e análise assintótica.
Enunciado em `enunciado/_ED1__trab_1.pdf`.

| Função | Responsável |
|---|---|
| `inverter` | João Pedro Oliveira — 17930847 |
| `buscaSequencial` | Matheus Amaral Alves — 17932255 |
| `buscaBinariaIterativa` | Lorenzo Vittorio Rudnik Spelta — 17906700 |
| `buscaBinariaRecursiva` | Gustavo Cestari Cassaro Mendes — 17990719 |

## Estrutura

```
entrega/trabalho1_runcodes.c   arquivo ÚNICO enviado ao Run.Codes (algoritmos + menu)
src/algoritmos.h, algoritmos.c protótipos, convenção de contagem e as 4 funções
src/trabalho1.c                menu de entrada/saída (usa algoritmos.c)
auxiliar/gerador.c             gera dados/dados_<n>.txt (seed 42) — código auxiliar
auxiliar/benchmark.c           100 medições de tempo + contagem no pior caso
auxiliar/rodar_benchmark.sh    gera dados (se faltar), compila e roda o benchmark
auxiliar/grafico.py            gráficos de comparação (log-log)
auxiliar/grafico_medicoes.py   100 medições por algoritmo, um painel por tamanho
testes/                        casos oficiais do professor (*.in/*.out) + rodar_testes.sh
resultados/                    .csv da rodada final (tempo médio, 100 medições, contagens)
graficos/                      .png usados no relatório
relatorio.md                   relatório final
```

## Entrega

- **Run.Codes:** `entrega/trabalho1_runcodes.c`. É um arquivo só; `src/trabalho1.c`
  sozinho não compila lá porque depende de `src/algoritmos.c`.
- **e-disciplinas:** `relatorio.md` (em PDF) + a pasta `auxiliar/`
  (código auxiliar de geração de dados, medição e gráficos).

## Reproduzir

```bash
./testes/rodar_testes.sh          # compila src/ e entrega/, compara com os 5 casos oficiais
./auxiliar/rodar_benchmark.sh     # gera dados/ se faltar, compila, mede (~2 s)
python3 auxiliar/grafico.py       # graficos/tempo_medio.png, comparacoes.png, atribuicoes.png
python3 auxiliar/grafico_medicoes.py   # graficos/<funcao>_100_medicoes.png
```

Todos os scripts funcionam de qualquer pasta (eles mesmos vão para a
raiz do repositório). `pip install --user matplotlib` uma vez.

Sobre a medição (detalhes na Seção 2.1 do relatório):

- `rodar_benchmark.sh` coloca o perfil de energia em `performance`
  durante o benchmark (restaura depois) e fixa o processo em um núcleo
  — sem isso os tempos variam até 2,7× entre rodadas na mesma máquina.
- Os números de tempo do relatório vêm de **uma rodada só, numa única
  máquina**, feita após integrar as quatro funções. Se rodar o benchmark
  de novo, `resultados/` e `graficos/` mudam; para manter o relatório
  coerente, ou atualize as tabelas das seções 2.x e 4.1 com o novo
  `resultado_completo.csv`, ou restaure com `git checkout -- resultados/ graficos/`.
- Contagens esperadas no pior caso (`x = v[n-1] + 1`), que o benchmark
  reproduz exatamente:

| n | inverter comp / atrib | sequencial comp / atrib | binária comp (iter. e rec.) | bin. iter. atrib | bin. rec. atrib |
|---|---|---|---|---|---|
| 1.000 | 501 / 2.502 | 2.001 / 1.001 | 31 | 22 | 10 |
| 50.000 | 25.001 / 125.002 | 100.001 / 50.001 | 49 | 34 | 16 |
| 100.000 | 50.001 / 250.002 | 200.001 / 100.001 | 52 | 36 | 17 |
| 500.000 | 250.001 / 1.250.002 | 1.000.001 / 500.001 | 58 | 40 | 19 |
| 1.000.000 | 500.001 / 2.500.002 | 2.000.001 / 1.000.001 | 61 | 42 | 20 |

## Alterar alguma função

1. Branch a partir de `master`; edite só `src/algoritmos.c`, mantendo a
   convenção de contagem de `src/algoritmos.h` (toda comparação de
   `while`/`if`, inclusive a última que dá falso; toda atribuição, de
   vetor ou de índice; parâmetro de função não conta; nunca zerar os
   contadores).
2. Replique a mudança em `entrega/trabalho1_runcodes.c` (é mantido à
   mão, sem os comentários longos).
3. `./testes/rodar_testes.sh` tem que passar 10/10 e as contagens do
   benchmark têm que bater com a tabela acima.
4. Commit, push, PR para `master`. Não commite binários nem `dados/`
   (já no `.gitignore`); não crie benchmark/scripts paralelos; se
   editar o `relatorio.md` no Windows, salve com fim de linha LF.
