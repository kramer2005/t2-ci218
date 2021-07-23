/**
 * @file equivalent_view.c
 * @author Wagner Kramer (wjkv18@inf.ufpr.br)
 * @brief Implementação das funções para teste de visão equivalente
 */

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <typedefs.h>
#include <equivalent_view.h>

/**
 * @brief Encontra a última operação do escalonamento
 * 
 * @param t - Primeira transação do escalonamento
 * @return op 
 */
op findLastInputOP(transaction *t)
{
    input *lastInput = t->last_input;
    t = t->next;
    while (t != NULL)
    {
        if (t->last_input->time > lastInput->time)
        {
            lastInput = t->last_input;
        }
        t = t->next;
    }
    return lastInput->operation;
}

readen *addToReaden(readen *r, char value)
{
    if (r == NULL)
    {
        r = malloc(sizeof(readen));
        r->value = value;
        r->next = NULL;
        return r;
    }
    if (r->value == value)
        return r;
    r->next = addToReaden(r->next, value);
    return r;
}

bool wasReaden(readen *r, char value, readen *lastReaden)
{
    if (r == NULL)
    {
        return false;
    }
    if (r->value == value)
    {
        return true;
    }
    if (r == lastReaden)
    {
        return false;
    }
    return wasReaden(r->next, value, lastReaden);
}

/**
 * @brief Executa o algoritmo de visão equivalente
 * 
 * @param t - Primeira transação do escalonamento 
 * @param arr - As transações que já estão sendo utilizadas
 * @param used - Quantidade de transações utilizadas
 */
bool detectViewEquivalent(transaction *t, transaction *arr[], size_t used, readen *r, op lastOp, bool *ended)
{
    transaction *actualArr[used + 1];
    for (size_t i = 0; i < used; i++)
    {
        actualArr[i] = arr[i];
    }
    transaction *actualT = t;
    readen *lastReaden = r;
    readen *aux = r;
    while (aux != NULL)
    {
        lastReaden = aux;
        aux = aux->next;
    }
    while (actualT != NULL)
    {
        bool viewed = false;
        for (size_t i = 0; i < used; i++)
        {
            if (actualT == actualArr[i])
            {
                viewed = true;
            }
        }

        if (!viewed)
        {
            input *actualI = actualT->last_input;
            actualArr[used] = actualT;
            while (actualI != NULL)
            {
                if (actualI->operation == read)
                {
                    r = addToReaden(r, actualI->attr);
                }
                else
                {
                    if (wasReaden(r, actualI->attr, lastReaden))
                    {
                        return false;
                    }
                }
                actualI = actualI->last;
            }
            if (detectViewEquivalent(t, actualArr, used + 1, r, lastOp, ended))
            {
                if (*ended)
                {
                    *ended = false;
                    if (lastOp == write && actualT->last_input->operation == write)
                        return true;
                    return false;
                } else {
                    return true;
                }
            }
        }

        actualT = actualT->next;
        if (lastReaden != NULL)
        {
            lastReaden->next = NULL;
        }
    }
    *ended = true;
    return true;
}

void removeCommit(transaction *t)
{
    while (t != NULL)
    {
        t->last_input = t->last_input->last;
        t->last_input->next = NULL;
        t = t->next;
    }
}

/**
 * @brief Prepara o algoritmo de visão equivalente
 * @param t - Primeira transação do escalonamento
 */
bool isViewEquivalent(scaling *s)
{
    transaction *arr[1];
    size_t used = 0;
    removeCommit(s->t);
    bool ended = false;
    return detectViewEquivalent(s->t, arr, used, NULL, findLastInputOP(s->t), &ended);
}
