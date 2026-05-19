#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_DESC 200
#define ARQUIVO_DADOS "tarefas.txt"

// Códigos ANSI para cores no terminal
#define COR_VERMELHA "\033[0;31m"
#define COR_AMARELA  "\033[0;33m"
#define COR_VERDE    "\033[0;32m"
#define COR_RESET    "\033[0m"

// Definição da Struct sugerida
typedef struct {
    int id;
    char descricao[MAX_DESC];
    int prioridade; // 1=baixa, 2=media, 3=alta
    int concluida;  // 0=nao, 1=sim
} Tarefa;

// Variáveis Globais para gerenciar o estado na memória
Tarefa *listaTarefas = NULL;
int totalTarefas = 0;
int proximoId = 1;

// --- FUNÇÕES AUXILIARES ---

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

// Remove a quebra de linha deixada pelo fgets
void removerNovaLinha(char *string) {
    size_t len = strlen(string);
    if (len > 0 && string[len - 1] == '\n') {
        string[len - 1] = '\0';
    }
}

// --- PERSISTÊNCIA ---

void salvarTarefas() {
    FILE *arquivo = fopen(ARQUIVO_DADOS, "w");
    if (arquivo == NULL) {
        printf("Erro ao abrir arquivo para salvar!\n");
        return;
    }
    for (int i = 0; i < totalTarefas; i++) {
        fprintf(arquivo, "%d|%s|%d|%d\n", 
                listaTarefas[i].id, 
                listaTarefas[i].descricao, 
                listaTarefas[i].prioridade, 
                listaTarefas[i].concluida);
    }
    fclose(arquivo);
}

void carregarTarefas() {
    FILE *arquivo = fopen(ARQUIVO_DADOS, "r");
    if (arquivo == NULL) {
        // Arquivo não existe ainda, o que é normal na primeira execução
        return;
    }

    char linha[300];
    while (fgets(linha, sizeof(linha), arquivo) != NULL) {
        Tarefa t;
        // Faz o parse da linha dividida por '|'
        // [^|] significa ler todos os caracteres até encontrar um '|'
        if (sscanf(linha, "%d|%199[^|]|%d|%d", &t.id, t.descricao, &t.prioridade, &t.concluida) == 4) {
            
            // Aloca espaço dinamicamente
            totalTarefas++;
            if (listaTarefas == NULL) {
                listaTarefas = (Tarefa*) malloc(sizeof(Tarefa));
            } else {
                listaTarefas = (Tarefa*) realloc(listaTarefas, totalTarefas * sizeof(Tarefa));
            }
            
            listaTarefas[totalTarefas - 1] = t;
            
            // Atualiza o auto-incremento para ser maior que o maior ID lido
            if (t.id >= proximoId) {
                proximoId = t.id + 1;
            }
        }
    }
    fclose(arquivo);
}

// --- FUNÇÃO RECURSIVA OBRIGATÓRIA ---

// Busca uma tarefa pelo ID de forma recursiva. 
// Retorna o índice no vetor ou -1 se não encontrar.
int buscarPorIdRecursivo(int idBuscado, int indiceAtual) {
    if (indiceAtual >= totalTarefas) {
        return -1; // Condição de parada: chegou ao fim da lista e não achou
    }
    if (listaTarefas[indiceAtual].id == idBuscado) {
        return indiceAtual; // Condição de parada: encontrou a tarefa
    }
    // Passo recursivo
    return buscarPorIdRecursivo(idBuscado, indiceAtual + 1);
}

// --- OPERAÇÕES CRUD ---

void adicionarTarefa() {
    Tarefa novaTarefa;
    novaTarefa.id = proximoId++;
    novaTarefa.concluida = 0;

    printf("\nDigite a descricao da tarefa: ");
    fgets(novaTarefa.descricao, MAX_DESC, stdin);
    removerNovaLinha(novaTarefa.descricao);

    printf("Prioridade (1-Baixa, 2-Media, 3-Alta): ");
    scanf("%d", &novaTarefa.prioridade);
    limparBuffer();

    // Expande a lista usando realloc
    totalTarefas++;
    if (listaTarefas == NULL) {
        listaTarefas = (Tarefa*) malloc(sizeof(Tarefa));
    } else {
        listaTarefas = (Tarefa*) realloc(listaTarefas, totalTarefas * sizeof(Tarefa));
    }

    listaTarefas[totalTarefas - 1] = novaTarefa;
    
    salvarTarefas(); // Salva imediatamente
    printf("Tarefa adicionada com sucesso!\n");
}

