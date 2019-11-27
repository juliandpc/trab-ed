#include <stdio.h>
#include <stdlib.h>
#include <string.h>



#define TAM 60

////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
///////// FUNCOES DA ARVORE B //////////
////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////

//ESTRUTURA DA ARVORE B
typedef struct ArvB{
    int nchaves;
    char folha, *chave;
    struct ArvB **filho;
}TAB;

TAB *criaB(int d){
    TAB* novo = (TAB*)malloc(sizeof(TAB));
    novo->nchaves = 0;
    novo->chave =(char*)malloc(sizeof(char*)*((d*2)-1));
    novo->folha=1;
    novo->filho = (TAB**)malloc(sizeof(TAB*)*d*2);
    int i;
    for(i=0; i<(d*2); i++) novo->filho[i] = NULL;
    return novo;
}

TAB *buscaB(TAB* x, char ch){
    TAB *resp = NULL;
    if(!x) return resp;
    int i = 0;
    while(i < x->nchaves && ch > x->chave[i]) i++;
    if(i < x->nchaves && ch == x->chave[i]) return x;
    if(x->folha) return resp;
    return buscaB(x->filho[i], ch);
}


void imprimeArvB(TAB *a, int andar){
    if(a){
        int i,j;
        for(i=0; i<=a->nchaves-1; i++){
            imprimeArvB(a->filho[i],andar+1);
            for(j=0; j<=andar; j++) printf("---");
            printf("%c\n", a->chave[i]);
        }
        imprimeArvB(a->filho[i],andar+1);
    }
}


TAB *divisaoB(TAB *x, int i, TAB* y, int d){
    TAB *z = criaB(d);
    z->nchaves = d - 1;
    z->folha = y->folha;
    int j;
    for(j=0;j<d-1;j++) z->chave[j] = y->chave[j+d];
    if(!y->folha){
        for(j=0;j<d;j++){
            z->filho[j] = y->filho[j+d];
            y->filho[j+d] = NULL;
        }
    }
    y->nchaves = d - 1;
    for(j=x->nchaves; j>=i; j--) x->filho[j+1]=x->filho[j];
    x->filho[i] = z;
    for(j=x->nchaves; j>=i; j--) x->chave[j] = x->chave[j-1];
    x->chave[i-1] = y->chave[d-1];
    x->nchaves++;
    return x;
}

TAB *insere_Nao_Completo(TAB *x, char k, int d){
    int i = x->nchaves-1;
    if(x->folha){
        while((i>=0) && (k<x->chave[i])){
            x->chave[i+1] = x->chave[i];
            i--;
        }
        x->chave[i+1] = k;
        x->nchaves++;
        return x;
    }
    while((i>=0) && (k<x->chave[i])) i--;
    i++;
    if(x->filho[i]->nchaves == ((2*d)-1)){
        x = divisaoB(x, (i+1), x->filho[i], d);
        if(k>x->chave[i]) i++;
    }
    x->filho[i] = insere_Nao_Completo(x->filho[i], k, d);
    return x;
}


TAB *insereB(TAB *T, char k, int d){
    if(buscaB(T,k)) return T;
    if(!T){
        T=criaB(d);
        T->chave[0] = k;
        T->nchaves=1;
        return T;
    }
    if(T->nchaves == (2*d)-1){
        TAB *S = criaB(d);
        S->nchaves=0;
        S->folha = 0;
        S->filho[0] = T;
        S = divisaoB(S,1,T,d);
        S = insere_Nao_Completo(S,k,d);
        return S;
    }
    T = insere_Nao_Completo(T,k,d);
    return T;
}


