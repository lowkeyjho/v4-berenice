/*
    Módulo de Relatórios da Berenice.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/cores.h"
#include "../include/constants.h"
#include "../include/utilidade.h"

#include "../include/structs.h"
#include "../include/relatorios.h"
#include "../include/ordenadores.h"
#include "../include/memoria.h"
#include "../include/arquivos.h"
#include "../include/cliente.h"


/**
    Função de utilidade.
    @param lista - Lista de itens
    @param quantidade - Total de itens
    @param codigo - Codigo alvo
*/
int contem_codigo(int* lista, int total, int codigo) {
    for (int index = 0; index < total; index++) {
        if (lista[index] == codigo) return 1;
    }
    return 0;
}

/**
    Exibe o relatorio de Clientes do sistema.
    @param sistema - Sistema da Berenice.
*/
void relatorio_clientes(Sistema *sistema) {
    printf("\n" BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n");
    printf(CLIENTE_HEADER, "CÓDIGO:", "NOME:", "WHATSAPP:", "CPF:");

    for (int index = 0; index < sistema->quantidades.clientes; index++) {
        Cliente* cliente = &(sistema->clientes[index]);

        printf(
            CLIENTE_IDENTIFIER,
            cliente->codigo,
            cliente->nome.completo,
            cliente->whatsapp,
            cliente->cpf
        );
    }

    printf(BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n");
}

/**
    Exibe o relatorio de Clientes, porem, ordena por nome (ascendente)
    @param sistema - Sistema da Berenice.
*/
void relatorio_clientes_ordenado(Sistema *sistema) {
    printf("\n" BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n");
    printf(CLIENTE_HEADER, "CÓDIGO:", "NOME:", "WHATSAPP:", "CPF:");

    // Pega o total de clientes
    int total = sistema->quantidades.clientes;

    // Aloca um vetor para copiar os clientes do sistema
    Cliente* copia = safe_malloc(
        "Clientes (COPY)",
        total * sizeof(Cliente)
    );

    if (!copia) return; // Alocamento falhou!

    // Copia os clientes do vetor para o vetor de copia
    memcpy(
        copia,
        sistema->clientes,
        total * sizeof(Cliente)
    );

    // Ordena os clientes pelo nome
    qsort(
        copia,
        total,
        sizeof(Cliente),
        comparar_clientes_nome
    );

    for (int index = 0; index < total; index++) {
        Cliente* cliente = &(copia[index]);

        printf(
            CLIENTE_IDENTIFIER,
            cliente->codigo,
            cliente->nome.completo,
            cliente->whatsapp,
            cliente->cpf
        );
    }

    // Libera a memória da copia e atribui o ponteiro como null (dangling pointer)
    free(copia);
    copia = NULL;

    printf(BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n");
}

/**
    Exibe o relatorio de Clientes que compraram em um certo periodo.
    @param sistema - Sistema da Berenice.
*/
void relatorio_clientes_periodo(Sistema *sistema) {
    // Pede para inserir o periodo que queremos analizar
    char periodo[12];
    prompt_string(
        "Insira o Periodo de Venda (dd-mm-YYYY): ",
        periodo,
        sizeof(periodo)
    );

    FILE* arquivo = arquivo_abrir(
        ARQUIVO_VENDAS,
        "r"
    );

    if (!arquivo) return; // Falha ao abrir! Paramos aqui (se nao da erro kkkkk)

    char linha[128]; // Buffer para nossa linha
    int clientes[100]; // Vetor para os clientes que compraram no periodo
    int total = 0; // Total de clientes que compraram no periodo

    while (fgets(linha, sizeof(linha), arquivo)) {
        // Remove o linebreak caso exista
        linha[strcspn(linha, "\n")] = '\0';

        // `strtok()` Separa a linha por tokens, pra o nosso caso de
        // separar itens usando ';', isso fica mais simples.
        char* data = strtok(linha, ";");
        char* codigo_venda = strtok(NULL, ";");
        char* codigo_cliente = strtok(NULL, ";");

        if (!data || !codigo_venda || !codigo_cliente) {
            output("Erro ao ler linha - Algum campo faltou!", 'a');
            continue;
        }

        // Se a data é diferente da que o usuario digitou, para
        if (strcmp(data, periodo) != 0) continue;

        // Transforma o codigo do cliente em um int
        int codigo_cliente_int = atoi(codigo_cliente);

        // Cliente ainda não existe no vetor de Clientes do periodo
        if (!contem_codigo(clientes, total, codigo_cliente_int)) {
            clientes[total] = codigo_cliente_int; // Adiciona ao vetor
            total += 1; // Incrementa o total
        }
    }

    arquivo_fechar(arquivo); // Fecha o arquivo, ja usamos ele!
    arquivo = NULL; // Limpa o ponteiro para o arquivo

    printf("\n" BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n");
    printf(CLIENTE_HEADER, "CÓDIGO:", "NOME:", "WHATSAPP:", "CPF:");

    for (int index = 0; index < total; index++) {
        Cliente* cliente = cliente_pegar(sistema, clientes[index]);
        if (!cliente) continue; // Cliente não foi achado!

        printf(
            CLIENTE_IDENTIFIER,
            cliente->codigo,
            cliente->nome.completo,
            cliente->whatsapp,
            cliente->cpf
        );
    }

    printf(BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n");
}

/**
    Relatorio dos produtos que foram criados.
    @param sistema - O sistema da Berenice.
*/
void relatorio_produtos(Sistema *sistema) {
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
    Relatorio dos produtos ordenados por descrição.
    @param sistema - O sistema da Berenice.
*/
void relatorio_produtos_ordenado(Sistema *sistema) {
    int total = (sistema->quantidades.produtos);
    Produto* produtos = safe_malloc(
        "Produtos (COPIA)",
        total * sizeof(Produto)
    );

    if (!produtos) return; // Alocamento falhou! (F)

    // Copia de um vetor para o outro
    memcpy(
        produtos,
        sistema->produtos,
        total * sizeof(Produto)
    );

    // Ordena nosso vetor de produtos (que seria a copia do vetor do sistema)
    qsort(
        produtos,
        total,
        sizeof(Produto),
        comparar_produtos_descricao
    );

    printf("\n" BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n");
    printf(PRODUTO_HEADER, "CÓDIGO:", "DESCRIÇÃO:", "CATEGORIA:", "PREÇO (R$):", "ESTOQUE:");

    for (int index = 0; index < total; index++) {
        Produto* produto = &(produtos[index]);

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

    free(produtos); // Libera a memoria da nossa copia
    produtos = NULL; // Define ponteiro como `null` para evitar danling pointer

    printf(BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n\n");
}

/**
    Relatorio dos Produtos com Estoque zerado ou minimo ordenado pela descrição.
    @param sistema - O sistema da Berenice.
*/
void relatorio_produtos_estoques(Sistema *sistema) {
    printf("\n" BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n");
    printf(PRODUTO_HEADER, "CÓDIGO:", "DESCRIÇÃO:", "CATEGORIA:", "PREÇO (R$):", "ESTOQUE:");

    int total = (sistema->quantidades.produtos);
    Produto* produtos = safe_malloc(
        "Produtos (COPIA)",
        total * sizeof(Produto)
    );

    if (!produtos) return; // Falhou o alocamento de memoria!

    // Copia de um vetor para o outro
    memcpy(
        produtos,
        sistema->produtos,
        total * sizeof(Produto)
    );

    // Ordena nosso vetor de produtos (que seria a copia do vetor do sistema)
    qsort(
        produtos,
        total,
        sizeof(Produto),
        comparar_produtos_descricao
    );

    for (int index = 0; index < total; index++) {
        Produto* produto = &(sistema->produtos[index]);

        if (produto->estoque.total == 0) {
            printf(
                PRODUTO_SEM_ESTOQUE,
                produto->codigo,
                produto->descricao,
                produto->categoria,
                produto->preco_venda,
                "(SEM ESTOQUE!)"
            );

        } else if (produto->estoque.total <= produto->estoque.minimo) {
            printf(
                PRODUTO_SEM_ESTOQUE, // Mesmo que tenha, esse ID permite botar texto no estoque
                produto->codigo,
                produto->descricao,
                produto->categoria,
                produto->preco_venda,
                "(MÍNIMO ATINGIDO!)"
            );
        }
    }

    free(produtos); // Libera a memoria da nossa copia
    produtos = NULL; // Define ponteiro como `null` para evitar danling pointer

    printf(BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n\n");
}

/**
    Relatorio dos Produtos mais vendidos em um Periodo.
    @param sistema - O sistema da Berenice.
*/
void relatorio_produtos_vendidos(Sistema *sistema) {
    char periodo[12];
    prompt_string(
        "Insira o Periodo de Venda (dd-mm-YYYY): ",
        periodo,
        sizeof(periodo)
    );

    FILE* arquivo = arquivo_abrir(
        ARQUIVO_PRODUTOS,
        "r+"
    );

    if (!arquivo) return; // Falha ao abrir arquivo de Produtos!

    // Vetor de produtos mais vendidos.
    ProdutoMaisVendido produtos[100];
    int total = 0;

    char linha[256];

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        // Remove linebreak se ele existir
        linha[strcspn(linha, "\n")] = '\0';

        char* data = strtok(linha, ";");
        char* codigo = strtok(NULL, ";");
        char* vendas = strtok(NULL, ";");

        strtok(NULL, ";"); // Pula
        strtok(NULL, ";"); // Pula Denovo

        char* descricao = strtok(NULL, ";");
        char* categoria = strtok(NULL, ";");

        if (!data || !codigo || !vendas || !descricao || !categoria) {
            output("Falha ao ler Linha - Algum campo faltou!", 'a');
            continue;
        }

        // Datas diferentes, pulamos!
        if (strcmp(data, periodo) != 0) continue; // Datas diferentes, pulamos!

        ProdutoMaisVendido* produto = &(produtos[total]);
        produto->codigo = atoi(codigo);
        produto->vendas = atoi(vendas);
        strncpy(produto->descricao, descricao, sizeof(produto->descricao));
        strncpy(produto->categoria, categoria, sizeof(produto->categoria));

        total += 1; // Incrementa o total de produtos mais vendidos
    }

    arquivo_fechar(arquivo); // Ja percorremos o arquivo, podemos fechar ele

    if (total == 0) {
        output("Nenhum produto mais vendido encontrado!", 'a');
        return;
    }

    // Ordena os produtos mais vendidos
    qsort(
        produtos,
        total,
        sizeof(ProdutoMaisVendido),
        comparar_produtos_vendidos
    );

    printf("\n" BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n");
    printf(RELATORIO_VENDA_HEADER,"CÓDIGO:", "VENDAS:", "DESCRIÇÃO:", "CATEGORIA:");

    for (int index = 0; index < total; index++) {
        ProdutoMaisVendido* produto = &(produtos[index]); // Pega o endereço de memoria do Produto
        printf(
            RELATORIO_VENDA_IDENTIFIER,
            produto->codigo,
            produto->vendas,
            produto->descricao,
            produto->categoria
        );
    }

    printf(BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n\n");
}

/**
    Relatorio das Vendas.
    @param sistema - O sistema da Berenice.
*/
void relatorio_vendas(Sistema *sistema) {
    printf("\n" BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n");
    printf(VENDA_HEADER, "CÓDIGO:", "QTD. ITENS:", "PAGAMENTO:");

    for (int index = 0; index < sistema->quantidades.vendas; index++) {
        Venda* venda = &(sistema->vendas[index]);
        printf(
            VENDA_IDENTIFIER,
            venda->codigo,
            venda->quantidade_itens,
            (venda->status == PAGAMENTO_ABERTO ? "(ABERTO!)" : "(PAGO!)")
        );
    }

    printf(BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n\n");
}

/**
    Relatorio de Vendas realizadas em um certo periodo.
    @param sistema - O sistema da Berenice.
*/
void relatorio_vendas_periodo(Sistema *sistema) {
    char periodo[12];
    prompt_string(
        "Insira o Periodo de Venda (dd-mm-YYYY): ",
        periodo,
        sizeof(periodo)
    );

    FILE* arquivo = arquivo_abrir(
        ARQUIVO_VENDAS,
        "r+"
    );

    if (!arquivo) return; // Falha ao abrir arquivo!

    char linha[256];
    Venda vendas[100];
    int total = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        // Remove linebreak caso ele exista
        linha[strcspn(linha, "\n")] = '\0';

        char* data = strtok(linha, ";");
        char* codigo = strtok(NULL, ";");
        char* codigo_cliente = strtok(NULL, ";");
        char* status = strtok(NULL, ";");
        char* quantidade = strtok(NULL, ";");

        if (!data || !codigo || !codigo_cliente || !status || !quantidade) {
            output("Erro ao ler linha - Algum campo faltou!", 'a');
            continue;
        }

        if (strcmp(data, periodo) != 0) continue; // Periodos diferentes!

        Venda* venda = &(vendas[total]);
        venda->codigo = atoi(codigo);
        venda->codigo_cliente = atoi(codigo_cliente);
        venda->quantidade_itens = atoi(quantidade);
        venda->status = (strcmp(status, "ABERTO") == 0 ? PAGAMENTO_ABERTO : PAGAMENTO_PAGO);
        venda->itens = NULL;

        total += 1;
    }

    arquivo_fechar(arquivo); // Ja usamos! Podemos fechar.

    if (total == 0) {
        output("Nenhuma venda foi encontrada no periodo selecionado!", 'a');
        return;
    }

    printf("\n" BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n");
    printf(VENDA_HEADER, "CÓDIGO:", "QTD. ITENS:", "PAGAMENTO:");

    for (int index = 0; index < total; index++) {
        Venda* venda = &(vendas[index]);
        printf(
            VENDA_IDENTIFIER,
            venda->codigo,
            venda->quantidade_itens,
            (venda->status == PAGAMENTO_ABERTO ? "ABERTO" : "PAGO")
        );
    }

    printf(BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n\n");
}

/**
    Relatorio de Conslidados de Venda, Recebidos em Dinheiro e Cartão.
    @param sistema - O sistema da Berenice.
*/
void relatorio_consolidado_periodo(Sistema *sistema) {
    char periodo[12];
    prompt_string(
        "Insira o Periodo de Venda (dd-mm-YYYY): ",
        periodo,
        sizeof(periodo)
    );

    FILE* arquivo = arquivo_abrir(
        ARQUIVO_FECHAMENTOS,
        "r+"
    );

    if (!arquivo) return; // Falha ao abrir arquivo

    Fechamento fechamentos[100];
    char linha[100];
    int total = 0;

    while (fgets(linha, sizeof(linha), arquivo)) {
        // Remove o linebreak caso exista
        linha[strcspn(linha, "\n")] = '\0';

        char* data = strtok(linha, ";");
        char* valor_dinheiro = strtok(NULL, ";");
        char* valor_cartao = strtok(NULL, ";");
        char* valor_total = strtok(NULL, ";");

        if (!data || !valor_dinheiro || !valor_cartao || !valor_total) {
            output("Erro ao ler linha - Algum campo faltou!", 'a');
            continue;
        }

        // Datas diferentes!
        if (strcmp(data, periodo) != 0) continue;

        Fechamento* fechamento = &(fechamentos[total]);
        fechamento->data = data;
        fechamento->recebido_dinheiro = atof(valor_dinheiro);
        fechamento->recebido_cartao = atof(valor_cartao);
        fechamento->recebido_total = atof(valor_total);
        total += 1;
    }

    if (total == 0) {
        output("Nenhum fechamento encontrado!", 'a');
        return;
    }

    printf("\n" BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n");
    printf(RELATORIO_CONSOLIDADO_HEADER, "DATA:", "PAGO DINHEIRO (R$):", "PAGO CARTÃO (R$):", "PAGO TOTAL (R$):");

    for (int index = 0; index < total; index++) {
        Fechamento* fechamento = &(fechamentos[index]);
        printf(
            RELATORIO_CONSOLIDADO_IDENTIFIER,
            fechamento->data,
            fechamento->recebido_dinheiro,
            fechamento->recebido_cartao,
            fechamento->recebido_total
        );
    }

    printf(BRANCO "━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━━" RESET "\n\n");
}
