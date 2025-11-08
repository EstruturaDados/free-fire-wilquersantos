#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>

#define MAX 10

// -----------------------------
// ESTRUTURA PRINCIPAL
// -----------------------------
typedef struct {
    char nome[30];
    char tipo[20];
    int quantidade;
    int prioridade; // usado no nível Mestre
} Item;

// Enumerador para critério de ordenação
typedef enum {
    POR_NOME = 1,
    POR_TIPO,
    POR_PRIORIDADE
} Criterio;

// -----------------------------
// FUNÇÕES AUXILIARES
// -----------------------------
void listarItens(Item v[], int n) {
    if (n == 0) {
        printf("\n⚠️  Nenhum item na mochila!\n");
        return;
    }

    printf("\n================== ITENS DA MOCHILA ==================\n");
    printf("%-25s | %-15s | %-10s | %s\n", "NOME", "TIPO", "QTD", "PRIORIDADE");
    printf("-------------------------------------------------------\n");
    for (int i = 0; i < n; i++) {
        printf("%-25s | %-15s | %-10d | %d\n", v[i].nome, v[i].tipo, v[i].quantidade, v[i].prioridade);
    }
    printf("-------------------------------------------------------\n");
}

// -----------------------------
// NÍVEL NOVATO - BÁSICO
// -----------------------------
int adicionarItem(Item v[], int *n) {
    if (*n >= MAX) {
        printf("\n⚠️  Mochila cheia! Capacidade máxima atingida.\n");
        return 0;
    }

    printf("\nDigite o nome do item: ");
    fgets(v[*n].nome, 30, stdin);
    v[*n].nome[strcspn(v[*n].nome, "\n")] = '\0';

    printf("Digite o tipo do item: ");
    fgets(v[*n].tipo, 20, stdin);
    v[*n].tipo[strcspn(v[*n].tipo, "\n")] = '\0';

    printf("Digite a quantidade: ");
    scanf("%d", &v[*n].quantidade);
    getchar();

    printf("Digite a prioridade (1 a 5): ");
    scanf("%d", &v[*n].prioridade);
    getchar();

    (*n)++;
    printf("✅ Item adicionado com sucesso!\n");
    return 1;
}

int removerItem(Item v[], int *n) {
    if (*n == 0) {
        printf("\n⚠️  A mochila está vazia!\n");
        return 0;
    }

    char alvo[30];
    printf("\nDigite o nome do item a remover: ");
    fgets(alvo, 30, stdin);
    alvo[strcspn(alvo, "\n")] = '\0';

    for (int i = 0; i < *n; i++) {
        if (strcmp(v[i].nome, alvo) == 0) {
            for (int j = i; j < *n - 1; j++) {
                v[j] = v[j + 1];
            }
            (*n)--;
            printf("🗑️  Item removido com sucesso!\n");
            return 1;
        }
    }

    printf("❌ Item não encontrado para remoção.\n");
    return 0;
}

// -----------------------------
// NÍVEL AVENTUREIRO - BUSCA SEQUENCIAL
// -----------------------------
void buscarItem(Item v[], int n) {
    if (n == 0) {
        printf("\n⚠️  A mochila está vazia!\n");
        return;
    }

    char alvo[30];
    printf("\nDigite o nome do item a buscar: ");
    fgets(alvo, 30, stdin);
    alvo[strcspn(alvo, "\n")] = '\0';

    bool encontrado = false;
    for (int i = 0; i < n; i++) {
        if (strcmp(v[i].nome, alvo) == 0) {
            printf("\n✅ ITEM ENCONTRADO!\n");
            printf("Nome: %s\nTipo: %s\nQuantidade: %d\nPrioridade: %d\n",
                   v[i].nome, v[i].tipo, v[i].quantidade, v[i].prioridade);
            encontrado = true;
            break;
        }
    }

    if (!encontrado)
        printf("❌ Item não encontrado.\n");
}

