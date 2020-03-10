# bellman_ford_otimization
Code Apps Repository

O algoritmo de Bellman-Ford resolve o problema do caminho mais curto de única origem para o caso mais geral, no qual os pesos das arestas podem ser negativos. Dado um grafo orientado ponderado <i>G = (V, E)</i> com origem em s e função peso w:E → R, o algoritmo retorna FALSE quando encontra um ciclo de peso negativo indicando que não existe solução, ou retorna TRUE indicando que produziu os caminhos mais curtos e seus pesos. O algoritmo usa a técnica do relaxamento, diminuindo progressivamente a estimativa <i>d[v]</i> no peso de um caminho mais curto.

O algoritmo começa iniciando as estimativas <i>d[v]</i> nos pesos dos caminhos mais curtos com infinito e os predecessores <i>π [v]</i> com NIL, através do procedimento INITIALIZE-SINGLE-SOURCE. 

Referências bibliográficas: (livro texto) Cormen, T.H.;Leiserson, C.E.; Rivest, R.L.; Stein, C.; Algoritmos. Tradução da 2ª edição americana Teoria e Prática, 2002.
