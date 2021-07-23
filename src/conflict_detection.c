/**
 * @file conflict_detection.c
 * @author Wagner Kramer (wjkv18@inf.ufpr.br)
 * @brief Implementação das funções para teste de seriabilidade por conflito
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <typedefs.h>
#include <conflict_detection.h>

/**
 * @brief Cria uma aresta
 * 
 * @param e - Aresta atual
 * @param t - Transação B que a aresta vai ligar a A
 * @return edge* 
 */
edge *addEdge(edge *e, transaction *t)
{
    if (e == NULL)
    {
        e = malloc(sizeof(edge));
        e->t = t;
        e->next = NULL;
        return e;
    }
    if (e->t != t)
    {
        e->next = addEdge(e->next, t);
    }
    return e;
}

/**
 * @brief Determina se deve ser criada uma aresta entre as transações
 * 
 * @param i - Inputs da primeira transação
 * @param j - Inputs da segunda transação
 */
bool mustCreateEdge(input *i, input *j)
{
    input *actualJ = j;
    while (actualJ != NULL)
    {
        if (actualJ->operation != commit)
        {
            input *actualI = i;
            while (actualI != NULL)
            {
                if (actualI->operation != commit)
                {
                    if (actualJ->attr == actualI->attr)
                    {
                        if (actualJ->time > actualI->time)
                        {
                            if (
                                (actualJ->operation == read && actualI->operation == write) ||
                                (actualJ->operation == write && actualI->operation == read) ||
                                (actualJ->operation == write && actualI->operation == write))
                            {
                                return true;
                            }
                            actualI = NULL;
                            break;
                        }
                    }
                }
                actualI = actualI->last;
            }
        }
        actualJ = actualJ->last;
    }
    return false;
}

/**
 * @brief Cria as arestas do escalonamento
 * 
 * @param s - Escalonamento
 */
void createEdges(scaling *s)
{
    if (s == NULL)
    {
        return;
    }
    transaction *actual = s->t;
    while (actual != NULL)
    {
        transaction *cmp = s->t;
        while (cmp != NULL)
        {
            if (cmp != actual)
            {
                if (mustCreateEdge(actual->last_input, cmp->last_input))
                {
                    actual->edges = addEdge(actual->edges, cmp);
                }
            }
            cmp = cmp->next;
        }

        actual = actual->next;
    }
    createEdges(s->next);
}

/**
 * @brief Segue as arestas até encontrar (ou não), ciclos
 * @param t - Transação atual
 * @param iteration - Iteração atual
 */
bool singleCicleIteration(transaction *t, int iteration)
{
    if (t == NULL)
    {
        return false;
    }
    edge *e = t->edges;
    bool ret = false;
    t->viewed = iteration;
    while (e != NULL)
    {
        if (e->t->viewed == iteration)
        {
            return true;
        }
        ret = singleCicleIteration(e->t, iteration);
        if (ret)
        {
            return true;
        }
        e = e->next;
    }
    return ret;
}

/**
 * @brief Detecta se o escalonamento tem ciclos
 * 
 * @param t - Primeira transação do escalonamento
 */
bool haveCicles(transaction *t)
{
    int iteration = 1;
    bool ret = false;
    while (t != NULL)
    {
        if (t->viewed == 0)
        {
            ret = singleCicleIteration(t, iteration);
        }
        iteration++;
        t = t->next;
    }
    return ret;
}
