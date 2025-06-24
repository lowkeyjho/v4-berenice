/*
    Módulo de Pagamentos da Berenice.
*/

#include <stdio.h>

#include "../include/structs.h"
#include "../include/cores.h"
#include "../include/pagamento.h"
#include "../include/memoria.h"
#include "../include/menus.h"
#include "../include/venda.h"
#include "../include/utilidade.h"
#include "../include/constants.h"
#include "../include/tempo.h"

/**
    Inicializa um novo pagamento. Diferente de algumas funções similiares, ele não
    pede informações manuais, ja que o sistema em si (do nosso menu) faz o trabalho
    de atribuir o tipo de pagamento e valor automaticamente.

    @param valor - O valor que foi pago
    @param tipo - O tipo do pagamento (seguindo o struct `TipoPagamento`)
    @return `novo*` - O novo pagamento criado.
*/
Pagamento* pagamento_init(const int codigo_venda, const float valor, const TipoPagamento tipo) {
    // Alocamos um novo pagamento
    Pagamento* novo = safe_malloc(
        "Pagamento",
        sizeof(Pagamento)
    );

    if (!novo) return NULL; // Alocamento falhou!

    novo->data = tempo_agora();
    novo->codigo_venda = codigo_venda;
    novo->valor = valor;
    novo->tipo = tipo;

    return novo;
}

/**
    Retorna o tipo do Pagamento em formato de string. Isso nos ajuda no processo
    de salvar arquivos, identificação, e muito mais.

    @param pagamento - O pagamento que vamos analizar
    @return Uma string contendo o tipo do Pagamento
*/
char* pagamento_tipo(Pagamento* pagamento) {
    if (pagamento->tipo == TIPO_DINHEIRO) {
        return "Dinheiro";
    } else if (pagamento->tipo == TIPO_CARTAO) {
        return "Cartao";
    } else if (pagamento->tipo == TIPO_MISTO_DINHEIRO) {
        return "Misto-Dinheiro";
    } else if (pagamento->tipo == TIPO_MISTO_CARTAO) {
        return "Misto-Cartao";
    }

    return "DESCONHECIDO";
}

