#ifndef __MENUS_H__
#define __MENUS_H__

#include "structs.h"

void menu_exibir(
    const char titulo[],
    const MenuOpcao menu_opcoes[],
    const int menu_opcoes_quantidade
);

void menu_exibir_produtos(
    Sistema* sistema
);

void menu_exibir_itens(
    Sistema* sistema,
    Venda* venda
);

void menu_exibir_usuarios(
    Sistema* sistema
);

void menu_exibir_vendas_abertas(
    Sistema* sistema
);

void menu_exibir_pagamento_opcoes(
    Sistema* sistema
);

void menu_exibir_pagamento_opcoes_cartao(
    Sistema* sistema
);

void menu_exibir_principal(
    Sistema* sistema
);

void menu_exibir_cadastros(
    Sistema* sistema
);

void menu_exibir_vendas(
    Sistema* sistema
);

void menu_exibir_abertura_caixa(
    Sistema* sistema
);

void menu_exibir_fechamento_caixa(
    Sistema* sistema
);

void menu_exibir_relatorios(
    Sistema* sistema
);

void menu_exibir_login(
    Sistema* sistema
);



#endif
