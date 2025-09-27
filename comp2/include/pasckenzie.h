#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h> //atof
#include <stddef.h>

typedef enum{
    ERRO,
    KW_PROGRAM,
    kW_SEMICOLON,
    KW_COMMA,
    KW_COLON,
    KW_OPEN_PARENTHESIS,
    KW_CLOSE_PARENTHESIS,
    KW_PERIOD,
    CONSTINT,
    CONSTCHAR,
    KW_VAR,
    KW_BEGIN,
    IDENTIFIER,
    TYPE,
    KW_READ,
    KW_WRITE,
    KW_IF,
    KW_ELSE,
    KW_THEN,
    KW_WHILE,
    KW_DO,
    RELATIONAL_OPERATOR,
    ADDING_OPERATOR,
    MULTIPLYING_OPERATOR,
    KW_NOT,
    KW_TOF,
    KW_END,
    ASSIGN,
    EOS
} TAtomo;

typedef struct{
    TAtomo atomo;
    int linha;
    union{
        int constInt;
        char id[16];
        char constChar;
    }atributo;
}TInfoAtomo;


// Struct para reconhecer palavras chave -----
typedef struct{
    char *keyword;
    TAtomo value;
} keyword;
// ------------------------------------------

void obter_atomo();
void reconhece_id(TInfoAtomo* infoAtomo);
