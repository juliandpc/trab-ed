#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

#define TAM 60


////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
////////////// STRUCTS /////////////////
////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
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

//  ^^^^^^ STRUCTS ^^^^^^


////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
///////// FUNCOES DA ARVORE B //////////
////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////

//da vanessa
/*
TAB *criaB(int d){
    TAB* novo = (TAB*)malloc(sizeof(TAB));
    novo->nchaves = 0;
    novo->chave =(char*)malloc(sizeof(char*)*(d*2));
    novo->folha = 1;
    novo->filho = (TAB**)malloc(sizeof(TAB*)*((d*2)+1));
    int i;
    for(i=0; i<((d*2)+1); i++) novo->filho[i] = NULL;
    return novo;
}
*/

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

int paginaDaqueleNivel(TAB* arv, char ch, int filhos){
    TAB* aux = arv;
    if(aux) {
        int i = 0;

        while(arv->filho[filhos] != NULL){
            printf("isso eh o q? %c\n", arv->filho[filhos]->chave[0]);
            while(i < aux->filho[filhos]->nchaves && ch >aux->filho[filhos]->chave[i]){

                if(i < aux->filho[filhos]->nchaves && ch == aux->filho[filhos]->chave[i]){
                    //printf("ta encontrando!!\n");
                    return filhos;
                }
                i++;
            }

            //filhos++;
            return paginaDaqueleNivel(aux, ch, filhos + 1);
        }
        //teste++;
        return paginaDaqueleNivel(aux->filho[i], ch, filhos);
    }
}

int nivel(TAB* arv, char ch, int andar){
    TAB* aux = arv;
    if(aux){
        int i = 0;
        while(i < aux->nchaves && ch >aux->chave[i]){
            i++;
        }
        if(i < aux->nchaves && ch == aux->chave[i]){
            return andar;
        }

        return nivel(aux->filho[i], ch, andar+1);
    }
}

int pagina(TAB* arv, char ch){
    TAB* aux = arv;
    if(aux){
        int i = 0;
        while(i < aux->nchaves && ch >aux->chave[i]){
            i++;
        }
        if(i < aux->nchaves && ch == aux->chave[i]){
            return i;
        }
        return pagina(aux->filho[i], ch);
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



//         /FUNCOES DA ARVORE B        //

////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
///////// FUNCOES DA ARVORE B+ /////////
////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////

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




//        /FUNCOES DA ARVORE B+        //

////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
//////////// FUNCOES GERAIS ////////////
////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////

char *criaCriptografia(char frase[TAM], int tipoArv, int d){
    if(tipoArv == 1){
        TAB *arvB = montaArvoreB(d, frase);
    }else if(tipoArv == 2){

    }else{
        return("Tipo de arvore invalida");
    }
}



int main() {

    char frase[TAM];
    int tipoArv;
    int d;
    int tipoInsercao;
    int acao;
/*
    int continuar = 1;
    while(continuar){

        printf("Digite a frase que deseja criptografar: \n");
        scanf("%[^\n]", frase);
        printf("\n");

        printf("Qual a ordem (d) da arvore selecionada? \n");
        scanf("%d", &d);

        printf("Com que tipo de arvore voce deseja criptografar a mensagem? \n");
        printf("(1) Arvore B \n");
        printf("(2) Arvore B+ \n");
        printf("(0) Sair \n");
        scanf("%d", &tipoArv);

        switch(tipoArv){
            case 1:
                imprimeArvB(montaArvoreB(d, frase),0);
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

    printf("esse eh o d: %d", d);
    printf("essa eh a arvri: %d", tipoArv);
*/

/*
    printf("\n");
    printf("\n");
    int i = 0;
    while(frase[i] != '\0'){
        printf("%c", frase[i]);
        i++;
    }

*/
    //printf("%s", frase);
    int ordem = 2;

    TAB* arv = criaB(ordem);

    /*
    arv = insereB(arv, 't', 2);
    arv = insereB(arv, 'e', 2);
    arv = insereB(arv, 's', 2);
    arv = insereB(arv, 't', 2);
    arv = insereB(arv, 'a', 2);
    arv = insereB(arv, 'n', 2);
    arv = insereB(arv, 'd', 2);
    arv = insereB(arv, 'o', 2);
     */
    //arv = insereB(arv, 'b', 2);
    //arv = insereB(arv, 'c', 2);
    //arv = insereB(arv, 'o', 2);

    arv = insereB(arv, 't', ordem);
    arv = insereB(arv, 'h', ordem);
    arv = insereB(arv, 'e', ordem);
    arv = insereB(arv, 'p', ordem);
    arv = insereB(arv, 'r', ordem);
    arv = insereB(arv, 'o', ordem);
    arv = insereB(arv, 'm', ordem);
    arv = insereB(arv, 'i', ordem);
    arv = insereB(arv, 's', ordem);
    arv = insereB(arv, 'e', ordem);
    arv = insereB(arv, 'o', ordem);
    arv = insereB(arv, 'f', ordem);
    arv = insereB(arv, 'q', ordem);
    arv = insereB(arv, 'u', ordem);
    arv = insereB(arv, 'a', ordem);
    arv = insereB(arv, 'n', ordem);
    arv = insereB(arv, 't', ordem);
    arv = insereB(arv, 'u', ordem);
    arv = insereB(arv, 'm', ordem);
    /*
    arv = insereB(arv, 'x', 2);
    arv = insereB(arv, 'w', 2);
    arv = insereB(arv, 'c', 2);
    arv = insereB(arv, 'b', 2);
    arv = insereB(arv, 'd', 2);
    arv = insereB(arv, 'k', 2);
    arv = insereB(arv, 'l', 2);
    arv = insereB(arv, 'r', 2);
    arv = insereB(arv, 'z', 2);
    arv = insereB(arv, 'v', 2);
    */

    imprimeArvB(arv, 0);
    //int numero = pagina(arv, 'a');
    //printf("esse numero eh: %d\n", numero);

    char ch = 's';

    int n = pagina(arv, ch);
    //printf("%d", n);
    printf("a posicao na pagina da chave >%c< eh: %d\n\n", ch, n);

    int n2 = nivel(arv, ch, 0);
    printf("o nivel da chave %c eh: %d\n\n", ch, n2);

    int x = paginaDaqueleNivel(arv, ch, 0);
    printf("ta na pagina: %d\n",x);
    return 0;
}
