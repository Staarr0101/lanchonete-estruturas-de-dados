#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* =====================================================================
 *  DEFINICOES DE CONSTANTES
 * ===================================================================== */
#define MAX_NOME 50
#define MAX_ATENDENTES 5

/* =====================================================================
 *  ESTRUTURAS DE DADOS
 * ===================================================================== */

/* --- 1. No da Lista Simplesmente Encadeada (Cardapio) --- */
typedef struct NoProduto {
    int codigo;
    char nome[MAX_NOME];
    float preco;
    struct NoProduto *proximo;  /* ponteiro para o proximo no */
} NoProduto;

/* --- 2. No da Lista Duplamente Encadeada (Historico de Vendas) --- */
typedef struct NoVenda {
    int idVenda;
    char nomeProduto[MAX_NOME];
    float valor;
    struct NoVenda *anterior;   /* ponteiro para o no anterior */
    struct NoVenda *proximo;    /* ponteiro para o proximo no */
} NoVenda;

/* --- 3. No da Lista Circular (Escala de Atendentes) --- */
typedef struct NoAtendente {
    char nome[MAX_NOME];
    struct NoAtendente *proximo; /* aponta de volta ao inicio quando chega ao fim */
} NoAtendente;

/* --- 4. No da Pilha (Historico de Pedidos para Desfazer) --- */
typedef struct NoPilha {
    int codigoProduto;
    char nomeProduto[MAX_NOME];
    float preco;
    struct NoPilha *proximo;    /* topo da pilha aponta para baixo */
} NoPilha;

/* --- 5. No da Fila (Fila de Clientes) --- */
typedef struct NoFila {
    char nomeCliente[MAX_NOME];
    struct NoFila *proximo;
} NoFila;

/* --- 6. No da Arvore Binaria de Busca (Catalogo de Produtos) --- */
typedef struct NoArvore {
    int codigo;
    char nome[MAX_NOME];
    float preco;
    struct NoArvore *esquerda;  /* filhos com codigo menor */
    struct NoArvore *direita;   /* filhos com codigo maior */
} NoArvore;

/* =====================================================================
 *  VARIAVEIS GLOBAIS (cabecas/raizes das estruturas)
 * ===================================================================== */
NoProduto   *cabecaCardapio   = NULL; /* inicio da lista do cardapio     */
NoVenda     *cabecaVendas     = NULL; /* inicio do historico de vendas    */
NoVenda     *fimVendas        = NULL; /* fim do historico de vendas       */
NoAtendente *atendentesCirc   = NULL; /* lista circular de atendentes     */
NoPilha     *topoPilha        = NULL; /* topo da pilha de pedidos         */
NoFila      *inicioFila       = NULL; /* inicio da fila de clientes       */
NoFila      *fimFila          = NULL; /* fim da fila de clientes          */
NoArvore    *raizArvore       = NULL; /* raiz da arvore binaria           */

int contadorVendas = 0; /* ID incremental de vendas */

/* =====================================================================
 *  FUNCOES DA LISTA SIMPLESMENTE ENCADEADA (CARDAPIO)
 * ===================================================================== */

/* Insere produto no inicio da lista do cardapio */
void inserirCardapio(int codigo, char *nome, float preco) {
    NoProduto *novo = (NoProduto*) malloc(sizeof(NoProduto));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente!\n");
        return;
    }
    novo->codigo = codigo;
    strcpy(novo->nome, nome);
    novo->preco = preco;
    novo->proximo = cabecaCardapio; /* novo aponta para o antigo inicio */
    cabecaCardapio = novo;          /* novo passa a ser o inicio        */
    printf("[CARDAPIO] Produto '%s' inserido com sucesso!\n", nome);
}

/* Remove produto do cardapio pelo codigo */
void removerCardapio(int codigo) {
    NoProduto *atual = cabecaCardapio;
    NoProduto *anterior = NULL;

    while (atual != NULL) {
        if (atual->codigo == codigo) {
            if (anterior == NULL) {
                /* o no a remover e o primeiro da lista */
                cabecaCardapio = atual->proximo;
            } else {
                /* liga o anterior diretamente ao proximo do removido */
                anterior->proximo = atual->proximo;
            }
            printf("[CARDAPIO] Produto '%s' removido!\n", atual->nome);
            free(atual); /* libera a memoria */
            return;
        }
        anterior = atual;
        atual = atual->proximo;
    }
    printf("[CARDAPIO] Produto com codigo %d nao encontrado.\n", codigo);
}

