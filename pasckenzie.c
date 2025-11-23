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

TTabelaSimbolos tabelaSimbolos;
int endereco_var_global = 0; // enderecamento das variaveis
int label = 1; // L1, L2...
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
    {"end", KW_END},
    {"or", RELATIONAL_OPERATOR},
    {"and", RELATIONAL_OPERATOR},
    {"div", MULTIPLYING_OPERATOR}
    };


static int buffer_read_file(char* filename){
    FILE *f = fopen(filename, "rb");
    if(f == NULL){
        char errorMessage[200]; sprintf(errorMessage, "Erro ao abrir arquivo %s\n", filename);
        perror(errorMessage); 
        exit(1);
    }
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
            // int linhaIni = nLinha; [PRINT REFERENTE À PARTE 1 DO PROJETO]
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
            // printf("%i-%i: Comentario\n", linhaIni, nLinha); [PRINT REFERENTE À PARTE 1 DO PROJETO]
            continue;
        }
        break;
    }

    infoA.linha = nLinha; 

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
        strncpy(lexema, ini_lexema,buffer-ini_lexema); // salvamos ini_lexema como inicio do buffer e salvamos deste ponto até onde o buffer esta agora (uma forma de copiar a string)
        lexema[buffer-ini_lexema] = '\0';
        strcpy(infoA->atributo.id, lexema);
        size_t keywordSize = sizeof(keywords)/sizeof(keyword);
        if(lexema[0] != '_'){ // palavras chave nunca começam com _, então se a string nova começar com _ ela pode só pular a verificação
            for(int i = 0; i < keywordSize; i++){
                if(strcmp(lexema, keywords[i].keyword) == 0){
                    // printf("%i: %s\n", infoA->linha, keywords[i].keyword); [PRINT REFERENTE À PARTE 1 DO PROJETO]
                    infoA->atomo = keywords[i].value;
                    return;
                }
            }
        }
        // printf("%i: %s: %s\n", infoA->linha, strMensagem[IDENTIFIER] ,infoA->atributo.id); [PRINT REFERENTE À PARTE 1 DO PROJETO]
        infoA->atomo = IDENTIFIER;
        return;

}

void reconhece_num(TInfoAtomo *infoAtomo){
    // digito+ ( (d (+|ε) digito+) |ε )
    char *ini_lexema = buffer;
    char *ptrPostDNum = NULL;
    char postDNum[15];

    q1:
        if(isdigit(*buffer)){
            buffer++;
            goto q1;
        }
        if(*buffer == 'd'){
            buffer++;
            if(*buffer == '+') {buffer++;}
            goto q2;
        }
        goto end;
    q2:
        ptrPostDNum = buffer;
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

        if(buffer - ini_lexema >= 15){
            printf("Erro na linha %i - Numero grande demais\n", nLinha);
            return;
        }

    end:
        strncpy(lexema, ini_lexema, buffer-ini_lexema);
        lexema[buffer-ini_lexema] = '\0';

        float num;
        
        if (ptrPostDNum != NULL){ // transforma o numero escrito na conversao (digito d (+|ε) digito+) em float correspondente
            char firstPart[15]; // separando a parte antes do 'd'
            strncpy(firstPart, lexema, ptrPostDNum - ini_lexema - 1); // -1 para nao copiar o 'd'
            firstPart[ptrPostDNum - ini_lexema - 1] = '\0';
            num = atof(firstPart);
            strncpy(postDNum, ptrPostDNum, buffer - ptrPostDNum); // copiar a parte depois do 'd'
            postDNum[buffer - ptrPostDNum] = '\0';
            float dNum = atof(postDNum);
            num = num * pow(10, dNum); // criar o numero final
        }else{
            num = atof(lexema); // se nao tiver 'd', so transforma o numero normalmente
        }

        infoAtomo->atributo.constInt = num;

        infoAtomo->atomo = CONSTINT;
        // printf("%i: %s: %f\n", infoAtomo->linha, strMensagem[CONSTINT], infoAtomo->atributo.constInt); [PRINT REFERENTE À PARTE 1 DO PROJETO]

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
    // printf("%i: %s: '%c'\n", infoA->linha, strMensagem[CONSTCHAR], c); [PRINT REFERENTE À PARTE 1 DO PROJETO]
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
        {"<=", RELATIONAL_OPERATOR},
        {">=", RELATIONAL_OPERATOR},
        {"<", RELATIONAL_OPERATOR},
        {">", RELATIONAL_OPERATOR},
        {"=", RELATIONAL_OPERATOR},
        {"+", ADDING_OPERATOR},
        {"-", ADDING_OPERATOR},
        {"*", MULTIPLYING_OPERATOR}
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
            strcpy(infoAtomo->atributo.id, values[i].key);
            buffer += len;
            // printf("%i: %s\n", infoAtomo->linha, strMensagem[values[i].value]); [PRINT REFERENTE À PARTE 1 DO PROJETO]
            return;
        }
    }
    return;
}


