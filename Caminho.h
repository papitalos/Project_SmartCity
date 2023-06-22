/** 
 
 * @file   Caminho.h
 * @brief  Estrutura de dados para guardar as informações dos caminhos percorridos
 * 
 * @author italo
 * @date   May 2023
 
**/

#pragma warning(disable : 4996)

#include "Consts.h"

#ifndef CAMINHO_H
#define CAMINHO_H

#define CAPACIDADE_DO_CAMINHAO 50 //em kilogramas

typedef struct ListaTrajetos Solucao;
typedef struct Caminhao Coletor;
typedef struct TrajetoParcial Trajeto;
typedef struct Caminhao Caminhao;

struct Caminhao {
    int pesoAtual;
};

struct TrajetoParcial {
    int vertice; 
    float peso;
};

struct ListaTrajetos{
    
    Trajeto TrajetoParcial;
    Solucao* prox;
    int tamanho;
    float distanciaTotal;
};

#endif
