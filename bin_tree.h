#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct no{
    char tipo;
    int id;
    int dado:2;
    struct no *dir, *esq;
}porta;

typedef struct{
    porta *saida;
    int entradas;
}descritor;

porta *busca(porta *raiz, char tip, int ide); // busca recursiva e retorna um no
void add_linha(descritor *circuito); // montagem 3-a
porta *add_filho(porta *pai, char lado); // adciona um filho a um lado do pai, retornando o filho
porta *add_rec(porta *pai); // adciona recursivamente filhos (montagem 3-b)
void exibe(porta *no); // funcao que exibe os nos in-ordem
int output(porta *logica); // resolve recursivamente uma porta logica
int add_teste(descritor *circuito); // adciona casos de teste
porta *libera(porta *raiz); // libera a arvore

porta *busca(porta *raiz, char tip, int ide){ // busca recursiva e retorna um no
    if(!raiz) return NULL;
    else if(raiz->tipo==tip && raiz->id == ide) return raiz;
    else{
        porta *aux;
        aux = busca(raiz->esq,tip,ide); // busca na arvore da esquerda
        if(!aux) aux = busca(raiz->dir,tip,ide); // se nao achou, busca na arvore da direita
        return aux;
    }
}

void add_linha(descritor *circuito){ // montagem tipo 3-a
    char tpai;
    int dpai;
    char id[4];
    porta *pai = circuito->saida;
    porta *fesq = (porta *)malloc(sizeof(porta));
    porta *fdir=NULL;
    setbuf(stdin,NULL);
    fscanf(stdin,"%s",id);
    tpai = id[0];
    dpai = atoi(id+1);
    fscanf(stdin,"%s",id);
    fesq->tipo = id[0];
    fesq->id = atoi(id+1);
    if(fesq->tipo == 'E') circuito->entradas++;
    fesq->esq=NULL;
    fesq->dir=NULL;
    if(tpai != 'N'){
        fdir = (porta *)malloc(sizeof(porta));
        fscanf(stdin,"%s",id);
        fdir->tipo = id[0];
        fdir->id = atoi(id+1);
        if(fdir->tipo == 'E') circuito->entradas++;
        fdir->dir=NULL;
        fdir->esq=NULL;
    }
    if(pai == NULL){ // pai deve ser a primeira porta
        pai=(porta *)malloc(sizeof(porta));
        circuito->saida=pai;
        pai->tipo=tpai;
        pai->id=dpai;
        if(pai->tipo == 'E') circuito->entradas++;
    }else{ // buscar o pai
        pai=busca(circuito->saida,tpai,dpai);
    }
    pai->esq=fesq;
    pai->dir=fdir;
}


porta *add_filho(porta *pai, char lado){ // adciona um filho a um lado do pai, retornando o filho
    char id[4];
    porta *filho = (porta *)malloc(sizeof(porta)); // aloca filho
    fscanf(stdin,"%s",id);
    filho->tipo = id[0];
    filho->id = atoi(id+1);
    if(!pai) pai=filho; // se o circuito esta vazio, cria a primeira porta
    else if(lado == 'e') pai->esq=filho; // adciona filho a esquerda
    else pai->dir=filho; // adciona filho a direita
    filho->esq=NULL;
    filho->dir=NULL;
    return filho;
}

porta *add_rec(porta *pai){ // adciona recursivamente filhos (montagem 3-b)
    if(!pai) return NULL;
    else if(pai->tipo == 'E') return NULL;
    else{
        porta *filho;
        if(!pai->esq) filho = add_filho(pai,'e');
        add_rec(filho);
        if(!pai->dir && pai->tipo != 'N') filho = add_filho(pai,'d');
        add_rec(filho);
        return NULL;
    }
}

void exibe(porta *raiz){ // funcao que exibe os nos in-ordem
    // foi utilizada para visualizar a arvore
    if(raiz == NULL) return;
    exibe(raiz->esq);
    printf("%c%d = %d\n",raiz->tipo,raiz->id,raiz->dado);
    exibe(raiz->dir);
}

void conta_entradas(porta *no, descritor *circuito){ // conta a quantidade de entradas no circuito
    if(no == NULL) return;
    if(no->tipo == 'E') circuito->entradas++; // incrementa na variavel de quantidade de entradas
    conta_entradas(no->esq,circuito); // conta na arvore da esquerda
    conta_entradas(no->dir,circuito); // conta na arvore da direita
}

int output(porta *logica){ // resolve recursivamente uma porta logica
    if(!logica) return 0;
    char tip=logica->tipo;
    if(tip=='A'){ // AND
        return (output(logica->esq) && output(logica->dir));
    }else if(tip=='O'){ // OR
        return (output(logica->esq) || output(logica->dir));
    }else if(tip=='D'){ // NAND
        return !(output(logica->esq) && output(logica->dir));
    }else if(tip=='R'){ // NOR
        return !(output(logica->esq) || output(logica->dir));
    }else if(tip=='X'){ // XOR
        return (output(logica->esq) ^ output(logica->dir));
    }else if(tip=='N'){ // NOT
        return (!output(logica->esq));
    }else if(tip=='E'){ // Entrada
        return logica->dado;
    }
    return -1;
}


void aplica_entradas(porta *raiz, int *in){
    if(raiz == NULL) return;
    if(raiz->tipo == 'E') raiz->dado = in[raiz->id]; // incrementa na variavel de quantidade de entradas
    aplica_entradas(raiz->esq,in); // aplica na arvore da esquerda
    aplica_entradas(raiz->dir,in); // conta na arvore da direita
}

int add_teste(descritor *circuito){ // adciona casos de teste
    int in[circuito->entradas];
    setbuf(stdin,NULL);
    for(int i=0;i<circuito->entradas;i++) fscanf(stdin,"%d",&in[i]); // le a entrada em ordem
    aplica_entradas(circuito->saida,in); // aplica as entradas
    return output(circuito->saida); // resolve o circuito
}

porta *libera(porta *raiz){ // libera a arvore
    if(raiz){
        libera(raiz->esq); // libera toda a arvore da esquerda
        libera(raiz->dir); // libera arvore da direita
        free(raiz); // libera sua raiz
    }
    return NULL;
}
