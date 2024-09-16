# include <stdio.h>
# include <stdbool.h>
# include <stdlib.h>


typedef struct no {
    bool folha;
    int quant;
    int* chaves;
    char** filhos;
} NoB;

typedef struct raiz {
    int ordem;
    NoB* primeiro;
} RaizB;

typedef struct encontrado {
    NoB* encontrado;
    int nivel;
    int posicaoVetor;
} EncontradoB;

NoB* inicializarNo(int ordem) {
    NoB* novoNo = (NoB*)malloc(sizeof(NoB));
    if (novoNo != NULL) printf ("alloc ok\n"); //debug

    novoNo->folha = true;
    if (novoNo->folha == true) printf ("folha ok\n"); //debug

    novoNo->chaves = (int*)malloc(sizeof(int));
    if (novoNo->chaves != NULL) printf ("chave ok\n"); //debug

    novoNo->quant = 0;
    if (novoNo->quant == 0) printf ("quant ok\n"); //debug

    novoNo->filhos = (char**)malloc(2*ordem*sizeof(char*));
    if (novoNo->filhos != NULL) printf ("filho ok\n"); //debug
    for (int i = 0; i < 2*ordem; i++) {
        *(novoNo->filhos+i) = NULL;
    }

    return novoNo;
}

RaizB* inicializarRaiz (int t) {
    RaizB* novaRaiz = (RaizB*)malloc(sizeof(RaizB));
    if (novaRaiz != NULL) printf ("1 ok\n"); //debug

    novaRaiz->ordem = t;
    if (novaRaiz->ordem == t) printf ("2 ok\n"); //debug

    novaRaiz->primeiro = inicializarNo(t);
    if (novaRaiz->primeiro != NULL) printf ("3 ok\n"); //debug
}

void escreverRaiz(const char *filename, const RaizB* raiz) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) printf ("1 nao abriu\n");
    else printf ("1 ok\n");
    fwrite(&raiz, sizeof(RaizB), 1, file);
    fclose(file);
}

void lerRaiz(const char *filename, RaizB* raiz) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) printf ("1 nao abriu\n");
    else printf ("1 ok\n");
    fread(raiz, sizeof(RaizB), 1, file);
    fclose(file);
}
void escreverNo(const char *filename, const NoB* noEsc) {
    FILE *file = fopen(filename, "wb");
    if (file == NULL) printf ("1 nao abriu\n");
    else printf ("1 ok\n");
    fwrite(&noEsc, sizeof(NoB), 1, file);
    fclose(file);
}

void lerNo(const char *filename, NoB* noLid) {
    FILE *file = fopen(filename, "rb");
    if (file == NULL) printf ("1 nao abriu\n");
    else printf ("1 ok\n");
    fread(noLid, sizeof(NoB), 1, file);
    fclose(file);
}

void dividirNo(NoB* pai, NoB* filho, int t) {
    int meio = t - 1;
    NoB* novoNo = inicializarNo(t);
    pai->quant = meio;

    for (int i = 0; i < meio; i++) {
        *(novoNo->chaves+i) = *(filho->chaves+meio+i+1);
    }
    if (!filho->folha) {
        for (int i = 0; i < t; i++) {
            *(novoNo->filhos+i) = *(filho->filhos+meio+i+1);
        }
    }
    novoNo->quant = meio;

    for (int i = pai->quant - 1; i >= 0 && *(pai->chaves+i) > *(filho->chaves+meio); i--) {
        *(pai->chaves+i+1) = *(pai->chaves+i);
    }
}

EncontradoB* buscarNo(NoB* NoObs, int t, int elemento) {
    int i = 0, qntElem = NoObs->quant;
    EncontradoB* enc = (EncontradoB*)malloc(sizeof(EncontradoB));

    while (i <= qntElem && elemento > *(NoObs->chaves+i)) {
        i++;
    }
    // sair do while significa que ou não há mais o que
    // percorrer, ou que o elemento é menor que o elemento
    // observado.
    if (i <= qntElem && elemento == *(NoObs->chaves+i)) {
        // Encontrou!
        enc->encontrado = NoObs;
        enc->nivel = encontrarNivel(NoObs);
        enc->posicaoVetor = i;
        return enc;
    }
    if (NoObs->folha) {
        // Fim da linha, o elemento não está na árvore.
        return NULL;
    }
    else {
        NoB* filhoLido = (NoB*)malloc(sizeof(NoB));
        lerNo(NoObs->filhos+i, filhoLido);
        enc = buscarNo(filhoLido, t, elemento);
        free(filhoLido);
        return enc;
    }
}

void inserirNo(NoB* no, int t, int elemento) {
    int qtdNo = no->quant;
    // primeiro, o nó está vazio?
    if (qtdNo == 0) {
        // se sim, adicionar o elemento direto!
        *(no->chaves+qtdNo) = elemento;
        no->quant++;
    }
    // se não:
    // verificar se está no limite!
    if (qtdNo == 2*t-1) {
        // se sim, tem que fazer o split
        
    }
    // se não:
    // verificar se
}


void removerNo() {}

int main () {
    int t = 0;
    printf("Num: ");
    scanf ("%d", &t);
    RaizB* raizA = inicializarRaiz(t);
    return 0;
}