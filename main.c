#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdbool.h>

#define TAM_CIDADE      20
#define TAM_TEMPO       12
#define TAM_REG         85
#define TAM_CAMPO_FIXO  8
#define STATUS_OK       '1'
#define REMOVIDO        '*'

#define LISTA_ADJ   9
#define DIJKSTRA    10
#define PRIM        11

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

typedef struct {
    char cidadeDest[20];
    char estadoDest[3];
    char cidadeAnt[20];
    char estadoAnt[3];
    int distancia;
    bool analisado;
} Antecessor;
typedef Antecessor* Antecessor_PTR;
typedef Antecessor** Antecessor_PTR_PTR;


bool file_integro(FILE*);
Vertice_PTR lista_adj(char*);
void inserir_lista_adj(Vertice_PTR_PTR, Vertice_PTR, Vertice_PTR);
void prim(char*, char*, char*);
void exibe_grafo(Vertice_PTR);
void scan_quote_string(char*);
void dijkstra(char*, char*, char*);

int main() {
    int funcao;
    char nome_file[50];
    char tipo_campo[50];
    char valor_campo[50];

    fscanf(stdin, "%d", &funcao);

    switch (funcao) {
    case LISTA_ADJ:
        fscanf(stdin, "%s", nome_file);
        Vertice_PTR lista = lista_adj(nome_file);
        exibe_grafo(lista);
        break;
    case DIJKSTRA:
        fscanf(stdin, "%s", nome_file);
        fscanf(stdin, "%s", tipo_campo);
        scan_quote_string(valor_campo);
        dijkstra(nome_file, tipo_campo, valor_campo);
        break;
        break;
    case PRIM:
        fscanf(stdin, "%s", nome_file);
        fscanf(stdin, "%s", tipo_campo);
        scan_quote_string(valor_campo);
        prim(nome_file, tipo_campo, valor_campo);
        break;
    }
    return 0;
}

Vertice_PTR lista_adj(char* nome_file) {
    FILE* file_bin = fopen(nome_file, "rb");
    if (!file_bin || !file_integro(file_bin)) {
        printf("Falha no carregamento do arquivo.");
        return NULL;
    }
    Vertice_PTR lista = NULL;
    Vertice_PTR elem_1;
    Vertice_PTR elem_2;

    int num_vertices, num_arestas;
    int tam_cidade_orig, tam_cidade_dest, tam_tempo;

    fread(&num_vertices, sizeof(int), 1, file_bin);
    fread(&num_arestas, sizeof(int), 1, file_bin);
    fseek(file_bin, 10 * sizeof(char), SEEK_CUR);
    char* aux;

    for (int i = 0; i < num_arestas; i++) {
        elem_1 = calloc(1, sizeof(Vertice));
        elem_2 = calloc(1, sizeof(Vertice));

        fread(elem_1->estado, 2 * sizeof(char), 1, file_bin);
        fread(elem_2->estado, 2 * sizeof(char), 1, file_bin);
        fread(&elem_1->distancia, sizeof(int), 1, file_bin);
        elem_2->distancia = elem_1->distancia;

        aux = calloc(TAM_REG - TAM_CAMPO_FIXO, sizeof(Vertice));
        fread(aux, (TAM_REG - TAM_CAMPO_FIXO) * sizeof(char), 1, file_bin);
        if (elem_1->estado[0] == REMOVIDO) {
            i--;
            continue;
        }

        tam_cidade_orig = strcspn(&aux[0], "|");
        strncpy(elem_1->cidade, &aux[0], tam_cidade_orig);

        tam_cidade_dest = strcspn(&aux[tam_cidade_orig + 1], "|");
        strncpy(elem_2->cidade, &aux[tam_cidade_orig + 1], tam_cidade_dest);

        tam_tempo = strcspn(&aux[tam_cidade_orig + tam_cidade_dest + 2], "|");
        strncpy(elem_2->tempo, &aux[tam_cidade_orig + tam_cidade_dest + 2], tam_tempo);
        strcpy(elem_1->tempo, elem_2->tempo);

        inserir_lista_adj(&lista, elem_1, elem_2);
        inserir_lista_adj(&lista, elem_2, elem_1);

        free(elem_1);
        free(elem_2);
    }
    fclose(file_bin);
    return lista;
}

