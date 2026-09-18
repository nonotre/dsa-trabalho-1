/*
 * Trabalho 1 - Contagem de Operacoes e de Tempo de Execucao
 * Estruturas de Dados I - ICMC-USP - 2026
 *
 * Implementacao dos 4 algoritmos exigidos pelo enunciado.
 * Este arquivo NAO tem main() - e compilado junto com trabalho1.c
 * (menu, para o Run.Codes) e junto com benchmark.c (medicao de
 * tempo/operacoes, para o e-disciplinas), para garantir que os dois
 * usem exatamente o mesmo codigo.
 *
 * Integrantes:
 *   - Joao Pedro Oliveira - 17930847
 *   - Matheus Amaral Alves - 17932255
 *   - NOME COMPLETO 3 - NUSP
 *   - NOME COMPLETO 4 - NUSP
 *
 * Responsaveis por cada funcao (preencher):
 *   - inverter ................... : Joao Pedro Oliveira
 *   - buscaSequencial ............ : Matheus Amaral Alves
 *   - buscaBinariaIterativa ...... : <nome>
 *   - buscaBinariaRecursiva ...... : <nome>
 *
 * Convencao de contagem de operacoes: ver algoritmos.h. Em resumo,
 * conta-se toda comparacao (condicao de laco e de if) e toda
 * atribuicao, tanto em posicoes do vetor quanto em variaveis de
 * controle (indices), incluindo inicializacoes e incrementos.
 */

#include "algoritmos.h"

long g_comparacoes = 0;
long g_atribuicoes = 0;

/*
 * Inverte a ordem dos elementos de v (em-place), v[0..n-1].
 * Troca v[i] com v[j], com i indo de 0 para o meio e j de n-1 para
 * o meio, ate que i >= j.
 *
 * Contagem (pela convencao de algoritmos.h):
 *   - 2 atribuicoes antes do laco (i = 0; j = n - 1).
 *   - Por iteracao (sao floor(n/2) iteracoes):
 *       1 comparacao  (i < j)
 *       3 atribuicoes na troca (temp = v[i]; v[i] = v[j]; v[j] = temp)
 *       2 atribuicoes nos indices (i++; j--)
 *   - 1 comparacao final (i < j avalia falso e encerra o laco).
 *
 *   T_comparacoes(n) = floor(n/2) + 1
 *   T_atribuicoes(n) = 5 * floor(n/2) + 2
 *
 * O numero de operacoes depende apenas de n, nunca dos valores do
 * vetor, logo melhor, medio e pior caso sao identicos: O(n).
 *
 * responsavel: Joao Pedro Oliveira
 */
void inverter(int v[], int n) {
    int i = 0;
    int j = n - 1;
    g_atribuicoes += 2;

    while (i < j) {
        g_comparacoes++;

        int temp = v[i];
        v[i] = v[j];
        v[j] = temp;
        g_atribuicoes += 3;

        i++;
        j--;
        g_atribuicoes += 2;
    }
    g_comparacoes++; /* comparacao que avalia falso e encerra o laco */
}

/*
 * Busca sequencial de x em v[0..n-1].
 * Retorna o indice de x se encontrado, ou -1 caso contrario.
 *
 * TODO: implementar - responsavel: <nome>
 *
 * Pseudocodigo de referencia (o relatorio ja assume exatamente este):
 *
 *     i = 0;                         // 1 atribuicao
 *     while (i < n) {                // 1 comparacao por avaliacao
 *         if (v[i] == x) return i;   // 1 comparacao
 *         i++;                       // 1 atribuicao
 *     }
 *     return -1;
 *
 * T(n) esperado:
 *   - melhor caso (x == v[0]):        2 comparacoes, 1 atribuicao
 *   - pior caso   (x ausente):        2n + 1 comparacoes, n + 1 atribuicoes
 *   - caso medio  (x presente, posicao uniforme): ~ n + 1 comparacoes,
 *                                                 ~ (n + 1)/2 atribuicoes
 * O benchmark usa x = v[n-1] + 1 (ausente) como pior caso.
 */
int buscaSequencial(int v[], int n, int x) {
    int i = 0;
    g_atribuicoes++;

    while (i < n) {
        g_comparacoes++; /* i < n */

        g_comparacoes++; /* v[i] == x */
        if (v[i] == x) {
            return i;
        }

        i++;
        g_atribuicoes++;
    }

    g_comparacoes++;

    return -1;
}

