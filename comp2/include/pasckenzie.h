#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h> //atof
#include <stddef.h>

typedef enum{
    ERRO,
    KW_PROGRAM,
    kW_SEMICOLON,
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

// VARIAVEIS GLOBAIS ------------------------
char lexema[20];
int nLinha;
char* buffer = "";

TAtomo lookAhead;
TInfoAtomo infoAtomo;

typedef struct{
    const char *keyWord;
    TAtomo token;
} kw;

static const kw keywords [] = {
    {"div", MULTIPLYING_OPERATOR},
    {"or", RELATIONAL_OPERATOR},
    {"and", RELATIONAL_OPERATOR},
    {"not", KW_NOT},
    {"if", KW_IF},
    {"then", KW_THEN},
    {"else", KW_ELSE},
    {"while", KW_WHILE},
    {"do", KW_DO},
    {"begin", KW_BEGIN},
    {"end", KW_END},
    {"read", KW_READ},
    {"write", KW_WRITE},
    {"var", KW_VAR},
    {"program", KW_PROGRAM},
    {"true", KW_TOF},
    {"false", KW_TOF},
    {"char", CONSTCHAR},
    {"integer", CONSTINT},
    {"boolean", TYPE}
};
// ------------------------------------------

TInfoAtomo obter_atomo();
void reconhece_id_kw_const();
int reconhe_id(TInfoAtomo *infoAtomo);
void reconhece_numero(TInfoAtomo *infoAtomo);
