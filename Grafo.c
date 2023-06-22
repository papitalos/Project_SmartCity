/** 
 
 * @file   Grafo.c
 * @brief  Funções de manipulação e exploração do grafo
 * 
 * @author italo
 * @date   May 2023
 
**/

#include "Grafo.h"
#include "Caminho.h"
#include "Transporte.h"
#include "Localizacao.h"


#pragma region FUNC_DE_CRIACAO

//Função para criar o grafo
Grafo* CriarGrafo(int nVert) {
	int i;

	//aloca espaço para um grafo
	Grafo* graf = (Grafo*)malloc(sizeof(Grafo));
	if (graf == NULL) return graf;
	graf->vert = (Vertice*)malloc(sizeof(Vertice*));
	if (graf->vert == NULL) return graf;


	//define o numero de vertices e começa com 0 arestas
	graf->nVertices = nVert;
	graf->nArestas = 0;
	for (i = 0; i < nVert; i++) {
		//define que cada adjacencia (1,2,3...)  de cada cabeca como nula
		graf->vert[i].cabeca = NULL;
		graf->vert[i].foiVisitado = false;

	}

	return graf;
}

// Função para destruir o grafo
void DestruirGrafo(Grafo* graf) {
	if (graf == NULL) return;

	// Libera a memória alocada para cada lista de adjacências
	for (int i = 0; i < graf->nVertices; i++) {
		Adjacencia* atual = graf->vert[i].cabeca;
		while (atual != NULL) {
			Adjacencia* proximo = atual->prox;
			free(atual);
			atual = proximo;
		}
	}

	
	// Libera a memória alocada para o grafo
	free(graf);
}

//Função que cria uma adjacencia
Adjacencia* CriaAdjacencia(int v, float peso) {
	//aloca espaço temporario para um nó da adjacencia
	Adjacencia* temp = (Adjacencia*)malloc(sizeof(Adjacencia));

	if (temp == NULL) return temp;

	//define os parametros dessa adjacencia temporaria
	temp->vertice = v; //vertice de destino
	temp->peso = peso; //peso da aresta sendo criada
	temp->prox = NULL; //define o proximo como nulo pra tirar o sujo
	return(temp);
}

//Função para contar os vertices a partir da lista de localizações
int ContarVertices(Grafo* graf, ListaLocal* lista) {
	if (graf == NULL) {
		int contador = 0;
			ListaLocal* ultimo_nodo = lista;
			while (ultimo_nodo != NULL)
			{
				contador++;
				ultimo_nodo = ultimo_nodo->prox;
			}

			return contador;
	}
	return 1;
}

//Função para endereçar cada vertice com um local
int EnderecarLocal(Grafo* graf,ListaLocal* lista) {
	
	if (lista == NULL) return -1;

	for (int i = 0; i < graf->nVertices; i++)
	{
		char* geocode = EncontrarLocal(lista, i);
		graf->vert[i].cabeca->local.geocode = geocode;
	}
	return 1;
}

//Função para endereçar uma lista de transportes para um geocode especifico
int ListarTransportesPorVertice(Grafo* graf, ListaTransporte* lista) {

	if (lista == NULL) return -1;

	for (int i = 0; i < graf->nVertices; i++)
	{
		graf->vert[i].cabeca->local.TransportesGeocode = ListarPorGeocode(lista, graf->vert[i].cabeca->local.geocode);
	}
	return 1;
}

//Função para criar uma aresta
int CriaAresta(Grafo* graf, int verIni, int verFin, float peso) {
	//Verifica se o grafo existe
	if (!graf) { return 2;}
	//Verifica se o numero de vertices nao sao negativos e se sao maiores do que o numero de vertices do graf
	if (verFin < 0 || verFin >= graf->nVertices) { return 3;}
	if (verIni < 0 || verFin >= graf->nVertices) { return 3;}
		
	//usa a função de cima para criar a adjacencia com o vertice final(destino) e o peso
	Adjacencia* novaAdj = CriaAdjacencia(verFin, peso);

	//coloco a nova adjacencia linkada no vertice inicial da lista
	novaAdj->prox = graf->vert[verIni].cabeca; //o proximo elemento da lista passa receber a cabeça da lista
	graf->vert[verIni].cabeca = novaAdj; // a nova adjacencia passa a ser a cabeça da lista
	graf->nArestas++; //atualiza o numero de arestas
	return 1;
}

