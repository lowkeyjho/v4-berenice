#ifndef __CONSTANTS_H__
#define __CONSTANTS_H__

#include "cores.h"

#define ARQUIVO_USUARIOS "data/__USUARIOS__.txt"
#define ARQUIVO_CLIENTES "data/__CLIENTES__.txt"
#define ARQUIVO_PRODUTOS "data/__PRODUTOS__.txt"
#define ARQUIVO_VENDAS "data/__VENDAS__.txt"
#define ARQUIVO_PAGAMENTOS "data/__PAGAMENTOS__.txt"
#define ARQUIVO_FECHAMENTOS "data/__FECHAMENTOS__.txt"

#define ARQUIVO_USUARIOS_MODO "a+"
#define ARQUIVO_CLIENTES_MODO "a+"
#define ARQUIVO_PRODUTOS_MODO "a+"
#define ARQUIVO_VENDAS_MODO "a+"
#define ARQUIVO_PAGAMENTOS_MODO "a+"
#define ARQUIVO_FECHAMENTOS_MODO "a+"

// Codigo;Tipo;Login;Senha
#define ARQUIVO_USUARIO_ID "%d;%d;%s;%s\n"

// Codigo;NomeCompleto;NomeSocial;Endereco;WhatsApp;CPF
#define ARQUIVO_CLIENTE_ID "%d;%s;%s;%s;%s;%s\n"

// Data;Codigo;Vendas;EstoqueTotal;EstoqueMin;Descricao;Categoria;PrecoCompra;PrecoVenda;MargemLucro
#define ARQUIVO_PRODUTO_ID "%s;%d;%d;%d;%d;%s;%s;%.2f;%.2f;%.2f\n"

// Data;Codigo;CodigoCliente;Status;QuantidadeItens
#define ARQUIVO_VENDA_ID "%s;%d;%d;%s;%d\n"

// Data;Codigo;Tipo;Valor
#define ARQUIVO_PAGAMENTO_ID "%s;%d;%s;%.2f\n"

// Data;ValorDinheiro;ValorCartao;ValorTotal
#define ARQUIVO_FECHAMENTO_ID "%s;%.2f;%.2f;%.2f\n"

#define PROMPT_SELECIONAR "Digite uma opção: "
#define PROMPT_INVALIDO "Opção inválida! Tente novamente."
#define PROMPT_STYLE VERDE "・>>> " RESET BRANCO "%s" RESET

#define CLIENTE_HEADER VERMELHO "%-24s" CIANO "%-48s" VERDE "%-24s" MAGENTA "%-24s" RESET "\n"
#define CLIENTE_IDENTIFIER VERMELHO "%-24d" CIANO "%-48s" VERDE "%-24s" MAGENTA "%-24s" RESET "\n"

#define USUARIO_HEADER VERMELHO "%-24s" CIANO "%-24s" AMARELO "%-24s" RESET "\n"
#define USUARIO_IDENTIFIER VERMELHO "%-24d" CIANO "%-24s" AMARELO "%-24s" RESET "\n"

#define PRODUTO_HEADER VERMELHO "%-24s" CIANO "%-32s" MAGENTA "%-32s" VERDE "%-24s" AMARELO "%-24s" RESET "\n"
#define PRODUTO_COM_ESTOQUE VERMELHO "%-24d" CIANO "%-32s" MAGENTA "%-32s" VERDE "R$%-22.2f" AMARELO "%-24d" RESET "\n"
#define PRODUTO_SEM_ESTOQUE VERMELHO "%-24d" CIANO "%-32s" MAGENTA "%-32s" VERDE "R$%-22.2f" VERMELHO "%-24s" RESET "\n"

#define ITEM_HEADER VERMELHO "%-24s" CIANO "%-32s" VERDE "%-24s" AMARELO "%-24s" VERDE "%-24s" RESET "\n"
#define ITEM_IDENTIFIER VERMELHO "%-24d" CIANO "%-32s" VERDE "R$%-22.2f" AMARELO "%-24d" VERDE "R$%-22.2f" RESET "\n"

#define VENDA_HEADER VERMELHO "%-24s" CIANO "%-24s" VERDE "%-24s" RESET "\n"
#define VENDA_IDENTIFIER VERMELHO "%-24d" CIANO "%-24d" VERMELHO "%-24s" RESET "\n"

#define RELATORIO_VENDA_HEADER VERMELHO "%-24s" VERDE "%-24s" CIANO "%-48s" MAGENTA "%-48s" RESET "\n"
#define RELATORIO_VENDA_IDENTIFIER VERMELHO "%-24d" VERDE "%-24d" CIANO "%-48s" MAGENTA "%-48s" RESET "\n"

#define RELATORIO_CONSOLIDADO_HEADER AMARELO "%-24s" VERDE "%-36s" CIANO "%-36s" MAGENTA "%-36s" RESET "\n"
#define RELATORIO_CONSOLIDADO_IDENTIFIER AMARELO "%-24s" VERDE "R$%-34.2f" CIANO "R$%-34.2f" MAGENTA "R$%-34.2f" RESET "\n"

#endif