// -----------------------------
// NÍVEL MESTRE - ORDENAÇÃO E BUSCA BINÁRIA
// -----------------------------
void insertionSort(Item v[], int n, Criterio criterio, long *comparacoes) {
    *comparacoes = 0;
    for (int i = 1; i < n; i++) {
        Item chave = v[i];
        int j = i - 1;
        while (j >= 0) {
            int cmp = 0;
            if (criterio == POR_NOME)
                cmp = strcmp(chave.nome, v[j].nome) < 0;
            else if (criterio == POR_TIPO)
                cmp = strcmp(chave.tipo, v[j].tipo) < 0;
            else
                cmp = chave.prioridade < v[j].prioridade;

            (*comparacoes)++;
            if (cmp) {
                v[j + 1] = v[j];
                j--;
            } else
                break;
        }
        v[j + 1] = chave;
    }
}

int buscaBinaria(Item v[], int n, char nomeBuscado[], long *comparacoes) {
    int inicio = 0, fim = n - 1;
    *comparacoes = 0;

    while (inicio <= fim) {
        int meio = (inicio + fim) / 2;
        (*comparacoes)++;
        int cmp = strcmp(v[meio].nome, nomeBuscado);

        if (cmp == 0)
            return meio;
        else if (cmp < 0)
            inicio = meio + 1;
        else
            fim = meio - 1;
    }
    return -1;
}

// -----------------------------
// FUNÇÃO PRINCIPAL
// -----------------------------
int main() {
    Item mochila[MAX];
    int qtd = 0;
    int opcao;
    bool ordenado = false;
    Criterio ultimoCriterio;
    long comparacoes;
    double tempo;

    do {
        printf("\n===== 🎖️ MENU PRINCIPAL - CODIGO DA ILHA 🎖️ =====\n");
        printf("[1] Adicionar item\n");
        printf("[2] Remover item\n");
        printf("[3] Listar itens\n");
        printf("[4] Buscar item por nome (sequencial)\n");
        printf("[5] Ordenar mochila (Insertion Sort)\n");
        printf("[6] Buscar item por nome (busca binária)\n");
        printf("[0] Sair\n");
        printf("Escolha uma opção: ");
        scanf("%d", &opcao);
        getchar();

        switch (opcao) {
            case 1:
                adicionarItem(mochila, &qtd);
                ordenado = false;
                break;

            case 2:
                removerItem(mochila, &qtd);
                ordenado = false;
                break;

            case 3:
                listarItens(mochila, qtd);
                break;

            case 4:
                buscarItem(mochila, qtd);
                break;

            case 5: {
                if (qtd == 0) {
                    printf("\n⚠️  Mochila vazia!\n");
                    break;
                }
                int c;
                printf("\nOrdenar por:\n1 - Nome\n2 - Tipo\n3 - Prioridade\nEscolha: ");
                scanf("%d", &c);
                getchar();

                clock_t inicio = clock();
                insertionSort(mochila, qtd, c, &comparacoes);
                tempo = (double)(clock() - inicio) / CLOCKS_PER_SEC;

                printf("\n✅ Mochila ordenada!\n");
                printf("Comparações: %ld | Tempo: %.6f s\n", comparacoes, tempo);
                listarItens(mochila, qtd);

                ordenado = (c == POR_NOME);
                ultimoCriterio = c;
                break;
            }

            case 6: {
                if (!ordenado) {
                    printf("\n⚠️  A busca binária exige lista ordenada por nome!\n");
                    break;
                }

                char alvo[30];
                printf("Digite o nome do item para buscar: ");
                fgets(alvo, 30, stdin);
                alvo[strcspn(alvo, "\n")] = '\0';

                int pos = buscaBinaria(mochila, qtd, alvo, &comparacoes);
                if (pos != -1) {
                    printf("\n✅ Item encontrado (busca binária)!\n");
                    printf("Nome: %s | Tipo: %s | Qtd: %d | Prioridade: %d\n",
                           mochila[pos].nome, mochila[pos].tipo,
                           mochila[pos].quantidade, mochila[pos].prioridade);
                } else {
                    printf("\n❌ Item não encontrado.\n");
                }
                printf("Comparações na busca: %ld\n", comparacoes);
                break;
            }

            case 0:
                printf("\n💼 Fim da jornada! Você sobreviveu ao Código da Ilha!\n");
                break;

            default:
                printf("\nOpção inválida. Tente novamente!\n");
                break;
        }

    } while (opcao != 0);

    return 0;
}