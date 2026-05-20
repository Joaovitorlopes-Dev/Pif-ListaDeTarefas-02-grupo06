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