#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <math.h>

#include "string.c"
#include "error.h"

#define EOL '\n'
#define SCALE_OF_NOTATION 10;

static FILE *code_file = NULL;
static t_str *string = NULL;

const char* const KEYWORDS[] = {"do","global","number","else","if","require","end","integer","return","function","local","string","nil","then","while"};
const int AMOUNT_OF_KEYWORDS = (sizeof(KEYWORDS)/sizeof(KEYWORDS[0]));
typedef enum {
    KEYWORD_DO = 0,
    KEYWORD_GLOBAL,
    KEYWORD_NUMBER,
    KEYWORD_ELSE,
    KEYWORD_IF,
    KEYWORD_REQUIRE,
    KEYWORD_END,
    KEYWORD_INTEGER,
    KEYWORD_RETURN,
    KEYWORD_FUNCTION,
    KEYWORD_LOCAL,
    KEYWORD_STRING,
    KEYWORD_NIL,
    KEYWORD_THEN,
    KEYWORD_WHILE,
}e_keyword;

typedef enum{
    TOKEN_PLUS = 20,                    // +1
    TOKEN_MINUS,                        // -1
    TOKEN_MULTIPLICATION,               // *1
    TOKEN_DIVISION,                     // / деление выследок number 1
    TOKEN_INT_DIVISION,                 // // деление выследок integer 1
    TOKEN_CONCATENATION,                // .. 1
    TOKEN_ASSIGNMENT,                   // =1

    TOKEN_GREATER,                      // > 1
    TOKEN_GREATER_OR_EQUAL,             // >=,1
    TOKEN_LESS,                         // < 1
    TOKEN_LESS_OR_EQUAL,                // <=1
    TOKEN_EQUALS,                       // ==1
    TOKEN_NOT_EQUALS,                   // ~= 1

    TOKEN_LENGTH,                       // #1
    TOKEN_WRIGHT,                       // %1
    TOKEN_ASSIGNMENT_TYPE,              // :1
    TOKEN_LEFT_BRACKET,                 // (1
    TOKEN_RIGHT_BRACKET,                // )1
    TOKEN_COMMA,                        //, 1

    TOKEN_KEYWORD,                      // keyword1
    TOKEN_EOF,                          // EOF1
//    TOKEN_EOL,                          // \01
//    TOKEN_SPACE,                        // пробел1
    TOKEN_IDENTIFIER,                   // переменная1

    TOKEN_INTEGER,                      // тип int1
    TOKEN_NUMBER,                       // тип double1
    TOKEN_NUMBER_EXPONENT,              // тип double c expon1
    TOKEN_STRING,                       // тип string1

}e_token_type;

typedef enum {
    LEXICAL_STATE_START = 100,// старт автомата //1
    LEXICAL_STATE_DIVISION, //1
    LEXICAL_STATE_INT_DIVISION, //1
    LEXICAL_STATE_IDENTIFIER, //1
    /////////////////////////////////////
    LEXICAL_STATE_NUMERIC, //1
    LEXICAL_STATE_NUMERIC_DOT, //1
    LEXICAL_STATE_NUMERIC_NUMBER, //1
    LEXICAL_STATE_NUMERIC_EXP, //1
    LEXICAL_STATE_NUMERIC_EXP_SIGN, //1
    LEXICAL_STATE_NUMERIC_EXP_FINAL, //1
    ////////////////////////////////////
    LEXICAL_STATE_STRING_START, //1
    LEXICAL_STATE_STRING_ESCAPE, //1
    LEXICAL_STATE_STRING_ESCAPE_FN_ZERO, //1
    LEXICAL_STATE_STRING_ESCAPE_FN_0NE,  //1
    LEXICAL_STATE_STRING_ESCAPE_FN_TWO,  //1
    LEXICAL_STATE_STRING_ESCAPE_FN_ZERO_TN_ZERO, //1
    LEXICAL_STATE_STRING_ESCAPE_TN, //1
    LEXICAL_STATE_STRING_ESCAPE_FN_TWO_TN_FIVE, //1
//    LEXICAL_STATE_STRING_STOP,
    ////////////////////////////////////
    LEXICAL_STATE_MINUS, //1
    LEXICAL_STATE_COMMENT_LINE_OR_BLOCK, //1
    LEXICAL_STATE_COMMENT_LINE, //1
    LEXICAL_STATE_COMMENT_BLOCK_START, //1
    LEXICAL_STATE_COMMENT_BLOCK, //1
    LEXICAL_STATE_COMMENT_BLOCK_STOP, //1
    ///////////////////////////////////
    LEXICAL_STATE_GREATER, //1
    LEXICAL_STATE_ASSIGNMENT, //1
    LEXICAL_STATE_LESS, //1
    LEXICAL_STATE_NOT, //1
    LEXICAL_STATE_CONCATENATE, //1
    LEXICAL_STATE_EOL, //1
    LEXICAL_STATE_EOF, //1
    LEXICAL_STATE_SPACE, //1
}e_lexical_state_fsm;

