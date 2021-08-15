#include <bits/stdc++.h>

#include <algorithm>
#include <iostream>
#include <queue>
#include <vector>

using namespace std;

#define POS_INF 1000000000
#define NEG_INF -1000000000

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

	/*DONE*
	* O argumento indicesVerticesVisitados serve para controlar quais
	* vertices ja foram visitados.
	* Lembrando que DFS eh uma funcao recursiva.
	**/
	void dfs(string rotuloVOrigem, bool* indicesVerticesVisitados) {
		int orig = obterIndiceVertice(rotuloVOrigem);

		if (indicesVerticesVisitados[orig])
			return;
		else {
			indicesVerticesVisitados[orig] = true;

			for (auto aresta : arestas[orig]) {
				dfs(vertices[aresta.first], indicesVerticesVisitados);
			}
		}
	}

   public:
	/*DONE*
	* Lembrem-se:
	*	  1) nao podemos inserir vertices com mesmo rotulo
	*	  2) toda vez que inserirmos um novo vertice, precisaremos
	*	 inserir um vetor para representar as conexoes daquele
	*	 vertice na lista de adjacencias
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
				cout << "Vertices " << rotuloVOrigem << " e " << rotuloVDestino << " sao conectados com peso " << it->second << endl;
				return true;
			} else {
				cerr << "Vertices " << rotuloVOrigem << " e " << rotuloVDestino << " nao sao conectados\n";
				return false;
			}
		}
	}

	/*DONE*
	* Verifica se ha algum caminho entre vertice rotuloVOrigem e 
	* vertice rotuloVDestino.
	* A melhor forma de fazer isto eh reusando a funcao dfs.
	**/
	bool haCaminho(string rotuloVOrigem, string rotuloVDestino) {
		int orig = obterIndiceVertice(rotuloVOrigem);
		int dest = obterIndiceVertice(rotuloVDestino);

		if (orig == -1 || dest == -1) {
			cerr << "Destino ou origem inexistente\n";
			return false;
		} else if (orig == dest && !saoConectados(rotuloVOrigem, rotuloVDestino)) {
			cerr << "Vertice " << rotuloVOrigem << " nao leva a ele mesmo" << endl;
			return false;
		} else {
			bool VerticesVisitados[vertices.size()] = {false};
			dfs(rotuloVOrigem, VerticesVisitados);
			string answer = VerticesVisitados[dest] ? " " : " nao ";
			cout << "Vertice " << rotuloVOrigem << answer + "leva a " << rotuloVDestino << endl;
			return VerticesVisitados[dest];
		}
	}

	/*DONE*
	* Muda os rotulos do vertices dos diferentes componentes para
	* nomes que representam cores.
	* Dica: procura componentes partindo do vertice v0 ou v1, em ordem
	* crescente (mas voce pode usar outra ordem se desejar). 
	* Retorna a quantidade de componentes.
	* A melhor forma de fazer isto eh reusando a funcao dfs.
	**/
	int colorir() {
		int cor = 1;
		int size = vertices.size();
		if (size == 0) {
			cerr << "Lista vazia!\n";
			return -1;
		}

		bool verticesColoridos[size] = {false};
		for (int i = 0; i < size; i++) {
			bool verticesVisitados[size] = {false};
			// Se nao colori vertice na posicao i, visito ele e coloro
			// seus pares
			if (!verticesColoridos[i]) {
				dfs(vertices[i], verticesVisitados);
				for (int j = 0; j < size; j++) {
					if (verticesVisitados[j]) {
						verticesColoridos[j] = true;
						vertices[j] = "corComp" + to_string(cor);
					}
				}

				cor++;
			}
		}
		cout << "Quantidade de cores = " << cor - 1 << endl;
		return cor - 1;
	}

	/*DONE*
	* Usa a abordagem de navegacao BFS para listar as distancias
	* entre o vertice rotuloVOrigem e cada um dos demais vertices.
	* A funcao BFS consegue listar a menor distancia entre um vertice
	* e os demais se o grafo for NAO-PONDERADO.
	* Retorna um vetor de inteiros com a quantidade de arestas 
	* (distancia), o vertice rotuloVOrigem e cada um dos demais vertices.
	* Nao eh uma funcao recursiva. 
	* Eh necessario utilizar a ED fila.
	**/
	int* bfs(string rotuloVOrigem) {
		queue<int> q;

		int orig = obterIndiceVertice(rotuloVOrigem);
		int size = vertices.size();

		int* distancia = new int[size]{0};
		bool visitados[size] = {false};

		visitados[orig] = true;
		distancia[orig] = 0;
		q.push(orig);

		while (!q.empty()) {
			int index = q.front();
			q.pop();

			for (auto a : arestas[index]) {
				if (!visitados[a.first]) {
					visitados[a.first] = true;
					distancia[a.first] = distancia[index] + 1;

					q.push(a.first);
				}
			}
		}
		cout << "Distancias a partir do vertice " << rotuloVOrigem << ":\n\t";
		for (int i = 0; i < size; i++) {
			string c = i > 9 ? " " : "  ";
			if (distancia[i] > 0) cout << vertices[i] << c + "---> " << distancia[i] << "\n\t";
		}
		cout << endl;
		return distancia;
	}

	/*DONE*
    * Usamos o BellmanFord para encontrar menor caminho
    * em grafos com arestas com pesos negativos.
    * O BellmanFord consegue detectar ciclos negativos
    * eh atribuir o valor NEG_INF (constante definida nesse arquivo)
    * aos vertices que perteneam ao ciclo.
    * POS_INF deve ser atribuedo aos vertices inalcaneeveis.
    * O aspecto negativo eh sua complexidade de tempo: O(V*E).
    * Isto acontece pois, como possui arestas negativas, cada vertice 
    * do grafo precisa ser processado V vezes.   
    * Pseudo-cedigo: https://github.com/eduardolfalcao/edii/blob/master/conteudos/Grafos.md#bellman-ford
    **/
	int* bellmanFord(string rotuloVOrigem) {
		int orig = obterIndiceVertice(rotuloVOrigem);
		if (orig == -1) {
			cerr << "Rotulo " << rotuloVOrigem << " desconhecido\n";
			return new int[1]{POS_INF};
		}

		int size = vertices.size();
		int* distancias = new int[size];
		fill(distancias, distancias + size, POS_INF);

		distancias[orig] = 0;

		for (int i = 0; i < size - 1; i++) {
			for (int j = 0; j < size; j++) {
				for (auto a : arestas[j]) {
					if (distancias[j] + a.second < distancias[a.first])
						distancias[a.first] = distancias[j] + a.second;
				}
				//cout << distancias[j] << "\t";
			}
			//cout << endl;
		}

		for (int i = 0; i < size - 1; i++) {
			for (int j = 0; j < size; j++) {
				for (auto a : arestas[j]) {
					if (distancias[j] + a.second < distancias[a.first])
						distancias[a.first] = NEG_INF;
				}
			}
		}
		//print();

		cout << "Distancias de " << rotuloVOrigem << ":\n\t";
		for (int v = 0; v < size; v++) {
			cout << vertices[v] << " : " << distancias[v] << "\n\t";
		}
		cout << endl;

		return distancias;
	}

	/*DONE*
    * Usamos o Dijkstra para encontrar menor caminho
    * em grafos sem arestas com pesos negativos.
    * O Dijkstra retorna respostas incorretas caso o grafo
    * possua arestas negativas, e portanto nao consegue
    * detectar ciclos negativos. Este eh o aspecto negativo.
    * POS_INF deve ser atribuido aos vertices inalcancaveis.
    * O aspecto positivo eh sua complexidade de tempo: O(V+E).
    * Isto acontece pois, como o grafo nao possui arestas negativas, 
    * cada vertice do grafo precisa ser processado apenas 1 vez.
    * Pseudo-codigo: https://github.com/eduardolfalcao/edii/blob/master/conteudos/Grafos.md#dijkstra
    * Ilustracao: https://docs.google.com/drawings/d/1NmkJPHpcg8uVcDZ24FQiYs3uHR5n-rdm1AZwD74WiMY/edit?usp=sharing
    **/
	int* dijkstra(string rotuloVOrigem) {
		int orig = obterIndiceVertice(rotuloVOrigem);
		if (orig == -1) {
			cerr << "Rotulo " << rotuloVOrigem << " desconhecido\n";
			return new int[1]{POS_INF};
		}

		priority_queue<pair<int, int>> q;
		int size = vertices.size();

		bool visitados[size] = {false};
		int* distancias = new int[size];
		fill(distancias, distancias + size, POS_INF);

		distancias[orig] = 0;

		q.push({0, orig});
		while (!q.empty()) {
			int a = q.top().second;
			q.pop();
			if (visitados[a]) continue;
			visitados[a] = true;
			for (auto e : arestas[a]) {
				int b = e.first, w = e.second;
				if (distancias[a] + w < distancias[b]) {
					distancias[b] = distancias[a] + w;
					q.push({-distancias[b], b});
				}
			}
		}

		cout << "Distancias de " << rotuloVOrigem << ":\n\t";
		for (int v = 0; v < size; v++) {
			cout << vertices[v] << " : " << distancias[v] << "\n\t";
		}
		cout << endl;

		return distancias;
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
			//if (arestas[edge_idx].size() > 0) {
			cout << "\t" << vertice << ": ";
			for (auto edge : arestas[edge_idx]) {
				cout << "(" << vertices[edge.first] << ", " << edge.second << ") ";
			}
			cout << endl;
			//}
		}
	}
};