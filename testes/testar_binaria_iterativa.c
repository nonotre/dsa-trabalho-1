/* Testes auxiliares da parte 3. Nao enviar este main ao Run.Codes. */
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../algoritmos.h"

static void testar_corretude(void) {
    int v[128], copia[128];
    long consultas = 0;

    /* Vetores estritamente crescentes e vetores com repeticoes. */
    for (int modo = 0; modo < 2; modo++) {
        for (int n = 1; n <= 128; n++) {
            for (int i = 0; i < n; i++) {
                v[i] = modo == 0 ? 2 * i - n : i / 3 - n / 6;
            }
            memcpy(copia, v, (size_t) n * sizeof(int));
            for (int x = -n - 1; x <= n + 1; x++) {
                int existe = 0;
                for (int i = 0; i < n; i++) {
                    if (v[i] == x) existe = 1;
                }
                int pos = buscaBinariaIterativa(v, n, x);
                assert((pos != -1) == existe);
                if (pos != -1) {
                    assert(pos >= 0 && pos < n);
                    assert(v[pos] == x);
                }
                assert(memcmp(v, copia, (size_t) n * sizeof(int)) == 0);
                consultas++;
            }
        }
    }
    int extremos[] = {INT_MIN, -1, 0, 1, INT_MAX};
    for (int i = 0; i < 5; i++) {
        assert(buscaBinariaIterativa(extremos, 5, extremos[i]) == i);
    }
    printf("Corretude: %ld consultas + extremos de int; vetor preservado.\n", consultas);
}

static void testar_contagens(void) {
    const int tamanhos[] = {1000, 50000, 100000, 500000, 1000000};
    const long comparacoes[] = {31, 49, 52, 58, 61};
    const long atribuicoes[] = {22, 34, 36, 40, 42};

    for (int t = 0; t < 5; t++) {
        int n = tamanhos[t];
        int *v = malloc((size_t) n * sizeof(int));
        assert(v != NULL);
        for (int i = 0; i < n; i++) v[i] = 2 * i;

        /* Sementes nao nulas verificam que a funcao nao zera contadores. */
        g_comparacoes = 7;
        g_atribuicoes = 11;
        assert(buscaBinariaIterativa(v, n, v[n - 1] + 1) == -1);
        assert(g_comparacoes == 7 + comparacoes[t]);
        assert(g_atribuicoes == 11 + atribuicoes[t]);

        g_comparacoes = 0;
        g_atribuicoes = 0;
        int meio = (n - 1) / 2;
        assert(buscaBinariaIterativa(v, n, v[meio]) == meio);
        assert(g_comparacoes == 2);
        assert(g_atribuicoes == 3);
        printf("n=%d: pior %ld/%ld; melhor 2/3 (comp/atrib): OK\n",
               n, comparacoes[t], atribuicoes[t]);
        free(v);
    }
}

int main(void) {
    testar_corretude();
    testar_contagens();
    puts("Todos os testes da busca binaria iterativa passaram.");
    return 0;
}
