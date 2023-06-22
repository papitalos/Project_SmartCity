/**

 * @file   Transporte.h
 * @brief  informações dos transportes
 *
 * @author italo
 * @date   March 2023

**/

#pragma warning(disable : 4996)

#include "Consts.h"

#ifndef TRANSPORTE_H
#define TRANSPORTE_H

typedef struct Transporte Transporte;
typedef struct ListaTransporte ListaTransporte;
typedef enum {
	trotinete,
	bicicleta,
	skate
} TipoTransporte;

//assinaturas das funções
ListaTransporte* EncontrarTransporte(ListaTransporte* lista, int id);
ListaTransporte* ListarPorGeocode(ListaTransporte* lista, char geocode[STRG]);

bool AdicionarTransporte(ListaTransporte** lista, Transporte transporteAtual);
bool RemoverTransporte(ListaTransporte** lista, ListaTransporte* transporteAtual);
bool OrganizarTransportePorID(ListaTransporte* lista);
bool EditarTransporte(ListaTransporte* lista, Transporte transporteEditado);
bool TrocarTransporte(ListaTransporte* transporte1, ListaTransporte* transporte2);
bool OrganizarTransportePorBateria(ListaTransporte* lista);

int ContarTransportesPoucaBateria(ListaTransporte* lista, char geocode[STRG], int tipo);
int LerFileTransporte(ListaTransporte** lista);
int SalvarFileTransporte(ListaTransporte* lista);

//struct de dados
struct Transporte {
	int id;
	TipoTransporte tipo;
	float bateria;
	float custo;
	char geocode[STRG]; //geocode de onde o transporte está
	int locatorio;
};

struct ListaTransporte {
	Transporte transporte;
	ListaTransporte* anter;
	ListaTransporte* prox;
};

#endif
