#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

// Estrutura do no da arvore B
typedef struct noArvoreB {
    // Quantidade de Chaves presentes no Nó
    int qtdChaves;
    // Vetor das Chaves
    int* chaves;
    // Indicador de Folha
    bool folha;
    // Vetor de ponteiros para os filhos
    struct noArvoreB** filhos;
} noB;

// Estrutura da árvore B, com a raiz, ou seja, o primeiro nó e a
// ordem da árvore
typedef struct noRaiz {
    noB* raiz;
    int ordem;
} arvoreB;

// Função que inicializa um nó da árvore
noB* inicializarNo(int t, bool folha) {
    noB* no = (noB*)malloc(sizeof(noB));
    // Define como folha
    no->folha = folha;
    // Como é um nó novo, não tem nenhuma chave
    no->qtdChaves = 0;
    // Aloca memória para 2t-1 chaves
    no->chaves = (int*)malloc((2*t-1)*sizeof(int));
    // Aloca memória para 2t filhos
    no->filhos = (noB**)malloc(2*t*sizeof(noB*));
    return no;
}

// Função que inicializa a árvore
arvoreB* inicializaArvore(int t) {
    arvoreB* arvore = (arvoreB*)malloc(sizeof(arvoreB));
    // defina a ordem da arvore
    arvore->ordem = t;
    // inicializa a raiz, como uma folha
    arvore->raiz = inicializarNo(t, true);
    return arvore;
}

// Função de busca por nós na árvore
noB* buscar(noB* raiz, int elem) {
    int i = 0;
    // Percorre o nó até ultima posição ou até encontrar uma
    // chave menor que o elemento
    while (i < raiz->qtdChaves && elem > raiz->chaves[i]) {
        i++;
    }
    if (i < raiz->qtdChaves && raiz->chaves[i] == elem) {
        return raiz; // encontrou
    }
    if (raiz->folha) {
        return NULL; // não tem
    }
    return buscar(raiz->filhos[i], elem); // nó não é folha
}

// Função para dividir o filho quando ele está cheio
void dividirFilho(noB* no, int pos, int t) {
    noB* novoFilho = inicializarNo(t, no->filhos[pos]->folha);
    noB* filho = no->filhos[pos]; // no que irá ser dividido
    novoFilho->qtdChaves = t-1; // metade das chaves
    
    // move as ultimas t-1 chaves para o novoFilho
    for (int i = 0; i < t-1; i++) {
        novoFilho->chaves[i] = filho->chaves[i+t];
    }

    // se o nó não for folha ele move os filhos para a
    // nova posição.
    if (!filho->folha) {
        for (int j = 0; j < t; j++) {
            novoFilho->filhos[j] = filho->filhos[j+t];
        }
    }
    // Atualiza o número de chaves no nó original.
    filho->qtdChaves = t-1;

    // Move os filhos do nó original para abrir
    // espaço para o novo filho
    for (int i = no->qtdChaves; i >= pos+1; i--) {
        no->filhos[i+1] = no->filhos[i];
    }
    
    // coloca o novo filho na posicao correta
    no->filhos[pos+1] = novoFilho;

    // move as chaves do nó original para
    // abrir espaço para a nova chave
    for (int i = no->qtdChaves-1; i >= pos; i--) {
        no->chaves[i+1] = no->chaves[i];
    }

    // Sobe a chave do meio.
    no->chaves[pos] = filho->chaves[t-1];

    // Atualiza a quantidade de chaves no nó.
    no->qtdChaves++;
}

// Função para inserir uma chave em um nó não cheio.
void inserirNaoCheio(noB* no, int elem, int t) {
    int i = no->qtdChaves-1;

    // Se o nó é folha, insere a chave diretamente
    if (no->folha) {
        while(i >= 0 && no->chaves[i] > elem) {
            no->chaves[i+1] = no->chaves[i];
            i--;
        }
        no->chaves[i+1] = elem;
        no->qtdChaves++;
    } else {
        // Se não é folha, encontra o filho que
        // a chave deve ser inserida
        while (i >= 0 && no->chaves[i] > elem) {
            i--;
        }
        i++;
        // Se o filho onde a chave deve ser
        // inserida está cheio, divide
        if (no->filhos[i]->qtdChaves == 2*t-1) {
            dividirFilho(no, i, t);
            if (no->chaves[i] < elem) {
                i++;
            }
        }
         // Insere recursivamente no filho
        inserirNaoCheio(no->filhos[i], elem, t);
    }
}

