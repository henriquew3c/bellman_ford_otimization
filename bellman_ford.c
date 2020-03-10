#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if defined(__APPLE__)
#include <time.h>
#include <unistd.h>
#endif

#if !defined(__APPLE__)
#include <malloc.h>
#endif

#ifdef _WIN32
#include <Windows.h>
#endif

#define laranja  0
#define azul     1
#define amarelo  2
#define verde    3
#define vermelho 4
#define preto    5
#define branco   6

int antiga_solucao=0;
// estrutura que define um vÈrtice
typedef struct vert
{
    char nome[30];
    int valor;
    int cor; // identificador da cor
    struct vert *prox; // encadeamento da lista de vÈrtices
    struct vert *adjacencia[100]; // vetor de ponteiros para os vertices que s„o adjacentes a ele
    int nadj; // quantidade de adjacencias (ou grau) do vertice

    // Bellman-ford
    int custo;
    struct vert *pred;
}vertice;

// estrutura que define as arestas
typedef struct arest
{
    vertice *origem;  // ponteiro para o vertice de origrm da aresta
    vertice *destino; // ponteiro para o vertice de destino da aresta
    int peso;  // peso
    struct arest *prox; // encadeamento da lista de arestas
}aresta;

// estrutura que define o grafo como sendo um conjunto de vÈrtices e um conjunto de arestas
typedef struct
{
    vertice *V;
    vertice *V_fim;
    aresta *A;
    aresta *A_fim;
    int nvertices;
}grafo;

// inicializa a lista de vÈrtices e a lista de arestas
void inicializa(grafo *g)
{
    g->V = NULL;
    g->A = NULL;
    g->nvertices = 0;
}

// insere um novo vÈrtice em um grafo g - par‚metros: o grafo, o nome do vÈrtice e um valor numÈrico v
int new_v(grafo *g, char str[30], int v)
{
    vertice *aux; // armazena temporariamente o novo vertice do grafo
    
    aux = (vertice *)malloc(sizeof(vertice));
    if(aux == NULL) // erro de alocaÁ„o
    return -1;
    
    aux->valor = v;            // copia o valor
    strcpy(aux->nome,str);     // copia o nome
    aux->cor = -1;             // inicialmente sem cor
    aux->nadj = 0;            // inicialmente sem adjacÍncias
    //aux->prox = g->V;          // encadeamento na lista de vÈrtices
    //g->V = aux;                // atualizaÁ„o do inÌcio da lista de vÈrtices
    
    if (g->V == NULL) {
        aux->prox = NULL;
        g->V = aux;
        g->V_fim = aux;
    }
    else {
        aux->prox = NULL;
        g->V_fim->prox = aux;
        g->V_fim = aux;
    }

    g->nvertices++; // atualiza a quantidade de vÈrtices
    
    return 1;               // valor de retorno para inserÁ„o realizada com sucesso
}

// busca em um grafo G um vÈrtice com valor V
vertice *busca(grafo *g, int v)
{
    vertice *aux;  // ponteiro que ir· percorrer a lista de vÈrtices
    
    aux = g->V;                // inicialmente receber· o primeiro vÈrtice
    while(aux != NULL)         // enquanto n„o chegar ao final da lista de vertices
    {
        if(aux->valor == v)      // verifica se o valor do vertice atual È igual ao valor procurado
        return aux;            // se achou o vertice com valor v retorna um ponteiro para o mesmo
        aux = aux->prox;         // caso n„o tenha achado, continua procurando
    }
    return NULL;  // vertice com valor v nao encontrado apos percorrer toda a lista de vertices
}


// insere uma nova aresta no grafo. Par‚metros: o grafo, o vertice de origem, o vertice de destino, o peso da aresta
int new_a(grafo *g,int v_orig, int v_dest, int p)
{
    aresta *aux_a;  // ponteiro tempor·rio para a nova aresta
    
    aux_a = (aresta*)malloc(sizeof(aresta));  // alocaÁ„o din‚mica
    if(aux_a == NULL)
    return 0;  // erro de alocaÁ„o de memÛria
    
    aux_a->origem = busca(g,v_orig);  // chama a funÁ„o busca para atribuir o vertice de origem da aresta
    if(aux_a->origem == NULL)  // verifica se a busca retornou NULL
    {
        free(aux_a);            // se a busca retornou NULL o vertice de origem n„o foi encontrado
        return -1;              // "desaloca" a memÛria para a aresta e retorna -1
    }                          // que È o cÛdigo para vertice de origem n„o encontrado
    
    
    aux_a->destino = busca(g,v_dest);  // operaÁ„o similar para o vÈrtice de destino da aresta
    if(aux_a->destino == NULL)
    {
        free(aux_a);
        return -2; // vertice de destino nao encontrado
    }
    
    aux_a->peso = p;  // atribui o peso
    
    //aux_a->prox = g->A;  // encadeamento na lista de arestas
    //g->A = aux_a;        // completa o encadeamento
    
    if (g->A == NULL) {
        aux_a->prox = NULL;
        g->A = aux_a;
        g->A_fim = aux_a;
    }
    else {
        aux_a->prox = NULL;
        g->A_fim->prox = aux_a;
        g->A_fim = aux_a;
    }
    
    // atualiza o grau e o vetor de adjacencias do vertice de origem
    aux_a->origem->nadj = aux_a->origem->nadj + 1;
    aux_a->origem->adjacencia[aux_a->origem->nadj] = aux_a->destino;
    
    aux_a->destino->nadj = aux_a->destino->nadj + 1;
    aux_a->destino->adjacencia[aux_a->destino->nadj] = aux_a->origem;
    
    
    return 1; // SUCESSO !!!!!
}

