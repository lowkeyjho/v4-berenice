#ifndef __ARQUIVOS_H__
#define __ARQUIVOS_H__

#include <stdio.h>
#include "structs.h"

FILE* arquivo_abrir(
    const char identificador[],
    const char* modo
);

void arquivo_fechar(
    FILE* arquivo
);

void arquivo_escrever(
    FILE* arquivo,
    const char* formato,
    ...
);

void arquivo_salvar_usuarios(
    Sistema* sistema
);

void arquivo_salvar_clientes(
    Sistema* sistema
);

void arquivo_salvar_produtos(
    Sistema* sistema
);

void arquivo_salvar_vendas(
    Sistema* sistema
);

void arquivo_salvar_pagamentos(
    Sistema* sistema
);

void arquivo_salvar_fechamento(
    Sistema* sistema
);

void arquivo_salvar_todos(
    Sistema* sistema
);

#endif
