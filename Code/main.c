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
    if (novo == NULL) {
        printf("Erro ao alocar memória\n");
    }

    // Foi criado um ponteiro para um lugar alocado na
    // memória apontado para as chaves. Com um número
    // igual ao máximo de de chaves.
    novo->chaves = (int*)malloc((2*t-1)*sizeof(int));
    if (novo->chaves == NULL) {
        printf("Erro ao alocar memória\n");
    }

    // Foi criado um ponteiro para um lugar que aloca
    // espaço na memória para ponteiros que podem
    // apontar para arquivos filhos.
    novo->filhos = (char**)malloc(2*t*sizeof(char*));
    if (novo->filhos == NULL) {
        printf("Erro ao alocar memória\n");
    }

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
    if (nova == NULL) {
        printf("Erro ao alocar memória\n");
    }
    nova->raiz = inicializaNo(t, true);
    nova->t = t;
    return nova;
}

// Função que verifica se o nó está cheio
// passado o paarâmetros do nó a ser checado
// e a ordem da árvore.
bool estaCheio(NoArvoreB* no, int t) {
    return no->qtd == (2 * t - 1);
}

// BUSCA

// Função  para buscar uma chave a partir de um
// nó específico
// Parâmetros: o nó e o elemento a ser buscado.
// Apenas indica se está presente.
bool buscaNo(NoArvoreB* no, int busca) {
    int i;

    // Iniciando em zero, enquanto o 'i' for menor que a
    // quantidade de chaves no nó, e o elemento 'busca'
    // for maior que o elemento na posição 'i' no array
    // Continuar avançando pelo array, até uma das
    // situações forem falsas.
    for (i = 0; i < no->qtd && busca > no->chaves[i]; i++);

    // Ao finalizar o for, se o i for menor que a quantidade
    // de chaves no nó, ou seja o que falhou foi o elemento
    // 'busca' ser maior que o elemento na posição 'i', 
    // então a primeira pergunta é:
    // "Esse é o elemento buscado?"
    if (i < no->qtd && busca == no->chaves[i]) {
        // Se sim, a busca foi concluída.
        return true; 
    }
    // Se não, e o nó for uma folha.
    else if (no->folha) {
        // Logo esse elemento não está na árvore.
        return false;
    }
    // Se ele não for uma folha, então o filho
    // dessa posição pode ter em seu array
    // o elemento 'busca'. 
    // PS: acho que toda vez que faz uma busca
    // nos filhos, significa que aumenta em um
    // o nível, talvez seja útil no futuro.
    else return buscaNo(no->filhos[i], busca);
}

// Como utilizamos um descritor, essa função serve para
// automaticamente já nos dirigir para a raiz.
bool buscaArvore(ArvoreB* arvore, int chave) {
    return buscaNo(arvore->raiz, chave);
}

// Inserção

// Remoção

// Impressão

// Armazenamento

int main () {

    return 0;
}