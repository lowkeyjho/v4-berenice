/*
    Módulo de Produtos da Berenice.
*/

#include <stdlib.h>

#include "../include/structs.h"
#include "../include/memoria.h"
#include "../include/produto.h"
#include "../include/utilidade.h"
#include "../include/tempo.h"

/**
    Inicializa um novo produto, pedindo para que insira as informações relacionadas
    a ele manualmente (algumas precisam de checagem extra por conta de requesitos)

    @return `Produto*` - O produto que foi criado.
*/
Produto* produto_init() {
    // Alocamos um novo Produto
    Produto* novo = safe_malloc(
        "Produto",
        sizeof(Produto)
    );

    if (!novo) return NULL; // Alocamento Falhou!

    novo->codigo = prompt_int("Insira o Código do Produto: ");

    // Registra direto no produto, apenas passamos o buffer
    prompt_string(
        "Insira a Descrição do Produto: ",
        novo->descricao,
        sizeof(novo->descricao)
    );

    prompt_string(
        "Insira a Categoria do Produto: ",
        novo->categoria,
        sizeof(novo->categoria)
    );

    novo->preco_compra = prompt_float("Insira o Preço de Compra do Produto: ");
    novo->preco_venda = prompt_float("Insira o Preço de Venda do Produto: ");
    novo->estoque.minimo = prompt_int("Insira o Estoque Mínimo do Produto: ");
    novo->estoque.total = prompt_int("Insira o Estoque Total do Produto: ");
    novo->margem_lucro = (novo->preco_venda - novo->preco_compra);
    novo->data = tempo_agora(); // Data de criação do produto

    return novo;
}

/**
    Percorre a estrutura de Produtos e retorna o produto baseado no seu codigo.
    Isso nos ajuda a captar informações rapidamente.

    @param sistema - O struct do nosso sistema.
    @param codigo - O codigo do produto a ser procurado.
*/
Produto* produto_pegar(Sistema* sistema, const int codigo) {
    for (int index = 0; index < sistema->quantidades.produtos; index++) {
        // Pegamos o struct do produto
        Produto* produto = &(sistema->produtos[index]);

        // Caso o codigo seja o que queremos, retornamos o produto!
        if (produto->codigo == codigo) return produto;
    }

    return NULL; // Nada achado!
}
