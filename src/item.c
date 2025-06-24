/*
    Módulo de Itens da Berenice.
*/

#include <stdio.h>

#include "../include/memoria.h"
#include "../include/produto.h"
#include "../include/structs.h"

/**
    Inicializa um novo item. Isto é usado mais de forma interna, ao contrario de
    criar o vetor de itens, aqui apenas criamos um objeto `Item`.

    @param codigo - O codigo do produto que nosso item carrega
    @param quantidade - A quantidade que o item carrega
    @return `Item*` - O item criado
*/
Item* item_init(Sistema* sistema, const int codigo, const int quantidade) {
    // Alocamos um novo item
    Item* novo = safe_malloc(
        "Item",
        sizeof(Item)
    );

    if (!novo) return NULL; // Alocamento falhou!

    // Atribuimos os valores
    novo->codigo = codigo;
    novo->quantidade = quantidade;
    novo->subtotal = quantidade * produto_pegar(sistema, codigo)->preco_venda;
    //                            ^^^ pegamos o produto em si para acessar

    return novo;
}

/**
    Atualiza um item que fica dentro da venda. Isso nos ajuda a evitar alocar memoria
    sem necessidade - supunhetamos que alguem adicione um Produto agora, e depois de
    um tempo, ela adicione o mesmo produto; se não atualizarmos o produto já existente
    nós vamos estar ocupando memoria sem necessidade.

    @param sistema - O struct do nosso sistema
    @param venda - A venda no qual vamos usar
    @param codigo - O codigo do item que vamos atualizar
    @param quantidade - A quantidade que vai ser somada com a atual do item
    @return `void`
*/
void item_update(Sistema* sistema, Venda* venda, const int codigo, const int quantidade) {
    for (int index = 0; index < venda->quantidade_itens; index++) {
        Item* item = &(venda->itens[index]);

        if (item->codigo == codigo) {
            item->quantidade += quantidade;
            item->subtotal = (item->quantidade * produto_pegar(sistema, codigo)->preco_venda);
        }
    }
}

/**
    Retorna (1) se o item com o `codigo` existe dentro da venda, caso contrario
    retorna (0) - isso nos ajuda a conferir se há alguma necessidade de atualizar
    valores, essa funcão é usada apenas para checagem.

    @param venda - O struct da venda que vamos confeir
    @param codigo - O codigo do item alvo
    @return `int` - (1) se existir, (0) caso contrário.
*/
int item_exists(Venda* venda, const int codigo) {
    for (int index = 0; index < venda->quantidade_itens; index++) {
        Item* item = &(venda->itens[index]);
        if (item->codigo == codigo) return 1; // Encontrado!
    }

    return 0; // Não achamos. :(
}

/**
    Retorna um item baseado no seu codigo. Geralmente, usamos isso para atualizar
    valores, evitar repetir certas iterações e em geral, otimizar.

    @param venda - A venda no qual vamos procurar pelo item
    @param codigo - O codigo do item que vamos checar
    @return `Item*` - O item encontrado (caso exista) ou `NULL` caso contrário.
*/
Item* item_pegar(Venda* venda, const int codigo) {
    // Percorre por todos os itens
    for (int index = 0; index < venda->quantidade_itens; index++) {
        Item* item = &(venda->itens[index]);
        if (item->codigo == codigo) return item; // Achamos!
    };

    return NULL; // Nada foi achado!
}
