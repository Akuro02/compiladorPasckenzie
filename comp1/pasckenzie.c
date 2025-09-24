#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h> //atof

typedef enum{
    ERRO,
    PROGRAMID,
    IDENTIFIER,
    CONSTINT,
    KW_VAR,
    KW_BEGIN,
    KW_PERIOD,
    KW_COMMA,
    KW_DOUBLEDOT,
    KW_SEMICOLON,
    KW_OPENPARENTHESIS,
    KW_CLOSEPARENTHESIS,
    KW_READ,
    KW_WRITE,
    KW_IF, KW_THEN, KW_ELSE,
    KW_WHILE, KW_DO,
    KW_END,
    ASSIGNMENT,
    RELATIONAL_OPERATOR,
    ADDING_OPERATOR,
    MULTIPLYING_OPERATOR,
    KW_NOT,
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
char* buffer = "program teste;\nvar\n	num:integer;\nbegin\n	write(num)\nend.";

TAtomo lookAhead;
TInfoAtomo infoAtomo;

TInfoAtomo obterAtomo();

void consome(TAtomo atomo);


void reconhece_ID_ou_Type(TInfoAtomo* infoAtomo);
void block();
void program();
void variable_declaration_part();
void variable_declaration();
void statement_part();
void statement();
void assingnment_statement();
void read_statement();
void write_statement();
void if_statement();
void while_statement();
void factor();
void term();
void simple_expression();
void expression();
void reconhece_numero(TInfoAtomo *infoAtomo);


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
    // Fazer as comparações

   // printf("Linha %i\n", nLinha); TODO
    if(islower(*buffer)){
        reconhece_ID_ou_Type(&infoAtomo);
    }
    if(isdigit(*buffer)){
        reconhece_numero(&infoAtomo);
    }
    else if(*buffer == ';'){
        buffer++;
        infoAtomo.atomo = KW_SEMICOLON;
    }
    else if(*buffer == '.'){
        buffer++;
        infoAtomo.atomo = KW_PERIOD;
    }
    else if(*buffer == ','){
        buffer++;
        infoAtomo.atomo = KW_COMMA;
    }
    else if(*buffer == ':'){
        buffer++;
        infoAtomo.atomo = KW_DOUBLEDOT;
    }
    else if(*buffer == '('){
        buffer++;
        infoAtomo.atomo = KW_OPENPARENTHESIS;
    }
    else if(*buffer == ')'){
        buffer++;
        infoAtomo.atomo = KW_CLOSEPARENTHESIS;
    }
    else if(*buffer == ':' && *buffer+1 == '='){
        buffer += 2;
        infoAtomo.atomo = ASSIGNMENT;
    }
    else if(*buffer == '+' || *buffer == '-'){
        buffer++;
        infoAtomo.atomo = ADDING_OPERATOR;
    }
    else if(*buffer == '*' || *buffer == '/'){
        buffer++;
        infoAtomo.atomo = MULTIPLYING_OPERATOR;
    }
    else if(*buffer == '\0'){
        infoAtomo.atomo = EOS;
    }
    return infoAtomo;


}