void imprimirTarefa(Tarefa t) {
    char *cor = COR_RESET;
    char *textoPrioridade = "";
    
    if (t.prioridade == 3) {
        cor = COR_VERMELHA;
        textoPrioridade = "ALTA ";
    } else if (t.prioridade == 2) {
        cor = COR_AMARELA;
        textoPrioridade = "MEDIA";
    } else if (t.prioridade == 1) {
        cor = COR_VERDE;
        textoPrioridade = "BAIXA";
    }

    printf("%s[%d] %-40s | Prioridade: %s | Status: %s%s\n", 
           cor, 
           t.id, 
           t.descricao, 
           textoPrioridade, 
           t.concluida ? "Concluida" : "Pendente", 
           COR_RESET);
}

void listarTarefas(int soPendentes, int filtroPrioridade) {
    printf("\n--- LISTA DE TAREFAS ---\n");
    int encontrou = 0;
    
    for (int i = 0; i < totalTarefas; i++) {
        int mostrar = 1;
        
        if (soPendentes && listaTarefas[i].concluida == 1) {
            mostrar = 0;
        }
        if (filtroPrioridade != 0 && listaTarefas[i].prioridade != filtroPrioridade) {
            mostrar = 0;
        }
        
        if (mostrar) {
            imprimirTarefa(listaTarefas[i]);
            encontrou = 1;
        }
    }
    
    if (!encontrou) {
        printf("Nenhuma tarefa encontrada com esses criterios.\n");
    }
}

void marcarConcluida() {
    int id;
    printf("\nDigite o ID da tarefa para concluir: ");
    scanf("%d", &id);
    limparBuffer();

    int indice = buscarPorIdRecursivo(id, 0); // Chamada da recursão
    if (indice != -1) {
        listaTarefas[indice].concluida = 1;
        salvarTarefas(); // Salva imediatamente
        printf("Tarefa [%d] marcada como concluida!\n", id);
    } else {
        printf("Erro: Tarefa com ID %d nao encontrada.\n", id);
    }
}

void removerTarefa() {
    int id;
    printf("\nDigite o ID da tarefa para remover: ");
    scanf("%d", &id);
    limparBuffer();

    int indice = buscarPorIdRecursivo(id, 0); // Chamada da recursão
    if (indice != -1) {
        // Desloca os elementos subsequentes para "apagar" a tarefa
        for (int i = indice; i < totalTarefas - 1; i++) {
            listaTarefas[i] = listaTarefas[i + 1];
        }
        totalTarefas--;
        
        // Reduz o tamanho do vetor com realloc
        if (totalTarefas > 0) {
            listaTarefas = (Tarefa*) realloc(listaTarefas, totalTarefas * sizeof(Tarefa));
        } else {
            free(listaTarefas);
            listaTarefas = NULL;
        }
        
        salvarTarefas(); // Salva imediatamente
        printf("Tarefa removida com sucesso!\n");
    } else {
        printf("Erro: Tarefa com ID %d nao encontrada.\n", id);
    }
}

void menu() {
    int opcao = 0;
    
    carregarTarefas(); // Carrega os dados persistidos no início

    while (opcao != 7) {
        printf("\n========================================\n");
        printf("        GERENCIADOR DE TAREFAS CLI        \n");
        printf("========================================\n");
        printf("1. Adicionar tarefa\n");
        printf("2. Listar todas as tarefas\n");
        printf("3. Listar tarefas pendentes\n");
        printf("4. Marcar tarefa como concluida\n");
        printf("5. Remover tarefa\n");
        printf("6. Filtrar por prioridade\n");
        printf("7. Salvar e sair\n");
        printf("Escolha uma opcao: ");
        
        if (scanf("%d", &opcao) != 1) {
            opcao = 0; // Previne loop infinito se digitar letra
        }
        limparBuffer();

        switch (opcao) {
            case 1:
                adicionarTarefa();
                break;
            case 2:
                listarTarefas(0, 0); // 0, 0 = mostra todas
                break;
            case 3:
                listarTarefas(1, 0); // 1 = só pendentes
                break;
            case 4:
                marcarConcluida();
                break;
            case 5:
                removerTarefa();
                break;
            case 6: {
                int prio;
                printf("Qual prioridade deseja listar? (1-Baixa, 2-Media, 3-Alta): ");
                scanf("%d", &prio);
                limparBuffer();
                listarTarefas(0, prio);
                break;
            }
            case 7:
                salvarTarefas(); // Garante o último salvamento
                printf("Saindo... Produtividade máxima!\n");
                break;
            default:
                printf("Opcao invalida. Tente novamente.\n");
        }
    }
    
    // Libera a memória alocada antes de fechar o programa
    if (listaTarefas != NULL) {
        free(listaTarefas);
    }
}

int main() {
    menu();
    return 0;
}