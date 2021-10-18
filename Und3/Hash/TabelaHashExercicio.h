#include <iostream>
#include <vector>
using namespace std;

template <typename K, typename V>
class Tupla {
   private:
	K chave;
	V valor;
	Tupla* prox;

   public:
	Tupla(K c, V v) {
		chave = c;
		valor = v;
		prox = NULL;
	}

	K getChave() {
		return chave;
	}

	V getValor() {
		return valor;
	}

	Tupla* getProx() {
		return prox;
	}

	void setProx(Tupla* prox) {
		Tupla::prox = prox;
	}
};

template <typename Chave, typename Valor>
class TabelaHash {
   private:
	Tupla<Chave, Valor>** tabela;

	//tamanho atual do array
	int qtde_buckets;
	//qtdade de elementos já inseridos na tabela hash
	int tamanho;

	unsigned int hashID(Chave key) {
		size_t hash_id = hash<Chave>{}(key);

		//cout << hash_id << "\t" << hash_id % qtde_buckets << endl;

		return hash_id % qtde_buckets;
	}

	/*DONE*
		* Função para inserir a tupla <c,v> na tabela.
		* É preciso calcular o código hash a partir da chave c.
		* Em seguida, usamos qtde_buckets para descobrir o índice
		* dessa tupla. Prestar atenção na alocação de tupla quando
		* há colisões. A função hash às vezes retorna valores
		* negativos: use abs() para evitar índices negativos.
	**/
	void inserir(Chave c, Valor v, Tupla<Chave, Valor>** tabela) {
		unsigned int id = hashID(c);

		Tupla<Chave, Valor>* tupla = new Tupla<Chave, Valor>(c, v);
		tupla->setProx(NULL);

		if (tabela[id] == NULL) {
			tabela[id] = tupla;
		} else {
			Tupla<Chave, Valor>* top = tabela[id];

			while (top->getProx() != NULL) {
				if (top->getChave() == c) {
					cout << "Chave existente." << endl;
					return;
				}
				top = top->getProx();
			}
			top->setProx(tupla);
		}
	}

	/*DONE*
		* Função para aumentar o tamanho do array quando o
		* fator de carga for >= 1. O tamanho do array
		* (qtde_buckets) deve ser multiplicado por 8 para
		* que essa operação seja feita com pouca frequência.
		* Por fim, precisamos reposicionar as tuplas, considerando
		* que a posição nesse novo array com maior tamanho
		* será diferente.
		**/
	void aumentaArray() {
		if (load_factor() < 1) return;

		int old_buckets = qtde_buckets;
		qtde_buckets *= 8;
		Tupla<Chave, Valor>** nv_tabela = new Tupla<Chave, Valor>*[qtde_buckets]();

		for (int i = 0; i < old_buckets; i++) {
			if (tabela[i] != NULL) {
				Tupla<Chave, Valor>* top = tabela[i];
				while (top != NULL) {
					inserir(top->getChave(), top->getValor(), nv_tabela);
					top = top->getProx();
				}
			}
		}

		delete[] tabela;
		tabela = nv_tabela;
	}

   public:
	/*DONE*
		* Inicializar o array de tuplas com capacidade = qtde_buckets.
		* Lembrar de setar todas as posições do array inicializado
		* para NULL.
		**/
	TabelaHash() {
		qtde_buckets = 8;
		tamanho = 0;

		tabela = new Tupla<Chave, Valor>*[qtde_buckets]();
		for (int i = 0; i < qtde_buckets; i++)
			tabela[i] = NULL;
	}

	/*DONE*
		* Essa é a função pública que nos permite inserir
		* uma tupla <c,v> na tabela. Nessa função nós
		* verificamos se é preciso aumentar o array de acordo
		* com o load factor. Essa função pode reusar sua versão
		* sobrecarregada para inserir a tupla na tabela.
		* Essa função também incrementa a quantidade de elementos
		* na tabela (variável tamanho).
		**/
	void inserir(Chave c, Valor v) {
		tamanho++;
		if (load_factor() >= 1.0)
			aumentaArray();

		inserir(c, v, tabela);
	}

	/*DONE*
		* Essa função retorna o fator de carga da Tabela Hash.
		**/
	double load_factor() {
        return (double)tamanho / (double)qtde_buckets;
	}