void init_tabela_simbolos(){
    for(int i = 0; i <PRIME_NUMBER; i++){
        tabelaSimbolos.entradas[i] = NULL;
    }
}

void insere_tabela_simbolos(char *id){
    int indice = hashMack(id);
    TNo *p = tabelaSimbolos.entradas[indice]; // P eh um ponteiro para a tabela[indice]

    while(p != NULL){ // loop vai percorrer a tabela[indice]
        if(strcmp(p->ID, id) == 0){ //se ja houver um id assim na tabela, sera retornado um erro
            printf("Erro Semantico: Variavel '%s' redeclarada na linha %d.\n", id, nLinha);
            exit(1);
        }
        p = p->prox;
    }

    // se for uma insercao valida, sera criado um novo no
    TNo *novo = (TNo*)malloc(sizeof(TNo));
    strcpy(novo->ID, id);
    novo->endereco = endereco_var_global++;
    // novo no sera adicionado ao inicio da tabela (como se fosse uma pilha, sera dado um push)
    novo->prox = tabelaSimbolos.entradas[indice];
    tabelaSimbolos.entradas[indice] = novo; 
}

int busca_tabela_simbolos(char *id){
    int indice = hashMack(id);
    TNo *p = tabelaSimbolos.entradas[indice];
    while(p != NULL){
        if(strcmp(p->ID, id) == 0) return p->endereco;
        p = p->prox;
    }
    printf("Erro Semantico: Variavel '%s' nao declarada (linha %d). \n", id, nLinha);
    exit(1);
}

int proximo_rotulo(){
    return label++;
}


int hashMack( char * s )
{
    char *p;
    unsigned int h = 0, g;
    for ( p = s; *p != '\0'; p = p + 1 ){
        h = ( h << 4 ) + (*p);
        g = h&0xf0000000U;
        if ( g ){
            h = h ^ ( g >> 24 );
            h = h ^ g;
        }
    }
    return h % PRIME_NUMBER;
}

void printfTabelaSimbolos(){
    printf("\n\nTABELA DE SIMBOLOS\n");
    for(int i = 0; i <PRIME_NUMBER; i++){
        if(tabelaSimbolos.entradas[i] != NULL){
            TNo* p = tabelaSimbolos.entradas[i];
            do{
                printf("Entrada Tabela Simbolos: [%i] => %s | Endereco: %i\n", i, p->ID, p->endereco);
                p = p->prox;
            } while(p != NULL);
        }
    }
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

    printfTabelaSimbolos();

    // printf("%i linhas analisadas, programa sintaticamente correto\n", nLinha); [PRINT REFERENTE À PARTE 1 DO PROJETO]
}

void program(){
    consome(KW_PROGRAM);
    consome(IDENTIFIER);
    consome(kW_SEMICOLON);

    printf("INPP\n");

    block();
    consome(KW_PERIOD);

    printf("PARA\n");
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

        if(endereco_var_global > 0){
            printf("AMEM %d\n", endereco_var_global);
        }
    }
}

