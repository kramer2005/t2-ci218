# Escalonamento de transações - T2 CI218

## Implementado por: Wagner J. Kramer Vieira

### GRR20186095

## Objetivo

### Implementar os algoritmos

- Teste de seriabilidade por conflito
- Teste de visão equivalente

## Observações

- O trabalho foi implementado em inglês porquê não gosto de misturar os idiomas durante a implementação, me limitei somente aos comentários em português e o nome do arquivo principal.
- O algoritmo de detecção de ciclos saiu inteiro da minha cabeça, visto que eu ainda não fiz grafos. Eu acredito que estaja correto, mas pode ser que exista um caso em que não funcione.
- A maioria das estruturas de dados é implementada em forma de lista encadeada ou duplamente encadeada, somente no algoritmo de visão equivalente é usado um array (agora fazendo o relatório eu acredito que poderia ser uma lista também, mas no momento o array foi mais rápido)
- Quando falo de "transação", estou me referindo a lista de operações com o mesmo identificador.

***

## Execução

Basta compilar, basta rodar:

```shell
make
```

Isto gerará o arquivo **escalona**, que deve ser utilizado seguindo as [especificações](https://www.inf.ufpr.br/eduardo/ensino/ci218/trabalhos/escalona/escalona.html).

## Limpeza

Para limpar a pasta, basta executar:

```shell
make clean
```

***

## Implementação

### Leitura e estruturação dos dados

Ao serem lidos, os dados já são separados em escalonamentos, sendo que sempre que todas as operações de uma transação acaba (todos commitam), é encerrado o escalonamento e criado um novo para continuar a leitura.

### Criação de arestas

Para cada transação em $T_j$, é verificado se a última operação em $T_i$ antes de $T_j$ satisfaz uma das propriedades para se cria uma aresta:

- Aresta $T_i$ -> $T_j$ para cada $r(x)$ em $T_j$ depois de $w(x)$ em $T_i$
- Aresta $T_i$ -> $T_j$ para cada $w(x)$ em $T_j$ depois de $r(x)$ em $T_i$
- Aresta $T_i$ -> $T_j$ para cada $w(x)$ em $T_j$ depois de $w(x)$ em $T_i$

### Detecção de ciclos

- Todas as transações começam com a propriedade ```viewed``` sendo 0
- Cria-se uma variável que representa um iterador, iniciando em 1
- Itera-se sobre todas as transações do escalonamento, sempre que é encontrado uma transação com a propriedade ```viewed``` igual a zero, adiciona-se 1 no iterador, seta-se o valor da aresta como o valor atual do iterador e segue-se todas as arestas da transação, quando a aresta conecta uma transação a outra com o mesmo valor na propriedade ```viewed```, significa que aquela transação já foi vista, logo se tem um ciclo.
- O escalonamento é serial se não existe ciclo no grafo.

![alt text](https://github.com/kramer2005/escalona/blob/master/ciclo-grafo.jpg?raw=true)

### Visão equivalente

- Detecta-se qual a última operação feita no escalonamento
- Remove-se todos os commits, pois não são mais necessários
- Duas informações principais são passadas recursivamente para a função que executa o algoritmo de visão equivalente
  - Quais são as transações sendo utilizadas na permutação
  - Quais são as propriedades que já foram lidas
- Para cada transação