TAB* removerB(TAB* arv, char ch, int t){
    if(!arv) return arv;
    int i;
    for(i = 0; i<arv->nchaves && arv->chave[i] < ch; i++);
    if(i < arv->nchaves && ch == arv->chave[i]){
        if(arv->folha){
            int j;
            for(j=i; j<arv->nchaves-1;j++) arv->chave[j] = arv->chave[j+1];
            arv->nchaves--;
            return arv;
        }
        if(!arv->folha && arv->filho[i]->nchaves >= t){
            TAB *y = arv->filho[i];
            while(!y->folha) y = y->filho[y->nchaves];
            char temp = y->chave[y->nchaves-1];
            arv->filho[i] = removerB(arv->filho[i], temp, t);
            arv->chave[i] = temp;
            return arv;
        }
        if(!arv->folha && arv->filho[i+1]->nchaves >= t){
            TAB *y = arv->filho[i+1];
            while(!y->folha) y = y->filho[0];
            char temp = y->chave[0];
            y = removerB(arv->filho[i+1], temp, t);
            arv->chave[i] = temp;
            return arv;
        }
        if(!arv->folha && arv->filho[i+1]->nchaves == t-1 && arv->filho[i]->nchaves == t-1){
            TAB *y = arv->filho[i];
            TAB *z = arv->filho[i+1];
            y->chave[y->nchaves] = ch;
            int j;
            for(j=0; j<t-1; j++)
                y->chave[t+j] = z->chave[j];
            for(j=0; j<=t; j++)
                y->filho[t+j] = z->filho[j];
            y->nchaves = 2*t-1;
            for(j=i; j < arv->nchaves-1; j++)
                arv->chave[j] = arv->chave[j+1];
            for(j=i+1; j <= arv->nchaves; j++)
                arv->filho[j] = arv->filho[j+1];
            arv->filho[j] = NULL;
            arv->nchaves--;
            arv->filho[i] = removerB(arv->filho[i], ch, t);
            return arv;
        }
    }

    TAB *y = arv->filho[i], *z = NULL;
    if (y->nchaves == t-1){
        if((i < arv->nchaves) && (arv->filho[i+1]->nchaves >=t)){
            z = arv->filho[i+1];
            y->chave[t-1] = arv->chave[i];
            y->nchaves++;
            arv->chave[i] = z->chave[0];
            int j;
            for(j=0; j < z->nchaves-1; j++)
                z->chave[j] = z->chave[j+1];
            y->filho[y->nchaves] = z->filho[0];
            for(j=0; j < z->nchaves; j++)
                z->filho[j] = z->filho[j+1];
            z->nchaves--;
            arv->filho[i] = removerB(arv->filho[i], ch, t);
            return arv;
        }
        if((i > 0) && (!z) && (arv->filho[i-1]->nchaves >=t)){
            z = arv->filho[i-1];
            int j;
            for(j = y->nchaves; j>0; j--)
                y->chave[j] = y->chave[j-1];
            for(j = y->nchaves+1; j>0; j--)
                y->filho[j] = y->filho[j-1];
            y->chave[0] = arv->chave[i-1];
            y->nchaves++;
            arv->chave[i-1] = z->chave[z->nchaves-1];
            y->filho[0] = z->filho[z->nchaves];
            z->nchaves--;
            arv->filho[i] = removerB(y, ch, t);
            return arv;
        }
        if(!z){
            if(i < arv->nchaves && arv->filho[i+1]->nchaves == t-1){
                z = arv->filho[i+1];
                y->chave[t-1] = arv->chave[i];
                y->nchaves++;
                int j;
                for(j=0; j < t-1; j++){
                    y->chave[t+j] = z->chave[j];
                    y->nchaves++;
                }
                if(!y->folha){
                    for(j=0; j<t; j++){
                        y->filho[t+j] = z->filho[j];
                    }
                }
                for(j=i; j < arv->nchaves-1; j++){
                    arv->chave[j] = arv->chave[j+1];
                    arv->filho[j+1] = arv->filho[j+2];
                }
                arv->nchaves--;
                arv = removerB(arv, ch, t);
                return arv;
            }
            if((i > 0) && (arv->filho[i-1]->nchaves == t-1)){
                z = arv->filho[i-1];
                if(i == arv->nchaves)
                    z->chave[t-1] = arv->chave[i-1];
                else
                    z->chave[t-1] = arv->chave[i];
                z->nchaves++;
                int j;
                for(j=0; j < t-1; j++){
                    z->chave[t+j] = y->chave[j];
                    z->nchaves++;
                }
                if(!z->folha){
                    for(j=0; j<t; j++){
                        z->filho[t+j] = y->filho[j];
                    }
                }
                arv->nchaves--;
                arv->filho[i-1] = z;
                arv = removerB(arv, ch, t);
                return arv;
            }
        }
    }
    arv->filho[i] = removerB(arv->filho[i], ch, t);
    return arv;
}

