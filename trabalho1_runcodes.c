/*
 * Trabalho 1 - Contagem de Operacoes e de Tempo de Execucao
 * Estruturas de Dados I - ICMC-USP - 2026
 *
 * Integrantes:
 *   Joao Pedro Oliveira - 17930847
 *   Matheus Amaral Alves - 17932255
 *   Lorenzo Vittorio Rudnik Spelta - 17906700
 *   Gustavo Cestari Cassaro Mendes - 17990719
 */

#include <stdio.h>
#include <stdlib.h>

/* Contadores de operacoes (comparacoes e atribuicoes), usados na
 * analise empirica do relatorio. Nao afetam a saida do programa. */
long g_comparacoes = 0;
long g_atribuicoes = 0;

/* Inverte o vetor no lugar, trocando as pontas ate o meio. */
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
    g_comparacoes++;
}

/* Busca sequencial: retorna o indice de x ou -1 se nao encontrar. */
int buscaSequencial(int v[], int n, int x) {
    int i = 0;
    g_atribuicoes++;

    while (i < n) {
        g_comparacoes++;

        g_comparacoes++;
        if (v[i] == x) {
            return i;
        }

        i++;
        g_atribuicoes++;
    }
    g_comparacoes++;

    return -1;
}

/* Busca binaria iterativa (vetor ordenado): indice de x ou -1. */
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

/* Busca binaria recursiva no intervalo [ini, fim]. */
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

    /* A inversao e feita numa copia, para o vetor original continuar
     * ordenado para as buscas binarias dos comandos seguintes. */
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