//  imprime a lista de vÈrtices do grafo
void print_v(grafo g)
{
    vertice *aux;
    aux = g.V;
    
    while(aux != NULL)
    {
        //printf("%s-%d-%d; ",aux->nome, aux->valor, aux->cor);
        printf("\n   %s -> [%d] [%s]; ",aux->nome, aux->custo, aux->pred->nome);
        aux = aux->prox;
    }
}

// imprime a lista de arestas do grafo
void print_a(grafo g)
{
    aresta *aux;
    
    aux = g.A;
    
    printf("\n\n");
    while(aux != NULL)
    {
        printf("%d->%d,%d,  ",aux->origem->valor,aux->destino->valor,aux->peso);
        aux = aux->prox;
    }
    printf("\n\n");
}

// imprime o vetor de adjacencias de um determinado vÈrtice
void imprime_adj_v(vertice *v1)
{
    int i;
    
    printf("%d (%d) = ",v1->valor,v1->nadj+1);
    for(i=0; i<= v1->nadj; i++)
    {
        printf("%d, ",v1->adjacencia[i]->valor);
    }
    printf("\n");
}

// imprime a lista de adjacencias de todos os vÈrtices
void imprime_adj_g(grafo g1)
{
    vertice *ptr;
    
    ptr = g1.V;
    printf("Adjacencias\n");
    while(ptr != NULL)
    {
        imprime_adj_v(ptr);
        ptr = ptr->prox;
    }
}

int le_grafo(grafo *g, char *filename)
{
    FILE *file;
    int valor, origem=0, destino, peso;
    char nome[30] = "";
    char linha[80];
    //char ch = '1';
    
    file = fopen(filename,"r");
    
    if(file == NULL)
    return -1;
    
    // printf("\n\nLendo arquivo do grafo\n\n");
    
    fscanf(file, "%s", linha); //printf("%s\n",linha);
    fscanf(file, "%s", linha); //printf("%s\n",linha);
    fscanf(file, "%s", linha); //printf("%s\n",linha);
    
    fscanf(file, "%s", nome);
    while( strcmp(nome,"FIM") != 0)
    {
        fscanf(file, "%d", &valor);
        new_v(g,nome,valor);
        //printf("%s - %d\n", nome, valor);
        fscanf(file, "%s", nome);
    }
    
    printf("\nVertices carregadas com sucesso!");
    
    fscanf(file, "%s", linha); //printf("%s",linha);
    fscanf(file, "%s", linha); //printf("%s",linha);
    fscanf(file, "%s", linha); //printf("%s",linha);
    fscanf(file, "%s", linha); //printf("%s\n",linha);
    
    sleep(2);
    
    fscanf(file, "%d", &origem);
    while( origem != -1)
    {
        fscanf(file, "%d", &destino);
        fscanf(file, "%d", &peso);
        new_a(g,origem,destino,peso);
        //printf("%d - %d - %d\n",origem, destino, peso);
        fscanf(file, "%d", &origem);
    }
    printf("\nArestas carregadas com sucesso!\n\n");
    fclose(file);
    return 1;  // sucesso
}

// Calcula o menor custo a partir de um vértice de origem
int bellman_ford(grafo *g, int origem) {
    
    vertice *vert;
    vertice *v_origem;
    vertice *v_destino;
    aresta *aresta;
    int infinito = 1000;
    int i;

    // Inicialização
    vert = g->V;
    aresta = g->A;

    while (vert != NULL) {
	
	    if (vert->valor == origem) // Se for o vértice de origem
	       vert->custo = 0;
        else
	       vert->custo = infinito;
	    
        vert->pred = NULL;
        vert = vert->prox; // Vai para o próximo vértice
    }
    
    // Relaxamento
    for (i=0; i<g->nvertices; i++) {
    	while (aresta != NULL) {

    		v_origem = aresta->origem;
    		v_destino = aresta->destino;

    		if ((v_origem->custo + aresta->peso) < v_destino->custo) {
    			v_destino->pred = v_origem;
    			v_destino->custo = v_origem->custo + aresta->peso;
    		}

    		aresta = aresta->prox;
    	}
    }
    
    // Checagem de ciclos negativos
    aresta = g->A; // Aponta para a primeira aresta

    while (aresta != NULL) {

    	v_origem = aresta->origem;
    	v_destino = aresta->destino;

    	if ((v_origem->custo + aresta->peso) < v_destino->custo)
    		printf("Ciclo negativo!\n");

    	aresta = aresta->prox;
    }

    return 1;
}

int main(int argc, char **argv)
{    
    int retorno = 0;

    if(argc != 2){
        printf("Parametros incorretos. Use o parametro: ""<arquivo de grafo> \n");
        return -1;
    }
    
    // Inicialização do grafo
    grafo g;
    inicializa(&g);
    le_grafo(&g, argv[1]);
    
    retorno = bellman_ford(&g, 0);
    print_v(g);

    if (retorno == 1)
    	printf("\n\nFinalizado com sucesso!\n");
    else
    	printf("\nErro na execução da função!\n");
}
