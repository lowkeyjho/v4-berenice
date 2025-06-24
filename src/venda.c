/*
    Módulo de Vendas da Berenice.
*/

#include <stdio.h>

#include "../include/structs.h"
#include "../include/memoria.h"
#include "../include/utilidade.h"

#include "../include/produto.h"
#include "../include/item.h"
#include "../include/menus.h"
#include "../include/venda.h"
#include "../include/pagamento.h"
#include "../include/cliente.h"
#include "../include/tempo.h"

/**
    Inicializa um struct de venda nova. Note que isso vai apenas CRIAR o struct;
    mas para iniciar a venda de fato, deve-se usar a função `venda_iniciar()`

    @param sistema - O struct do nosso sistema.
    @return `nova*` - O ponteiro da nova venda.
*/
Venda* venda_init(Sistema* sistema) {
    // Alocamos uma nova venda na memoria
    Venda* nova = safe_malloc(
        "Venda",
        sizeof(Venda)
    );

    if (!nova) return NULL; // Falhou logo de cara, retornamos `NULL`

    // Alocamos os itens da venda
    nova->itens = safe_malloc(
        "Itens (@Venda)",
        sizeof(Item)
    );

    if (!nova->itens) return NULL; // Erro novamente? Paramos!

    nova->data = tempo_agora();
    nova->quantidade_itens = 0;
    nova->status = PAGAMENTO_ABERTO;

    return nova;
}

/**
    Atualiza as vendas de um produto, percorrendo a estrutura de vendas.
    Quando realizarmos o pagamento de uma venda, precisamos atualizar a quantidade
    de vendas de TODOS os itens (que carregam o codigo de um produto) - se a gente
    fosse atualizar no momento que adicionamos ao carrinho, daria muita confusão.

    @param sistema - O nosso sistema.
    @param venda - A venda que foi paga e vamos percorrer.
    @return `void`
*/
void venda_atualizar_produtos(Sistema* sistema, Venda* venda) {
    for (int index = 0; index < venda->quantidade_itens; index++) {
        Item* item = &(venda->itens[index]);
        Produto* produto = produto_pegar(sistema, item->codigo);

        if (!produto) continue; // Produto não encontrado!

        produto->vendas += (item->quantidade);
    }
}

/**
    Retorna (1) se o a venda com o codigo providenciado estiver aberta;
    (0) se não estiver, usamos para evitar pegar vendas erradas ou fechadas.

    @param sistema - O nosso sistema.
    @param codigo - O codigo da venda que queremos verificar.
    @return `int` - 1 se a venda estiver aberta, 0 se nao.
*/
int venda_is_aberta(Sistema* sistema, const int codigo) {
    Venda* venda = venda_pegar(
        sistema,
        codigo
    );

    if (!venda) {
        output("Venda não encontada! Tente novamente.", 'a');
        return 0; // Nada encontrado!
    }

    return (venda->status == PAGAMENTO_ABERTO ? 1 : 0);
}

/**
    Retorna a venda que possui o `codigo` informado. Caso não exista, retorna
    `NULL` (usado para evitar acessar informações de uma venda que não existe).

    @param sistema - O nosso sistema.
    @param codigo - O codigo da venda que queremos buscar.
    @return `Venda*` - A venda encontrada, ou `NULL` se não existir.
*/
Venda* venda_pegar(Sistema* sistema, const int codigo) {
    for (int index = 0; index < sistema->quantidades.vendas; index++) {
        Venda* venda = &(sistema->vendas[index]);
        if (venda->codigo == codigo) return venda; // Achamos!
    }

    return NULL; // Nada encontrado!
}

/**
    Calcula o valor total da nossa venda. Usamos uma função para isso ao invés de
    atribuir uma propriedade tipo 'total' por alguns motivos;

    1. Se concedermos o desconto e sairmos, o valor passa a ser (valor - desconto)
    2. O usuario pode adicionar mais itens a venda, e o valor total deve ser recalculado
    3. O usuario pode cancelar a venda, fazendo com que o mesmo citado no (1) aconteça.

    @param venda - A venda que queremos calcular o valor.
    @return O valor total da venda.
*/
float venda_calcular_valor(Venda* venda) {
    if (venda->quantidade_itens == 0) {
        output("Itens insuficientes para calcular o valor da Venda!", 'a');
        return 0;
    }

    float total = 0;

    for (int index = 0; index < venda->quantidade_itens; index++) {
        Item* item = &(venda->itens[index]);
        total += item->subtotal;
    }

    return total;
}

