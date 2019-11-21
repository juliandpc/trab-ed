
typedef struct ArvBM{
    int nchaves;
    char folha, *chave;
    struct ArvBM **filho, *prox;
}TABM;