/* Exibe todos os produtos do cardapio */
void exibirCardapio() {
    NoProduto *atual = cabecaCardapio;
    if (atual == NULL) {
        printf("[CARDAPIO] Cardapio vazio!\n");
        return;
    }
    printf("\n========== CARDAPIO ==========\n");
    printf("%-6s %-20s %s\n", "Cod.", "Produto", "Preco");
    printf("-------------------------------\n");
    while (atual != NULL) {
        printf("%-6d %-20s R$ %.2f\n", atual->codigo, atual->nome, atual->preco);
        atual = atual->proximo;
    }
    printf("===============================\n");
}

/* Busca produto no cardapio pelo codigo */
NoProduto* buscarCardapio(int codigo) {
    NoProduto *atual = cabecaCardapio;
    while (atual != NULL) {
        if (atual->codigo == codigo) return atual;
        atual = atual->proximo;
    }
    return NULL; /* nao encontrado */
}

/* =====================================================================
 *  FUNCOES DA LISTA DUPLAMENTE ENCADEADA (HISTORICO DE VENDAS)
 * ===================================================================== */

/* Insere uma venda no final do historico */
void inserirVenda(char *nomeProduto, float valor) {
    NoVenda *novo = (NoVenda*) malloc(sizeof(NoVenda));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente!\n");
        return;
    }
    contadorVendas++;
    novo->idVenda = contadorVendas;
    strcpy(novo->nomeProduto, nomeProduto);
    novo->valor = valor;
    novo->proximo = NULL;
    novo->anterior = fimVendas; /* o anterior do novo e o atual ultimo */

    if (fimVendas != NULL) {
        fimVendas->proximo = novo; /* o antigo ultimo aponta para o novo */
    } else {
        cabecaVendas = novo; /* lista estava vazia, novo e o primeiro */
    }
    fimVendas = novo; /* novo e o ultimo agora */
}

/* Exibe o historico de vendas do inicio ao fim */
void exibirVendasAscendente() {
    NoVenda *atual = cabecaVendas;
    if (atual == NULL) {
        printf("[VENDAS] Nenhuma venda registrada.\n");
        return;
    }
    printf("\n===== HISTORICO DE VENDAS (mais antigo -> mais recente) =====\n");
    while (atual != NULL) {
        printf("Venda #%d | Produto: %-20s | R$ %.2f\n",
               atual->idVenda, atual->nomeProduto, atual->valor);
        atual = atual->proximo;
    }
    printf("=============================================================\n");
}

/* Exibe o historico de vendas do fim ao inicio (vantagem da lista dupla) */
void exibirVendasDescendente() {
    NoVenda *atual = fimVendas;
    if (atual == NULL) {
        printf("[VENDAS] Nenhuma venda registrada.\n");
        return;
    }
    printf("\n===== HISTORICO DE VENDAS (mais recente -> mais antigo) =====\n");
    while (atual != NULL) {
        printf("Venda #%d | Produto: %-20s | R$ %.2f\n",
               atual->idVenda, atual->nomeProduto, atual->valor);
        atual = atual->anterior; /* percorre para tras! */
    }
    printf("=============================================================\n");
}

/* =====================================================================
 *  FUNCOES DA LISTA CIRCULAR (ESCALA DE ATENDENTES)
 * ===================================================================== */

/* Insere atendente na lista circular */
void inserirAtendente(char *nome) {
    NoAtendente *novo = (NoAtendente*) malloc(sizeof(NoAtendente));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente!\n");
        return;
    }
    strcpy(novo->nome, nome);

    if (atendentesCirc == NULL) {
        /* primeiro atendente aponta para si mesmo (circulo de 1) */
        novo->proximo = novo;
        atendentesCirc = novo;
    } else {
        /* percorre ate o ultimo (quem aponta para o primeiro) */
        NoAtendente *atual = atendentesCirc;
        while (atual->proximo != atendentesCirc) {
            atual = atual->proximo;
        }
        atual->proximo = novo;        /* ultimo aponta para o novo */
        novo->proximo = atendentesCirc; /* novo aponta para o inicio (circulo!) */
    }
    printf("[ATENDENTES] Atendente '%s' adicionado a escala.\n", nome);
}

/* Avanca para o proximo atendente na escala circular */
void proximoAtendente() {
    if (atendentesCirc == NULL) {
        printf("[ATENDENTES] Nenhum atendente cadastrado!\n");
        return;
    }
    printf("[ATENDENTES] Atendente atual: %s\n", atendentesCirc->nome);
    atendentesCirc = atendentesCirc->proximo; /* avanca circularmente */
    printf("[ATENDENTES] Proximo da vez: %s\n", atendentesCirc->nome);
}