typedef struct s_lexeme{
    t_str* inter;
    e_keyword keyword;
    int integer;
    double number;
}t_lexeme;

typedef struct s_token{
    e_token_type token_name;
    t_lexeme* lexeme;
}t_token;

int file_ptr(FILE* f){
    code_file = f;
    if(!code_file){
        //TODO обработка ошибок
        return ERROR_INTERNAL;
    }
    return IT_IS_OK;
}

void keyword_check(t_token* token)
{
    int number_keyword = 0;
    for(;number_keyword <= AMOUNT_OF_KEYWORDS - 1;number_keyword++){
        if(!string_arr_cmp(string, KEYWORDS[number_keyword])){
            token->lexeme->keyword = number_keyword;
            token->token_name = TOKEN_KEYWORD;
            return;
        }
    }
    token->token_name = TOKEN_IDENTIFIER;
}


int convert_escape(char* arr){
    errno = 0;
    char* endptr = NULL;
    long symbol = strtol(arr,&endptr,10);
    if(endptr == arr || *endptr != NUL || ((symbol == LONG_MIN ||  symbol == LONG_MAX) && errno == ERANGE))
        return ERROR_INTERNAL;
    string_wright_char(string,symbol);
    printf("%s\n",string->data);
    return IT_IS_OK;
}

int convert_str_numeric(t_token* token){
    errno = 0;
    char* endptr = NULL;
    if (token->token_name == TOKEN_INTEGER){
        int symbol = (int) strtol(string->data,&endptr,10);
        if(endptr == string->data || *endptr != NUL || ((symbol == INT_MIN ||  symbol == INT_MAX) && errno == ERANGE))
            return ERROR_INTERNAL;
        token->lexeme->integer = symbol;
        return IT_IS_OK;
    }else if(token->token_name == TOKEN_NUMBER || token->token_name == TOKEN_NUMBER_EXPONENT){
        double symbol = strtod(string->data,&endptr);
        if(endptr == string->data || *endptr != NUL || ((symbol == HUGE_VAL ||  symbol == -HUGE_VAL) && errno == ERANGE))
            return ERROR_INTERNAL;
        token->lexeme->number = symbol;
        return IT_IS_OK;
    }
    return ERROR_INTERNAL;
}

