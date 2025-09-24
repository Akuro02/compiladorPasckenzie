#include "include/pasckenzie.h"

int main(){
    printf("Analisando...\n");
    nLinha = 1;

    infoAtomo = obterAtomo();
    lookAhead = infoAtomo.atomo;

    program();

    printf("%i linhas analisadas, programa sintaticamente correto\n");
}