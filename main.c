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
/*
int paginaDaqueleNivelB(TAB* arv, char ch, int andar){
    TAB* aux = arv;

    if(aux){
        int j = 0;
        while(aux->filho[j] != NULL){
            j++;
        }
        printf("a chave %c tem %d filhos\n", aux->chave[0], j);

        return paginaDaqueleNivelB(arv->filho[0], ch, andar+1);
    }

}
*/

/*
char *criaCriptografia(char frase[TAM], int tipoArv, int d){
    if(tipoArv == 1){
        TAB *arvB = montaArvoreB(d, frase);
    }else if(tipoArv == 2){
        TABM *arvBM = montaArvoreBM(d, frase);
    }else{
        return("Tipo de arvore invalida");
    }
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

int main() {
    char frase[TAM];
    char fraseWithSpaces[TAM];
    char vogais[5] = "aeiou";
    char consoantes[21] = "bcdfghjklmnpqrstvwxyz";
    int tipoArv;
    int acao;
    int ordem = 2;

    int continuar = 1;
    while(continuar){

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

        TAB* arv = criaB(ordem);

        switch(tipoArv){
            case 1:

                for(int i=0;i<strlen(frase);i++){
                    arv = insereB(arv,  frase[i], ordem);
                }

                imprimeArvB(arv, 0);
                char* encriptacao = encriptaFrase(frase, arv);
                printf("Frase: %s| encriptado: %s \n", fraseWithSpaces, encriptacao);
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

        printf("Com as arvores montadas e a mensagem criptografada, o que voce deseja fazer agora? \n");
        printf("(1) Comparar o codigo gerado \n");
        //printf("(2) Buscar informacoes subordinadas \n");
        //printf("(3) Alterar a frequencia de uma letra \n");
        printf("(2) Buscar todas as consoantes \n");
        printf("(3) Buscar todas as vogais \n");
        printf("(4) Remover todas as consoantes \n");
        printf("(5) Remover todas as vogais \n");
        printf("(0) Sair \n");
        scanf("%d", &acao);

        switch(acao){
            case 1:
                printf("arvore B \n" ); 
                break;
            case 4:
            for(int elem=0; elem<strlen(consoantes); elem++){
                retiraB(arv, consoantes[elem], ordem);
            }
            case 5:
            for(int elem=0; elem<strlen(vogais); elem++){
                retiraB(arv, vogais[elem], ordem);
            }       
            case 0:
            continuar == 0;
                break;
        }
    }
    return 0;
}
