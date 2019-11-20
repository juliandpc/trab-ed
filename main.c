#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TAM 60

/*
//PALAVRA A SER CRIPTOGRAFADA [DESNECESSAURO]
typedef struct letra{
    char letra;
    int nivel;
    int pagina;
    int posicao;
}TLetra;
*/

//ESTRUTURA DA ARVORE B
typedef struct ArvB{
    int nchaves;
    char folha, *chave;
    struct ArvB **filho;
}TAB;

//ESTRUTURA DA ARVORE B+
typedef struct ArvBM{
    int nchaves;
    char folha, *chave;
    struct ArvBM **filho, *prox;
}TABM;


//
//
//  ^^^^^^ STRUCTS ^^^^^^
//
//

//FUNCOES DA ARVORE B

TAB *criaB(int d){
    TAB* novo = (TAB*)malloc(sizeof(TAB));
    novo->nchaves = 0;
    novo->chave =(char*)malloc(sizeof(char*)*((d*2)-1));
    novo->folha = 1;
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
            for(j=0; j<=andar; j++) printf("   ");
            printf("%c\n", a->chave[i]);
        }
        imprimeArvB(a->filho[i],andar+1);
    }
}

TAB *divisaoB(TAB *x, int i, TAB* y, int d){
    TAB *z = criaB(d);
    z->nchaves= d - 1;
    z->folha = y->folha;
    int j;
    for(j=0;j<d-1;j++) z->chave[j] = y->chave[j+d];
    if(!y->folha){
        for(j=0;j<d;j++){
            z->filho[j] = y->filho[j+d];
            y->filho[j+d] = NULL;
        }
    }
    y->nchaves = d-1;
    for(j=x->nchaves; j>=i; j--) x->filho[j+1]=x->filho[j];
    x->filho[i] = z;
    for(j=x->nchaves; j>=i; j--) x->chave[j] = x->chave[j-1];
    x->chave[i-1] = y->chave[d-1];
    x->nchaves++;
    return x;
}

TAB *Insere_Nao_Completo(TAB *x, char k, int d){
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
    x->filho[i] = Insere_Nao_Completo(x->filho[i], k, d);
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
        S = Insere_Nao_Completo(S,k,d);
        return S;
    }
    T = Insere_Nao_Completo(T,k,d);
    return T;
}



//FUNCOES DA ARVORE B+
void imprimeArvBM(TABM *a, int andar){
    if(a){
        int i,j;
        for(i=0; i<=a->nchaves-1; i++){
            imprimeArvBM(a->filho[i],andar+1);
            for(j=0; j<=andar; j++) printf("   ");
            printf("%c\n", a->chave[i]);
        }
        imprimeArvBM(a->filho[i],andar+1);
    }
}








int main() {
    /*
    char frase[TAM];
    int tipoArv;
    int d;
    int tipoInsercao;
    int acao;

    int continuar = 1;
    while(continuar){

        printf("Digite a frase que deseja criptografar: \n");
        scanf("%[^\n]", frase);
        printf("\n");

        printf("Com que tipo de arvore voce deseja criptografar a mensagem? \n");
        printf("(1) Arvore B \n");
        printf("(2) Arvore B+ \n");
        printf("(0) Sair \n");
        scanf("%d", &tipoArv);

        switch(tipoArv){
            case 1:
                printf("Arvore B!!! \n\n");
                break;
            case 2:
                printf("Arvore B+!!! \n\n");
                break;
            case 0:
                continuar = 0;
                break;
        }

        if(continuar == 0){
            break;
        }

        printf("Qual a ordem (d) da arvore selecionada? \n");
        scanf("%d", &d);

        printf("Qual a ordem de insercao das letras da frase na arvore? \n");
        printf("(1) Alfabetica \n");
        printf("(2) Por frequencia \n");
        printf("(0) Sair \n");
        scanf("%d", &tipoInsercao);

        switch(tipoInsercao){
            case 1:
                printf("Alfabetica!!! \n\n");
                break;
            case 2:
                printf("Por frequencia!!! \n\n");
                break;
            case 0:
                continuar = 0;
                break;
        }

        if(continuar == 0){
            break;
        }

        printf("Com a arvore montada e a mensagem criptografada, o que voce deseja fazer agora? \n");
        printf("(1) Comparar o codigo gerado \n");
        printf("(2) Buscar informacoes subordinadas \n");
        printf("(3) Alterar a frequencia de uma letra \n");
        printf("(4) Buscar todas as letras de uma determinada classificação \n");
        printf("(5) Remover todas as letras de uma determinada classificação \n");
        printf("(0) Sair \n");
        scanf("%d", &acao);

        if(acao == 0){
            break;
        }
    }



    printf("\n");
    printf("\n");
    int i = 0;
    while(frase[i] != '\0'){
        printf("%c", frase[i]);
        i++;
    }
    */

    //printf("%s", frase);

    TAB* arv = criaB(2);

    arv = insereB(arv, 'b', 2);
    arv = insereB(arv, 'd', 2);
    arv = insereB(arv, 'a', 2);
    arv = insereB(arv, 'e', 2);
    arv = insereB(arv, 'f', 2);
    arv = insereB(arv, 'c', 2);
    arv = insereB(arv, 't', 2);

    imprimeArvB(arv, 0);



    return 0;
}