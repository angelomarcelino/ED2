#include <limits.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

struct GrafoMatrizAdj {
	int** arestas;
	char** rotuloVertices;
	int verticesInseridos;
	int maxNumVertices;
	//faz mais sentido ser apenas numVertices (consertar para a prox turma)
};

/** DONE
 * Se o grafo for ponderado, inicializamos cada posicao da matriz com INT_MAX.
 * Se o grafo for não ponderado, inicializamos cada posicao da matriz com 0.
 **/
struct GrafoMatrizAdj* inicializar(int numVertices, bool ponderado) {
	GrafoMatrizAdj* g = (struct GrafoMatrizAdj*)malloc(sizeof(struct GrafoMatrizAdj));
	g->maxNumVertices = numVertices;
	g->rotuloVertices = (char**)malloc(numVertices * sizeof(char*));
	g->verticesInseridos = 0;

	// Memory allocation
	g->arestas = (int**)malloc(numVertices * sizeof(int*));

	for (int i = 0; i < numVertices; i++) {
		g->arestas[i] = (int*)malloc(numVertices * sizeof(int));
	}

	// Matrix inicialization
	for (int i = 0; i < numVertices; i++) {
		for (int j = 0; j < numVertices; j++) {
			g->arestas[i][j] = ponderado ? INT_MAX : 0;
		}
	}

	return g;
}

/** DONE
 * A principio nao temos nenhuma ordenacao usando os rotulos.
 * Portanto, usaremos busca linear
 **/
int obterIndiceVertice(struct GrafoMatrizAdj* grafo, char* rotuloVertice) {
	for (int i = 0; i < grafo->maxNumVertices; i++) {
		if (grafo->rotuloVertices[i] == rotuloVertice) return i;
	}
	printf("Erro! Rótulo %s inexistente\n", rotuloVertice);
	return -1;
}

/** DONE
 * Se o grafo for ponderado, usamos a variavel peso para especificar o peso da aresta.
 * Se o grafo for não ponderado, passaremos o valor 1 para a variavel peso, de modo que represente existência da aresta.
 * Se um dos rotulos dos vertices nao existir, simplesmente nao faremos nada.
 **/
void inserirAresta(struct GrafoMatrizAdj* grafo, char* rotuloVOrigem, char* rotuloVDestino, int peso) {
	int orig = obterIndiceVertice(grafo, rotuloVOrigem);
	int dest = obterIndiceVertice(grafo, rotuloVDestino);

	if (orig == -1 || dest == -1) {
		return;
	} else {
		grafo->arestas[orig][dest] = peso;
		grafo->arestas[dest][orig] = peso;
	}
	return;
}

/** DONE
 * Usamos a variavel grafo->verticesInseridos para controlar a quantidade de vertices que ja foi inserido no grafo.
 * Logo, ela pode/deve ser usada para especificar em qual posicao o vertice a ser inserido sera alocado.
 **/
void inserirVertice(struct GrafoMatrizAdj* grafo, char* rotuloVertice) {
	if (grafo->verticesInseridos > grafo->maxNumVertices) {
		printf("Erro! Número máximo de vértices atingido %d > %d\n", grafo->verticesInseridos, grafo->maxNumVertices);
		return;
	} else {
		grafo->rotuloVertices[grafo->verticesInseridos] = rotuloVertice;
		grafo->verticesInseridos++;
		return;
	}
}

/**
 * Tomar cuidado com as diferenças semânticas entre haver conexão entre dois vertices em um grafo ponderado,
 * e haver conexão entre dois vertices em um grafo nao-ponderado.
 **/
bool saoConectados(struct GrafoMatrizAdj* grafo, char* rotuloVOrigem, char* rotuloVDestino) {

	int orig = obterIndiceVertice(grafo, rotuloVOrigem);
	int dest = obterIndiceVertice(grafo, rotuloVDestino);

	if (orig == -1 || dest == -1) {
		return false;
	} else {
		int value = grafo->arestas[orig][dest];
		return (value == 0 || value == INT_MAX) ? false : true;
	}
}

void printGraph(struct GrafoMatrizAdj* graph) {
	printf("->maxNumVertices: %d\n", graph->maxNumVertices);
	printf("->verticesInseridos: %d\n", graph->verticesInseridos);

	printf("->Vertices: \n");
	for (int i = 0; i < graph->verticesInseridos; i++) {
		printf(" %s ", graph->rotuloVertices[i]);
	}

	printf("\n->Matriz: \n");
	for (int i = 0; i < graph->maxNumVertices; i++) {
		printf("\t| ");
		for (int j = 0; j < graph->maxNumVertices; j++) {
			if (j == graph->maxNumVertices - 1) {
				printf("%d |\n", (graph->arestas[i][j] == INT_MAX) ? 0 : graph->arestas[i][j]);
			} else {
				printf("%d ", (graph->arestas[i][j] == INT_MAX) ? 0 : graph->arestas[i][j]);
			}
		}
	}
}