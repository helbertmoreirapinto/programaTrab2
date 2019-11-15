#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define LIST_ADJ 9
#define DIJKSTRA 10
#define PRIM     11

#define STATUS_OK '1'
#define REMOVIDO  '*'

#define TAM_CAB  19
#define TAM_REG  85
#define TAM_FIXO 8

typedef struct {
    int distancia;
    char UF_orig[2];
    char UF_dest[2];
    char cidade_orig[25];
    char cidade_dest[25];
    char tempo_viagem[25];
} Registro;
typedef Registro* Registro_PTR;

struct Vertice {
    char cidade[25];
    char UF[2];
    int distancia;
    struct Vertice** listaAdj;
    int tamListaAdj;
};
typedef struct Vertice   Vertice;
typedef struct Vertice*  Vertice_PTR;
typedef struct Vertice** Vertice_PTR_PTR;

typedef struct {
    Vertice_PTR_PTR listaVertice;
    int tamLista;
} ListaAdjacencias;
typedef ListaAdjacencias* ListaAdjacencias_PTR;

void lista_adjacencias(char*);
bool file_bin_integro(FILE*);
bool ler_registro(FILE*, Registro_PTR);

int main() {
    int funcao;
    char nome_file_bin[50];

    scanf("%d %s", &funcao, nome_file_bin);

    switch(funcao) {
    case LIST_ADJ:
        lista_adjacencias(nome_file_bin);
        break;
    case DIJKSTRA:
        break;
    case PRIM:
        break;
    default:
        printf("Erro funcao\n");
    }
    return 0;
}

void lista_adjacencias(char* nome_file_bin) {
    FILE* file_bin = fopen(nome_file_bin, "rb");
    if(!file_bin || !file_bin_integro(file_bin)) {
        printf("Falha na execução da funcionalidade.");
        return;
    }
    rewind(file_bin);
    fseek(file_bin, TAM_CAB, SEEK_SET);

    ListaAdjacencias_PTR lista = (ListaAdjacencias_PTR) calloc(1, sizeof(ListaAdjacencias));
    Registro_PTR reg;
    Vertice_PTR novo;
    bool ja_existe_na_lista;
    int tam, aux_cmp, inf, meio, sup, ind;

    while(true) {
        reg = (Registro_PTR) calloc(1,sizeof(Registro));
        if(!ler_registro(file_bin, reg)) {
            free(reg);
            break;
        } else if(reg->UF_orig[0] == REMOVIDO) {
            free(reg);
            continue;
        }

        ja_existe_na_lista = false;
        tam = lista->tamLista;
        inf = 0;
        sup = tam-1;
        ind = 0;

        //Adaptacao ao metodo de busca binaria
        while (inf <= sup) {
            meio = (int)(inf + sup)/2;
            aux_cmp = strcmp(reg->cidade_orig, lista->listaVertice[meio]->cidade);
            if(aux_cmp < 0) {
                sup = meio - 1;
                ind = meio;
            } else if(aux_cmp > 0) {
                inf = meio+1;
                ind = inf;
            } else {
                lista->listaVertice[meio]->tamListaAdj++;
                ja_existe_na_lista = true;
                break;
            }
        }

        //Inserindo no vetor na posicao ordenada
        if(!ja_existe_na_lista) {
            novo = (Vertice_PTR) malloc(sizeof(Vertice));
            strcpy(novo->cidade, reg->cidade_orig);
            novo->tamListaAdj = 1;
            lista->listaVertice = (Vertice_PTR_PTR) realloc(lista->listaVertice, (tam+1) * sizeof(Vertice_PTR));
            lista->tamLista++;
            for(int i = tam; i > ind; i--)      //Shiftando todos elementos das posicoes posteriores ao elemento
                lista->listaVertice[i] = lista->listaVertice[i-1];
            lista->listaVertice[ind] = novo;   //Inserindo o novo elemento no vetor
        }

        for(int i = 0; i < lista->tamLista; i++){
            printf("%s\n",lista->listaVertice[i]->cidade);
        }
        system("PAUSE");
        free(reg);
    }
}

bool file_bin_integro(FILE* file_bin) {
    rewind(file_bin);
    char status;
    fread(&status, sizeof(char), 1, file_bin);
    return (status == STATUS_OK);
}

bool ler_registro(FILE* file_bin, Registro_PTR reg) {
    bool aux = false;
    char tam_variavel[(TAM_REG - TAM_FIXO)+1];

    int tam_cidade_orig, tam_cidade_dest, tam_tempo;

    aux = fread(reg->UF_orig, 2 * sizeof(char), 1, file_bin);
    if(!aux || reg->UF_orig[0] == REMOVIDO)
        return false;
    fread(reg->UF_dest, 2 * sizeof(char), 1, file_bin);
    fread(&reg->distancia, sizeof(int), 1, file_bin);
    fread(tam_variavel, (TAM_REG - TAM_FIXO)* sizeof(char), 1, file_bin);

    tam_cidade_orig = strcspn(&tam_variavel[0], "|");
    strncpy(reg->cidade_orig, &tam_variavel[0], tam_cidade_orig);
    tam_cidade_dest = strcspn(&tam_variavel[tam_cidade_orig+1], "|");
    strncpy(reg->cidade_dest, &tam_variavel[tam_cidade_orig+1], tam_cidade_dest);
    tam_tempo = strcspn(&tam_variavel[tam_cidade_orig+tam_cidade_dest+2], "|");
    strncpy(reg->tempo_viagem, &tam_variavel[tam_cidade_orig+tam_cidade_dest+2], tam_tempo);

    return true;
}

