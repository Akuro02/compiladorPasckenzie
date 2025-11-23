#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h> //atof
#include <math.h>
#include <stddef.h>

#define PRIME_NUMBER 211

typedef struct _TNo{
    char ID[16];
    int endereco;
    struct _TNo *prox;
}  TNo;

typedef struct {
    TNo *entradas[PRIME_NUMBER];
} TTabelaSimbolos;



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

const char *strMensagem[] = {
    "ERRO",
    "KW_PROGRAM",
    "kW_SEMICOLON: ';'",
    "KW_COMMA: ','",
    "KW_COLON: ':'",
    "KW_OPEN_PARENTHESIS: '('",
    "KW_CLOSE_PARENTHESIS: ')'",
    "KW_PERIOD: '.'",
    "CONSTINT",
    "CONSTCHAR",
    "KW_VAR",
    "KW_BEGIN",
    "IDENTIFIER",
    "TYPE",
    "KW_READ",
    "KW_WRITE",
    "KW_IF",
    "KW_ELSE",
    "KW_THEN",
    "KW_WHILE",
    "KW_DO",
    "RELATIONAL_OPERATOR",
    "ADDING_OPERATOR",
    "MULTIPLYING_OPERATOR",
    "KW_NOT",
    "KW_TOF",
    "KW_END",
    "ASSIGN",
    "EOS"
};

typedef struct{
    TAtomo atomo;
    int linha;
    union{
        float constInt;
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

TInfoAtomo obter_atomo();
void reconhece_id(TInfoAtomo* infoAtomo);
void reconhece_num(TInfoAtomo *infoAtomo);
void reconhece_char(TInfoAtomo *infoA);
void reconhece_specialChars(TInfoAtomo *infoAtomo);
void consome(TAtomo atomo);

void program();
void block();
void variable_declaration_part();
void variable_declaration();
void statement_part();
void statement();
void factor();
void term();
void simple_expression();
void expression();
void assignment_statement();
void read_statement();
void write_statement();
void if_statement();
void while_statement();

void init_tabela_simbolos();
int hashMack(char *str);
void insere_tabela_simbolos(char *id);
int busca_tabela_simbolos(char *id);
int proximo_rotulo();
void printfTabelaSimbolos();