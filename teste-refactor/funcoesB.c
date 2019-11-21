#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
// #include "arvoreB.h"

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
            for(j=0; j<=andar; j++) printf("   ");
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
    printf("Removendo %d...\n", ch);
    for(i = 0; i<arv->nchaves && arv->chave[i] < ch; i++);
    if(i < arv->nchaves && ch == arv->chave[i]){ //CASOS 1, 2A, 2B e 2C
        if(arv->folha){ //CASO 1
            printf("\nCASO 1\n");
            int j;
            for(j=i; j<arv->nchaves-1;j++) arv->chave[j] = arv->chave[j+1];
            arv->nchaves--;
            return arv;
        }
        if(!arv->folha && arv->filho[i]->nchaves >= t){ //CASO 2A
            printf("\nCASO 2A\n");
            TAB *y = arv->filho[i];  //Encontrar o predecessor k' de k na árvore com raiz em y
            while(!y->folha) y = y->filho[y->nchaves];
            char temp = y->chave[y->nchaves-1];
            arv->filho[i] = removerB(arv->filho[i], temp, t);
            //Eliminar recursivamente K e substitua K por K' em x
            arv->chave[i] = temp;
            return arv;
        }
        if(!arv->folha && arv->filho[i+1]->nchaves >= t){ //CASO 2B
            printf("\nCASO 2B\n");
            TAB *y = arv->filho[i+1];  //Encontrar o sucessor k' de k na árvore com raiz em y
            while(!y->folha) y = y->filho[0];
            char temp = y->chave[0];
            y = removerB(arv->filho[i+1], temp, t); //Eliminar recursivamente K e substitua K por K' em x
            arv->chave[i] = temp;
            return arv;
        }
        if(!arv->folha && arv->filho[i+1]->nchaves == t-1 && arv->filho[i]->nchaves == t-1){ //CASO 2C
            printf("\nCASO 2C\n");
            TAB *y = arv->filho[i];
            TAB *z = arv->filho[i+1];
            y->chave[y->nchaves] = ch;          //colocar ch ao final de filho[i]
            int j;
            for(j=0; j<t-1; j++)                //juntar chave[i+1] com chave[i]
                y->chave[t+j] = z->chave[j];
            for(j=0; j<=t; j++)                 //juntar filho[i+1] com filho[i]
                y->filho[t+j] = z->filho[j];
            y->nchaves = 2*t-1;
            for(j=i; j < arv->nchaves-1; j++)   //remover ch de arv
                arv->chave[j] = arv->chave[j+1];
            for(j=i+1; j <= arv->nchaves; j++)  //remover ponteiro para filho[i+1]
                arv->filho[j] = arv->filho[j+1];
            arv->filho[j] = NULL; //Campello
            arv->nchaves--;
            arv->filho[i] = removerB(arv->filho[i], ch, t);
            return arv;
        }
    }

    TAB *y = arv->filho[i], *z = NULL;
    if (y->nchaves == t-1){ //CASOS 3A e 3B
        if((i < arv->nchaves) && (arv->filho[i+1]->nchaves >=t)){ //CASO 3A
            printf("\nCASO 3A: i menor que nchaves\n");
            z = arv->filho[i+1];
            y->chave[t-1] = arv->chave[i];   //dar a y a chave i da arv
            y->nchaves++;
            arv->chave[i] = z->chave[0];     //dar a arv uma chave de z
            int j;
            for(j=0; j < z->nchaves-1; j++)  //ajustar chaves de z
                z->chave[j] = z->chave[j+1];
            //z->chave[j] = 0; Rosseti
            y->filho[y->nchaves] = z->filho[0]; //enviar ponteiro menor de z para o novo elemento em y
            for(j=0; j < z->nchaves; j++)       //ajustar filhos de z
                z->filho[j] = z->filho[j+1];
            z->nchaves--;
            arv->filho[i] = removerB(arv->filho[i], ch, t);
            return arv;
        }
        if((i > 0) && (!z) && (arv->filho[i-1]->nchaves >=t)){ //CASO 3A
            printf("\nCASO 3A: i igual a nchaves\n");
            z = arv->filho[i-1];
            int j;
            for(j = y->nchaves; j>0; j--)               //encaixar lugar da nova chave
                y->chave[j] = y->chave[j-1];
            for(j = y->nchaves+1; j>0; j--)             //encaixar lugar dos filhos da nova chave
                y->filho[j] = y->filho[j-1];
            y->chave[0] = arv->chave[i-1];              //dar a y a chave i da arv
            y->nchaves++;
            arv->chave[i-1] = z->chave[z->nchaves-1];   //dar a arv uma chave de z
            y->filho[0] = z->filho[z->nchaves];         //enviar ponteiro de z para o novo elemento em y
            z->nchaves--;
            arv->filho[i] = removerB(y, ch, t);
            return arv;
        }
        if(!z){ //CASO 3B
            if(i < arv->nchaves && arv->filho[i+1]->nchaves == t-1){
                printf("\nCASO 3B: i menor que nchaves\n");
                z = arv->filho[i+1];
                y->chave[t-1] = arv->chave[i];     //pegar chave [i] e coloca ao final de filho[i]
                y->nchaves++;
                int j;
                for(j=0; j < t-1; j++){
                    y->chave[t+j] = z->chave[j];     //passar filho[i+1] para filho[i]
                    y->nchaves++;
                }
                if(!y->folha){
                    for(j=0; j<t; j++){
                        y->filho[t+j] = z->filho[j];
                    }
                }
                for(j=i; j < arv->nchaves-1; j++){ //limpar referências de i
                    arv->chave[j] = arv->chave[j+1];
                    arv->filho[j+1] = arv->filho[j+2];
                }
                arv->nchaves--;
                arv = removerB(arv, ch, t);
                return arv;
            }
            if((i > 0) && (arv->filho[i-1]->nchaves == t-1)){
                printf("\nCASO 3B: i igual a nchaves\n");
                z = arv->filho[i-1];
                if(i == arv->nchaves)
                    z->chave[t-1] = arv->chave[i-1]; //pegar chave[i] e poe ao final de filho[i-1]
                else
                    z->chave[t-1] = arv->chave[i];   //pegar chave [i] e poe ao final de filho[i-1]
                z->nchaves++;
                int j;
                for(j=0; j < t-1; j++){
                    z->chave[t+j] = y->chave[j];     //passar filho[i+1] para filho[i]
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

int pagina(TAB* arv, char ch, int andar){
    TAB* aux = arv;
    if(aux){
        int i;
        for(i = 0; i <= aux->nchaves-1; i++){
            pagina(aux->filho[i], ch, andar+1);
            if(aux->chave[i] == ch){
                return i;
            }
        }
        pagina(aux->filho[i], ch, andar+1);
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

