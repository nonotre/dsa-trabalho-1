/*
 * Trabalho 1 - Contagem de Operacoes e de Tempo de Execucao
 * Estruturas de Dados I - ICMC-USP - 2026
 *
 * Este arquivo contem apenas o menu de entrada/saida exigido pelo
 * enunciado. As implementacoes dos 4 algoritmos ficam em
 * algoritmos.c/algoritmos.h, compartilhadas com o codigo de
 * benchmark (que roda separadamente, fora do Run.Codes).
 *
 * A versao em arquivo unico enviada ao Run.Codes (este menu + os
 * algoritmos) esta em entrega/trabalho1_runcodes.c.
 *
 * Compilar: gcc -Wall -Wextra -std=c99 -Isrc -o trabalho1 src/trabalho1.c src/algoritmos.c
 * Testar:   ./testes/rodar_testes.sh
 *
 * Integrantes:
 *   - Joao Pedro Oliveira - 17930847
 *   - Matheus Amaral Alves - 17932255
 *   - Lorenzo Vittorio Rudnik Spelta - 17906700
 *   - Gustavo Cestari Cassaro Mendes - 17990719
 */

#include <stdio.h>
#include <stdlib.h>
#include "algoritmos.h"

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
