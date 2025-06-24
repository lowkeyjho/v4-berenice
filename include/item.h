#ifndef __ITEM_H__
#define __ITEM_H__

#include "structs.h"

Item* item_init(
    Sistema* sistema,
    const int codigo,
    int quantidade
);

void item_update(
    Sistema* sistema,
    Venda* venda,
    const int codigo,
    const int quantidade
);

int item_exists(
    Venda* venda,
    const int codigo
);

Item* item_pegar(
    Venda* venda,
    const int codigo
);



#endif
