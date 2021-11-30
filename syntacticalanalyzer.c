
#include "syntacticalanalyzer.h"
#include "codgen.h"
char func[] = "function";
char strin[] = "string";
char integ[] = "integer";
char numb[] = "number";


node *global_table;
s_stack *stack_table;
t_ast_node* ast_node;

int start_program(t_token *token){

    GET_TOKEN(token);

    if(token->lexeme->keyword == KEYWORD_REQUIRE) {
        GET_TOKEN(token);
        if (!string_arr_cmp(token->lexeme->inter, "ifj21"))
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
                    repeat_function->it_is_define = false;
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
                    repeat_function->it_is_define = true;
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
                global_function->it_is_define = true;
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

        //////////
        ast_node->it_is_function_define = true;
        if(repeat_function){
            ast_node->func = tree_search(global_table, hashcode(repeat_function->name->data))->data;
        }
        else
            ast_node->func = tree_search(global_table, hashcode(global_function->name->data))->data;
        ast_node->global = global_table;
        ast_node->in_function = tree_search(table_top(stack_table), hashcode(ast_node->func->name->data));
        send_ast();
        //////////

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

int function_call(t_token *token){// заходит с чистым аст стромом либо заполненой принемающимим параметрами
    node* global_function = tree_search(global_table, hashcode(token->lexeme->inter->data));
    if(!global_function){
        return ERROR_SEMANTIC_ANALYSIS;
    }

    ast_node->func = global_function->data;
    if(ast_node->it_is_variable_){
        ast_node->it_is_variable_call_function = true;
        ast_node->it_is_variable_ = false;

        if(ast_node->count_variable > global_function->data->count_returned_params){
            return ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC;
            //параметров которыйе принимают возврат с функции больше чем возвращаемых параметров
        }else{
            for(int i = 1; i <= ast_node->count_variable; i++){
                if(!string_param_cmp_string_param(ast_node->type_variable, i,global_function->data->type_returned_params, i)){
                    if(string_param_cmp_arr(ast_node->type_variable,i,numb) && string_param_cmp_arr(global_function->data->type_returned_params, i, integ))
                        continue; //если ожидем number а возвращается инт
                    return ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC;
                    //не сходятся типы возвращаемых аргументов
                }
            }
        }

    }else{
        ast_node->it_is_call_function = true;
    }

    GET_TOKEN(token);
    if(token->token_name != TOKEN_LEFT_BRACKET) {
        return ERROR_SYN_ANALYSIS;
        //TODO обработка ошибок ожидалась скобка
    }

    if(global_function->data->count_params != 0 || global_function->data->system_function_infinity_param){
        if (args(token)) {
            return ERROR_SYN_ANALYSIS;
            //TODO ожидались параметры функции
        }
    }

    GET_TOKEN(token);
    if(token->token_name != TOKEN_RIGHT_BRACKET) {
        return ERROR_SYN_ANALYSIS;
        //TODO обработка ошибок ожидалась скобка
    }
    //TODO отослать ast стром
    return IT_IS_OK;
}

int args(t_token *token){ //TODO предпологаю что функция будет определять параметры есть или нет
    GET_TOKEN(token);
    if(ast_node->count_func_param >= ast_node->func->count_params){
        return ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC;
    }else{
        ast_node->count_func_param++;
    }

    if(!ast_node->func_param){
        ast_node->func_param = malloc(sizeof (t_str_param));
        string_param_init(ast_node->func_param);
    }

    if(token->token_name == TOKEN_INTEGER){
        if(string_param_cmp_arr(ast_node->func->type_params,ast_node->count_func_param,integ) ||
                string_param_cmp_arr(ast_node->func->type_params,ast_node->count_func_param,numb)){
            //записываем число в параметры;
            string_param_copy_string(ast_node->func_param, token->lexeme->inter);
        }else{
            return ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC;
        }
    }else if(token->token_name == TOKEN_STRING){
        if(string_param_cmp_arr(ast_node->func->type_params,ast_node->count_func_param,strin)){
            //записываем строку в параметры;
            string_param_copy_string(ast_node->func_param, token->lexeme->inter);
        }else{
            return ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC;
        }
    }else if(token->token_name == TOKEN_NUMBER || token->token_name == TOKEN_NUMBER_EXPONENT ){
        if(string_param_cmp_arr(ast_node->func->type_params,ast_node->count_func_param,numb)){
            //записываем число в параметры;
            string_param_copy_string(ast_node->func_param, token->lexeme->inter);
        }else{
            return ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC;
        }
    }else if(token->token_name == TOKEN_IDENTIFIER){
        if(ast_node->it_is_in_function){
            node* function_var = tree_search(table_top(stack_table), hashcode(token->lexeme->inter->data));
            if(function_var){
                string_param_copy_string(ast_node->func_param, token->lexeme->inter);
            }else{
                return ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC;
            }
        }else{
            return ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC;
        }
    }else{
        return ERROR_SYN_ANALYSIS;
        // ожидался параметр
    }
    if(next_args(token)){
        return ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC;
    }
    return IT_IS_OK;
}

int next_args(t_token* token){
    GET_TOKEN(token);
    if(token->token_name == TOKEN_COMMA){
        if(args(token)){
            return ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC;
        }

//        if(next_args(token)){
//            return ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC;
//        }
    }else{
        if(ast_node->count_func_param == ast_node->func->count_params){
            hold_token();
            return IT_IS_OK;
        }
        return ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC;
    }
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
            if((function_node->data->count_returned_params != function_node->data->help_count) && ret_param){
                return ERROR_SYN_ANALYSIS;
                //TODO количество возвращемых параметров в дефениции и деклорацуии не соответсвует
            }
            if ((function_node->data->count_params != function_node->data->help_count) && !ret_param) {
                return ERROR_SYN_ANALYSIS;
                //TODO корличесво парметров не соответствует в деклорации и дефеницуии
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
    if(token->lexeme->keyword == KEYWORD_INTEGER || token->lexeme->keyword == KEYWORD_NUMBER || token->lexeme->keyword == KEYWORD_STRING){
        if (!ret_param) {//если функция только декларовна
            if (function_node->data->declaration && !(function_node->data->define)) {
                //копируем тип глобальную таблицу
                string_param_copy_string(function_node->data->type_params, token->lexeme->inter);
                //увеличиваем счетчик переменных
                function_node->data->count_params++;

                //если функция была и дикларована и дифинована
            }else if (function_node->data->declaration && function_node->data->define){
                //увеличиваем количество переменных на +1
                function_node->data->help_count++;
                //проверяем одинаковый ли тип у перменных в дифиниции и деклорации
                if (!string_param_cmp_string(function_node->data->type_params,(function_node->data->help_count), token->lexeme->inter)) {
                    return ERROR_SEMANTIC_ANALYSIS;
                    //TODO не сответствует тип переменнорй в декларации и дефиниции
                }
                if(function_node->data->it_is_define){
                    node *in_function = tree_search(table_top(stack_table),hashcode(function_node->data->params->data[function_node->data->help_count - 1]->data));

                    in_function->data->type = malloc(sizeof(t_str)); //аллакуем место под тип переменной
                    if (!in_function->data->type)
                        return ERROR_INTERNAL;

                    string_init(in_function->data->type);
                    string_copy(token->lexeme->inter,in_function->data->type);
                }
            }else if (!(function_node->data->declaration) && function_node->data->define){
                //записываем до глобальной таблицы
                string_param_copy_string(function_node->data->type_params, token->lexeme->inter);
                function_node->data->count_params++; // увеличиваем число параметров потомучто они уже записаны
                //забираем таблицу функции со стека
                node *in_function = tree_search(table_top(stack_table),hashcode(function_node->data->params->data[function_node->data->count_params - 1]->data));

                in_function->data->type = malloc(sizeof(t_str)); //аллакуем место под тип переменной
                if (!in_function->data->type)
                    return ERROR_INTERNAL;

                string_init(in_function->data->type);
                string_copy(token->lexeme->inter,in_function->data->type);//копируем тип переменой в таблицу функции
           }

        } else {// если это возвращаемые парамеры то
            //если функция была только декларована
            if (function_node->data->declaration && !(function_node->data->define) ||
                (!(function_node->data->declaration) && function_node->data->define)) {
                //копируем параметры до главной таблицы
                string_param_copy_string(function_node->data->type_returned_params, token->lexeme->inter);
                function_node->data->count_returned_params++; //увенличиваем счетчик

            //если функция была либо дикларована или дифинована
            }else if (function_node->data->declaration && function_node->data->define) {
                //увеличим счетчик помощник потомучто точно знаем что там будет параметр
                function_node->data->help_count++;
                //проверяем равняется ли тип параметров при дифиниции и декларации
                if (!string_param_cmp_string(function_node->data->type_returned_params,
                                             (function_node->data->help_count), token->lexeme->inter)) {
                    return ERROR_SEMANTIC_ANALYSIS;
                    //TODO не сответствует тип переменнорй в декларации и дефиниции
                }
            }
        }
    }else{
        return ERROR_SYN_ANALYSIS;
        //ожидался тип параметра функции
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
    return IT_IS_OK;
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
            function_var->declaration = true;
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

        }else if(function_node->data->declaration && function_node->data->define && function_node->data->it_is_define) {
//            function_node->data->help_count++;
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
            function_var->declaration = true;
            in_function = tree_insert(in_function, hashcode(function_var->name->data),function_var);
            table_push(stack_table,in_function);

            function_node->data->params = malloc(sizeof (t_str_param));
            if(!function_node->data->params){
                return ERROR_INTERNAL;
            }
            string_param_init(function_node->data->params);
            string_param_copy_string(function_node->data->params, token->lexeme->inter);


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
    node* in_function;
    sData* function_var;
    if(token->token_name == TOKEN_COMMA){
        GET_TOKEN(token);
        if(!sting_param_uncmp_string(function_node->data->params, token->lexeme->inter)){
            return ERROR_SYN_ANALYSIS;
            //Одинаковое название переменных
        }
        //добавляем в главную таблицу
        string_param_copy_string(function_node->data->params, token->lexeme->inter);

        //добавляем в таблицу функции
        in_function = table_top(stack_table);
        function_var = malloc(sizeof (sData));
        if(!function_var){
            return ERROR_INTERNAL;
        }
        s_data_init(function_var);
        function_var->name = malloc(sizeof (t_str));
        string_init(function_var->name);
        string_copy(token->lexeme->inter, function_var->name);
        function_var->declaration = true;
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

int next_expression(t_token *token){
    //TODO first write the prec analysis table in code
    GET_TOKEN(token);
    if(token->token_name == TOKEN_COMMA){
        if(expression(token)){
            return ERROR_SYN_ANALYSIS;
            //TODO error
        }

        if(next_expression(token)){
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


void ast_init(t_ast_node* ast){
    ast->global = NULL;
    ast->in_function = NULL;
    ast->local = NULL;

    ast->variable = NULL;
    ast->type_variable = NULL;
    ast->count_variable = 0;

    ast->func = NULL;
    ast->func_param = NULL;
    ast->count_func_param = 0;

    ast->return_param = NULL;
    ast->count_return_param = 0;


    //что то там что то там expression
    ast->it_is_variable_ = false;
    ast->it_is_variable_expression = false;
    ast->it_is_variable_call_function = false;
    ast->it_is_declaration_variable = false;
    ast->it_is_loop = false;
    ast->it_is_if = false;
    ast->it_is_function_define = false;
    ast->it_is_call_function = false;
    ast->if_loop_end = false;
    ast->if_else = false;
    ast->it_is_in_function = false;
    ast->active_node = NULL;
    ast->next_node = NULL;

}
void ast_free(t_ast_node* ast){
    if(ast->variable)
        string_param_free(ast->variable);
    if(ast->type_variable)
        string_param_free(ast->type_variable);
    if(ast->func_param)
        string_param_free(ast->func_param);
    if(ast->return_param)
        string_param_free(ast->return_param);
    t_ast_node *ptr = ast->next_node;
    while(ast->next_node){
        ast->next_node = ptr->next_node;
        free(ptr);
        ptr = ast->next_node;
    }

    ast_init(ast);
}
int add_table_symbols_system_function(char* name){
    sData *function_var = NULL;
    t_str* str = NULL;
    str = malloc(sizeof (t_str));
    string_init(str);

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
    function_var->type = malloc(sizeof (t_str));
    if(!function_var->type){
        return ERROR_INTERNAL;
    }
    string_init(function_var->type);
    string_wright_arr(function_var->name, name);
    string_wright_arr(function_var->type, func);

    function_var->declaration = true;
    function_var->define = true;
    function_var->system_function = true;
    function_var->count_returned_params = 1;


    function_var->type_returned_params = malloc(sizeof (t_str_param));
    if(!function_var->type_returned_params){
        return ERROR_INTERNAL;
    }
    string_param_init(function_var->type_returned_params);
    string_wright_arr(str,name);

    if(string_arr_cmp(str,"write")){
        function_var->count_returned_params = 0;
        function_var->system_function_infinity_param = true;
    }else if(string_arr_cmp(str,"reads")){
        string_wright_arr(str, strin);
    }else if(string_arr_cmp(str,"readi")){
        string_wright_arr(str, integ);
    }else if(string_arr_cmp(str,"readn")){
        string_wright_arr(str, numb);
    }else if(string_arr_cmp(str,"tointeger")){
        function_var->type_params = malloc(sizeof (t_str_param));
        if(!function_var->type_params){
            return ERROR_INTERNAL;
        }
        string_param_init(function_var->type_params);
        string_wright_arr(str,numb);
        string_param_copy_string(function_var->type_params, str);
        function_var->count_params = 1;

        string_wright_arr(str, integ);
    }else if(string_arr_cmp(str,"substr")){
        function_var->type_params = malloc(sizeof (t_str_param));
        if(!function_var->type_params){
            return ERROR_INTERNAL;
        }
        string_param_init(function_var->type_params);
        string_wright_arr(str, strin);
        string_param_copy_string(function_var->type_params, str);
        string_wright_arr(str, numb);
        string_param_copy_string(function_var->type_params, str);
        string_param_copy_string(function_var->type_params, str);
        function_var->count_params = 3;

        string_wright_arr(str, strin);
    }else if(string_arr_cmp(str,"ord")){
        function_var->type_params = malloc(sizeof (t_str_param));
        if(!function_var->type_params){
            return ERROR_INTERNAL;
        }
        string_param_init(function_var->type_params);
        string_wright_arr(str, strin);
        string_param_copy_string(function_var->type_params, str);
        string_wright_arr(str, integ);
        string_param_copy_string(function_var->type_params, str);
        function_var->count_params = 2;

        string_wright_arr(str, integ);

    }else if(string_arr_cmp(str,"chr")){
        function_var->type_params = malloc(sizeof (t_str_param));
        if(!function_var->type_params){
            return ERROR_INTERNAL;
        }
        string_param_init(function_var->type_params);
        string_wright_arr(str, integ);
        string_param_copy_string(function_var->type_params, str);
        function_var->count_params = 1;

        string_wright_arr(str, strin);
    }

    string_param_copy_string(function_var->type_returned_params, str);
    global_table = tree_insert(global_table, hashcode(function_var->name->data),function_var);

    string_free(str);
    return IT_IS_OK;
}

void send_ast(){
    code_gen(ast_node);
    if(ast_node->it_is_in_function || ast_node->it_is_function_define){
        node* gl = ast_node->global;
        node* in_f = ast_node->in_function;
        ast_free(ast_node);
        ast_init(ast_node);
        ast_node->it_is_in_function = true;
        ast_node->global = gl;
        ast_node->in_function = in_f;
    }else{
        ast_free(ast_node);
        ast_init(ast_node);
    }
}

int start_analysis(t_token *token){

    ast_node = malloc(sizeof (t_ast_node));
    ast_init(ast_node);
    add_table_symbols_system_function("reads");
    add_table_symbols_system_function("readi");
    add_table_symbols_system_function("readn");
    add_table_symbols_system_function("write");
    add_table_symbols_system_function("tointeger");
    add_table_symbols_system_function("substr");
    add_table_symbols_system_function("ord");
    add_table_symbols_system_function("chr");

    stack_table = malloc(sizeof (s_stack));
    table_init(stack_table);

    return (start_program(token));

}

