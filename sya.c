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

//typedef struct s_node{
//    // TODO Какаяниюудь стабулка ссимволов где искть
//
//    sData head;
//    sData params;
//    sData body;
//
//    bool it_is_loop;
//    bool is_is_declaration;
//    bool it_is_if;
//    bool it_is_def_function;
//    bool it_is_function;
//}t_node;
//

int chunk(t_token *token);
int function(t_token *token);
int params(t_token *token);
int return_types(t_token *token);
int statement(t_token *token);
int function_call(t_token *token);
int args(t_token *token);
int data_type(t_token *token);
int next_data_type(t_token *token);
int next_param(t_token *token);
int value(t_token *token);
int expression(t_token *token);
int new_expression(t_token *token);
int tmp(t_token *token);
int next_id(t_token *token);




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
        GET_TOKEN(token);
        if(token->token_name == TOKEN_IDENTIFIER){
            //TODO Записать до табулки симвалов
        }else{
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок
        }

        GET_TOKEN(token);
        if(token->token_name != TOKEN_LEFT_BRACKET) {
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок ожидалась скобка
        }

        GET_TOKEN(token);
        if(token->token_name != TOKEN_ASSIGNMENT_TYPE){
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок ожидалась двоеточие
        }

        if(token->token_name != TOKEN_KEYWORD || token->lexeme->keyword != KEYWORD_FUNCTION){
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок function
        }

        GET_TOKEN(token);
        if(token->token_name != TOKEN_LEFT_BRACKET) {
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок ожидалась скобка
        }

        if(!params(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO ожидались параметры функции
        }

        GET_TOKEN(token);
        if(token->token_name != TOKEN_RIGHT_BRACKET) {
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок ожидалась скобка
        }

        GET_TOKEN(token);
        if(token->token_name != TOKEN_ASSIGNMENT_TYPE){
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок ожидалась двоеточие
        }

        if(!return_types(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок
        }

        if(!chunk(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок
        }
        return IT_IS_OK;
    }

    else if(token->token_name == TOKEN_KEYWORD && token->lexeme->keyword == KEYWORD_FUNCTION){
        GET_TOKEN(token);
        if(token->token_name == TOKEN_IDENTIFIER){
            //TODO Записать до табулки симвалов
        }else{
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок
        }

        GET_TOKEN(token);
        if(token->token_name != TOKEN_LEFT_BRACKET) {
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок ожидалась скобка
        }

        if(!params(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO ожидались параметры функции
        }

        GET_TOKEN(token);
        if(token->token_name != TOKEN_RIGHT_BRACKET) {
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок ожидалась скобка
        }

        if(!return_types(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO оюработка ошибок
        }

        if(!statement(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO оюработка ошибок
        }

        if(token->token_name == TOKEN_KEYWORD && token->lexeme->keyword == KEYWORD_END){
            if(!chunk(token)){
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок
            }
        }else{
            return ERROR_SYN_ANALYSIS;
            //TODO ожидался end
        }
        return IT_IS_OK;
    }

//    else if(token->token_name == TOKEN_IDENTIFIER){
//        //TODO проверка индификатора на то что это функция
//
//        GET_TOKEN(token);
//        if(token->token_name != TOKEN_LEFT_BRACKET) {
//            return ERROR_SYN_ANALYSIS;
//            //TODO обработка ошибок ожидалась скобка
//        }
//
//        if(!args(token)){
//            return ERROR_SYN_ANALYSIS;
//            //TODO ожидались параметры функции
//        }
//
//        GET_TOKEN(token);
//        if(token->token_name == TOKEN_RIGHT_BRACKET) {
//            if(!chunk(token)){
//                return ERROR_SYN_ANALYSIS;
//                //TODO обработка ошибок
//            }
//        }else{
//            return ERROR_SYN_ANALYSIS;
//            //TODO обработка ошибок ожидалась скобка
//        }
//        return IT_IS_OK;
//    }
    else{
        if(!function_call(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок
        }
        if(!chunk(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок
        }

        return IT_IS_OK;
    }

    return IT_IS_OK;
}

int params(t_token *token){
    return IT_IS_OK;
}

int return_types(t_token *token){
    return IT_IS_OK;
}

int statement(t_token *token){
    return IT_IS_OK;
}

int function_call(t_token *token){
    if(token->token_name == TOKEN_IDENTIFIER){
        //TODO проверка индификатора на то что это функция
        //TODO вернет с табулкисимволов назев и если назев подхолдит под что то то сверяем аргументы
        t_str id_fun; //TODO заглушшка перерписать

        //TODO у каждего ифа долдна происзодить запиись до узла
        if(string_arr_cmp(id_fun,"write")){
            GET_TOKEN(token);
            if(token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            if(!args(token)){
                return ERROR_SYN_ANALYSIS;
                //TODO ожидались параметры функции
            }

            GET_TOKEN(token);
            if(token->token_name != TOKEN_RIGHT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }
        }
        else if(string_arr_cmp(id_fun,"reads")){
            GET_TOKEN(token);
            if(token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            GET_TOKEN(token);
            if(token->token_name != TOKEN_RIGHT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }
        }
        else if(string_arr_cmp(id_fun,"readi")){
            GET_TOKEN(token);
            if(token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            GET_TOKEN(token);
            if(token->token_name != TOKEN_RIGHT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }
        }
        else if(string_arr_cmp(id_fun,"readn")){

            GET_TOKEN(token);
            if(token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            GET_TOKEN(token);
            if(token->token_name != TOKEN_RIGHT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }
        }
        else if(string_arr_cmp(id_fun,"tointager")){
            GET_TOKEN(token);
            if(token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            if(!args(token)){
                return ERROR_SYN_ANALYSIS;
                //TODO ожидались параметры функции
            }

            GET_TOKEN(token);
            if(token->token_name != TOKEN_RIGHT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }
        }
        else if(string_arr_cmp(id_fun,"substr")){
            GET_TOKEN(token);
            if(token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            if(!args(token)){
                return ERROR_SYN_ANALYSIS;
                //TODO ожидались параметры функции
            }

            GET_TOKEN(token);
            if(token->token_name != TOKEN_RIGHT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }
        }
        else if(string_arr_cmp(id_fun,"ord")){
            GET_TOKEN(token);
            if(token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            if(!args(token)){
                return ERROR_SYN_ANALYSIS;
                //TODO ожидались параметры функции
            }

            GET_TOKEN(token);
            if(token->token_name != TOKEN_RIGHT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }
        }
        else if(string_arr_cmp(id_fun,"chr")){
            GET_TOKEN(token);
            if(token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            if(!args(token)){
                return ERROR_SYN_ANALYSIS;
                //TODO ожидались параметры функции
            }

            GET_TOKEN(token);
            if(token->token_name != TOKEN_RIGHT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }
        }
        else {
            //TODO спросить у табулке символов а был ли он определен если нет ошибка
            GET_TOKEN(token);
            if (token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            if (!args(token)) {
                return ERROR_SYN_ANALYSIS;
                //TODO ожидались параметры функции
            }

            GET_TOKEN(token);
            if (token->token_name != TOKEN_RIGHT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }
            return IT_IS_OK;
        }
    }
    return IT_IS_OK;
}

int args(t_token *token){ //TODO предпологаю что функция будет определять параметры есть или нет
    return IT_IS_OK;
}

int data_type(t_token *token){
    return IT_IS_OK;
}

int next_data_type(t_token *token){
    return IT_IS_OK;
}

int next_param(t_token *token){
    return IT_IS_OK;
}

int value(t_token *token){ //todo равенства чего с чемто
    return IT_IS_OK;
}

int expression(t_token *token){
    return IT_IS_OK;
}

int new_expression(t_token *token){
    return IT_IS_OK;
}

int tmp(t_token *token){
    return IT_IS_OK;
}

int next_id(t_token *token){
    return IT_IS_OK;
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

