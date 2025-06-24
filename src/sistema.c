/*
    Módulo de Sistema da Berenice.
*/

#include <stdlib.h>
#include <memory.h>
#include <string.h>

#include "../include/structs.h"
#include "../include/utilidade.h"
#include "../include/memoria.h"
#include "../include/sistema.h"
#include "../include/tempo.h"

/**
    Inicializa o nosso sistema - Isso vai instantiar os structs, pre-definir valores
    caso haja necessidade (como produtos, usuario admin, por default) e entre outros.
    Na hipotese de a alocação falhar, ele vai limpando conforme os erros ocorrem;
    ex: Sistema (Ok) -> Sistema.x (Ok) -> Sistema.y (falhou) -> Limpa os anteriores.

    @return `sistema*` - Ponteiro do nosso sistema.
*/
Sistema* sistema_iniciar() {
    Sistema* sistema = safe_malloc(
        "Sistema",
        sizeof(Sistema)
    );

    if (!sistema) return NULL;

    sistema->usuarios = safe_malloc(
        "Usuarios (@Sistema)",
        sizeof(Usuario)
    );

    if (!sistema->usuarios) {
        free(sistema);
        sistema = NULL;
        return NULL;
    }

    sistema->clientes = safe_malloc(
        "Clientes (@Sistema)",
        sizeof(Cliente)
    );

    if (!sistema->clientes) {
        free(sistema->usuarios);
        sistema->usuarios = NULL;

        free(sistema);
        sistema = NULL;

        return NULL;
    }

    sistema->produtos = safe_malloc(
        "Produtos (@Sistema)",
        sizeof(Produto)
    );

    if (!sistema->produtos) {
        free(sistema->clientes);
        sistema->clientes = NULL;

        free(sistema->usuarios);
        sistema->usuarios = NULL;

        free(sistema);
        sistema = NULL;

        return NULL;
    }

    sistema->pagamentos = safe_malloc(
        "Pagamentos (@Sistema)",
        sizeof(Pagamento)
    );

    if (!sistema->pagamentos) {
        free(sistema->produtos);
        sistema->produtos = NULL;

        free(sistema->clientes);
        sistema->clientes = NULL;

        free(sistema->usuarios);
        sistema->usuarios = NULL;

        free(sistema);
        sistema = NULL;

        return NULL;
    }

    sistema->categorias = safe_malloc(
        "Categorias (@Sistema)",
        sizeof(Categoria)
    );

    if (!sistema->categorias) {
        free(sistema->pagamentos);
        sistema->pagamentos = NULL;

        free(sistema->produtos);
        sistema->produtos = NULL;

        free(sistema->clientes);
        sistema->clientes = NULL;

        free(sistema->usuarios);
        sistema->usuarios = NULL;

        free(sistema);
        sistema = NULL;

        return NULL;
    }

    sistema->vendas = safe_malloc(
        "Vendas (@Sistema)",
        sizeof(Venda)
    );

    if (!sistema->vendas) {
        free(sistema->pagamentos);
        sistema->pagamentos = NULL;

        free(sistema->produtos);
        sistema->produtos = NULL;

        free(sistema->clientes);
        sistema->clientes = NULL;

        free(sistema->usuarios);
        sistema->usuarios = NULL;

        free(sistema->categorias);
        sistema->categorias = NULL;

        free(sistema);
        sistema = NULL;

        return NULL;
    }

    sistema->caixa = safe_malloc(
        "Caixa (@Sistema)",
        sizeof(Caixa)
    );

    if (!sistema->caixa) {
        free(sistema->pagamentos);
        sistema->pagamentos = NULL;

        free(sistema->produtos);
        sistema->produtos = NULL;

        free(sistema->clientes);
        sistema->clientes = NULL;

        free(sistema->usuarios);
        sistema->usuarios = NULL;

        free(sistema->categorias);
        sistema->categorias = NULL;

        free(sistema->vendas);
        sistema->vendas = NULL;

        free(sistema);
        sistema = NULL;

        return NULL;
    }

    sistema->fechamentos = safe_malloc(
        "Fechamentos (@Sistema)",
        sizeof(Fechamento)
    );


    if (!sistema->fechamentos) {
        free(sistema->caixa);
        sistema->caixa = NULL;

        free(sistema->pagamentos);
        sistema->pagamentos = NULL;

        free(sistema->produtos);
        sistema->produtos = NULL;

        free(sistema->clientes);
        sistema->clientes = NULL;

        free(sistema->usuarios);
        sistema->usuarios = NULL;

        free(sistema->categorias);
        sistema->categorias = NULL;

        free(sistema->vendas);
        sistema->vendas = NULL;

        free(sistema);
        sistema = NULL;

        return NULL;
    }

    // Iniciamos o caixa, atribuindo valores, mas o status dele permanece
    // como fechado, pois vamos abrir ele no menu de abertura do caixa manualmente.
    sistema->caixa->quantidades.abertura = 0;
    sistema->caixa->quantidades.faturamento = 0;
    sistema->caixa->quantidades.vendas = 0;
    sistema->caixa->quantidades.pago_dinheiro = 0;
    sistema->caixa->quantidades.pago_cartao = 0;
    sistema->caixa->quantidades.pago_misto_dinheiro = 0;
    sistema->caixa->quantidades.pago_misto_cartao = 0;
    sistema->caixa->status = STATUS_FECHADO;
    sistema->caixa->data = tempo_agora();

    // Quantidades representa o numero de X algo no sistema
    // Usamos para controle, codigos e varias outras coisas
    sistema->quantidades = (QuantidadeSistema) {
        .usuarios = 1,
        .clientes = 0,
        .produtos = 0,
        .categorias = 0,
        .vendas = 0,
        .pagamentos = 0,
        .fechamentos = 0,
    };

    // Por padrao, o usuario default é um admin.
    sistema->usuario_atual = safe_malloc(
        "Usuario (ADMIN)",
        sizeof(Usuario)
    );

    if (!sistema->usuario_atual) return NULL;

    sistema->usuario_atual->codigo = 0;
    sistema->usuario_atual->tipo = 1;
    strcpy_s(sistema->usuario_atual->login, sizeof(sistema->usuario_atual->login), "ADMIN");
    strcpy_s(sistema->usuario_atual->senha, sizeof(sistema->usuario_atual->senha), "admin123");

    sistema->usuarios[0] = *sistema->usuario_atual;
    sistema->venda_atual = NULL;

    output("SISTEMA INICIALIZADO!", 'l');

    return sistema;
}


