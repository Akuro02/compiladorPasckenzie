#include "include/pasckenzie.h"

// VARIAVEIS GLOBAIS ------------------------
char lexema[16];
int nLinha;

char* buffer = "\n\n\n (* This is a comment, \nI eat a bug *)\nprogram ex1;\n";

TAtomo lookAhead;
TInfoAtomo infoAtomo;
// ------------------------------------------

keyword keywords[] = {
    {"program", KW_PROGRAM},
    {"var", KW_VAR},
    {"begin", KW_BEGIN},
    {"boolean", TYPE},
    {"char", TYPE},
    {"integer", TYPE},
    {"read", KW_READ},
    {"write", KW_WRITE},
    {"if", KW_IF},
    {"else", KW_ELSE},
    {"then", KW_THEN},
    {"while", KW_WHILE},
    {"do", KW_DO},
    {"not", KW_NOT},
    {"true", KW_TOF},
    {"false", KW_TOF},
    {"end", KW_END}
    };


void obter_atomo(TInfoAtomo* infoA){
    infoA->atomo = ERRO;

    for(;;){
        if(*buffer == '\0') {
            infoA->atomo = EOS; 
            return;
        }
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
            int linhaIni = nLinha;
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
            printf("%i-%i: Comentario\n", linhaIni, nLinha);
            continue;
        }
        break;
    }

    
    infoA->linha = nLinha;

    if(islower(*buffer) || *buffer == '_'){
        reconhece_id(infoA);
    }








    return;
}

void reconhece_id(TInfoAtomo* infoA){
    const char *ini_lexema = buffer;
    int size = 0;

    q1:
        if(size++ >= 15){printf("Erro na linha %i - Identifier grande demais\n", nLinha); return;}
        if(isalnum(*buffer) || *buffer == '_'){
            buffer++;
            goto q1;
        }
        if(isupper(*buffer))
            return;
        
        strncpy(lexema, ini_lexema,buffer-ini_lexema); // salvamos ini_lexema como inicio do buffer e salvamos deste ponto até onde o buffer esta agora (uma forma de copiar a string)
        lexema[buffer-ini_lexema] = '\0';
        strcpy(infoA->atributo.id, lexema);
        size_t keywordSize = sizeof(keywords)/sizeof(keyword);
        if(lexema[0] != '_'){ // palavras chave nunca começam com _, então se a string nova começar com _ ela pode só pular a verificação
            for(int i = 0; i < keywordSize; i++){
                if(strcmp(lexema, keywords[i].keyword) == 0){
                    printf("%i: %s\n", infoAtomo.linha, keywords[i].keyword);
                    infoA->atomo = keywords[i].value;
                    return;
                }
            }
        }
        printf("%i: identifier: %s\n", infoAtomo.linha, infoA->atributo.id);
        infoA->atomo = IDENTIFIER;
        return;

}

int main(){
    printf("Analisando...\n");
    nLinha = 1;

    obter_atomo(&infoAtomo);
    obter_atomo(&infoAtomo);
    lookAhead = infoAtomo.atomo;

    printf("%i linhas analisadas, programa sintaticamente correto\n", nLinha);
}