/** 
 
 * @file   Localizacao.c
 * @brief  Funções para salvar e ler todas as localizações existentes no mapa atual
 * 
 * @author italo
 * @date   May 2023
 
**/
#include "Localizacao.h"

bool AdicionarLocal(ListaLocal** lista, Local localAtual) {

	ListaLocal* newLocal = (ListaLocal*)malloc(sizeof(ListaLocal));

	if (newLocal == NULL) return false;

	newLocal->local = localAtual;
	newLocal->anter = NULL;
	newLocal->prox = NULL;

	if (*lista == NULL) {
		*lista = newLocal;
		return true;
	}

	ListaLocal* ultimo_local = *lista;

	while (ultimo_local->prox != NULL) {
		ultimo_local = ultimo_local->prox;
	}
	ultimo_local->prox = newLocal;
	ultimo_local->prox->anter = ultimo_local;
	return true;
}
bool AdicionarLocalPorGeocode(ListaLocal** lista, char* geocode, int id) {

	ListaLocal* newLocal = (ListaLocal*)malloc(sizeof(ListaLocal));

	if (newLocal == NULL) return false;
	
	newLocal->local.id = id;
	strcpy(newLocal->local.geocode, geocode);
	strcpy(newLocal->local.nome, "[RANDOM]");
	newLocal->anter = NULL;
	newLocal->prox = NULL;

	if (*lista == NULL) {
		*lista = newLocal;
		return true;
	}

	ListaLocal* ultimo_local = *lista;

	while (ultimo_local->prox != NULL) {
		ultimo_local = ultimo_local->prox;
	}
	ultimo_local->prox = newLocal;
	ultimo_local->prox->anter = ultimo_local;
	return true;
}
bool RemoverLocal(ListaLocal** lista, ListaLocal* localAtual) {

	// Verifica se o local atual é nulo
	if (localAtual == NULL) return false;

	// Atualiza a lista de locais
	if (*lista == localAtual) {
		*lista = localAtual->prox;
		(*lista)->anter = NULL;
	}
	else {
		if (localAtual->anter != NULL)
			localAtual->anter->prox = localAtual->prox;

		if (localAtual->prox != NULL)
			localAtual->prox->anter = localAtual->anter;
	}

	// Libera a memória do local atual
	free(localAtual);

	return true;
}
bool TrocarLocal(ListaLocal* local1, ListaLocal* local2) {

	// Troca os dados dos locais usando uma variável auxiliar
	Local aux = local1->local;
	local1->local = local2->local;
	local2->local = aux;
	return 1;
}
bool EditarLocal(ListaLocal* lista, Local localEditado) {

	// Verifica se a lista é nula
	if (lista == NULL) return false;

	// Atualiza o transporte na lista com os dados do transporte editado
	lista->local = localEditado;

	return true;
}
bool OrganizarLocalPorID(ListaLocal* lista) {

	// Verifica se a lista é nula
	if (lista == NULL) return false;

	bool terminou = false;
	ListaLocal* listaAtual = NULL;

	// Ordena a lista de transportes por ID usando o algoritmo Bubble Sort
	while (!terminou)
	{
		listaAtual = lista;
		while (listaAtual->prox != NULL)
		{
			if (listaAtual->local.id > listaAtual->prox->local.id) {
				TrocarLocal(listaAtual, listaAtual->prox);
				terminou = false;
			}
			listaAtual = listaAtual->prox;
		}
		terminou = true;
	}
	return true;
}

char* EncontrarLocal(ListaLocal* lista, int id) {

	// Verifica se a lista é nula
	if (lista == NULL) return NULL;

	ListaLocal* listaAtual = lista;

	// Percorre a lista de locais e procura o local com o ID especificado
	while (listaAtual != NULL)
	{
		if (listaAtual->local.id == id) return listaAtual->local.geocode;
		listaAtual = listaAtual->prox;
	}

	return NULL;
}

int LerFileLocal(ListaLocal** lista) {

	Local tempLocal = { 0 };

	FILE* file;

	fopen_s(&file, "Data\\local.txt", "r");


	if (file == NULL) return 2;

	char buffer[256];
	while (fgets(buffer, sizeof(buffer), file) != NULL)
	{
		if (sscanf(buffer, "%d;%[^;];%[^;]\n",
			&tempLocal.id,tempLocal.geocode,tempLocal.nome) != 3)
			return 3;

		AdicionarLocal(lista, tempLocal);
	}

	fclose(file);
	return 1;
}
int SalvarFileLocal(ListaLocal* lista) {

	if (lista == NULL) return 4;

	ListaLocal* tempLocal = lista;

	FILE* file;

	fopen_s(&file, "Data\\transporte.bin", "wb");

	if (file == NULL) return 2;

	do
	{
		fwrite(&(tempLocal->local), sizeof(ListaLocal), 1, file);

		tempLocal = tempLocal->prox;
	} while (tempLocal != NULL);

	fclose(file);
	return 1;
}