/**
    Calcula o desconto da nossa venda, fazemos isso pedindo a % de desconto
    manualmente, com isso, subtraimos por meio de um calculo (sob o `valor_total`)

    @param venda - A venda que queremos calcular o desconto.
    @param valor_tota - O valor totla da nossa venda
    @return O valor de desconto.
*/
float venda_calcular_desconto(Venda *venda, float valor_total) {
    int valor_desconto = prompt_int("Informe % de Desconto (0 para nenhum): ");

    while (valor_desconto < 0 || valor_desconto >= 100) {
        output("Valor de Desconto Invalido! Tente novamente.", 'a');
        valor_desconto = prompt_int("Informe % de Desconto (0 para nenhum): ");
    }

    return (
        valor_desconto == 0 ? 0 : // Desconto = 0 (mesmo valor)
        (valor_total * valor_desconto) / 100 // Desconto > 0 (calcular)
    );
}

/**
    Inicia uma venda nova, incluind o processo de adicionar itens, estoque, etc...
    Uma vez chamada, ela vai seguir a logica de adicionar, ate que a opcao de "item
    novo?" seja N. (não)

    @return `void`
*/
void venda_iniciar(Sistema* sistema) {
    // O caixa deve estar aberto antes de iniciar qualquer venda
    // Isso é necessario, pois vamos acumular valores no final
    // e isso requere que o caixa esteja aberto.
    if (sistema->caixa->status == STATUS_FECHADO) {
        output("Abra o Caixa primeiro, antes de iniciar uma Venda!", 'a');
        return;
    }

    // Para evitar erros, checamos se no nosso sistema tem pelos menos 1 produto;
    // isso nos ajuda a poupar tempo e alguns bugs (loop infinito, ...)
    if (sistema->quantidades.produtos <= 0) {
        output("Registre pelo menos 1 Produto antes de iniciar uma venda!", 'a');
        return;
    }

    // Como cada venda precisa ter um cliente (que executa ela), precisamos
    // saber qual que é, para armazenar na venda. Porem, se tem nada, ai complica
    // kkkkkkkk
    if (sistema->quantidades.clientes <= 0) {
        output("Registre pelo menos 1 Cliente antes de iniciar uma venda!", 'a');
        return;
    }

    // Cria uma venda nova, alias, o codigo da venda sempre vai ser a quantidade
    // de venda somado com (1), então atualizamos a quantidade da mesma maneira.
    Venda* venda = venda_init(sistema);

    if (!venda) return; // Falha ao alocar a nossa venda!

    int codigo_cliente = prompt_int("Informe o Código do Cliente (Realizando a Venda): ");

    // Codigo deve ser maior que zero
    while (codigo_cliente <= 0) {
        output("Código de Cliente Inválido! Tente novamente.", 'a');
        codigo_cliente = prompt_int("Informe o Código do Cliente (Realizando a Venda): ");
    }

    Cliente* cliente = cliente_pegar(sistema, codigo_cliente);

    // Enquanto não encontrar o cliente, pedimos o código novamente
    // e buscamos até achar - aqui precisamos ter a certeza dele existir.
    while (!cliente) {
        output("Cliente não encontrado! Tente novamente.", 'a');
        codigo_cliente = prompt_int("Informe o Código do Cliente (Realizando a Venda): ");

        // Codigo deve ser maior que zero
        while (codigo_cliente <= 0) {
            output("Código de Cliente Inválido! Tente novamente.", 'a');
            codigo_cliente = prompt_int("Informe o Código do Cliente (Realizando a Venda): ");
        }

        cliente = cliente_pegar(sistema, codigo_cliente);
    }

    const int nova_quantidade = (sistema->quantidades.vendas + 1);
    sistema->vendas = safe_realloc(
        "Vendas (+)",
        sistema->vendas,
        nova_quantidade * sizeof(Venda)
    );

    if (!sistema->vendas) return; // Falha ao re-alocar vendas!

    // Aqui significa que o cliente existe (e a venda tambem), então podemos
    // atribuir o codigo a venda atual.
    venda->codigo = nova_quantidade;
    venda->codigo_cliente = codigo_cliente;

    sistema->vendas[nova_quantidade - 1] = *venda;
    sistema->quantidades.vendas = nova_quantidade;
    sistema->venda_atual = venda;

    // Aqui vem um erro, que foi percebido no meio do programa
    // (wagner nao me mate, o wesley fica de boa, mas o wagner vai dar uma surra)
    // quando atribuimos ao vetor, atribuimos a copia, ou seja, local.
    // muitas coisas dependem do vetor, ou seja, para evitar modificar localmente
    // e no vetor ainda permanecer intacto, nós pegamos o endereço dentro do vetor.
    // TINHA COMO EVITAR? tinha. mas eh plenas 01:21 da madrugada, percebi agora.
    // (tive que buscar pelos arquivos inteiros para tentar resolver, acabei achando.)
    venda = venda_pegar(sistema, venda->codigo);

    while (1) {
        // Exibe os produtos disponiveis do nosso sistema
        menu_exibir_produtos(sistema);

        // Pede para inserir o codigo do produto e tentar buscar ele
        int codigo = prompt_int("Insira o Código do Produto: ");
        Produto* produto = produto_pegar(sistema, codigo);

        // Enquanto o produto não existir, pedimos para inserir um codigo valido.
        while (!produto) {
            output("Código Invalido! Tente novamente.", 'a');
            codigo = prompt_int("Insira o Código do Produto: ");
            produto = produto_pegar(sistema, codigo);
        }

        // Aqui o produto existe, apenas precisamos saber se o estoque
        // é maior que zero, caso contrario, impedimos a venda, mesmo antes
        // do usuario inserir a quantidade desejada.
        if (produto->estoque.total == 0) {
            output("Produto esgotado! Tente novamente.", 'a');
            continue;
        }

        // O produto existe e seu estoque é maior que zero
        int quantidade = prompt_int("Insira a Quantidade do Produto: ");

        // Enquanto a quantidae for maior que o estoque total, ou ela for
        // menor que zero (negativo) pedimos para inserir uma quantidade valida.
        while (quantidade > produto->estoque.total || quantidade < 0) {
            output("Quantidade Invalida! Tente novamente. (Digite 0 para cancelar)", 'a');
            quantidade = prompt_int("Insira a Quantidade do Produto: ");
        }

        // Usamos 0 caso o usuario queira cancelar o ato de adicionar o item
        // no carrinho, na parte de quantidade. (ja que no produto, algum pode ter 0 como codigo)
        if (quantidade == 0) continue;

        // Calculo do estoque minimo
        int diferenca = (produto->estoque.total - quantidade);

        // Caso o valor digitado tenha atingido o estoque minimo...
        if (diferenca <= produto->estoque.minimo) {
            char mensagem[128];
            snprintf(
                mensagem,
                sizeof(mensagem),
                "Deseja realmente comprar %d %s? Tem %d no Estoque! [S/N]: ",
                quantidade,
                produto->descricao,
                produto->estoque.total
            );

            // Aqui pedimos a confirmação do usuario, se ele realmente quer comprar
            // embora atinja o valor de estoque minimo.
            char decisao = prompt_char(mensagem);

            while (decisao != 'S' && decisao != 'N') {
                output("Opção Invalida! Tente novamente.", 'a');
                decisao = prompt_char(mensagem);
            }

            // Aqui a desição já foi atribuida com um valor valido
            // Se for 'N' -> volta ao inicio do loop
            // Se for 'S' -> faz nada, vai continuar do mesmo jeito
            if (decisao == 'N') {
                continue;
            }
        }

        char item_novo = prompt_char("Item novo no Carrinho? [S/N]: ");
        if (item_novo == 'N') break; // Sem item novo, saimos do loop duma vez só.

        // Aqui, temos a certeza que o usuario realmente tem um item novo, vamos realizar
        // a logica de subtrair o estoque do produto e atualizar o item, ou criar um novo.
        if (item_exists(venda, codigo)) {
            // Item ja existe! Apenas atualizamos.
            item_update(
                sistema,
                venda,
                codigo,
                quantidade
            );

            output("ITEM ATUALIZADO!", 'l');
        } else {
            // Item não existe, vamos ter que criar um novo!
            Item* item = item_init(
                sistema,
                codigo,
                quantidade
            );

            // PUTZ, Falhou de cara kkkkk, agora apenas resta chorar :p
            if (!item) {
                output("FATAL; Item mal alocado - encerrando!", 'e');
                return;
            }

            // Re-alocamos o vetor de itens
            const int nova_quantidade = (venda->quantidade_itens + 1);
            venda->itens = safe_realloc(
                "Itens (+)",
                venda->itens,
                nova_quantidade * sizeof(Item)
            );

            // FALHOU DENOVO (choro imenso)
            if (!venda->itens) {
                output("FATAL; Itens mal alocados - encerrando!", 'e');
                return;
            }

            // agora ta tudo ok!
            venda->itens[nova_quantidade - 1] = *item;
            venda->quantidade_itens = nova_quantidade;
            output("ITEM CRIADO!", 'l');
        }

        // Subtraimos o estoque o produto (se não tirar é falha de semantica kkk)
        produto->estoque.total -= quantidade;
        output("COMPRA DO PRODUTO OK!", 'l');
    };

    // Estamos fora do loop, aqui devemos exibir o menu de itens da venda
    // e realizar o pagamento logo em seguida.
    menu_exibir_itens(sistema, venda);
    pagamento_iniciar(sistema, venda);
    printf("\n --- STATUS: %s ---\n", (venda->status == PAGAMENTO_ABERTO ? "PAGAMENTO ABERTO" : "PAGAMENTO FECHADO"));
    output("VENDA ~ FIM (@venda_iniciar)", 'l');
}
