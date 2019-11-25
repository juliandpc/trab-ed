#include <stdio.h>
#include <stdlib.h>
#include <string.h>

const int ordem = 3;

int saida_mais_infos = 0;

typedef struct letra {
    char let;
} letra;

typedef struct ArvBM {
    void ** ponteiros;
    char *chave;
    struct ArvBM * pai;
    int folha;
    int nchaves;
    struct ArvBM * prox;
} TABM;

letra *cria_letra(char let);
TABM *cria_nodo(void);
TABM *cria_folha(void);
int pega_indice_esquerdo(TABM *pai, TABM *esq);
TABM *insere_na_folha(TABM *folha, char ch, letra *ponteiro);
TABM *inserir_na_folha_apos_divisao(TABM *arv, TABM *folha, char ch, letra *ponteiro);
TABM *inserir_in_nodo(TABM *arv, TABM *n,
                      int indice_esquerdo, char ch, TABM *dir);
TABM *inserir_in_nodo_apos_divisao(TABM *arv, TABM *antigo_nodo, int indice_esquerdo, char ch, TABM *dir);
TABM *inserir_in_pai(TABM *arv, TABM *esq, char ch, TABM *dir);
TABM *inserir_em_nova_raiz(TABM *esq, char ch, TABM *dir);
TABM *cria_nova_arvore(char ch, letra *ponteiro);
TABM *inserir(TABM *arv, char ch, char let);
int calculaAltura(TABM * const raiz);

TABM *fila = NULL;

// INSERTION

/* Creates a new record to hold the value
 * to which a key refers.
 */
//ESSE EH O MAKE RECORD
letra *cria_letra(char let) {
    letra *nova_letra = (letra*)malloc(sizeof(letra));
    if (nova_letra == NULL) {
        return NULL;
    }
    else {
        nova_letra->let = let;
    }
    return nova_letra;
}


/* Creates a new general node, which can be adapted
 * to serve as either a leaf or an internal node.
 */
//ESSE EH O MAKE NODE
TABM *cria_nodo(void) {
    TABM *novo_nodo;
    novo_nodo = malloc(sizeof(TABM));
    if (novo_nodo == NULL) {
        return NULL;
    }
    novo_nodo->chave = malloc((ordem - 1) * sizeof(char));
    if (novo_nodo->chave == NULL) {
        return NULL;
    }
    novo_nodo->ponteiros = malloc(ordem * sizeof(void *));
    if (novo_nodo->ponteiros == NULL) {
        return NULL;
    }
    novo_nodo->folha = 0;
    novo_nodo->nchaves = 0;
    novo_nodo->pai = NULL;
    novo_nodo->prox = NULL;
    return novo_nodo;
}

/* Creates a new leaf by creating a node
 * and then adapting it appropriately.
 */
//ESSE EH O MAKE LEAF
TABM *cria_folha(void) {
    TABM *folha = cria_nodo();
    folha->folha = 1;
    return folha;
}

TABM *busca_folha(TABM *const arv, char ch, int flag) {
    if (arv == NULL) {
        if (flag)
            printf("Arvore vazia.\n");
        return arv;
    }
    int i = 0;
    TABM *c = arv;
    while (!c->folha) {
        if (flag) {
            printf("[");
            for (i = 0; i < c->nchaves - 1; i++)
                printf("%c ", c->chave[i]);
            printf("%c] ", c->chave[i]);
        }
        i = 0;
        while (i < c->nchaves) {
            if (ch >= c->chave[i]) i++;
            else break;
        }
        if (flag)
            printf("%c ->\n", i);
        c = (TABM *)c->ponteiros[i];
    }
    if (flag) {
        printf("Folha [");
        for (i = 0; i < c->nchaves - 1; i++)
            printf("%c ", c->chave[i]);
        printf("%c] ->\n", c->chave[i]);
    }
    return c;
}

