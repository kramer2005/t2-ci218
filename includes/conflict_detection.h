/**
 * @file conflict_detection.h
 * @author Wagner Kramer (wjkv18@inf.ufpr.br)
 * @brief Definição das funções que o usuário utilizará para teste de seriabilidade por conflito
 */

#ifndef CONFLICT_SERIALIBILITY_DETECTION
#define CONFLICT_SERIALIBILITY_DETECTION

/**
 * @brief Determina se existem ciclos no escalonamento
 * 
 * @param t - Primeira transação do escalonamento
 */
bool haveCicles(transaction *t);

/**
 * @brief Cria as arestas do escalonamento
 * 
 * @param s - Escalonamento
 */
void createEdges(scaling *s);

#endif
