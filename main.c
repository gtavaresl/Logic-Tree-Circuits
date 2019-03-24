
#include "bin_tree.h"

int main(){
    int montagem;
    int testes;
    descritor *circuito=calloc(1,sizeof(descritor));
    setbuf(stdin,NULL);
    fscanf(stdin,"%d",&montagem);
    if(montagem == 0){ // montagem tipo 3-a
        int qtdlinhas;
        setbuf(stdin,NULL);
        fscanf(stdin,"%d",&qtdlinhas);
        for(int i=0; i<qtdlinhas ; i++) add_linha(circuito);
    }else{ // montagem tipo 3-b
        setbuf(stdin,NULL);
        circuito->saida = add_filho(circuito->saida,'p'); // cria a raiz da arvore
        add_rec(circuito->saida);
        conta_entradas(circuito->saida,circuito);
    }
    setbuf(stdin,NULL);
    fscanf(stdin,"%d",&testes);
    for(int i=0;i<testes;i++) printf("%d\n",add_teste(circuito));
    libera(circuito->saida);
    return 0;
}
