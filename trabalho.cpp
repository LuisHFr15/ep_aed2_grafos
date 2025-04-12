//--------------------------------------------------------------
// COLOQUE OS NOMES DOS RESPONSÁVEIS AQUI: Luís Henrique Fernandes Ramires
//--------------------------------------------------------------

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <malloc.h>
#include <iostream>

// INFORME O NRO DO GRUPO CONFORME PLANILHA DIVULGADA
int grupo() {
  return 19;
}

// ######### ESCREVA O NROUSP AQUI
int nroUSP1() {
    return 13671998;
}

// E O SEGUNDO NROUSP AQUI, SE HOUVER
int nroUSP2() {
    return 0;
}

// elemento da lista de resposta - NAO ALTERE ESTA DEFINICAO
typedef struct estr {
      int adj; // um nro. de vertice
      struct estr *prox;
} NO;

// -- utils
typedef struct {
	NO* inicio;
	int flagV1;
	int distV1;
	int flagV2;
	int distV2;
}VERTICE;

#define INFINITY 9999

// -- funcoes auxiliares
void inicializaGrafo(VERTICE* g, int V) {
	for(int i = 1; i <= V; i++) {
			g[i].inicio = NULL;
			g[i].distV1 = 0;
			g[i].distV2 = 0;
	}
}

void flagsZero(VERTICE* g, int V) {
	int i;
	for(i = 1; i <= V; i++) {
		g[i].flagV1 = 0;
		g[i].flagV2 = 0;
	}
}

NO* arestaExist(VERTICE* g, int V, int i, int j, NO* *ant) {
	*ant = NULL;
	NO* p = g[i].inicio;

	while(p) {
			if(p -> adj == j) {
					return p;
			}
			*ant = p;
			p = p -> prox;
	}
	return NULL;
}

bool insereAresta(VERTICE* g, int V, int i, int j) {
	if(i > V || j > V || i < 1 || j < 1) return false;

	NO* ant;
	NO* p = arestaExist(g, V, i, j, &ant);
	if(p) return false;

	NO* novo = (NO*) malloc(sizeof(NO));
	novo -> adj = j;

	novo -> prox = g[i].inicio;
	g[i].inicio = novo;

	return true;
}

bool removeAresta(VERTICE* g, int V, int i, int j) {
	if(i > V || j > V || i < 1 || j < 1) return false;

	NO* ant;
	NO* p = arestaExist(g, V, i, j, &ant);
	if(!p) return false;

	if(ant) ant -> prox = p -> prox;
	else g[i].inicio = p -> prox;
	free(p);

	return true;
}

void deletaGrafo(VERTICE* g, int V) {
	int i;
	for(i = 1; i <= V; i++) {
		NO* p = g[i].inicio;
		while(p) {
			NO* aux = p;
			p = p -> prox;
			free(aux);
		}
	}
	free(g);
}

VERTICE* criaGrafo(int V, int A, int ij[]) {
	VERTICE *g = (VERTICE*) malloc(sizeof(VERTICE) * (V + 1));
	inicializaGrafo(g, V);
	if(!ij || A == 0 || !A) return g;

	int i;
	for(i = 0; i < A; i++) {
		insereAresta(g, V, ij[2 * i], ij[(2 * i) + 1]);
		insereAresta(g, V, ij[(2 * i) + 1], ij[2 * i]);
	}

	return g;
}

void dijkstra(VERTICE* g, int V, int v1, int v2) {
	flagsZero(g, V);

	// o vertice principal servirá como o vértice a ser usado para calcular distancia
	int i;
	for(i = 1; i <= V; i++) {
		g[i].distV1 = INFINITY;
		g[i].distV2 = INFINITY;
	}

	g[v1].distV1 = 0;

	int z = v1;
	while(z > 0) {
		g[z].flagV1 = 1;
		NO* p = g[z].inicio;

		while(p) {
			if(g[p -> adj].flagV1 == 0) {
				int distAux = g[z].distV1 + 1;

				if(g[p -> adj].distV1 > distAux) {
					g[p -> adj].distV1 = distAux;
				}
			}
			p = p -> prox;
		}

		z = 0;
		int menorCusto = INFINITY;
		for(i = 1; i <= V; i++) {
			if(g[i].flagV1 == 0) {
				if(g[i].distV1 < menorCusto) {
					menorCusto = g[i].distV1;
					z = i;
				}
			}
		}
	}

	g[v2].distV2 = 0;

	z = v2;
	while(z > 0) {
		g[z].flagV2 = 1;
		NO* p = g[z].inicio;

		while(p) {
			if(g[p -> adj].flagV2 == 0) {
				int distAux = g[z].distV2 + 1;

				if(g[p -> adj].distV2 > distAux) {
					g[p -> adj].distV2 = distAux;
				}
			}
			p = p -> prox;
		}

		z = 0;
		int menorCusto = INFINITY;
		for(i = 1; i <= V; i++) {
			if(g[i].flagV2 == 0) {
				if(g[i].distV2 < menorCusto) {
					menorCusto = g[i].distV2;
					z = i;
				}
			}
		}
	}
}



// funcao principal
NO *equidistantes(int V, int A, int ij[], int v1, int v2);

NO *equidistantes(int V, int A, int ij[], int v1, int v2)
{
	VERTICE* g = criaGrafo(V, A, ij);
	dijkstra(g, V, v1, v2);

	NO* resultado = NULL;

	int i;
	for(i = 1; i <= V; i++) {
		if(g[i].distV1 == g[i].distV2 && i != v1 && i != v2) {
			NO* novo = (NO*) malloc(sizeof(NO));
			novo -> adj = i;
			novo -> prox = resultado;
			resultado = novo;
		}
	}

	deletaGrafo(g, V);

	return resultado;
}