void inserir_lista_adj(Vertice_PTR_PTR lista, Vertice_PTR A, Vertice_PTR B) {
    Vertice_PTR elem = (*lista);
    Vertice_PTR NOVO;
    Vertice_PTR ADJ2 = calloc(1, sizeof(Vertice));

    strcpy(ADJ2->cidade, B->cidade);
    strcpy(ADJ2->estado, B->estado);
    strcpy(ADJ2->tempo, B->tempo);
    ADJ2->distancia = B->distancia;

    if (!elem) {
        Vertice_PTR V1 = calloc(1, sizeof(Vertice));
        Vertice_PTR V2 = calloc(1, sizeof(Vertice));
        strcpy(V1->cidade, A->cidade);
        strcpy(V1->estado, A->estado);
        strcpy(V2->cidade, B->cidade);
        strcpy(V2->estado, B->estado);
        V1->adjacente = ADJ2;

        if(strcmp(A->cidade, B->cidade) < 0) {
            (*lista) = V1;
            V1->proxLista = V2;
        } else {
            (*lista) = V2;
            V2->proxLista = V1;
        }

        return;
    }

    bool vertice_na_lista;
    Vertice_PTR ant_elem;

    //Inserindo aresta A -> B
    vertice_na_lista = false;
    elem = (*lista);
    ant_elem = NULL;

    while (elem) {
        if (strcmp(elem->cidade, A->cidade) >= 0) {
            if (!strcmp(elem->cidade, A->cidade)) {
                vertice_na_lista = true;
            }
            break;
        }
        ant_elem = elem;
        elem = elem->proxLista;
    }

    //Inserindo vertice A
    if (!vertice_na_lista) {
        NOVO = calloc(1, sizeof(Vertice));
        strcpy(NOVO->cidade, A->cidade);
        strcpy(NOVO->estado, A->estado);
        NOVO->adjacente = ADJ2;

        if (!ant_elem) {
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
        while (elem) {
            if (strcmp(elem->cidade, B->cidade) > 0) {
                break;
            }
            ant_elem = elem;
            elem = elem->adjacente;
        }
        ant_elem->adjacente = ADJ2;
        ADJ2->adjacente = elem;
    }

    vertice_na_lista = false;
    elem = (*lista);
    ant_elem = NULL;

    while (elem) {
        if (strcmp(elem->cidade, B->cidade) >= 0) {
            if (!strcmp(elem->cidade, B->cidade)) {
                vertice_na_lista = true;
            }
            break;
        }
        ant_elem = elem;
        elem = elem->proxLista;
    }

    //Inserindo vertice B
    if (!vertice_na_lista) {
        NOVO = calloc(1, sizeof(Vertice));
        strcpy(NOVO->cidade, B->cidade);
        strcpy(NOVO->estado, B->estado);

        if (!ant_elem) {
            NOVO->proxLista = (*lista);
            (*lista) = NOVO;
            elem = NOVO;
        } else {
            NOVO->proxLista = elem;
            ant_elem->proxLista = NOVO;
        }
    }
}

void dijkstra(char* nome_file, char* campo, char* valor) {
    Vertice_PTR lista = lista_adj(nome_file);
    if (!lista) {
        return;
    }
    FILE* file_bin = fopen(nome_file, "rb");
    fseek(file_bin, 1 * sizeof(char), SEEK_SET);
    int numvertices;
    fread(&numvertices, sizeof(int), 1, file_bin);
    fclose(file_bin);

    Antecessor_PTR_PTR antec = calloc(numvertices-1, sizeof(Antecessor_PTR));
    for(int i = 0; i< numvertices-1; i++) {
        antec[i] = calloc(1,sizeof(Antecessor));
    }
    Vertice_PTR elem = lista;
    bool achou_vertice = false;
    Vertice_PTR origem;
    int i = 0;

    // procurando origem e setando dados no vetor de antecessor
    while(elem) {
        if(!strcmp(campo, "cidadeOrigem") && !strcmp(elem->cidade, valor)) {
            achou_vertice = true;
        } else {
            strcpy(antec[i]->cidadeDest, elem->cidade);
            strcpy(antec[i]->estadoDest, elem->estado);
            antec[i]->distancia = -1;
            i++;
        }
        elem = elem->proxLista;
    }

    if(!achou_vertice) {
        return;
    }

    // iniciando algoritmo pela origem
    elem = lista;
    while(elem) {
        if(!strcmp(elem->cidade, valor)) {
            break;
        }
        elem = elem->proxLista;
    }
    origem = elem;
    elem = elem->adjacente;
    while(elem) {
        for(int i = 0; i < numvertices-1; i++) {
            if(!strcmp(elem->cidade, antec[i]->cidadeDest)) {
                strcpy(antec[i]->cidadeAnt, origem->cidade);
                strcpy(antec[i]->estadoAnt, origem->estado);
                antec[i]->distancia = elem->distancia;
                antec[i]->analisado = false;
                break;
            }
        }
        elem = elem->adjacente;
    }

    bool continuar_analisando = false;

    if(numvertices > 1)
        continuar_analisando = true;
    // aqui todos adjacentes a origem estao iniciados

    int auxIndice, nova_dist;

    // nos a ser analisados numvertices - 1
    // reorganizar for
    while(continuar_analisando) {

        //pegando um elem pra ser analisado -> auxIndice
        for(int j = 0; j < numvertices-1; j++) {
            if(!antec[j]->analisado && antec[j]->distancia != -1) {
                auxIndice = j;
                break;
            }
        }

        //procurando elemento a ser analisado na lista encadeada
        elem = lista;
        while(elem) {
            if(!strcmp(elem->cidade, antec[auxIndice]->cidadeDest)) {
                break;
            }
            elem = elem->proxLista;
        }

        // analisando elem
        elem = elem->adjacente;
        while(elem) {
            for(int j = 0; j < numvertices-1; j++) {
                if(strcmp(elem->cidade, antec[j]->cidadeDest)) {
                    continue;
                }
                nova_dist = antec[auxIndice]->distancia + elem->distancia;

                // nova dist eh menor ou no que ngm chegou ainda
                if(nova_dist < antec[j]->distancia || antec[j]->distancia == -1) {
                    strcpy(antec[j]->cidadeAnt, antec[auxIndice]->cidadeDest);
                    strcpy(antec[j]->estadoAnt, antec[auxIndice]->estadoDest);
                    antec[j]->distancia = nova_dist;
                    antec[j]->analisado = false;
                }
                break;
            }

            elem = elem->adjacente;
            if(elem && !strcmp(elem->cidade, valor))
                elem = elem->adjacente;

        }
        antec[auxIndice]->analisado = true;

        continuar_analisando = false;
        for(int i = 0; i < numvertices-1; i++) {
            if(!antec[i]->analisado) {
                continuar_analisando = true;
                break;
            }
        }
    }

    for(int i = 0; i < numvertices-1; i++) {
        if(!strcmp(antec[i]->cidadeDest, origem->cidade))
            continue;
        printf("%s %s %s %s %d %s %s\n", origem->cidade, origem->estado, antec[i]->cidadeDest, antec[i]->estadoDest, antec[i]->distancia, antec[i]->cidadeAnt, antec[i]->estadoAnt);
    }

}

void prim(char* nome_file, char* campo, char* valor) {
    Vertice_PTR lista = lista_adj(nome_file);
    if (!lista) {
        return;
    }
    FILE* file_bin = fopen(nome_file, "rb");
    fseek(file_bin, 1 * sizeof(char), SEEK_SET);
    int numvertices;
    fread(&numvertices, sizeof(int), 1, file_bin);
    fclose(file_bin);

    VerticeList_PTR conjVertices = calloc(1, sizeof(VerticeList));
    ArestaList_PTR conjAuxArestas;
    Aresta_PTR aresta;
    Vertice_PTR orig;
    Vertice_PTR dest;
    Vertice_PTR elem;
    Vertice_PTR aux;
    bool cond, elementoConexoDistinto;
    Vertice_PTR lista_min = NULL;

    cond = false;
    int menor, menor_ind;
    elem = lista;

    while (elem) {
        cond = (!strcmp(campo, "cidadeOrigem") && !strcmp(elem->cidade, valor));
        if (cond) {
            conjVertices->listaVertices = realloc(conjVertices->listaVertices, ++(conjVertices->tam) * sizeof(Vertice));
            conjVertices->listaVertices[conjVertices->tam - 1] = elem;
            break;
        }
        elem = elem->proxLista;
    }

    if (!conjVertices->tam) {
        printf("Cidade inexistente.");
        return;
    }

    for (int i = 0; i < (numvertices - 1); i++) {
        menor = -1;
        cond = false;
        conjAuxArestas = calloc(1, sizeof(VerticeList));
        orig = calloc(1, sizeof(Vertice));
        aux = NULL;

        for (int j = 0; j < conjVertices->tam; j++) {
            elem = lista;

            while(elem) {
                if(!strcmp(elem->cidade, conjVertices->listaVertices[j]->cidade)) {
                    break;
                }
                elem = elem->proxLista;
            }

            elem = elem->adjacente;

            while (elem) {
                if (elem->distancia < menor || menor == -1) {

                    elementoConexoDistinto = true;
                    for (int k = 0; k < conjVertices->tam; k++) {
                        if (!strcmp(conjVertices->listaVertices[k]->cidade, elem->cidade)) {
                            elementoConexoDistinto = false;
                            break;
                        }
                    }

                    if (elementoConexoDistinto) {
                        aux = elem;
                        menor = elem->distancia;
                        strcpy(orig->cidade, conjVertices->listaVertices[j]->cidade);
                        strcpy(orig->estado, conjVertices->listaVertices[j]->estado);
                    }
                }
                elem = elem->adjacente;
            }
            if(!aux)
                continue;
            //listaAux de arestas
            dest = calloc(1, sizeof(Vertice));
            strcpy(dest->cidade, aux->cidade);
            strcpy(dest->estado, aux->estado);
            strcpy(dest->tempo, aux->tempo);
            dest->distancia = aux->distancia;
            aresta = calloc(1, sizeof(Aresta));
            aresta->orig = orig;
            aresta->dest = dest;
            conjAuxArestas->listaAresta = (Aresta_PTR_PTR) realloc(conjAuxArestas->listaAresta, ++(conjAuxArestas->tam) * sizeof(Aresta_PTR));
            conjAuxArestas->listaAresta[conjAuxArestas->tam - 1] = aresta;
        }
        //dentre a lista auxiliar inserir a menor
        menor = -1;
        for(int j = 0; j < conjAuxArestas->tam; j++) {
            if(conjAuxArestas->listaAresta[j]->dest->distancia < menor || menor == -1) {
                menor = conjAuxArestas->listaAresta[j]->dest->distancia;
                menor_ind = j;
            }
        }
        conjVertices->listaVertices = (Vertice_PTR_PTR) realloc(conjVertices->listaVertices, ++(conjVertices->tam) * sizeof(Vertice_PTR));
        conjVertices->listaVertices[conjVertices->tam - 1] = dest;

        orig = conjAuxArestas->listaAresta[menor_ind]->orig;
        dest = conjAuxArestas->listaAresta[menor_ind]->dest;

        inserir_lista_adj(&lista_min, orig, dest);

    }
    exibe_grafo(lista_min);
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

    while (auxProx) {
        fprintf(stdout, "%s %s", auxProx->cidade, auxProx->estado);
        auxAdj = auxProx->adjacente;
        while (auxAdj) {
            fprintf(stdout, " %s %s %d", auxAdj->cidade, auxAdj->estado, auxAdj->distancia);
            if (auxAdj->tempo != NULL && strlen(auxAdj->tempo) > 0)
                fprintf(stdout, " %s", auxAdj->tempo);
            auxAdj = auxAdj->adjacente;
        }
        fprintf(stdout, "\n");
        auxProx = auxProx->proxLista;
    }
}

void scan_quote_string(char *str) {
    char R;
    while((R = getchar()) != EOF && isspace(R)); // ignorar espaços, \r, \n...
    while((R = getchar()) != EOF && isspace(R)); // ignorar espa�os, \r, \n...
    if(R == 'N' || R == 'n') { // campo NULO
        getchar();
        getchar();
        getchar(); // ignorar o "ULO" de NULO.
        strcpy(str, ""); // copia string vazia
    } else if(R == '\"') {
        if(scanf("%[^\"]", str) != 1) { // ler até o fechamento das aspas
        if(scanf("%[^\"]", str) != 1) { // ler at� o fechamento das aspas
            strcpy(str, "");
        }
        getchar(); // ignorar aspas fechando
    } else if(R != EOF) { // vc tá tentando ler uma string que não tá entre aspas! Fazer leitura normal %s então...
    } else if(R != EOF) { // vc t� tentando ler uma string que n�o t� entre aspas! Fazer leitura normal %s ent�o...
        str[0] = R;
        scanf("%s", &str[1]);
    } else { // EOF
        strcpy(str, "");
    }
}

