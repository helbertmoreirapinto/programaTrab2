/*****************************************|
|**  TRABALHO II - ESTUTURA DE DADOS 3  **|
|** Itegrantes:                NUSP:    **|
|** Felipe Caldana Ricioli     10716654 **|
|** Helbert Moreira Pinto      10716504 **|
|** Higor Tessari              10345251 **|
|*****************************************/

#include "headers/funcoes.h"

//-----------------------------------DEFINES------------------------------------//
#define LIST_ADJ    9
#define DIJKSTRA    10
#define PRIM        11
#define STATUS_OK   '1'

//------------------------------------MAIN--------------------------------------//
int main() {
    int funcao;
    char nome_file[50];
    char tipo_campo[50];
    char valor_campo[50];

    fscanf(stdin, "%d", &funcao);

    switch (funcao) {
    case LIST_ADJ:
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

    case PRIM:
        fscanf(stdin, "%s", nome_file);
        fscanf(stdin, "%s", tipo_campo);
        scan_quote_string(valor_campo);
        prim(nome_file, tipo_campo, valor_campo);
        break;

    default:
        printf("FUNCTION INVALID");

    }
    return 0;
}

//----------------------------------FUNCTIONS-----------------------------------//
bool file_integro(FILE* file_bin) {
    rewind(file_bin);                           //Poiter at the beginning of the file
    char status;
    fread(&status, sizeof(char), 1, file_bin);  //Read status
    return status == STATUS_OK;                 //Return file integrity
}


void exibe_grafo(Vertice_PTR elem) {
    Vertice_PTR auxAdj;
    Vertice_PTR auxProx = elem;

    while (auxProx) {                                                                           //If NULL, finished list
        fprintf(stdout, "%s %s", auxProx->cidade, auxProx->estado);                             //Display home city
        auxAdj = auxProx->adjacente;                                                            //Next adjacent
        while (auxAdj) {                                                                        //If NULL, finished adjacent list
            fprintf(stdout, " %s %s %d", auxAdj->cidade, auxAdj->estado, auxAdj->distancia);    //Display adjacent city
            if (auxAdj->tempo != NULL && strlen(auxAdj->tempo) > 0)
                fprintf(stdout, " %s", auxAdj->tempo);
            auxAdj = auxAdj->adjacente;                                                         //Next adjacent city
        }
        fprintf(stdout, "\n");
        auxProx = auxProx->proxLista;                                                           //Next home city
    }
}


void scan_quote_string(char *str) {
    char R;
    while((R = getchar()) != EOF && isspace(R));
    if(R == 'N' || R == 'n') {
        getchar();
        getchar();
        getchar();
        strcpy(str, "");
    } else if(R == '\"') {
        if(scanf("%[^\"]", str) != 1) {
            strcpy(str, "");
        }
        getchar();
    } else if(R != EOF) {
        str[0] = R;
        scanf("%s", &str[1]);
    } else {
        strcpy(str, "");
    }
}

