#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "string.c"

FILE *code_file;
t_str *string;

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
    TOKEN_PLUS = 20,                     // +
    TOKEN_MINUS,                        // -
    TOKEN_MULTIPLICATION,               // *
    TOKEN_DIVISION,                     // / деление выследок number
    TOKEN_INT_DIVISION,                 // // деление выследок integer
    TOKEN_CONCATENATION,                // ..
    TOKEN_ASSIGNMENT,                   // =

    TOKEN_GREATER,                      // >
    TOKEN_GREATER_OR_EQUAL              // >=,
    TOKEN_LESS,                         // <
    TOKEN_LESS_OR_EQUAL,                // <=
    TOKEN_EQUALS,                       // ==
    TOKEN_NOT_EQUALS,                   // ~=

    TOKEN_LENGTH,                       // #
    TOKEN_WRIGHT,                       // %
    TOKEN_ASSIGNMENT_TYPE,              // :
    TOKEN_LEFT_BRACKET,                 // (
    TOKEN_RIGHT_BRACKET,                // )

    TOKEN_KEYWORD,                      // keyword
    TOKEN_EOF,                          // EOF
    TOKEN_EOL,                          // \0
    TOKEN_SPACE,                        // пробел
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
    LEXICAL_STATE_LESS,
    LEXICAL_STATE_NOT,
}e_lexical_state_fsm;

typedef struct s_lexeme{
    t_str inter;
    e_keyword keyword;
    int integer;
    double number;
}t_lexeme;

typedef struct s_token{
    e_token_type token_name;
    t_lexeme lexeme;
}t_token;



