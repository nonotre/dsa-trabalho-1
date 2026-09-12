/*
 * Trabalho 1 - Estruturas de Dados I - ICMC-USP - 2026
 *
 * Codigo auxiliar de benchmark - NAO faz parte da entrega do
 * Run.Codes. Para cada algoritmo e cada tamanho de entrada, mede:
 *   - tempo de CADA UMA das NUM_REPETICOES execucoes (relogio
 *     CLOCK_MONOTONIC, via clock_gettime), e a media delas;
 *   - comparacoes e atribuicoes no PIOR CASO (uma execucao a parte,
 *     com os contadores globais de algoritmos.c zerados antes).
 *
 * Metodologia:
 *   - Os tamanhos sao medidos SEQUENCIALMENTE, um processo so, para
 *     nao haver interferencia entre medicoes (uma versao anterior
 *     rodava os tamanhos em paralelo e isso distorcia os tempos dos
 *     vetores grandes).
 *   - Antes de medir cada tamanho o processador e mantido ocupado por
 *     AQUECIMENTO_GLOBAL_S segundos (para a frequencia do nucleo
 *     estabilizar no maximo), e antes das NUM_REPETICOES medicoes de
 *     cada (algoritmo, tamanho) sao feitas NUM_AQUECIMENTO chamadas
 *     nao cronometradas (para aquecer a cache).
 *   - O tempo medido INCLUI o custo dos incrementos dos contadores
 *     g_comparacoes/g_atribuicoes, pois os algoritmos sao sempre
 *     compilados com eles. Esse overhead e uniforme entre os
 *     algoritmos e nao muda a forma das curvas.
 *   - Para "inverter", o vetor e invertido de novo a cada chamada
 *     (o custo de inverter um vetor nao depende dos valores, so de n).
 *   - Pior caso de busca: valor buscado ausente do vetor (v[n-1] + 1),
 *     o que forca a busca sequencial a percorrer tudo e as buscas
 *     binarias a descerem ate o intervalo vazio.
 *
 * Uso:
 *   ./benchmark
 *
 * Le os vetores gerados por gerador.c em dados/dados_<tamanho>.txt e
 * escreve:
 *   resultados/resultado_completo.csv
 *       algoritmo,tamanho,tempo_medio_s,comparacoes_pior_caso,atribuicoes_pior_caso
 *   resultados/tempos_detalhados.csv
 *       algoritmo,tamanho,repeticao,tempo_s   (uma linha por execucao)
 */

#define _POSIX_C_SOURCE 199309L

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "algoritmos.h"

#define NUM_REPETICOES 100
#define NUM_AQUECIMENTO 3
#define AQUECIMENTO_GLOBAL_S 0.3 /* segundos de CPU ocupada antes de cada tamanho */
#define NUM_TAMANHOS 5

static const int TAMANHOS[NUM_TAMANHOS] = {1000, 50000, 100000, 500000, 1000000};

/*
 * Assinatura unica para os 4 algoritmos: recebe o vetor, n e o valor
 * buscado (ignorado pelo inverter). Retorna um int qualquer.
 */
typedef int (*Algoritmo)(int v[], int n, int x);

/* Adaptador para o inverter caber na assinatura Algoritmo. */
static int chamarInverter(int v[], int n, int x) {
    (void) x;
    inverter(v, n);
    return 0;
}

static int *lerVetor(const char *caminho, int *n_out) {
    FILE *f = fopen(caminho, "r");
    if (f == NULL) {
        fprintf(stderr, "Erro ao abrir %s\n", caminho);
        return NULL;
    }

    int n;
    if (fscanf(f, "%d", &n) != 1) {
        fclose(f);
        return NULL;
    }

    int *v = (int *) malloc((size_t) n * sizeof(int));
    for (int i = 0; i < n; i++) {
        if (fscanf(f, "%d", &v[i]) != 1) {
            fprintf(stderr, "Arquivo %s truncado\n", caminho);
            free(v);
            fclose(f);
            return NULL;
        }
    }

    fclose(f);
    *n_out = n;
    return v;
}

/* Relogio de alta resolucao (segundos), para medir uma unica chamada. */
static double agoraSegundos(void) {
    struct timespec ts;
    clock_gettime(CLOCK_MONOTONIC, &ts);
    return (double) ts.tv_sec + (double) ts.tv_nsec / 1e9;
}

/*
 * Mantem o processador ocupado por AQUECIMENTO_GLOBAL_S segundos antes
 * de medir um tamanho. Sem isso, os tamanhos pequenos (que sao medidos
 * primeiro e terminam em poucos milissegundos) rodam com o nucleo ainda
 * em frequencia baixa, e os grandes rodam ja com o nucleo em frequencia
 * maxima - o que fazia o custo por elemento parecer cair pela metade
 * entre n = 100.000 e n = 500.000. As NUM_AQUECIMENTO chamadas
 * (microssegundos) nao bastam para o governador de frequencia reagir.
 */
