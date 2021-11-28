
#include "syntacticalanalyzer.h"
#include "codgen.h"
char func[] = "function";


node *global_table;
s_stack *stack_table;

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
    code_header();
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
//    GET_TOKEN(token);

    if (token->token_name == TOKEN_KEYWORD && token->lexeme->keyword == KEYWORD_GLOBAL){
        GET_TOKEN(token);
        sData *repeat_function = NULL;
        sData *global_function = NULL;
        if(token->token_name == TOKEN_IDENTIFIER){
            node *repeat_function_node =  tree_search(global_table, hashcode(token->lexeme->inter->data));
            if(repeat_function_node){
                repeat_function = repeat_function_node->data;
                // проверка была ли уже декларация функции если да то ошибка
                if(repeat_function->declaration){
                    return ERROR_SEMANTIC_ANALYSIS;
                    //TODO error
                }else{//если функция уже была дифинована
                    repeat_function->declaration = true;
                }
            //если функция не была не декларована не дифинована
            }else {
                global_function = malloc(sizeof(sData));
                if (!global_function) {
                    return ERROR_INTERNAL;
                }
                s_data_init(global_function);
                global_function->type = malloc(sizeof (t_str));
                if(!global_function->type){
                    return ERROR_INTERNAL;
                }
                string_init(global_function->type);
                string_wright_arr(global_function->type, func);
                global_function->declaration = true;
                global_function->name = malloc(sizeof(t_str));
                if(!global_function->name){
                    return ERROR_INTERNAL;
                }
                string_init(global_function->name);
                string_copy(token->lexeme->inter, global_function->name);
                // заливаем узел в дерево
                global_table = tree_insert(global_table, hashcode(global_function->name->data), global_function);
            }
        }else{
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок
        }


        GET_TOKEN(token);
        if(token->token_name != TOKEN_ASSIGNMENT_TYPE){
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок ожидалась двоеточие
        }
        GET_TOKEN(token);
        if(token->token_name != TOKEN_KEYWORD && token->lexeme->keyword != KEYWORD_FUNCTION){
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок function
        }

        GET_TOKEN(token);
        if(token->token_name != TOKEN_LEFT_BRACKET) {
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок ожидалась скобка
        }
        //повторется функция или нет
        if(repeat_function){
            if (global_params(token, hashcode(repeat_function->name->data))) {
                return ERROR_SYN_ANALYSIS;
                //TODO ожидались параметры функции
            }
        }else{
            if (global_params(token, hashcode(global_function->name->data))) {
                return ERROR_SYN_ANALYSIS;
                //TODO ожидались параметры функции
            }
        }


        GET_TOKEN(token);
        if(token->token_name != TOKEN_RIGHT_BRACKET) {
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок ожидалась скобка
        }

//        GET_TOKEN(token);
//        if(token->token_name != TOKEN_ASSIGNMENT_TYPE){
//            return ERROR_SYN_ANALYSIS;
//            //TODO обработка ошибок ожидалась двоеточие
//        }

        if(repeat_function) {
            if (return_types(token, hashcode(repeat_function->name->data))) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок
            }
        }else{
            if (return_types(token, hashcode(global_function->name->data))) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок
            }
        }

        if(chunk(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок
        }
        return IT_IS_OK;
    }

    else if(token->token_name == TOKEN_KEYWORD && token->lexeme->keyword == KEYWORD_FUNCTION){
        sData *repeat_function = NULL;
        sData *global_function = NULL;
        GET_TOKEN(token);
        if(token->token_name == TOKEN_IDENTIFIER){
            node *repeat_function_node =  tree_search(global_table, hashcode(token->lexeme->inter->data));
            if(repeat_function_node) {
                repeat_function = repeat_function_node->data;
                // проверка была ли уже декларация функции если да то ошибка
                if (repeat_function->define) {
                    return ERROR_SEMANTIC_ANALYSIS;
                    //TODO error
                } else {//если функция уже была дифинована
                    repeat_function->define = true;
                }
                //TODO Записать до табулки симвалов
            }else{
                global_function = malloc(sizeof(sData));
                if (!global_function) {
                    return ERROR_INTERNAL;
                }
                s_data_init(global_function);
                global_function->type = malloc(sizeof (t_str));
                if(!global_function->type){
                    return ERROR_INTERNAL;
                }
                string_init(global_function->type);
                string_wright_arr(global_function->type, func);
                global_function->define = true;
                global_function->name = malloc(sizeof(t_str));
                if(!global_function->name){
                    return ERROR_INTERNAL;
                }
                string_init(global_function->name);
                string_copy(token->lexeme->inter, global_function->name);
                // заливаем узел в дерево
                global_table = tree_insert(global_table, hashcode(global_function->name->data), global_function);
            }
        }else{
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок
        }

        GET_TOKEN(token);
        if(token->token_name != TOKEN_LEFT_BRACKET) {
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок ожидалась скобка
        }

        if(repeat_function) {
            if (params(token, hashcode(repeat_function->name->data))) {
                return ERROR_SYN_ANALYSIS;
                //TODO ожидались параметры функции
            }
        }else{
            if (params(token, hashcode(global_function->name->data))) {
                return ERROR_SYN_ANALYSIS;
                //TODO ожидались параметры функции
            }
        }

        GET_TOKEN(token);
        if(token->token_name != TOKEN_RIGHT_BRACKET) {
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок ожидалась скобка
        }
        if(repeat_function) {
            if (return_types(token, hashcode(repeat_function->name->data))) {
                return ERROR_SYN_ANALYSIS;
                //TODO оюработка ошибок
            }
        }else{
            if (return_types(token, hashcode(global_function->name->data))) {
                return ERROR_SYN_ANALYSIS;
                //TODO оюработка ошибок
            }
        }

        if(statement(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO оюработка ошибок
        }

        if(token->token_name == TOKEN_KEYWORD && token->lexeme->keyword == KEYWORD_END){
            if(chunk(token)){
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
        if(function_call(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок
        }
        if(chunk(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO обработка ошибок
        }

        return IT_IS_OK;
    }

    return IT_IS_OK;
}


int return_types(t_token *token, ul hash){
    bool ret_param = false;
    GET_TOKEN(token);
    node *function_node = tree_search(global_table, hash);
    //token  == :
    if(token->token_name == TOKEN_ASSIGNMENT_TYPE) {
        if ((function_node->data->declaration && !(function_node->data->define)) || (!(function_node->data->declaration) && function_node->data->define)) {
            function_node->data->type_returned_params = malloc(sizeof(t_str_param));
            if(!function_node->data->type_returned_params){
                return ERROR_INTERNAL;
            }
            string_param_init(function_node->data->type_returned_params);
            ret_param = true;
            if (data_type(token,function_node, ret_param)) {
                //TODO добавить обрабутку ошибок
                return ERROR_SYN_ANALYSIS;
            }
            if (next_data_type(token, function_node, ret_param)) {
                return ERROR_SYN_ANALYSIS;
            }
        }else if(function_node->data->declaration && function_node->data->define){
            ret_param = true;
//            function_node->data->help_count++;
            if (data_type(token,function_node, ret_param)) {
                //TODO добавить обрабутку ошибок
                return ERROR_SYN_ANALYSIS;
            }
            if (next_data_type(token, function_node, ret_param)) {
                return ERROR_SYN_ANALYSIS;
            }
        }
    }else{
        if(function_node->data->declaration && function_node->data->define){
            if (function_node->data->count_returned_params != function_node->data->help_count) {
                return ERROR_SYN_ANALYSIS;
                //TODO количество возвращаемых параметров не соответсвует в делорации и дефениции
            }
        }
        hold_token();
    }
    // -> ε
    return IT_IS_OK;
}

int statement(t_token *token){
    GET_TOKEN(token);
    if(token->token_name == TOKEN_KEYWORD && token->lexeme->keyword == KEYWORD_LOCAL){
        GET_TOKEN(token);
        if(token->token_name == TOKEN_IDENTIFIER){
            //TODO add to symbol table
        }else
        {
            return ERROR_SYN_ANALYSIS;
            //TODO ошибка
        }
        GET_TOKEN(token);
        if(token->token_name != TOKEN_ASSIGNMENT_TYPE){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

//        if(data_type(token)){
//            return ERROR_SYN_ANALYSIS;;
//            //TODO error
//        } TODO откоментировать

        GET_TOKEN(token);
        if(token->token_name != TOKEN_ASSIGNMENT){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

        if(value(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }
        if(statement(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

    }else if(token->token_name == TOKEN_IDENTIFIER){
        //TODO add to symbol table

        if(next_id(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

        GET_TOKEN(token);
        if(token->token_name != TOKEN_ASSIGNMENT){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

        if(value(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

        if(statement(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

    }else if(token->token_name == TOKEN_KEYWORD && token->lexeme->keyword == KEYWORD_IF ){
        if(expression(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

        GET_TOKEN(token);
        if(token->token_name != TOKEN_KEYWORD && token->lexeme->keyword != KEYWORD_THEN){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

        if(statement(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

        GET_TOKEN(token);
        if(token->token_name != TOKEN_KEYWORD && token->lexeme->keyword != KEYWORD_ELSE){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

        if(!statement(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

        GET_TOKEN(token);
        if(token->token_name != TOKEN_KEYWORD && token->lexeme->keyword != KEYWORD_END){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

        if(statement(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

    }else if(token->token_name == TOKEN_KEYWORD && token->lexeme->keyword == KEYWORD_WHILE){
        if(expression(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

        GET_TOKEN(token);
        if(token->token_name != TOKEN_KEYWORD && token->lexeme->keyword != KEYWORD_DO){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

        if(statement(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

        GET_TOKEN(token);
        if(token->token_name != TOKEN_KEYWORD && token->lexeme->keyword != KEYWORD_END){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

        if(statement(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }
    }else if(token->token_name == TOKEN_KEYWORD && token->lexeme->keyword == KEYWORD_RETURN){
        if(expression(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }
        if(statement(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }
    }
    else{
        return IT_IS_OK;
    }
    return IT_IS_OK;
}

int function_call(t_token *token){
    if(token->token_name == TOKEN_IDENTIFIER){
        //TODO проверка индификатора на то что это функция
        //TODO вернет с табулкисимволов назев и если назев подхолдит под что то то сверяем аргументы
        t_str id_fun; //TODO заглушшка перерписать

        //TODO у каждего ифа долдна происзодить запиись до узла
        if(string_arr_cmp(&id_fun,"write")){
            GET_TOKEN(token);
            if(token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            if(args(token)){
                return ERROR_SYN_ANALYSIS;
                //TODO ожидались параметры функции
            }

            GET_TOKEN(token);
            if(token->token_name != TOKEN_RIGHT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }
        }
        else if(string_arr_cmp(&id_fun,"reads")){
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
        else if(string_arr_cmp(&id_fun,"readi")){
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
        else if(string_arr_cmp(&id_fun,"readn")){

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
        else if(string_arr_cmp(&id_fun,"tointager")){
            GET_TOKEN(token);
            if(token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            if(args(token)){
                return ERROR_SYN_ANALYSIS;
                //TODO ожидались параметры функции
            }

            GET_TOKEN(token);
            if(token->token_name != TOKEN_RIGHT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }
        }
        else if(string_arr_cmp(&id_fun,"substr")){
            GET_TOKEN(token);
            if(token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            if(args(token)){
                return ERROR_SYN_ANALYSIS;
                //TODO ожидались параметры функции
            }

            GET_TOKEN(token);
            if(token->token_name != TOKEN_RIGHT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }
        }
        else if(string_arr_cmp(&id_fun,"ord")){
            GET_TOKEN(token);
            if(token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            if(args(token)){
                return ERROR_SYN_ANALYSIS;
                //TODO ожидались параметры функции
            }

            GET_TOKEN(token);
            if(token->token_name != TOKEN_RIGHT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }
        }
        else if(string_arr_cmp(&id_fun,"chr")){
            GET_TOKEN(token);
            if(token->token_name != TOKEN_LEFT_BRACKET) {
                return ERROR_SYN_ANALYSIS;
                //TODO обработка ошибок ожидалась скобка
            }

            if(args(token)){
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

            if (args(token)) {
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

int next_data_type(t_token *token, node* function_node, bool ret_param){
    GET_TOKEN(token);
    //token == ,
    if(token->token_name == TOKEN_COMMA){
        if(data_type(token,function_node,ret_param)){
            //TODO добавить обрабутку ошибок
            return ERROR_SYN_ANALYSIS;
        }
        if(next_data_type(token,function_node,ret_param)){
            return ERROR_SYN_ANALYSIS;
        }
    }else{
        //если функция была и декларована и дифинована то проверяем сходятся ли счетчики параметров после проврки
        if(function_node->data->declaration && function_node->data->define){
            if(function_node->data->count_returned_params != function_node->data->help_count){
                return ERROR_SYN_ANALYSIS;
                //TODO количество возвращемых параметров в дефениции и деклорацуии не соответсвует
            }
        }
        hold_token();
    }

    // -> ε
    return IT_IS_OK;
}

int data_type(t_token *token, node* function_node, bool ret_param){
    GET_TOKEN(token);
    //если это не возвращаемые параметры то
    if(!ret_param) {
        //если функция только декларовна
        if (function_node->data->declaration && !(function_node->data->define)) {
            if (token->lexeme->keyword == KEYWORD_INTEGER || token->lexeme->keyword == KEYWORD_NUMBER ||token->lexeme->keyword == KEYWORD_STRING) {
                //копируем тип глобальную таблицу
                string_param_copy_string(function_node->data->type_params, token->lexeme->inter);
                //увеличиваем счетчик переменных
                function_node->data->count_params++;
            } else {
                return ERROR_SYN_ANALYSIS;
            }
        //если функция была и дикларована и дифинована
        }else if(function_node->data->declaration && function_node->data->define){
            //увеличиваем количество переменных на +1
            function_node->data->help_count++;
            if (token->lexeme->keyword == KEYWORD_INTEGER || token->lexeme->keyword == KEYWORD_NUMBER
            ||token->lexeme->keyword == KEYWORD_STRING){
                //проверяем одинаковый ли тип у перменных в дифиниции и деклорации
                if(!string_param_cmp_string(function_node->data->type_params, (function_node->data->help_count - 1), token->lexeme->inter)){
                    return ERROR_SYN_ANALYSIS;
                    //TODO не сответствует тип переменнорй в декларации и дефиниции
                }
            } else {
                return ERROR_SYN_ANALYSIS;
            }
        }else if(!(function_node->data->declaration) && function_node->data->define){
            if (token->lexeme->keyword == KEYWORD_INTEGER || token->lexeme->keyword == KEYWORD_NUMBER
                                                            ||token->lexeme->keyword == KEYWORD_STRING) {
                //записываем до глобальной таблицы
                string_param_copy_string(function_node->data->type_params, token->lexeme->inter);
                function_node->data->count_params++; // увеличиваем число параметров потомучто они уже записаны
                //забираем таблицу функции со стека
                node* in_function = tree_search(table_top(stack_table), hashcode(function_node->data->params->data[function_node->data->count_params-1]->data));
                in_function->data->type = malloc(sizeof (t_str)); //аллакуем место под тип переменной
                if(!in_function->data->type){
                    return ERROR_INTERNAL;
                }
                string_init(in_function->data->type);
                string_copy(token->lexeme->inter, in_function->data->type);//копируем тип переменой в таблицу функции
            } else {
                return ERROR_SYN_ANALYSIS;
            }
        }
    }else{// если это возвращаемые парамеры то
        //если функция была только декларована
        if (function_node->data->declaration && !(function_node->data->define) || (!(function_node->data->declaration) && function_node->data->define)) {
            if (token->lexeme->keyword == KEYWORD_INTEGER || token->lexeme->keyword == KEYWORD_NUMBER ||token->lexeme->keyword == KEYWORD_STRING) {
                //копируем параметры до главной таблицы
                string_param_copy_string(function_node->data->type_returned_params, token->lexeme->inter);
                function_node->data->count_returned_params++; //увенличиваем счетчик
            } else {
                return ERROR_SYN_ANALYSIS;
            }
            //если функция была либо дикларована или дифинована
        } else if(function_node->data->declaration && function_node->data->define){
            //увеличим счетчик помощник потомучто точно знаем что там будет параметр
            function_node->data->help_count++;
            if (token->lexeme->keyword == KEYWORD_INTEGER || token->lexeme->keyword == KEYWORD_NUMBER ||token->lexeme->keyword == KEYWORD_STRING){
                //проверяем равняется ли тип параметров при дифиниции и декларации
                if(!string_param_cmp_string(function_node->data->type_returned_params, (function_node->data->help_count - 1), token->lexeme->inter)){
                    return ERROR_SYN_ANALYSIS;
                    //TODO не сответствует тип переменнорй в декларации и дефиниции
                }
            } else {
                return ERROR_SYN_ANALYSIS;
            }
        }
    }
    return IT_IS_OK;
}
int global_params(t_token *token, ul hash){
    GET_TOKEN(token);
    bool ret_param = false;
    //token == id
    node *function_node =  tree_search(global_table, hash);
    if(token->token_name != TOKEN_RIGHT_BRACKET) {
        if(function_node->data->declaration && !(function_node->data->define)){
            function_node->data->type_params = malloc(sizeof (t_str_param));
            if(!function_node->data->type_params){
                return ERROR_INTERNAL;
            }
            string_param_init(function_node->data->type_params);
            hold_token();

            if(data_type(token, function_node,ret_param)){
                return ERROR_SYN_ANALYSIS;
                //TODO добавить обрабутку ошибок
            }

            if(next_data_type(token, function_node, ret_param)){
                return ERROR_SYN_ANALYSIS;
                //TODO добавить обработку ошибок
            }

        }else if(function_node->data->declaration && function_node->data->define) {

            hold_token();

            if (data_type(token, function_node,ret_param)) {
                return ERROR_SYN_ANALYSIS;
                //TODO добавить обрабутку ошибок
            }

            if (next_data_type(token, function_node, ret_param)) {
                return ERROR_SYN_ANALYSIS;
                //TODO добавить обработку ошибок
            }
            function_node->data->help_count = 0;
        }
    }else{
        if(function_node->data->count_params != 0){
            return ERROR_SYN_ANALYSIS;
            //TODO количесво параметров в декларации и дефиниции не совпадает
        }
        hold_token();
    }
}
int params(t_token *token, ul hash){
    GET_TOKEN(token);
    bool ret_param = false;
    //token == id
    node *function_node =  tree_search(global_table, hash);
    node *in_function = NULL;
    sData *function_var = NULL;
    if(token->token_name == TOKEN_IDENTIFIER){
        if(!(function_node->data->declaration) && function_node->data->define){
            function_node->data->params = malloc(sizeof (t_str_param));
            if(!function_node->data->params){
                return ERROR_INTERNAL;
            }
            function_node->data->type_params = malloc(sizeof (t_str_param));
            if(!function_node->data->type_params){
                return ERROR_INTERNAL;
            }
            string_param_init(function_node->data->params);
            string_param_init(function_node->data->type_params);
            function_var = malloc(sizeof (sData));
            if(!function_var){
                return ERROR_INTERNAL;
            }
            s_data_init(function_var);
            function_var->name = malloc(sizeof (t_str));
            if(!function_var->name){
                return ERROR_INTERNAL;
            }
            string_init(function_var->name);
            string_copy(token->lexeme->inter, function_var->name);
            in_function = tree_insert(in_function, hashcode(function_var->name->data),function_var);
            table_push(stack_table,in_function);

            string_param_copy_string(function_node->data->params, token->lexeme->inter);

            GET_TOKEN(token);

            if(token->token_name != TOKEN_ASSIGNMENT_TYPE){
                //TODO добавить обработку ошибок
                return ERROR_SYN_ANALYSIS;
            }

            if(data_type(token, function_node,ret_param)){
                return ERROR_SYN_ANALYSIS;
                //TODO добавить обрабутку ошибок
            }

            if(next_param(token, function_node, ret_param)){
                return ERROR_SYN_ANALYSIS;
                //TODO добавить обработку ошибок
            }

        }else if(function_node->data->declaration && function_node->data->define) {
            function_node->data->help_count++;
            if(!string_param_cmp_string(function_node->data->params, (function_node->data->help_count - 1), token->lexeme->inter)){
                return ERROR_SYN_ANALYSIS;
                //TODO не сответствует название переменнорй в декларации и дефиниции
            }
            GET_TOKEN(token);

            if (token->token_name != TOKEN_ASSIGNMENT_TYPE) {
                //TODO добавить обработку ошибок
                return ERROR_SYN_ANALYSIS;
            }

            if (data_type(token, function_node,ret_param)) {
                return ERROR_SYN_ANALYSIS;
                //TODO добавить обрабутку ошибок
            }

            if (next_param(token, function_node, ret_param)) {
                return ERROR_SYN_ANALYSIS;
                //TODO добавить обработку ошибок
            }
            function_node->data->help_count = 0;
        }

    }else if(function_node->data->count_params != 0){
        return ERROR_SYN_ANALYSIS;
        //TODO количесво параметров в декларации и дефиниции не совпадает
    }
    // -> ε
    return IT_IS_OK;
}

/// Maybe leave only check if there comma or not ?
int next_param(t_token *token, node* function_node, bool ret_param){
    GET_TOKEN(token);
    if(token->token_name == TOKEN_COMMA){
        GET_TOKEN(token);
        if(function_node->data->declaration && !(function_node->data->define)) { //token == id
            if (token->token_name == TOKEN_IDENTIFIER) {
                string_param_copy_string(function_node->data->params, token->lexeme->inter);
                GET_TOKEN(token);

                if (token->token_name != TOKEN_ASSIGNMENT_TYPE) {
                    //TODO добавить обработку ошибок
                    return ERROR_SYN_ANALYSIS;
                }

                if (data_type(token, function_node, ret_param)) {
                    return ERROR_SYN_ANALYSIS;
                    //TODO добавить обрабутку ошибок
                }

                if (next_param(token,function_node, ret_param)) {
                    return ERROR_SYN_ANALYSIS;
                    //TODO добавить обработку ошибок
                }

            }
            // если функция уже была декларована или дифинована
        }else if(function_node->data->declaration && function_node->data->define){
            function_node->data->help_count++;
            if(!string_param_cmp_string(function_node->data->params, (function_node->data->help_count - 1), token->lexeme->inter)){
                return ERROR_SYN_ANALYSIS;
                //TODO не сответствует название переменнорй в декларации и дефиниции
            }

            if (token->token_name != TOKEN_ASSIGNMENT_TYPE) {
                //TODO добавить обработку ошибок
                return ERROR_SYN_ANALYSIS;
            }

            if (data_type(token, function_node, ret_param)) {
                return ERROR_SYN_ANALYSIS;
                //TODO добавить обрабутку ошибок
            }

            if (next_param(token,function_node, ret_param)) {
                return ERROR_SYN_ANALYSIS;
                //TODO добавить обработку ошибок
            }
            //если строка только дифинована
        }else if(!(function_node->data->declaration) && function_node->data->define){
            //добавляем в главную таблицу
            string_param_copy_string(function_node->data->params, token->lexeme->inter);

            //добавляем в таблицу функции
            node* in_function = table_top(stack_table);
            sData* function_var = malloc(sizeof (sData));
            if(!function_var){
                return ERROR_INTERNAL;
            }
            s_data_init(function_var);
            function_var->name = malloc(sizeof (t_str));
            string_init(function_var->name);
            string_copy(token->lexeme->inter, function_var->name);
            in_function = tree_insert(in_function, hashcode(function_var->name->data),function_var);

            GET_TOKEN(token);

            if (token->token_name != TOKEN_ASSIGNMENT_TYPE) {
                //TODO добавить обработку ошибок
                return ERROR_SYN_ANALYSIS;
            }

            if (data_type(token, function_node, ret_param)) {
                return ERROR_SYN_ANALYSIS;
                //TODO добавить обрабутку ошибок
            }

            if (next_param(token,function_node, ret_param)) {
                return ERROR_SYN_ANALYSIS;
                //TODO добавить обработку ошибок
            }


        }
    }else{// если идет не запятая
        if(function_node->data->declaration && function_node->data->define) {
            if (function_node->data->count_params != function_node->data->help_count) {
                return ERROR_SYN_ANALYSIS;
                //TODO корличесво парметров не соответствует в деклорации и дефеницуии
            }
        }
        hold_token();
    }
    // -> ε
    return IT_IS_OK;
}

int value(t_token *token){ //todo равенства чего с чемто
    //TODO first write the prec analysis table in code
    return IT_IS_OK;
}

int expression(t_token *token){
    //TODO first write the prec analysis table in code
    return IT_IS_OK;
}

int new_expression(t_token *token){
    //TODO first write the prec analysis table in code
    GET_TOKEN(token);
    if(token->token_name == TOKEN_COMMA){
        if(expression(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

        if(new_expression(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

    }else{
        return IT_IS_OK;
    }
    return IT_IS_OK;
}


int next_id(t_token *token){
    GET_TOKEN(token);
    if(token->token_name == TOKEN_COMMA){
        if(token->token_name == TOKEN_IDENTIFIER){
            //TODO add to symbol table
        }else{
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

        if(next_id(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

    }else{
        return IT_IS_OK;
    }
    return IT_IS_OK;
}


int start_analysis(t_token *token){

    stack_table = malloc(sizeof (s_stack));
    table_init(stack_table);

    return (start_program(token));

}

