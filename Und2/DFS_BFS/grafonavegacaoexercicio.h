#include <bits/stdc++.h>

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

class GrafoListaAdj {
   private:
	vector<string> vertices;

	//first eh o indice do vertice, second eh o peso (caso o grafo seja ponderado)
	vector<vector<pair<int, int>>> arestas;

	/*DONE*
    * A principio nao temos nenhuma ordenacao usando os rotulos.
    * Portanto, usaremos busca linear.
    **/
	int obterIndiceVertice(string rotuloVertice) {
		int index;
		auto it = find(vertices.begin(), vertices.end(), rotuloVertice);

		if (it != vertices.end()) {
			index = it - vertices.begin();
		} else {
			//cout << "Elemento não está presente no vetor\n";
			index = -1;
		}
		return index;
	}

	/*DOING*
    * O argumento indicesVerticesVisitados serve para controlar quais
    * vertices ja foram visitados.
    * Lembrando que DFS eh uma funcao recursiva.
    **/
	void dfs(string rotuloVOrigem, bool* indicesVerticesVisitados) {
		//IMPLEMENTAR
	}

   public:
	/*DONE*
    * Lembrem-se:
    *      1) nao podemos inserir vertices com mesmo rotulo
    *      2) toda vez que inserirmos um novo vertice, precisaremos
    *     inserir um vetor para representar as conexoes daquele
    *     vertice na lista de adjacencias
    **/
	void inserirVertice(string rotuloVertice) {
		if (obterIndiceVertice(rotuloVertice) == -1) {
			vertices.push_back(rotuloVertice);
			vector<pair<int, int>> vec;
			arestas.push_back(vec);
		} else {
			cerr << "Aresta " << rotuloVertice << " já existente\n";
		}
		return;
	}

	/**
    * Sempre que o grafo for nao ponderado, adicionaremos o peso 1,
    * por conveniencia.
    **/
	void inserirArestaDirecionada(string rotuloVOrigem, string rotuloVDestino) {
		inserirArestaDirecionada(rotuloVOrigem, rotuloVDestino, 1);
	}

	void inserirArestaNaoDirecionada(string rotuloVOrigem, string rotuloVDestino) {
		inserirArestaDirecionada(rotuloVOrigem, rotuloVDestino, 1);
		inserirArestaDirecionada(rotuloVDestino, rotuloVOrigem, 1);
	}

	void inserirArestaNaoDirecionada(string rotuloVOrigem, string rotuloVDestino, int peso) {
		inserirArestaDirecionada(rotuloVOrigem, rotuloVDestino, peso);
		inserirArestaDirecionada(rotuloVDestino, rotuloVOrigem, peso);
	}

	/*DONE*
    * Inserir uma aresta entre rotuloVOrigem eh rotuloVDestino com o peso
    * especificado.
    **/
	void inserirArestaDirecionada(string rotuloVOrigem, string rotuloVDestino, int peso) {
		int orig = obterIndiceVertice(rotuloVOrigem);
		int dest = obterIndiceVertice(rotuloVDestino);
		if (orig == -1 || dest == -1) {
			cerr << "Destino ou origem inexistente\n";
			return;
		} else {
			pair<int, int> aresta(dest, peso);
			arestas[orig].push_back(aresta);
			return;
		}
	}

	/*DONE*
    * Verifica se vertice rotuloVOrigem eh vertice rotuloVDestino seo
    * conectados (vizinhos).
    **/
	bool saoConectados(string rotuloVOrigem, string rotuloVDestino) {
		int orig = obterIndiceVertice(rotuloVOrigem);
		int dest = obterIndiceVertice(rotuloVDestino);
		if (orig == -1 || dest == -1) {
			cerr << "Destino ou origem inexistente\n";
			return false;
		} else {
			auto it = find_if(arestas[orig].begin(), arestas[orig].end(),
							  [dest](pair<int, int> v) { return (v.first == dest); });
			if (it != arestas[orig].end()) {
				cout << "Vértices " << rotuloVOrigem << " eh " << rotuloVDestino << " sao conectados com peso " << it->second << endl;
				return true;
			} else {
				cerr << "Vértices " << rotuloVOrigem << " eh " << rotuloVDestino << " não-conectados\n";
				return false;
			}
		}
	}

	/*DOING*
    * Verifica se ha algum caminho entre vertice rotuloVOrigem e 
    * vertice rotuloVDestino.
    * A melhor forma de fazer isto eh reusando a funcao dfs.
    **/
	bool haCaminho(string rotuloVOrigem, string rotuloVDestino) {
		return true;
	}

	/*DOING*
    * Muda os rotulos do vertices dos diferentes componentes para
    * nomes que representam cores.
    * Dica: procura componentes partindo do vertice v0 ou v1, em ordem
    * crescente (mas voce pode usar outra ordem se desejar). 
    * Retorna a quantidade de componentes.
    * A melhor forma de fazer isto eh reusando a funcao dfs.
    **/
	int colorir() {
		return -1;
	}

	/*DOING*
    * Usa a abordagem de navegacao BFS para listar as distancias
    * entre o vertice rotuloVOrigem e cada um dos demais vertices.
    * A funcao BFS consegue listar a menor distancia entre um vertice
    * e os demais se o grafo for NAO-PONDERADO.
    * Retorna um vetor de inteiros com a quantidade de arestas 
    * (distancia), o vertice rotuloVOrigem e cada um dos demais vertices.
    * Nao eh uma funcao recursiva. 
    * eh necesserio utilizar a ED fila.
    **/
	int* bfs(string rotuloVOrigem) {
		//IMPLEMENTAR
	}

	vector<string> getVertices() {
		return vertices;
	}

	vector<vector<pair<int, int>>> getArestas() {
		return arestas;
	}

	void print() {
		cout << "Graph:\n"
			 << "-> Vertices:\n\t";

		for (string vertice : vertices) {
			cout << " " << vertice << " ";
		}

		cout << "\n-> Edges:\n";

		for (int edge_idx = 0; edge_idx < arestas.size(); edge_idx++) {
			string vertice = vertices[edge_idx];
			if (arestas[edge_idx].size() > 0) {
				cout << "\t" << vertice << ": ";
				for (auto edge : arestas[edge_idx]) {
					cout << "(" << vertices[edge.first] << ", " << edge.second << ") ";
				}
				cout << endl;
			}
		}
	}
};