static void aquecerProcessador(void) {
    volatile unsigned long x = 0;
    double inicio = agoraSegundos();
    while (agoraSegundos() - inicio < AQUECIMENTO_GLOBAL_S) {
        for (int k = 0; k < 1000; k++) {
            x += (unsigned long) k;
        }
    }
    (void) x;
}

/*
 * Mede um algoritmo sobre um vetor de tamanho n:
 *   1. NUM_AQUECIMENTO chamadas nao cronometradas;
 *   2. NUM_REPETICOES chamadas cronometradas individualmente, cada
 *      uma gravada em arqDetalhado (algoritmo,tamanho,repeticao,tempo_s);
 *   3. uma chamada extra com os contadores zerados, para a contagem
 *      de operacoes no pior caso.
 * Escreve a linha-resumo em arqResumo.
 *
 * O algoritmo recebe uma COPIA do vetor original, pois o inverter o
 * modifica. As buscas nao modificam nada, mas usar a copia para todos
 * mantem o codigo uniforme.
 */
static void medirAlgoritmo(const char *nome, Algoritmo alg,
                           const int *original, int n, int alvo,
                           FILE *arqResumo, FILE *arqDetalhado) {
    int *v = (int *) malloc((size_t) n * sizeof(int));
    memcpy(v, original, (size_t) n * sizeof(int));

    for (int r = 0; r < NUM_AQUECIMENTO; r++) {
        alg(v, n, alvo);
    }

    double soma = 0.0;
    for (int r = 0; r < NUM_REPETICOES; r++) {
        double t0 = agoraSegundos();
        alg(v, n, alvo);
        double t1 = agoraSegundos();
        double dt = t1 - t0;

        soma += dt;
        fprintf(arqDetalhado, "%s,%d,%d,%.9f\n", nome, n, r + 1, dt);
    }
    double tempoMedio = soma / NUM_REPETICOES;

    g_comparacoes = 0;
    g_atribuicoes = 0;
    alg(v, n, alvo);
    fprintf(arqResumo, "%s,%d,%.9f,%ld,%ld\n", nome, n, tempoMedio, g_comparacoes, g_atribuicoes);

    free(v);
}

static void medirTamanho(int n, FILE *arqResumo, FILE *arqDetalhado) {
    char caminho[64];
    snprintf(caminho, sizeof(caminho), "dados/dados_%d.txt", n);

    int nLido;
    int *v = lerVetor(caminho, &nLido);
    if (v == NULL) {
        fprintf(stderr, "Pulando tamanho %d (nao consegui ler %s)\n", n, caminho);
        return;
    }

    int alvoAusente = v[nLido - 1] + 1; /* pior caso: elemento nao existe no vetor */

    aquecerProcessador();
    medirAlgoritmo("inverter", chamarInverter, v, nLido, alvoAusente, arqResumo, arqDetalhado);
    medirAlgoritmo("buscaSequencial", buscaSequencial, v, nLido, alvoAusente, arqResumo, arqDetalhado);
    medirAlgoritmo("buscaBinariaIterativa", buscaBinariaIterativa, v, nLido, alvoAusente, arqResumo, arqDetalhado);
    medirAlgoritmo("buscaBinariaRecursiva", buscaBinariaRecursiva, v, nLido, alvoAusente, arqResumo, arqDetalhado);

    fflush(arqResumo);
    fflush(arqDetalhado);
    fprintf(stderr, "Tamanho %d medido.\n", nLido);
    free(v);
}

int main(void) {
    const char *nomeResumo = "resultados/resultado_completo.csv";
    const char *nomeDetalhado = "resultados/tempos_detalhados.csv";

    FILE *arqResumo = fopen(nomeResumo, "w");
    if (arqResumo == NULL) {
        fprintf(stderr, "Erro ao criar %s (a pasta 'resultados/' existe?)\n", nomeResumo);
        return 1;
    }
    fprintf(arqResumo, "algoritmo,tamanho,tempo_medio_s,comparacoes_pior_caso,atribuicoes_pior_caso\n");

    FILE *arqDetalhado = fopen(nomeDetalhado, "w");
    if (arqDetalhado == NULL) {
        fprintf(stderr, "Erro ao criar %s (a pasta 'resultados/' existe?)\n", nomeDetalhado);
        fclose(arqResumo);
        return 1;
    }
    fprintf(arqDetalhado, "algoritmo,tamanho,repeticao,tempo_s\n");

    for (int i = 0; i < NUM_TAMANHOS; i++) {
        medirTamanho(TAMANHOS[i], arqResumo, arqDetalhado);
    }

    fclose(arqResumo);
    fclose(arqDetalhado);
    printf("Resultado salvo em %s e %s\n", nomeResumo, nomeDetalhado);
    return 0;
}
