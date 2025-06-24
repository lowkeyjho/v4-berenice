/*
    Cadastros da Berenice.
*/

#include "../include/structs.h"
#include "../include/utilidade.h"
#include "../include/memoria.h"
#include "../include/cadastros.h"
#include "../include/produto.h"
#include "../include/cliente.h"
#include "../include/usuario.h"


/**
    Cadastra um novo usuario por meio de um sistema de prompt no terminal.
    A partir do momento que a função for chamada, esse prompt vai aparecer e
    não pode ser interrompido.

    @param sistema - O struct do nosso sistema.
    @return `void`
*/
void cadastrar_usuario(Sistema *sistema) {
    // Pega a quantidade de usuarios e adiciona + 1;
    // Essa variavel vai ser usada para atribuir
    // ao nosso vetor de `Usuarios` (dentro de `sistema->usuarios`)
    const int nova_quantidade = (sistema->quantidades.usuarios + 1);

    // Tentamos re-alocar o vetor de Usuarios
    sistema->usuarios = safe_realloc(
        "Usuarios (+)",
        sistema->usuarios,
        nova_quantidade * sizeof(Usuario)
    );

    if (!sistema->usuarios) return; // Alocamento falhou!

    // Index em C inicia com 0, por conta disso, precisamos subtrair
    sistema->usuarios[nova_quantidade - 1] = *usuario_init();
    sistema->quantidades.usuarios = nova_quantidade;

    // Por fim, apenas avisamos que o cadastro ocorreu como esperado.
    output("USUÁRIO CADASTRADO!", 's');
}

/**
    Cadastra um novo cliente por meio de um sistema de prompt no terminal.
    A partir do momento que a função for chamada, esse prompt vai aparecer e
    não pode ser interrompido.

    @param sistema - O struct do nosso sistema.
    @return `void`
*/
void cadastrar_cliente(Sistema *sistema) {
    // Pega a quantidade de usuarios e adiciona + 1;
    // Essa variavel vai ser usada para atribuir
    // ao nosso vetor de `Clientes` (dentro de `sistema->clientes`)
    const int nova_quantidade = (sistema->quantidades.clientes + 1);

    // Tentamos re-alocar o vetor de Clientes
    sistema->clientes = safe_realloc(
        "Clientes (+)",
        sistema->clientes,
        nova_quantidade * sizeof(Cliente)
    );

    if (!sistema->clientes) return; // Alocamento falhou!

    // Index em C inicia com 0, por conta disso, precisamos subtrair
    sistema->clientes[nova_quantidade - 1] = *cliente_init();
    sistema->quantidades.clientes = nova_quantidade;

    // Por fim, apenas avisamos que o cadastro ocorreu como esperado.
    output("CLIENTE CADASTRADO!", 's');
}

/**
    Cadastra um novo produto por meio de um sistema de prompt no terminal.
    A partir do momento que a função for chamada, esse prompt vai aparecer e
    não pode ser interrompido.

    @param sistema - O struct do nosso sistema.
    @return `void`
*/
void cadastrar_produto(Sistema *sistema) {
    // Pega a quantidade de usuarios e adiciona + 1;
    // Essa variavel vai ser usada para atribuir
    // ao nosso vetor de `Produtos` (dentro de `sistema->produtos`)
    const int nova_quantidade = (sistema->quantidades.produtos + 1);

    // Tentamos re-alocar o vetor de Produtos
    sistema->produtos = safe_realloc(
        "Produtos (+)",
        sistema->produtos,
        nova_quantidade * sizeof(Produto)
    );

    if (!sistema->produtos) return; // Alocamento falhou!

    // Index em C inicia com 0, por conta disso, precisamos subtrair
    sistema->produtos[nova_quantidade - 1] = *produto_init();
    sistema->quantidades.produtos = nova_quantidade;

    // Por fim, apenas avisamos que o cadastro ocorreu como esperado.
    output("PRODUTO CADASTRADO!", 's');
}

/**
    Cadastra uma nova categoria por meio de um sistema de prompt no terminal.
    A partir do momento que a função for chamada, esse prompt vai aparecer e
    não pode ser interrompido.

    @param sistema - O struct do nosso sistema.
    @return `void`
*/
void cadastrar_categoria(Sistema *sistema) {

}
