/*
 * Codigo auxiliar - gerador de casos de teste
 * Trabalho 1 - Estruturas de Dados I - ICMC-USP - 2026
 *
 * Gera vetores ORDENADOS de inteiros aleatorios nos tamanhos exigidos
 * pelo enunciado (1000, 50000, 100000, 500000, 1000000) e salva cada
 * um em um arquivo .txt dentro da pasta "dados/".
 *
 * Formato de cada arquivo (apenas o vetor, sem comandos):
 *   linha 1: N
 *   linha 2: N inteiros ordenados, separados por espaco
 *
 * Este arquivo NAO faz parte do programa entregue no Run.Codes -
 * e apenas o codigo auxiliar usado para gerar os dados experimentais
 * (tempo de execucao e contagem de operacoes), conforme permitido
 * pelo enunciado.
 *
 * Convencao para o PIOR CASO de busca: usar como valor buscado um
 * elemento que NAO esta no vetor, por exemplo v[n-1] + 1. Isso forca
 * a busca sequencial a percorrer o vetor inteiro e a busca binaria a
 * descer ate o fim das divisoes, sem precisar salvar esse valor no
 * arquivo (ele e sempre facil de recalcular a partir do proprio vetor).
 *
 * Seed fixa (42) para que os dados sejam os mesmos sempre que o
 * gerador for executado por qualquer integrante do grupo.
 */

#include <stdio.h>
#include <stdlib.h>

#define SEED 42
#define NUM_TAMANHOS 5
#define GAP_MAX 50 /* diferenca maxima entre elementos consecutivos */

static const int TAMANHOS[NUM_TAMANHOS] = {1000, 50000, 100000, 500000, 1000000};

/* Gera um vetor ordenado (estritamente crescente) de tamanho n. */
static void gerarVetorOrdenado(int v[], int n) {
    int valor = rand() % GAP_MAX;
    for (int i = 0; i < n; i++) {
        v[i] = valor;
        valor += 1 + (rand() % GAP_MAX);
    }
}

static int salvarArquivo(const char *caminho, int v[], int n) {
    FILE *f = fopen(caminho, "w");
    if (f == NULL) {
        return 0;
    }

    fprintf(f, "%d\n", n);
    for (int i = 0; i < n; i++) {
        fprintf(f, "%d%c", v[i], (i == n - 1) ? '\n' : ' ');
    }

    fclose(f);
    return 1;
}

int main(void) {
    srand(SEED);

    for (int i = 0; i < NUM_TAMANHOS; i++) {
        int n = TAMANHOS[i];

        int *v = (int *) malloc((size_t) n * sizeof(int));
        if (v == NULL) {
            fprintf(stderr, "Erro de alocacao para n=%d\n", n);
            return 1;
        }

        gerarVetorOrdenado(v, n);

        char caminho[64];
        snprintf(caminho, sizeof(caminho), "dados/dados_%d.txt", n);

        if (!salvarArquivo(caminho, v, n)) {
            fprintf(stderr, "Erro ao salvar %s (a pasta 'dados/' existe?)\n", caminho);
            free(v);
            return 1;
        }

        printf("Gerado: %s (n=%d)\n", caminho, n);
        free(v);
    }

    return 0;
}
