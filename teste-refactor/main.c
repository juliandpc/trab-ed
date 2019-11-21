#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "funcoesB.c"
#include "funcoesBPlus.c"
#include "arvoreB.h"
// #include "arvoreBPlus.h"

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
int nivel(node * const root) {
    int h = 0;
    node * c = root;
    while (!c->is_leaf) {
        c = c->pointers[0];
        h++;
    }
    return h;
}
*/


//         /FUNCOES DA ARVORE B        //

////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////
//////////// FUNCOES GERAIS ////////////
////////////////////////////////////////
////////////////////////////////////////
////////////////////////////////////////


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


    TAB* arv = criaB(2);

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

    arv = insereB(arv, 't', 2);
    arv = insereB(arv, 'h', 2);
    arv = insereB(arv, 'e', 2);
    arv = insereB(arv, 'p', 2);
    arv = insereB(arv, 'r', 2);
    arv = insereB(arv, 'o', 2);
    arv = insereB(arv, 'm', 2);
    arv = insereB(arv, 'i', 2);
    arv = insereB(arv, 's', 2);
    arv = insereB(arv, 'e', 2);
    arv = insereB(arv, 'o', 2);
    arv = insereB(arv, 'f', 2);
    arv = insereB(arv, 'q', 2);
    arv = insereB(arv, 'u', 2);
    arv = insereB(arv, 'a', 2);
    arv = insereB(arv, 'n', 2);
    arv = insereB(arv, 't', 2);
    arv = insereB(arv, 'u', 2);
    arv = insereB(arv, 'm', 2);
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
    char ch = 'u';
    int n = pagina(arv, ch, 0);
    printf("a posicao na pagina da chave %c eh: %d", ch, n);


    return 0;
}