TAB* retiraB(TAB* arv, char k, int t){
    if(!arv || !buscaB(arv, k)) return arv;
    return removerB(arv, k, t);
}

int max(int a, int b){
    return a > b ? a : b;
}


int nivelB(TAB* arv, char ch, int andar){
    TAB* aux = arv;
    if(aux){
        int i = 0;
        while(i < aux->nchaves && ch >aux->chave[i]){
            i++;
        }
        if(i < aux->nchaves && ch == aux->chave[i]){
            return andar;
        }

        return nivelB(aux->filho[i], ch, andar+1);
    }
}


int paginaB(TAB* arv, char ch){
    TAB* aux = arv;
    if(aux){
        int i = 0;
        while(i < aux->nchaves && ch >aux->chave[i]){
            i++;
        }
        if(i < aux->nchaves && ch == aux->chave[i]){
            return i;
        }
        return paginaB(aux->filho[i], ch);
    }

}

TAB *montaArvoreB(int d, char frase[TAM]){
    TAB* arv = criaB(d);
    int i = 0;
    while(frase[i] != '\0'){
        arv = insereB(arv, frase[i], d);
        i++;
    }

    return arv;
}

char* encriptaLetra(int andar, int pagina, int posicao) {
    char* encriptacao = malloc(5);
    char castado[12];
    sprintf(castado, "%d", andar);
    strcat(encriptacao, castado);
    sprintf(castado, "%d", pagina);
    if (andar != 0) strcat(encriptacao, castado);
    sprintf(castado, "%d", posicao);
    strcat(encriptacao, castado);
    return encriptacao;
}

int calculaPagina(TAB* arv, char letra, int pagina, int andarAtual, int andar) {
    for (int j = 0; j < arv->nchaves; j++) {
        if (letra == arv->chave[j])
            return pagina;
    }
    if (andarAtual == andar) return pagina + 1;
    for (int i = 0; i <= arv->nchaves; i++) {
        int temp = pagina;
        pagina = calculaPagina(arv->filho[i], letra, pagina, andarAtual + 1, andar);
        if (pagina == temp) break;
    }
    return pagina;
}

char* encripta(char letra, TAB* arv, int andar, TAB* raiz) {
    for (int j = 0; j < arv->nchaves; j++) {
        if (letra == arv->chave[j]) {
            int pagina = calculaPagina(raiz, letra, 0, 0, andar);
            if (pagina < 0) pagina = -pagina;
            char *encriptacao = encriptaLetra(andar, pagina, j);
            strcat(encriptacao, " ");
            return encriptacao;
        }
    }
    if (arv->filho[0] == NULL) return "";
    int i = 0;
    while(i < arv->nchaves && letra > arv->chave[i]) i++;
    return encripta(letra, arv->filho[i], andar + 1, raiz);
}

char* encriptaFrase(char* frase, TAB* arv) {
    char* encriptacao = malloc(strlen(frase) * 5);
    for (int i = 0; i < strlen(frase); i++) {
        strcat(encriptacao, encripta(frase[i], arv, 0, arv));
    }
    return encriptacao;
}

// void retiraCategoriaB(TAB* arv, char* categoria, int ordem){
//     for(int elem=0; elem<strlen(categoria); elem++){
//         imprimeArvB(arv, 0);
//         retiraB(arv, categoria[elem], ordem);
//         }
// }


//         /FUNCOES DA ARVORE B        //

////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
///////// FUNCOES DA ARVORE B+ /////////
////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
/*
void imprimeArvBM(TABM *a, int andar){
    if(a){
        int i,j;
        for(i=0; i<=a->nchaves-1; i++){
            imprimeArvBM(a->filho[i],andar+1);
            for(j=0; j<=andar; j++) printf("---");
            printf("%c\n", a->chave[i]);
        }
        imprimeArvBM(a->filho[i],andar+1);
    }
}*/
//        /FUNCOES DA ARVORE B+        //

////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
//////////// FUNCOES GERAIS ////////////
////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////

void *limpaFrase(char *frase){
    int len = strlen(frase);
    for(int i=0; i<len; i++)
    {
        if(frase[i]==' ')
        {
            for(int j=i; j<len; j++)
            {
                frase[j]=frase[j+1];
            }
            len--;
        }
    }

}