letra *busca(TABM *arv, char ch, int flag, TABM **folha_fora) {
    if (arv == NULL) {
        if (folha_fora != NULL) {
            *folha_fora = NULL;
        }
        return NULL;
    }

    int i = 0;
    TABM *folha = NULL;

    folha = busca_folha(arv, ch, flag);

    /* If root != NULL, leaf must have a value, even
     * if it does not contain the desired key.
     * (The leaf holds the range of keys that would
     * include the desired key.)
     */

    for (i = 0; i < folha->nchaves; i++)
        if (folha->chave[i] == ch) break;
    if (folha_fora != NULL) {
        *folha_fora = folha;
    }
    if (i == folha->nchaves)
        return NULL;
    else
        return (letra *)folha->ponteiros[i];
}

int corta(int tamanho) {
    if (tamanho % 2 == 0)
        return tamanho/2;
    else
        return tamanho/2 + 1;
}

/* Helper function used in insert_into_parent
 * to find the index of the parent's pointer to
 * the node to the left of the key to be inserted.
 */
int pega_indice_esquerdo(TABM *pai, TABM *esq) {
    int indice_esquerdo = 0;
    while (indice_esquerdo <= pai->nchaves && pai->ponteiros[indice_esquerdo] != esq)
        indice_esquerdo++;
    return indice_esquerdo;
}

/* Inserts a new pointer to a record and its corresponding
 * key into a leaf.
 * Returns the altered leaf.
 */
//ESSE EH O insert_into_leaf
TABM *insere_na_folha(TABM *folha, char ch, letra *ponteiro) {

    int i, posicao_da_insercao;
    posicao_da_insercao = 0;

    while (posicao_da_insercao < folha->nchaves && folha->chave[posicao_da_insercao] < ch)
        posicao_da_insercao++;

    for (i = folha->nchaves; i > posicao_da_insercao; i--) {
        folha->chave[i] = folha->chave[i - 1];
        folha->ponteiros[i] = folha->ponteiros[i - 1];
    }
    folha->chave[posicao_da_insercao] = ch;
    folha->ponteiros[posicao_da_insercao] = ponteiro;
    folha->nchaves++;
    return folha;
}


/* Inserts a new key and pointer
 * to a new record into a leaf so as to exceed
 * the tree's order, causing the leaf to be split
 * in half.
 */
//ESSE EH O insert_into_leaf_after_splitting
TABM *inserir_na_folha_apos_divisao(TABM *arv, TABM *folha, char ch, letra *ponteiro) {

    TABM *nova_folha;
    char *temp_chave;
    void **temp_ponteiro;
    int insercao_indice, divide, i, j;
    char nova_chave;

    nova_folha = cria_folha();
    temp_chave = malloc(ordem * sizeof(char));

    if (temp_chave == NULL) {
        return NULL;
    }

    temp_ponteiro = malloc(ordem * sizeof(void *));
    if (temp_ponteiro == NULL) {
        return NULL;
    }

    insercao_indice = 0;
    while (insercao_indice < ordem - 1 && folha->chave[insercao_indice] < ch)
        insercao_indice++;

    for (i = 0, j = 0; i < folha->nchaves; i++, j++) {
        if (j == insercao_indice) j++;
        temp_chave[j] = folha->chave[i];
        temp_ponteiro[j] = folha->ponteiros[i];
    }

    temp_chave[insercao_indice] = ch;
    temp_ponteiro[insercao_indice] = ponteiro;

    folha->nchaves = 0;

    divide = corta(ordem - 1);

    for (i = 0; i < divide; i++) {
        folha->ponteiros[i] = temp_ponteiro[i];
        folha->chave[i] = temp_chave[i];
        folha->nchaves++;
    }

    for (i = divide, j = 0; i < ordem; i++, j++) {
        nova_folha->ponteiros[j] = temp_ponteiro[i];
        nova_folha->chave[j] = temp_chave[i];
        nova_folha->nchaves++;
    }

    free(temp_ponteiro);
    free(temp_chave);

    nova_folha->ponteiros[ordem - 1] = folha->ponteiros[ordem - 1];
    folha->ponteiros[ordem - 1] = nova_folha;

    for (i = folha->nchaves; i < ordem - 1; i++)
        folha->ponteiros[i] = NULL;
    for (i = nova_folha->nchaves; i < ordem - 1; i++)
        nova_folha->ponteiros[i] = NULL;

    nova_folha->pai = folha->pai;
    nova_chave = nova_folha->chave[0];

    return inserir_in_pai(arv, folha, nova_chave, nova_folha);
}


