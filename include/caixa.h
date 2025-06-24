#ifndef __CAIXA_H__
#define __CAIXA_H__

#include "structs.h"

void caixa_abrir(
    Sistema* sistema
);

void caixa_fechar(
    Sistema* sistema
);

void caixa_retirar(
    Sistema* sistema
);

void caixa_reset(
    Sistema* sistema
);

#endif
