#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tarefa.h"

void limparBuffer(void) {
    int c;

    while ((c = getchar()) != '\n' && c != EOF) {
    }
}

void salvarTarefas(Tarefa *tarefas, int quantidade) {
    FILE *arquivo = fopen(ARQUIVO_TAREFAS, "w");

    if (arquivo == NULL) {
        printf("Erro ao abrir o arquivo para escrita.\n");
        return;
    }

    for (int i = 0; i < quantidade; i++) {
        fprintf(
            arquivo,
            "%d|%s|%d|%d\n",
            tarefas[i].id,
            tarefas[i].descricao,
            tarefas[i].prioridade,
            tarefas[i].concluida
        );
    }

    fclose(arquivo);
}

void carregarTarefas(Tarefa **tarefas, int *quantidade, int *capacidade, int *proximoId) {
    FILE *arquivo = fopen(ARQUIVO_TAREFAS, "r");
    char linha[300];

    if (arquivo == NULL) {
        return;
    }

    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        Tarefa nova;

        if (sscanf(
                linha,
                "%d|%199[^|]|%d|%d",
                &nova.id,
                nova.descricao,
                &nova.prioridade,
                &nova.concluida
            ) == 4) {

            if (*quantidade >= *capacidade) {
                Tarefa *temporario;

                *capacidade *= 2;
                temporario = realloc(*tarefas, (size_t)(*capacidade) * sizeof(Tarefa));

                if (temporario == NULL) {
                    printf("Erro ao realocar memoria.\n");
                    fclose(arquivo);
                    free(*tarefas);
                    exit(1);
                }

                *tarefas = temporario;
            }

            (*tarefas)[*quantidade] = nova;
            (*quantidade)++;

            if (nova.id >= *proximoId) {
                *proximoId = nova.id + 1;
            }
        }
    }

    fclose(arquivo);
}

void adicionarTarefa(Tarefa **tarefas, int *quantidade, int *capacidade, int *proximoId) {
    Tarefa nova;

    if (*quantidade >= *capacidade) {
        Tarefa *temporario;

        *capacidade *= 2;
        temporario = realloc(*tarefas, (size_t)(*capacidade) * sizeof(Tarefa));

        if (temporario == NULL) {
            printf("Erro ao realocar memoria.\n");
            free(*tarefas);
            exit(1);
        }

        *tarefas = temporario;
    }

    nova.id = *proximoId;
    (*proximoId)++;

    limparBuffer();

    printf("Digite a descricao da tarefa: ");
    fgets(nova.descricao, MAX_DESC, stdin);
    nova.descricao[strcspn(nova.descricao, "\n")] = '\0';

    printf("Digite a prioridade 1=baixa, 2=media, 3=alta: ");
    while (scanf("%d", &nova.prioridade) != 1 || nova.prioridade < 1 || nova.prioridade > 3) {
        printf("Prioridade invalida. Digite 1, 2 ou 3: ");
        limparBuffer();
    }

    nova.concluida = 0;

    (*tarefas)[*quantidade] = nova;
    (*quantidade)++;

    salvarTarefas(*tarefas, *quantidade);

    printf("Tarefa adicionada com sucesso.\n");
}

void imprimirTarefa(Tarefa tarefa) {
    const char *prioridadeTexto;
    const char *statusTexto;

    if (tarefa.prioridade == 1) {
        prioridadeTexto = "Baixa";
    } else if (tarefa.prioridade == 2) {
        prioridadeTexto = "Media";
    } else {
        prioridadeTexto = "Alta";
    }

    if (tarefa.concluida == 1) {
        statusTexto = "Concluida";
    } else {
        statusTexto = "Pendente";
    }

    printf(
        "ID: %d | Descricao: %s | Prioridade: %s | Status: %s\n",
        tarefa.id,
        tarefa.descricao,
        prioridadeTexto,
        statusTexto
    );
}  

void listarTodasRecursivo(Tarefa *tarefas, int quantidade, int indice) {
    if (indice == 0) {
        printf("\n--- LISTA DE TAREFAS ---\n");
    }

    if (quantidade == 0) {
        printf("Nenhuma tarefa cadastrada.\n");
        return;
    }

    if (indice >= quantidade) {
        return;
    }

    imprimirTarefa(tarefas[indice]);

    listarTodasRecursivo(tarefas, quantidade, indice + 1);
}

void listarPendentes(Tarefa *tarefas, int quantidade) {
    int encontrou = 0;

    printf("\n--- TAREFAS PENDENTES ---\n");

    for (int i = 0; i < quantidade; i++) {
        if (tarefas[i].concluida == 0) {
            imprimirTarefa(tarefas[i]);
            encontrou = 1;
        }
    }

    if (encontrou == 0) {
        printf("Nenhuma tarefa pendente.\n");
    }
}

int buscarPorIdRecursivo(Tarefa *tarefas, int quantidade, int id, int indice) {
    if (indice >= quantidade) {
        return -1;
    }

    if (tarefas[indice].id == id) {
        return indice;
    }

    return buscarPorIdRecursivo(tarefas, quantidade, id, indice + 1);
}

void marcarComoConcluida(Tarefa *tarefas, int quantidade) {
    int id;
    int indice;

    printf("Digite o ID da tarefa que deseja concluir: ");

    if (scanf("%d", &id) != 1) {
        printf("Entrada invalida.\n");
        limparBuffer();
        return;
    }

    indice = buscarPorIdRecursivo(tarefas, quantidade, id, 0);

    if (indice == -1) {
        printf("Tarefa nao encontrada.\n");
        return;
    }

    tarefas[indice].concluida = 1;
    salvarTarefas(tarefas, quantidade);

    printf("Tarefa marcada como concluida.\n");
}

void removerTarefa(Tarefa *tarefas, int *quantidade) {
    int id;
    int indice;

    printf("Digite o ID da tarefa que deseja remover: ");

    if (scanf("%d", &id) != 1) {
        printf("Entrada invalida.\n");
        limparBuffer();
        return;
    }

    indice = buscarPorIdRecursivo(tarefas, *quantidade, id, 0);

    if (indice == -1) {
        printf("Tarefa nao encontrada.\n");
        return;
    }

    for (int i = indice; i < (*quantidade) - 1; i++) {
        tarefas[i] = tarefas[i + 1];
    }

    (*quantidade)--;

    salvarTarefas(tarefas, *quantidade);

    printf("Tarefa removida com sucesso.\n");
}

void filtrarPorPrioridade(Tarefa *tarefas, int quantidade) {
    int prioridade;
    int encontrou = 0;

    printf("Digite a prioridade 1=baixa, 2=media, 3=alta: ");

    if (scanf("%d", &prioridade) != 1 || prioridade < 1 || prioridade > 3) {
        printf("Prioridade invalida.\n");
        limparBuffer();
        return;
    }

    printf("\n--- TAREFAS FILTRADAS ---\n");

    for (int i = 0; i < quantidade; i++) {
        if (tarefas[i].prioridade == prioridade) {
            imprimirTarefa(tarefas[i]);
            encontrou = 1;
        }
    }

    if (encontrou == 0) {
        printf("Nenhuma tarefa encontrada com essa prioridade.\n");
    }
}

void liberarMemoria(Tarefa *tarefas) {
    free(tarefas);
}