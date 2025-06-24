#ifndef __MEMORIA_H__
#define __MEMORIA_H__

void* safe_malloc(
    const char identificador[],
    const size_t tamanho
);

void* safe_realloc(
    const char identificador[],
    void* original,
    const size_t tamanho
);

#endif