// Função para inserir uma chave na árvore B.
void inserir(arvoreB* arvore, int elem) {
    noB* raiz = arvore->raiz;
    int t = arvore->ordem;

    // Verifica se está cheia
    if (raiz->qtdChaves == 2*t-1) {
        // cria novo nó para ser a raiz
        noB* novoNo = inicializarNo(t, false);
        novoNo->filhos[0] = raiz;

        // divide a raiz e coloca a nova raiz
        dividirFilho(novoNo, 0, t);
        arvore->raiz = novoNo;

        inserirNaoCheio(novoNo, elem, t);
    } else {
        inserirNaoCheio(raiz, elem, t);
    }
}

void remover(arvoreB* arvore, int elem);
void removerDeNo(noB* no, int elem, int t);
int obterPredecessor(noB* no);
int obterSucessor(noB* no);
void fundir(noB* no, int pos, int t);
void emprestarEsquerda(noB* no, int pos);
void emprestarDireita(noB* no, int pos);
void removerDeFolha(noB* no, int pos);
void removerDeInterno(noB* no, int pos, int t);

void emprestarDireita (noB* no, int pos) {
     noB* filho = no->filhos[pos];
    noB* irmao = no->filhos[pos+1];

    filho->chaves[filho->qtdChaves] = no->chaves[pos];

    if (!filho->folha) {
        filho->filhos[filho->qtdChaves+1] = irmao->filhos[0];
    }

    no->chaves[pos] = irmao->chaves[0];

    for (int i = 1; i < irmao->qtdChaves; i++) {
        irmao->chaves[i-1] = irmao->chaves[i];
    }

    if (!irmao->folha) {
        for (int i = 1; i <= irmao->qtdChaves; i++) {
            irmao->filhos[i-1] = irmao->filhos[i];
        }
    }

    filho->qtdChaves++;
    irmao->qtdChaves--;
}

void emprestarEsquerda (noB* no, int pos) {
    noB* filho = no->filhos[pos];
    noB* irmao = no->filhos[pos-1];

    for (int i = filho->qtdChaves-1; i >= 0; i--) {
        filho->chaves[i+1] = filho->chaves[i];
    }

    if (!filho->folha) {
        for (int i = filho->qtdChaves; i >= 0; i--) {
            filho->filhos[i+1] = filho->filhos[i];
        }
    }

    filho->chaves[0] = no->chaves[pos-1];

    if (!filho->folha) {
        filho->filhos[0] = irmao->filhos[irmao->qtdChaves];
    }

    no->chaves[pos-1] = irmao->chaves[irmao->qtdChaves-1];
    filho->qtdChaves++;
    irmao->qtdChaves--;
}

void fundir (noB* no, int pos, int t) {
    noB* filho = no->filhos[pos];
    noB* irmao = no->filhos[pos+1];

    filho->chaves[t-1] = no->chaves[pos];

    for (int i = 0; i < irmao->qtdChaves; i++) {
        filho->chaves[i+t] = irmao->chaves[i];
    }

    if (!filho->folha) {
        for (int i = 0; i <= irmao->qtdChaves; i++) {
            filho->filhos[i+t] = irmao->filhos[i];
        }
    }

    for (int i = pos+1; i < no->qtdChaves; i++) {
        no->chaves[i-1] = no->chaves[i];
    }

    for (int i = pos+2; i <= no->qtdChaves; i++) {
        no->filhos[i-1] = no->filhos[i];
    }

    filho->qtdChaves += irmao->qtdChaves + 1;
    no->qtdChaves--;

    // libera o irmão
    free(irmao);
}

int obterAnterior(noB* no) {
    while (!no->folha) {
        no = no->filhos[no->qtdChaves];
    }
    return no->chaves[no->qtdChaves-1];
}

int obterProximo(noB* no) {
    while (!no->folha) {
        no = no->filhos[0];
    }
    return no->chaves[0];
}

void removerDeInterno (noB* no, int pos, int t) {
    int elem = no->chaves[pos];
    if (no->filhos[pos]->qtdChaves >= t) {
        int ant = obterAnterior(no->filhos[pos]);
        no->chaves[pos] = ant;
        removerDeNo(no->filhos[pos], ant, t);
    } else if (no->filhos[pos+1]->qtdChaves >= t) {
        int prox = obterProximo(no->filhos[pos+1]);
        no->chaves[pos] = prox;
        removerDeNo(no->filhos[pos+1], prox, t);
    } else {
        fundir(no, pos, t);
        removerDeNo(no->filhos[pos], elem, t);
    }
}

void removerDeFolha (noB* no, int pos) {
    for (int i = pos+1; i < no->qtdChaves; i++) {
        no->chaves[i-1] = no->chaves[i];
    }
    no->qtdChaves--;
}

