#ifndef __CLIENTE_H__
#define __CLIENTE_H__

#include "structs.h"

Cliente* cliente_init();
Cliente* cliente_pegar(
    Sistema *sistema,
    int codigo
);

#endif
