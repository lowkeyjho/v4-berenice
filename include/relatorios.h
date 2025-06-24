#ifndef __RELATORIOS_H__
#define __RELATORIOS_H__

#include "structs.h"

void relatorio_clientes(
    Sistema* sistema
);

void relatorio_clientes_ordenado(
    Sistema* sistema
);

void relatorio_clientes_periodo(
    Sistema* sistema
    /*, periodo*/
);

void relatorio_produtos(
    Sistema* sistema
);

void relatorio_produtos_ordenado(
    Sistema* sistema
);

void relatorio_produtos_estoques(
    Sistema* sistema
);

void relatorio_produtos_vendidos(
    Sistema* sistema
    /*, periodo*/
);

void relatorio_vendas(
    Sistema* sistema
);

void relatorio_vendas_periodo(
    Sistema* sistema
    /*, periodo*/
);

void relatorio_consolidado_periodo(
    Sistema* sistema
);


#endif