//Função que liga os vertices as adjacencias a partir de um mapa matriz
int CriarMapaMatriz(Grafo* graf) {
	//para cada linha e coluna dentro do numero max de vertices da matriz
	for (int i = 0; i < MAX_VERTICES; i++) {
		for (int j = 0; j < MAX_VERTICES; j++) {
			if (graf->matrizAdj[i][j] != 0) {
				//cria uma aresta no mapa, ligando i e j com o peso da matriz
				CriaAresta(graf, i, j, graf->matrizAdj[i][j]);
			}
		}
	}
	return 1;
}

//Função para debugar o grafo
void DebugGrafo(Grafo* graf) {
	printf("\n---------------- Grafo ----------------\n");
	printf("Numero de vertices: %d\n", graf->nVertices);
	printf("Numero de arestas: %d\n", graf->nArestas);
	printf("Matriz de Adjacencia:\n");
	for (int i = 0; i < graf->nVertices; i++) {
		for (int j = 0; j < graf->nVertices; j++) {
			if (graf->matrizAdj[i][j] > 99) {
				printf("%.0f", graf->matrizAdj[i][j]);
			}
			else {
				printf(" %.0f ", graf->matrizAdj[i][j]);
			}	
		}
		printf("\n");
	}
	printf("Lista de Adjacencias:\n");
	for (int i = 0; i < graf->nVertices; i++) {
		printf("Vertice %d -> ", i);
		Adjacencia* adj = graf->vert[i].cabeca;
		while (adj != NULL) {
			printf("(%d, peso: %.1f) -> ", adj->vertice, adj->peso);
			adj = adj->prox;
		}
		printf("NULL\n");
	}
	printf("---------------------------------------\n");
}

#pragma endregion

