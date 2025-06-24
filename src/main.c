/*
    Berenice versão 4.0 - made by jho (feat. josé & nicholas)
    A documentação pode ser encontrada dentro da pasta ´docs´;
    suas devidas areas (cadastro, venda, ...) estão ali dentro.
*/

#include <stdlib.h>
#include <windows.h>
#include <locale.h>

#include "../include/arquivos.h"
#include "../include/menus.h"
#include "../include/sistema.h"
#include "../include/utilidade.h"
#include "../include/arquivos.h"
#include "../include/utilidade.h"
//#include "../include/constants.h"

int main(void) {
    // Adiciona suporte para caracteres e simbolos do Portugues + UTF8.
    // Compatibilidade extra; alguns terminais não vem com isso por padrão.
    SetConsoleOutputCP(CP_UTF8);
    setlocale(LC_ALL, "pt-BR.UTF_8");

    Sistema* sistema = sistema_iniciar();
    if (!sistema) return EXIT_FAILURE; // Alocação falhou! Paramos.

    // Exibe o menu principal, vai rodar um loop e vai apenas para quando
    // o usuário digitar um a opção de saída no menu principal.
    menu_exibir_principal(sistema);

    // Se o caixa ainda estiver aberto, e o usuario negar a saida, exibe o
    // menu novamente. Porém, dessa vez, se ele decidir sair, perdeu de vez!
    if (sistema->caixa->status == STATUS_ABERTO) {
        char opcao_continuar = prompt_char("O Caixa ainta esta Aberto! Deseja continuar mesmo assim? [S/N]: ");
        if (opcao_continuar == 'N') menu_exibir_principal(sistema);
    }

    char opcao_salvar = prompt_char("Você deseja salvar TODAS as informações em arquivos? [S/N]: ");
    if (opcao_salvar == 'S') arquivo_salvar_todos(sistema);

    // Encerra o sistema, liberando a memória e executando etapas extras.
    sistema_encerrar(&sistema);
    return EXIT_SUCCESS;
}