/* Inserts a new key and pointer to a node
 * into a node into which these can fit
 * without violating the B+ tree properties.
 */
//insert_into_node
TABM *inserir_in_nodo(TABM *arv, TABM *n, int indice_esquerdo, char ch, TABM *dir) {
    int i;

    for (i = n->nchaves; i > indice_esquerdo; i--) {
        n->ponteiros[i + 1] = n->ponteiros[i];
        n->chave[i] = n->chave[i - 1];
    }
    n->ponteiros[indice_esquerdo + 1] = dir;
    n->chave[indice_esquerdo] = ch;
    n->nchaves++;
    return arv;
}


/* Inserts a new key and pointer to a node
 * into a node, causing the node's size to exceed
 * the order, and causing the node to split into two.
 */
//insert_into_node_after_splitting
TABM *inserir_in_nodo_apos_divisao(TABM *arv, TABM *antigo_nodo, int indice_esquerdo, char ch, TABM *dir) {

    int i, j, divide;
    char k;
    TABM *novo_nodo, *filho;
    char * temp_chave;
    TABM ** temp_ponteiro;


    temp_ponteiro = malloc((ordem + 1) * sizeof(TABM *));
    if (temp_ponteiro == NULL) {
        return NULL;
    }
    temp_chave = malloc(ordem * sizeof(char));
    if (temp_chave == NULL) {
        return NULL;
    }

    for (i = 0, j = 0; i < antigo_nodo->nchaves + 1; i++, j++) {
        if (j == indice_esquerdo + 1) j++;
        temp_ponteiro[j] = antigo_nodo->ponteiros[i];
    }

    for (i = 0, j = 0; i < antigo_nodo->nchaves; i++, j++) {
        if (j == indice_esquerdo) j++;
        temp_chave[j] = antigo_nodo->chave[i];
    }

    temp_ponteiro[indice_esquerdo + 1] = dir;
    temp_chave[indice_esquerdo] = ch;

    /* Create the new node and copy
     * half the keys and pointers to the
     * old and half to the new.
     */
    divide = corta(ordem);
    novo_nodo = cria_nodo();
    antigo_nodo->nchaves = 0;
    for (i = 0; i < divide - 1; i++) {
        antigo_nodo->ponteiros[i] = temp_ponteiro[i];
        antigo_nodo->chave[i] = temp_chave[i];
        antigo_nodo->nchaves++;
    }
    antigo_nodo->ponteiros[i] = temp_ponteiro[i];
    k = temp_chave[divide - 1];
    for (++i, j = 0; i < ordem; i++, j++) {
        novo_nodo->ponteiros[j] = temp_ponteiro[i];
        novo_nodo->chave[j] = temp_chave[i];
        novo_nodo->nchaves++;
    }
    novo_nodo->ponteiros[j] = temp_ponteiro[i];
    free(temp_ponteiro);
    free(temp_chave);
    novo_nodo->pai = antigo_nodo->pai;
    for (i = 0; i <= novo_nodo->nchaves; i++) {
        filho = novo_nodo->ponteiros[i];
        filho->pai = novo_nodo;
    }

    return inserir_in_pai(arv, antigo_nodo, k, novo_nodo);
}



/* Inserts a new node (leaf or internal node) into the B+ tree.
 * Returns the root of the tree after insertion.
 */
