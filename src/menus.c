/*
    Módulo de Exibição de menus para a Berenice.
*/

#include <stdio.h>
#include <stdlib.h>

#include "../include/structs.h"
#include "../include/constants.h"
#include "../include/cores.h"
#include "../include/utilidade.h"

#include "../include/relatorios.h"
#include "../include/cadastros.h"
#include "../include/caixa.h"
#include "../include/menus.h"
#include "../include/venda.h"
#include "../include/produto.h"
#include "../include/pagamento.h"
#include "../include/login.h"

/**
    Formata a exibe um menu, a exibição é de forma estilizada, com linhas e cores
    para deixar tudo visualmente mais estético e diferenciado, evitando confusão.

    Note que, qualquer menu exibido, é de se esperar que ele tenha uma opção para;
    sair dele, por conta disso, reservamos `opcoes_quantidade` + 1 como o numero que;
    vai ser usado para exibir a opção de sair.

    @param titulo - O titulo que o menu vai exibir.
    @param opcoes - As opções que ficam dentro do menu. (apenas visual)
    @param opcoes_quantidade - Auto descritivo. Usado para limitar a exibição.
    @return `void`
*/
void menu_exibir(const char titulo[], const MenuOpcao menu_opcoes[], const int menu_opcoes_quantidade) {
    printf("\n" BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET VERDE ITALIC " %s " RESET BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n",titulo);

    for (int index = 0; index < menu_opcoes_quantidade; index++) {
        MenuOpcao opcao = menu_opcoes[index];
        printf(
            VERDE "%d. " RESET BRANCO "%s" RESET "\n",
            opcao.numero,
            opcao.mensagem
        );
    }

    printf(VERMELHO "%d. SAIR" RESET "\n", menu_opcoes_quantidade + 1);
    printf(BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n");
}

/**
    Exibe o menu de produtos no nosso sistema, isso vai formatar os produtos para
    deixar duma forma estetica e visualmente apelativa. Note que isso é apenas visual;
    a lógica de compra deve ser implementada separadamente.

    @param sistema - O struct do nosso sistena.
*/
void menu_exibir_produtos(Sistema* sistema) {
    printf("\n" BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n");
    printf(PRODUTO_HEADER, "CÓDIGO:", "DESCRIÇÃO:", "CATEGORIA:", "PREÇO (R$):", "ESTOQUE:");

    for (int index = 0; index < sistema->quantidades.produtos; index++) {
        Produto* produto = &(sistema->produtos[index]);

        if (produto->estoque.total > 0) {
            printf(
                PRODUTO_COM_ESTOQUE,
                produto->codigo,
                produto->descricao,
                produto->categoria,
                produto->preco_venda,
                produto->estoque.total
            );
        } else {
            printf(
                PRODUTO_SEM_ESTOQUE,
                produto->codigo,
                produto->descricao,
                produto->categoria,
                produto->preco_venda,
                "(SEM ESTOQUE!)"
            );
        }
    }

    printf(BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n\n");
}

/**
    Exibe o menu de itens da venda. Note que itens seriam o que foi adicionado a venda
    e não os produtos do sistema em si, significando que o que seja que for exibido esta
    dentro do escopo da venda providenciada.

    @param sistema - O struct do nosso sistema.
    @param venda - A venda no qual carrega os itens
*/
void menu_exibir_itens(Sistema *sistema, Venda *venda) {
    printf("\n" BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n");
    printf(ITEM_HEADER, "CÓDIGO:", "DESCRIÇÃO:", "PREÇO (R$):", "QNT:", "TOTAL (R$):");

    for (int index = 0; index < venda->quantidade_itens; index++) {
        Item* item = &(venda->itens[index]);
        Produto* produto = produto_pegar(sistema, item->codigo);

        printf(
            ITEM_IDENTIFIER,
            item->codigo,
            produto->descricao,
            produto->preco_venda,
            item->quantidade,
            item->subtotal
        );
    }

    printf(BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n\n");
}

/**
    Exibe o menu de Usuarios (dependendo do escopo, chamado de Logins) numa forma
    visualmente organizada e estética. Isso apenas exibe por detalhe, a lógica
    deve ser aplicada de forma separada.

    @param sistema - O struct do nosso sistema.
*/
void menu_exibir_usuarios(Sistema* sistema) {
    if (sistema->quantidades.usuarios == 0) {
        output("É necessario pelo menos 1 Usuário para exibir o Menu!", 'a');
        return;
    }

    printf("\n" BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n");
    printf(USUARIO_HEADER, "CÓDIGO:", "NOME:", "TIPO:");

    for (int index = 0; index < sistema->quantidades.usuarios; index++) {
        Usuario* usuario = &(sistema->usuarios[index]);
        char tipo[8];
        snprintf(
            tipo,
            sizeof(tipo),
            usuario->tipo == 1 ? "ADMIN" : "USUARIO"
        );

        printf(
            USUARIO_IDENTIFIER,
            usuario->codigo,
            usuario->login,
            tipo
        );
    }

    printf(BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n\n");
}

/**
    Exibe as vendas que estão em aberto.
    @param sistema - O struct do nosso sistema.
*/
void menu_exibir_vendas_abertas(Sistema* sistema) {
    if (sistema->quantidades.vendas == 0) {
        output("Não há vendas!", 'a');
        return;
    }

    printf("\n" BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n");
    printf(VENDA_HEADER, "CÓDIGO:", "QTD. ITENS:", "PAGAMENTO:");

    for (int index = 0; index < sistema->quantidades.vendas; index++) {
        Venda* venda = &(sistema->vendas[index]);
        if (venda_is_aberta(sistema, venda->codigo) == 0) continue;

        printf(
            VENDA_IDENTIFIER,
            venda->codigo,
            venda->quantidade_itens,
            "(ABERTO)"
        );
    }

    printf(BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n\n");
}

/**
    Exibe o menu de opções de pagamentos. Note que isso apenas é visual, e é feito
    para reduzir a quantidade de prints, a lógica de processamento deve ser exibido
    de forma separada.
*/
void menu_exibir_pagamento_opcoes(Sistema* sistema) {
    const MenuOpcao opcoes[] = {
        {1, "Pagamento no Cartão"},
        {2, "Pagamento no Dinheiro"},
    };

    menu_exibir(
        "PAGAMENTO",
        opcoes,
        2
    );
}

/**
    Exibe o menu de opções de status de pagamento caso seja cartão. Note que isso apenas é visua
    e é feito para reduzir a quantidade de prints, a lógica de processamento deve ser exibido
    de forma separada.
*/
void menu_exibir_pagamento_opcoes_cartao(Sistema* sistema) {
    const MenuOpcao opcoes[] = {
        {1, "PAGAMENTO OK!"},
        {2, "PAGAMENTO NÃO OK!"},
    };

    menu_exibir(
        "STATUS",
        opcoes,
        2
    );
}

/**
    Exibe o menu principal do programa, no qual vai ser a primeira coisa que
    o usuario vai ver quando entrar nele. A lógica de seleção do menu deve ser
    implementada de maneira separada.

    @param sistema - O struct do nosso sistema, lógica geral.
*/
void menu_exibir_principal(Sistema* sistema) {
    const MenuOpcao opcoes[] = {
        {1, "Cadastros"},
        {2, "Vendas"},
        {3, "Abertura de Caixa"},
        {4, "Fechamento de Caixa"},
        {5, "Relatórios"},
        {6, "Login"}
    };

    int opcao_atual = -1;

    do {
        // Exibe o menu e pede para o usuario digitar uma opção
        menu_exibir("BERENICE", opcoes, 6);
        opcao_atual = prompt_int(PROMPT_SELECIONAR);

        // Valida a opção para evitar de selecionar algo invalido
        while (opcao_atual <= 0 || opcao_atual > 7) {
            output(PROMPT_INVALIDO, 'a');
            opcao_atual = prompt_int(PROMPT_SELECIONAR);
        }

        // Checagem para conferir qual menu deve ser exibido baseado na escolha
        switch (opcao_atual) {
            case 1:
                menu_exibir_cadastros(sistema);
                break;
            case 2:
                menu_exibir_vendas(sistema);
                break;
            case 3:
                menu_exibir_abertura_caixa(sistema);
                break;
            case 4:
                menu_exibir_fechamento_caixa(sistema);
                break;
            case 5:
                menu_exibir_relatorios(sistema);
                break;
            case 6:
                menu_exibir_login(sistema);
                break;
            case 7:
                break;
            default:
                output("FATAL! Isso nunca deve rodar - bye!", 'e');
                exit(EXIT_FAILURE);
                break;
        }
    } while(opcao_atual != 7); // Repete até selecionar a opção de sair
}

/**
    Exibe o menu de cadastros, aonde é possivel cadastrar Clientes, Produtos;
    Usuarios e Categorias.

    @param sistema - O struct do nosso sistema, lógica geral.
*/
void menu_exibir_cadastros(Sistema* sistema) {
    const MenuOpcao opcoes[] = {
        {1, "Cadastro de Usuários"},
        {2, "Cadastro de Clientes"},
        {3, "Cadastro de Produtos"},
        {4, "Cadastro de Categorias"},
    };

    int opcao_atual = -1;

    do {
        // Exibe o menu e pede para o usuario digitar uma opção
        menu_exibir("CADASTROS", opcoes, 4);
        opcao_atual = prompt_int(PROMPT_SELECIONAR);

        // Valida a opção para evitar de selecionar algo invalido
        while (opcao_atual <= 0 || opcao_atual > 5) {
            output(PROMPT_INVALIDO, 'a');
            opcao_atual = prompt_int(PROMPT_SELECIONAR);
        }

        // Checagem para conferir qual menu deve ser exibido baseado na escolha
        switch (opcao_atual) {
            case 1:
                cadastrar_usuario(sistema);
                break;
            case 2:
                cadastrar_cliente(sistema);
                break;
            case 3:
                cadastrar_produto(sistema);
                break;
            case 4:
                cadastrar_categoria(sistema);
                break;
            case 5:
                break;
            default:
                output("FATAL! Isso nunca deve rodar - bye!", 'e');
                exit(EXIT_FAILURE);
                break;
        }
    } while(opcao_atual != 5); // Repete até selecionar a opção de sair
}

/**
    Exibe o menu de vendas, aonde o usuário pode tanto iniciar uma venda quanto
    realizar tarefas como retirada de caixa (apenas para Admins) e pagamentos.

    @param sistema - O struct do nosso sistema, lógica geral.
*/
void menu_exibir_vendas(Sistema* sistema) {
    const MenuOpcao opcoes[] = {
        {1, "Nova Venda"},
        {2, "Retirada (SANGRIA)"},
        {3, "Pagamento"},
    };

    int opcao_atual = -1;

    do {
        // Exibe o menu e pede para o usuario digitar uma opção
        menu_exibir("VENDAS", opcoes, 3);
        opcao_atual = prompt_int(PROMPT_SELECIONAR);

        // Valida a opção para evitar de selecionar algo invalido
        while (opcao_atual <= 0 || opcao_atual > 4) {
            output(PROMPT_INVALIDO, 'a');
            opcao_atual = prompt_int(PROMPT_SELECIONAR);
        }

        // Checagem para conferir qual menu deve ser exibido baseado na escolha
        switch (opcao_atual) {
            case 1:
                venda_iniciar(sistema);
                break;
            case 2:
                // TODO: Implementar a retirada de caixa
                caixa_retirar(sistema);
                break;
            case 3:
                pagamento_iniciar(sistema, NULL);
                break;
            case 4:
                break;
            default:
                output("FATAL! Isso nunca deve rodar - bye!", 'e');
                exit(EXIT_FAILURE);
                break;
        }
    } while(opcao_atual != 4); // Repete até selecionar a opção de sair
}

/**
    Exibe o menu de abertura do caixa, segundo pilar principal do sistema.
    Sem abrir o caixa, funções como cadastros, vendas (...) ficam indisponiveis.
    Note que, para realizar a abertura, o usuario atual deve ser um Admin (1).

    @param sistema - O struct do nosso sistema, lógica geral.
*/
void menu_exibir_abertura_caixa(Sistema* sistema) {
    const MenuOpcao opcoes[] = {
        {1, "Abrir o Caixa"}
    };

    int opcao_atual = -1;

    do {
        // Exibe o menu e pede para o usuario digitar uma opção
        menu_exibir("ABERTURA CAIXA", opcoes, 1);
        opcao_atual = prompt_int(PROMPT_SELECIONAR);

        // Valida a opção para evitar de selecionar algo invalido
        while (opcao_atual <= 0 || opcao_atual > 2) {
            output(PROMPT_INVALIDO, 'a');
            opcao_atual = prompt_int(PROMPT_SELECIONAR);
        }

        // Checagem para conferir qual menu deve ser exibido baseado na escolha
        switch (opcao_atual) {
            case 1:
                caixa_abrir(sistema);
                break;
            case 2:
                break;
            default:
                output("FATAL! Isso nunca deve rodar - bye!", 'e');
                exit(EXIT_FAILURE);
                break;
        }
    } while(opcao_atual != 2); // Repete até selecionar a opção de sair
}

/**
    Exibe o menu de fechamento do caixa. A partir do momento que fechamos o caixa;
    o sistema continua rodando, mas para usar ele novamente, deve-se realizar o;
    login. (para fechar ele, e, usar as mesmas opções, deve ser admin)

    @param sistema - O struct do nosso sistema, lógica geral.
*/
void menu_exibir_fechamento_caixa(Sistema* sistema) {
    const MenuOpcao opcoes[] = {
        {1, "Fechar o Caixa"}
    };

    int opcao_atual = -1;

    do {
        // Exibe o menu e pede para o usuario digitar uma opção
        menu_exibir("FECHAMENTO CAIXA", opcoes, 1);
        opcao_atual = prompt_int(PROMPT_SELECIONAR);

        // Valida a opção para evitar de selecionar algo invalido
        while (opcao_atual <= 0 || opcao_atual > 2) {
            output(PROMPT_INVALIDO, 'a');
            opcao_atual = prompt_int(PROMPT_SELECIONAR);
        }

        // Checagem para conferir qual menu deve ser exibido baseado na escolha
        switch (opcao_atual) {
            case 1:
                caixa_fechar(sistema);
                break;
            case 2:
                break;
            default:
                output("FATAL! Isso nunca deve rodar - bye!", 'e');
                exit(EXIT_FAILURE);
                break;
        }
    } while(opcao_atual != 2); // Repete até selecionar a opção de sair
}

/**
    Exibe o menu de Relatórios. Os relatórios são o que ocupam mais espaço no;
    quesito de exibição - espere uma boa quantidade de opções, algumas podem ser
    exibidas tanto no geral quanto em um certo periodo.

    @param sistema - O struct do nosso sistema, lógica geral.
*/
void menu_exibir_relatorios(Sistema* sistema) {
    const MenuOpcao opcoes[] = {
        {1, "Listar Clientes"},
        {2, "Litsar Clientes (ORDENADO)"},
        {3, "Listar Clientes Compras (PERIODO)"},
        {4, "Listar Produtos"},
        {5, "Listar Produtos (ORDENADO)"},
        {6, "Listar Produtos (MAIS VENDIDOS)"},
        {7, "Listar Vendas"},
        {8, "Listar Vendas (PERIODO)"},
        {9, "Listar Faturamento (PERIODO)"},
    };

    int opcao_atual = -1;

    do {
        // Exibe o menu e pede para o usuario digitar uma opção
        menu_exibir("RELATÓRIOS", opcoes, 9);
        opcao_atual = prompt_int(PROMPT_SELECIONAR);

        // Valida a opção para evitar de selecionar algo invalido
        while (opcao_atual <= 0 || opcao_atual > 10) {
            output(PROMPT_INVALIDO, 'a');
            opcao_atual = prompt_int(PROMPT_SELECIONAR);
        }

        // Checagem para conferir qual menu deve ser exibido baseado na escolha
        switch (opcao_atual) {
            case 1:
                relatorio_clientes(sistema);
                break;
            case 2:
                relatorio_clientes_ordenado(sistema);
                break;
            case 3:
                relatorio_clientes_periodo(sistema);
                break;
            case 4:
                relatorio_produtos(sistema);
                break;
            case 5:
                relatorio_produtos_ordenado(sistema);
                break;
            case 6:
                relatorio_produtos_estoques(sistema);
                break;
            case 7:
                relatorio_produtos_vendidos(sistema);
                break;
            case 8:
                relatorio_vendas(sistema);
                break;
            case 9:
                relatorio_vendas_periodo(sistema);
                break;
            case 10:
                break;
            default:
                output("FATAL! Isso nunca deve rodar - bye!", 'e');
                exit(EXIT_FAILURE);
                break;
        }
    } while(opcao_atual != 10); // Repete até selecionar a opção de sair

}

/**
    Exibe o menu de logins. Isso mostra os usuarios que existem

    @param sistema - O struct do nosso sistema, lógica geral.
*/
void menu_exibir_login(Sistema *sistema) {
    const MenuOpcao opcoes[] = {
        {1, "Realizar Login"},
        {2, "Ver Usuários"}
    };

    int opcao_atual = -1;

    do {
        // Exibe o menu e pede para o usuario digitar uma opção
        menu_exibir("LOGINS", opcoes, 2);
        opcao_atual = prompt_int(PROMPT_SELECIONAR);

        // Valida a opção para evitar de selecionar algo invalido
        while (opcao_atual <= 0 || opcao_atual > 3) {
            output(PROMPT_INVALIDO, 'a');
            opcao_atual = prompt_int(PROMPT_SELECIONAR);
        }

        // Checagem para conferir qual menu deve ser exibido baseado na escolha
        switch (opcao_atual) {
            case 1:
                login_iniciar(sistema);
                break;
            case 2:
                menu_exibir_usuarios(sistema);
                break;
            case 3:
                break;
            default:
                output("FATAL! Isso nunca deve rodar - bye!", 'e');
                exit(EXIT_FAILURE);
                return;
        }
    } while(opcao_atual != 3); // Repete até selecionar a opção de sair
}
