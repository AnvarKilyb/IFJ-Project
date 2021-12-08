/**
 * Project: Implementace překladače imperativního jazyka IFJ21
 *
 * File:     lexicalanalysis.h
 * Subject:  IFJ2021
 *
 * @author:  Vladislav Mikheda  	xmikhe00
 * @author:  Khrisanov Vladislav    xkhris00
 * @author:  Kilybayev Anvar        xkilyb00
 * @author:  Gazizov Zhasdauren     xgaziz00
 */


#ifndef IFJ_PROJEKT_LEXICALANALYSIS_H
#define IFJ_PROJEKT_LEXICALANALYSIS_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <math.h>

#include "string.h"
#include "error.h"

#define EOL '\n'
#define SCALE_OF_NOTATION 10;
//static FILE *code_file = NULL;
//static t_str *string = NULL;
//static int hold = 0;


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
    NONE_K = 50,
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
    NONE_T,
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
    bool str_bool;
    bool int_bool;
    bool number_bool;
}t_lexeme;

typedef struct s_token{
    e_token_type token_name;
    t_lexeme* lexeme;
    int str;
}t_token;


int file_ptr(FILE* f);
void keyword_check(t_token* token);
int convert_escape(char* arr);
int convert_str_numeric(t_token* token);
int find_token(t_token* token);
int prepar_analysis(t_token* token);
int get_token(t_token* token);
void hold_token();
int old_token_allocate();
void get_old_token(t_token* token);
int to_old_token(t_token* token);
void token_free();

#endif //IFJ_PROJEKT_LEXICALANALYSIS_H
