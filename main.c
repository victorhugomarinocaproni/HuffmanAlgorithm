#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Node_de_arvore {
    char byte;
    char *novoBinario;
    int qtdRepeticoes;
    struct Node_de_arvore *esquerda;
    struct Node_de_arvore *direita;
} Node_de_arvore;

typedef struct Node_de_lista_de_prioridade {
    Node_de_arvore *node_de_arvore_pointer;
    struct Node_de_lista_de_prioridade *next_node;
    struct Node_de_lista_de_prioridade *previous_node;
} Node_de_lista_de_prioridade;

typedef struct {
    int qtdElementos;
    Node_de_lista_de_prioridade *primeiro_node;
} Lista_de_prioridade;

static Node_de_arvore *cria_node_de_arvore(unsigned int byte) {
    Node_de_arvore *novo_node = (Node_de_arvore *) malloc(sizeof(Node_de_arvore));
    novo_node->byte = byte;
    novo_node->novoBinario = "0";
    novo_node->qtdRepeticoes = 1;
    novo_node->esquerda = NULL;
    novo_node->direita = NULL;

    return novo_node;
}

void aumenta_qtd_btyes(Node_de_arvore *node) {
    node->qtdRepeticoes += 1;
}

static void cria_nova_lista_de_prioridade(Lista_de_prioridade *lista) {
    lista->qtdElementos = 0;
    lista->primeiro_node = NULL;
}

static Node_de_lista_de_prioridade *
cria_novo_node_da_fila(Node_de_arvore *node_de_arvore_pointer, Node_de_lista_de_prioridade *next_node_da_fila,
                       Node_de_lista_de_prioridade *previous_node_da_fila) {
    Node_de_lista_de_prioridade *novo_node = (Node_de_lista_de_prioridade *) malloc(
        sizeof(Node_de_lista_de_prioridade));

    novo_node->node_de_arvore_pointer = node_de_arvore_pointer;
    novo_node->next_node = next_node_da_fila;
    novo_node->previous_node = previous_node_da_fila;

    return novo_node;
}

static void monta_lista_de_prioridade_ordenada(Lista_de_prioridade *lista, Node_de_arvore *node_de_arvore_pointer) {
    if (lista->primeiro_node == NULL) {
        lista->primeiro_node = cria_novo_node_da_fila(node_de_arvore_pointer, NULL, NULL);
    } else {
        Node_de_lista_de_prioridade *node_atual = lista->primeiro_node;
        Node_de_lista_de_prioridade *node_anterior_ao_atual = NULL;

        while (node_atual != NULL) {
            if (node_atual->node_de_arvore_pointer->qtdRepeticoes > node_de_arvore_pointer->qtdRepeticoes) {
                break;
            }
            node_anterior_ao_atual = node_atual;
            node_atual = node_atual->next_node;
        }

        Node_de_lista_de_prioridade *novo_node = cria_novo_node_da_fila(node_de_arvore_pointer, node_atual,
                                                                        node_anterior_ao_atual);
        if (node_anterior_ao_atual == NULL) {
            lista->primeiro_node = novo_node;
        } else {
            node_anterior_ao_atual->next_node = novo_node;
        }
        if (node_atual != NULL) {
            node_atual->previous_node = novo_node;
        }
    }
}

static void incrementa_nmr_elementos_lista_prioridade(Lista_de_prioridade* lista) {
    lista->qtdElementos += 1;
}

static void monta_huffman_tree(Lista_de_prioridade *lista) {
    while(lista->qtdElementos > 1) {

        Node_de_lista_de_prioridade *node_atual = lista->primeiro_node;

        Node_de_arvore* novo_node_de_arvore_pointer = (Node_de_arvore*)malloc(sizeof(Node_de_arvore));
        int qtdPrimeiroNode = node_atual->node_de_arvore_pointer->qtdRepeticoes;
        int qtdSegundoNode = node_atual->next_node->node_de_arvore_pointer->qtdRepeticoes;

        novo_node_de_arvore_pointer->byte = -1;
        novo_node_de_arvore_pointer->qtdRepeticoes = qtdPrimeiroNode + qtdSegundoNode;
        novo_node_de_arvore_pointer->esquerda = node_atual->node_de_arvore_pointer;
        novo_node_de_arvore_pointer->direita = node_atual->next_node->node_de_arvore_pointer;

        lista->primeiro_node = node_atual->next_node->next_node;

        free(node_atual->next_node);
        free(node_atual);

        lista->qtdElementos -= 1;

        monta_lista_de_prioridade_ordenada(lista, novo_node_de_arvore_pointer);
    }
}

static void printa_arvore_em_pos_ordem(Node_de_arvore* raiz) {
    if(raiz == NULL) return;

    printa_arvore_em_pos_ordem(raiz->esquerda);
    printa_arvore_em_pos_ordem(raiz->direita);

    printf("Byte: %d - Content: %d\n", raiz->byte, raiz->qtdRepeticoes);
}

