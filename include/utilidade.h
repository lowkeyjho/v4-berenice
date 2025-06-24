#ifndef __UTILIDADE_H__
#define __UTILIDADE_H__

void output(
    const char mensagem[],
    const char especificador
);

void prompt_limpar();

int prompt_int(
    const char mensagem[]
);

float prompt_float(
    const char mensagem[]
);

void prompt_string(
    const char mensagem[],
    char buffer[],
    const size_t buffer_tamanho
);

char prompt_char(
    const char mensagem[]
);

#endif
