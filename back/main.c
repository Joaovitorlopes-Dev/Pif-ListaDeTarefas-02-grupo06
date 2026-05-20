#include <stdio.h>
#include <stdlib.h>
#include "tarefa.h"

int main(void) {
    Tarefa *tarefas;
    int quantidade = 0;
    int capacidade = CAPACIDADE_INICIAL;
    int proximoId = 1;
    int opcao = 0;

    tarefas = malloc((size_t)capacidade * sizeof(Tarefa));

    if (tarefas == NULL) {
        printf("Erro ao alocar memoria.\n");
        return 1;
    }

    carregarTarefas(&tarefas, &quantidade, &capacidade, &proximoId);

    do {
        printf("\n===== GERENCIADOR DE TAREFAS =====\n");
        printf("1 - Adicionar tarefa\n");
        printf("2 - Listar todas\n");
        printf("3 - Listar pendentes\n");
        printf("4 - Marcar como concluida\n");
        printf("5 - Remover tarefa\n");
        printf("6 - Filtrar por prioridade\n");
        printf("7 - Salvar e sair\n");
        printf("Escolha uma opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Opcao invalida.\n");
            limparBuffer();
            continue;
        }

        switch (opcao) {
            case 1:
                adicionarTarefa(&tarefas, &quantidade, &capacidade, &proximoId);
                break;

            case 2:
                listarTodasRecursivo(tarefas, quantidade, 0);
                break;

            case 3:
                listarPendentes(tarefas, quantidade);
                break;

            case 4:
                marcarComoConcluida(tarefas, quantidade);
                break;

            case 5:
                removerTarefa(tarefas, &quantidade);
                break;

            case 6:
                filtrarPorPrioridade(tarefas, quantidade);
                break;

            case 7:
                salvarTarefas(tarefas, quantidade);
                printf("Dados salvos. Programa encerrado.\n");
                break;

            default:
                printf("Opcao invalida.\n");
                break;
        }

    } while (opcao != 7);

    liberarMemoria(tarefas);

    return 0;
}
