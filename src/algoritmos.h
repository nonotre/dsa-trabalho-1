/*
 * Trabalho 1 - Estruturas de Dados I - ICMC-USP - 2026
 *
 * Prototipos dos 4 algoritmos exigidos pelo enunciado.
 * Usado tanto pelo programa de menu (trabalho1.c, entregue no
 * Run.Codes) quanto pelo codigo auxiliar de benchmark (contagem de
 * tempo e de operacoes, submetido separadamente no e-disciplinas).
 */

#ifndef ALGORITMOS_H
#define ALGORITMOS_H

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

#endif
