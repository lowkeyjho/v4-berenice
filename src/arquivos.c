/*
    Módulo de Arquivos da Berenice.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>

#include "../include/constants.h"
#include "../include/structs.h"
#include "../include/arquivos.h"
#include "../include/utilidade.h"
#include "../include/pagamento.h"
#include "../include/fechamento.h"
#include "../include/memoria.h"

/**
    Abre um arquivo de forma segura para manipular. Caso haja algum erro, exibe uma mensagem
    detalhada e bonitinha.

    @param identificador - Identificador do arquivo que será aberto
    @param modo - Modo de abertura do arquivo.
    @return Um ponteiro para o arquivo aberto.
*/
FILE* arquivo_abrir(const char identificador[], const char* modo) {
    // Abre o arquivo usando o `fopen_s` - uma versão seguro do `fopen`.
    // A função retorna um erro, por isso criamos uma variavel do tipo `errno_t` para
    // armazenar o erro de abertura do arquivo.
    FILE* arquivo = NULL;
    errno_t erro = fopen_s(&arquivo, identificador, modo);

    if (erro != 0 || !arquivo) { // Blindagem extra com checagem dupla (medo de errar)
        char mensagem[128];
        snprintf(
            mensagem,
            sizeof(mensagem),
            "Falha ao abrir arquivo %s no modo %s! [Erro %d]",
            identificador,
            modo,
            erro
        );

        output(mensagem, 'e');
        exit(EXIT_FAILURE);
        return NULL;
    }

    return arquivo; // Retornamos o ponteiro
}

/**
    Fecha um arquivo aberto com segurança.
    Exibe erro caso o fechamento falhe.

    @param arquivo - Ponteiro para o arquivo que será fechado.
*/
void arquivo_fechar(FILE* arquivo) {
    if (!arquivo) {
        output("Tentativa de fechar um arquivo NULL!", 'e');
        return;
    }

    // Tenta fechar o arquivo e valida se deu certo
    if (fclose(arquivo) != 0) {
        output("Erro ao fechar o arquivo!", 'e');
        exit(EXIT_FAILURE);
    }
}

/**
    Escreve uma string formatada em um arquivo aberto.

    @param arquivo - Ponteiro para o arquivo onde os dados serão escritos.
    @param formato - String de formatação no estilo printf.
    @param ... - Argumentos variáveis para formatação.
*/
void arquivo_escrever(FILE* arquivo, const char* formato, ...) {
    if (!arquivo) {
        output("Erro ao escrever: arquivo ou formato nulo!", 'e');
        exit(EXIT_FAILURE);
        return;
    }

    // Aqui criamos uma lista de argumentos variaveis
    va_list args;
    va_start(args, formato);

    // Escreve no arquivo usando vfprintf (para argumentos variáveis)
    int resultado = vfprintf(
        arquivo,
        formato,
        args
    );

    va_end(args);

    if (resultado < 0) {
        output("Erro ao escrever no arquivo!", 'e');
        exit(EXIT_FAILURE);
        return;
    }
}

/**
    Salva os usuarios do Sistema em um arquivo de texto.
    @param sistema - O nosso sistema.
*/
void arquivo_salvar_usuarios(Sistema* sistema) {
    // Tenta abrir o arquivo de usuarios
    FILE* arquivo = arquivo_abrir(
        ARQUIVO_USUARIOS,
        ARQUIVO_USUARIOS_MODO
    );

    if (!arquivo) return; // Erro ao abrir :(

    // Itera por cada usuario do nosso sistema
    for (int index = 0; index<sistema->quantidades.usuarios; index++) {
        Usuario* usuario = &(sistema->usuarios[index]); // Pega o usuario
        arquivo_escrever(
            arquivo,
            ARQUIVO_USUARIO_ID,
            usuario->codigo,
            usuario->tipo,
            usuario->login,
            usuario->senha
        );
    }

    arquivo_fechar(arquivo);
}

/**
    Salva os Clientes do Sistema em um arquivo de texto.
    @param sistema - O nosso sistema.
*/
void arquivo_salvar_clientes(Sistema* sistema) {
    // Tenta abrir o arquivo de clientes
    FILE* arquivo = arquivo_abrir(
        ARQUIVO_CLIENTES,
        ARQUIVO_CLIENTES_MODO
    );

    if (!arquivo) return; // Erro ao abrir :(

    for (int index = 0; index < sistema->quantidades.clientes; index++) {
        Cliente* cliente = &(sistema->clientes[index]);
        char endereco[128];
        snprintf(endereco, sizeof(endereco), "%s %d, %s", cliente->endereco.rua, cliente->endereco.numero, cliente->endereco.bairro);

        arquivo_escrever(
            arquivo,
            ARQUIVO_CLIENTE_ID,
            cliente->codigo,
            cliente->nome.completo,
            cliente->nome.social,
            endereco,
            cliente->whatsapp,
            cliente->cpf
        );
    }

    arquivo_fechar(arquivo);
}

