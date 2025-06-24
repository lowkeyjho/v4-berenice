/*
    Módulo de Tempo da Berenice.
*/

#include <time.h>

#include "../include/tempo.h"
#include "../include/memoria.h"

/**
    Pega o tempo atual e retorna uma string com o tempo em formato de string.
    Isso vai passar pelo processo de conversão de tempo UNIX -> tempo local.

    @return String com o tempo atual.
*/
char* tempo_agora() {
    time_t tempo = time(NULL); // Segundos desde 1970
    struct tm *tempo_info = localtime(&tempo); // Struct de Info do Tempo
    char* tempo_buffer = safe_malloc(
        "Buffer de Tempo",
        64
    ); // Criamos um buffer para armazenar o tempo em string

    if (!tempo_buffer) return NULL; // Erro no alocamento, paramos!

    // Formata o tempo no buffer, note que `strftime` ajusta valores divergentes
    // como por exemplo; mes do struct inicia em 0 -> ele arruma para 1, ...
    strftime(
        tempo_buffer,
        64,
        "%d-%m-%Y",
        tempo_info
    );

    return tempo_buffer; // Retornamos o buffer
}
