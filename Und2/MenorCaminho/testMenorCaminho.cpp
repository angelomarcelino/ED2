#include <gtest/gtest.h>
#include"grafomenorcaminhoexercicio.h"

using namespace std;

class MenorCaminhoTest : public ::testing::Test {
   protected:
	virtual void TearDown() {
		delete (grafo);
	}

	virtual void SetUp() {
		grafo = new GrafoListaAdj();
	}

	GrafoListaAdj* grafo;
};

/* Funcao auxiliar para construir o seguinte grafo ponderado:
 * https://github.com/eduardolfalcao/edii/blob/master/conteudos/imgs/grafo-ponderado-representacao-matriz-preenchido.png
 * Note que aqui o grafo � representado com lista de adjacencias.
 */
void construirGrafoPonderado(GrafoListaAdj* grafo) {
	grafo->inserirArestaNaoDirecionada("v1", "v2", 6);
	grafo->inserirArestaNaoDirecionada("v1", "v3", 4);
	grafo->inserirArestaNaoDirecionada("v2", "v4", 5);
	grafo->inserirArestaNaoDirecionada("v3", "v4", 2);
	grafo->inserirArestaNaoDirecionada("v3", "v5", 4);
	grafo->inserirArestaNaoDirecionada("v4", "v6", 5);
	grafo->inserirArestaNaoDirecionada("v4", "v7", 5);
	grafo->inserirArestaNaoDirecionada("v5", "v9", 9);
	grafo->inserirArestaNaoDirecionada("v6", "v8", 6);
	grafo->inserirArestaNaoDirecionada("v8", "v9", 8);
}

/* Funcao auxiliar para inserir uma sequencia de vertices comecando
 * em ini e terminando em fim. Ex: v1, v2, v3, ..., v9.
 */
void inserirVertices(GrafoListaAdj* grafo, int ini, int fim) {
	for (int i = ini; i <= fim; i++) {
		string rotulo;
		stringstream sstm;
		sstm << "v" << i;
		rotulo = sstm.str();
		grafo->inserirVertice(rotulo);
	}
}

TEST_F(MenorCaminhoTest, BellmanFordGrafoSemCicloNegativo) {
	inserirVertices(grafo, 1, 9);
	construirGrafoPonderado(grafo);

	int* distancias = grafo->bellmanFord("v1");
	EXPECT_EQ(distancias[0], 0);
	EXPECT_EQ(distancias[1], 6);
	EXPECT_EQ(distancias[2], 4);
	EXPECT_EQ(distancias[3], 6);
	EXPECT_EQ(distancias[4], 8);
	EXPECT_EQ(distancias[5], 11);
	EXPECT_EQ(distancias[6], 11);
	EXPECT_EQ(distancias[7], 17);
	EXPECT_EQ(distancias[8], 17);
	free(distancias);

	distancias = grafo->bellmanFord("v7");
	EXPECT_EQ(distancias[0], 11);
	EXPECT_EQ(distancias[1], 10);
	EXPECT_EQ(distancias[2], 7);
	EXPECT_EQ(distancias[3], 5);
	EXPECT_EQ(distancias[4], 11);
	EXPECT_EQ(distancias[5], 10);
	EXPECT_EQ(distancias[6], 0);
	EXPECT_EQ(distancias[7], 16);
	EXPECT_EQ(distancias[8], 20);
	free(distancias);

	distancias = grafo->bellmanFord("v9");
	EXPECT_EQ(distancias[0], 17);
	EXPECT_EQ(distancias[1], 20);
	EXPECT_EQ(distancias[2], 13);
	EXPECT_EQ(distancias[3], 15);
	EXPECT_EQ(distancias[4], 9);
	EXPECT_EQ(distancias[5], 14);
	EXPECT_EQ(distancias[6], 20);
	EXPECT_EQ(distancias[7], 8);
	EXPECT_EQ(distancias[8], 0);
	free(distancias);
}