//insert_into_parent
TABM *inserir_in_pai(TABM *arv, TABM *esq, char ch, TABM *dir) {

    int indice_esquerdo;
    TABM *pai;

    pai = esq->pai;

    if (pai == NULL)
        return inserir_em_nova_raiz(esq, ch, dir);

    indice_esquerdo = pega_indice_esquerdo(pai, esq);

    if (pai->nchaves < ordem - 1)
        return inserir_in_nodo(arv, pai, indice_esquerdo, ch, dir);

    return inserir_in_nodo_apos_divisao(arv, pai, indice_esquerdo, ch, dir);
}


/* Creates a new root for two subtrees
 * and inserts the appropriate key into
 * the new root.
 */
//insert_into_new_root
TABM *inserir_em_nova_raiz(TABM *esq, char ch, TABM *dir) {

    TABM *raiz = cria_nodo();
    raiz->chave[0] = ch;
    raiz->ponteiros[0] = esq;
    raiz->ponteiros[1] = dir;
    raiz->nchaves++;
    raiz->pai = NULL;
    esq->pai = raiz;
    dir->pai = raiz;
    return raiz;
}



/* First insertion:
 * start a new tree.
 */
TABM *cria_nova_arvore(char ch, letra *ponteiro) {

    TABM *raiz = cria_folha();
    raiz->chave[0] = ch;
    raiz->ponteiros[0] = ponteiro;
    raiz->ponteiros[ordem - 1] = NULL;
    raiz->pai = NULL;
    raiz->nchaves++;
    return raiz;
}



/* Master insertion function.
 * Inserts a key and an associated value into
 * the B+ tree, causing the tree to be adjusted
 * however necessary to maintain the B+ tree
 * properties.
 */
TABM *inserir(TABM *arv, char ch, char let) {

    letra *letra_ponteiro = NULL;
    TABM *folha = NULL;

    /* The current implementation ignores
     * duplicates.
     */

    letra_ponteiro = busca(arv, ch, 0, NULL);

    if (letra_ponteiro != NULL) {
        letra_ponteiro->let = let;
        return arv;
    }

    /* Create a new record for the
     * value.
     */
    letra_ponteiro = cria_letra(let);

    /* Case: the tree does not exist yet.
     * Start a new tree.
     */

    if (arv == NULL)
        return cria_nova_arvore(ch, letra_ponteiro);


    /* Case: the tree already exists.
     * (Rest of function body.)
     */

    folha = busca_folha(arv, ch, 0);

    /* Case: leaf has room for key and record_pointer.
     */

    if (folha->nchaves < ordem - 1) {
        folha = insere_na_folha(folha, ch, letra_ponteiro);
        return arv;
    }


    /* Case:  leaf must be split.
     */

    return inserir_na_folha_apos_divisao(arv, folha, ch, letra_ponteiro);
}

int caminho_ate_raiz(TABM *const arv, TABM *filho) {
    int tamanho = 0;
    TABM *c = filho;
    while (c != arv) {
        c = c->pai;
        tamanho++;
    }
    return tamanho;
}

void naFila(TABM *novo_nodo) {
    TABM *c;
    if (fila == NULL) {
        fila = novo_nodo;
        fila->prox = NULL;
    }
    else {
        c = fila;
        while(c->prox != NULL) {
            c = c->prox;
        }
        c->prox = novo_nodo;
        novo_nodo->prox = NULL;
    }
}

TABM *tiraFila(void) {
    TABM *n = fila;
    fila = fila->prox;
    n->prox = NULL;
    return n;
}

void imprime_arvore(TABM *const arv) {

    TABM * n = NULL;
    int i = 0;
    int rank = 0;
    int new_rank = 0;

    if (arv == NULL) {
        printf("Arvore vazia.\n");
        return;
    }
    fila = NULL;
    naFila(arv);
    while(fila != NULL) {
        n = tiraFila();
        if (n->pai != NULL && n == n->pai->ponteiros[0]) {
            new_rank = caminho_ate_raiz(arv, n);
            if (new_rank != rank) {
                rank = new_rank;
                printf("\n");
            }
        }
        for (i = 0; i < n->nchaves; i++) {
            printf("%c ", n->chave[i]);
        }
        if (!n->folha)
            for (i = 0; i <= n->nchaves; i++)
                naFila(n->ponteiros[i]);
        printf("| ");
    }
    printf("\n");
}

