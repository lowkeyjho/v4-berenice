#ifndef __VENDA_H__
#define __VENDA_H__

#include "structs.h"

Venda* venda_init(
    Sistema* sistema
);

void venda_iniciar(
    Sistema* sistema
);

int venda_is_aberta(
    Sistema* sistema,
    const int codigo
);

void venda_atualizar_produtos(
    Sistema* sistema,
    Venda* venda
);

Venda* venda_pegar(
    Sistema* sistema,
    const int codigo
);

float venda_calcular_valor(
    Venda* venda
);

float venda_calcular_desconto(
    Venda* venda,
    float valor_total
);

#endif
