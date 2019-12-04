

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
    bool vertice_existe;
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

        vertice_existe = false;
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
                vertice_existe = true;
                break;
            }
        }

        //Inserindo no vetor na posicao ordenada
        if(!vertice_existe) {
            novo = (Vertice_PTR) malloc(sizeof(Vertice));
            strcpy(novo->cidade, reg->cidade_orig);
            novo->tamListaAdj = 1;
            lista->listaVertice = (Vertice_PTR_PTR) realloc(lista->listaVertice, (tam+1) * sizeof(Vertice_PTR));
            lista->tamLista++;
            for(int i = tam; i > ind; i--)      //Shiftando todos elementos das posicoes posteriores ao elemento
                lista->listaVertice[i] = lista->listaVertice[i-1];
            lista->listaVertice[ind] = novo;   //Inserindo o novo elemento no vetor
        }

        for(int i = 0; i < lista->tamLista; i++) {
            printf("%s\n",lista->listaVertice[i]->cidade);
        }
        system("PAUSE");
        free(reg);
    }
}

void dijkstra(char* nome_file_bin, char* vertice) {
}