////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
///////// FUNCOES DA ARVORE B+ /////////
////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////



const int ordem = 4;

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

int pega_indice_esquerdo(TABM *pai, TABM *esq) {
    int indice_esquerdo = 0;
    while (indice_esquerdo <= pai->nchaves && pai->ponteiros[indice_esquerdo] != esq)
        indice_esquerdo++;
    return indice_esquerdo;
}

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


TABM *cria_nova_arvore(char ch, letra *ponteiro) {

    TABM *raiz = cria_folha();
    raiz->chave[0] = ch;
    raiz->ponteiros[0] = ponteiro;
    raiz->ponteiros[ordem - 1] = NULL;
    raiz->pai = NULL;
    raiz->nchaves++;
    return raiz;
}


TABM *inserir(TABM *arv, char ch, char let) {

    letra *letra_ponteiro = NULL;
    TABM *folha = NULL;

    letra_ponteiro = busca(arv, ch, 0, NULL);

    if (letra_ponteiro != NULL) {
        letra_ponteiro->let = let;
        return arv;
    }

    letra_ponteiro = cria_letra(let);

    if (arv == NULL)
        return cria_nova_arvore(ch, letra_ponteiro);

    folha = busca_folha(arv, ch, 0);

    if (folha->nchaves < ordem - 1) {
        folha = insere_na_folha(folha, ch, letra_ponteiro);
        return arv;
    }

    return inserir_na_folha_apos_divisao(arv, folha, ch, letra_ponteiro);
}


//REMOCAO ARV B+

int pega_indice_vizinho(TABM * n);
TABM *ajusta_raiz(TABM *raiz);
TABM *unir_nodos(TABM *raiz, TABM *n, TABM *vizinho, int indice_vizinho, char k);
TABM *redistribui_nodos(TABM *arv, TABM *n, TABM *vizinho, int indice_vizinho, int k_indice, char k);
TABM *remove_entrada(TABM *arv, TABM *n, char ch, void * ponteiro);
TABM *remover(TABM *arv, char ch);


int pega_indice_vizinho(TABM * n) {

    int i;

    for (i = 0; i <= n->pai->nchaves; i++)
        if (n->pai->ponteiros[i] == n)
            return i - 1;

    printf("Ponteiro para o noh pai nao existe.\n");
    return 0;
}

TABM *ajusta_raiz(TABM *raiz){

    TABM *nova_raiz;


    if (raiz->nchaves > 0)
        return raiz;


    if (!raiz->folha) {
        nova_raiz = raiz->ponteiros[0];
        nova_raiz->pai = NULL;
    }

    else
        nova_raiz = NULL;

    free(raiz->chave);
    free(raiz->ponteiros);
    free(raiz);

    return nova_raiz;
}

TABM *unir_nodos(TABM *raiz, TABM *n, TABM *vizinho, int indice_vizinho, char k){
    int i, j, indice_insercao_vizinho, n_end;
    TABM *tmp;

    if (indice_vizinho == -1) {
        tmp = n;
        n = vizinho;
        vizinho = tmp;
    }

    indice_insercao_vizinho = vizinho->nchaves;

    if (!n->folha) {

        vizinho->chave[indice_insercao_vizinho] = k;
        vizinho->nchaves++;


        n_end = n->nchaves;

        for (i = indice_insercao_vizinho + 1, j = 0; j < n_end; i++, j++) {
            vizinho->chave[i] = n->chave[j];
            vizinho->ponteiros[i] = n->ponteiros[j];
            vizinho->nchaves++;
            n->nchaves--;
        }

        vizinho->ponteiros[i] = n->ponteiros[j];

        for (i = 0; i < vizinho->nchaves + 1; i++) {
            tmp = (TABM *)vizinho->ponteiros[i];
            tmp->pai = vizinho;
        }
    }

    else {
        for (i = indice_insercao_vizinho, j = 0; j < n->nchaves; i++, j++) {
            vizinho->chave[i] = n->chave[j];
            vizinho->ponteiros[i] = n->ponteiros[j];
            vizinho->nchaves++;
        }
        vizinho->ponteiros[ordem - 1] = n->ponteiros[ordem - 1];
    }

    raiz = remove_entrada(raiz, n->pai, k, n);
    free(n->chave);
    free(n->ponteiros);
    free(n);
    return raiz;
}