/**
    Inicia um pagamento de uma venda, checamos o objeto da venda diretamente
    para realizar o processo. Porem, temos checagens para situaçôes diferentes;

    - Se `venda` for nulo, assumimos que queremos listar as vendas em aberto;
    - Caso `venda` realmente exista, realizamos o pagamento dela.

    @param venda - A venda que vamos analizar
    @return `void`
*/
void pagamento_iniciar(Sistema* sistema, Venda* venda) {
    // Venda é null, vamos exibir as vendas em aberto!
    // Isso significa que entramos de forma direta na opção de pagar;
    // no qual pede para que percorremos a estrutura de pagamentos e exibimos
    // as vendas em aberto (para pagar elas)
    if (venda == NULL) {
        if (sistema->quantidades.vendas == 0) {
            output("Vendas insuficientes para pagar! Tente novamente.", 'a');
            return;
        }

        menu_exibir_vendas_abertas(sistema); // Exibe as vendas em aberto
        int codigo = prompt_int("Digite o Código da Venda: ");

        // Enquanto a venda que foi inserida estiver fechada, pedimos para
        // inserir uma venda que esta aberta (para pagar)
        while (!venda_is_aberta(sistema, codigo)) {
            output("Venda Fechada! Tente novamente.", 'a');
            codigo = prompt_int("Digite o Código da Venda: ");
        }

        // Procura pela venda, já que ela não foi passada como parametro
        Venda* venda = venda_pegar(sistema, codigo);
        menu_exibir_itens(sistema, venda);

        // Pega o valor da venda e calcula o desconto
        float valor_venda = venda_calcular_valor(venda);
        float valor_desconto = venda_calcular_desconto(
            venda,
            valor_venda
        );

        valor_venda -= valor_desconto; // Aplica de forma local (sem modificar original)
        printf(BRANCO ITALIC "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━❯ TOTAL: " VERDE "R$%.2f" RESET "\n", valor_venda);

        menu_exibir_pagamento_opcoes(sistema); // Exibe o menu de opções de pagamento
        int opcao_pagamento = prompt_int(PROMPT_SELECIONAR);

        while (opcao_pagamento <= 0 || opcao_pagamento > 3) {
            output(PROMPT_INVALIDO, 'a');
            opcao_pagamento = prompt_int(PROMPT_SELECIONAR);
        }

        switch (opcao_pagamento) {
            case 1:
                pagamento_cartao(sistema, venda, valor_venda, 0);
                break;
            case 2:
                pagamento_dinheiro(sistema, venda, valor_venda, 0);
                break;
            case 3:
                return; // Sair
        }
    } else {
        // Venda existe! Sem necessida de tentar achar ela.
        float valor_venda = venda_calcular_valor(venda);
        float valor_desconto = venda_calcular_desconto(
            venda,
            valor_venda
        ); // Calcula o desconto (sem aplicar totalmente ao valor final)

        valor_venda -= valor_desconto; // Aplica de forma local (sem modificar original)
        printf(BRANCO ITALIC "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━❯ TOTAL: " VERDE "R$%.2f" RESET "\n", valor_venda);

        menu_exibir_pagamento_opcoes(sistema); // Exibe as opções de pagamento
        int opcao_pagamento = prompt_int(PROMPT_SELECIONAR);

        while (opcao_pagamento <= 0 || opcao_pagamento > 3) {
            output(PROMPT_INVALIDO, 'a');
            opcao_pagamento = prompt_int(PROMPT_SELECIONAR);
        }

        switch (opcao_pagamento) {
            case 1:
                pagamento_cartao(sistema, venda, valor_venda, 0);
                break;
            case 2:
                pagamento_dinheiro(sistema, venda, valor_venda, 0);
                break;
            case 3:
                return; // Sair
        }
    }
}

