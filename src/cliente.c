/*
    Módulo de Clientes da Berenice.
*/

#include <stdio.h>

#include "../include/memoria.h"
#include "../include/utilidade.h"
#include "../include/structs.h"
#include "../include/cliente.h"

/**
    Inicializa um novo cliente, pedindo para que insira as informações relacionadas
    a ele manualmente (algumas precisam de checagem extra por conta de requesitos)

    @return `Cliente*`- Ponteiro do Cliente ou `NULL` caso falhe.
*/
Cliente* cliente_init() {
    // Alocamos um novo cliente
    Cliente* novo = safe_malloc(
        "Cliente",
        sizeof(Cliente)
    );

    if (!novo) return NULL; // Alocação falhou!

    novo->codigo = prompt_int("Insira o Código do Cliente: ");

    prompt_string(
        "Insira o Nome Completo do Cliente: ",
        novo->nome.completo,
        sizeof(novo->nome.completo)
    );

    prompt_string(
        "Insira o Nome Social do Cliente: ",
        novo->nome.social,
        sizeof(novo->nome.social)
    );

    prompt_string(
        "Insira a Rua do Cliente: ",
        novo->endereco.rua,
        sizeof(novo->endereco.rua)
    );

    novo->endereco.numero = prompt_int(
        "Insira o Numero da Rua do Cliente: "
    );

    prompt_string(
        "Insira o Bairro do Cliente: ",
        novo->endereco.bairro,
        sizeof(novo->endereco.bairro)
    );

    prompt_string(
        "Insira o WhatsApp do Cliente: ",
        novo->whatsapp,
        sizeof(novo->whatsapp)
    );

    prompt_string(
        "Insira o CPF do Cliente: ",
        novo->cpf,
        sizeof(novo->cpf)
    );

    return novo;

}

/**
    Tenta retornar um Cliente a partir do `codigo` passado.
    @param sistema - O nosso sistema.
    @param codigo - O código do cliente a ser buscado.
    @return Ponteiro para o Cliente, `NULL` caso não seja encontrado.
*/
Cliente* cliente_pegar(Sistema *sistema, int codigo) {
    for (int index = 0; index < sistema->quantidades.clientes; index++) {
        Cliente* cliente = &(sistema->clientes[index]);
        if (cliente->codigo == codigo) return cliente; // Achamos!
    }

    return NULL; // Nada encontrado! :(
}