TABM *redistribui_nodos(TABM *arv, TABM *n, TABM *vizinho, int indice_vizinho, int k_indice, char k) {

    int i;
    TABM *tmp;

    if (indice_vizinho != -1) {
        if (!n->folha)
            n->ponteiros[n->nchaves + 1] = n->ponteiros[n->nchaves];
        for (i = n->nchaves; i > 0; i--) {
            n->chave[i] = n->chave[i - 1];
            n->ponteiros[i] = n->ponteiros[i - 1];
        }
        if (!n->folha) {
            n->ponteiros[0] = vizinho->ponteiros[vizinho->nchaves];
            tmp = (TABM *)n->ponteiros[0];
            tmp->pai = n;
            vizinho->ponteiros[vizinho->nchaves] = NULL;
            n->chave[0] = k;
            n->pai->chave[k_indice] = vizinho->chave[vizinho->nchaves - 1];
        }
        else {
            n->ponteiros[0] = vizinho->ponteiros[vizinho->nchaves - 1];
            vizinho->ponteiros[vizinho->nchaves - 1] = NULL;
            n->chave[0] = vizinho->chave[vizinho->nchaves - 1];
            n->pai->chave[k_indice] = n->chave[0];
        }
    }

    else {
        if (n->folha) {
            n->chave[n->nchaves] = vizinho->chave[0];
            n->ponteiros[n->nchaves] = vizinho->ponteiros[0];
            n->pai->chave[k_indice] = vizinho->chave[1];
        }
        else {
            n->chave[n->nchaves] = k;
            n->ponteiros[n->nchaves + 1] = vizinho->ponteiros[0];
            tmp = (TABM *)n->ponteiros[n->nchaves + 1];
            tmp->pai = n;
            n->pai->chave[k_indice] = vizinho->chave[0];
        }
        for (i = 0; i < vizinho->nchaves - 1; i++) {
            vizinho->chave[i] = vizinho->chave[i + 1];
            vizinho->ponteiros[i] = vizinho->ponteiros[i + 1];
        }
        if (!n->folha)
            vizinho->ponteiros[i] = vizinho->ponteiros[i + 1];
    }

    n->nchaves++;
    vizinho->nchaves--;

    return arv;
}

TABM *remove_entrada_do_nodo(TABM *n, char ch, TABM *ponteiro){
    int i, num_ponteiros;

    i = 0;
    while (n->chave[i] != ch)
        i++;
    for (++i; i < n->nchaves; i++)
        n->chave[i - 1] = n->chave[i];

    num_ponteiros = n->folha ? n->nchaves : n->nchaves + 1;
    i = 0;
    while (n->ponteiros[i] != ponteiro)
        i++;
    for (++i; i < num_ponteiros; i++)
        n->ponteiros[i - 1] = n->ponteiros[i];

    n->nchaves--;

    if (n->folha)
        for (i = n->nchaves; i < ordem - 1; i++)
            n->ponteiros[i] = NULL;
    else
        for (i = n->nchaves + 1; i < ordem; i++)
            n->ponteiros[i] = NULL;

    return n;
}

TABM *remove_entrada(TABM *arv, TABM *n, char ch, void * ponteiro){
    int minimo_chaves;
    TABM *vizinho;
    int indice_vizinho;
    int k_indice;
    char k;
    int capacidade_do_no;

    n = remove_entrada_do_nodo(n, ch, ponteiro);

    if (n == arv)
        return ajusta_raiz(arv);

    minimo_chaves = n->folha ? corta(ordem - 1) : corta(ordem) - 1;

    if (n->nchaves >= minimo_chaves)
        return arv;

    indice_vizinho = pega_indice_vizinho(n);
    k_indice = indice_vizinho == -1 ? 0 : indice_vizinho;
    k = n->pai->chave[k_indice];
    vizinho = indice_vizinho == -1 ? n->pai->ponteiros[1] :
               n->pai->ponteiros[indice_vizinho];

    capacidade_do_no = n->folha ? ordem : ordem - 1;

    if (vizinho->nchaves + n->nchaves < capacidade_do_no)
        return unir_nodos(arv, n, vizinho, indice_vizinho, k);

    else
        return redistribui_nodos(arv, n, vizinho, indice_vizinho, k_indice, k);
}

