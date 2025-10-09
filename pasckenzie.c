// AUTORES
// Marco Antonio de Camargo - 10418309
// Nicolas Henriques de Almeida - 10418357

#include "include/pasckenzie.h"

// VARIAVEIS GLOBAIS ------------------------
char lexema[16];
int nLinha;

char* buffer;

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


static int buffer_read_file(char * filename){
    FILE *f = fopen(filename, "rb");
    if(f == NULL){
        char errorMessage[200]; sprintf(errorMessage, "Erro ao abrir arquivo %s\n", filename);
        perror(errorMessage); 
        exit(1);}
    fseek(f, 0, SEEK_END);
    size_t size = ftell(f); // pointer foi para o fim do arquivo - ftell determina a posicao do ponteio a partir do inicio (isso é como fazer size = fim do ponteiro - inicio do ponteiro)
    rewind(f);
    char* buffer_start = (char*)malloc(size+1);
    if(!buffer_start){perror("Falha ao criar buffer\n"); fclose(f); exit(1);}
    size_t r = fread(buffer_start, 1, size, f);
    fclose(f);
    buffer_start[r] = '\0';
    // Remover \r que podem vir acompanhados dos \n quando utiliza Windows
    char *src = buffer_start, *dst = buffer_start;
    while (*src) {
        if (*src != '\r') {
            *dst++ = *src;
        }
        src++;
    }
    *dst = '\0';
    //
    buffer = buffer_start;
    return 0;
}