int find_token(t_token* token){
    int state = LEXICAL_STATE_START;
    char escape_seq[4] = "000"; //TODO magic number

    while(true){
        char symbol =(char) getc(code_file);
        switch (state) {
            case LEXICAL_STATE_START:
                if(symbol == '+') {
                    token->token_name = TOKEN_PLUS;
                    return IT_IS_OK;
                }else if(symbol == '*'){
                    token->token_name = TOKEN_MULTIPLICATION;
                    string_wright_char(string, symbol);
                    return IT_IS_OK;
                }else if(symbol == '#'){
                    token->token_name = TOKEN_LENGTH;
                    string_wright_char(string, symbol);
                    return IT_IS_OK;
                }else if(symbol == '%'){
                    token->token_name = TOKEN_WRIGHT;
                    string_wright_char(string, symbol);
                    return IT_IS_OK;
                }else if(symbol == ':'){
                    token->token_name = TOKEN_ASSIGNMENT_TYPE;
                    string_wright_char(string, symbol);
                    return IT_IS_OK;
                }else if(symbol == '('){
                    token->token_name = TOKEN_LEFT_BRACKET;
                    string_wright_char(string, symbol);
                    return IT_IS_OK;
                }else if(symbol == ')'){
                    token->token_name = TOKEN_RIGHT_BRACKET;
                    string_wright_char(string, symbol);
                    return IT_IS_OK;
                }else if(symbol == ','){
                    token->token_name = TOKEN_COMMA;
                    string_wright_char(string, symbol);
                    return IT_IS_OK;
                }else if(symbol == EOF){
                    state = LEXICAL_STATE_EOF;
                }else if((symbol >= 'A' && symbol <= 'Z' ) || ( symbol >= 'a' && symbol <= 'z')|| symbol == '_'){
                        string_wright_char(string,symbol);
                        state = LEXICAL_STATE_IDENTIFIER;
                }else if(symbol == '\"'){
                    state = LEXICAL_STATE_STRING_START;
//                    string_wright_char(string,symbol);
                }else if(symbol >= '0' && symbol <= '9' ){
                    state = LEXICAL_STATE_NUMERIC;
                    string_wright_char(string,symbol);
                }else if(symbol == ' '){
                    state = LEXICAL_STATE_SPACE;
                }else if(symbol == EOL) {
                    state = LEXICAL_STATE_EOL;
                }else if(symbol == '-'){
                    state = LEXICAL_STATE_MINUS;
                    string_wright_char(string, symbol);
                }else if(symbol == '/'){
                    state = LEXICAL_STATE_DIVISION;
                    string_wright_char(string, symbol);
                }else if(symbol == '.'){
                    state = LEXICAL_STATE_CONCATENATE;
                    string_wright_char(string, symbol);
                }else if(symbol == '>'){
                    state = LEXICAL_STATE_GREATER;
                    string_wright_char(string, symbol);
                }else if(symbol == '<'){
                    state = LEXICAL_STATE_LESS;
                    string_wright_char(string, symbol);
                } else if(symbol == '='){
                    state = LEXICAL_STATE_ASSIGNMENT;
                    string_wright_char(string, symbol);
                } else if(symbol == '~'){
                    state = LEXICAL_STATE_NOT;
                    string_wright_char(string, symbol);
                }else{
                    return ERROR_LEX_ANALYSIS;
                }
                break;

            case LEXICAL_STATE_DIVISION:
                if(symbol=='/'){
                    token->token_name = TOKEN_INT_DIVISION;
                    string_wright_char(string, symbol);
                    return IT_IS_OK;
                }else if(symbol == ' '){
                    token->token_name = TOKEN_DIVISION;
                    return IT_IS_OK;
                }else{
                    token->token_name = TOKEN_DIVISION;
                    ungetc(symbol,code_file);
                    return IT_IS_OK;
                }

            case LEXICAL_STATE_CONCATENATE:
                if(symbol == '.'){
                    token->token_name = TOKEN_CONCATENATION;
                    string_wright_char(string, symbol);
                    return IT_IS_OK;
                }else{
                    return ERROR_LEX_ANALYSIS;
                }

            case LEXICAL_STATE_GREATER:
                if(symbol == '='){
                    token->token_name = TOKEN_GREATER_OR_EQUAL;
                    string_wright_char(string, symbol);
                    return IT_IS_OK;
                }else{
                    ungetc(symbol,code_file);
                    token->token_name = TOKEN_GREATER;
                    return IT_IS_OK;
                }

            case LEXICAL_STATE_LESS:
                if(symbol == '='){
                    token->token_name = TOKEN_LESS_OR_EQUAL;
                    string_wright_char(string, symbol);
                    return IT_IS_OK;
                }else{
                    ungetc(symbol,code_file);
                    token->token_name = TOKEN_LESS;
                    return IT_IS_OK;
                }

            case LEXICAL_STATE_ASSIGNMENT:
                if(symbol == '='){
                    token->token_name = TOKEN_EQUALS;
                    string_wright_char(string, symbol);
                    return IT_IS_OK;
                }else{
                    ungetc(symbol,code_file);
                    token->token_name = TOKEN_LESS;
                    return IT_IS_OK;
                }

            case LEXICAL_STATE_NOT:
                if(symbol == '='){
                    token->token_name = TOKEN_NOT_EQUALS;
                    string_wright_char(string, symbol);
                    return IT_IS_OK;
                }else{
                    return ERROR_LEX_ANALYSIS;
                }
            case LEXICAL_STATE_IDENTIFIER:
                if(symbol == ' ' || symbol == EOL){
                    keyword_check(token);
                    return IT_IS_OK;
                }else if(symbol == EOF){
                    ungetc(symbol,code_file);
                    keyword_check(token);
                    return IT_IS_OK;
                }
                else if((symbol >= 'A' && symbol <= 'Z' )||( symbol >= 'a' && symbol <= 'z')|| symbol == '_'){
                    string_wright_char(string, symbol);
                    break;
                }

            case LEXICAL_STATE_MINUS:
                if(symbol == '-'){
                    state = LEXICAL_STATE_COMMENT_LINE_OR_BLOCK;
                    string_init_state(string); // сбросить стороку
                    break;
                }else{
                    ungetc(symbol,code_file);
                    token->token_name = TOKEN_MINUS;
                    return IT_IS_OK;
                }

            case LEXICAL_STATE_COMMENT_LINE_OR_BLOCK:
                if(symbol == EOL)
                {
                    state = LEXICAL_STATE_START;
                    break;
                }else if(symbol == '['){
                    state = LEXICAL_STATE_COMMENT_BLOCK_START;
                    break;
                }
                else{
                    state = LEXICAL_STATE_COMMENT_LINE;
                }

            case LEXICAL_STATE_COMMENT_LINE:
                if(symbol == EOL || symbol == EOF)
                {
                    state = LEXICAL_STATE_START;
                    break;
                }else{
                    break;
                }

            case LEXICAL_STATE_COMMENT_BLOCK_START:
                if(symbol == '['){
                    state = LEXICAL_STATE_COMMENT_BLOCK;
                    break;
                }else if(symbol == EOL){
                    state = LEXICAL_STATE_START;
                    break;
                }
                else{
                    state = LEXICAL_STATE_COMMENT_LINE;
                    break;
                }

            case LEXICAL_STATE_COMMENT_BLOCK:
                if(symbol == ']'){
                    state = LEXICAL_STATE_COMMENT_BLOCK_STOP;
                    break;
                }else if(symbol == EOF){
                    return ERROR_LEX_ANALYSIS;
                }else
                {
                    break;
                }

            case LEXICAL_STATE_COMMENT_BLOCK_STOP:
                if(symbol == ']'){
                    state = LEXICAL_STATE_START;
                    break;
                }else if(symbol == EOF){
                    return ERROR_LEX_ANALYSIS;
                }else{
                    state = LEXICAL_STATE_COMMENT_BLOCK;
                    break;
                }
            case LEXICAL_STATE_SPACE:
                if(symbol != ' '){
                    ungetc(symbol,code_file);
                    state = LEXICAL_STATE_START;
                }
                break;

            case LEXICAL_STATE_EOL:
                if(symbol != ' ' && symbol != EOL){
                    ungetc(symbol,code_file);
                    state = LEXICAL_STATE_START;
                }
                break;

            case LEXICAL_STATE_EOF:
                token->token_name = TOKEN_EOF;
                return IT_IS_OK;


            case LEXICAL_STATE_NUMERIC:
                if(symbol >= '0' && symbol <= '9'){
                    string_wright_char(string, symbol);
                    break;
                }else if(symbol == '.'){
                    state = LEXICAL_STATE_NUMERIC_DOT;
                    string_wright_char(string, symbol);
                    break;
                }else if(symbol == 'e' || symbol == 'E'){
                    state = LEXICAL_STATE_NUMERIC_EXP;
                    string_wright_char(string, symbol);
                    break;
                }else{
                    token->token_name = TOKEN_INTEGER;
                    ungetc(symbol,code_file);
                    convert_str_numeric(token);
                    return IT_IS_OK;
                }


            case LEXICAL_STATE_NUMERIC_DOT:
                if(symbol >= '0' && symbol <= '9'){
                    string_wright_char(string, symbol);
                    state = LEXICAL_STATE_NUMERIC_NUMBER;
                    break;
                }else{
                    return ERROR_LEX_ANALYSIS;
                }


            case LEXICAL_STATE_NUMERIC_NUMBER:
                if(symbol >= '0' && symbol <= '9'){
                    string_wright_char(string, symbol);
                    break;
                }else if(symbol == 'e' || symbol == 'E'){
                    state = LEXICAL_STATE_NUMERIC_EXP;
                    string_wright_char(string, symbol);
                    break;
                }else{
                    ungetc(symbol,code_file);
                    token->token_name = TOKEN_NUMBER;
                    convert_str_numeric(token);
                    return IT_IS_OK;
                }


            case LEXICAL_STATE_NUMERIC_EXP:
                if(symbol == '+' || symbol == '-'){
                    state = LEXICAL_STATE_NUMERIC_EXP_SIGN;
                    string_wright_char(string, symbol);
                    break;
                }else if(symbol >= '0' && symbol <= '9'){
                    state = LEXICAL_STATE_NUMERIC_EXP_FINAL;
                    string_wright_char(string, symbol);
                    break;
                }else{
                    return ERROR_LEX_ANALYSIS;
                }


            case LEXICAL_STATE_NUMERIC_EXP_SIGN:
                if(symbol >= '0' && symbol <= '9'){
                    state = LEXICAL_STATE_NUMERIC_EXP_FINAL;
                    string_wright_char(string, symbol);
                    break;
                }else{
                    return ERROR_LEX_ANALYSIS;
                }


            case LEXICAL_STATE_NUMERIC_EXP_FINAL:
                if(symbol >= '0' && symbol <= '9'){
                    string_wright_char(string, symbol);
                    break;
                }else{
                    token->token_name = TOKEN_NUMBER_EXPONENT;
                    ungetc(symbol,code_file);
                    convert_str_numeric(token);
                    return IT_IS_OK;
                }


            case LEXICAL_STATE_STRING_START:
                if(symbol == '\"'){
                    token->token_name = TOKEN_STRING;
                    return IT_IS_OK;
                }else if(symbol < 32){
                    return ERROR_LEX_ANALYSIS;
                }else if(symbol == '\\'){
                    state = LEXICAL_STATE_STRING_ESCAPE;
                    break;
                }
                else{
                    string_wright_char(string, symbol);
                    break;
                }


            case LEXICAL_STATE_STRING_ESCAPE:
                if(symbol == '\\'){
                    string_wright_char(string, '\\');
                    state = LEXICAL_STATE_STRING_START;
                    break;
                }else if(symbol == '\"'){
                    string_wright_char(string, '\"');
                    state = LEXICAL_STATE_STRING_START;
                    break;
                }else if(symbol == 'n'){
                    string_wright_char(string, '\n');
                    state = LEXICAL_STATE_STRING_START;
                    break;
                }else if(symbol == 't'){
                    string_wright_char(string, '\t');
                    state = LEXICAL_STATE_STRING_START;
                    break;
                }else if(symbol == '0')
                {
                    state = LEXICAL_STATE_STRING_ESCAPE_FN_ZERO;
                    escape_seq[0] = symbol;
                    break;
                }else if(symbol == '1')
                {
                    state = LEXICAL_STATE_STRING_ESCAPE_FN_0NE;
                    escape_seq[0] = symbol;
                    break;
                }else if(symbol == '2')
                {
                    state = LEXICAL_STATE_STRING_ESCAPE_FN_TWO;
                    escape_seq[0] = symbol;
                    break;
                }else{
                    return ERROR_LEX_ANALYSIS;
                }


            case LEXICAL_STATE_STRING_ESCAPE_FN_ZERO:
                if(symbol == '0'){
                    state = LEXICAL_STATE_STRING_ESCAPE_FN_ZERO_TN_ZERO;
                    escape_seq[1] = symbol;
                    break;
                }else if((symbol >= '1') && (symbol <= '9')){
                    state = LEXICAL_STATE_STRING_ESCAPE_TN;
                    escape_seq[1] = symbol;
                    break;
                }else{
                    return ERROR_LEX_ANALYSIS;
                }


            case LEXICAL_STATE_STRING_ESCAPE_FN_0NE:
                if((symbol >= '0') && (symbol <= '9')){
                    state = LEXICAL_STATE_STRING_ESCAPE_TN;
                    escape_seq[1] = symbol;
                    break;
                }else{
                    return ERROR_LEX_ANALYSIS;
                }


            case LEXICAL_STATE_STRING_ESCAPE_FN_TWO:
                if((symbol >= '0') && (symbol <= '4')){
                    state = LEXICAL_STATE_STRING_ESCAPE_TN;
                    escape_seq[1] = symbol;
                    break;
                }else if(symbol == '5'){
                    state = LEXICAL_STATE_STRING_ESCAPE_FN_TWO_TN_FIVE;
                    escape_seq[1] = symbol;
                    break;
                }
                else{
                    return ERROR_LEX_ANALYSIS;
                }


            case LEXICAL_STATE_STRING_ESCAPE_FN_ZERO_TN_ZERO:
                if((symbol >= '1') && (symbol <= '9')){
                    state = LEXICAL_STATE_STRING_START;
                    escape_seq[2] = symbol;
                    if(convert_escape(escape_seq)){
                        return ERROR_INTERNAL;
                    }
                    break;
                }else{
                    return ERROR_LEX_ANALYSIS;
                }


            case LEXICAL_STATE_STRING_ESCAPE_TN:
                if((symbol >= '0') && (symbol <= '9')){
                    state = LEXICAL_STATE_STRING_START;
                    escape_seq[2] = symbol;
                    if(convert_escape(escape_seq)){
                        return ERROR_INTERNAL;
                    }
                    break;
                }else{
                    return ERROR_LEX_ANALYSIS;
                }


            case LEXICAL_STATE_STRING_ESCAPE_FN_TWO_TN_FIVE:
                if((symbol >= '0') && (symbol <= '5')){
                    state = LEXICAL_STATE_STRING_START;
                    escape_seq[2] = symbol;
                    if(convert_escape(escape_seq)){
                        return ERROR_INTERNAL;
                    }
                    break;
                }else{
                    return ERROR_LEX_ANALYSIS;
                }

        }
    }
}

int prepar_analysis(t_token* token){

    if(!token){
        return ERROR_INTERNAL;
    }
    else if(!token->lexeme){
        return ERROR_INTERNAL;
    }
    else if(!token->lexeme->inter){
        return ERROR_INTERNAL;
    }


    if(!code_file)
        return ERROR_INTERNAL;

    if(!string){
        string = malloc(sizeof(t_str));
        if(string_init(string))
            return ERROR_INTERNAL;
    }else{
        string_init_state(string);
    }

    return IT_IS_OK;
}

int get_token(t_token* token){
    if (prepar_analysis(token)){
        //TODO обработка ошибок
        return 1;
    }

    if(find_token(token)){
        //TODO обработка ошибок
        string_free(string);
        return 2;
    }

    if(token->token_name != TOKEN_EOF)
        string_copy(string,token->lexeme->inter);

    return IT_IS_OK;
}


/// TODO пернести дефиниции и enum(ы) с константами в файл библиотеки дописать обработку ошибок
/// TODO дописать пару тестов

