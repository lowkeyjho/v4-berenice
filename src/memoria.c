/*
    Módulo de Funções para alocação de memoria da Berenice.
*/

#include <stdio.h>
#include <stdlib.h>

#include "../include/memoria.h"
#include "../include/utilidade.h"

/**
    Uma versão safe ("segura") do `malloc()` que captura erros
    caso eles acontecam, exibindo uma mensagem no terminal. Se não
    houver nada, apenas retorna o ponteiro.

    Caso a alocação falhe, iremos encerrar o nosso programa, isso se
    da por conta de motivos simples, mas que são plausiveis:

    - 1. Se estamos alocando algo, PRECISAMOS daquilo. (erro? game over.)
    - 2. Não iremos tratar os erros como se fosse um projeto de larga escala.
    - 3. Dito o mesmo no motivo 2, melhor a se fazer seria avisar e vazar.

    @param identificador - String com o nome para identificar o que foi alocado.
    @param tamanho - Tamanho em bytes necessário para alocar
    @return `ptr*` - O ponteiro alocado.
*/
void* safe_malloc(const char identificador[], const size_t tamanho) {
    void* ptr = malloc(tamanho);

    // Ponteiro é null! Deu erro, hora de encerrar.
    if (ptr == NULL) {
        char mensagem[128];
        snprintf(mensagem, sizeof(mensagem), "Falha ao alocar %s! (%zu bytes)", identificador, tamanho);
        output(mensagem, 'e');
        exit(EXIT_FAILURE);
    }

    return ptr;
}

/**
    Quase a mesma coisa que o `safe_malloc()`, exceto que re-alocamos o ponteiro
    original para um novo ponteiro. Seguimos o mesmo processo de encerrar caso
    tenha algum erro.

    @param identificador - String com o nome para identificar o que foi alocado.
    @param original - Ponteiro original que vai ser re-alocado
    @param tamanho - Tamanho em bytes necessário para alocar
    @return `ptr*` - O ponteiro re-alocado.
*/
void* safe_realloc(const char identificador[], void* original, const size_t tamanho) {
    void* ptr = realloc(original, tamanho);

    // Deu erro denovo; encerramento na certa.
    if (ptr == NULL) {
        char mensagem[128];
        snprintf(mensagem, sizeof(mensagem), "Falha ao re-alocar %s! (%zu bytes)", identificador, tamanho);
        output(mensagem, 'e');
        exit(EXIT_FAILURE);
    }

    return ptr;
}
