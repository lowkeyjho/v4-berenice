/*
    Módulo do Caixa da Berenice.
*/

#include <stdio.h>
#include <math.h>

#include "../include/structs.h"
#include "../include/utilidade.h"
#include "../include/caixa.h"
#include "../include/venda.h"
#include "../include/tempo.h"
#include "../include/arquivos.h"

/**
    Abre o caixa do nosso sistema. Uma vez aberto, ele deve ser fechado para abrir
    novamente. A abertura de caixa apenas pode ser realizada por algum usuario do
    tipo Admin (1).

    @param sistema - O struct do nosso sistema
    @return `void`
*/
void caixa_abrir(Sistema *sistema) {
    // Se o `tipo` do usuario atual for diferente de (1) Admin
    // devemos impedir a abertura.
    if (sistema->usuario_atual->tipo != 1) {
        output("Apenas ADMINS podem abrir o Caixa!", 'a');
        return;
    }

    // Não podemos abrir o caixa se ele já está aberto.
    if (sistema->caixa->status == STATUS_ABERTO) {
        output("Caixa já foi aberto!", 'a');
        return;
    }

    // Pede para inserir o valor de abertura do caixa.
    float valor_abertura = prompt_float("Insira o Valor de Abertura do Caixa (R$): ");

    // Valida para que seja maior que zero.
    while (valor_abertura <= 0) {
        output("Valor de Abertura deve ser maior que zero! Tente novamente.", 'a');
        valor_abertura = prompt_float("Insira o Valor de Abertura do Caixa (R$): ");
    }

    sistema->caixa->quantidades.abertura = valor_abertura;
    sistema->caixa->status = STATUS_ABERTO;
    output("CAIXA ABERTO!", 's');
}

/**
    Fecha o caixa do nosso sistema. Apenas (1) Admins podem fechar ele.
    Caso haja uma divergencia de valores no caixa, devemos ajustar.

    @param sistema - O struct do nosso sistema
    @return `void`
*/
void caixa_fechar(Sistema *sistema) {
    // Para fechar, primeiro devemos abrir o caixa
    if (sistema->caixa->status != STATUS_ABERTO) {
        output("Caixa não está aberto! Abra ele primeiro!", 'a');
        return;
    }

    float faturamento = 0.0f;

    // Calcula o faturamento com vendas fechadas
    for (int index = 0; index < sistema->quantidades.vendas; index++) {
        Venda* venda = &(sistema->vendas[index]);

        // Pulamos venda aberta
        if (venda_is_aberta(sistema, venda->codigo) == 0) continue;

        faturamento += venda_calcular_valor(venda);
    }

    // Calcula valores de diferentes tipos de pagamentos
    float valor_abertura = sistema->caixa->quantidades.abertura;
    float valor_pago_dinheiro = (
        sistema->caixa->quantidades.pago_dinheiro +
        sistema->caixa->quantidades.pago_misto_dinheiro
    );

    float valor_pago_cartao = (
        sistema->caixa->quantidades.pago_cartao +
        sistema->caixa->quantidades.pago_misto_cartao
    );

    // Valor de ajuste
    float ajuste = (faturamento - valor_abertura - valor_pago_dinheiro - valor_pago_cartao);
    sistema->caixa->ajuste = ajuste;

    // Se for 0, fechou tudo ok!
    if (ajuste == 0) {
        output("CAIXA FECHOU CORRETAMENTE!", 's');
        arquivo_salvar_fechamento(sistema);
        caixa_reset(sistema);
    } else if (ajuste > 0) {
        char mensagem[128];
        snprintf(mensagem, sizeof(mensagem), "CAIXA FECHOU COM SOBRA DE R$%.2f!", ajuste);
        output(mensagem, 'i');

        // Pede o valor de ajuste
        float valor_ajuste = prompt_float("Insira o Valor de Ajuste: ");
        float reconciliado = (ajuste - valor_ajuste); // Diminuimos, pois é positivo

        if (fabs(reconciliado) < 0.01f) {
            output("AJUSTE APLICADO!", 's');
            sistema->caixa->ajuste = 0.0f;
            arquivo_salvar_fechamento(sistema);
            caixa_reset(sistema);
        } else {
            char mensagem[128];
            snprintf(mensagem, sizeof(mensagem), "AJUSTE INCORRETO! AINDA FALTA R$%.2f", reconciliado);
            output(mensagem, 'e');
        }
    } else if (ajuste < 0) {
        char mensagem[128];
        snprintf(mensagem, sizeof(mensagem), "CAIXA FECHOU COM FALTA DE R$%.2f!", ajuste);
        output(mensagem, 'e');

        // Pede o valor de ajuste
        float valor_ajuste = prompt_float("Insira o Valor de Ajuste: ");
        float reconciliado = (ajuste + valor_ajuste); // Somamos, pois é negativo

        if (fabs(reconciliado) < 0.01f) {
            output("AJUSTE APLICADO!", 's');
            sistema->caixa->ajuste = 0.0f;
            arquivo_salvar_fechamento(sistema);
            caixa_reset(sistema);
        } else {
            char mensagem[128];
            snprintf(mensagem, sizeof(mensagem), "AJUSTE INCORRETO! AINDA FALTA R$%.2f", reconciliado);
            output(mensagem, 'e');
        }
    }
}