void reconhece_ID_ou_Type(TInfoAtomo* infoAtomo){
    char* ini_lexema = buffer;

q1:
    if( islower(*buffer) || isdigit(*buffer) ){
        buffer++;
        goto q1;
    }
    printf("\n");
    if( isupper(*buffer)) 
        return; // sai com erro
    
    // preenche o atributo do atomo IDENTIFICADOR
    strncpy(infoAtomo->atributo.id,ini_lexema,buffer-ini_lexema);
    infoAtomo->atributo.id[buffer-ini_lexema] = '\0'; // aqui temos um IDENTIFICADOR


    if(strcmp(infoAtomo->atributo.id, "program") == 0){
        infoAtomo->atomo = PROGRAMID;
        printf("PROGRAM");
        return;
    }

    if(strcmp(infoAtomo->atributo.id, "char") == 0 || strcmp(infoAtomo->atributo.id, "integer") == 0 || strcmp(infoAtomo->atributo.id, "boolean") == 0){
        infoAtomo->atomo = TYPE;
        return;
    }

    if(strcmp(infoAtomo->atributo.id, "var") == 0){
        infoAtomo->atomo = KW_VAR;
        return;
    }

    if(strcmp(infoAtomo->atributo.id, "begin") == 0){
        infoAtomo->atomo = KW_BEGIN;
        return;
    }

    if(strcmp(infoAtomo->atributo.id, "end") == 0){
        infoAtomo->atomo = KW_END;
        return;
    }

    if(strcmp(infoAtomo->atributo.id, "read") == 0){
        infoAtomo->atomo = KW_READ;
        return;
    }

    if(strcmp(infoAtomo->atributo.id, "write") == 0){
        infoAtomo->atomo = KW_WRITE;
        return;
    }

    if(strcmp(infoAtomo->atributo.id, "if") == 0){
        infoAtomo->atomo = KW_IF;
        return;
    }

    if(strcmp(infoAtomo->atributo.id, "else") == 0){
        infoAtomo->atomo = KW_ELSE;
        return;
    }

    if(strcmp(infoAtomo->atributo.id, "then") == 0){
        infoAtomo->atomo = KW_THEN;
        return;
    }

    if(strcmp(infoAtomo->atributo.id, "while") == 0){
        infoAtomo->atomo = KW_WHILE;
        return;
    }

    if(strcmp(infoAtomo->atributo.id, "do") == 0){
        infoAtomo->atomo = KW_DO;
        return;
    }

    if(strcmp(infoAtomo->atributo.id, "not") == 0){
        infoAtomo->atomo = KW_NOT;
        return;
    }

    if(strcmp(infoAtomo->atributo.id, "true") == 0 || strcmp(infoAtomo->atributo.id, "false") == 0){
        infoAtomo->atomo = FACTOR;
        return;
    }

   const char *relationalOps[] = {"<>", "<" , "<=" ,  ">=" , ">" , "=" , "or" , "and" };

    for(int i = 0; i < 8; i++){
        if(strcmp(infoAtomo->atributo.id, relationalOps[i]) == 0){
            infoAtomo->atomo = RELATIONAL_OPERATOR;
            return;
        }
    }

    infoAtomo->atomo = IDENTIFIER;
    printf("IDENTIFIER");

    return ;
}

void reconhece_numero(TInfoAtomo *infoAtomo){
    char *ini_lexema = buffer;
q1:
    if( isdigit(*buffer) ){
        buffer++;
        goto q1;
    }
    if( *buffer == '.' ){
        buffer++;
        goto q2;
    }
    return;

q2:
    if( isdigit(*buffer) ){
        buffer++;
        goto q3;
    }
    return;
q3:
    if( isdigit(*buffer) ){
        buffer++;
        goto q3;
    }
    if( isalpha(*buffer)){
        return;
    }
    // recorta lexama 
    strncpy(lexema,ini_lexema,buffer-ini_lexema);
    lexema[buffer-ini_lexema] = '\0'; // aqui temos uma string

    infoAtomo->atributo.numero = atof(lexema);

    infoAtomo->atomo = CONSTINT;

    return;

}

void consome( TAtomo atomo ){
    if( lookAhead == atomo ){
        infoAtomo = obterAtomo();
        lookAhead = infoAtomo.atomo;
    }
    else{
        printf("\n#%2d:Erro sintatico: \n",infoAtomo.linha);
        exit(1);
    }
}

void program(){
    printf("Program:\n");
    consome(PROGRAMID);
    consome(IDENTIFIER);

    consome(KW_SEMICOLON);
    
    block();
    consome(KW_PERIOD);
    printf("Exit Program:\n");
}

void block(){
    printf("block\n");
    variable_declaration_part();
    statement_part();
    printf("exit block\n");
}

void variable_declaration_part(){
    printf("variable_declaration_part\n");
    if(infoAtomo.atomo != KW_VAR) return;
    consome(KW_VAR);
    
    variable_declaration();
    consome(KW_SEMICOLON);
    
    while(infoAtomo.atomo != KW_BEGIN){
        variable_declaration();
        consome(KW_SEMICOLON);
    }
   printf("exit variable_declaration_part\n");
}

void variable_declaration(){
    printf("variable_declaration\n");
    consome(IDENTIFIER);
    
    while(infoAtomo.atomo == KW_COMMA){
        consome(KW_COMMA);
        consome(IDENTIFIER);
    }
    consome(KW_DOUBLEDOT);
    
    consome(TYPE);
    printf("exit variable_declaration\n");
}

