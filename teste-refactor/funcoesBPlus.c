#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "arvoreBPlus.h"


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