char* encriptaLetra(int andar, int pagina, int posicao) {
    char* encriptacao = malloc(3*sizeof(char)+1);
    char castado[2];
    sprintf(castado, "%d", andar);
    strncpy(encriptacao, castado, sizeof(char)*3);
    sprintf(castado, "%d", pagina);
    if (andar != 0) strcat(encriptacao, castado);
    sprintf(castado, "%d", posicao);
    strcat(encriptacao, castado);
    return encriptacao;
}

int calculaAltura(TABM * const raiz) {
	int h = 0;
	TABM * c = raiz;
	while (!c->folha) {
		c = c->ponteiros[0];
		h++;
	}
	return h;
}

char* encripta(char letra, TABM* arv) {
    int andar = calculaAltura(arv);
    TABM * n = NULL;
    int i;
    int pagina = 0;
    fila = NULL;
    naFila(arv);
    while (fila != NULL) {
        n = tiraFila();
        if (n->folha) {
            for (i = 0; i < n->nchaves; i++) {
                if (letra == n->chave[i]) {
                    char* codigo = encriptaLetra(andar, pagina, i);
                    strcat(codigo, " ");
                    return codigo;
                }
            }
            
            pagina++;
        }

        if (!n->folha)
            for (i = 0; i <= n->nchaves; i++)
                naFila(n->ponteiros[i]);
    }
}

char* encriptaFraseBM(char* frase, TABM* arv) {
    char* encriptacao = malloc(strlen(frase) * 5);
    for (int i = 0; i < strlen(frase); i++) {
        char* codigo = encripta(frase[i], arv);
        if (codigo != NULL)
            strcat(encriptacao, codigo);
    }
    return encriptacao;
}

////////////////////////////


int main() {

    //letra *let = 't';

    TABM *arv = NULL;

    //TABM *arv = cria_nova_arvore('t', let);


    arv = inserir(arv, 't', 't');
    arv = inserir(arv, 'h', 'h');
    arv = inserir(arv, 'e', 'e');
    arv = inserir(arv, 'p', 'p');
    arv = inserir(arv, 'r', 'r');
    arv = inserir(arv, 'o', 'o');
    arv = inserir(arv, 'm', 'm');
    arv = inserir(arv, 'i', 'i');
    arv = inserir(arv, 's', 's');

    /*
    arv = insereBM(arv2, 'h', ordem);
    arv = insereBM(arv2, 'e', ordem);
    arv = insereBM(arv2, 'p', ordem);
    arv = insereBM(arv2, 'r', ordem);
    arv = insereBM(arv2, 'o', ordem);
    arv = insereBM(arv2, 'm', ordem);
    arv2 = insereBM(arv2, 'i', ordem);
    arv2 = insereBM(arv2, 's', ordem);
    arv2 = insereBM(arv2, 'e', ordem);
    arv2 = insereBM(arv2, 'o', ordem);
    arv2 = insereBM(arv2, 'f', ordem);
    arv2 = insereBM(arv2, 'q', ordem);
    arv2 = insereBM(arv2, 'u', ordem);
    arv2 = insereBM(arv2, 'a', ordem);
    arv2 = insereBM(arv2, 'n', ordem);
    arv2 = insereBM(arv2, 't', ordem);
    arv2 = insereBM(arv2, 'u', ordem);
    arv2 = insereBM(arv2, 'm', ordem);
    */

    imprime_arvore(arv);
    printf("\n -- \n");
    printf("%s", encriptaFraseBM("the promise", arv));

    return 0;
}