/**
    Executa uma limpeza completa no sistema, destrói todos os dados alocados
    dinamicamente dentro do `Sistema`, incluindo o próprio ponteiro principal.
    Após isso, tudo será NULL.

    @param sistema - O struct do nosso sistema.
    @return `void`
*/
void sistema_limpar(Sistema** sistema) {
    // Nada pra fazer? Ou ponteiro pro sistema já é nulo? Então saímos.
    if (!sistema || !(*sistema)) return;

    // Limpamos as categorias, se existirem.
    if ((*sistema)->categorias) {
        free((*sistema)->categorias);
        (*sistema)->categorias = NULL; // Zera pra evitar dangling pointer
    }

    // Limpamos os pagamentos, se existirem.
    if ((*sistema)->pagamentos) {
        free((*sistema)->pagamentos);
        (*sistema)->pagamentos = NULL;
    }

    // Limpamos os produtos, se existirem.
    if ((*sistema)->produtos) {
        free((*sistema)->produtos);
        (*sistema)->produtos = NULL;
    }

    // Limpamos os clientes, se existirem.
    if ((*sistema)->clientes) {
        free((*sistema)->clientes);
        (*sistema)->clientes = NULL;
    }

    // Limpamos os usuários, se existirem.
    if ((*sistema)->usuarios) {
        free((*sistema)->usuarios);
        (*sistema)->usuarios = NULL;
    }

    // Limpamos as vendas, se existirem.
    if ((*sistema)->vendas) {
        free((*sistema)->vendas);
        (*sistema)->vendas = NULL;
    }

    // Limpamos o caixa, se existir.
    if ((*sistema)->caixa) {
        free((*sistema)->caixa);
        (*sistema)->caixa = NULL;
    }

    // Limpamos o usuário logado (usuario_atual), se existir.
    if ((*sistema)->usuario_atual) {
        free((*sistema)->usuario_atual);
        (*sistema)->usuario_atual = NULL;
    }

    // Finalmente, liberamos o próprio struct Sistema
    free(*sistema);
    *sistema = NULL;
}

/**
    Essa função roda por ultima. Ela executa tudo que deve ser feito antes de encerrar
    o sistema por completo (no caso, antes de retornar na função principal) - dito isso;
    qualquer código relacionado a fechamento/limpeza, deve ser incluido aqui.

    @param sistema - O nosso sistema que vai ser encerrado.
    @return `void`
*/
void sistema_encerrar(Sistema** sistema) {
    output("ENCERRANDO SISTEMA!", 'l');
    sistema_limpar(sistema);
}