/**
    Retira dinheiro do Caixa.
    @param sistema - O struct do nosso sistema.
    @return `void`
*/
void caixa_retirar(Sistema* sistema) {
    // Apenas admins podem retirar dinheiro.
    if (sistema->usuario_atual->tipo != 1) {
        output("Apenas ADMINS podem retirar dinheiro!", 'a');
        return;
    }

    // Variavel de controle
    float total_dinheiro = 0.0f;

    // Soma todos os pagamentos em `DINHEIRO` ou `MISTO_DINHEIRO`
    for (int index = 0; index < sistema->quantidades.pagamentos; index++) {
        Pagamento* pagamento = &(sistema->pagamentos[index]);

        if (pagamento->tipo == TIPO_DINHEIRO || pagamento->tipo == TIPO_MISTO_DINHEIRO) {
            total_dinheiro += pagamento->valor; // Incrementa
        }
    }

    // Mostra total em dinheiro
    char msg[128];
    snprintf(msg, sizeof(msg), "TOTAL DISPONIVEL EM DINHEIRO: R$%.2f", total_dinheiro);
    output(msg, 'i');

    float valor = prompt_float("Digite o valor a ser Retirado: ");

    // Validações, para evitar erros ter segurança (se errar ferrou)
    if (valor <= 0) {
        output("Valor inválido para retirada! Tente novamente.", 'e');
        return;
    }

    if (valor >= total_dinheiro) {
        output("Você não pode retirar TODO o dinheiro do caixa! Tente novamente.", 'e');
        return;
    }

    if ((total_dinheiro - valor) < 50.0f) {
        output("A Retirada deixaria o caixa com menos de R$50 para troco! Tente novamente.", 'e');
        return;
    }

    output("RETIRADO COM SUCESSO!", 's');
}

/**
    Reseta o caixa.
    @param sistema - O struct do nosso sistema.
    @return `void`
*/
void caixa_reset(Sistema* sistema) {
    sistema->caixa->status = STATUS_FECHADO;
    sistema->caixa->data = tempo_agora();
    sistema->caixa->quantidades.abertura = 0;
    sistema->caixa->quantidades.faturamento = 0;
    sistema->caixa->quantidades.vendas = 0;
    sistema->caixa->quantidades.pago_dinheiro = 0;
    sistema->caixa->quantidades.pago_cartao = 0;
    sistema->caixa->quantidades.pago_misto_dinheiro = 0;
    sistema->caixa->quantidades.pago_misto_cartao = 0;
}
