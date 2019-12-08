#ifndef FUNCOES_H
#define FUNCOES_H

//-----------------------------------INCLUDES-----------------------------------//
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

//-----------------------------------DEFINES------------------------------------//
#define TAM_CIDADE      20
#define TAM_TEMPO       12


//------------------------------------SRUCTS------------------------------------//
struct Vertice {
    struct Vertice* adjacente;
    struct Vertice* proxLista;
    char cidade[TAM_CIDADE + 1];
    char estado[3];
    int distancia;
    char tempo[TAM_TEMPO + 1];
};
typedef struct Vertice Vertice;
typedef struct Vertice* Vertice_PTR;
typedef struct Vertice** Vertice_PTR_PTR;


typedef struct {
    Vertice_PTR_PTR listaVertices;
    int tam;
} VerticeList;
typedef VerticeList* VerticeList_PTR;


typedef struct {
    Vertice_PTR orig;
    Vertice_PTR dest;
} Aresta;
typedef Aresta* Aresta_PTR;
typedef Aresta** Aresta_PTR_PTR;


typedef struct {
    Aresta_PTR_PTR listaAresta;
    int tam;
} ArestaList;
typedef ArestaList* ArestaList_PTR;

//For Dijkstra
typedef struct {
    char cidadeDest[TAM_CIDADE + 1];
    char cidadeAnt[TAM_CIDADE + 1];
    char estadoDest[3];
    char estadoAnt[3];
    int distancia;
    bool analisado;
} Antecessor;
typedef Antecessor* Antecessor_PTR;
typedef Antecessor** Antecessor_PTR_PTR;


//----------------------------------FUNCTIONS-----------------------------------//

//FUNC[9] - ADJACENCY LIST
Vertice_PTR lista_adj(char*);

//FUNC[10] - DIJKSTRA
void dijkstra(char*, char*, char*);

//FUNC[11] - PRIM
void prim(char*, char*, char*);

//Auxiliary function for adjacency list
void inserir_lista_adj(Vertice_PTR_PTR, Vertice_PTR, Vertice_PTR);

//Display graph
void exibe_grafo(Vertice_PTR);

//Check file integrity
bool file_integro(FILE*);

//Receive custon String [Function's Matheus -> Monitor]
void scan_quote_string(char*);

#endif
