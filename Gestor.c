/** 
 
 * @file   Gestor.c
 * @brief  arquivo de manipulação de dados dos gestores
 * 
 * @author italo
 * @date   March 2023
 
**/

#include "Gestor.h"

/*
* 
*/
bool AdicionarGestor(ListaGestor** lista, Gestor gestorAtual) {

	ListaGestor* newGestor = (ListaGestor*)malloc(sizeof(ListaGestor));

	if (newGestor == NULL) {
		free(newGestor);
		return false;
	}

	newGestor->gestor = gestorAtual;
	newGestor->anter = NULL;
	newGestor->prox = NULL;

	if (*lista == NULL) {
		*lista = newGestor;
		return true;
	}

	ListaGestor* ultimo_gestor = *lista;

	while (ultimo_gestor->prox != NULL) {
		ultimo_gestor = ultimo_gestor->prox;
	}

	ultimo_gestor->prox = newGestor;
	ultimo_gestor->prox->anter = ultimo_gestor;
	return true;
}
bool RemoverGestor(ListaGestor** lista, ListaGestor* gestorAtual) {

	// Verifica se o gestor atual é nulo
	if (gestorAtual == NULL) return false;

	// Atualiza a lista de gestores
	if (*lista == gestorAtual) {
		*lista = gestorAtual->prox;
		(*lista)->anter = NULL;
	}
	else {
		if (gestorAtual->anter != NULL)
			gestorAtual->anter->prox = gestorAtual->prox;

		if (gestorAtual->prox != NULL)
			gestorAtual->prox->anter = gestorAtual->anter;
	}

	// Libera a memória do gestor atual
	free(gestorAtual);

	return true;
}
bool TrocarGestor(ListaGestor* gestor1, ListaGestor* gestor2) {

	// Troca os dados dos gestores usando uma variável auxiliar
	Gestor aux = gestor1->gestor;
	gestor1->gestor = gestor2->gestor;
	gestor2->gestor = aux;
	return 1;
}
bool EditarGestor(ListaGestor* lista, Gestor gestorEditado) {

	// Verifica se a lista é nula
	if (lista == NULL) return false;

	// Atualiza o gestor na lista com os dados do gestor editado
	lista->gestor = gestorEditado;

	return true;
}
bool OrganizaGestorrPorID(ListaGestor* lista) {

	// Verifica se a lista é nula
	if (lista == NULL) return false;

	bool terminou = false;
	ListaGestor* listaAtual = NULL;

	// Ordena a lista de gestores por ID usando o algoritmo Bubble Sort
	while (!terminou)
	{
		listaAtual = lista;
		while (listaAtual->prox != NULL)
		{
			if (listaAtual->gestor.id > listaAtual->prox->gestor.id) {
				TrocarGestor(listaAtual, listaAtual->prox);
				terminou = false;
			}
			listaAtual = listaAtual->prox;
		}
		terminou = true;
	}
	return true;
}

ListaGestor* EncontrarGestor(ListaGestor* lista, int id) {

	// Verifica se a lista é nula
	if (lista == NULL) return NULL;

	ListaGestor* listaAtual = lista;

	// Percorre a lista de gestores e procura o gestor com o ID especificado
	while (listaAtual != NULL)
	{
		if (listaAtual->gestor.id == id) return listaAtual;
		listaAtual = listaAtual->prox;
	}

	return NULL;
}


int LerFileGestor(ListaGestor** lista) {

	Gestor tempGestor = { 0 };

	FILE* file;

	fopen_s(&file, "Data\\gestor.txt", "r");


	if (file == NULL) return 2;

	char buffer[256];
	while (fgets(buffer, sizeof(buffer), file) != NULL)
	{
		if (sscanf(buffer, "%d;%[^;];%[^;];%s\n",
			&tempGestor.id, tempGestor.nome, tempGestor.email, tempGestor.senha) != 4)
			return 3;

		AdicionarGestor(lista, tempGestor);
	}

	fclose(file);
	return 1;
}

/*
*/
int SalvarFileGestor(ListaGestor* lista) {

	if (lista == NULL) return 4;

	ListaGestor* tempGestor = lista;

	FILE* file;

	fopen_s(&file, "Data\\gestor.bin", "wb");

	if (file == NULL) return 2;

	do
	{
		fwrite(&(tempGestor->gestor), sizeof(ListaGestor), 1, file);

		tempGestor = tempGestor->prox;
	} while (tempGestor != NULL);

	fclose(file);
	return 1;
}
