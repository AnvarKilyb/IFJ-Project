#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "string.c"
#include "error.h"

static FILE *code_file = NULL;
static t_str *string = NULL;

const char* const KEYWORDS[] = {"do","global","number","else","if","require","end","integer","return","function","local","string","nil","then","while"};

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

    TOKEN_KEYWORD,                      // keyword
    TOKEN_EOF,                          // EOF1
//    TOKEN_EOL,                          // \01
//    TOKEN_SPACE,                        // пробел1
    TOKEN_IDENTIFIER,                   // переменная

    TOKEN_INTEGER,                      // тип int
    TOKEN_NUMBER,                       // тип double
    TOKEN_NUMBER_EXPONENT,              // тип double c expon

}e_token_type;

typedef enum {
    LEXICAL_STATE_START = 100,// старт автомата
    LEXICAL_STATE_DIVISION,
    LEXICAL_STATE_INT_DIVISION,
    LEXICAL_STATE_IDENTIFIER,
    /////////////////////////////////////
    LEXICAL_STATE_NUMERIC,
    LEXICAL_STATE_NUMERIC_DOT,
    LEXICAL_STATE_NUMERIC_NUMBER,
    LEXICAL_STATE_NUMERIC_EXP,
    LEXICAL_STATE_NUMERIC_EXP_SIGN,
    LEXICAL_STATE_NUMERIC_EXP_FINAL,
    ////////////////////////////////////
    LEXICAL_STATE_STRING_START,
    LEXICAL_STATE_STRING_ESCAPE,
    LEXICAL_STATE_STRING_ESCAPE_FN_ZERO,
    LEXICAL_STATE_STRING_ESCAPE_FN_0NE,
    LEXICAL_STATE_STRING_ESCAPE_FN_TWO,
    LEXICAL_STATE_STRING_ESCAPE_FN_ZERO_TN_ZERO,
    LEXICAL_STATE_STRING_ESCAPE_TN_ZERO_NINE,
    LEXICAL_STATE_STRING_ESCAPE_TN_ONE_NINE,
    LEXICAL_STATE_STRING_ESCAPE_FN_TWO_TN_FIVE,
    LEXICAL_STATE_STRING_STOP,
    ////////////////////////////////////
    LEXICAL_STATE_MINUS,
    LEXICAL_STATE_COMMENT_LINE,
    LEXICAL_STATE_COMMENT_BLOCK_START,
    LEXICAL_STATE_COMMENT_BLOCK,
    LEXICAL_STATE_COMMENT_BLOCK_STOP,
    ///////////////////////////////////
    LEXICAL_STATE_GREATER,
    LEXICAL_STATE_ASSIGNMENT,
    LEXICAL_STATE_LESS,
    LEXICAL_STATE_NOT,
    LEXICAL_STATE_CONCATENATE,
    LEXICAL_STATE_EOL,
    LEXICAL_STATE_SPACE,
}e_lexical_state_fsm;

typedef struct s_lexeme{
    t_str* inter;    //TODO *
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
        return 1;
    }
}

