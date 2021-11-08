#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <math.h>
#include "lexicalanalysis.c"

//#include "mystack.c"

#define GET_TOKEN(token) if(get_token(token)) return ERROR_LEX_ANALYSIS

typedef struct s_node{
    // TODO Какаяниюудь стабулка ссимволов где искть

    sData head;
    sData params;
    sData body;

    bool it_is_loop;
    bool is_is_declaration;
    bool it_is_if;
    bool it_is_def_function;
    bool it_is_function;
}t_node;


int chunk(t_token *token);
int function(t_token *token);



int start_program(t_token *token){

    GET_TOKEN(token);

    if(token->lexeme->keyword == KEYWORD_REQUIRE) {
        GET_TOKEN(token);
        if (string_arr_cmp(token->lexeme->inter, "ifj21"))
            return ERROR_SYN_ANALYSIS;
    }
    else
        return ERROR_SYN_ANALYSIS;

    //TODO добавить переход в функцию в генераторе кода где будет реалтзована запись в строку начала программы
    if(chunk(token)){
        return ERROR_SYN_ANALYSIS;
        //TODO ошибки
    }
    return IT_IS_OK;

}

int chunk(t_token *token){
    GET_TOKEN(token);
    // Заверщение программы
    if(token->token_name == TOKEN_EOF){
        return IT_IS_OK;
    }
    if(function(token)){
        return ERROR_SYN_ANALYSIS;
        //TODO ошибки
    }
    return IT_IS_OK;
}

int function(t_token *token){
    GET_TOKEN(token);

    if (token->token_name == TOKEN_KEYWORD && token->lexeme->keyword == KEYWORD_GLOBAL){
        //Записать до табулки симвалов
    }
}




//int token_analysis(t_token *token){
//    switch (stack_top(&stack)) {
//        case PROG:
//            get_token(token);
//            if(token->lexeme->keyword == KEYWORD_REQUIRE) {
//                get_token(token);
//                if (string_arr_cmp(token->lexeme->inter, "\"ifj21\"")) {
//                    stack_pop(&stack);
//                    stack_push(&stack, CHUNK);
//                    printf("IT is OK\n");
//                } else
//                    return ERROR_SYM_ANALYSIS;
//            }
//            else
//                return ERROR_SYM_ANALYSIS;
//    }
//}


int start_analysis(t_token *token){
    return (start_program(token));

}

