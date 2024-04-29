#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    unsigned int byte;
    int qtdRepeticoes;
} Node_de_frequencia;

typedef struct Node_de_lista_de_prioridade
{
    Node_de_frequencia *node_de_frequencia_pointer;
    struct Node_de_lista_de_prioridade *next_node;
    struct Node_de_lista_de_prioridade *previous_node;

} Node_de_lista_de_prioridade;

typedef struct
{
    Node_de_lista_de_prioridade * primeiro_node;
} Lista_de_prioridade;

typedef struct Node_arvore_huffman
{
    int somaNodes;
    void* esquerda;
    void* direita;
} Node_arvore_huffman;


static Node_de_frequencia* cria_node_de_frequencia(unsigned int byte) {
    Node_de_frequencia* novo_node = (Node_de_frequencia*) malloc(sizeof(Node_de_frequencia));
    novo_node->byte = byte;
    novo_node->qtdRepeticoes = 1;

    return novo_node;
}

void aumenta_qtd_btyes(Node_de_frequencia* node) {
    node->qtdRepeticoes += 1;
}

static void cria_nova_lista_de_prioridade(Lista_de_prioridade* lista) {
    lista->primeiro_node = NULL;
}

static Node_de_lista_de_prioridade*
cria_novo_node_da_fila(Node_de_frequencia* node_frequencia_pointer, Node_de_lista_de_prioridade* next_node_da_fila,
                       Node_de_lista_de_prioridade* previous_node_da_fila) {
    Node_de_lista_de_prioridade* novo_node = (Node_de_lista_de_prioridade*) malloc(
            sizeof(Node_de_lista_de_prioridade));

    novo_node->node_de_frequencia_pointer = node_frequencia_pointer;
    novo_node->next_node = next_node_da_fila;
    novo_node->previous_node = previous_node_da_fila;

    return novo_node;
}

static void monta_lista_de_prioridade_ordenada(Lista_de_prioridade* lista, Node_de_frequencia* node_de_frequencia_pointer) {

    if (lista->primeiro_node == NULL)
    {
        lista->primeiro_node = cria_novo_node_da_fila(node_de_frequencia_pointer, NULL, NULL);
        return;
    }
    else
    {
        Node_de_lista_de_prioridade* node_atual = lista->primeiro_node;
        Node_de_lista_de_prioridade* node_anterior_ao_atual = NULL;

        while (node_atual != NULL) {
            if (node_atual->node_de_frequencia_pointer->qtdRepeticoes > node_de_frequencia_pointer->qtdRepeticoes) {
                break;
            }
            node_anterior_ao_atual = node_atual;
            node_atual = node_atual->next_node;
        }

        Node_de_lista_de_prioridade *novo_node = cria_novo_node_da_fila(node_de_frequencia_pointer, node_atual, node_anterior_ao_atual);
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

static void mostra_lista_de_prioridade(Lista_de_prioridade* lista) {
    Node_de_lista_de_prioridade* node_atual = lista->primeiro_node;

    while (node_atual != NULL) {
        printf("Byte: %d, qtdReps: %d\n", node_atual->node_de_frequencia_pointer->byte,
               node_atual->node_de_frequencia_pointer->qtdRepeticoes);
        node_atual = node_atual->next_node;
    }
}

int main(void) {

    FILE *compactFile = fopen("C:\\Users\\VH-working\\Desktop\\huffman\\file_compressor_and_decompressor\\teste.txt", "rb+");

    if (compactFile == NULL) {
        printf("Erro ao encontrar arquivo para compactar");
    } else {
        unsigned int qtdBytesLidos;
        unsigned char dados[1024];
        Node_de_frequencia *bytes[256] = {NULL};

        do {
            qtdBytesLidos = fread(dados, 1, 1024, compactFile);

            for (size_t i = 0; i < qtdBytesLidos; i++) {
                if (dados[i] == '\0') {
                    break;
                }
                unsigned int byte_atual = dados[i];

                if (bytes[byte_atual] == NULL) {
                    bytes[byte_atual] = cria_node_de_frequencia(byte_atual);
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
            }
        }

        mostra_lista_de_prioridade(&lista_prioridade);
    }
}
