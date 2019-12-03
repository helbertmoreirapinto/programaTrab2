#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define TAM_CIDADE      20
#define TAM_TEMPO       12
#define TAM_REG         85
#define TAM_CAMPO_FIXO  8
#define STATUS_OK       '1'
#define REMOVIDO        '*'

#define LISTA_ADJ 9

struct Vertice {
    struct Vertice* adjacente;
    struct Vertice* proxLista;
    char cidade[TAM_CIDADE+1];
    char estado[3];
    int distancia;
    char tempo[TAM_TEMPO+1];
};
typedef struct Vertice Vertice;
typedef struct Vertice* Vertice_PTR;
typedef struct Vertice** Vertice_PTR_PTR;

bool file_integro(FILE*);
void lista_adj(char*);
void inserir_lista_adj(Vertice_PTR_PTR, Vertice_PTR, Vertice_PTR);
void exibe_grafo(Vertice_PTR);

int main() {
    int funcao;
    char param1[50];
    fscanf(stdin, "%d", &funcao);
    switch(funcao) {
    case LISTA_ADJ:
        fscanf(stdin, "%s", param1);
        lista_adj(param1);
        break;
    }
    return 0;
}

void lista_adj(char* nome_file) {
    FILE* file_bin = fopen(nome_file, "rb");
    if(!file_bin || !file_integro(file_bin)) {
        printf("Falha no carregamento do arquivo.");
        return;
    }
    Vertice_PTR lista = NULL;
    Vertice_PTR elem_1;
    Vertice_PTR elem_2;

    int num_vertices, num_arestas;
    int tam_cidade_orig, tam_cidade_dest, tam_tempo;

    fread(&num_vertices, sizeof(int), 1, file_bin);
    fread(&num_arestas, sizeof(int), 1, file_bin);
    fseek(file_bin, 10*sizeof(char), SEEK_CUR);
    char* aux;

    for(int i = 0; i < num_arestas; i++) {
        aux = calloc(TAM_REG-TAM_CAMPO_FIXO, sizeof(Vertice));
        elem_1 = calloc(1, sizeof(Vertice));
        elem_2 = calloc(1, sizeof(Vertice));

        fread(elem_1->estado, 2*sizeof(char),   1, file_bin);
        if(elem_1->estado[0] == REMOVIDO) {
            i--;
            continue;
        }
        fread(elem_2->estado, 2*sizeof(char),   1, file_bin);
        fread(&elem_1->distancia, sizeof(int),  1, file_bin);
        elem_2->distancia = elem_1->distancia;
        fread(aux, (TAM_REG-TAM_CAMPO_FIXO)*sizeof(char), 1, file_bin);

        tam_cidade_orig = strcspn(&aux[0], "|");
        strncpy(elem_1->cidade, &aux[0], tam_cidade_orig);

        tam_cidade_dest = strcspn(&aux[tam_cidade_orig+1], "|");
        strncpy(elem_2->cidade, &aux[tam_cidade_orig+1], tam_cidade_dest);

        tam_tempo = strcspn(&aux[tam_cidade_orig+tam_cidade_dest+2], "|");
        strncpy(elem_2->tempo, &aux[tam_cidade_orig+tam_cidade_dest+2], tam_tempo);
        strcpy(elem_1->tempo, elem_2->tempo);

        inserir_lista_adj(&lista, elem_1, elem_2);
    }
    exibe_grafo(lista);
}