TABM * remover(TABM *arv, char ch) {

    TABM *chave_folha = NULL;
    letra *chave_letra = NULL;

    chave_letra = busca(arv, ch, 0, &chave_folha);

    if (chave_letra != NULL && chave_folha != NULL) {
        arv = remove_entrada(arv, chave_folha, ch, chave_letra);
        free(chave_letra);
    }
    return arv;
}

//FUNCOES GERAIS

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

char* encriptaLetraBM(int andar, int pagina, int posicao) {
    char* encriptacao = malloc(3*sizeof(char)+1);
    char castado[2];
    strncpy(encriptacao, "\0", sizeof(char));
    sprintf(castado, "%d", andar);
    strcat(encriptacao, castado);
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

char* encriptaBM(char letra, TABM* arv) {
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
                    char* codigo = encriptaLetraBM(andar, pagina, i);
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
        char* codigo = encriptaBM(frase[i], arv);
        if (codigo != NULL)
            strcat(encriptacao, codigo);
    }
    return encriptacao;
}


//////////  /ARVORE B+      /////////////////////////

char *menor(char *frase1, char *frase2){
    int a = 0;
    int b = 0;
    while(frase1[a] != '\0'){
        a++;
    }
    while(frase2[b] != '\0'){
        b++;
    }
    return a < b ? frase1 : frase2;
}


int comparaCodigos(char *frase1, char *frase2, TAB* arv1, TABM* arv2){
    int contadorIguais = 0;
    char *frase = menor(frase1, frase2);
    for (int i = 0; frase[i] != '\0'; i++) {
        char* codigo1 = encripta(frase1[i], arv1, 0, arv1);
        char* codigo2 = encriptaBM(frase2[i], arv2);
        if(strcmp(codigo1, codigo2) == 0){
            contadorIguais++;
        }
    }

    return contadorIguais;
}