/* Exibe todos os atendentes na escala */
void exibirAtendentes() {
    if (atendentesCirc == NULL) {
        printf("[ATENDENTES] Nenhum atendente cadastrado!\n");
        return;
    }
    NoAtendente *inicio = atendentesCirc;
    NoAtendente *atual = atendentesCirc;
    printf("\n===== ESCALA DE ATENDENTES =====\n");
    do {
        printf("  -> %s\n", atual->nome);
        atual = atual->proximo;
    } while (atual != inicio); /* para quando voltar ao inicio */
    printf("================================\n");
}

/* =====================================================================
 *  FUNCOES DA PILHA (HISTORICO DE PEDIDOS - DESFAZER PEDIDO)
 * ===================================================================== */

/* Empilha (push) um pedido na pilha */
void empilharPedido(int codigo, char *nome, float preco) {
    NoPilha *novo = (NoPilha*) malloc(sizeof(NoPilha));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente!\n");
        return;
    }
    novo->codigoProduto = codigo;
    strcpy(novo->nomeProduto, nome);
    novo->preco = preco;
    novo->proximo = topoPilha; /* novo vai para cima da pilha */
    topoPilha = novo;
    printf("[PILHA] Pedido '%s' adicionado ao carrinho!\n", nome);
}

/* Desempilha (pop) o ultimo pedido - funcao desfazer */
void desfazerPedido() {
    if (topoPilha == NULL) {
        printf("[PILHA] Nenhum pedido para desfazer!\n");
        return;
    }
    NoPilha *removido = topoPilha;
    topoPilha = topoPilha->proximo; /* topo sobe para o proximo */
    printf("[PILHA] Pedido '%s' desfeito! (R$ %.2f devolvido)\n",
           removido->nomeProduto, removido->preco);
    free(removido);
}

/* Exibe todos os pedidos no carrinho */
void exibirPilha() {
    NoPilha *atual = topoPilha;
    if (atual == NULL) {
        printf("[PILHA] Carrinho de pedidos vazio!\n");
        return;
    }
    float total = 0;
    printf("\n===== CARRINHO DE PEDIDOS =====\n");
    while (atual != NULL) {
        printf("  %s - R$ %.2f\n", atual->nomeProduto, atual->preco);
        total += atual->preco;
        atual = atual->proximo;
    }
    printf("-------------------------------\n");
    printf("  TOTAL: R$ %.2f\n", total);
    printf("===============================\n");
}

/* Confirma o pedido: esvazia a pilha e registra as vendas */
void confirmarPedido() {
    if (topoPilha == NULL) {
        printf("[PILHA] Nenhum pedido para confirmar!\n");
        return;
    }
    printf("\n[PEDIDO] Confirmando pedido...\n");
    while (topoPilha != NULL) {
        inserirVenda(topoPilha->nomeProduto, topoPilha->preco);
        NoPilha *tmp = topoPilha;
        topoPilha = topoPilha->proximo;
        free(tmp);
    }
    printf("[PEDIDO] Pedido confirmado e registrado no historico!\n");
}

/* =====================================================================
 *  FUNCOES DA FILA (FILA DE CLIENTES)
 * ===================================================================== */

/* Enfileira (enqueue) um cliente */
void enfileirarCliente(char *nome) {
    NoFila *novo = (NoFila*) malloc(sizeof(NoFila));
    if (novo == NULL) {
        printf("Erro: memoria insuficiente!\n");
        return;
    }
    strcpy(novo->nomeCliente, nome);
    novo->proximo = NULL;

    if (fimFila != NULL) {
        fimFila->proximo = novo; /* entra no final da fila */
    } else {
        inicioFila = novo;       /* fila estava vazia */
    }
    fimFila = novo;
    printf("[FILA] Cliente '%s' entrou na fila!\n", nome);
}

/* Desenfileira (dequeue) o primeiro cliente */
void atenderCliente() {
    if (inicioFila == NULL) {
        printf("[FILA] Nenhum cliente na fila!\n");
        return;
    }
    NoFila *atendido = inicioFila;
    inicioFila = inicioFila->proximo; /* proximo da fila avanca */
    if (inicioFila == NULL) {
        fimFila = NULL; /* fila ficou vazia */
    }
    printf("[FILA] Atendendo cliente: %s\n", atendido->nomeCliente);
    free(atendido);
}

