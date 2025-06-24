#ifndef __PRODUTO_H__
#define __PRODUTO_H__

#include "structs.h"

Produto* produto_init();
Produto* produto_pegar(
    Sistema* sistema,
    const int codigo
);

#endif
