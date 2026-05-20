# Gerenciador de Tarefas CLI em C

Sistema de gerenciamento de tarefas em linha de comando desenvolvido na linguagem C, utilizando os principais conceitos fundamentais da disciplina: structs, modularização, arquivos `.txt`, alocação dinâmica, recursão e menu interativo com `switch-case`.

---

# Grupo 6

## Integrantes

- Arthur Victor
- Christopher Mark
- Jean Augusto
- João Vitor
- Kellwen Igor
- Kaio Cerqueira
- Lucas Farias
- Lucas Rogério
- Mateus Xavier
  
---

# Objetivo do Projeto

Criar uma aplicação funcional de produtividade estilo:

- Todoist
- Microsoft To-Do list
- Google Tasks

permitindo:

- adicionar tarefas
- listar tarefas
- concluir tarefas
- remover tarefas
- salvar automaticamente em arquivo

---

# Funcionalidades

- Adicionar tarefas
- Listar todas as tarefas
- Listar tarefas pendentes
- Marcar tarefa como concluída
- Remover tarefa
- Filtrar por prioridade
- Persistência automática em arquivo `.txt`
- IDs auto incrementais
- Lista dinâmica usando `malloc` e `realloc`
- Funções recursivas

---

# Estrutura do Projeto

```txt
Lista-de-Tarefas/
│
├── main.c
├── tarefa.c
├── tarefa.h
├── tarefas.txt
└── Makefile
```

---

# Conceitos Aplicados

## Struct

```c
typedef struct {
    int id;
    char descricao[MAX_DESC];
    int prioridade;
    int concluida;
} Tarefa;
```

---

## Menu com switch-case

```c
switch (opcao) {
    case 1:
        adicionarTarefa(...);
        break;
}
```

---

## Alocação Dinâmica

### malloc

```c
tarefas = malloc((size_t)capacidade * sizeof(Tarefa));
```

### realloc

```c
temporario = realloc(*tarefas, (size_t)(*capacidade) * sizeof(Tarefa));
```

### free

```c
free(tarefas);
```

---

# Recursão

## Busca recursiva por ID

```c
return buscarPorIdRecursivo(
    tarefas,
    quantidade,
    id,
    indice + 1
);
```

## Impressão recursiva

```c
listarTodasRecursivo(
    tarefas,
    quantidade,
    indice + 1
);
```

---

# Persistência em Arquivo

As tarefas são armazenadas no arquivo:

```txt
tarefas.txt
```

Formato:

```txt
1|Estudar PIF|3|0
2|Fazer exercicio de C|2|1
```

Onde:

| Campo | Significado |
|---|---|
| 1 | ID |
| Estudar PIF | Descrição |
| 3 | Prioridade alta |
| 0 | Não concluída |

---

# Compilação

## Linux / Git Bash / WSL

```bash
gcc -Wall main.c tarefa.c -o todo
```

Executar:

```bash
./todo
```

---

## Windows (MinGW)

```bash
gcc -Wall main.c tarefa.c -o todo.exe
```

Executar:

```bash
todo.exe
```

---

# Usando Makefile

## Compilar

```bash
make
```

## Executar

```bash
make run
```

## Limpar executável

```bash
make clean
```

---

# Exemplo do Menu

```txt
===== TODO CLI =====

1 - Adicionar tarefa
2 - Listar todas
3 - Listar pendentes
4 - Marcar como concluida
5 - Remover tarefa
6 - Filtrar por prioridade
7 - Salvar e sair
```

---

# Requisitos Obrigatórios Atendidos

| Requisito | Implementado |
|---|---|
| Struct | Sim |
| fopen/fclose | Sim |
| switch-case | Sim |
| Recursão | Sim |
| Modularização (.h e .c) | Sim |
| malloc/realloc | Sim |
| free | Sim |
| Persistência em .txt | Sim |
| gcc -Wall sem warnings | Sim |

---

# Autor

Projeto acadêmico desenvolvido em linguagem C para prática de:

- lógica de programação
- manipulação de memória
- estruturas de dados básicas
- persistência de dados
- modularização de sistemas