/**
    Salva os Produtos do nosso Sistema em um arquivo de texto.
    @param sistema - O struct do sistema.
*/
void arquivo_salvar_produtos(Sistema* sistema) {
    // Tenta abrir o arquivo de produtos
    FILE* arquivo = arquivo_abrir(
        ARQUIVO_PRODUTOS,
        ARQUIVO_PRODUTOS_MODO
    );

    if (!arquivo) return; // Erro ao abrir :(

    for (int index = 0; index < sistema->quantidades.produtos; index++) {
        Produto* produto = &(sistema->produtos[index]);

        arquivo_escrever(
            arquivo,
            ARQUIVO_PRODUTO_ID,
            produto->data,
            produto->codigo,
            produto->vendas,
            produto->estoque.total,
            produto->estoque.minimo,
            produto->descricao,
            produto->categoria,
            produto->preco_compra,
            produto->preco_venda,
            produto->margem_lucro
        );
    }

    arquivo_fechar(arquivo);
}

/**
    Salva as Vendas do nosso Sistema em um arquivo de texto.
    @param sistema - O struct do sistema.
*/
void arquivo_salvar_vendas(Sistema* sistema) {
    // Tenta abrir o arquivo de vendas
    FILE* arquivo = arquivo_abrir(
        ARQUIVO_VENDAS,
        ARQUIVO_VENDAS_MODO
    );

    if (!arquivo) return; // Erro ao abrir :(

    for (int index = 0; index < sistema->quantidades.vendas; index++) {
        Venda* venda = &(sistema->vendas[index]);

        arquivo_escrever(
            arquivo,
            ARQUIVO_VENDA_ID,
            venda->data,
            venda->codigo,
            venda->codigo_cliente,
            (venda->status == PAGAMENTO_ABERTO ? "ABERTO" : "PAGO"),
            venda->quantidade_itens
        );
    }

    arquivo_fechar(arquivo);
}

/**
    Salva os Pagamentos do nosso Sistema em um arquivo de texto.
    @param sistema - O struct do sistema.
*/
void arquivo_salvar_pagamentos(Sistema* sistema) {
    // Tenta abrir o arquivo de pagamentos
    FILE* arquivo = arquivo_abrir(
        ARQUIVO_PAGAMENTOS,
        ARQUIVO_PAGAMENTOS_MODO
    );

    if (!arquivo) return; // Erro ao abrir :(

    for (int index = 0; index < sistema->quantidades.pagamentos; index++) {
        Pagamento* pagamento = &(sistema->pagamentos[index]);

        arquivo_escrever(
            arquivo,
            ARQUIVO_PAGAMENTO_ID,
            pagamento->data,
            pagamento->codigo_venda,
            pagamento_tipo(pagamento),
            pagamento->valor
        );
    }

    arquivo_fechar(arquivo);
}

/**
    Salva um fechamento.
    @param sistema - O nosso sistema.
*/
void arquivo_salvar_fechamento(Sistema* sistema) {
    // Tenta abrir o arquivo de fechamento
    FILE* arquivo = arquivo_abrir(
        ARQUIVO_FECHAMENTOS,
        ARQUIVO_FECHAMENTOS_MODO
    );

    if (!arquivo) return; // Falha ao abrir arquivo :(

    Fechamento* fechamento = fechamento_init(sistema);

    if (!fechamento) return; // Fechamento deu falha!

    const int nova_quantidade = (sistema->quantidades.fechamentos + 1);
    sistema->fechamentos = safe_realloc(
        "Fechamentos (+)",
        sistema->fechamentos,
        nova_quantidade * sizeof(Fechamento)
    ); // Expande o vetor de fechamentos


    if (!sistema->fechamentos) return; // Expansão de fechamentos falhou! (porra kkkkkkkk)

    // Adicionamos nosso novo fechamento ao vetor
    sistema->fechamentos[nova_quantidade - 1] = *fechamento;
    sistema->quantidades.fechamentos = nova_quantidade;

    // Agora vai escrever no arquivo
    arquivo_escrever(
        arquivo,
        ARQUIVO_FECHAMENTO_ID,
        fechamento->data,
        fechamento->recebido_dinheiro,
        fechamento->recebido_cartao,
        fechamento->recebido_total
    );

    arquivo_fechar(arquivo); // Escrevemos, podemos fechar!
}

/**
    Salva todos os arquivos duma vez só.
    @param sistema - O nosso sistema.
*/
void arquivo_salvar_todos(Sistema* sistema) {
    arquivo_salvar_usuarios(sistema);
    arquivo_salvar_clientes(sistema);
    arquivo_salvar_produtos(sistema);
    arquivo_salvar_vendas(sistema);
    arquivo_salvar_pagamentos(sistema);
}
