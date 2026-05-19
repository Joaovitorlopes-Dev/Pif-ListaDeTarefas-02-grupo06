#ifndef TAREFA_H
#define TAREFA_H

#define MAX_DESC 200
#define ARQUIVO_TAREFAS "tarefas.txt"
#define CAPACIDADE_INICIAL 5

typedef struct {
    int id;
    char descricao[MAX_DESC];
    int prioridade;
    int concluida;
} Tarefa;

void carregarTarefas(Tarefa **tarefas, int *quantidade, int *capacidade, int *proximoId);
void salvarTarefas(Tarefa *tarefas, int quantidade);

void adicionarTarefa(Tarefa **tarefas, int *quantidade, int *capacidade, int *proximoId);
void listarTodasRecursivo(Tarefa *tarefas, int quantidade, int indice);
void listarPendentes(Tarefa *tarefas, int quantidade);
void marcarComoConcluida(Tarefa *tarefas, int quantidade);
void removerTarefa(Tarefa *tarefas, int *quantidade);
void filtrarPorPrioridade(Tarefa *tarefas, int quantidade);

int buscarPorIdRecursivo(Tarefa *tarefas, int quantidade, int id, int indice);
void limparBuffer(void);
void liberarMemoria(Tarefa *tarefas);

#endif