TEST_F(MenorCaminhoTest, BellmanFordGrafoComCicloNegativo) {
	inserirVertices(grafo, 1, 9);
	construirGrafoPonderado(grafo);
	grafo->inserirArestaNaoDirecionada("v1", "v1", -2);

	//grafo com la�o com peso negativo em v1
	//como o grafo so tem um componente e eh nao direcionado
	//ent�o todas as dist�ncias podem ser diminu�das

	int* distancias = grafo->bellmanFord("v1");
	for (int i = 0; i < 9; i++)
		EXPECT_EQ(distancias[i], NEG_INF);
	free(distancias);

	//nao interessa o v�rtice de origem,
	//todas as dist�ncias sempre podem ser diminu�das
	distancias = grafo->bellmanFord("v5");
	for (int i = 0; i < 9; i++)
		EXPECT_EQ(distancias[i], NEG_INF);
	free(distancias);

	//n�o interessa o v�rtice de origem,
	//todas as dist�ncias sempre podem ser diminu�das
	distancias = grafo->bellmanFord("v9");
	for (int i = 0; i < 9; i++)
		EXPECT_EQ(distancias[i], NEG_INF);
	free(distancias);
}

TEST_F(MenorCaminhoTest, BellmanFordGrafoCom2Componentes) {
	//componente 1
	inserirVertices(grafo, 1, 9);
	construirGrafoPonderado(grafo);

	//componente 2
	inserirVertices(grafo, 10, 12);
	grafo->inserirArestaNaoDirecionada("v10", "v11", 10);
	grafo->inserirArestaNaoDirecionada("v10", "v12", 15);

	//como o grafo tem dois componentes
	//ent�o N�O S�O todas as dist�ncias QUE podem ser diminu�das
	//as dist�ncias QUE podem ser diminu�das envolvem
	//todos os v�rtices que s�o alcanc�veis a partir do v�rtice origem

	int* distancias = grafo->bellmanFord("v10");
	//vertices inalcancaveis pois estao em componente
	//diferente do componente do vertice de origem: v10
	//{v1,v2,v3,v4,v5,v6,v7,v8,v9}
	for (int i = 0; i < 9; i++)
		EXPECT_EQ(distancias[i], POS_INF);
	//vertices alcancaveis pois estao no mesmo componente
	//do vertice de origem: v10
	//{v10,v11,v12}
	EXPECT_EQ(distancias[9], 0);
	EXPECT_EQ(distancias[10], 10);
	EXPECT_EQ(distancias[11], 15);
	free(distancias);

	distancias = grafo->bellmanFord("v11");
	//vertices inalcancaveis pois estao em componente
	//diferente do componente do vertice de origem: v11
	//{v1,v2,v3,v4,v5,v6,v7,v8,v9}
	for (int i = 0; i < 9; i++)
		EXPECT_EQ(distancias[i], POS_INF);
	//vertices alcancaveis pois estao no mesmo componente
	//do vertice de origem: v10
	//{v10,v11,v12}
	EXPECT_EQ(distancias[9], 10);
	EXPECT_EQ(distancias[10], 0);
	EXPECT_EQ(distancias[11], 25);
	free(distancias);

	distancias = grafo->bellmanFord("v12");
	//vertices inalcancaveis pois estao em componente
	//diferente do componente do vertice de origem: v12
	//{v1,v2,v3,v4,v5,v6,v7,v8,v9}
	for (int i = 0; i < 9; i++)
		EXPECT_EQ(distancias[i], POS_INF);
	//vertices alcancaveis pois estao no mesmo componente
	//do vertice de origem: v10
	//{v10,v11,v12}
	EXPECT_EQ(distancias[9], 15);
	EXPECT_EQ(distancias[10], 25);
	EXPECT_EQ(distancias[11], 0);
	free(distancias);
}