/*
 * Busca binaria iterativa de x em v[0..n-1] (v deve estar ordenado).
 * Retorna o indice de x se encontrado, ou -1 caso contrario.
 *
 * TODO: implementar - responsavel: <nome>
 *
 * Pseudocodigo de referencia (o relatorio ja assume exatamente este):
 *
 *     ini = 0; fim = n - 1;                    // 2 atribuicoes
 *     while (ini <= fim) {                     // 1 comparacao
 *         meio = (ini + fim) / 2;              // 1 atribuicao
 *         if (v[meio] == x) return meio;       // 1 comparacao
 *         if (v[meio] < x) ini = meio + 1;     // 1 comparacao + 1 atribuicao
 *         else             fim = meio - 1;     //                1 atribuicao
 *     }
 *     return -1;
 *
 * Por iteracao completa: 3 comparacoes e 2 atribuicoes.
 * Com x = v[n-1] + 1 (ausente, maior que todos) a busca sempre vai
 * para a direita e executa exatamente floor(log2 n) + 1 iteracoes.
 *
 * T(n) esperado:
 *   - melhor caso (x == v[meio] na 1a iteracao): 2 comparacoes, 3 atribuicoes
 *   - pior caso   (x ausente): 3*(floor(log2 n) + 1) + 1 comparacoes
 *                              2*(floor(log2 n) + 1) + 2 atribuicoes
 *   - caso medio: ~ 3*log2 n comparacoes, ~ 2*log2 n atribuicoes
 *
 * Valores esperados no pior caso, por tamanho (floor(log2 n)+1 / comp / atrib):
 *   n=1.000     -> 10 / 31 / 22
 *   n=50.000    -> 16 / 49 / 34
 *   n=100.000   -> 17 / 52 / 36
 *   n=500.000   -> 19 / 58 / 40
 *   n=1.000.000 -> 20 / 61 / 42
 */
int buscaBinariaIterativa(int v[], int n, int x) {
    (void) v;
    (void) n;
    (void) x;
    return -1;
}

/*
 * Busca binaria recursiva de x em v[0..n-1] (v deve estar ordenado).
 * Retorna o indice de x se encontrado, ou -1 caso contrario.
 *
 * TODO: implementar - responsavel: <nome>
 *
 * Pseudocodigo de referencia (o relatorio ja assume exatamente este):
 *
 *     static int bbRec(int v[], int ini, int fim, int x) {
 *         if (ini > fim) return -1;                          // 1 comparacao
 *         meio = (ini + fim) / 2;                            // 1 atribuicao
 *         if (v[meio] == x) return meio;                     // 1 comparacao
 *         if (v[meio] < x) return bbRec(v, meio + 1, fim, x); // 1 comparacao
 *         return bbRec(v, ini, meio - 1, x);
 *     }
 *     int buscaBinariaRecursiva(int v[], int n, int x) {
 *         return bbRec(v, 0, n - 1, x);
 *     }
 *
 * Passagem de parametro (ini, fim) NAO conta como atribuicao; so o
 * "meio" conta. Numero de chamadas no pior caso = floor(log2 n) + 2
 * (floor(log2 n) + 1 chamadas "cheias" + 1 chamada final com ini > fim).
 *
 * T(n) esperado:
 *   - melhor caso (x == v[meio] na 1a chamada): 2 comparacoes, 1 atribuicao
 *   - pior caso   (x ausente): 3*(floor(log2 n) + 1) + 1 comparacoes
 *                              floor(log2 n) + 1 atribuicoes
 *   - caso medio: ~ 3*log2 n comparacoes, ~ log2 n atribuicoes
 *
 * Valores esperados no pior caso, por tamanho (comp / atrib):
 *   n=1.000     -> 31 / 10
 *   n=50.000    -> 49 / 16
 *   n=100.000   -> 52 / 17
 *   n=500.000   -> 58 / 19
 *   n=1.000.000 -> 61 / 20
 */
int buscaBinariaRecursiva(int v[], int n, int x) {
    (void) v;
    (void) n;
    (void) x;
    return -1;
}