/**
    Processa um pagamento em dinheiro. O parametro `valor_venda` se refere ao valor
    da venda, e o `valor_restante` se refere ao o que falta, referente a outra função
    que tenha chamado (por exemplo, 1 pagamento cartão -> restante dinheiro ...)

    @param sistema - O nosso sistema.
    @param venda - A venda que foi pagada.
    @param valor_venda - O valor da venda.
    @param valor_restante - O valor que falta para pagar.
    @return `void`
*/
void pagamento_dinheiro(Sistema* sistema, Venda* venda, float valor_venda, float valor_restante) {
    // EPA !! Estamos vindo da função de `pagamento_cartao` significa que esse
    // Pagamento vai ser considerado misto-dinheiro (cartao+dinheiro) e vamos
    // basear o valor total no `valor_restante`
    if (valor_restante > 0.0f) {
        // Pedimos para inserir o valor pago
        float valor_pago = prompt_float("Digite o valor Pago em Dinheiro (R$): ");

        // Checagem; valor pago deve ser maior que 0
        while (valor_pago <= 0) {
            output("Valor deve ser acima de (0) Zero! Tente novamente.", 'a');
            valor_pago = prompt_float("Digite o valor Pago em Dinheiro (R$): ");
        }

        // Como estamos vindo do pagamento em cartão, precisamos finalizar
        // o pagamento agora, então o valor pago deve ser igual ou maior que
        // `valor_restante` (que é o que falta para pagar)
        while (valor_pago < valor_restante) {
            output("Valor pago deve ser igual ou maior que o valor restante! Tente novamente.", 'a');
            valor_pago = prompt_float("Digite o valor Pago em Dinheiro (R$): ");
        }

        // Maior que o valor restante? HORA DO TROCO !!
        if (valor_pago > valor_restante) {
            char mensagem[64];
            snprintf(
                mensagem,
                sizeof(mensagem),
                "VALOR TROCO: R$%.2f",
                (float) (valor_pago - valor_restante)
            );
            output(mensagem, 's');
        }

        // Alocamos um novo pagamento e o atribuimos suas propriedades
        const int nova_quantidade = (sistema->quantidades.pagamentos + 1);
        Pagamento* pagamento = pagamento_init(
            venda->codigo,
            valor_venda,
            TIPO_MISTO_DINHEIRO // Parcial em dinheiro
        );

        if (!pagamento) return; // Falha ao alocar!

        sistema->pagamentos = safe_realloc(
            "Pagamentos (+)",
            sistema->pagamentos,
            nova_quantidade * sizeof(Pagamento)
        );

        if (!sistema->pagamentos) return; // Falha ao alocar!

        sistema->pagamentos[nova_quantidade - 1] = *pagamento;
        sistema->quantidades.pagamentos = nova_quantidade;
        sistema->caixa->quantidades.pago_misto_dinheiro += valor_venda;
        venda->status = PAGAMENTO_PAGO;
        venda_atualizar_produtos(sistema, venda); // Atualiza as vendas de cada produto

        output("PAGAMENTO CONCLUIDO!", 's');
        return;
    }

    // Pagamento inicia AGORA!
    // Antes, estavamos pagando caso o usuario tivesse pagando no Cartâo e decidisse
    // trocar para o dinheiro, mas, agora é certeza que o usuario escolheu dinheiro.
    float valor_pago = prompt_float("Digite o valor Pago no Dinheiro (R$): ");

    while (valor_pago <= 0) {
        output("Valor deve ser acima de (0) Zero! Tente novamente.", 'a');
        valor_pago = prompt_float("Digite o valor Pago no Dinheiro (R$): ");
    }

    // O valor pago é menor que o valor da venda, então temos
    // que pedir se o usuario quer pagar o resto no cartão.
    if (valor_pago < valor_venda) {
        float valor_restante = (valor_venda - valor_pago);

        char mensagem[128]; // Mensagem bonitinha pra imprimir kkkkkk
        snprintf(
            mensagem,
            sizeof(mensagem),
            "VALOR A SER PAGO NO CARTÃO: R$%.2f",
            valor_restante
        );
        output(mensagem, 'l');

        char opcao_pagar = prompt_char("Deseja Pagar o Restante no Cartão? [S/N]: ");

        // Valida até termos uma opção válida
        while (opcao_pagar != 'S' && opcao_pagar != 'N') {
            output("Opção inválida! Tente novamente.", 'a');
            opcao_pagar = prompt_char("Deseja Pagar o Restante no Cartão? [S/N]: ");
        }

        // Se for sim, pagamos o restante no cartão
        if (opcao_pagar == 'S')  {
            pagamento_cartao(sistema, venda, valor_venda, valor_restante);
            return; // Retornamos, pois vamos para aqui e prosseguir no cartão
        } else {
            // Vai ter que pagar de qualquer jeito, mesmo recusando kkkkkkk
            while (valor_pago < valor_venda) {
                output("Valor pago deve ser igual ou maior que o valor da venda! Tente novamente.", 'a');
                valor_pago = prompt_float("Digite o valor Pago no Dinheiro (R$): ");
            }

            // Sem return! Vamos ir para a parte de baixo (troco, etc...)
        }
    }

    // Aqui, o valor pago deve ser igual ou maior que o valor da venda.
    // Caso `valor_pago` for maior que o valor da venda
    // significa que precisamos devolver troco!
    if (valor_pago > valor_venda) {
        char mensagem[64];
        snprintf(
            mensagem,
            sizeof(mensagem),
            "VALOR TROCO: R$%.2f",
            (float) (valor_pago - valor_venda)
        );
        output(mensagem, 's');
    }

    const int nova_quantidade = (sistema->quantidades.pagamentos + 1);
    Pagamento* pagamento = pagamento_init(
        venda->codigo,
        valor_venda,
        TIPO_DINHEIRO
    );

    if (!pagamento) return; // Falha ao alocar!

    sistema->pagamentos = safe_realloc(
        "Pagamentos (+)",
        sistema->pagamentos,
        nova_quantidade * sizeof(Pagamento)
    );

    if (!sistema->pagamentos) return; // Falha ao alocar!

    sistema->pagamentos[nova_quantidade - 1] = *pagamento;
    sistema->quantidades.pagamentos = nova_quantidade;
    sistema->caixa->quantidades.pago_dinheiro += valor_venda;
    venda->status = PAGAMENTO_PAGO;
    venda_atualizar_produtos(sistema, venda); // Atualiza as vendas de cada produto

    output("PAGAMENTO CONCLUIDO!", 's');
}