TEST_F(MenorCaminhoTest, BellmanFordGrafoCom2ComponentesECicloNegativo) {
	//componente 1
	inserirVertices(grafo, 1, 9);
	construirGrafoPonderado(grafo);
	//ciclo negativo (laco)
	grafo->inserirArestaNaoDirecionada("v1", "v1", -2);

	//componente 2
	inserirVertices(grafo, 10, 12);
	grafo->inserirArestaNaoDirecionada("v10", "v11", 10);
	grafo->inserirArestaNaoDirecionada("v10", "v12", 15);

	int* distancias = grafo->bellmanFord("v10");
	//componente com ciclos negativos
	//{v1,v2,v3,v4,v5,v6,v7,v8,v9}
	for (int i = 0; i < 9; i++)
		EXPECT_EQ(distancias[i], NEG_INF);

	/* EXPLICA��O PARA RESULTADO ACIMA */
	//Infelizmente, o BellmanFord nao retorna POS_INF
	//pois se o algoritmo encontrar um ciclo negativo
	//no grafo, ent�o todos os n�s do ciclo negativo
	//recebem NEG_INF (mesmo que o mais intuitivo para nos
	//eh que eles recebessem POS_INF).
	//Isso acontece mesmo que n�o exista caminho entre
	//v10 e {v1,v2,v3,v4,v5,v6,v7,v8,v9}.
	//Uma forma de obter POS_INF seria usar o algoritmo
	//de coloracao pra detectar os componentes, e depois
	//aplicar bellman-ford somente no componente que
	//possui o vertice de origem (mas nao fiz isso nesse
	//exercicio).
	//===============================================
	//componente sem ciclos negativos
	//{v10,v11,v12}

	EXPECT_EQ(distancias[9], 0);
	EXPECT_EQ(distancias[10], 10);
	EXPECT_EQ(distancias[11], 15);
	free(distancias);

	distancias = grafo->bellmanFord("v11");
	//componente com ciclos negativos
	//{v1,v2,v3,v4,v5,v6,v7,v8,v9}
	for (int i = 0; i < 9; i++)
		EXPECT_EQ(distancias[i], NEG_INF);
	//componente sem ciclos negativos
	//{v10,v11,v12}
	EXPECT_EQ(distancias[9], 10);
	EXPECT_EQ(distancias[10], 0);
	EXPECT_EQ(distancias[11], 25);
	free(distancias);

	distancias = grafo->bellmanFord("v12");
	//componente com ciclos negativos
	//{v1,v2,v3,v4,v5,v6,v7,v8,v9}
	for (int i = 0; i < 9; i++)
		EXPECT_EQ(distancias[i], NEG_INF);
	//componente sem ciclos negativos
	//{v10,v11,v12}
	EXPECT_EQ(distancias[9], 15);
	EXPECT_EQ(distancias[10], 25);
	EXPECT_EQ(distancias[11], 0);
	free(distancias);

	distancias = grafo->bellmanFord("v1");
	//componente com ciclos negativos
	//{v1,v2,v3,v4,v5,v6,v7,v8,v9}
	for (int i = 0; i < 9; i++)
		EXPECT_EQ(distancias[i], NEG_INF);
	//componente sem ciclos negativos
	//{v10,v11,v12}
	for (int i = 9; i < 12; i++)
		EXPECT_EQ(distancias[i], POS_INF);
	free(distancias);

	distancias = grafo->bellmanFord("v5");
	//componente com ciclos negativos
	//{v1,v2,v3,v4,v5,v6,v7,v8,v9}
	for (int i = 0; i < 9; i++)
		EXPECT_EQ(distancias[i], NEG_INF);
	//componente sem ciclos negativos
	//{v10,v11,v12}
	for (int i = 9; i < 12; i++)
		EXPECT_EQ(distancias[i], POS_INF);
	free(distancias);
}

TEST_F(MenorCaminhoTest, DijkstraGrafoSemCicloNegativo) {
	inserirVertices(grafo, 1, 9);
	construirGrafoPonderado(grafo);

	int* distancias = grafo->dijkstra("v1");
	EXPECT_EQ(distancias[0], 0);
	EXPECT_EQ(distancias[1], 6);
	EXPECT_EQ(distancias[2], 4);
	EXPECT_EQ(distancias[3], 6);
	EXPECT_EQ(distancias[4], 8);
	EXPECT_EQ(distancias[5], 11);
	EXPECT_EQ(distancias[6], 11);
	EXPECT_EQ(distancias[7], 17);
	EXPECT_EQ(distancias[8], 17);
	free(distancias);

	distancias = grafo->dijkstra("v7");
	EXPECT_EQ(distancias[0], 11);
	EXPECT_EQ(distancias[1], 10);
	EXPECT_EQ(distancias[2], 7);
	EXPECT_EQ(distancias[3], 5);
	EXPECT_EQ(distancias[4], 11);
	EXPECT_EQ(distancias[5], 10);
	EXPECT_EQ(distancias[6], 0);
	EXPECT_EQ(distancias[7], 16);
	EXPECT_EQ(distancias[8], 20);
	free(distancias);

	distancias = grafo->dijkstra("v9");
	EXPECT_EQ(distancias[0], 17);
	EXPECT_EQ(distancias[1], 20);
	EXPECT_EQ(distancias[2], 13);
	EXPECT_EQ(distancias[3], 15);
	EXPECT_EQ(distancias[4], 9);
	EXPECT_EQ(distancias[5], 14);
	EXPECT_EQ(distancias[6], 20);
	EXPECT_EQ(distancias[7], 8);
	EXPECT_EQ(distancias[8], 0);
	free(distancias);
}

int main(int argc, char** argv) {
	testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}