#include <climits>
#include <cstdbool>
#include <cstdlib>
#include <iostream>
#include <string>

using namespace std;

struct GrafoMatrizAdj {
	int** arestas;
	char** rotuloVertices;
	int verticesInseridos;
	int maxNumVertices;
	//faz mais sentido ser apenas numVertices (consertar para a prox turma)
};

/**
 * Se o grafo for ponderado, inicializamos cada posicao da matriz com INT_MAX.
 * Se o grafo for não ponderado, inicializamos cada posicao da matriz com 0.
 **/
struct GrafoMatrizAdj* inicializar(int numVertices, bool ponderado) {
	GrafoMatrizAdj* g = new GrafoMatrizAdj;
	g->maxNumVertices = numVertices;
	g->rotuloVertices = new char*[numVertices];
	g->verticesInseridos = 0;

	// Memory allocation
	g->arestas = new int*[numVertices];

	for (int i = 0; i < numVertices; i++) {
		g->arestas[i] = new int[numVertices];
	}

	// Matrix inicialization
	for (int i = 0; i < numVertices; i++) {
		for (int j = 0; j < numVertices; j++) {
			g->arestas[i][j] = ponderado ? INT_MAX : 0;
		}
	}

	return g;
}

int main() {

	GrafoMatrizAdj* g;
	g = inicializar(3, true);
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 3; j++) {
			cout << g->arestas[i][j] << " ";
		}
		cout << endl;
	}
}