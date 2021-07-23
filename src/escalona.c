#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <typedefs.h>
#include <conflict_detection.h>
#include <equivalent_view.h>

/**
 * @brief Verifica se todas as transações de um escalonamento acabaram 
 * O objetivo é verificar se deve ser criado um novo escalonamento
 * @param t - Transação atual
 */
bool allEnded(transaction *t)
{
    if (t == NULL)
    {
        return true;
    }
    if (t->last_input->operation == commit)
    {
        return allEnded(t->next);
    }
    return false;
}

/**
 * @brief Adiciona um input a uma transação
 * 
 * @param t - Transação atual
 * @param i - Input
 * @return transaction* 
 */
transaction *addToTransaction(transaction *t, input *i)
{
    i->next = NULL;
    if (t == NULL)
    {
        i->last = NULL;

        transaction *t = malloc(sizeof(transaction));
        t->id = i->id;
        t->last_input = i;
        t->edges = NULL;
        t->next = NULL;
        t->viewed = 0;
        return t;
    }
    if (t->id == i->id)
    {
        i->last = t->last_input;
        t->last_input = i;
    }
    else
    {
        t->next = addToTransaction(t->next, i);
    }
    return t;
}

/**
 * @brief Cria os escalonamentos
 * Deve ser chamada com o primeiro parâmetro sendo vazio.
 * @param s - Escalonamento atual
 * @param id - Id do escalonamento atual
 * @return scaling* 
 */
scaling *readLines(scaling *s, int id)
{
    if (s == NULL)
    {
        s = malloc(sizeof(scaling));
        s->id = id;
        s->t = NULL;
        s->next = NULL;
    }
    input *i = malloc(sizeof(input));
    if (scanf("%d %d %c %c", &i->time, &i->id, (char *)&i->operation, &i->attr) != EOF)
    {

        s->t = addToTransaction(s->t, i);

        if (allEnded(s->t))
        {
            s->next = readLines(s->next, id + 1);
        }
        else
        {
            s = readLines(s, id);
        }
        return s;
    }
    return NULL;
}

/**
 * @brief Imprime o resultado
 * 
 * @param s 
 */
void printScalingInfo(scaling *s)
{
    if (s == NULL)
        return;
    printf("%d ", s->id);
    transaction *t = s->t;
    while (t != NULL)
    {
        if (t->next == NULL)
        {
            printf("%d ", t->id);
        }
        else
        {
            printf("%d,", t->id);
        }
        t = t->next;
    }

    if (haveCicles(s->t))
    {
        printf("NS ");
    }
    else
    {
        printf("SS ");
    }


    if (isViewEquivalent(s))
    {
        printf("SV\n");
    }
    else
    {
        printf("NV\n");
    }
    printScalingInfo(s->next);
}

int main(int argc, char const *argv[])
{
    scaling *s = readLines(NULL, 1);
    createEdges(s);

    printScalingInfo(s);

    return 0;
}