int find_token(t_token* token){
    int state = LEXICAL_STATE_START;

    while(true){
        char symbol =(char) getc(code_file);
        switch (state) {
            case LEXICAL_STATE_START:
                if(symbol == '+') {
                    token->token_name = TOKEN_PLUS;
                    return IT_IS_OK;
                }else if(symbol == '*'){
                    token->token_name = TOKEN_MULTIPLICATION;
                    string_wright_char_begin(string, symbol);
                    return IT_IS_OK;
                }else if(symbol == '#'){
                    token->token_name = TOKEN_LENGTH;
                    string_wright_char_begin(string, symbol);
                    return IT_IS_OK;
                }else if(symbol == '%'){
                    token->token_name = TOKEN_WRIGHT;
                    string_wright_char_begin(string, symbol);
                    return IT_IS_OK;
                }else if(symbol == ':'){
                    token->token_name = TOKEN_ASSIGNMENT_TYPE;
                    string_wright_char_begin(string, symbol);
                    return IT_IS_OK;
                }else if(symbol == '('){
                    token->token_name = TOKEN_LEFT_BRACKET;
                    string_wright_char_begin(string, symbol);
                    return IT_IS_OK;
                }else if(symbol == ')'){
                    token->token_name = TOKEN_RIGHT_BRACKET;
                    string_wright_char_begin(string, symbol);
                    return IT_IS_OK;
                }else if(symbol == ','){
                    token->token_name = TOKEN_COMMA;
                    string_wright_char_begin(string, symbol);
                    return IT_IS_OK;
                }else if(symbol == EOF){
                    token->token_name = TOKEN_EOF;
                    return IT_IS_OK;
                }else if(symbol == ' '){
                    state = LEXICAL_STATE_SPACE;
                }else if(symbol == EOL) {
                    state = LEXICAL_STATE_EOL;
                }else if(symbol == '-'){
                    state = LEXICAL_STATE_MINUS;
                    string_wright_char_begin(string, symbol);
                }else if(symbol == '/'){
                    state = LEXICAL_STATE_DIVISION;
                    string_wright_char_begin(string, symbol);
                }else if(symbol == '.'){
                    state = LEXICAL_STATE_CONCATENATE;
                    string_wright_char_begin(string, symbol);
                }else if(symbol == '>'){
                    state = LEXICAL_STATE_GREATER;
                    string_wright_char_begin(string, symbol);
                }else if(symbol == '<'){
                    state = LEXICAL_STATE_LESS;
                    string_wright_char_begin(string, symbol);
                } else if(symbol == '='){
                    state = LEXICAL_STATE_ASSIGNMENT;
                    string_wright_char_begin(string, symbol);
                } else if(symbol == '~'){
                    state = LEXICAL_STATE_NOT;
                    string_wright_char_begin(string, symbol);
                }else{
                    return ERROR_LEX_ANALYSIS;
                }
                break;

            case LEXICAL_STATE_DIVISION:
                if(symbol=='/'){
                    token->token_name = TOKEN_INT_DIVISION;
                    string_wright_char_behind(string, symbol);
                    return IT_IS_OK;
                }else if(symbol == ' '){
                    token->token_name = TOKEN_DIVISION;
                    return IT_IS_OK;
                }else{
                    token->token_name = TOKEN_DIVISION;
                    ungetc(symbol,code_file);
                    return IT_IS_OK;;
                }

            case LEXICAL_STATE_CONCATENATE:
                if(symbol == '.'){
                    token->token_name = TOKEN_CONCATENATION;
                    string_wright_char_behind(string, symbol);
                    return IT_IS_OK;
                }else{
                    return ERROR_LEX_ANALYSIS;
                }

            case LEXICAL_STATE_GREATER:
                if(symbol == '='){
                    token->token_name = TOKEN_GREATER_OR_EQUAL;
                    string_wright_char_behind(string, symbol);
                    return IT_IS_OK;
                }else{
                    token->token_name = TOKEN_GREATER;
                    return IT_IS_OK;
                }

            case LEXICAL_STATE_LESS:
                if(symbol == '='){
                    token->token_name = TOKEN_LESS_OR_EQUAL;
                    string_wright_char_behind(string, symbol);
                    return IT_IS_OK;
                }else{
                    token->token_name = TOKEN_LESS;
                    return IT_IS_OK;
                }

            case LEXICAL_STATE_ASSIGNMENT:
                if(symbol == '='){
                    token->token_name = TOKEN_EQUALS;
                    string_wright_char_behind(string, symbol);
                    return IT_IS_OK;
                }else{
                    token->token_name = TOKEN_LESS;
                    return IT_IS_OK;
                }

            case LEXICAL_STATE_NOT:
                if(symbol == '='){
                    token->token_name = TOKEN_NOT_EQUALS;
                    string_wright_char_behind(string, symbol);
                    return IT_IS_OK;
                }else{
                    return ERROR_LEX_ANALYSIS;
                }
            case LEXICAL_STATE_IDENTIFIER:
                break;
        }
    }

}

int prepar_analysis(t_token* token){
    if((!token) || (!token->lexeme) || (!token->lexeme->inter))
        return 1; // TODO обработка ошибок

    if(!code_file)
        return 1; //TODO обработка ошибок

    if(!string){
        string = malloc(sizeof(t_str));
        if(string_init(string))
            return 1; // TODO обработка ошибок
    }else
        string_init_state(string);
    return 0;
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
}


