/*
 * Trabalho 1 - Contagem de Operacoes e de Tempo de Execucao
 * Estruturas de Dados I - ICMC-USP - 2026
 *
 * Versao em ARQUIVO UNICO para o Run.Codes: contem os 4 algoritmos
 * (algoritmos.h + algoritmos.c do repositorio) e o menu de
 * entrada/saida. Gerada a partir dos mesmos fontes usados no
 * benchmark, para que o codigo medido seja o codigo entregue.
 *
 * Compilar: gcc -Wall -Wextra -std=c99 -o trabalho1 trabalho1_runcodes.c
 *
 * Integrantes:
 *   - Joao Pedro Oliveira - 17930847
 *   - Matheus Amaral Alves - 17932255
 *   - Lorenzo Vittorio Rudnik Spelta - 17906700
 *   - Gustavo Cestari Cassaro Mendes - 17990719
 */

#include <stdio.h>
#include <stdlib.h>
/* ===== algoritmos.h ===== */
/*
 * Trabalho 1 - Estruturas de Dados I - ICMC-USP - 2026
 *
 * Prototipos dos 4 algoritmos exigidos pelo enunciado.
 * Usado tanto pelo programa de menu (trabalho1.c, entregue no
 * Run.Codes) quanto pelo codigo auxiliar de benchmark (contagem de
 * tempo e de operacoes, submetido separadamente no e-disciplinas).
 */


/*
 * Contadores globais de operacoes, usados pelo benchmark.c para a
 * contagem empirica de comparacoes e atribuicoes (secao "Analise
 * Empirica" do relatorio).
 *
 * CONVENCAO DE CONTAGEM (obrigatoria para as 4 funcoes, para que os
 * numeros do relatorio sejam comparaveis entre os algoritmos):
 *
 *   - g_comparacoes++ a cada COMPARACAO executada pelo algoritmo:
 *       * condicao de laco (ex: i < j, i < n, ini <= fim), inclusive
 *         a ultima avaliacao, que da falso e encerra o laco;
 *       * condicao de if envolvendo elementos do vetor ou indices
 *         (ex: v[i] == x, v[meio] < x, ini > fim).
 *
 *   - g_atribuicoes++ a cada ATRIBUICAO feita pelo algoritmo, seja em
 *     uma posicao do vetor ou em uma VARIAVEL DE CONTROLE:
 *       * escrita no vetor (v[i] = ...);
 *       * inicializacao de variavel de controle (i = 0, j = n - 1,
 *         ini = 0, fim = n - 1, temp = v[i]);
 *       * atualizacao de variavel de controle (i++, j--, meio = ...,
 *         ini = meio + 1, fim = meio - 1).
 *     Passagem de parametro em chamada de funcao NAO conta como
 *     atribuicao (relevante para a busca binaria recursiva).
 *
 *   - Quem chama a funcao (benchmark.c) zera os contadores antes,
 *     entao dentro da funcao e so incrementar, nunca zerar.
 *   - Veja a implementacao de "inverter" em algoritmos.c como
 *     exemplo de como instrumentar sua propria funcao, e o comentario
 *     TODO de cada busca com o pseudocodigo de referencia e o T(n)
 *     esperado no pior caso.
 */
extern long g_comparacoes;
extern long g_atribuicoes;

void inverter(int v[], int n);
int buscaSequencial(int v[], int n, int x);
int buscaBinariaIterativa(int v[], int n, int x);
int buscaBinariaRecursiva(int v[], int n, int x);


