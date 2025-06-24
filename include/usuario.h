#ifndef __USUARIO_H__
#define __USUARIO_H__

#include "structs.h"

Usuario* usuario_init();
Usuario* usuario_pegar(
    Sistema* sistema,
    char nome[]
);

#endif