#pragma region FUNC_SUPORTE

	//Função que cria um caminhao
	int CriarCaminhao(Coletor* novoColetor) {
		//Aloca espaço para o caminhão
		novoColetor = (Coletor*)malloc(sizeof(Coletor*));

		//Verifica se alocou espaço corretamente
		if (novoColetor == NULL) return -1; 

		//Define o peso atual do caminhão como 0
		novoColetor->pesoAtual = 0;

		/*Espaço para caso venha adicionar coisas extras para o caminhão*/

		return 1;
	}

	//Função que cria um NodoPath para a lista da solução atual
	Trajeto CriarNodoPath(Adjacencia* adjacencia) {
		
		//Cria um novo nodoPath
		Trajeto newNodoPath = {0};

		//Define suas variaveis como as variaveis da adjacencia atual
		newNodoPath.peso = adjacencia->peso;
		newNodoPath.vertice = adjacencia->vertice;

		return newNodoPath;
	}

	//Função que adiciona o NodoPath na lista de uma solução
	int AdicionarNodoPath(Solucao** path, Trajeto trajeto) {

		//Aloca espaço para o novo nodo da solução
		Solucao* novoNodo = (Solucao*)malloc(sizeof(Solucao));

		//Verifica se a alocação funcionou
		if (novoNodo == NULL) return -1;

		//Define as variaveis do novo nodo 
		novoNodo->TrajetoParcial = trajeto;
		novoNodo->prox = NULL;

		//Verifica se é o primeiro nodo a adicionar a lista da solução
		if (*path == NULL) {
			*path = novoNodo;
		}

		//Percorre a lista
		else {
			Solucao* ultimo_nodo = *path;
			while (ultimo_nodo->prox != NULL) {
				ultimo_nodo = ultimo_nodo->prox;
			}
			//Define o ultimo nodo como o novo nodo
			ultimo_nodo->prox = novoNodo;
		}

		//Atualiza o tamanho e distancia do trajeto existente na lista
		Solucao* nodo_atual = *path;
		int tamanho = 0;
		float distanciaTotal = 0;
		//Percorre a lista novamente (agora com o novo nodo adicionado)
		while (nodo_atual != NULL) {
			tamanho++;
			distanciaTotal += nodo_atual->TrajetoParcial.peso;
			nodo_atual = nodo_atual->prox;
		}
		novoNodo->tamanho = tamanho;
		novoNodo->distanciaTotal = distanciaTotal;
		return 1;
	}

	//Função que remove o ultimo NodoPath da lista de uma solução
	int RemoverUltimoNodoPath(Solucao** path) {

		//Verifica se existe algum nodo na lista da solução
		if (*path == NULL) {
			return 1; //Não há nada para remover
		}

		Solucao* nodoRemover = *path;
		Solucao* nodoAnterior = NULL;

		//Percorre a lista até chegar ao último nó
		while (nodoRemover->prox != NULL) {
			nodoAnterior = nodoRemover;
			nodoRemover = nodoRemover->prox;
		}

		//Atualiza o tamanho do caminho atual
		(*path)->tamanho--;

		//Remove o último nó do caminho atual
		if (nodoAnterior == NULL) {
			*path = NULL; //O caminho ficou vazio
		}
		else {
			nodoAnterior->prox = NULL;
			free(nodoRemover);
		}
		return 1;
	}

	//Função que calcula a distancia total de qualquer solução
	int CalcularDistanciaTotal(Solucao* solCalc) {

		if (solCalc == NULL) return -1; //Verifica se a solucao existe

		Solucao* ultimo_nodoA = solCalc;
		float distancia = 0;
		while (ultimo_nodoA != NULL) {
			distancia += ultimo_nodoA->TrajetoParcial.peso;
			ultimo_nodoA = ultimo_nodoA->prox;
		}
		return distancia;
	}

	//Função que calcula a melhor solução de todas de acordo com a distancia
	Solucao* CalcularMelhorSolucao(Solucao* solAtual, Solucao* solMelhor) {
		 
		//Caso seja Nulo, ou seja a primeira vez
		Solucao* solProvisoria;
		if (solMelhor == NULL) {
			solProvisoria = NULL;
			while (solAtual != NULL) {
				AdicionarNodoPath(&solProvisoria, solAtual->TrajetoParcial);
				solAtual = solAtual->prox;
			}

			return solProvisoria;
		}

		//Calcula as distancias totais
		int distSolMelhor = CalcularDistanciaTotal(solMelhor); 
		int distSolAtual = CalcularDistanciaTotal(solAtual);

		// Verifica se a distância total do trajeto atual é menor que a da melhor solução
		if (distSolAtual < distSolMelhor) {
			solProvisoria = NULL;
			while (solAtual != NULL) {	
				AdicionarNodoPath(&solProvisoria, solAtual->TrajetoParcial);
				solAtual = solAtual->prox;
			}
			solMelhor = solProvisoria;
		}
		//ImprimirSolucao(solMelhor);//Imprime a melhor solução percorrida
		return solMelhor;
	}

	//Função para debugar os caminhos de todas soluções percorridas
	void ImprimirCaminho(Solucao* solucao) {
		if (solucao == NULL) {
			printf("Caminho vazio.\n");
			return;
		}

		Solucao* nodo_atual = solucao;
		while (nodo_atual != NULL) {
			printf("\nVertice: %d, Peso: %.2f", nodo_atual->TrajetoParcial.vertice, nodo_atual->TrajetoParcial.peso);
			nodo_atual = nodo_atual->prox;
		}
		printf("\n");
	}

	//Função para debugar o melhor caminho de todas as soluções percorridas
	void ImprimirSolucao(Solucao* solucao) {
		if (solucao == NULL) {
			printf("Caminho vazio.\n");
			return;
		}

		Solucao* nodo_atual = solucao;
		printf("\nMelhor solucao ate agora:");
		while (nodo_atual != NULL) {
			printf("\nVertice: %d, Peso: %.2f", nodo_atual->TrajetoParcial.vertice, nodo_atual->TrajetoParcial.peso);
			nodo_atual = nodo_atual->prox;
		}
		printf("\n");
	}

#pragma endregion

