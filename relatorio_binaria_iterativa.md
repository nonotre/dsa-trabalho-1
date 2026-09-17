# Busca binária iterativa - parte 3

Estruturas de Dados I - ICMC-USP - 2026

Responsável: Lorenzo Vittorio Rudnik Spelta.

Este documento reúne somente a parte atribuída a Lorenzo. A tabela, o gráfico e a discussão abaixo estão prontos para a seção 2.4 do relatório do grupo.

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

### 3.4 Busca Binária Iterativa (`buscaBinariaIterativa`)

O vetor deve estar ordenado em ordem crescente. A função mantém o intervalo `[ini, fim]`, calcula `meio = (ini + fim) / 2` e compara o elemento central com o alvo. Se os valores coincidirem, retorna o índice. Caso contrário, descarta a metade que não pode conter o alvo. Se o intervalo ficar vazio, retorna -1. A função não altera o vetor e usa espaço auxiliar O(1).

Pela convenção de `algoritmos.h`, cada iteração que não encontra o alvo custa três comparações (`ini <= fim`, `v[meio] == x`, `v[meio] < x`) e duas atribuições (cálculo de `meio` e atualização de um limite). Há duas atribuições iniciais. Se a busca terminar sem encontrar o alvo, existe ainda a comparação final do laço, que avalia falso. O incremento dos próprios contadores não é contado novamente como uma operação do algoritmo.

**Melhor caso.** O alvo coincide com o elemento central na primeira iteração. Há duas comparações e três atribuições: T(n) = 5 e complexidade O(1).

**Pior caso.** Para o alvo ausente maior que todos os elementos, a sequência de tamanhos é n, ⌊n/2⌋, ⌊n/4⌋, ..., 1, 0. Assim, k = ⌊log₂ n⌋ + 1 iterações completas são necessárias. As contagens são C(n) = 3k + 1 e A(n) = 2k + 2. Somando-as com custo unitário, T(n) = 5k + 3 = 5⌊log₂ n⌋ + 8. O termo dominante é log n, portanto a complexidade é O(log n). Outros alvos ausentes podem encerrar em um nível menos profundo; a fórmula acima representa o máximo e é alcançada pelo alvo do benchmark.

**Caso médio.** Adota-se a hipótese explícita de elementos distintos, alvo presente e mesma probabilidade para cada uma das n posições. Se o alvo é encontrado na iteração t, existem t - 1 iterações completas e uma última iteração com retorno: C = 3t - 1, A = 2t + 1 e T = 5t. Seja D(n) a média de t sobre as n posições. Para a divisão pelo elemento central, com h = ⌊log₂ n⌋:

`D(n) = h + 1 - (2^(h+1) - h - 2) / n`.

A fórmula resulta da soma dos níveis da árvore de busca: há 2^(j-1) elementos encontrados na iteração j para 1 ≤ j ≤ h, e os demais são encontrados na iteração h + 1. Logo, C_médio(n) = 3D(n) - 1, A_médio(n) = 2D(n) + 1 e T_médio(n) = 5D(n). Como D(n) = Θ(log n), o caso médio é O(log n). A distribuição de alvos foi especificada porque não existe uma média única sem uma hipótese probabilística.

| Caso | Comparações | Atribuições | T(n) = C + A | Complexidade |
|---|---|---|---|---|
| Melhor | 2 | 3 | 5 | O(1) |
| Médio, alvo presente uniforme | 3D(n) - 1 | 2D(n) + 1 | 5D(n) | O(log n) |
| Pior, k = ⌊log₂ n⌋ + 1 | 3k + 1 | 2k + 2 | 5k + 3 | O(log n) |

**Corretude.** Se o alvo estiver no vetor, ele permanece no intervalo considerado: a ordenação permite descartar somente posições que não podem contê-lo. Após uma iteração sem sucesso, `ini` aumenta ou `fim` diminui, de modo que a busca termina. Um retorno com índice ocorre apenas quando a igualdade foi verificada. A posição central usada respeita o pseudocódigo do grupo; com n ≤ 1.000.000, a soma dos índices cabe nos inteiros de 32 bits do ambiente usado.

## Validação e contribuição

O caso oficial 3 passou com saída idêntica ao arquivo 3.out. Também passaram 33.792 consultas sobre vetores pequenos, com e sem valores repetidos, buscas presentes e ausentes; verificou-se que a função não modifica o vetor. Foram testados os extremos de int e as contagens nos cinco tamanhos exigidos, no melhor e no pior caso. Contadores com valores iniciais não nulos confirmaram que a função não os zera.

A contribuição da parte 3 consiste na implementação, instrumentação, testes, medições, gráficos e análise da busca binária iterativa. O prazo indicado no enunciado é 18/09. O código do menu é entregue no Run.Codes; o relatório e os auxiliares são entregues no e-disciplinas.

Fontes: enunciado fornecido; algoritmos.h, algoritmos.c e README.md do repositório https://github.com/nonotre/dsa-trabalho-1; código da busca sequencial fornecido pelo usuário. A implementação da opção 3 segue o pseudocódigo do grupo.