void removerDeNo(noB* no, int elem, int t) {
    int pos = 0;
    while (pos < no->qtdChaves && no->chaves[pos] < elem) {
        pos++;
    }
    if (pos < no->qtdChaves && no->chaves[pos] == elem) {
        // Se o elemento a ser eliminado está no nó
        if (no->folha) {
            // remove a folha diretamente
            removerDeFolha(no, pos);
        } else {
            // remove de um nó interno
            removerDeInterno(no, pos, t);
        }
    } else {
        if (no->folha) {
            printf ("Chave %d nao encontrada na arvore.\n", elem);
            return;
        }

        if (no->filhos[pos]->qtdChaves < t) {
            if (pos != 0 && no->filhos[pos-1]->qtdChaves >= t) {
                emprestarEsquerda(no, pos);
            } else if (pos != no->qtdChaves && no->filhos[pos+1]->qtdChaves >= t) {
                emprestarDireita(no, pos);
            } else {
                if (pos != no->qtdChaves) {
                    fundir(no, pos, t);
                } else {
                    fundir(no, pos-1, t);
                }
            }
        }

        if (pos < no->qtdChaves) {
            removerDeNo(no->filhos[pos], elem, t);
        }
    }
}

void remover(arvoreB* arvore, int elem) {
    if(!arvore->raiz) {
        printf ("Arvore Nula\n");
        return;
    }
    removerDeNo(arvore->raiz, elem, arvore->ordem);

    if (arvore->raiz->qtdChaves == 0) {
        noB* no = arvore->raiz;
        if (arvore->raiz->folha) {
            arvore->raiz = NULL;
        } else {
            arvore->raiz = arvore->raiz->filhos[0];
        }
        free(no);
    }
}

void liberarNo(noB* no, int t) {
    if (!no->folha) {
        for (int i = 0; i <= no->qtdChaves; i++) {
            liberarNo(no->filhos[i], t);
        }
    }
    free(no->chaves);
    free(no->filhos);
    free(no);
}

void liberarArvore(arvoreB* arvore) {
    liberarNo(arvore->raiz, arvore->ordem);
    free(arvore);
}

void imprimirNo(noB* no, int nivel, int t) {
    if (no == NULL) return;
    for (int i = 0; i < nivel; i++) {
        printf("  ");
    }
    printf("[");
    for (int i = 0; i < no->qtdChaves; i++) {
        printf("%d ", no->chaves[i]);
    }
    printf("]");
    if (!no->folha) {
        for (int i = 0; i <= no->qtdChaves; i++) {
            imprimirNo(no->filhos[i], nivel + 1, t);
        }
    }
}

void imprimirArvore (arvoreB* arvore) {
    imprimirNo(arvore->raiz, 0, arvore->ordem);
}



int main() {
    int t;  // Ordem mínima da árvore B
    printf ("Digite a ordem: ");
    scanf("%d", &t);
    arvoreB* arvore = inicializaArvore(t);

    // Inserindo chaves na árvore B
    int chaves[] = {10, 20, 5, 6, 12, 30, 7, 17, 100, 80, 50, 30, 16, 23};
    printf("Inserindo chaves: ");
    for (int i = 0; i < sizeof(chaves)/sizeof(chaves[0]); i++) {
        inserir(arvore, chaves[i]);
        printf("%d ", chaves[i]);
    }
    printf("\n");
    imprimirArvore(arvore);
    // Testando busca
    int m = 6;
    printf("Buscando a chave %d: ", m);
    if (buscar(arvore->raiz, m) != NULL) 
        printf("Presente\n");
    else 
        printf("Nao Presente\n");

    m = 15;
    printf("Buscando a chave %d: ", m);
    if (buscar(arvore->raiz, m) != NULL) 
        printf("Presente\n");
    else 
        printf("Nao Presente\n");

    // Testando remoção
    printf("Removendo a chave 6\n");
    remover(arvore, 6);
    imprimirArvore(arvore);
    printf("Buscando a chave 6 apos remocao: ");
    if (buscar(arvore->raiz, 6) != NULL) 
        printf("Presente\n");
    else 
        printf("Nao Presente\n");

    printf("Removendo a chave 13 (nao existe)\n");
    remover(arvore, 13);

    printf("Removendo a chave 7\n");
    remover(arvore, 7);
    imprimirArvore(arvore);
    printf("Buscando a chave 7 apos remocao: ");
    if (buscar(arvore->raiz, 7) != NULL) 
        printf("Presente\n");
    else 
        printf("Nao Presente\n");

    imprimirArvore(arvore);

    liberarArvore(arvore);
    return 0;
}