/* Exibe todos os clientes na fila */
void exibirFila() {
    NoFila *atual = inicioFila;
    if (atual == NULL) {
        printf("[FILA] Fila de espera vazia!\n");
        return;
    }
    int posicao = 1;
    printf("\n===== FILA DE CLIENTES =====\n");
    while (atual != NULL) {
        printf("  %d. %s\n", posicao++, atual->nomeCliente);
        atual = atual->proximo;
    }
    printf("============================\n");
}

/* =====================================================================
 *  FUNCOES DA ARVORE BINARIA DE BUSCA (CATALOGO DE PRODUTOS)
 * ===================================================================== */

/* Insere produto na arvore (codigo e a chave de ordenacao) */
NoArvore* inserirArvore(NoArvore *raiz, int codigo, char *nome, float preco) {
    if (raiz == NULL) {
        /* cria novo no folha */
        NoArvore *novo = (NoArvore*) malloc(sizeof(NoArvore));
        if (novo == NULL) {
            printf("Erro: memoria insuficiente!\n");
            return NULL;
        }
        novo->codigo = codigo;
        strcpy(novo->nome, nome);
        novo->preco = preco;
        novo->esquerda = NULL;
        novo->direita = NULL;
        return novo;
    }
    /* insere a esquerda se codigo menor, a direita se maior */
    if (codigo < raiz->codigo) {
        raiz->esquerda = inserirArvore(raiz->esquerda, codigo, nome, preco);
    } else if (codigo > raiz->codigo) {
        raiz->direita = inserirArvore(raiz->direita, codigo, nome, preco);
    } else {
        printf("[ARVORE] Produto com codigo %d ja existe!\n", codigo);
    }
    return raiz;
}

/* Busca produto na arvore pelo codigo (busca binaria O(log n)) */
NoArvore* buscarArvore(NoArvore *raiz, int codigo) {
    if (raiz == NULL) return NULL;         /* nao encontrado */
    if (codigo == raiz->codigo) return raiz; /* encontrado! */
    if (codigo < raiz->codigo)
        return buscarArvore(raiz->esquerda, codigo); /* busca a esquerda */
    else
        return buscarArvore(raiz->direita, codigo);  /* busca a direita  */
}

/* Exibe a arvore em ordem (in-order = ordem crescente de codigo) */
void exibirArvoreEmOrdem(NoArvore *raiz) {
    if (raiz == NULL) return;
    exibirArvoreEmOrdem(raiz->esquerda);
    printf("  Codigo: %-4d | %-20s | R$ %.2f\n",
           raiz->codigo, raiz->nome, raiz->preco);
    exibirArvoreEmOrdem(raiz->direita);
}

/* Remove produto da arvore (retorna a nova raiz apos remocao) */
NoArvore* menorNo(NoArvore *no) {
    NoArvore *atual = no;
    while (atual->esquerda != NULL)
        atual = atual->esquerda;
    return atual;
}

NoArvore* removerArvore(NoArvore *raiz, int codigo) {
    if (raiz == NULL) {
        printf("[ARVORE] Codigo %d nao encontrado!\n", codigo);
        return NULL;
    }
    if (codigo < raiz->codigo) {
        raiz->esquerda = removerArvore(raiz->esquerda, codigo);
    } else if (codigo > raiz->codigo) {
        raiz->direita = removerArvore(raiz->direita, codigo);
    } else {
        /* encontrou o no a remover */
        if (raiz->esquerda == NULL) {
            /* caso: sem filho esquerdo */
            NoArvore *tmp = raiz->direita;
            printf("[ARVORE] Produto '%s' removido!\n", raiz->nome);
            free(raiz);
            return tmp;
        } else if (raiz->direita == NULL) {
            /* caso: sem filho direito */
            NoArvore *tmp = raiz->esquerda;
            printf("[ARVORE] Produto '%s' removido!\n", raiz->nome);
            free(raiz);
            return tmp;
        }
        /* caso: dois filhos - substitui pelo menor da subarvore direita */
        NoArvore *substituto = menorNo(raiz->direita);
        raiz->codigo = substituto->codigo;
        strcpy(raiz->nome, substituto->nome);
        raiz->preco = substituto->preco;
        raiz->direita = removerArvore(raiz->direita, substituto->codigo);
    }
    return raiz;
}

/* =====================================================================
 *  FUNCOES AUXILIARES DE MENU
 * ===================================================================== */

void limparBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void popularDadosIniciais() {
    /* Cardapio (Lista Simplesmente Encadeada) */
    inserirCardapio(101, "X-Burguer", 15.90);
    inserirCardapio(102, "X-Salada", 17.50);
    inserirCardapio(103, "Coca-Cola 350ml", 6.00);
    inserirCardapio(104, "Suco de Laranja", 7.50);
    inserirCardapio(105, "Batata Frita P", 9.00);

    /* Arvore Binaria (Catalogo rapido) */
    raizArvore = inserirArvore(raizArvore, 103, "Coca-Cola 350ml", 6.00);
    raizArvore = inserirArvore(raizArvore, 101, "X-Burguer", 15.90);
    raizArvore = inserirArvore(raizArvore, 105, "Batata Frita P", 9.00);
    raizArvore = inserirArvore(raizArvore, 102, "X-Salada", 17.50);
    raizArvore = inserirArvore(raizArvore, 104, "Suco de Laranja", 7.50);

    /* Atendentes (Lista Circular) */
    inserirAtendente("Ana");
    inserirAtendente("Bruno");
    inserirAtendente("Carlos");

    /* Fila inicial de clientes */
    enfileirarCliente("Maria");
    enfileirarCliente("Joao");

    printf("\n[SISTEMA] Dados iniciais carregados com sucesso!\n");
}

/* =====================================================================
 *  MENU PRINCIPAL E SUBMENUS
 * ===================================================================== */

