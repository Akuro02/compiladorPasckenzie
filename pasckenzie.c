#include <stdio.h>

typedef enum{
    ERRO,
    IDENTIFIER,
    TYPE,
    FACTOR,
    EOS
}TAtomo;

typedef struct{
    TAtomo atomo;
    int linha;
    union{
        int numero; // constint
        char id[16]; // identifier
        char ch; // atomo constchar
    }atributo;
}TInfoAtomo;

char lexema[20];
int nLinha;
char* buffer = "program teste;\nvar\n	num:integer;\nbegin\n	write(num);\nend.";

TAtomo lookAhead;
TInfoAtomo infoAtomo;

TInfoAtomo obterAtomo();

void consome(TAtomo atomo);

void gramatica();


int main(){
    printf("Analisando:\n");
    nLinha = 1;

    infoAtomo = obterAtomo();
}


TInfoAtomo obterAtomo(){
    TInfoAtomo infoAtomo;

    infoAtomo.atomo = ERRO;

    for(;;){
        if(*buffer == '\0') break;
        if(*buffer == ' ' || *buffer == '\t'){
            buffer++;
            continue;
        }
        if(*buffer == '\n'){
            nLinha++;
            buffer++;
            continue;
        }

        if(buffer[0] == '(' && buffer[1] == '*'){
            buffer += 2;
            while(*buffer != '\0'){
                if(buffer[0] == '*' && buffer[1] == ')'){
                    buffer += 2;
                    break;
                }
                if(*buffer == '\n'){
                    nLinha++;
                }
                buffer++;
            }
            continue;
        }
        break;
    }

    infoAtomo.linha = nLinha;

    if(*buffer == '\0'){
        infoAtomo.atomo = EOS;
        return infoAtomo;
    }

    // Fazer as comparações

    printf("Linha %i\n", nLinha);
    return infoAtomo;


}