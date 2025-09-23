#include <stdio.h>

typedef enum{
    ERRO,
    IDENTIFIER,
    KW_VAR,
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
    RELATIONAL_OPERATOR,
    ADDING_OPERATOR,
    MULTIPLYING_OPERATOR,
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
    lookAhead = infoAtomo.atomo;

    program();
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
    // Fazer as comparações

    printf("Linha %i\n", nLinha);
    if(islower(*buffer)){
        reconheceID(&infoAtomo);
    }
    else if(*buffer == ';'){
        buffer++;
        infoAtomo.atomo = KW_SEMICOLON;
    }
    else if(*buffer == '.'){
        buffer++;
        infoAtomo.atomo = KW_PERIOD;
    }
    else if(*buffer == '\0'){
        infoAtomo.atomo = EOS;
    }
    return infoAtomo;


}

TInfoAtomo reconheceID(TInfoAtomo* infoAtomo){
    char* ini_lexema = buffer;

q1:
    if( islower(*buffer) || isdigit(*buffer) ){
        buffer++;
        goto q1;
    }
    if( isupper(*buffer)) 
        return; // sai com erro
    
    // preenche o atributo do atomo IDENTIFICADOR
    strncpy(infoAtomo->atributo.id,ini_lexema,buffer-ini_lexema);
    infoAtomo->atributo.id[buffer-ini_lexema] = '\0'; // aqui temos um IDENTIFICADOR

    infoAtomo->atomo = IDENTIFIER;

    return ;
}

void consome( TAtomo atomo ){
    if( lookAhead == atomo ){
        infoAtomo = obterAtomo();
        lookAhead = infoAtomo.atomo;
    }
    else{
        printf("\n#%2d:Erro sintatico: esperado [%s] encontrado [%s]\n",infoAtomo.linha,strMensagem[atomo],strMensagem[lookahead]);
        exit(1);
    }
}

void program(){
    consome(IDENTIFIER);
    consome(KW_SEMICOLON);
    block();
    consome(KW_PERIOD);
}