void menuCardapio() {
    int opcao, codigo;
    char nome[MAX_NOME];
    float preco;

    printf("\n--- GERENCIAR CARDAPIO (Lista Simples) ---\n");
    printf("1. Ver cardapio\n");
    printf("2. Adicionar produto\n");
    printf("3. Remover produto\n");
    printf("4. Buscar produto\n");
    printf("0. Voltar\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    limparBuffer();

    switch (opcao) {
        case 1:
            exibirCardapio();
            break;
        case 2:
            printf("Codigo: "); scanf("%d", &codigo); limparBuffer();
            printf("Nome: ");   fgets(nome, MAX_NOME, stdin);
            nome[strcspn(nome, "\n")] = 0; /* remove newline */
            printf("Preco: ");  scanf("%f", &preco); limparBuffer();
            inserirCardapio(codigo, nome, preco);
            break;
        case 3:
            printf("Codigo do produto: "); scanf("%d", &codigo); limparBuffer();
            removerCardapio(codigo);
            break;
        case 4:
            printf("Codigo do produto: "); scanf("%d", &codigo); limparBuffer();
            NoProduto *p = buscarCardapio(codigo);
            if (p) printf("Encontrado: %s - R$ %.2f\n", p->nome, p->preco);
            else   printf("Produto nao encontrado.\n");
            break;
        case 0: break;
        default: printf("Opcao invalida!\n");
    }
}

void menuFila() {
    int opcao;
    char nome[MAX_NOME];

    printf("\n--- FILA DE CLIENTES (Fila/Queue) ---\n");
    printf("1. Ver fila\n");
    printf("2. Adicionar cliente\n");
    printf("3. Atender proximo cliente\n");
    printf("0. Voltar\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    limparBuffer();

    switch (opcao) {
        case 1: exibirFila(); break;
        case 2:
            printf("Nome do cliente: ");
            fgets(nome, MAX_NOME, stdin);
            nome[strcspn(nome, "\n")] = 0;
            enfileirarCliente(nome);
            break;
        case 3: atenderCliente(); break;
        case 0: break;
        default: printf("Opcao invalida!\n");
    }
}

void menuPedidos() {
    int opcao, codigo;

    printf("\n--- PEDIDOS / CARRINHO (Pilha/Stack) ---\n");
    printf("1. Ver carrinho\n");
    printf("2. Adicionar produto ao carrinho\n");
    printf("3. Desfazer ultimo item\n");
    printf("4. Confirmar pedido\n");
    printf("0. Voltar\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    limparBuffer();

    switch (opcao) {
        case 1: exibirPilha(); break;
        case 2:
            exibirCardapio();
            printf("Codigo do produto: "); scanf("%d", &codigo); limparBuffer();
            NoProduto *p = buscarCardapio(codigo);
            if (p) empilharPedido(p->codigo, p->nome, p->preco);
            else   printf("Produto nao encontrado!\n");
            break;
        case 3: desfazerPedido(); break;
        case 4: confirmarPedido(); break;
        case 0: break;
        default: printf("Opcao invalida!\n");
    }
}

void menuVendas() {
    int opcao;

    printf("\n--- HISTORICO DE VENDAS (Lista Dupla) ---\n");
    printf("1. Ver do mais antigo ao mais recente\n");
    printf("2. Ver do mais recente ao mais antigo\n");
    printf("0. Voltar\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    limparBuffer();

    switch (opcao) {
        case 1: exibirVendasAscendente(); break;
        case 2: exibirVendasDescendente(); break;
        case 0: break;
        default: printf("Opcao invalida!\n");
    }
}

void menuArvore() {
    int opcao, codigo;
    char nome[MAX_NOME];
    float preco;

    printf("\n--- CATALOGO RAPIDO (Arvore Binaria) ---\n");
    printf("1. Ver catalogo (em ordem)\n");
    printf("2. Buscar produto por codigo (busca binaria)\n");
    printf("3. Inserir produto no catalogo\n");
    printf("4. Remover produto do catalogo\n");
    printf("0. Voltar\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    limparBuffer();

    switch (opcao) {
        case 1:
            printf("\n===== CATALOGO (Arvore - In-Order) =====\n");
            exibirArvoreEmOrdem(raizArvore);
            printf("=========================================\n");
            break;
        case 2:
            printf("Codigo: "); scanf("%d", &codigo); limparBuffer();
            NoArvore *na = buscarArvore(raizArvore, codigo);
            if (na) printf("Encontrado: %s - R$ %.2f\n", na->nome, na->preco);
            else    printf("Produto nao encontrado na arvore.\n");
            break;
        case 3:
            printf("Codigo: "); scanf("%d", &codigo); limparBuffer();
            printf("Nome: ");   fgets(nome, MAX_NOME, stdin);
            nome[strcspn(nome, "\n")] = 0;
            printf("Preco: ");  scanf("%f", &preco); limparBuffer();
            raizArvore = inserirArvore(raizArvore, codigo, nome, preco);
            printf("[ARVORE] Produto inserido!\n");
            break;
        case 4:
            printf("Codigo: "); scanf("%d", &codigo); limparBuffer();
            raizArvore = removerArvore(raizArvore, codigo);
            break;
        case 0: break;
        default: printf("Opcao invalida!\n");
    }
}

void menuAtendentes() {
    int opcao;
    char nome[MAX_NOME];

    printf("\n--- ESCALA DE ATENDENTES (Lista Circular) ---\n");
    printf("1. Ver escala\n");
    printf("2. Proximo atendente\n");
    printf("3. Adicionar atendente\n");
    printf("0. Voltar\n");
    printf("Opcao: ");
    scanf("%d", &opcao);
    limparBuffer();

    switch (opcao) {
        case 1: exibirAtendentes(); break;
        case 2: proximoAtendente(); break;
        case 3:
            printf("Nome: ");
            fgets(nome, MAX_NOME, stdin);
            nome[strcspn(nome, "\n")] = 0;
            inserirAtendente(nome);
            break;
        case 0: break;
        default: printf("Opcao invalida!\n");
    }
}

/* =====================================================================
 *  FUNCAO PRINCIPAL
 * ===================================================================== */

int main() {
    int opcao;

    printf("=============================================\n");
    printf("   SISTEMA DE LANCHONETE - Estrutura de Dados\n");
    printf("=============================================\n");

    popularDadosIniciais(); /* carrega dados de exemplo */

    do {
        printf("\n========== MENU PRINCIPAL ==========\n");
        printf("1. Gerenciar Cardapio      (Lista Simples)\n");
        printf("2. Fila de Clientes        (Fila/Queue)\n");
        printf("3. Carrinho de Pedidos     (Pilha/Stack)\n");
        printf("4. Historico de Vendas     (Lista Dupla)\n");
        printf("5. Catalogo Rapido         (Arvore Binaria)\n");
        printf("6. Escala de Atendentes    (Lista Circular)\n");
        printf("0. Sair\n");
        printf("=====================================\n");
        printf("Opcao: ");

        if (scanf("%d", &opcao) != 1) {
            printf("Entrada invalida! Digite um numero.\n");
            limparBuffer();
            continue;
        }
        limparBuffer();

        switch (opcao) {
            case 1: menuCardapio();   break;
            case 2: menuFila();       break;
            case 3: menuPedidos();    break;
            case 4: menuVendas();     break;
            case 5: menuArvore();     break;
            case 6: menuAtendentes(); break;
            case 0: printf("Encerrando o sistema. Ate logo!\n"); break;
            default: printf("Opcao invalida! Tente novamente.\n");
        }
    } while (opcao != 0);

    return 0;
}
