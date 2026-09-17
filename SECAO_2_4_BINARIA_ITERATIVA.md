### 2.4 Busca Binária Iterativa (`buscaBinariaIterativa`)

| n | tempo médio (s) | comparações (medido) | comparações (teórico) | atribuições (medido) | atribuições (teórico) |
|---|---|---|---|---|---|
| 1.000 | 0,000000035 | 31 | 31 | 22 | 22 |
| 50.000 | 0,000000039 | 49 | 49 | 34 | 34 |
| 100.000 | 0,000000040 | 52 | 52 | 36 | 36 |
| 500.000 | 0,000000043 | 58 | 58 | 40 | 40 |
| 1.000.000 | 0,000000044 | 61 | 61 | 42 | 42 |

![100 medições da busca binária iterativa por tamanho](graficos/buscaBinariaIterativa_100_medicoes.png)

**Discussão.** As contagens medidas coincidiram com as teóricas em todos os tamanhos: sendo k = ⌊log₂ n⌋ + 1, a função realiza 3k + 1 comparações e 2k + 2 atribuições no pior caso utilizado. O alvo foi `v[n-1] + 1`, ausente e maior que todos os elementos, de modo que a busca sempre segue pela metade direita até o intervalo ficar vazio. Ao passar de 1.000 para 1.000.000 de elementos, o número de iterações aumenta de 10 para 20; as comparações passam de 31 para 61 e as atribuições, de 22 para 42. Esse crescimento confirma a função de custo logarítmica.

Nesta rodada, os tempos médios registrados foram 35, 39, 40, 43, 44 ns, respectivamente. As chamadas são muito curtas, e o custo de leitura do relógio, a instrumentação, os efeitos de cache e o escalonamento têm peso relevante no tempo observado. Os pontos do gráfico se concentram em poucos patamares, com variações entre chamadas. Por isso, a pequena diferença entre as médias não deve ser interpretada como custo constante: a evidência mais clara do crescimento O(log n) é a contagem determinística de operações. As 100 repetições usam o mesmo vetor e alvo após aquecimento; os resultados descrevem esse cenário e não representam todas as possíveis cargas de busca.

**Origem destas medições.** Execução real no ambiente Linux do assistente, separado do CS50 e do computador do aluno: AMD EPYC 9V74 80-Core Processor, Linux-6.18.44-x86_64-with-glibc2.39, gcc (Ubuntu 13.3.0-6ubuntu2~24.04) 13.3.0, CPU lógica 0. Compilação: `gcc -Wall -Wextra -Werror -O2 -std=c99`. Foram usados os vetores do `gerador.c` do grupo (semente 42), aquecimento de 0,3 s e três chamadas descartadas, seguidos de 100 chamadas cronometradas individualmente com `clock_gettime(CLOCK_MONOTONIC)` por tamanho. A leitura e geração dos vetores ficam fora do intervalo cronometrado. As comparações e atribuições foram contadas em uma chamada adicional, com contadores zerados pelo benchmark. O tempo inclui a instrumentação da função. Os CSVs guardam segundos com nove casas decimais.

**Arquivos desta parte.** `resultados/resultado_binaria_iterativa.csv`, `resultados/tempos_binaria_iterativa.csv` e `resultados/ambiente_binaria_iterativa.json`. A semente reproduz os dados com a mesma implementação de `rand`; os hashes das entradas identificam os vetores efetivamente usados. As medições das outras funções, feitas por colegas em outro ambiente, não permitem calcular diretamente uma razão de velocidade entre algoritmos. Uma comparação de tempos deve usar uma nova rodada conjunta na mesma máquina.
