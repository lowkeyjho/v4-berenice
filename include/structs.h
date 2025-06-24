#ifndef __STRUCTS_H__
#define __STRUCTS_H__

typedef struct {
    int numero;
    char rua[32];
    char bairro[32];
} EnderecoCliente;

typedef struct {
    char completo[64];
    char social[64];
} NomeCliente;

typedef struct {
    EnderecoCliente endereco;
    NomeCliente nome;
    int codigo;
    char whatsapp[20];
    char cpf[20];
} Cliente;

typedef struct {
    int total;
    int minimo;
} EstoqueProduto;

typedef struct {
    EstoqueProduto estoque;
    int codigo;
    int vendas;

    char* data;
    char descricao[32];
    char categoria[32];

    float preco_compra;
    float preco_venda;
    float margem_lucro;
} Produto;

typedef struct {
    int codigo;
    int vendas;
    char descricao[32];
    char categoria[32];
} ProdutoMaisVendido;

typedef struct {
    int codigo;
    int tipo; // 1. ADMIN / 2. USUARIO
    char login[13];
    char senha[9];
} Usuario;

typedef struct {
    int codigo;
    char nome[32];
} Categoria;

typedef enum {
    PAGAMENTO_ABERTO,
    PAGAMENTO_PAGO,
} VendaPagamento;

typedef struct {
    int codigo;
    int quantidade;
    float subtotal;
} Item;

typedef struct {
    Item* itens;
    VendaPagamento status;
    char* data;
    int codigo_cliente;
    int codigo;
    int quantidade_itens;
} Venda;

typedef enum {
    STATUS_ABERTO,
    STATUS_FECHADO,
} StatusCaixa;

typedef struct {
    int vendas;

    float abertura;
    float fechamento;
    float faturamento;

    float pago_dinheiro;
    float pago_misto_dinheiro;

    float pago_cartao;
    float pago_misto_cartao;
} QuantidadesCaixa;

typedef struct {
    StatusCaixa status;
    QuantidadesCaixa quantidades;
    char* data;
    float ajuste;
} Caixa;

typedef enum {
    TIPO_DINHEIRO,
    TIPO_CARTAO,
    TIPO_MISTO_DINHEIRO,
    TIPO_MISTO_CARTAO,
} TipoPagamento;

typedef struct {
    TipoPagamento tipo;
    char* data;
    int codigo_venda;
    float valor;
} Pagamento;

typedef struct {
    int clientes;
    int produtos;
    int usuarios;
    int categorias;
    int vendas;
    int pagamentos;
    int fechamentos;
} QuantidadeSistema;

typedef struct {
    char* data;
    float recebido_dinheiro;
    float recebido_cartao;
    float recebido_total;
} Fechamento;

typedef struct {
    Cliente* clientes;
    Produto* produtos;
    Usuario* usuarios;
    Venda* vendas;
    Pagamento* pagamentos;
    Categoria* categorias;
    Fechamento* fechamentos;

    Caixa* caixa;
    Usuario* usuario_atual;
    Venda* venda_atual;

    QuantidadeSistema quantidades;
} Sistema;

typedef struct {
    int numero;
    char mensagem[128];
} MenuOpcao;

#endif