void variable_declaration(){
    if(lookAhead == IDENTIFIER){
        insere_tabela_simbolos(infoAtomo.atributo.id);
        consome(IDENTIFIER);
    }
    while(lookAhead == KW_COMMA){
        consome(KW_COMMA);
        if(lookAhead == IDENTIFIER){
            insere_tabela_simbolos(infoAtomo.atributo.id);
            consome(IDENTIFIER);
        }
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
    printf("#%2d:Erro sintatico\nEsperado <statement> - Entregue[%s]", nLinha,strMensagem[lookAhead]);
    exit(1);
}

void factor(){
    if(lookAhead == IDENTIFIER){
        int end = busca_tabela_simbolos(infoAtomo.atributo.id);
        printf("CRVL %d\n", end);
        consome(IDENTIFIER);
        return;
    }
    if(lookAhead == CONSTCHAR){
        printf("CRCT %c\n", infoAtomo.atributo.constChar); 
        consome(CONSTCHAR);
        return;
    }
    if(lookAhead == CONSTINT){
        printf("CRCT %.0f\n", infoAtomo.atributo.constInt);
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
        printf("NEGA\n");
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
        char op[5];
        strcpy(op, infoAtomo.atributo.id);
        consome(MULTIPLYING_OPERATOR);
        factor();

        if(strcmp(op, "*") == 0) printf("MULT\n");
        else if (strcmp(op, "div") == 0) printf("DIVI\n");
    }
}

void simple_expression(){
    term();
    while(lookAhead == ADDING_OPERATOR){
        char op[5];
        strcpy(op, infoAtomo.atributo.id);
        consome(ADDING_OPERATOR);
        term();

        if(strcmp(op, "+") == 0) printf("SOMA\n");
        else if(strcmp(op, "-") == 0) printf("SUBT\n");
    }
}

void expression(){
    simple_expression();
    if(lookAhead == RELATIONAL_OPERATOR){
        char op[5];
        strcpy(op, infoAtomo.atributo.id);
        consome(RELATIONAL_OPERATOR);
        simple_expression();

        if(strcmp(op, "=")==0) printf("CMIG\n");
        else if(strcmp(op, "<") == 0) printf("CMME\n");
        else if(strcmp(op, ">") == 0) printf("CMMA\n");
        else if(strcmp(op, "<=") == 0) printf("CMEG\n");
        else if(strcmp(op, ">=") == 0) printf("CMAG\n");
        else if(strcmp(op, "<>") == 0) printf("CMDG\n");
    }
}

void assignment_statement(){
    // Variavel que vai receber o valor
    char idDestino[16];
    strcpy(idDestino, infoAtomo.atributo.id);
    int end = busca_tabela_simbolos(idDestino);

    consome(IDENTIFIER);
    consome(ASSIGN);
    expression();

    printf("ARMZ %d\n", end);
}

void read_statement(){
    consome(KW_READ);
    consome(KW_OPEN_PARENTHESIS);
    if (lookAhead == IDENTIFIER){
        int end = busca_tabela_simbolos(infoAtomo.atributo.id);
        printf("LEIT\n");
        printf("ARMZ %d\n", end);
        consome(IDENTIFIER);
    }
    while(lookAhead == KW_COMMA){
        consome(KW_COMMA);
        if (lookAhead == IDENTIFIER){
            int end = busca_tabela_simbolos(infoAtomo.atributo.id);
            printf("LEIT\n");
            printf("ARMZ %d\n", end);
            consome(IDENTIFIER);
        }
    }
    consome(KW_CLOSE_PARENTHESIS);
}

void write_statement(){
    consome(KW_WRITE);
    consome(KW_OPEN_PARENTHESIS);
    if(lookAhead == IDENTIFIER){
        int end = busca_tabela_simbolos(infoAtomo.atributo.id);
        printf("CRVL %d\n", end);
        consome(IDENTIFIER);
    }
    printf("IMPR\n");
    while(lookAhead == KW_COMMA){
        consome(KW_COMMA);
        if(lookAhead == IDENTIFIER){
            int end = busca_tabela_simbolos(infoAtomo.atributo.id);
            printf("CRVL %d\n", end);
            consome(IDENTIFIER);
            printf("IMPR\n");
        }
    }
    consome(KW_CLOSE_PARENTHESIS);
}

void if_statement(){
    int L1 = proximo_rotulo();
    int L2 = proximo_rotulo();
    consome(KW_IF);
    expression();
    consome(KW_THEN);
    printf("DSVF L%d\n", L1);
    statement();
    printf("DSVS L%d\n", L2);
    printf("L%d: NADA\n", L1);
    if(lookAhead == KW_ELSE){
        consome(KW_ELSE);
        statement();
    }
    printf("L%d: NADA\n", L2);
}

void while_statement(){
    int L1 = proximo_rotulo();
    int L2 = proximo_rotulo();
    printf("L%d: NADA\n", L1);
    consome(KW_WHILE);
    expression();
    consome(KW_DO);
    printf("DSVF L%d\n", L2);
    statement();
    printf("DSVS L%d\n", L1);
    printf("L%d: NADA\n", L2);
}
