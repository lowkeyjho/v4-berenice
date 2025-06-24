/*
    Módulo de Usuarios da Berenice.
*/

#include <string.h>

#include "../include/memoria.h"
#include "../include/utilidade.h"
#include "../include/structs.h"

/**
    Inicializa um novo usuario, pedindo para que insira as informações relacionadas
    a ele manualmente (algumas precisam de checagem extra por conta de requesitos)

    @return `novo*` - Ponteiro do novo usuario ou `NULL` caso a alocação falhe.
*/
Usuario* usuario_init() {
    // Alocamos um novo usuario
    Usuario* novo = safe_malloc(
        "Usuario",
        sizeof(Usuario)
    );

    if (!novo) return NULL; // Alocação falhou!

    // Pede para inserir o codigo do cliente
    novo->codigo = prompt_int("Insira o Código do Usuário: ");

    // Agora o tipo (muda os privilegios)
    novo->tipo = prompt_int("Insira o Tipo do Usuario [1. ADMIN/2. USUARIO]: ");

    // Se o `tipo` do usuario for menor que um, ou maior que dois;
    // significa que o que foi inserido esta fora do intervalo de opções
    while (novo->tipo != 1 && novo->tipo != 2) {
        output("Tipo Invalido! Tente novamente.", 'a');
        novo->tipo = prompt_int("Insira o Tipo do Usuario [1. ADMIN/2. USUARIO]: ");
    }

    // Logica de inserção do Login do Usuario, incluindo validação
    // e checagem extra, para certificar que seja inserido corretamente.
    do {
        prompt_string(
            "Insira o Login do Usuário [Mínimo 8, Maxímo 12]: ",
            novo->login,
            sizeof(novo->login)
        );

        int tamanho_login = strlen(novo->login);

        if (tamanho_login < 8 || tamanho_login > 12) {
            output("Login Invalido! Tente novamente.", 'a');
            continue;
        }
    } while(strlen(novo->login) < 8 || strlen(novo->login) > 12);

    // Mesma coisa, mas se aplica para a senha do usuario.
    do {
        prompt_string(
            "Insira a Senha do Usuario [Minimo 6, Maximo 8]: ",
            novo->senha,
            sizeof(novo->senha)
        );

        int tamanho_senha = strlen(novo->senha);

        if (tamanho_senha < 6 || tamanho_senha > 8) {
            output("Senha Invalida! Tente novamente.", 'a');
            continue;
        }
    } while(strlen(novo->senha) < 6 || strlen(novo->senha) > 8);

    return novo;
}

/**
    Pega um usuario baseado em seu `login` (ou nome) caso ele não exista, retorna NULL.

    @param sistema - O struct do sistema, que contem todos os dados do sistema.
    @param nome - O login (nome) do usuario que deseja procurar.
    @return `usuario*` ou `NULL` caso não encontre.
*/
Usuario* usuario_pegar(Sistema* sistema, char nome[]) {
    // Percorre o vetor de usuarios, procurando pelo login
    for (int index = 0; index < sistema->quantidades.usuarios; index++) {
        Usuario* usuario = &(sistema->usuarios[index]);

        if (strcmp(usuario->login, nome) == 0) {
            return usuario;
        }
    }

    return NULL; // Nada foi encontrado!
}
