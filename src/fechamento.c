/*
    Modulo de Fechamento da Berenice.
*/

#include <stdio.h>
#include "../include/structs.h"
#include "../include/memoria.h"
#include "../include/tempo.h"

Fechamento* fechamento_init(Sistema* sistema) {
    Fechamento* fechamento = safe_malloc(
        "Fechamento",
        sizeof(Fechamento)
    );

    if (!fechamento) return NULL; // Falha no alocamento da memoria :(

    fechamento->data = tempo_agora();

    fechamento->recebido_dinheiro = (
        sistema->caixa->quantidades.pago_dinheiro +
        sistema->caixa->quantidades.pago_misto_dinheiro
    );

    fechamento->recebido_cartao = (
        sistema->caixa->quantidades.pago_cartao +
        sistema->caixa->quantidades.pago_misto_cartao
    );

    fechamento->recebido_total = (
        fechamento->recebido_dinheiro +
        fechamento->recebido_cartao
    );

    return fechamento;
}