static void mostra_novos_valores_binarios_de_cada_byte(Node_de_arvore* raiz) { // erro aqui!!!!
    if(raiz == NULL) return;

    mostra_novos_valores_binarios_de_cada_byte(raiz->esquerda);
    mostra_novos_valores_binarios_de_cada_byte(raiz->direita);

    printf("Byte: %d - Novo Binario: %s\n", raiz->byte, raiz->novoBinario);
}

static void mostra_lista_de_prioridade(Lista_de_prioridade *lista) {

    printf("qtdElementos: %d\n", lista->qtdElementos);
    Node_de_lista_de_prioridade *node_atual = lista->primeiro_node;
    int posicao = 1;

    while (node_atual != NULL) {
        printf("Posicao: %d Byte: %d, qtdReps: %d\n",
               posicao,
               node_atual->node_de_arvore_pointer->byte,
               node_atual->node_de_arvore_pointer->qtdRepeticoes);
        node_atual = node_atual->next_node;
        posicao++;
    }
}

static int calcula_altura_da_arvore(Node_de_arvore* raiz) {
    if(raiz == NULL) {
        return 0;
    }

    int alturaEsquerda = calcula_altura_da_arvore(raiz->esquerda);
    int alturaDireita = calcula_altura_da_arvore(raiz->direita);

    if(alturaEsquerda > alturaDireita) {
        return alturaEsquerda += 1;
    }
    else {
        return alturaDireita += 1;
    }

}

static void monta_novos_codigos_binarios(Node_de_arvore* raiz, char *binario_cache) {
    if(raiz == NULL) return;

    if(raiz->esquerda != NULL) {
        strcat(binario_cache, "0");
    }

    monta_novos_codigos_binarios(raiz->esquerda, binario_cache);

    if(raiz->direita != NULL) {
        strcat(binario_cache, "1");
    }

    monta_novos_codigos_binarios(raiz->direita, binario_cache);

    int tamanho = strlen(binario_cache);

    if(raiz->esquerda == NULL && raiz->direita == NULL) {
        raiz->novoBinario = malloc((tamanho+1) * sizeof(char));
        strcpy(raiz->novoBinario, binario_cache);
    }

    binario_cache[tamanho-1] = '\0';

}

static void monta_array_novos_codigos_binarios_montados(Node_de_arvore* raiz, Node_de_arvore *array[]) {
    if(raiz == NULL) return;

    monta_array_novos_codigos_binarios_montados(raiz->esquerda, array);

    monta_array_novos_codigos_binarios_montados(raiz->direita, array);

    if(raiz->esquerda == NULL && raiz->direita == NULL) {
        array[raiz->byte] = raiz;
    }
}


int main(void) {
    FILE *compactFile = fopen("C:\\Users\\victo\\Desktop\\huffman\\file_compressor_and_decompressor\\texto.txt", "rb+");

    if (compactFile == NULL) {
        printf("Erro ao encontrar arquivo para compactar");
    } else {
        unsigned int qtdBytesLidos;
        Node_de_arvore *bytes[256] = {NULL};

        do {
            unsigned char dados[1024];
            qtdBytesLidos = fread(dados, 1, 1024, compactFile);

            for (size_t i = 0; i < qtdBytesLidos; i++) {
                if (dados[i] == '\0') {
                    break;
                }
                unsigned int byte_atual = dados[i];

                if (bytes[byte_atual] == NULL) {
                    bytes[byte_atual] = cria_node_de_arvore(byte_atual);
                } else {
                    aumenta_qtd_btyes(bytes[byte_atual]);
                }
            }
        } while (qtdBytesLidos > 0);

        fclose(compactFile);

        Lista_de_prioridade lista_prioridade;
        cria_nova_lista_de_prioridade(&lista_prioridade);

        for (int i = 0; i < 256; i++) {
            if (bytes[i] != NULL) {
                monta_lista_de_prioridade_ordenada(&lista_prioridade, bytes[i]);
                incrementa_nmr_elementos_lista_prioridade(&lista_prioridade);
            }
        }
        printf("=========================\n");
        mostra_lista_de_prioridade(&lista_prioridade);
        monta_huffman_tree(&lista_prioridade);

        Node_de_arvore* raiz = lista_prioridade.primeiro_node->node_de_arvore_pointer;

        printf("=========================\n");
        printf("Post-order\n");
        printa_arvore_em_pos_ordem(raiz);
        printf("=========================\n");
        printf("Altura da Arvore: %d\n", calcula_altura_da_arvore(raiz));
        printf("=========================\n");

        char binarioCache[calcula_altura_da_arvore(raiz)];
        binarioCache[0] = '\0';

        monta_novos_codigos_binarios(raiz, binarioCache);
        mostra_novos_valores_binarios_de_cada_byte(raiz);
        printf("=========================\n");


        Node_de_arvore* bytes_com_binarios_montados[256] = {NULL};

        monta_array_novos_codigos_binarios_montados(raiz, bytes_com_binarios_montados);


    }
}