void statement_part(){
    printf("statement_part\n");
    if(infoAtomo.atomo != KW_BEGIN){consome(ERRO);}
    consome(KW_BEGIN);
    
    statement();
    while(infoAtomo.atomo == KW_SEMICOLON){
        consome(KW_SEMICOLON);
        statement();
    }
    printf("exit statement_part\n");
}

void statement(){
    printf("statement\n");
    if(infoAtomo.atomo == IDENTIFIER){
        assingnment_statement();
        return;
    }
    if(infoAtomo.atomo == KW_READ){
        read_statement();
        return;
    }
    if(infoAtomo.atomo == KW_WRITE){
        write_statement();
        return;
    }
    if(infoAtomo.atomo == KW_IF){
        if_statement();
        return;
    }
    if(infoAtomo.atomo == KW_WHILE){
        while_statement();
        return;
    }
    statement_part();
    printf("exit statement\n");
}

void assingnment_statement(){
    printf("assingnment_statement\n");
    consome(IDENTIFIER);
    consome(ASSIGNMENT);
    expression();
    printf("exit assingnment_statement\n");
}

void read_statement(){
    printf("read_statement\n");
    consome(KW_READ);
    consome(KW_OPENPARENTHESIS);
    consome(IDENTIFIER);
    while(infoAtomo.atomo == KW_COMMA){
        consome(KW_COMMA);
        consome(IDENTIFIER);
    }
    consome(KW_CLOSEPARENTHESIS);
    printf("exit read_statement\n");
    return;
}

void write_statement(){
    printf("write_statement\n");
    consome(KW_WRITE);
    consome(KW_OPENPARENTHESIS);
    consome(IDENTIFIER);
    while(infoAtomo.atomo == KW_COMMA){
        consome(KW_COMMA);
        consome(IDENTIFIER);
    }
    consome(KW_CLOSEPARENTHESIS);
    printf("exit write_statement\n");
    return;
}

void if_statement(){
    printf("if_statement\n");
    consome(KW_IF);
    expression();
    consome(KW_THEN);
    statement();
    if(infoAtomo.atomo == KW_ELSE){
        consome(KW_ELSE);
        statement();
    }
    printf("exit if_statement\n");
}

void while_statement(){
    printf("while_statement\n");
    consome(KW_WHILE);
    expression();
    consome(KW_DO);
    statement();
    printf("exit while_statement\n");
}

void expression(){
    printf("expression\n");
    simple_expression();
    if (infoAtomo.atomo == RELATIONAL_OPERATOR){
        consome(RELATIONAL_OPERATOR);
        simple_expression();
    }
    printf("exit expression\n");
}

void simple_expression(){
    printf("simple_expression\n");
    term();
    if(infoAtomo.atomo == ADDING_OPERATOR){
        consome(ADDING_OPERATOR);
        term();
    }
    printf("exit simple_expression\n");
}

void term(){
    printf("term\n");
    factor();
    while(infoAtomo.atomo == MULTIPLYING_OPERATOR){
        consome(MULTIPLYING_OPERATOR);
        factor();
    }
    printf("exit term\n");
}

void factor(){
    printf("factor\n");
    if(infoAtomo.atomo == IDENTIFIER){
        consome(IDENTIFIER);
        printf("exit factor\n");
        return;
    }
    if(infoAtomo.atomo == CONSTINT){
        consome(CONSTINT);
        printf("exit factor\n");
        return;
    }
    if(infoAtomo.atomo == KW_OPENPARENTHESIS){
        consome(KW_OPENPARENTHESIS);
        expression();
        consome(KW_CLOSEPARENTHESIS);
        printf("exit factor\n");
        return;
    }
    if(infoAtomo.atomo == KW_NOT){
        consome(KW_NOT);
        factor();
        printf("exit factor\n");
        return;
    }
    if(infoAtomo.atomo == FACTOR){
        consome(FACTOR);
        printf("exit factor\n");
        return;
    }
    else{
        printf("exit factor\n");
        consome(ERRO);
    }
}


int main(){
    printf("Analisando:\n");
    nLinha = 1;

    infoAtomo = obterAtomo();
    lookAhead = infoAtomo.atomo;

    program();

    printf("Funciona!\n");
}