#pragma region FUNC_DE_SEARCH
	//Função DFS
	int DFS(Grafo* graf, int vertAtual, Solucao* solAtual) {

		//Verificações das variaveis
		if (graf == NULL || vertAtual < 0) return -1;

		//Cria uma suposta melhor solução com valor estático
		static Solucao* solMelhor;

		//Marca o vértice atual como visitado
		graf->vert[vertAtual].foiVisitado = true;
		
		//Percorre as adjacências do vértice atual
		Adjacencia* adj = graf->vert[vertAtual].cabeca;
		while (adj != NULL) {
			int proxVertice = adj->vertice;

			//Se o vertice não foi visitado e tem adjacencia com o vertice atual
			if (!graf->vert[proxVertice].foiVisitado && graf->matrizAdj[vertAtual][proxVertice] != 0) {

				//Adiciona ao caminho atual
				Trajeto nodoPath = CriarNodoPath(adj);
				AdicionarNodoPath(&solAtual, nodoPath);

				//Chama a recursividade
				DFS(graf, proxVertice, solAtual);
			}

			//Senao vai para o proximo vertice adjacente do vertice atual
			adj = adj->prox;
		}

		//Ao percorrer tudo calcula a melhor solução e imprime o caminho percorrido
		if (adj == NULL) {
			//ImprimirCaminho(solAtual); //Imprime todos os caminhos percorridos
			solMelhor = CalcularMelhorSolucao(solAtual, solMelhor);
	
		}
		
		//Desmarca o vértice atual como visitado e remove do caminho atual
		graf->vert[vertAtual].foiVisitado = false;
		RemoverUltimoNodoPath(&solAtual);

		
		
		return 1;
	}

#pragma endregion

#pragma region FUNC_FILES

	//Função para salvar o grafo em um arquivo binário
	int SalvarGrafoBinario(Grafo* graf, char* fileName) {
		//Verifica se o grafo existe
		if (graf == NULL) return -1;
		
		//Abre o arquivo binário em modo de escrita
		FILE* file = fopen(fileName, "wb");
		if (file == NULL) return -1;

		//Escreve o número de vértices e o número de arestas no arquivo
		fwrite(&(graf->nVertices), sizeof(int), 1, file);
		fwrite(&(graf->nArestas), sizeof(int), 1, file);

		for (int i = 0; i < graf->nVertices; i++) {
			for (int j = 0; j < graf->nVertices; j++) {
				fwrite(&(graf->matrizAdj[i][j]), sizeof(int), 1, file);
			}
		}
		// Escreve os vértices e suas adjacências no arquivo
		for (int i = 0; i < graf->nVertices; i++) {
			Adjacencia* adj = graf->vert[i].cabeca;
			while (adj != NULL) {
				AdjFile adjFile = { 0 };
				adjFile.origem = i;
				adjFile.destino = adj->vertice;
				adjFile.peso = adj->peso;
				adjFile.geocode = adj->local.geocode;
				fwrite(&adjFile, sizeof(AdjFile), 1, file);

				adj = adj->prox;
			}
		}
		//Fecha o arquivo
		fclose(file);
		//Destroi o grafo
		DestruirGrafo(graf);
		return 1;
	}

	// Função para ler o grafo de um arquivo binário
	Grafo* LerGrafoBinario(char* fileName, ListaLocal* lista) {
		// Abre o arquivo binário em modo de leitura
		FILE* file = fopen(fileName, "rb");
		if (file == NULL) return -1;

		// Lê o número de vértices e o número de arestas do arquivo
		int nVertices = 0, nArestas = 0;
		fread(&nVertices, sizeof(int), 1, file);
		fread(&nArestas, sizeof(int), 1, file);

		// Cria o grafo com o número de vértices lido do arquivo
		Grafo * graf = CriarGrafo(nVertices);


		// Lê a matriz de adjacência do arquivo
		float tempMatriz[MAX_VERTICES][MAX_VERTICES] = { 0 };
		for (int i = 0; i < nVertices; i++) {
			for (int j = 0; j < nVertices; j++) {
				fread(&(tempMatriz[i][j]), sizeof(int), 1, file);
				graf->matrizAdj[i][j] = tempMatriz[i][j];
			}
		}


		//Inicializo fora para usar em outra logica
		char* geocode = NULL;

		for (int i = 0; i < nVertices; i++)
		{
			char* geocodeEncontrado = EncontrarLocal(lista, i);
			Adjacencia* adj = graf->vert[i].cabeca;
			while (adj != NULL) {
				if (adj->vertice == i) {
					adj->local.geocode = geocodeEncontrado;
					break;
				}
				adj = adj->prox;
			}

			if (EncontrarLocal(lista, i) == NULL) {
				// Cria uma nova localização e adiciona à lista
				AdicionarLocalPorGeocode(&lista, geocode, i);
			}
		}

		// Fecha o arquivo
		fclose(file);

		return graf;
	}

#pragma endregion
