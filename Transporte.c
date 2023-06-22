/** 
 
 * @file   Transportee.c
 * @brief  arquivo de manipulação de dados dos transportes
 * 
 * @author italo
 * @date   March 2023
 
**/

#include "Transporte.h"

bool AdicionarTransporte(ListaTransporte** lista, Transporte transporteAtual) {

	ListaTransporte* newTransporte = (ListaTransporte*)malloc(sizeof(ListaTransporte));

	if (newTransporte == NULL) return false;

	newTransporte->transporte = transporteAtual;
	newTransporte->anter = NULL;
	newTransporte->prox = NULL;

	if (*lista == NULL) {
		*lista = newTransporte;
		return true;
	}

	ListaTransporte* ultimo_transporte = *lista;

	while (ultimo_transporte->prox != NULL) {
		ultimo_transporte = ultimo_transporte->prox;
	}
	ultimo_transporte->prox = newTransporte;
	ultimo_transporte->prox->anter = ultimo_transporte;
	return true;
}
bool RemoverTransporte(ListaTransporte** lista, ListaTransporte* transporteAtual) {

	// Verifica se o transporte atual é nulo
	if (transporteAtual == NULL) return false;

	// Atualiza a lista de transportes
	if (*lista == transporteAtual) {
		*lista = transporteAtual->prox;
		(*lista)->anter = NULL;
	}
	else {
		if (transporteAtual->anter != NULL)
			transporteAtual->anter->prox = transporteAtual->prox;

		if (transporteAtual->prox != NULL)
			transporteAtual->prox->anter = transporteAtual->anter;
	}

	// Libera a memória do transporte atual
	free(transporteAtual);

	return true;
}
bool TrocarTransporte(ListaTransporte* transporte1, ListaTransporte* transporte2) {

	// Troca os dados dos transportes usando uma variável auxiliar
	Transporte aux = transporte1->transporte;
	transporte1->transporte = transporte2->transporte;
	transporte2->transporte = aux;
	return 1;
}
bool EditarTransporte(ListaTransporte* lista, Transporte transporteEditado) {

	// Verifica se a lista é nula
	if (lista == NULL) return false;

	// Atualiza o transporte na lista com os dados do transporte editado
	lista->transporte = transporteEditado;

	return true;
}
bool OrganizarTransportePorID(ListaTransporte* lista) {

	// Verifica se a lista é nula
	if (lista == NULL) return false;

	bool terminou = false;
	ListaTransporte* listaAtual = NULL;

	// Ordena a lista de transportes por ID usando o algoritmo Bubble Sort
	while (!terminou)
	{
		listaAtual = lista;
		while (listaAtual->prox != NULL)
		{
			if (listaAtual->transporte.id > listaAtual->prox->transporte.id) {
				TrocarTransporte(listaAtual, listaAtual->prox);
				terminou = false;
			}
			listaAtual = listaAtual->prox;
		}
		terminou = true;
	}
	return true;
}
bool OrganizarTransportePorBateria(ListaTransporte* lista) {

	// Verifica se a lista é nula
	if (lista == NULL) return false;

	bool terminou = false;
	ListaTransporte* listaAtual = NULL;

	// Ordena a lista de transportes por vida útil da bateria usando o algoritmo Bubble Sort
	while (!terminou)
	{
		terminou = true;
		listaAtual = lista;
		while (listaAtual->prox != NULL)
		{
			if (listaAtual->transporte.bateria < listaAtual->prox->transporte.bateria) {
				TrocarTransporte(listaAtual, listaAtual->prox);
				terminou = false;
			}
			listaAtual = listaAtual->prox;
		}
	}
	return true;
}

ListaTransporte* EncontrarTransporte(ListaTransporte* lista, int id) {

	// Verifica se a lista é nula
	if (lista == NULL) return NULL;

	ListaTransporte* listaAtual = lista;

	// Percorre a lista de transportes e procura o transporte com o ID especificado
	while (listaAtual != NULL)
	{
		if (listaAtual->transporte.id == id) return listaAtual;
		listaAtual = listaAtual->prox;
	}

	return NULL;
}
ListaTransporte* ListarPorGeocode(ListaTransporte* lista, char geocode[STRG]) {
	if (lista == NULL) return NULL;

	ListaTransporte* listaGeocode = NULL;
	ListaTransporte* listaAtual = lista;


	while (listaAtual != NULL)
	{
		if (strcmp(listaAtual->transporte.geocode, geocode) == 0) {
			AdicionarLocal(&listaGeocode, listaAtual->transporte);
		}
		listaAtual = listaAtual->prox;
	}

	return listaGeocode;
}

int ContarTransportesPoucaBateria(ListaTransporte* lista, char geocode[STRG], int tipo) {
	// Verifica se a lista é nula
	if (lista == NULL) return NULL;

	ListaTransporte* listaAtual = lista;
	int qtdTransporte = 0;
	// Percorre a lista de transportes e procura o transporte com menos ou igual de 50% de bateria
	while (listaAtual != NULL)
	{
		if (listaAtual->transporte.bateria <= 50 && strcmp(listaAtual->transporte.geocode, geocode) == 0 && listaAtual->transporte.tipo == tipo) {
			qtdTransporte++;
		}
		listaAtual = listaAtual->prox;
		
	}

	return qtdTransporte;//retorna a quantidade de transportes abaixo de 50% o geocode
}
int LerFileTransporte(ListaTransporte** lista) {

	Transporte tempTransporte = { 0 };

	FILE* file;

	fopen_s(&file, "Data\\transporte.txt", "r");


	if (file == NULL) return 2;

	char buffer[256];
	while (fgets(buffer, sizeof(buffer), file) != NULL)
	{
		if (sscanf(buffer, "%d;%d;%f;%f;%[^;];%d\n",
			&tempTransporte.id, &tempTransporte.tipo, &tempTransporte.bateria, &tempTransporte.custo, tempTransporte.geocode, &tempTransporte.locatorio) != 6)
			return 3;

		AdicionarTransporte(lista, tempTransporte);
	}

	fclose(file);
	return 1;
}
int SalvarFileTransporte(ListaTransporte* lista) {

	if (lista == NULL) return 4;

	ListaTransporte* tempTransporte = lista;

	FILE* file;

	fopen_s(&file, "Data\\transporte.bin", "wb");

	if (file == NULL) return 2;

	do
	{
		fwrite(&(tempTransporte->transporte), sizeof(ListaTransporte), 1, file);

		tempTransporte = tempTransporte->prox;
	} while (tempTransporte != NULL);

	fclose(file);
	return 1;
}