TInfoAtomo obter_atomo(){
    TInfoAtomo infoA;
    infoA.atomo = ERRO;

    for(;;){
        if(*buffer == '\0') {
            infoA.atomo = EOS; 
            return infoA;
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

    infoA.linha = nLinha;

    printf("DEBUG: buffer atual: '%c'\n", *buffer);  

    if(islower(*buffer)|| isupper(*buffer) || *buffer == '_'){
        reconhece_id(&infoA);
    }
    else if(isdigit(*buffer)){
        reconhece_num(&infoA);
    }
    else if(*buffer == '\''){
        reconhece_char(&infoA);
    }
    else{
        reconhece_specialChars(&infoA);
    }

    return infoA;
}

void consome(TAtomo atomo){
    if(lookAhead == atomo){
        infoAtomo = obter_atomo();
        lookAhead = infoAtomo.atomo;
    }else{
        printf("\n#%2d:Erro sintatico\nEsperado[%s] - Entregue[%s]",infoAtomo.linha, strMensagem[atomo], strMensagem[lookAhead]);
        exit(1);
    }
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
        printf("DEBUG: lexema copiado: %s\n", lexema);
        size_t keywordSize = sizeof(keywords)/sizeof(keyword);
        if(lexema[0] != '_'){ // palavras chave nunca começam com _, então se a string nova começar com _ ela pode só pular a verificação
            for(int i = 0; i < keywordSize; i++){
                if(strcmp(lexema, keywords[i].keyword) == 0){
                    printf("%i: %s\n", infoA->linha, keywords[i].keyword);
                    infoA->atomo = keywords[i].value;
                    return;
                }
            }
        }
        printf("%i: %s: %s\n", infoA->linha, strMensagem[IDENTIFIER] ,infoA->atributo.id);
        infoA->atomo = IDENTIFIER;
        return;

}

void reconhece_num(TInfoAtomo *infoAtomo){
    char *ini_lexema = buffer;

    q1:
        if(isdigit(*buffer)){
            buffer++;
            goto q1;
        }
        if(*buffer == '.'){
            buffer++;
            goto q2;
        }
        goto q3;
    q2:
        if(isdigit(*buffer)){
            buffer++;
            goto q3;
        }
        return;
    q3:
        if(isdigit(*buffer)){
            buffer++;
            goto q3;
        }
        if(isalpha(*buffer)){
            return;
        }
    
        strncpy(lexema, ini_lexema, buffer-ini_lexema);
        lexema[buffer-ini_lexema] = '\0';

        infoAtomo->atributo.constInt = atof(lexema);

        infoAtomo->atomo = CONSTINT;
        printf("%i: %s: %f\n", infoAtomo->linha, strMensagem[CONSTINT], infoAtomo->atributo.constInt);

        return;
}

void reconhece_char(TInfoAtomo *infoA){
    buffer++;
    if(*buffer == '\0'){infoA->atomo = EOS; return;}
    char c = *buffer;
    buffer++;
    if(*buffer != '\''){
        return;
    }
    buffer++;
    infoA->atributo.constChar = c;
    infoA->atomo = CONSTCHAR;
    printf("%i: %s: '%c'\n", infoA->linha, strMensagem[CONSTCHAR], c);
}

void reconhece_specialChars(TInfoAtomo *infoAtomo){
    struct {const char* key; TAtomo value;} values[] = {
        {":=", ASSIGN},
        {",", KW_COMMA},
        {".", KW_PERIOD},
        {":", KW_COLON},
        {";", kW_SEMICOLON},
        {"(", KW_OPEN_PARENTHESIS},
        {")", KW_CLOSE_PARENTHESIS},
        {"<>", RELATIONAL_OPERATOR},
        {"<", RELATIONAL_OPERATOR},
        {">", RELATIONAL_OPERATOR},
        {"<=", RELATIONAL_OPERATOR},
        {">=", RELATIONAL_OPERATOR},
        {"=", RELATIONAL_OPERATOR},
        {"or", RELATIONAL_OPERATOR},
        {"and", RELATIONAL_OPERATOR},
        {"+", ADDING_OPERATOR},
        {"-", ADDING_OPERATOR},
        {"*", MULTIPLYING_OPERATOR},
        {"div", MULTIPLYING_OPERATOR}
    };

    if(buffer == NULL || *buffer == '\0'){
        infoAtomo->atomo = EOS;
        return;
    }

    size_t size = sizeof(values)/sizeof(values[0]);

    for (size_t i = 0; i < size; ++i){
        size_t len = strlen(values[i].key);
        if(strncmp(values[i].key, buffer, len) == 0){
            infoAtomo->atomo = values[i].value;
            buffer += len;
            printf("%i: %s\n", infoAtomo->linha, strMensagem[values[i].value]);
            return;
        }
    }
    return;
}

int main(int argc, char* argv[]){
    if(argc != 2){
        printf("Entrada invalida - Utilize o formato: ./nomeDoEXE <nomeDoArquivoPasckenzie>\n");
        exit(1);
    }
    buffer_read_file(argv[1]);
    printf("Analisando...\n");
    nLinha = 1;
    infoAtomo = obter_atomo();
    lookAhead = infoAtomo.atomo;
    program();

    printf("%i linhas analisadas, programa sintaticamente correto\n", nLinha);
}

void program(){
    consome(KW_PROGRAM);
    consome(IDENTIFIER);
    consome(kW_SEMICOLON);
    block();
    consome(KW_PERIOD);
}

void block(){
    variable_declaration_part();
    statement_part();
}

void variable_declaration_part(){
    if(lookAhead == KW_VAR){
        consome(KW_VAR);
        variable_declaration();
        consome(kW_SEMICOLON);
        while(lookAhead != KW_BEGIN){
            variable_declaration();
            consome(kW_SEMICOLON);
        }
    }
}

void variable_declaration(){
    consome(IDENTIFIER);
    while(lookAhead == KW_COMMA){
        consome(KW_COMMA);
        consome(IDENTIFIER);
    }
    consome(KW_COLON);
    consome(TYPE);
}

void statement_part(){
    consome(KW_BEGIN);
    statement();
    while(lookAhead == kW_SEMICOLON){
        consome(kW_SEMICOLON);
        statement();
    }
    consome(KW_END);
}

void statement(){
    if(lookAhead == IDENTIFIER){
        assignment_statement();
        return;
    }
    if(lookAhead == KW_READ){
        read_statement();
        return;
    }
    if(lookAhead == KW_WRITE){
        write_statement();
        return;
    }
    if(lookAhead == KW_IF){
        if_statement();
        return;
    }
    if(lookAhead == KW_WHILE){
        while_statement();
        return;
    }
    if(lookAhead == KW_BEGIN){
        statement_part();
        return;
    }
}

void factor(){
    if(lookAhead == IDENTIFIER){
        consome(IDENTIFIER);
        return;
    }
    if(lookAhead == CONSTCHAR){
        consome(CONSTCHAR);
        return;
    }
    if(lookAhead == CONSTINT){
        consome(CONSTINT);
        return;
    }
    if(lookAhead == KW_OPEN_PARENTHESIS){
        consome(KW_OPEN_PARENTHESIS);
        expression();
        consome(KW_CLOSE_PARENTHESIS);
        return;
    }
    if(lookAhead == KW_NOT){
        consome(KW_NOT);
        factor();
        return;
    }
    if(lookAhead == KW_TOF){
        consome(KW_TOF);
        return;
    }
}

void term(){
    factor();
    while(lookAhead == MULTIPLYING_OPERATOR){
        consome(MULTIPLYING_OPERATOR);
        factor();
    }
}

void simple_expression(){
    term();
    while(lookAhead == ADDING_OPERATOR){
        consome(ADDING_OPERATOR);
        term();
    }
}

void expression(){
    simple_expression();
    if(lookAhead == RELATIONAL_OPERATOR){
        consome(RELATIONAL_OPERATOR);
        simple_expression();
    }
}

void assignment_statement(){
    consome(IDENTIFIER);
    consome(ASSIGN);
    expression();
}

void read_statement(){
    consome(KW_READ);
    consome(KW_OPEN_PARENTHESIS);
    consome(IDENTIFIER);
    while(lookAhead == KW_COMMA){
        consome(KW_COMMA);
        consome(IDENTIFIER);
    }
    consome(KW_CLOSE_PARENTHESIS);
}

void write_statement(){
    consome(KW_WRITE);
    consome(KW_OPEN_PARENTHESIS);
    consome(IDENTIFIER);
    while(lookAhead == KW_COMMA){
        consome(KW_COMMA);
        consome(IDENTIFIER);
    }
    consome(KW_CLOSE_PARENTHESIS);
}

void if_statement(){
    consome(KW_IF);
    expression();
    consome(KW_THEN);
    statement();
    if(lookAhead == KW_ELSE){
        consome(KW_ELSE);
        statement();
    }
}

void while_statement(){
    consome(KW_WHILE);
    expression();
    consome(KW_DO);
    statement();
}
