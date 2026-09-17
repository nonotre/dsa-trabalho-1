/* Auxiliar da parte 3: 100 tempos e contagem no pior caso.
 * Usa a mesma funcao de algoritmos.c chamada pelo menu do Run.Codes.
 * Compilar em Linux/CS50: gcc -O2 -Wall -Wextra -std=c99
 *   benchmark_binaria_iterativa.c algoritmos.c -o benchmark_binaria_iterativa
 */
#define _POSIX_C_SOURCE 199309L
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "algoritmos.h"

static double agora(void) {
    struct timespec ts;
    if (clock_gettime(CLOCK_MONOTONIC, &ts) != 0) {
        perror("clock_gettime");
        exit(EXIT_FAILURE);
    }
    return (double) ts.tv_sec + (double) ts.tv_nsec / 1e9;
}

static void aquecer(void) {
    volatile unsigned long soma = 0;
    double inicio = agora();
    while (agora() - inicio < 0.3) {
        for (int k = 0; k < 1000; k++) soma += (unsigned long) k;
    }
    (void) soma;
}

static int medir(int n, FILE *resumo, FILE *detalhes) {
    char caminho[80];
    snprintf(caminho, sizeof(caminho), "dados/dados_%d.txt", n);
    FILE *entrada = fopen(caminho, "r");
    if (entrada == NULL) {
        perror(caminho);
        return 0;
    }
    int n_lido;
    if (fscanf(entrada, "%d", &n_lido) != 1 || n_lido != n) {
        fclose(entrada);
        return 0;
    }
    int *v = malloc((size_t) n * sizeof(int));
    if (v == NULL) {
        fclose(entrada);
        return 0;
    }
    for (int i = 0; i < n; i++) {
        if (fscanf(entrada, "%d", &v[i]) != 1) {
            free(v);
            fclose(entrada);
            return 0;
        }
    }
    fclose(entrada);
    /* Seguro para os valores produzidos pelo gerador.c do grupo. */
    int alvo = v[n - 1] + 1;

    aquecer();
    for (int r = 0; r < 3; r++) (void) buscaBinariaIterativa(v, n, alvo);
    double soma = 0.0;
    for (int r = 1; r <= 100; r++) {
        double inicio = agora();
        (void) buscaBinariaIterativa(v, n, alvo);
        double fim = agora();
        double tempo = fim - inicio;
        soma += tempo;
        fprintf(detalhes, "buscaBinariaIterativa,%d,%d,%.9f\n", n, r, tempo);
    }

    g_comparacoes = 0;
    g_atribuicoes = 0;
    int posicao = buscaBinariaIterativa(v, n, alvo);
    if (posicao != -1) {
        fprintf(stderr, "Falha: alvo ausente foi encontrado.\n");
        free(v);
        return 0;
    }
    fprintf(resumo, "buscaBinariaIterativa,%d,%.9f,%ld,%ld\n",
            n, soma / 100.0, g_comparacoes, g_atribuicoes);
    printf("n=%d: media %.9f s; %ld comparacoes; %ld atribuicoes\n",
           n, soma / 100.0, g_comparacoes, g_atribuicoes);
    free(v);
    return 1;
}

int main(void) {
    const int tamanhos[] = {1000, 50000, 100000, 500000, 1000000};
    FILE *resumo = fopen("resultados/resultado_binaria_iterativa.csv", "w");
    if (resumo == NULL) return EXIT_FAILURE;
    FILE *detalhes = fopen("resultados/tempos_binaria_iterativa.csv", "w");
    if (detalhes == NULL) {
        fclose(resumo);
        return EXIT_FAILURE;
    }
    fprintf(resumo, "algoritmo,tamanho,tempo_medio_s,comparacoes_pior_caso,atribuicoes_pior_caso\n");
    fprintf(detalhes, "algoritmo,tamanho,repeticao,tempo_s\n");
    int ok = 1;
    for (int i = 0; i < 5 && ok; i++) ok = medir(tamanhos[i], resumo, detalhes);
    if (fclose(resumo) != 0) ok = 0;
    if (fclose(detalhes) != 0) ok = 0;
    return ok ? EXIT_SUCCESS : EXIT_FAILURE;
}
