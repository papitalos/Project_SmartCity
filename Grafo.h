/**

 * @file   Grafo.h
 * @brief  definição das structs do grafo
 *
 * @author italo
 * @date   May 2023

**/

#pragma warning(disable : 4996)

#include "Consts.h"
#include "Caminho.h"
#include "Transporte.h"
#include "Localizacao.h"

#ifndef GRAFOS_H
#define GRAFOS_H

typedef struct LocalGrafo LocalGrafo;
typedef struct Grafo Grafo;
typedef struct NodosAdjacentes Adjacencia;
typedef struct NodoVertice Vertice;
typedef struct AdjFile AdjFile;

//Assinaturas das funções do grafo e DFS
Grafo* CriarGrafo(int vert);
Grafo* LerGrafoBinario(char* fileName, ListaLocal* lista);
Adjacencia* CriaAdjacencia(int v, float peso);

int CriaAresta(Grafo* graf, int verIni, int verFin, float peso);
int CriarMapaMatriz(Grafo* graf);

int ContarVertices(Grafo* graf, ListaLocal* lista);
int EnderecarLocal(Grafo* graf, ListaLocal* lista);
int ListarTransportesPorVertice(Grafo* graf, ListaTransporte* lista);

Solucao* CalcularMelhorSolucao(Solucao* solAtual, Solucao* solMelhor);
Trajeto CriarNodoPath(Adjacencia* adjacencia);

int CalcularDistanciaTotal(Solucao* solCalc);
int DFS(Grafo* graf, int vertAtual, Solucao* solAtual);
int AdicionarNodoPath(Solucao** path, Trajeto trajeto);

void DebugGrafo(Grafo* graf);
void ImprimirCaminho(Solucao* solucao);
void ImprimirSolucao(Solucao* solucao);

int SalvarGrafoBinario(Grafo* graf, char* fileName);


//Structs de file

struct AdjFile {
	int origem;
	int destino;
	float peso;
	char* geocode;
};

//Structs do grafo
struct LocalGrafo {
	char* geocode; //geocode do lugar do vertice
	ListaTransporte* TransportesGeocode; //lista de transportes daquele geocode especifico
};
//Adjacencias
struct NodosAdjacentes {
	int vertice;
	float peso;
	LocalGrafo local;

	struct NodosAdjacentes *prox;
};
//Vertice
struct NodoVertice {
	Adjacencia* cabeca;
	
	bool foiVisitado;

};
//Grafo
struct Grafo {
	Vertice* vert;
	
	int nVertices;
	int nArestas;
	float matrizAdj[MAX_VERTICES][MAX_VERTICES];
	
};

#endif
