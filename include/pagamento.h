#ifndef __PAGAMENTO_H__
#define __PAGAMENTO_H__

#include "structs.h"

Pagamento* pagamento_init(
    const int codigo_venda,
    const float valor,
    const TipoPagamento tipo
);

char* pagamento_tipo(
    Pagamento* pagamento
);

void pagamento_iniciar(
    Sistema* sistema,
    Venda* venda
);

void pagamento_dinheiro(
    Sistema* sistema,
    Venda* venda,
    float valor_venda,
    float valor_restante
);

void pagamento_cartao(
    Sistema* sistema,
    Venda* venda,
    float valor_venda,
    float valor_restante
);

#endif
