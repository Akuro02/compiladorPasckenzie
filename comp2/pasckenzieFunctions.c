#include "include/pasckenzie.h"



TInfoAtomo obter_atomo(){
    TInfoAtomo infoAtomo;

    infoAtomo.atomo = ERRO;

    for(;;){
        if(*buffer == '\0') infoAtomo.atomo = EOS; return infoAtomo;
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

    infoAtomo.linha = nLinha;
    int i = 0;
    while(isalnum(*buffer)){
        lexema[i] = *buffer;
        buffer++;
        i++;
    }
    lexema[i] = '\0';
    if(i == 0){ // caso nao seja alfanumerico
        if(*buffer == '+' || *buffer == '-'){
            infoAtomo.atomo = ADDING_OPERATOR;
        }
        else if(*buffer == '*'){
            infoAtomo.atomo = MULTIPLYING_OPERATOR;
        }
        else if(*buffer == '.'){
            infoAtomo.atomo = KW_PERIOD;
        }
        else if(*buffer == ';'){
            infoAtomo.atomo = kW_SEMICOLON;
        }
        else if(*buffer == ':'){
            infoAtomo.atomo = KW_COLON;
        }
        else if(*buffer == ')'){
            infoAtomo.atomo = KW_CLOSE_PARENTHESIS;
        }
        else if(*buffer == ':' && *buffer+1 == '='){
            buffer++;
            infoAtomo.atomo = ASSIGN;
        }
        buffer++;
        return infoAtomo;
    }
    // LEXEMA AGORA TEM O ATOMO A SER ANALISADO.

    if(islower(*lexema)){
        reconhece_id_kw_const();
        return infoAtomo;
    }
    if(isdigit(*lexema)){
        reconhece_numero(&infoAtomo);
        return infoAtomo;
    }
}

void reconhece_id_kw_const(){
    size_t keyWordCount = sizeof(keywords) / sizeof(kw);
    for(int i = 0; i < keyWordCount; i++){
        if(strcmp(*lexema, keywords[i].keyWord) == 0){
            infoAtomo.atomo = keywords[i].token;
        }
    }

    if (reconhece_id(&infoAtomo) == 0){
        return;
    }

    infoAtomo.atomo = CONSTCHAR;
}

int reconhe_id(TInfoAtomo *infoAtomo){
    int i = 0;
q1:
    if( islower(lexema[i]) || isdigit(lexema[i]) ){
        i++;
        goto q1;
    }
    if( isupper(lexema[i])) 
        return; // sai com erro
    
    infoAtomo->atomo = IDENTIFIER;
    return 0;
}

void reconhece_numero(TInfoAtomo *infoAtomo){
    int i = 0;
q1:
    if( isdigit(lexema[i]) ){
        i++;
        goto q1;
    }
    if( lexema[i] == '.' ){
        i++;
        goto q2;
    }
    return;

q2:
    if( isdigit(lexema[i]) ){
        i++;
        goto q3;
    }
    return;
q3:
    if( isdigit(lexema[i]) ){
        i++;
        goto q3;
    }
    if( isalpha(lexema[i])){
        return;
    }

    infoAtomo->atomo = CONSTINT;

    return;

}