/**
    Realiza um pagamento com cartão de crédito. O parametro `valor_venda` se refere
    ao valor da venda, e o `valor_restante` se refere ao o que falta, referente a
    outra função que tenha chamado (por exemplo, 1 pagamento cartão -> restante dinheiro ...)

    @param sistema - O nosso sistema.
    @param venda - A venda que foi pagada.
    @param valor_venda - O valor da venda.
    @param valor_restante - O valor que falta para pagar.
    @return `void`
*/
void pagamento_cartao(Sistema *sistema, Venda *venda, float valor_venda, float valor_restante) {
    // EPA !! Estamos vindo da função de `pagamento_dinheiro` significa que esse
    // Pagamento vai ser considerado misto-cartao (dinheiro+cartao) e vamos
    // basear o valor total no `valor_restante`
    if (valor_restante > 0) {
        // Pedimos para inserir o valor pago
        float valor_pago = prompt_float("Digite o valor Pago no Cartão (R$): ");

        // Checagem; valor pago deve ser maior que 0
        while (valor_pago <= 0) {
            output("Valor deve ser acima de (0) Zero! Tente novamente.", 'a');
            valor_pago = prompt_float("Digite o valor Pago no Cartão (R$): ");
        }

        // Como estamos vindo do pagamento em cartão, precisamos finalizar
        // o pagamento agora, então o valor pago deve ser igual ou maior que
        // `valor_restante` (que é o que falta para pagar)
        while (valor_pago < valor_restante) {
            output("Valor pago deve ser igual ou maior que o valor restante! Tente novamente.", 'a');
            valor_pago = prompt_float("Digite o valor Pago no Cartão (R$): ");
        }

        // Maior que o valor restante? HORA DO TROCO !!
        if (valor_pago > valor_restante) {
            char mensagem[64];
            snprintf(
                mensagem,
                sizeof(mensagem),
                "VALOR TROCO: R$%.2f",
                (float) (valor_pago - valor_restante)
            );
            output(mensagem, 's');
        }

        // Alocamos um novo pagamento e o atribuimos suas propriedades
        const int nova_quantidade = (sistema->quantidades.pagamentos + 1);
        Pagamento* pagamento = pagamento_init(
            venda->codigo,
            valor_venda,
            TIPO_MISTO_CARTAO // Parcial em cartão
        );

        if (!pagamento) return; // Falha ao alocar!

        sistema->pagamentos = safe_realloc(
            "Pagamentos (+)",
            sistema->pagamentos,
            nova_quantidade * sizeof(Pagamento)
        );

        if (!sistema->pagamentos) return; // Falha ao alocar!

        sistema->pagamentos[nova_quantidade - 1] = *pagamento;
        sistema->quantidades.pagamentos = nova_quantidade;
        sistema->caixa->quantidades.pago_misto_cartao += valor_venda;
        venda->status = PAGAMENTO_PAGO;
        venda_atualizar_produtos(sistema, venda); // Atualiza as vendas de cada produto

        output("PAGAMENTO CONCLUIDO!", 's');
        return;
    }

    // Aqui, o pagamento vai ser 100% apenas no cartão (não estamos vindo de outra forma de pagamento)
    // Repetimos alguma partes, diferenciando em certos fatores (como 0 para cancelar o pagamamento)
    float valor_pago = prompt_float("Digite o valor Pago no Cartão (R$): ");

    while (valor_pago <= 0) {
        output("Valor deve ser acima de (0) Zero! Tente novamente.", 'a');
        valor_pago = prompt_float("Digite o valor Pago no Cartão (R$): ");
    }

    // O valor pago é menor que o valor da venda, então temos
    // que pedir se o usuario quer pagar o resto no cartão.
    if (valor_pago < valor_venda) {
        float valor_restante = (valor_venda - valor_pago);

        char mensagem[128]; // Mensagem bonitinha pra imprimir kkkkkk
        snprintf(
            mensagem,
            sizeof(mensagem),
            "VALOR A SER PAGO NO DINHEIRO: R$%.2f",
            valor_restante
        );
        output(mensagem, 'l');

        char opcao_pagar = prompt_char("Deseja Pagar o Restante no Dinheiro? [S/N]: ");

        // Valida até termos uma opção válida
        while (opcao_pagar != 'S' && opcao_pagar != 'N') {
            output("Opção inválida! Tente novamente.", 'a');
            opcao_pagar = prompt_char("Deseja Pagar o Restante no Dinheiro? [S/N]: ");
        }

        // Se for sim, vamos pagar o resto no dinheiro
        if (opcao_pagar == 'S')  {
            pagamento_dinheiro(sistema, venda, valor_venda, valor_restante);
            return; // Retornamos, pois vamos para aqui e prosseguir no cartão
        } else {
            // Cliente fdp kkkkkkkkkkkkkk
            // Não quer pagar o resto no dinheiro, vai pagar de qualquer jeito
            while (valor_pago < valor_venda) {
                output("Valor pago deve ser igual ou maior que o valor da venda! Tente novamente.", 'a');
                valor_pago = prompt_float("Digite o valor Pago no Cartão (R$): ");
            }

            // Sem return! Vamos ir para a parte de baixo (troco, etc...)
        }
    }

    // Exibe as opções de status de pagamento (1) OK (2) NÃO OK, no cartão
    menu_exibir_pagamento_opcoes_cartao(sistema);
    int opcao_pagamento = prompt_int(PROMPT_SELECIONAR);

    // Valida nossa opção para não ter um valor inválido
    while (opcao_pagamento <= 0 || opcao_pagamento > 3) {
        output("Status Invalido! Tente novamente.", 'a');
        opcao_pagamento = prompt_int(PROMPT_SELECIONAR);
    }

    // Ai foi de estourar a boca do balão, pagamento não ok, exibimos outros metodos
    // Se for ok, fazemos nada (porque vamos continuar de qualquer maneira)
    // e se for (3) sair, apenas damos um return. (porque vamos sair do pagamento)
    if (opcao_pagamento == 2) {
        pagamento_iniciar(sistema, venda);
        return;
    } else if (opcao_pagamento == 3) {
        return;
    }

    // Aqui, o valor pago deve ser igual ou maior que o valor da venda.
    // Caso `valor_pago` for maior que o valor da venda
    // significa que precisamos devolver troco!
    if (valor_pago > valor_venda) {
        char mensagem[64];
        snprintf(
            mensagem,
            sizeof(mensagem),
            "VALOR TROCO: R$%.2f",
            (float) (valor_pago - valor_venda)
        );
        output(mensagem, 's');
    }

    const int nova_quantidade = (sistema->quantidades.pagamentos + 1);
    Pagamento* pagamento = pagamento_init(
        venda->codigo,
        valor_venda,
        TIPO_CARTAO
    );

    if (!pagamento) return; // Falha ao alocar!

    sistema->pagamentos = safe_realloc(
        "Pagamentos (+)",
        sistema->pagamentos,
        nova_quantidade * sizeof(Pagamento)
    );

    if (!sistema->pagamentos) return; // Falha ao alocar!

    sistema->pagamentos[nova_quantidade - 1] = *pagamento;
    sistema->quantidades.pagamentos = nova_quantidade;
    sistema->caixa->quantidades.pago_cartao += valor_venda;
    venda->status = PAGAMENTO_PAGO;
    venda_atualizar_produtos(sistema, venda); // Atualiza as vendas de cada produto

    output("PAGAMENTO CONCLUIDO!", 's');
}