void inserir_lista_adj(Vertice_PTR_PTR lista, Vertice_PTR A, Vertice_PTR B) {
    Vertice_PTR elem = (*lista);

    if(!elem) {
        Vertice_PTR V1 = calloc(1,sizeof(Vertice));
        Vertice_PTR V2 = calloc(1,sizeof(Vertice));

        strcpy(V1->cidade, A->cidade);
        strcpy(V1->estado, A->estado);
        V1->adjacente = B;

        strcpy(V2->cidade, B->cidade);
        strcpy(V2->estado, B->estado);
        V2->adjacente = A;

        if(strcmp(A->cidade, B->cidade) < 0) {
            (*lista) = V1;
            V1->proxLista = V2;
            return;
        } else if(strcmp(A->cidade, B->cidade) > 0) {
            (*lista) = V2;
            V2->proxLista = V1;
            return;
        }
    }

    bool vertice_na_lista;
    Vertice_PTR ant_elem;
    Vertice_PTR ADJ1 = calloc(1, sizeof(Vertice));
    Vertice_PTR ADJ2 = calloc(1, sizeof(Vertice));
    Vertice_PTR NOVO;

    strcpy(ADJ1->cidade, A->cidade);
    strcpy(ADJ1->estado, A->estado);
    strcpy(ADJ1->tempo, A->tempo);
    ADJ1->distancia = A->distancia;

    strcpy(ADJ2->cidade, B->cidade);
    strcpy(ADJ2->estado, B->estado);
    strcpy(ADJ2->tempo, B->tempo);
    ADJ2->distancia = B->distancia;

    //Inserindo aresta A -> B
    vertice_na_lista = false;
    elem = (*lista);
    ant_elem = NULL;

    while(elem) {
        if(strcmp(elem->cidade, A->cidade) >= 0) {
            if(!strcmp(elem->cidade, A->cidade)) {
                vertice_na_lista = true;
            }
            break;
        }
        ant_elem = elem;
        elem = elem->proxLista;
    }

    //Inserindo vertice A
    if(!vertice_na_lista) {
        NOVO = calloc(1, sizeof(Vertice));
        strcpy(NOVO->cidade, A->cidade);
        strcpy(NOVO->estado, A->estado);
        NOVO->adjacente = ADJ2;

        if(!ant_elem) {
            NOVO->proxLista = (*lista);
            (*lista) = NOVO;
            elem = NOVO;
        } else {
            NOVO->proxLista = elem;
            ant_elem->proxLista = NOVO;
        }
    } else {
        ant_elem = elem;
        elem = elem->adjacente;
        while(elem) {
            if(strcmp(elem->cidade, B->cidade) > 0) {
                break;
            }
            ant_elem = elem;
            elem = elem->adjacente;
        }
        ant_elem->adjacente = ADJ2;
        ADJ2->adjacente = elem;
    }

    //Inserindo aresta B -> A
    vertice_na_lista = false;
    elem = (*lista);
    ant_elem = NULL;

    while(elem) {
        if(strcmp(elem->cidade, B->cidade) >= 0) {
            if(!strcmp(elem->cidade, B->cidade)) {
                vertice_na_lista = true;
            }
            break;
        }
        ant_elem = elem;
        elem = elem->proxLista;
    }

    //Inserindo vertice A
    if(!vertice_na_lista) {
        Vertice_PTR NOVO = calloc(1, sizeof(Vertice));
        strcpy(NOVO->cidade, B->cidade);
        strcpy(NOVO->estado, B->estado);
        NOVO->adjacente = ADJ1;

        if(!ant_elem) {
            NOVO->proxLista = (*lista);
            (*lista) = NOVO;
            elem = NOVO;
        } else {
            NOVO->proxLista = elem;
            ant_elem->proxLista = NOVO;
        }
    } else {
        ant_elem = elem;
        elem = elem->adjacente;
        while(elem) {
            if(strcmp(elem->cidade, A->cidade) > 0) {
                break;
            }
            ant_elem = elem;
            elem = elem->adjacente;
        }
        ant_elem->adjacente = ADJ1;
        ADJ1->adjacente = elem;
    }
}

bool file_integro(FILE* file_bin) {
    rewind(file_bin);
    char status;
    fread(&status, sizeof(char), 1, file_bin);
    return status == STATUS_OK;
}

void exibe_grafo(Vertice_PTR elem) {
    Vertice_PTR auxAdj;
    Vertice_PTR auxProx = elem;

    while(auxProx) {
        fprintf(stdout, "%s %s", auxProx->cidade, auxProx->estado);
        auxAdj = auxProx->adjacente;
        while(auxAdj) {
            fprintf(stdout, " %s %s %d", auxAdj->cidade, auxAdj->estado, auxAdj->distancia);
            if(auxAdj->tempo != NULL && strlen(auxAdj->tempo) > 0)
                fprintf(stdout, " %s", auxAdj->tempo);
            auxAdj = auxAdj->adjacente;
        }
        fprintf(stdout, "\n");
        auxProx = auxProx->proxLista;
    }
}

