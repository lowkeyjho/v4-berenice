/*
    Módulo de Ordenadores da Berenice
*/

#include <string.h>

#include "../include/structs.h"
#include "../include/ordenadores.h"

/**
    Compara o nome de dois clientes.

    @param a - Um cliente
    @param b - Outro cliente
    @return Comparação entre os nomes dos clientes.
*/
int comparar_clientes_nome(const void* a, const void* b) {
    // Fazemos um cast, por que `qsort()` recebe os argumentos como
    // ponteiros para void, e não para o tipo da struct.
    Cliente* v1 = (Cliente*)a;
    Cliente* v2 = (Cliente*)b;

    return strcmp(
        v1->nome.completo,
        v2->nome.completo
    );
}

/**
    Compara a descrição de dois produtos.

    @param a - Um produto
    @param b - Outro produto
    @return Comparação entre as descrições dos produtos.
*/
int comparar_produtos_descricao(const void* a, const void* b) {
    // Fazemos um cast, por que `qsort()` recebe os argumentos como
    // ponteiros para void, e não para o tipo da struct.
    Produto* v1 = (Produto*)a;
    Produto* v2 = (Produto*)b;

    return strcmp(
        v1->descricao,
        v2->descricao
    );
}

/**
    Compara produtos mais vendios.
    @param a - Um produto
    @param b - Outro produto
    @return Comparação entre os produtos mais vendidos.
*/
int comparar_produtos_vendidos(const void* a, const void* b) {
    // Fazemos um cast, por que `qsort()` recebe os argumentos como
    // ponteiros para void, e não para o tipo da struct.
    ProdutoMaisVendido* v1 = (ProdutoMaisVendido*)a;
    ProdutoMaisVendido* v2 = (ProdutoMaisVendido*)b;

    return (v2->vendas - v1->vendas);
}