/* ===== algoritmos.c ===== */
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
 *   - Lorenzo Vittorio Rudnik Spelta - 17906700
 *   - Gustavo Cestari Cassaro Mendes - 17990719
 *
 * Responsaveis por cada funcao:
 *   - inverter ................... : Joao Pedro Oliveira
 *   - buscaSequencial ............ : Matheus Amaral Alves
 *   - buscaBinariaIterativa ...... : Lorenzo Vittorio Rudnik Spelta
 *   - buscaBinariaRecursiva ...... : Gustavo Cestari Cassaro Mendes
 *
 * Convencao de contagem de operacoes: ver algoritmos.h. Em resumo,
 * conta-se toda comparacao (condicao de laco e de if) e toda
 * atribuicao, tanto em posicoes do vetor quanto em variaveis de
 * controle (indices), incluindo inicializacoes e incrementos.
 */


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
 * responsavel: Matheus Amaral Alves
 *
 * Algoritmo e pontos de contagem:
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
 * responsavel: Lorenzo Vittorio Rudnik Spelta
 *
 * Algoritmo e pontos de contagem:
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
    int ini = 0;
    int fim = n - 1;
    g_atribuicoes += 2;

    while (ini <= fim) {
        g_comparacoes++;

        int meio = (ini + fim) / 2;
        g_atribuicoes++;

        g_comparacoes++;
        if (v[meio] == x) {
            return meio;
        }

        g_comparacoes++;
        if (v[meio] < x) {
            ini = meio + 1;
        } else {
            fim = meio - 1;
        }
        g_atribuicoes++;
    }
    g_comparacoes++;

    return -1;
}

/*
 * Busca binaria recursiva de x em v[0..n-1] (v deve estar ordenado).
 * Retorna o indice de x se encontrado, ou -1 caso contrario.
 *
 * responsavel: Gustavo Cestari Cassaro Mendes
 *
 * Implementacao (a funcao auxiliar bbRec recebe o intervalo [ini, fim]):
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
static int bbRec(int v[], int ini, int fim, int x) {
    g_comparacoes++;
    if (ini > fim) {
        return -1;
    }

    int meio = (ini + fim) / 2;
    g_atribuicoes++;

    g_comparacoes++;
    if (v[meio] == x) {
        return meio;
    }

    g_comparacoes++;
    if (v[meio] < x) {
        return bbRec(v, meio + 1, fim, x);
    }
    return bbRec(v, ini, meio - 1, x);
}

int buscaBinariaRecursiva(int v[], int n, int x) {
    return bbRec(v, 0, n - 1, x);
}

/* ===== menu ===== */

/* ---------- main: menu de entrada e saida ---------- */
int main(void) {
    int n;
    if (scanf("%d", &n) != 1) {
        return 0;
    }

    int *v = (int *) malloc((size_t) n * sizeof(int));
    if (v == NULL) {
        return 1;
    }
    for (int i = 0; i < n; i++) {
        scanf("%d", &v[i]);
    }

    /*
     * Buffer auxiliar usado apenas para IMPRIMIR a inversao.
     * O vetor "v" nunca e alterado aqui, pois ele precisa continuar
     * ordenado para que buscaBinariaIterativa/Recursiva funcionem
     * corretamente nos comandos seguintes (confirmado pelo caso de
     * teste 5 fornecido pelo professor: buscas apos uma inversao
     * continuam avaliando o vetor original ordenado).
     */
    int *tmp = (int *) malloc((size_t) n * sizeof(int));
    if (tmp == NULL) {
        free(v);
        return 1;
    }

    int q;
    while (scanf("%d", &q) == 1 && q != 5) {
        int p, achou;

        switch (q) {
            case 1:
                for (int i = 0; i < n; i++) {
                    tmp[i] = v[i];
                }
                inverter(tmp, n);
                for (int i = 0; i < n; i++) {
                    printf("%d ", tmp[i]);
                }
                printf("\n");
                break;

            case 2:
                scanf("%d", &p);
                achou = buscaSequencial(v, n, p);
                printf("%s\n", achou != -1 ? "SIM" : "NAO");
                break;

            case 3:
                scanf("%d", &p);
                achou = buscaBinariaIterativa(v, n, p);
                printf("%s\n", achou != -1 ? "SIM" : "NAO");
                break;

            case 4:
                scanf("%d", &p);
                achou = buscaBinariaRecursiva(v, n, p);
                printf("%s\n", achou != -1 ? "SIM" : "NAO");
                break;

            default:
                break;
        }
    }

    free(tmp);
    free(v);
    return 0;
}
