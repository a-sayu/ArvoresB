#include <stdio.h>
#include <stdbool.h>

// Struct padrão do nó da árvore.
typedef struct No {
    // vetor dos numeros armazenados
    int *chaves;
    // quantidade de numeros armazenados no nó
    int qtd;
    // indicador de folha
    bool folha;
    // vetor de strings/nomes dos arquivos
    char **filhos;
} NoArvoreB;

// Struct que descreve a árvore.
typedef struct ArvoreB {
    // o primeiro nó
    NoArvoreB* raiz;
    // a ordem da árvore
    int t;
} ArvoreB;

// Essa função inicializa um novo nó.
// . 't' é o grau que define o máximo e mínimo
// de chaves e filhos
// . 'folha' indica se é um nó folha.
NoArvoreB* inicializaNo (int t, bool folha) {
    // Aqui foi feita a criação de um ponteiro para um local
    // alocado na memória para o novo nó.
    NoArvoreB* novo = (NoArvoreB*)malloc(sizeof(NoArvoreB));
    // Foi criado um ponteiro para um lugar alocado na
    // memória apontado para as chaves. Com um número
    // igual ao máximo de de chaves.
    novo->chaves = (int*)malloc((2*t-1)*sizeof(int));
    // Foi criado um ponteiro para um lugar que aloca
    // espaço na memória para ponteiros que podem
    // apontar para arquivos filhos.
    novo->filhos = (char**)malloc(2*t*sizeof(char*));
    // Não há nenhuma chave inicialmente duh.
    novo->qtd = 0;
    // É uma folha por não ter nenhum filho.
    novo->folha = true;
    return novo;
}

// Essa função inicializa uma nova árvore.
// . 't' é o grau da árvore, definindo máximo e
// mínimo de chaves e filhos.
ArvoreB* inicializaArvore(int t) {
    ArvoreB* nova = (ArvoreB*)malloc(sizeof(ArvoreB));
    nova->raiz = inicializaNo(t, true);
    nova->t = t;
    return nova;
}

// Afim de manter o balanceamento da árvore e capacidade
// dos nós, utiliza-se a seguinte função de divisõa de nó
// complementando a função de inserção e busca
// . 'arvore' é o descritor que vai indicar o t;
// . 'pai' é o pai do nó que vai ser dividido.
// . 'filho' é o no que será dividido
// . 'pos' é a posição do filho em relação ao pai
void divideNo(ArvoreB* arvore, NoArvoreB* pai,
NoArvoreB* filho, int pos) {
    // como parâmetro de toda inicialização é preciso
    // indicar a ordem do nó.
    int t = arvore->t;
    // Para lidar com a divisão de um nó, cria-se um
    // novo nó para se ter mais espaço e manter a
    // árvore equilibrada, sendo o irmão do filho
    // passado no parâmetro.
    NoArvoreB* novoFilho = inicializaNo(t, filho->folha);
    // o novo filho irá receber metade das chaves
    // do filho.
    novoFilho->qtd = t - 1;
    for (int i = 0; i < t - 1; i++) {
        // Ele copia a metade da direita do filho para
        // o novo filho
        *(novoFilho->chaves+i) = *(filho->chaves+i+t);
    }
    
}

// Busca

// Inserção

// Remoção

// Impressão

// Armazenamento

int main () {

    return 0;
}