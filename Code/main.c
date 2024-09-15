#include <stdio.h>
#include <stdbool.h>

// Struct padrão do nó da árvore.
typedef struct No {
    char **pai;
    // Vetor dos numeros armazenados
    int *chaves;
    // Quantidade de numeros armazenados no nó
    int qtd;
    // Indicador de folha
    bool folha;
    // Vetor de strings/nomes dos arquivos
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

    novo->pai = (char**)malloc(2*t*sizeof(char*));
    if (novo->pai == NULL) {
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
bool buscaNo(NoArvoreB* no, int elemento) {
    int i;

    // Iniciando em zero, enquanto o 'i' for menor que a
    // quantidade de chaves no nó, e o elemento 'elemento'
    // for maior que o elemento na posição 'i' no array
    // Continuar avançando pelo array, até uma das
    // situações forem falsas.
    for (i = 0; i < no->qtd && elemento > no->chaves[i];
    i++);

    // Ao finalizar o for, se o i for menor que a quantidade
    // de chaves no nó, ou seja o que falhou foi o elemento
    // 'elemento' ser maior que o elemento na posição 'i', 
    // então a primeira pergunta é:
    // "Esse é o elemento buscado?"
    if (i < no->qtd && elemento == no->chaves[i]) {
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
    // o elemento 'elemento'. 
    // PS: acho que toda vez que faz uma busca
    // nos filhos, significa que aumenta em um
    // o nível, talvez seja útil no futuro.
    else return buscaNo(no->filhos[i], elemento);
}

// Como utilizamos um descritor, essa função serve para
// automaticamente já nos dirigir para a raiz.
bool buscaArvore(ArvoreB* arvore, int elemento) {
    return buscaNo(arvore->raiz, elemento);
}

// Divisão
// O filho vai ter um no menor e um no maior, e o
// do meio irá subir para cima.
void dividirNo(ArvoreB* arvore, int pos,
NoArvoreB* filhoCheio) {
    int t = arvore->t;
    NoArvoreB* pai = filhoCheio->pai;
    NoArvoreB* novoFilho = inicializaNo(t, filhoCheio->folha);
    novoFilho->qtd = t - 1;

    for (int i = 0; i < t - 1; i++) {
        *(novoFilho->chaves+i) = *(filhoCheio->chaves+i+t);
    }
    
    if (!filhoCheio->folha) {
        for (int i = 0; i < t; i++) {
            *(novoFilho->filhos+i) = *(filhoCheio->filhos+i+t);
        }
    }

    filhoCheio->qtd = t - 1;

    if (!estaCheio(pai, t)) {
        for (int i = pai->qtd; i >= pos+1; i--) {
            *(pai->filhos+i+1) = *(pai->filhos+i);
        }

        *(pai->filhos+pos+1) = novoFilho;

        for (int i = pai->qtd - 1; i >= pos; i--) {
            *(pai->chaves+i+1) = *(pai->chaves+i);
        }

        *(pai->chaves+pos) = *(filhoCheio->chaves+t-1);

        pai->qtd++;

    } else {
        dividirNo(arvore, pos, pai);
    }
    novoFilho->pai = filhoCheio->pai;
}

// Inserção

// Função de Inserção do No na árvore.
void inserirNo(ArvoreB* arvore, NoArvoreB* no, int elemento) {
    int t = arvore->t;
    // Começa do final para facilitar o deslocamento.
    int pos = no->qtd-1;
    // Checa se está cheio para não ocorrer de
    // ultrapassar o máximo.
    bool cheio = estaCheio(no, t);

    // Se o no estiver cheio, não seria possivel realizar
    // nada enquanto ele não diminui de tamanho.
    if (!cheio) {
        // Se o nó não tiver filhos/folha, deve ser encontrado
        // onde ele deve ser inserido. Ou seja, enquanto o
        // elemento for inferior ao elemento da chave, diminui
        // para encontrar um ponto em que ele é maior.
        if (no->folha) {
            for (pos; pos >= 0
            && elemento < *(no->chaves+pos); pos--) {
                // Dessa forma, move-se o elemento para o
                // lado.
                *(no->chaves+pos+1) = *(no->chaves+pos);
            }
        }
        // Se o nó tiver filhos/não é folha, deve ser
        // encontrado o local em que o elemento é o menor
        // diminuindo a posição.
        else {
            for (pos; pos >= 0
            && elemento < *(no->chaves+pos); pos--);
            pos++;
            inserirNo(*(no->filhos+pos), elemento, t);
        }
    } else {
        if (no->folha) {
            dividirNo(arvore, pos, no);
        } else {
            dividirNo(arvore, pos, *(no->filhos+pos));
            if (elemento > *(no->chaves+pos)) {
                pos++;
            }
        }
        inserirNo(arvore, *(no->filhos+pos), elemento);
    }
}

// Remoção

// Impressão

// Armazenamento

int main () {

    return 0;
}