int main() {
    char vogais[6] = "aeiou";
    char consoantes[22] = "bcdfghjklmnpqrstvwxyz";
    int tipoArv;
    int acao;
    int ordemB = 2;
    int qtd;

    int continuar = 1;
    while(continuar){
        char frase[TAM];
        char fraseWithSpaces[TAM];
        printf("Digite a frase que deseja criptografar: \n");
        scanf("%[^\n]", frase);
        strcpy(fraseWithSpaces,frase);
        limpaFrase(frase);
        printf("\n");
        printf("Com que tipo de arvore voce deseja criptografar a mensagem? \n");
        printf("(1) Arvore B \n");
        printf("(2) Arvore B+ \n");
        printf("(0) Sair \n");
        scanf("%d", &tipoArv);

        TAB* arv = criaB(ordemB);
        TABM* arv2 = cria_folha();
        char* encriptacao = malloc(strlen(frase) * 5);
        char* encriptacao2 = malloc(strlen(frase) * 5);

        switch(tipoArv){
            case 1:

                for(int i=0;i<strlen(frase);i++){
                    arv = insereB(arv,  frase[i], ordemB);
                }

                imprimeArvB(arv, 0);
                printf("\n");
                encriptacao = encriptaFrase(frase, arv);
                printf("Frase: %s|\nEncriptado: %s \n\n", fraseWithSpaces, encriptacao);
                break;
            case 2:

                for(int i=0;i<strlen(frase);i++){
                    arv2 = inserir(arv2, frase[i], frase[i]);
                }

                imprime_arvore(arv2);
                printf("\n");
                encriptacao2 = encriptaFraseBM(frase, arv2);
                printf("Frase: %s|\nEncriptado: %s \n\n", fraseWithSpaces, encriptacao2);

                break;
            case 0:
                continuar = 0;
                break;
        }

        if(continuar == 0){
            break;
        }

        printf("Com as arvores montadas e a mensagem criptografada, o que voce deseja fazer agora? \n");
        printf("(1) Comparar o codigo gerado \n");
        printf("(2) Buscar todas as consoantes \n");
        printf("(3) Buscar todas as vogais \n");
        printf("(4) Remover todas as consoantes \n");
        printf("(5) Remover todas as vogais \n");
        printf("(0) Sair \n");
        scanf("%d", &acao);

        switch(acao){
            case 1:
                if(tipoArv == 1){
                    for(int i=0;i<strlen(frase);i++){
                        arv2 = inserir(arv2, frase[i], frase[i]);
                    }

                    imprime_arvore(arv2);
                    printf("\n");
                    encriptacao2 = encriptaFraseBM(frase, arv2);
                    printf("\n");
                    imprimeArvB(arv, 0);
                    printf("\n");
                    printf("Encriptacao da arvore B:\n %s", encriptacao);
                    printf("\n");
                    printf("Encriptacao da arvore B+:\n %s", encriptacao2);
                    printf("\n");
                    qtd = comparaCodigos(frase, frase, arv, arv2);

                    printf("As duas arvores possuem %d letras iguais nas mesmas posicoes!!\n\n", qtd);

                }

                if(tipoArv == 2){
                    for(int i=0;i<strlen(frase);i++){
                        arv = insereB(arv, frase[i], ordemB);
                    }

                    imprimeArvB(arv, 0);
                    printf("\n");
                    encriptacao = encriptaFrase(frase, arv);
                    printf("\n");
                    imprime_arvore(arv2);
                    printf("\n");
                    printf("Encriptacao da arvore B:\n %s", encriptacao);
                    printf("\n");
                    printf("Encriptacao da arvore B+:\n %s", encriptacao2);
                    printf("\n");
                    qtd = comparaCodigos(frase, frase, arv, arv2);

                    printf("As duas arvores possuem %d letras iguais nas mesmas posicoes!!\n\n", qtd);
                }


                break;
            case 2:
                if(tipoArv == 1){
                    int i = 0;
                    while(consoantes[i] != '\0'){
                        if(buscaB(arv, consoantes[i])){
                            printf("%c ", consoantes[i]);
                        }
                        i++;
                    }

                    printf("\n");
                    imprimeArvB(arv, 0);
                }
                if(tipoArv == 2){

                    int i = 0;
                    while(consoantes[i] != '\0'){
                        if(busca_folha(arv2, consoantes[i], 0)){
                            printf("%c ", consoantes[i]);
                        }
                        i++;
                    }

                    printf("\n");
                    imprime_arvore(arv2);
                }

                break;

            case 3:
                if(tipoArv == 1){
                    int i = 0;
                    while(vogais[i] != '\0'){
                        if(buscaB(arv, vogais[i])){
                            printf("%c ", vogais[i]);
                        }
                        i++;
                    }

                    printf("\n");
                    imprimeArvB(arv, 0);
                }
                if(tipoArv == 2){
                    int i = 0;
                    while(vogais[i] != '\0'){
                        if(busca_folha(arv2, vogais[i], 0)){
                            printf("%c ", vogais[i]);
                        }
                        i++;
                    }

                    printf("\n");
                    imprime_arvore(arv2);
                }

                break;


            case 4:
                if(tipoArv == 1){
                    for(int elem = 0; elem < strlen(consoantes); elem++){
                        retiraB(arv, consoantes[elem], ordemB);

                    }
                    imprimeArvB(arv, 0);
                }
                if(tipoArv == 2){
                    for(int elem=0; elem<strlen(consoantes); elem++){
                        remover(arv2, consoantes[elem]);
                    }
                    imprime_arvore(arv2);
                }

                break;
            case 5:
                if(tipoArv == 1){
                    for(int elem=0; elem<strlen(vogais); elem++){
                        retiraB(arv, vogais[elem], ordemB);
                    }
                    imprimeArvB(arv, 0);
                }
                if(tipoArv == 2){
                    for(int elem=0; elem<strlen(vogais); elem++){
                        remover(arv2, vogais[elem]);
                    }
                    imprime_arvore(arv2);
                }

                break;
            case 0:
                continuar = 0;
                break;
        }

        free(arv);
        free(arv->chave);
        free(arv->filho);
        free(arv2);
        free(arv2->chave);
        free(arv2->ponteiros);
        free(arv2->pai);
        free(arv2->prox);
        continuar = 0;
    }
    return 0;
}