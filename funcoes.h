#ifndef  FUNCOES_H
#define  FUNCOES_H

#include <stdio.h>
#include <stdlib.h>

#define LIST_ADJ 9
#define DIJKSTRA 10
#define PRIM     11

#define STATUS_OK '1'
#define REMOVIDO  '*'

#define TAM_CAB  19
#define TAM_REG  85
#define TAM_FIXO 8

void lista_adjacencias(char*);
void dijkstra(char*, char*);
