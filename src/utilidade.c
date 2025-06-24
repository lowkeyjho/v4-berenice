/*
    Módulo de utilidades para a berenice.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "../include/constants.h"
#include "../include/cores.h"
#include "../include/utilidade.h"

/**
    Imprime uma mensagem no terminal, com cor, emoji e tag para deixar estetico.

    @param mensagem - Mensagem que vai ser exibida.
    @param especificador - Especifica o tipo da mensagem. (altera visual)
*/
void output(const char mensagem[], const char especificador) {
    const char* cor;
    const char* emoji;
    const char* tag;

    switch (especificador) {
        case 'a':
            cor = AMARELO;
            emoji = "🟨";
            tag = "AVISO";
            break;
        case 'e':
            cor = VERMELHO;
            emoji = "🛑";
            tag = "ERRO";
            break;
        case 's':
            cor = VERDE;
            emoji = "✅";
            tag = "SUCESSO";
            break;
        case 'l':
            cor = MAGENTA;
            emoji = "🟣";
            tag = "LOG";
            break;
    }

    printf(
        "\n%s・%s [%s] %s%s\n",
        cor,
        emoji,
        tag,
        mensagem,
        RESET
    );
}

/**
    Realiza uma limpeza no buffer (stdin) para evitar lixo e restos na leitura.
    Essa função é extremamente util; sem limpeza -> acumulação sem parar e erros.
*/
void prompt_limpar() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

/**
    Imprime uma mensagem no terminal e executa a leitura do que foi digitado;
    após isso, transforma o que foi digitado em um int (por meio de conversão).

    @param mensagem - A mensagem que vai ser exibida.
    @return O valor inteiro digitado.
*/
int prompt_int(const char mensagem[]) {
    char buffer[32]; // Buffer temporario que guarda 32 digitos.
    char *endptr; // Ponteiro que aponta para o primeiro char invalido

    while (1) {
        printf("\n" PROMPT_STYLE, mensagem);

        // Erro de leitura no buffer? Vamos limpar e repetir.
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            prompt_limpar();
            continue;
        }

        // Sem linha final, Provavelmente ainda tem lixo no stdin!
        if (!strchr(buffer, '\n')) prompt_limpar();

        // Caso tenha linha final no buffer, removemos!
        buffer[strcspn(buffer, "\n")] = '\0';

        // Buffer não tem nada? zero? vish...
        if (strlen(buffer) == 0) {
            output("Nada foi digitado! Tente novamente.", 'a');
            continue;
        }

        // Pega como float, depois converte pra int
        // strtof converte buffer -> float.
        float valor = strtof(buffer, &endptr);

        // Validação de input inválido;
        // (endptr == buffer) -> Nada foi convertido!
        // (*endptr != '\0') -> Sobrou lixo (letras, etc...) depois do numero!
        if (endptr == buffer || *endptr != '\0') {
            output("Número inteiro inválido.", 'a');
            continue;
        }

        // Converte pra int truncando
        return (int) valor;
    }
}

/**
    Mesmo esquema do `prompt_int()` exceto que essa função retorna um float.
    A simples diferença é que retornamos no final uma conversão diferente.

    @param mensagem - A mensagem que vai ser exibida.
    @return O valor float digitado.
*/
float prompt_float(const char mensagem[]) {
    char buffer[32]; // Buffer temporario que guarda 32 digitos.
    char *endptr; // Ponteiro que aponta para o primeiro char invalido

    while (1) {
        printf("\n" PROMPT_STYLE, mensagem);

        // Erro de leitura no buffer? Vamos limpar e repetir.
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            prompt_limpar();
            continue;
        }

        // Sem linha final, Provavelmente ainda tem lixo no stdin!
        if (!strchr(buffer, '\n')) prompt_limpar();

        // Caso tenha linha final no buffer, removemos!
        buffer[strcspn(buffer, "\n")] = '\0';

        // Buffer não tem nada? zero? vish...
        if (strlen(buffer) == 0) {
            output("Nada foi digitado! Tente novamente.", 'a');
            continue;
        }

        // Pega como float, depois converte pra int
        // strtof converte buffer -> float.
        float valor = strtof(buffer, &endptr);

        // Validação de input inválido;
        // (endptr == buffer) -> Nada foi convertido!
        // (*endptr != '\0') -> Sobrou lixo (letras, etc...) depois do numero!
        if (endptr == buffer || *endptr != '\0') {
            output("Número decimal inválido.", 'a');
            continue;
        }

        // Já é um float, mas convertemos para deixar explicito.
        return (float) valor;
    }
}

/**
    Diferente dos outros prompts, o de strings acaba armazenando o resultado
    em um buffer totalmente separado, ao invés de isolar ele ao escopo da função.
    Por conta disso, caso tente ler, deve-se declarar um buffer novo antes.

    @param mensagem - A mensagem que vai ser exibida
    @param buffer - Buffer aonde vai armazenar a string
    @param buffer_tamanho - Tamanho de leitura do buffer
    @return `void`
*/
void prompt_string(const char mensagem[], char buffer[], const size_t buffer_tamanho) {
    while (1) {
        printf("\n" PROMPT_STYLE, mensagem);

        // Erro na leitura do buffer, vamos repetir
        if (!fgets(buffer, buffer_tamanho, stdin)) {
            prompt_limpar();
            continue;
        }

        // Ainda tem lixo no buffer! Hora de limpar.
        if (!strchr(buffer, '\n')) prompt_limpar();

        // Remove a quebra de linha caso exista.
        buffer[strcspn(buffer, "\n")] = '\0';

        // Nada escrito? nem precisa continuar.
        if (strlen(buffer) == 0) {
            output("Nada foi digitado! Tente novamente.", 'a');
            continue;
        }

        break; // Tudo certo! Sai do loop.
    }
}

/**
    Diferente do `prompt_string()`, Faz a leitura de um unico caractere
    e retorna ele em sua versão maiscula, nada alem disso.

    @param mensagem - A mensagem que vai ser exibida no prompt.
    @return Char maiusculo que foi digitado.
*/
char prompt_char(const char mensagem[]) {
    char buffer[4]; // Sobra espaço de boas, pra garantir qualquer lixo.

    while (1) {
        printf("\n" PROMPT_STYLE, mensagem);

        // Tentativa de leitura, se deu falha, limpamos e repetimos
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            prompt_limpar();
            continue;
        }

        // Se não tem \n, então sobrou lixo -> limpar
        if (!strchr(buffer, '\n')) prompt_limpar();

        // Remove o \n se existir
        buffer[strcspn(buffer, "\n")] = '\0';

        // Nada digitado? Mancada kkkkkkk.
        if (strlen(buffer) == 0) {
            output("Nada foi digitado! Digite algo.", 'a');
            continue;
        }

        // Queremos apenas 1 caractere e nada alem disso.
        // Se foi digitado alem, avisamos!
        if (strlen(buffer) > 1) {
            output("Digite apenas 1 caractere!", 'a');
            continue;
        }

        // Trabalho feito - retornamos o primeiro char maiusculo.
        return toupper(buffer[0]);
    }
}