	/*DONE*
		* Há apenas um valor associado a uma chave.
		* Essa função retorna esse valor, caso a chave exista.
		* Se a chave não existir a função retorna NULL.
		* Lembre-se: em caso de colisão, é preciso navegar
		* no bucket (lista ligada) para ter certeza se a chave
		* existe ou não.
		**/
	Valor getValor(Chave chave) {
		int id = hashID(chave);

		if (tabela[id] != NULL) {
			Tupla<Chave, Valor>* top = tabela[id];
			while (top->getChave() != chave) {
				if (top->getProx() == NULL) {
					cout << "Chave não está na tabela" << endl;
					return 0;
				}
				top = top->getProx();
			}
			return top->getValor();
		}

		cout << "Chave "<< chave << " não está na tabela" << endl;
		return 0;
	}

	/*DONE*
		* Essa função retorna true caso a chave exista,
		* false caso contrário.
		* Lembre-se: em caso de colisão, é preciso navegar
		* no bucket (lista ligada) para ter certeza se a chave
		* existe ou não.
		**/
	bool contemChave(Chave chave) {
		int id = hashID(chave);

		if (tabela[id] != NULL) {
			Tupla<Chave, Valor>* top = tabela[id];
			while (top->getChave() != chave) {
				if (top->getProx() == NULL) {
					cout << "Chave não está na tabela" << endl;
					return false;
				}
				top = top->getProx();
			}
			return true;
		}

		cout << "Chave " << chave << " não está na tabela" << endl;
		return false;
	}

	/*DONE*
		* Essa função retorna um vetor com todas as chaves
		* já inseridas na tabela.
		**/
	vector<Chave> getChaves() {
		vector<Chave> chaves;

		for (int i = 0; i < qtde_buckets; i++) {
			if (tabela[i] != NULL) {
				Tupla<Chave, Valor>* top = tabela[i];
				while (top != NULL) {
					chaves.push_back(top->getChave());
					top = top->getProx();
				}
			}
		}

		return chaves;
	}

	/*DONE*
		* Essa função desaloca os nós previamente alocados, e muda
		* o tamanho do array de tuplas para 8.
		**/
	void clear() {
		for (int i = 0; i < qtde_buckets; i++){
			Tupla<Chave, Valor>* top = tabela[i];
			while (top != NULL){
				Tupla<Chave, Valor>* temp = top;
				top = top->getProx();
				delete temp;
			}
			tabela[i] = NULL;
		}

		delete[] tabela;

		qtde_buckets = 8;
		tamanho = 0;

		tabela = new Tupla<Chave, Valor>*[qtde_buckets]();
		for (int i = 0; i < qtde_buckets; i++)
			tabela[i] = NULL;
	}

	/*DONE*
		* Há apenas um nó associado com uma mesma chave.
		* Essa função remove esse nó da tabela, caso a chave exista.
		* Se a chave não existir a função não faz nada.
		* Lembre-se: em caso de colisão, é preciso navegar
		* no bucket (lista ligada) para ter certeza se a chave
		* existe ou não.
		* Dica: olhar algoritmo de remoção em lista ligada, pois
		* após a remoção de um nó a lista precisa permanecer íntegra,
		* ou seja, navegável.
		**/
	void remover(Chave chave) {
		unsigned int id = hashID(chave);

		if (tabela[id] != NULL) {
			Tupla<Chave, Valor>* top = tabela[id];
			Tupla<Chave, Valor>* before = NULL;

			while (top->getChave() != chave) {
				before = top;
				top = top->getProx();
				if (top == NULL) {
					cout << "Chave não existente." << endl;
					return;
				}
			}

			if (before == NULL) {
				tabela[id] = top->getProx();
			} else {
				before->setProx(top->getProx());
			}
			tamanho--;
			delete top;
		}
	}

	/**
		* Essa função retorna a quantidade de pares
		* que já foram inseridos na Tabela Hash.
		**/
	int size() {
		return tamanho;
	}

	/**
		* Essa função retorna o tamanho atual do
		* array usado para armazenar a Tabela Hash.
		**/
	int bucket_count() {
		return qtde_buckets;
	}

	friend std::ostream& operator<<(std::ostream& os, TabelaHash<Chave, Valor> hash_table) {
		for (int i = 0; i < hash_table.qtde_buckets; i++) {
			if (hash_table.tabela[i] != NULL) {
				os << std::setw(3) << i << ": (" << hash_table.tabela[i]->getChave() << ", " << hash_table.tabela[i]->getValor() << ")";

				if (hash_table.tabela[i]->getProx() != NULL) {
					auto next = hash_table.tabela[i]->getProx();
					do {
						os << " -> (" << next->getChave() << ", " << next->getValor() << ")";

						next = next->getProx();

					} while (next != NULL);

					os << " -> NULL\n";

				} else {
					os << " -> NULL\n";
				}
			} else {
				os << std::setw(3) << i << ": NULL\n";
			}
		}

		return os;
	}
};