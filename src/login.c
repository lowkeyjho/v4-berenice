/*
    Módulo de Login da Berenice.
*/

#include <stdio.h>
#include <string.h>

#include "../include/menus.h"
#include "../include/utilidade.h"
#include "../include/login.h"
#include "../include/structs.h"
#include "../include/usuario.h"

/**
    Inicia o login. Isso vai pedir para inserir o login (nome), e senha.
    Caso a validação de login dê certo, o sistema vai re-definir a variavel `usuario_atual` para
    o novo login. Note que isso pode fazer com que certas funções fiquem indisponiveis.

    @param sistema - O struct do nosso sistema.
    @return `void`
*/
void login_iniciar(Sistema* sistema) {
    // Precisamos de pelo menos 2 usuarios cadastrados no sistema.
    if (sistema->quantidades.usuarios < 2) {
        output("Para fazer login, você precisa de pelo menos dois (2) Usuários Cadastrados!", 'a');
        return;
    }

    // Loop da lógica de login
    while (1) {
        menu_exibir_usuarios(sistema);

        char nome[13];
        prompt_string(
            "Digite o Nome do Usuário: ",
            nome,
            sizeof(nome)
        );

        Usuario* usuario = usuario_pegar(sistema, nome);

        // Usuario não encontrado! Repetimos usando `continue` ao invés de `break`
        if (!usuario) {
            output("Usuário não encontrado!", 'a');
            continue;
        }

        char senha[9];
        prompt_string(
            "Digite a Senha do Usuário: ",
            senha,
            sizeof(senha)
        );

        // Enquanto a senha for diferente da senha do usuário, repetimos!
        while (strcmp(usuario->senha, senha) != 0) {
            output("Senha incorreta! Tente novamente!", 'a');
            prompt_string(
            "Digite a Senha do Usuário: ",
            senha,
            sizeof(senha)
            );
        }

        // Aqui significa que a senha é a mesma! Então podemos definir o usuario atual.
        sistema->usuario_atual = usuario;

        // Mensagem de sucesso! (bem bonitinha hehe)
        char mensagem[128];
        snprintf(
            mensagem,
            sizeof(mensagem),
            "LOGADO COMO %s - SEJA BEM VINDO(a)!",
            usuario->login
        );

        output(mensagem, 's');
        break; // Terminamos o loop!
    }
}
