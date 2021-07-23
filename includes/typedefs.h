/**
 * @file typedefs.h
 * @author Wagner Kramer (wjkv18@inf.ufpr.br)
 * @brief Definição das estruturas de dados necessários para o trabalho
 */

#include <stdio.h>
#ifndef TYPEDEFS_H
#define TYPEDEFS_H

struct transaction;
struct edge;

/**
 * @brief Definição das possíveis operações de entrada
 * 
 */
typedef enum
{
    read = 'R',
    write = 'W',
    commit = 'C'
} op;

/**
 * @brief Struct que armazenará os dados da entrada em forma de fila
 * 
 */
typedef struct input
{
    int time;
    int id;
    op operation;
    char attr;
    struct input *next;
    struct input *last;
} input;

/**
 * @brief Aresta conectando a->n
 * 
 */
typedef struct edge
{
    struct transaction *t;
    struct edge *next;
} edge;

/**
 * @brief Representação de uma transação
 * 
 */
typedef struct transaction
{
    int id;
    int viewed;
    struct input *last_input;
    struct edge *edges;
    struct transaction *next;
} transaction;

/**
 * @brief Representa um escalonamento
 * 
 */
typedef struct scaling
{
    int id;
    struct transaction *t;
    struct scaling *next;
} scaling;

/**
 * @brief Representa um valor lido em uma transação
 * 
 */
typedef struct readen
{
    char value;
    struct readen *next;
} readen;

#endif