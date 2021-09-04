#include <iostream>
#include <queue>
#include <vector>

#include "stdbool.h"

using namespace std;

class GrafoListaAdj {
   private:
	vector<string> vertices;

	//first eh o indice do vertice, second eh o peso (caso o grafo seja ponderado)
	vector<vector<pair<int, int>>> arestas;

	//Grupo do union-find
	class Grupo {
	   public:
		int pai;
		int tamanho;
	};

	vector<Grupo> grupos;
	int num_components = 0 ;

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

   public:
	/*DONE*
    * Lembrem-se: 
    *       1) nao podemos inserir vertices com mesmo rotulo
    *       2) toda vez que inserirmos um novo vehrtice, precisaremos
    *          inserir um vetor para representar as conexoes daquele 
    *          vehrtice na lista de adjacencias
    **/
	void inserirVertice(string rotuloVertice) {
		Grupo grupo;
		if (obterIndiceVertice(rotuloVertice) == -1) {
			vertices.push_back(rotuloVertice);
			vector<pair<int, int>> vec;
			arestas.push_back(vec);

			grupo.pai = vertices.size() - 1;
			grupo.tamanho = 1;
			grupos.push_back(grupo);

			num_components++;
		} else {
			cerr << "Aresta " << rotuloVertice << " já existente\n";
		}
		return;
	}

	/**
     * Sempre que o grafo for nao ponderado, adicionaremos o peso 1,
     * por conveniehncia.
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
    * Inserir uma aresta entre rotuloVOrigem e rotuloVDestino com o peso
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

	//encontrar raiz
	//pode ser com ou sem path compression
	int encontrarRaiz(int i) {
		if (i > grupos.size() -1) return -1;
		if (i < 0) return -1;
		int root = i;
		while (root != grupos[root].pai)
			root = grupos[root].pai;

		while (i != root) {
			int next = grupos[i].pai;
			grupos[i].pai = root;
			i = next;
		}

		return root;
	}

	bool mesmoGrupo(int a, int b) {
		return encontrarRaiz(a) == encontrarRaiz(b);
	}

	int tamanhoGrupo(int a) {
		if (a > grupos.size() -1|| a < 0) return -1;
		return grupos[encontrarRaiz(a)].tamanho;
	}

	void unirGrupos(int a, int b) {
		if (a > grupos.size() - 1 || b > grupos.size() - 1) return;
		if (a < 0 || b < 0) return;
		if (mesmoGrupo(a, b)) return;

		int root1 = encontrarRaiz(a);
		int root2 = encontrarRaiz(b);

		if (grupos[root1].tamanho < grupos[root2].tamanho){
			grupos[root2].tamanho += grupos[root1].tamanho;
			grupos[root1].pai = root2;
		} else {
			grupos[root1].tamanho += grupos[root2].tamanho;
			grupos[root2].pai = root1;
		}
		num_components--;
	}

	//criei essa classe pra facilitar a ordenacao
	//das arestas com menor peso (que faz parte do
	//algoritmo de Kruskal)
	class Aresta {
	   public:
		int origem;
		int destino;
		int peso;
		Aresta(int origem, int destino, int peso)
			: origem(origem), destino(destino), peso(peso) {}
	};

	//sobrescrever operator< para que a priority_queue
	//ordene como desejamos
	friend bool operator<(const Aresta& a1, const Aresta& a2) {
		return a1.peso > a2.peso;
	}

	GrafoListaAdj* KruskalMST() {
		//IMPLEMENTAR

		//instanciamos um novo grafo, que armazenara
		//apenas as arestas da MST
		GrafoListaAdj* mst = new GrafoListaAdj();

		//adicionar vertices
        for (int i = 0; i < vertices.size(); i++)
			mst->inserirVertice(vertices.at(i));

		//(NAO PRECISO) inicializar estrutura do union-find (array de grupos)

		//inicializar priority_queue
		priority_queue<Aresta> arestasMenorPeso;
		//adicionar arestas na fila
        for (int i = 0; i < arestas.size(); i++)
        {
            for (int j = 0; j < arestas[i].size(); j++)
            {
				Aresta a(i, arestas[i][j].first, arestas[i][j].second);
				arestasMenorPeso.push(a);
			}
		}

		//iterar na fila de arestas
		//se os vertices da arestas nao pertencem ao mesmo grupo
		//una-os, e adicione a aresta no MST
		while (!arestasMenorPeso.empty())
        {
			Aresta a = arestasMenorPeso.top();
			arestasMenorPeso.pop();
			//cout << "{" << a.origem << "," << a.destino << "} " << a.peso << endl;
			if (!mesmoGrupo(a.origem, a.destino)) {
				unirGrupos(a.origem, a.destino);

				string orig = vertices[a.origem];
				string dest = vertices[a.destino];
				mst->inserirArestaNaoDirecionada(orig, dest, a.peso);
			}
			
		}

		return mst;
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