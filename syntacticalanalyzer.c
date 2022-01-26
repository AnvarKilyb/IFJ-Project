/**
 * Project: Implementace překladače imperativního jazyka IFJ21
 *
 * File:     syntacticalanalyzer.c
 * Subject:  IFJ2021
 *
 * @author:  Vladislav Mikheda  	xmikhe00
 * @author:  Khrisanov Vladislav    xkhris00
 * @author:  Kilybayev Anvar        xkilyb00
 * @author:  Gazizov Zhasdauren     xgaziz00
 */



#include "syntacticalanalyzer.h"
#include "precedence_sa.h"
#include "codgen.h"
char func[] = "function";
char strin[] = "string";
char integ[] = "integer";
char numb[] = "number";
char nil[] = "nil";
e_error_message ERROR_ALL = IT_IS_OK;
bool error_check = false;


node *global_table;
s_stack *stack_table;
t_ast_node* ast_node;

int start_program(t_token *token){

    GET_TOKEN(token);

    if(token->lexeme->keyword == KEYWORD_REQUIRE) {
        GET_TOKEN(token);
        if (!string_arr_cmp(token->lexeme->inter, "ifj21")){
            ERROR_TEXT("expected ifj21");
            RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_ALL);
        }
    }
    else{
        ERROR_TEXT("require expected");
        RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
    }

//    code_header();
    if(chunk(token)){
//        return ERROR_SYN_ANALYSIS;
        RETURN_ERROR;
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
        RETURN_ERROR;
    }
    return IT_IS_OK;
}

int function(t_token *token){
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
                    ERROR_TEXT("the function has already been declared");
                    RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
                }else{//если функция уже была дифинована
                    repeat_function->declaration = true;
                    repeat_function->it_is_define = false;
                }
                //если функция не была не декларована не дифинована
            }else {
                global_function = malloc(sizeof(sData));
                if (!global_function) {
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                s_data_init(global_function);
                global_function->type = malloc(sizeof (t_str));
                if(!global_function->type){
                    free(global_function);
                    global_function = NULL;
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                if(string_init(global_function->type)){
                    free(global_function);
                    global_function = NULL;
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                if(string_wright_arr(global_function->type, func)){
                    string_free(global_function->type);
                    free(global_function);
                    global_function = NULL;
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                global_function->declaration = true;
                global_function->name = malloc(sizeof(t_str));
                if(!global_function->name){
                    string_free(global_function->type);
                    free(global_function);
                    global_function = NULL;
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                string_init(global_function->name);
                if(string_copy(token->lexeme->inter, global_function->name)){
                    string_free(global_function->name);
                    string_free(global_function->type);
                    free(global_function);
                    global_function = NULL;
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                // заливаем узел в дерево
                global_table = tree_insert(global_table, hashcode(global_function->name->data), global_function);
                if(!global_table){ // проверка ошибок
                    string_free(global_function->name);
                    string_free(global_function->type);
                    free(global_function);
                    global_function = NULL;
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
            }
        }else{
            ERROR_TEXT("expected function name");
            RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
        }


        GET_TOKEN(token);
        if(token->token_name != TOKEN_ASSIGNMENT_TYPE){
            ERROR_TEXT("colon expected");
            RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
        }

        GET_TOKEN(token);
        if(token->token_name != TOKEN_KEYWORD && token->lexeme->keyword != KEYWORD_FUNCTION){
            ERROR_TEXT("expected keyword FUNCTION");
            RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
        }

        GET_TOKEN(token);
        if(token->token_name != TOKEN_LEFT_BRACKET) {
            ERROR_TEXT("left bracket expected");
            RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
        }

        //повторется функция или нет
        if(repeat_function){
            if (global_params(token, hashcode(repeat_function->name->data))) {
                RETURN_ERROR;
            }
        }else{
            if (global_params(token, hashcode(global_function->name->data))) {
                RETURN_ERROR;
            }
        }


        GET_TOKEN(token);
        if(token->token_name != TOKEN_RIGHT_BRACKET) {
            //обработка ошибок ожидалась скобка
            ERROR_TEXT("right bracket expected");
            RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
        }

        if(repeat_function) {
            if (return_types(token, hashcode(repeat_function->name->data))) {
                RETURN_ERROR;
            }
        }else{
            if (return_types(token, hashcode(global_function->name->data))) {
                RETURN_ERROR;
            }
        }

        if(chunk(token)){
            RETURN_ERROR;
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
                    ERROR_TEXT("the function has already been defined");
                    RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
                } else {//если функция уже была дифинована
                    repeat_function->define = true;
                    repeat_function->it_is_define = true;
                }
            }else{
                global_function = malloc(sizeof(sData));
                if (!global_function) {
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                s_data_init(global_function);
                global_function->type = malloc(sizeof (t_str));
                if(!global_function->type){
                    free(global_function);
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                if(string_init(global_function->type)){
                    free(global_function->type);
                    free(global_function);
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                if(string_wright_arr(global_function->type, func)){
                    string_free(global_function->type);
                    free(global_function);
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                global_function->define = true;
                global_function->it_is_define = true;
                global_function->name = malloc(sizeof(t_str));
                if(!global_function->name){
                    string_free(global_function->type);
                    free(global_function);
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                if(string_init(global_function->name)){
                    string_free(global_function->type);
                    free(global_function->name);
                    free(global_function);
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                if(string_copy(token->lexeme->inter, global_function->name)){
                    string_free(global_function->type);
                    string_free(global_function->name);
                    free(global_function);
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                // заливаем узел в дерево
                global_table = tree_insert(global_table, hashcode(global_function->name->data), global_function);
                if(!global_table){
                    string_free(global_function->type);
                    string_free(global_function->name);
                    free(global_function);
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
            }
        }else{
            RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
        }

        GET_TOKEN(token);
        if(token->token_name != TOKEN_LEFT_BRACKET) {
            //ожидалась скобка
            ERROR_TEXT("left bracket expected");
            RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
        }

        if(repeat_function) {
            if (params(token, hashcode(repeat_function->name->data))) {
                RETURN_ERROR;
            }
        }else{
            if (params(token, hashcode(global_function->name->data))) {
                RETURN_ERROR;
            }
        }

        GET_TOKEN(token);
        if(token->token_name != TOKEN_RIGHT_BRACKET) {
            //обработка ошибок ожидалась скобка
            ERROR_TEXT("right bracket expected");
            RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
        }
        if(repeat_function) {
            if (return_types(token, hashcode(repeat_function->name->data))) {
                RETURN_ERROR;
            }
        }else{
            if (return_types(token, hashcode(global_function->name->data))) {
                RETURN_ERROR;
            }
        }

        //////////
        ast_node->it_is_function_define = true;
        if(repeat_function){
            ast_node->func = tree_search(global_table, hashcode(repeat_function->name->data))->data;
        }
        else
            ast_node->func = tree_search(global_table, hashcode(global_function->name->data))->data;

        ast_node->function_info = ast_node->func;
        ast_node->global = global_table;
        ast_node->in_function = table_top(stack_table);
        if(!ast_node->in_function){
            fake_variable();
            ast_node->in_function = table_top(stack_table);
        }

        ast_node->it_is_in_function = true;
        send_ast();
        //////////

        if(statement(token)){
            RETURN_ERROR;
        }

        if(token->token_name == TOKEN_KEYWORD && token->lexeme->keyword == KEYWORD_END){
            ast_node->it_is_function_end = true;
            send_ast();
            ast_node->it_is_in_function = false;
            ast_node->function_info = NULL;
            if(ast_node->in_function){
                node* func_del = table_pop(stack_table);
                if(func_del)
                    tree_delete(func_del);

                ast_node->in_function = NULL;
            }
            if(chunk(token)){
                RETURN_ERROR;
            }
        }else{
            //ожидался end
            ERROR_TEXT("expected end");
            RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
        }
        return IT_IS_OK;
    }

    else{
        GET_TOKEN(token);
        if(token->token_name == TOKEN_LEFT_BRACKET){
            hold_token();
            get_old_token(token);
            if (function_call(token)) {
                RETURN_ERROR;
            }else{
                if(!ast_node->it_is_loop_or_if) {
                    if (send_ast()) {
                        RETURN_ERROR;
                    }
                }
            }
            if (chunk(token)) {
                RETURN_ERROR;
            }

            return IT_IS_OK;
        }else{
            get_old_token(token);
            ERROR_TEXT("Either a function definition, a function declaration, or a function call was expected");
            RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
        }
    }

    return IT_IS_OK;
}


int return_types(t_token *token, ul hash){ /// проверенная
    bool ret_param = false;
    GET_TOKEN(token);
    node *function_node = tree_search(global_table, hash);
    //token  == :
    if(token->token_name == TOKEN_ASSIGNMENT_TYPE) {
        if ((function_node->data->declaration && !(function_node->data->define)) || (!(function_node->data->declaration) && function_node->data->define)) {

            function_node->data->type_returned_params = malloc(sizeof(t_str_param));
            if(!function_node->data->type_returned_params){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }

            if(string_param_init(function_node->data->type_returned_params)){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }

            ret_param = true;
            if (data_type(token,function_node, ret_param)) {
                RETURN_ERROR;
            }
            if (next_data_type(token, function_node, ret_param)) {
                RETURN_ERROR;
            }
        }else if(function_node->data->declaration && function_node->data->define){
            ret_param = true;
//            function_node->data->help_count++;
            if (data_type(token,function_node, ret_param)) {
                RETURN_ERROR;
            }
            if (next_data_type(token, function_node, ret_param)) {
                RETURN_ERROR;
            }
        }
    }else{
        if(function_node->data->declaration && function_node->data->define){
            if (function_node->data->count_returned_params != function_node->data->help_count) {
                //количество возвращаемых параметров не соответсвует в делорации и дефениции
                ERROR_TEXT("the return function type of parameters in the definition and declaration does not match");
                RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
            }
        }
        hold_token();
    }
    // -> ε
    return IT_IS_OK;
}

int variable_data_type(t_token* token, ul hash){ ///проверенна
    GET_TOKEN(token);
    if(token->lexeme->keyword == KEYWORD_INTEGER || token->lexeme->keyword == KEYWORD_NUMBER || token->lexeme->keyword == KEYWORD_STRING || token->lexeme->keyword == KEYWORD_NIL){
        node* in_function = tree_search(table_top(stack_table),hash);
        //уложим тип переменной до табулки символов
        in_function->data->type = malloc(sizeof (t_str));
        if(!in_function->data->type){
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }
        if(string_init(in_function->data->type)){
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }
        if(string_copy(token->lexeme->inter,in_function->data->type)){
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }
        //уложим тип перемменной до ast
        ast_node->type_variable = malloc(sizeof (t_str_param));
        if(!ast_node->type_variable){
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }
        if(string_param_init(ast_node->type_variable)){
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }
        if(string_param_copy_string(ast_node->type_variable, token->lexeme->inter)){
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }
    }else{
        ERROR_TEXT("expected variable type");
        RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
    }
    return IT_IS_OK;
}

int statement(t_token *token){ ///проверенная кроме if и while
    GET_TOKEN(token);
    if(token->token_name == TOKEN_KEYWORD && token->lexeme->keyword == KEYWORD_LOCAL){
        if(ast_node->it_is_loop_or_if){
            if (if_loop_ast_next()) {
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
        }
        sData* function_var = NULL;
        GET_TOKEN(token);
        if(token->token_name == TOKEN_IDENTIFIER){
            ul hash = hashcode(token->lexeme->inter->data);

            node* check_var = NULL;
//                    tree_search(table_top(stack_table),hash);
//            if(check_var){
//                ERROR_TEXT("a variable with the given name has already been defined in the block");
//                RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
//            }
            if(ast_node->it_is_loop_or_if){
                if(ast_node->local){
                    check_var = tree_search(ast_node->local, hash);
                    if (check_var) {
                        ERROR_TEXT("a variable with the given name has already been defined in the block");
                        RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
                    }
                }
            }else{
                if(ast_node->in_function){
                    check_var = tree_search(ast_node->in_function, hash);
                    if (check_var) {
                        ERROR_TEXT("a variable with the given name has already been defined in the block");
                        RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
                    }
                }
            }


            check_var = tree_search(global_table,hash);
            if(check_var){
                ERROR_TEXT("the name of the variable being defined is the same as the name of the function");
                RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
            }
            function_var = malloc(sizeof (sData));
            if(!function_var){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            s_data_init(function_var);
            function_var->name = malloc(sizeof (t_str));
            if(!function_var->name){
                free(function_var);
                function_var = NULL;
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(string_init(function_var->name)){
                free(function_var->name);
                free(function_var);
                function_var = NULL;
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(string_copy(token->lexeme->inter, function_var->name)){
                string_free(function_var->name);
                free(function_var);
                function_var = NULL;
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }

            if(ast_node->it_is_loop_or_if)
            {
                function_var->counter_in = ast_node->count_nesting;
                if(ast_node->local){
                    node* in_function = NULL;
                    in_function = table_top(stack_table);
                    in_function = tree_insert(in_function, hash, function_var);
                    if(!in_function){
                        free(function_var->name);
                        free(function_var);
                        function_var = NULL;
                        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                    }
                }else{
                    node* in_function = NULL;
                    in_function = tree_insert(in_function, hash, function_var);
                    if (table_push(stack_table,in_function)){
                        in_function = tree_delete(in_function);
                        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                    }

                    ast_node->local = table_top(stack_table);
                }
            }else{

                if(ast_node->in_function){
                    node* in_function = NULL;
                    in_function = table_top(stack_table);
                    in_function = tree_insert(in_function, hash, function_var);
                    if(!in_function){
                        free(function_var->name);
                        free(function_var);
                        function_var = NULL;
                        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                    }
                }else{
                    node* in_function = NULL;
                    in_function = tree_insert(in_function, hash, function_var);
                    if (table_push(stack_table,in_function)){
                        in_function = tree_delete(in_function);
                        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                    }

                    ast_node->in_function= table_top(stack_table);
                }
            }

//            if(!ast_node->in_function){
//                ast_node->in_function = table_top(stack_table);
//            }
            ast_node->it_is_declaration_variable = true;
            ast_node->count_variable++;
            ast_node->variable = malloc(sizeof (t_str_param));
            if(!ast_node->variable){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(string_param_init(ast_node->variable)){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(string_param_copy_string(ast_node->variable,token->lexeme->inter)){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
        }else
        {
            ERROR_TEXT("variable name expected");
            RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
        }
        GET_TOKEN(token);
        if(token->token_name != TOKEN_ASSIGNMENT_TYPE){
            ERROR_TEXT("colon was expected after variable name");
            RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
        }

        if(variable_data_type(token, hashcode(function_var->name->data))){
            RETURN_ERROR;
        }

        GET_TOKEN(token);
        if(token->token_name == TOKEN_ASSIGNMENT){
            ast_node->it_is_variable_ = true;
            if(value(token)){
                RETURN_ERROR;
            }else{
                if(!ast_node->it_is_loop_or_if) {
                    if(send_ast()){
                        RETURN_ERROR;
                    }
                }
            }
        }else{ //если не равно то мы должны либо отправить узел, либо продолжить лист если это while или if
            hold_token();
            if(!ast_node->it_is_loop_or_if) {
                if(send_ast()){
                    RETURN_ERROR;
                }
            }
        }

        if(statement(token)){
            RETURN_ERROR;
        }

    }else if(token->token_name == TOKEN_IDENTIFIER){
        if(ast_node->it_is_loop_or_if){
            if (if_loop_ast_next()) {
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
        }
        GET_TOKEN(token);
        if(token->token_name == TOKEN_LEFT_BRACKET) {
            hold_token();
            get_old_token(token);
            if(function_call(token)){
                RETURN_ERROR;
            }else{
                if(!ast_node->it_is_loop_or_if){
                    if(send_ast()){
                        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                    }
                }
            }
            if(statement(token)){
                RETURN_ERROR;
            }
        }else{
            hold_token();
            get_old_token(token);
            node *function_var = NULL;
            ul hash = hashcode(token->lexeme->inter->data);

            bool error_nul = false;
            function_var = check_type_stack(error_nul,hash);
            if(error_nul){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if (!function_var) {
                ERROR_TEXT("variable was not defined");
                RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
            }

            ast_node->it_is_variable_ = true;
            ast_node->count_variable++;
            ast_node->variable = malloc(sizeof(t_str_param));
            if (!ast_node->variable) {
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if (string_param_init(ast_node->variable)) {
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if (string_param_copy_string(ast_node->variable, token->lexeme->inter)) {
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            //тип
            ast_node->type_variable = malloc(sizeof(t_str_param));
            if (!ast_node->type_variable) {
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if (string_param_init(ast_node->type_variable)) {
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if (string_param_copy_string(ast_node->type_variable, function_var->data->type)) {
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }

            if (next_id(token)) {
                RETURN_ERROR;
            }

            GET_TOKEN(token);
            if (token->token_name != TOKEN_ASSIGNMENT) {
                ERROR_TEXT("equal sign expected");
                RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
            }

            if (value(token)) {
                RETURN_ERROR;
            }else {
                if(!ast_node->it_is_loop_or_if) {
                    if(send_ast()){
                        RETURN_ERROR;
                    }
                }
            }

            if (statement(token)) {
                RETURN_ERROR;
            }
        }

    }else if(token->token_name == TOKEN_KEYWORD && token->lexeme->keyword == KEYWORD_IF ){
        if(ast_node->it_is_loop_or_if){
            if (if_loop_ast_next()) {
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
        }

        ast_node->local = NULL;
        ///
        if(fake_variable()){
            RETURN_ERROR;
        }
        ast_node->local = table_top(stack_table);
        ///
        ast_node->count_nesting++;
        ast_node->it_is_if = true;
        ast_node->it_is_start_if_loop = true;
        ast_node->it_is_loop_or_if = true;
        ast_node->it_is_loop = false;

        if(!ast_node->expression){
            ast_node->expression = malloc(sizeof (t_exp_list));
            if(!ast_node->expression){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            exp_init(ast_node->expression);
        }

        GET_TOKEN(token);

        AST_leaf *tree = NULL;
        tree = precede_expression(token, &ERROR_ALL);
        if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS){
            ERROR_TEXT("invalid expression");
            RETURN_ERROR_NUMBER(ERROR_ALL);
        }
        else if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS_EXPR){
            ERROR_TEXT("incompatible type");
            RETURN_ERROR_NUMBER(ERROR_ALL);
        }
        ERROR_ALL = check_expression(tree,ast_node);
        ast_node->expression->preced_expression_tree = tree;
        ast_node->expression->tree = true;

        if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS){
            ERROR_TEXT("invalid expression");
            RETURN_ERROR_NUMBER(ERROR_ALL);
        }
        else if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS_EXPR){
            ERROR_TEXT("incompatible type");
            RETURN_ERROR_NUMBER(ERROR_ALL);
        }
//        if_loop_ast_next();


        GET_TOKEN(token);
        if(token->token_name != TOKEN_KEYWORD && token->lexeme->keyword != KEYWORD_THEN){
            ERROR_TEXT("expected THEN");
            RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
        }

        if(statement(token)){
            RETURN_ERROR;
        }

//        GET_TOKEN(token);
        if(if_loop_ast_next()){
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }
        if(token->token_name == TOKEN_KEYWORD && token->lexeme->keyword == KEYWORD_ELSE){
            ast_node->if_else = true;
            ast_node->it_is_if = true;
        } else{
            ERROR_TEXT("expected ELSE");
            RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
//            if(if_loop_ast_next()){
//                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
//            }
        }

        if(statement(token)){
            RETURN_ERROR;
        }
        if(if_loop_ast_next()){
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }
        if(token->token_name == TOKEN_KEYWORD && token->lexeme->keyword == KEYWORD_END){
            if(ast_node->count_nesting == 1) {
                ast_node->it_is_loop = false;
                ast_node->it_is_if = true;
                ast_node->if_loop_end = true;
                ast_node = ast_node->first_node;
//                if(ast_node->expression->first_exp)
//                    ast_node->expression = ast_node->expression->first_exp;
                ast_node->count_nesting--;
                if(ast_node->local){
                    node* local_del = table_pop(stack_table);
                    if(local_del)
                        tree_delete(local_del);

                    ast_node->local = table_top(stack_table);
                }
                send_ast();
            }else if(ast_node->count_nesting > 1){
                ast_node->it_is_loop = false;
                ast_node->if_loop_end = true;
                ast_node->it_is_if = true;
                ast_node->count_nesting--;
                if(ast_node->local){
                    node* local_del = table_pop(stack_table);
                    if(local_del)
                        tree_delete(local_del);

                    ast_node->local = table_top(stack_table);
                }
            }else{
                ERROR_TEXT("structure nesting error");
                RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
            }
        }else{
            ERROR_TEXT("expected END");
            RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
        }

        if(statement(token)){
            RETURN_ERROR;
        }

    }else if(token->token_name == TOKEN_KEYWORD && token->lexeme->keyword == KEYWORD_WHILE){
        if(ast_node->it_is_loop_or_if){
            if (if_loop_ast_next()) {
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
        }
        ast_node->local = NULL;
        ///
        if(fake_variable()){
            RETURN_ERROR;
        }
        ast_node->local = table_top(stack_table);
        ///
        ast_node->count_nesting++;
        ast_node->it_is_loop = true;
        ast_node->it_is_start_if_loop = true;
        ast_node->it_is_loop_or_if = true;
        ast_node->it_is_if = false;

        if(!ast_node->expression){
            ast_node->expression = malloc(sizeof (t_exp_list));
            if(!ast_node->expression){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            exp_init(ast_node->expression);
        }

        GET_TOKEN(token);

        AST_leaf *tree;
        tree = precede_expression(token, &ERROR_ALL);
        if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS){
            ERROR_TEXT("invalid expression");
            RETURN_ERROR_NUMBER(ERROR_ALL);
        }
        else if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS_EXPR){
            ERROR_TEXT("incompatible type");
            RETURN_ERROR_NUMBER(ERROR_ALL);
        }
        ERROR_ALL = check_expression(tree,ast_node);
        ast_node->expression->preced_expression_tree = tree;
        ast_node->expression->tree = true;

        if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS){
            ERROR_TEXT("invalid expression");
            RETURN_ERROR_NUMBER(ERROR_ALL);
        }
        else if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS_EXPR){
            ERROR_TEXT("incompatible type");
            RETURN_ERROR_NUMBER(ERROR_ALL);
        }


        GET_TOKEN(token);
        if(token->token_name != TOKEN_KEYWORD && token->lexeme->keyword != KEYWORD_DO){
            ERROR_TEXT("expected DO");
            RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
        }

        if(statement(token)){
            RETURN_ERROR;
        }

//        GET_TOKEN(token);
        if(if_loop_ast_next()){
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }
        if(token->token_name == TOKEN_KEYWORD && token->lexeme->keyword == KEYWORD_END){
            if(ast_node->count_nesting == 1) {
                ast_node->it_is_if = false;
                ast_node->it_is_loop = true;
                ast_node->if_loop_end = true;
                ast_node = ast_node->first_node;
                if(ast_node->local){
                    node* local_del = table_pop(stack_table);
                    if(local_del)
                        tree_delete(local_del);

                    ast_node->local = table_top(stack_table);
                }
                send_ast();
            }else if(ast_node->count_nesting > 1){
                ast_node->it_is_if = false;
                ast_node->it_is_loop = true;
                ast_node->if_loop_end = true;
                ast_node->count_nesting--;
                if(ast_node->local){
                    node* local_del = table_pop(stack_table);
                    if(local_del)
                        tree_delete(local_del);

                    ast_node->local = table_top(stack_table);
                }
            }else{
                ERROR_TEXT("structure nesting error");
                RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
            }
        }else{
            ERROR_TEXT("expected END");
            RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
        }

        if(statement(token)){
            RETURN_ERROR;
        }

    }else if(token->token_name == TOKEN_KEYWORD && token->lexeme->keyword == KEYWORD_RETURN){
        if(ast_node->it_is_loop_or_if){
            if (if_loop_ast_next()) {
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
        }
        if(return_exp(token)){
            RETURN_ERROR;
        }else{
            if(ast_node->count_expression < ast_node->function_info->count_returned_params){
                ast_node->it_is_return_exp = true;
                while(ast_node->count_expression < ast_node->function_info->count_returned_params){
                    if(exp_next()){
                        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                    }
                    ast_node->expression->nil = true;
                    ast_node->count_expression++;
                }
                if(ast_node->expression->first_exp){
                    ast_node->expression = ast_node->expression->first_exp;
                }
            }
            if(!ast_node->it_is_loop_or_if) {
                if(send_ast()){
                    RETURN_ERROR;
                }
            }
        }
        if(statement(token)){
            RETURN_ERROR;
        }
    }
    else{
        return IT_IS_OK;
        if(statement(token)){
            RETURN_ERROR;
        }
    }
    return IT_IS_OK;
}

int return_exp(t_token* token){ ///проверенная
    ast_node->it_is_return = true;
    ast_node->function_info->was_return = true;

    GET_TOKEN(token);
    if((token->lexeme->keyword != KEYWORD_END) && (token->lexeme->keyword != KEYWORD_ELSE)){
        if (ast_node->function_info->count_returned_params) {
            ast_node->it_is_return_exp = true;
            hold_token();
            if(value(token)){
                RETURN_ERROR;
            }
            return IT_IS_OK;
        }else{
            ERROR_TEXT("the function should not return parameters");
            RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC);
        }
    }else{
        hold_token();
    }
    return IT_IS_OK;
}


int function_call(t_token *token){ /// проверенная
    node* global_function = tree_search(global_table, hashcode(token->lexeme->inter->data));
    if(!global_function){
        ERROR_TEXT("the function before the call was neither declared nor defined");
        RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
    }

    ast_node->func = global_function->data;
    if(ast_node->it_is_variable_){
        ast_node->it_is_variable_call_function = true;
        ast_node->it_is_variable_expression = false;
        ast_node->it_is_variable_ = false;

        if(ast_node->count_variable > global_function->data->count_returned_params){
            //параметров которыйе принимают возврат с функции больше чем возвращаемых параметров
            ERROR_TEXT("there are more parameters on the left side of the expression than the function returns");
            RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC);
        }else{
            for(int i = 1; i <= ast_node->count_variable; i++){
                if(!string_param_cmp_string_param(ast_node->type_variable, i,global_function->data->type_returned_params, i)){
                    if(string_param_cmp_arr(ast_node->type_variable,i,numb) && string_param_cmp_arr(global_function->data->type_returned_params, i, integ))
                        continue; //если ожидем number а возвращается инт

                    //не сходятся типы возвращаемых аргументов
                    ERROR_TEXT("the types of the arguments on the left side of the expression and the return types of the arguments do not match");
                    RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC);
                }
            }
        }

    }else{
        ast_node->it_is_call_function = true;
    }

    GET_TOKEN(token);
    if(token->token_name != TOKEN_LEFT_BRACKET) {
        //ожидалась скобка
        ERROR_TEXT("left bracket expected");
        RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
    }

    if(global_function->data->count_params != 0 || global_function->data->system_function_infinity_param){
        if (args(token)) {
            RETURN_ERROR;
        }
    }

    GET_TOKEN(token);
    if(token->token_name != TOKEN_RIGHT_BRACKET) {
        if(global_function->data->count_params == 0 ){
            ERROR_TEXT("function call with unexpected parameters");
            RETURN_ERROR_NUMBER( ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC);
        }
        //ожидалась скобка
        ERROR_TEXT("right bracket expected");
        RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
    }
    //TODO отослать ast стром
    return IT_IS_OK;
}

int args(t_token *token){ /// проверенная
    GET_TOKEN(token);
    if((ast_node->count_func_param >= ast_node->func->count_params) && !ast_node->func->system_function_infinity_param){
        ERROR_TEXT("the number of function parameters received does not match the declaration or definition");
        RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC);
    }else{
        ast_node->count_func_param++;
    }

//    if(!ast_node->func_param){
//        ast_node->func_param = malloc(sizeof (t_str_param));
//        string_param_init(ast_node->func_param);
//    }
    if(!ast_node->expression){
        ast_node->expression = malloc(sizeof (t_exp_list));
        if(!ast_node->expression){
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }
        exp_init(ast_node->expression);
    }

    if(string_arr_cmp(ast_node->func->name,"write")){
        ast_node->func->count_params =  ast_node->count_func_param;
        if(token->token_name == TOKEN_IDENTIFIER){
            bool error_nul = false;
            node *function_var = check_type_stack(error_nul,hashcode(token->lexeme->inter->data));
            if(error_nul){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(!function_var){
                ERROR_TEXT("variable is used before definition");
                RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
            }
            ast_node->expression->var = true;
            if(string_arr_cmp(function_var->data->type,numb)){
                ast_node->expression->numb = true;
            }else if(string_arr_cmp(function_var->data->type,integ)){
                ast_node->expression->integer = true;
            }else if(string_arr_cmp(function_var->data->type,strin)){
                ast_node->expression->str = true;
            }else if(string_arr_cmp(function_var->data->type,nil)){
                ast_node->expression->nil = true;
            }
        }else if(token->token_name == TOKEN_NUMBER || token->token_name == TOKEN_NUMBER_EXPONENT){
            ast_node->expression->numb = true;
            ast_node->expression->data_double = token->lexeme->number;
        }else if(token->token_name == TOKEN_INTEGER){
            ast_node->expression->integer = true;
        }else if(token->token_name == TOKEN_STRING){
            ast_node->expression->str = true;
        }

        ast_node->expression->data = malloc(sizeof(t_str));
        if(!ast_node->expression->data){
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }
        if(string_init(ast_node->expression->data)){
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }
        if(string_copy(token->lexeme->inter,ast_node->expression->data)){
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }
    }else if(token->token_name == TOKEN_INTEGER){
        if(string_param_cmp_arr(ast_node->func->type_params,ast_node->count_func_param,integ) ||
           string_param_cmp_arr(ast_node->func->type_params,ast_node->count_func_param,numb)){
            //записываем число в параметры;
            ast_node->expression->integer = true;
            ast_node->expression->data = malloc(sizeof(t_str));
            if(!ast_node->expression->data){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(string_init(ast_node->expression->data)){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(string_copy(token->lexeme->inter,ast_node->expression->data)){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
        }else{
            ERROR_TEXT("the function does not take an integer parameter as the given parameter");
            RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC);
        }
    }else if(token->token_name == TOKEN_STRING){
        if(string_param_cmp_arr(ast_node->func->type_params,ast_node->count_func_param,strin)){
            //записываем строку в параметры;
            ast_node->expression->str = true;
            ast_node->expression->data = malloc(sizeof(t_str));
            if(!ast_node->expression->data){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(string_init(ast_node->expression->data)){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(string_copy(token->lexeme->inter,ast_node->expression->data)){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
        }else{
            ERROR_TEXT("the function does not take a string parameter as the given parameter");
            RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC);
        }
    }else if(token->token_name == TOKEN_NUMBER || token->token_name == TOKEN_NUMBER_EXPONENT){
        if(string_param_cmp_arr(ast_node->func->type_params,ast_node->count_func_param,numb)){
            //записываем число в параметры;
            ast_node->expression->numb = true;
            ast_node->expression->data_double = token->lexeme->number;
            ast_node->expression->data = malloc(sizeof(t_str));
            if(!ast_node->expression->data){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(string_init(ast_node->expression->data)){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(string_copy(token->lexeme->inter,ast_node->expression->data)){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
        }else{
            ERROR_TEXT("the function does not take a number parameter as the given parameter");
            RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC);
        }
    }else if(token->token_name == TOKEN_IDENTIFIER){
        if(ast_node->it_is_in_function){
            bool error_nul = false;
            node *function_var = check_type_stack(error_nul,hashcode(token->lexeme->inter->data));
            if(error_nul){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(function_var){
                //проверяем на тип
                if((string_param_cmp_string(ast_node->func->type_params,ast_node->count_func_param,function_var->data->type)) ||
                        (string_param_cmp_arr(ast_node->func->type_params,ast_node->count_func_param,numb) &&
                                string_arr_cmp(function_var->data->type,integ))){
                    //записываем
                    ast_node->expression->var = true;
                    if(string_arr_cmp(function_var->data->type,numb)){
                        ast_node->expression->numb = true;
                    }else if(string_arr_cmp(function_var->data->type,integ)){
                        ast_node->expression->integer = true;
                    }else if(string_arr_cmp(function_var->data->type,strin)){
                        ast_node->expression->str = true;
                    }else if(string_arr_cmp(function_var->data->type,nil)){
                        ast_node->expression->nil = true;
                    }
                    ast_node->expression->data = malloc(sizeof(t_str));
                    if(!ast_node->expression->data){
                        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                    }
                    if(string_init(ast_node->expression->data)){
                        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                    }
                    if(string_copy(token->lexeme->inter,ast_node->expression->data)){
                        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                    }
                }else{
                    ERROR_TEXT("the function does not accept parameters of this type as the given parameter");
                    RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC);
                }
            }else{
                ERROR_TEXT("variable is used before definition");
                RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
            }
        }else{
            ERROR_TEXT("passing a parameter to a function, a variable in the main part of the program");
            RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
        }
    }else if(token->token_name == TOKEN_KEYWORD && token->lexeme->keyword == KEYWORD_NIL){
        ast_node->expression->nil = true;
        ast_node->expression->data = malloc(sizeof(t_str));
        if(!ast_node->expression->data){
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }
        if(string_init(ast_node->expression->data)){
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }
        if(string_copy(token->lexeme->inter,ast_node->expression->data)){
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }
    }else{
        // ожидался параметр
        ERROR_TEXT("expected function parameter");
        RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC);
    }
    if(next_args(token)){
        RETURN_ERROR;
    }
    return IT_IS_OK;
}

int next_args(t_token* token){ /// проверенная
    GET_TOKEN(token);
    if(token->token_name == TOKEN_COMMA){
        if(exp_next()){
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }
        if(args(token)){
            RETURN_ERROR;
        }

    }else{
        if(ast_node->count_func_param == ast_node->func->count_params){
            if(ast_node->expression->first_exp){
                ast_node->expression = ast_node->expression->first_exp;
            }
            hold_token();
            return IT_IS_OK;
        }
        ERROR_TEXT("the number of parameters of the function call and in its declarations or definitions do not match");
        RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC);
    }
    return IT_IS_OK;
}

int next_data_type(t_token *token, node* function_node, bool ret_param){ /// проверенная
    GET_TOKEN(token);
    //token == ,
    if(token->token_name == TOKEN_COMMA){
        if(data_type(token,function_node,ret_param)){
            RETURN_ERROR;
        }
        if(next_data_type(token,function_node,ret_param)){
            RETURN_ERROR;
        }
    }else{
        //если функция была и декларована и дифинована то проверяем сходятся ли счетчики параметров после проврки
        if(function_node->data->declaration && function_node->data->define){
            if((function_node->data->count_returned_params != function_node->data->help_count) && ret_param){
                //количество возвращемых параметров в дефениции и деклорацуии не соответсвует
                ERROR_TEXT("the number of function parameters in the declaration and definition does not match");
                RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
            }
            if ((function_node->data->count_params != function_node->data->help_count) && !ret_param) {
                //корличесво парметров не соответствует в деклорации и дефеницуии
                ERROR_TEXT("the number of function parameters in the declaration and definition does not match");
                RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
            }
        }
        hold_token();
    }

    // -> ε
    return IT_IS_OK;
}

int data_type(t_token *token, node* function_node, bool ret_param){ /// проверенная
    GET_TOKEN(token);
    //если это не возвращаемые параметры то
    if(token->lexeme->keyword == KEYWORD_INTEGER || token->lexeme->keyword == KEYWORD_NUMBER || token->lexeme->keyword == KEYWORD_STRING || token->lexeme->keyword == KEYWORD_NIL){
        if (!ret_param) {//если функция только декларовна
            if (function_node->data->declaration && !(function_node->data->define)) {
                //копируем тип глобальную таблицу
                if(string_param_copy_string(function_node->data->type_params, token->lexeme->inter)){
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                //увеличиваем счетчик переменных
                function_node->data->count_params++;

                //если функция была и дикларована и дифинована
            }else if (function_node->data->declaration && function_node->data->define){
                //увеличиваем количество переменных на +1
                function_node->data->help_count++;
                //проверяем одинаковый ли тип у перменных в дифиниции и деклорации
                if(!function_node->data->type_params){
                    //не сответствует тип переменнорй в декларации и дефиниции
                    ERROR_TEXT("function parameter type in definition and declaration does not match");
                    RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
                }
                if (!string_param_cmp_string(function_node->data->type_params,(function_node->data->help_count), token->lexeme->inter)) {
                    //не сответствует тип переменнорй в декларации и дефиниции
                    ERROR_TEXT("function parameter type in definition and declaration does not match");
                    RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
                }
                if(function_node->data->it_is_define){
                    node *in_function = tree_search(table_top(stack_table),hashcode(function_node->data->params->data[function_node->data->help_count - 1]->data));

                    in_function->data->type = malloc(sizeof(t_str)); //аллакуем место под тип переменной
                    if (!in_function->data->type){
                        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                    }

                    if(string_init(in_function->data->type)){
                        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                    }
                    if(string_copy(token->lexeme->inter,in_function->data->type)){
                        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                    }
                }
            }else if (!(function_node->data->declaration) && function_node->data->define){
                //записываем до глобальной таблицы
                if(string_param_copy_string(function_node->data->type_params, token->lexeme->inter)){
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                function_node->data->count_params++; // увеличиваем число параметров потомучто они уже записаны
                //забираем таблицу функции со стека
                node *in_function = tree_search(table_top(stack_table),hashcode(function_node->data->params->data[function_node->data->count_params - 1]->data));

                in_function->data->type = malloc(sizeof(t_str)); //аллакуем место под тип переменной
                if (!in_function->data->type){
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }

                if(string_init(in_function->data->type)){
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                //копируем тип переменой в таблицу функции
                if(string_copy(token->lexeme->inter,in_function->data->type)){
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
            }

        }else{// если это возвращаемые парамеры то
            //если функция была только декларована
            if (((function_node->data->declaration) && (!function_node->data->define)) ||
            ((!function_node->data->declaration) && (function_node->data->define))) {
                //копируем параметры до главной таблицы
                if(string_param_copy_string(function_node->data->type_returned_params, token->lexeme->inter)){
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                function_node->data->count_returned_params++; //увенличиваем счетчик

                //если функция была либо дикларована или дифинована
            }else if (function_node->data->declaration && function_node->data->define) {
                //увеличим счетчик помощник потомучто точно знаем что там будет параметр
                function_node->data->help_count++;
                //проверяем равняется ли тип параметров при дифиниции и декларации
                if(!function_node->data->type_returned_params){
                    //не сответствует тип переменнорй в декларации и дефиниции
                    ERROR_TEXT("the return function type of parameters in the definition and declaration does not match");
                    RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
                }
                if (!string_param_cmp_string(function_node->data->type_returned_params,
                                             (function_node->data->help_count), token->lexeme->inter)) {
                    //не сответствует тип переменнорй в декларации и дефиниции
                    ERROR_TEXT("the return function type of parameters in the definition and declaration does not match");
                    RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
                }
            }
        }
    }else{
        //ожидался тип параметра функции
        ERROR_TEXT("parameter type expected");
        RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
    }
    return IT_IS_OK;
}

int global_params(t_token *token, ul hash){ ///проверенная
    GET_TOKEN(token);
    bool ret_param = false;
    //token == id
    node *function_node =  tree_search(global_table, hash);
    if(token->token_name != TOKEN_RIGHT_BRACKET) {

        if(function_node->data->declaration && !(function_node->data->define)){
            function_node->data->type_params = malloc(sizeof (t_str_param));
            if(!function_node->data->type_params){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            string_param_init(function_node->data->type_params);

            hold_token();

            if(data_type(token, function_node,ret_param)){
                RETURN_ERROR;
            }

            if(next_data_type(token, function_node, ret_param)){
                RETURN_ERROR;
            }

        }else if(function_node->data->declaration && function_node->data->define) {

            hold_token();

            if (data_type(token, function_node,ret_param)) {
                RETURN_ERROR;
            }

            if (next_data_type(token, function_node, ret_param)) {
                RETURN_ERROR;
            }
            function_node->data->help_count = 0;
        }
    }else{
        if(function_node->data->count_params != 0){
            ERROR_TEXT("the number of function parameters in the declaration and definition does not match");
            RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
        }
        hold_token();
    }
    return IT_IS_OK;
}
int params(t_token *token, ul hash){ ///проверенная
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
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            function_node->data->type_params = malloc(sizeof (t_str_param));
            if(!function_node->data->type_params){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(string_param_init(function_node->data->params)){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(string_param_init(function_node->data->type_params)){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }

            function_var = malloc(sizeof (sData));
            if(!function_var){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            s_data_init(function_var);
            function_var->name = malloc(sizeof (t_str));
            if(!function_var->name){
                free(function_var);
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(string_init(function_var->name)){
                free(function_var->name);
                free(function_var);
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(string_copy(token->lexeme->inter, function_var->name)){
                string_free(function_var->name);
                free(function_var);
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }

            function_var->declaration = true;
            in_function = tree_insert(in_function, hashcode(function_var->name->data),function_var);
            if(!in_function){
                string_free(function_var->name);
                free(function_var);
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(table_push(stack_table,in_function)){
                tree_delete(in_function);
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }

            if(string_param_copy_string(function_node->data->params, token->lexeme->inter)){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }

            GET_TOKEN(token);

            if(token->token_name != TOKEN_ASSIGNMENT_TYPE){
                ERROR_TEXT("colon expected");
                RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
            }

            if(data_type(token, function_node,ret_param)){
                RETURN_ERROR;
            }

            if(next_param(token, function_node, ret_param)){
                RETURN_ERROR;
            }

        }else if(function_node->data->declaration && function_node->data->define && function_node->data->it_is_define) {
//            function_node->data->help_count++;
            function_var = malloc(sizeof (sData));
            if(!function_var){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            s_data_init(function_var);
            function_var->name = malloc(sizeof (t_str));
            if(!function_var->name){
                free(function_var);
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(string_init(function_var->name)){
                free(function_var->name);
                free(function_var);
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(string_copy(token->lexeme->inter, function_var->name)){
                string_free(function_var->name);
                free(function_var);
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            function_var->declaration = true;

            in_function = tree_insert(in_function, hashcode(function_var->name->data),function_var);
            if(!in_function){
                string_free(function_var->name);
                free(function_var);
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }

            if(table_push(stack_table,in_function)){
                tree_delete(in_function);
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }

            function_node->data->params = malloc(sizeof (t_str_param));
            if(!function_node->data->params){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(string_param_init(function_node->data->params)){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(string_param_copy_string(function_node->data->params, token->lexeme->inter)){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }


            GET_TOKEN(token);

            if (token->token_name != TOKEN_ASSIGNMENT_TYPE) {
                ERROR_TEXT("colon expected");
                RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
            }

            if (data_type(token, function_node,ret_param)) {
                RETURN_ERROR;
            }

            if (next_param(token, function_node, ret_param)) {
                RETURN_ERROR;
            }
            function_node->data->help_count = 0;
        }

    }else if(function_node->data->count_params != 0){
        //количесво параметров в декларации и дефиниции не совпадает
        ERROR_TEXT("the number of function parameters in the declaration and definition does not match");
        RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
    }else if(function_node->data->count_params == 0){
        hold_token();
    }
    // -> ε
    return IT_IS_OK;
}

int next_param(t_token *token, node* function_node, bool ret_param){ /// проверенная
    GET_TOKEN(token);
    node* in_function;
    sData* function_var;
    if(token->token_name == TOKEN_COMMA){
        GET_TOKEN(token);
        if(!sting_param_uncmp_string(function_node->data->params, token->lexeme->inter)){
            //Одинаковое название переменных
            ERROR_TEXT("a variable with the specified name was already in the function definition");
            RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
        }
        //добавляем в главную таблицу
        if(string_param_copy_string(function_node->data->params, token->lexeme->inter)){
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }

        //добавляем в таблицу функции
        in_function = table_top(stack_table);
        function_var = malloc(sizeof (sData));
        if(!function_var){
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }
        s_data_init(function_var);
        function_var->name = malloc(sizeof (t_str));
        if(!function_var->name){
            free(function_var);
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }
        if(string_init(function_var->name)){
            free(function_var);
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }
        if(string_copy(token->lexeme->inter, function_var->name)){
            string_free(function_var->name);
            free(function_var);
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }

        function_var->declaration = true;
        in_function = tree_insert(in_function, hashcode(function_var->name->data),function_var);
        if(!in_function){
            string_free(function_var->name);
            free(function_var);
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }

        GET_TOKEN(token);

        if (token->token_name != TOKEN_ASSIGNMENT_TYPE) {
            ERROR_TEXT("colon expected");
            RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
        }

        if (data_type(token, function_node, ret_param)) {
            RETURN_ERROR;
        }

        if (next_param(token,function_node, ret_param)) {
            RETURN_ERROR;
        }

    }else{// если идет не запятая
        if(function_node->data->declaration && function_node->data->define) {
            if (function_node->data->count_params != function_node->data->help_count) {
                //корличесво парметров не соответствует в деклорации и дефеницуии
                ERROR_TEXT("the number of function parameters in the declaration and definition does not match");
                RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
            }
        }
        hold_token();
    }
    // -> ε
    return IT_IS_OK;
}

int value(t_token *token){ ///проверенна
    ast_node->count_expression++;

    t_str_param* check_type = NULL;
    // счетчик для проверки равенства переменных и выражения
    int check_count = 0;
    if(ast_node->it_is_return){
        check_type = ast_node->function_info->type_returned_params;
        check_count = ast_node->function_info->count_returned_params;
    }else{
        check_type = ast_node->type_variable;
        check_count = ast_node->count_variable;
    }

    if(check_count < ast_node->count_expression){
        if(ast_node->it_is_return){
            ERROR_TEXT("the number of parameters returned by the function is more than it should be");
            RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC);
        }else{
            ERROR_TEXT("the number of expressions in the assignment is greater than the number of variables");
            RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_EQ);
        }
    }

    if(!ast_node->expression){
        ast_node->expression = malloc(sizeof (t_exp_list));
        if(!ast_node->expression){
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }
        exp_init(ast_node->expression);
    }
    if(ast_node->it_is_variable_){
        ast_node->it_is_variable_expression = true;
    }
    GET_TOKEN(token);
    if(token->token_name == TOKEN_IDENTIFIER){
        GET_TOKEN(token);
        if((token->token_name == TOKEN_LEFT_BRACKET) && !ast_node->it_is_return){
            //Есили в присвоение несколько раз вызов функции
            if (ast_node->count_expression > 1) {
                ERROR_TEXT("the function in the assignment can be called at most once");
                RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_ALL);
            }
            check_count = ast_node->count_expression;
            hold_token();
            get_old_token(token);
            if (function_call(token)) {
                RETURN_ERROR;
            }
        }else if(token->token_name == TOKEN_CONCATENATION) {
            hold_token();
            get_old_token(token);
            if (!string_param_cmp_arr(check_type, ast_node->count_expression, strin)) {
                if(ast_node->it_is_return){
                    ERROR_TEXT("expected string of variable type mismatch");
                    RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC);
                }
                ERROR_TEXT("expected string of variable type mismatch");
                RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_EXPR);
            }
            AST_leaf *tree;

            tree = precede_expression(token, &ERROR_ALL);
            if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS){
                ERROR_TEXT("invalid expression");
                RETURN_ERROR_NUMBER(ERROR_ALL);
            }
            else if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS_EXPR){
                ERROR_TEXT("incompatible type");
                RETURN_ERROR_NUMBER(ERROR_ALL);
            }
            ERROR_ALL = check_expression(tree,ast_node);
            ast_node->expression->preced_expression_tree = tree;
            ast_node->expression->tree = true;

            if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS){
                ERROR_TEXT("invalid expression");
                RETURN_ERROR_NUMBER(ERROR_ALL);
            }
            else if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS_EXPR){
                ERROR_TEXT("incompatible type");
                RETURN_ERROR_NUMBER(ERROR_ALL);
            }
            //TODO прецеденчни анализа ..вставил
        }else if(token->token_name == TOKEN_PLUS || token->token_name == TOKEN_MINUS || token->token_name == TOKEN_MULTIPLICATION || token->token_name == TOKEN_DIVISION || token->token_name == TOKEN_INT_DIVISION){
            hold_token();
            get_old_token(token);
            AST_leaf *tree;

            tree = precede_expression(token, &ERROR_ALL);
            if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS){
                ERROR_TEXT("invalid expression");
                RETURN_ERROR_NUMBER(ERROR_ALL);
            }
            else if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS_EXPR){
                ERROR_TEXT("incompatible type");
                RETURN_ERROR_NUMBER(ERROR_ALL);
            }
            ERROR_ALL = check_expression(tree,ast_node);
            ast_node->expression->preced_expression_tree = tree;
            ast_node->expression->tree = true;

            if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS){
                ERROR_TEXT("invalid expression");
                RETURN_ERROR_NUMBER(ERROR_ALL);
            }
            else if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS_EXPR){
                ERROR_TEXT("incompatible type");
                RETURN_ERROR_NUMBER(ERROR_ALL);
            }
        }else{
            hold_token();
            get_old_token(token);
            node* function_var = NULL;
            //tree_search()
            bool error_nul = false;
            function_var = check_type_stack(error_nul,hashcode(token->lexeme->inter->data));
            if(error_nul){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(!function_var){
                ERROR_TEXT("the variable being assigned was not defined");
                RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
            }

            //если переменная идентифицирована то аллакуем под нее все
            ast_node->expression->var = true;
            if(string_arr_cmp(function_var->data->type,numb)){
                ast_node->expression->numb = true;
            }else if(string_arr_cmp(function_var->data->type,integ)){
                ast_node->expression->integer = true;
            }else if(string_arr_cmp(function_var->data->type,strin)){
                ast_node->expression->str = true;
            }else if(string_arr_cmp(function_var->data->type,nil)){
                ast_node->expression->nil = true;
            }
            // проверяем тип переменной
            if(string_param_cmp_string(check_type, ast_node->count_expression, function_var->data->type) ||
            (string_param_cmp_arr(check_type,ast_node->count_expression,numb) && string_arr_cmp(function_var->data->type,integ))){
                ast_node->expression->data = malloc(sizeof(t_str));
                if(!ast_node->expression->data){
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                if(string_init(ast_node->expression->data)){
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                if(string_copy(token->lexeme->inter,ast_node->expression->data)){
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
            }else{
                if(ast_node->it_is_return){
                    ERROR_TEXT("the type of the variable returned by the function does not match the definition");
                    RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC);
                }

                ERROR_TEXT("the type of the variable and the value assigned to it do not match");
                RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_EQ);
            }
        }
    }else if(token->lexeme->keyword == KEYWORD_NIL){
//        if(string_param_cmp_arr(check_type, ast_node->count_expression, nil)){
            ast_node->expression->nil = true;
            ast_node->expression->data = malloc(sizeof(t_str));
            if(!ast_node->expression->data){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(string_init(ast_node->expression->data)){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if(string_copy(token->lexeme->inter,ast_node->expression->data)){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
//        }else{
//            if(ast_node->it_is_return){
//                ERROR_TEXT("the type of the variable returned by the function does not match the definition");
//                RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC);
//            }
//
//            ERROR_TEXT("the type of the variable and the value assigned to it do not match");
//            RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_EQ);
//        }
    }else if(token->token_name == TOKEN_STRING){
        GET_TOKEN(token);
        ast_node->expression->str =true;
        if(token->token_name == TOKEN_CONCATENATION){
            hold_token();
            get_old_token(token);

            AST_leaf *tree;

            tree = precede_expression(token, &ERROR_ALL);

            if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS){
                ERROR_TEXT("invalid expression");
                RETURN_ERROR_NUMBER(ERROR_ALL);
            }
            else if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS_EXPR){
                ERROR_TEXT("incompatible type");
                RETURN_ERROR_NUMBER(ERROR_ALL);
            }
            ERROR_ALL = check_expression(tree,ast_node);
            ast_node->expression->preced_expression_tree = tree;
            ast_node->expression->tree = true;

            if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS){
                ERROR_TEXT("invalid expression");
                RETURN_ERROR_NUMBER(ERROR_ALL);
            }
            else if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS_EXPR){
                ERROR_TEXT("incompatible type");
                RETURN_ERROR_NUMBER(ERROR_ALL);
            }
            //TODO ..вставил
        }else{
            hold_token();
            get_old_token(token);
            if (string_param_cmp_arr(check_type, ast_node->count_expression, strin)) {
                ast_node->expression->data = malloc(sizeof(t_str));
                if(!ast_node->expression->data){
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                if(string_init(ast_node->expression->data)){
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                if(string_copy(token->lexeme->inter,ast_node->expression->data)){
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
            } else {
                if(ast_node->it_is_return) {
                    ERROR_TEXT("the function does not return a string");
                    RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC);
                }
                ERROR_TEXT("the type of the variable and the value assigned to it do not match");
                RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_EQ);
            }
        }
    }else if(token->token_name == TOKEN_INTEGER){
        GET_TOKEN(token);
        if(token->token_name == TOKEN_PLUS || token->token_name == TOKEN_MINUS || token->token_name == TOKEN_MULTIPLICATION || token->token_name == TOKEN_DIVISION || token->token_name == TOKEN_INT_DIVISION){
            hold_token();
            get_old_token(token);

            AST_leaf *tree;

            tree = precede_expression(token, &ERROR_ALL);
            if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS){
                ERROR_TEXT("invalid expression");
                RETURN_ERROR_NUMBER(ERROR_ALL);
            }
            else if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS_EXPR){
                ERROR_TEXT("incompatible type");
                RETURN_ERROR_NUMBER(ERROR_ALL);
            }
            ERROR_ALL = check_expression(tree,ast_node);
            ast_node->expression->preced_expression_tree = tree;
            ast_node->expression->tree = true;

            if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS){
                ERROR_TEXT("invalid expression");
                RETURN_ERROR_NUMBER(ERROR_ALL);
            }
            else if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS_EXPR){
                ERROR_TEXT("incompatible type");
                RETURN_ERROR_NUMBER(ERROR_ALL);
            }
            //TODO анализа прецеденчни .. вставил
        }else{
            hold_token();
            get_old_token(token);
            if(!string_param_cmp_arr(check_type, ast_node->count_expression, integ)) {
                if (!string_param_cmp_arr(check_type, ast_node->count_expression, numb)) {
                    if(ast_node->it_is_return){
                        ERROR_TEXT("the function does not return a integer");
                        RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC);
                    }

                    ERROR_TEXT("the type of the variable and the value assigned to it do not match");
                    RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_EQ);
                }else{
                    ast_node->expression->numb = true;
                    ast_node->expression->data_double = (double) token->lexeme->integer;
                    ast_node->expression->data = malloc(sizeof(t_str));
                    if(!ast_node->expression->data){
                        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                    }
                    if(string_init(ast_node->expression->data)){
                        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                    }
                    if(string_copy(token->lexeme->inter,ast_node->expression->data)){
                        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                    }
                }
            }else{
                ast_node->expression->integer = true;
                ast_node->expression->data = malloc(sizeof(t_str));
                if(!ast_node->expression->data){
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                if(string_init(ast_node->expression->data)){
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                if(string_copy(token->lexeme->inter,ast_node->expression->data)){
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
            }
        }

    }else if(token->token_name == TOKEN_NUMBER || token->token_name == TOKEN_NUMBER_EXPONENT){
        GET_TOKEN(token);
        if(token->token_name == TOKEN_PLUS || token->token_name == TOKEN_MINUS || token->token_name == TOKEN_MULTIPLICATION || token->token_name == TOKEN_DIVISION || token->token_name == TOKEN_INT_DIVISION){
            hold_token();
            get_old_token(token);

            AST_leaf *tree;

            tree = precede_expression(token, &ERROR_ALL);
            if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS){
                ERROR_TEXT("invalid expression");
                RETURN_ERROR_NUMBER(ERROR_ALL);
            }
            else if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS_EXPR){
                ERROR_TEXT("incompatible type");
                RETURN_ERROR_NUMBER(ERROR_ALL);
            }
            ERROR_ALL = check_expression(tree,ast_node);
            ast_node->expression->preced_expression_tree = tree;
            ast_node->expression->tree = true;

            if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS){
                ERROR_TEXT("invalid expression");
                RETURN_ERROR_NUMBER(ERROR_ALL);
            }
            else if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS_EXPR){
                ERROR_TEXT("incompatible type");
                RETURN_ERROR_NUMBER(ERROR_ALL);
            }
            //TODO анализа прецеденчни .. вставил
        }else{
            hold_token();
            get_old_token(token);
            if(!string_param_cmp_arr(check_type, ast_node->count_expression, numb)) {
                if(ast_node->it_is_return) {
                    ERROR_TEXT("the function does not return a number");
                    RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC);
                }

                ERROR_TEXT("the type of the variable and the value assigned to it do not match");
                RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_EQ);
            }else{
                ast_node->expression->numb = true;
                ast_node->expression->data_double = token->lexeme->number;
                ast_node->expression->data = malloc(sizeof(t_str));
                if(!ast_node->expression->data){
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                if(string_init(ast_node->expression->data)){
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
                if(string_copy(token->lexeme->inter,ast_node->expression->data)){
                    RETURN_ERROR_NUMBER(ERROR_INTERNAL);
                }
            }
        }
    }else if(token->token_name == TOKEN_LENGTH){
//        hold_token();
        AST_leaf *tree;

        tree = precede_expression(token, &ERROR_ALL);
        if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS){
            ERROR_TEXT("invalid expression");
            RETURN_ERROR_NUMBER(ERROR_ALL);
        }
        else if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS_EXPR){
            ERROR_TEXT("incompatible type");
            RETURN_ERROR_NUMBER(ERROR_ALL);
        }
        ERROR_ALL = check_expression(tree,ast_node);
        ast_node->expression->preced_expression_tree = tree;
        ast_node->expression->tree = true;

        if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS){
            ERROR_TEXT("invalid expression");
            RETURN_ERROR_NUMBER(ERROR_ALL);
        }
        else if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS_EXPR){
            ERROR_TEXT("incompatible type");
            RETURN_ERROR_NUMBER(ERROR_ALL);
        }
    }else if(token->token_name == TOKEN_LEFT_BRACKET){
//        hold_token();
        AST_leaf *tree;

        tree = precede_expression(token, &ERROR_ALL);
        if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS){
            ERROR_TEXT("invalid expression");
            RETURN_ERROR_NUMBER(ERROR_ALL);
        }
        else if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS_EXPR){
            ERROR_TEXT("incompatible type");
            RETURN_ERROR_NUMBER(ERROR_ALL);
        }
        ERROR_ALL = check_expression(tree,ast_node);
        ast_node->expression->preced_expression_tree = tree;
        ast_node->expression->tree = true;

        if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS){
            ERROR_TEXT("invalid expression");
            RETURN_ERROR_NUMBER(ERROR_ALL);
        }
        else if(ERROR_ALL == ERROR_SEMANTIC_ANALYSIS_EXPR){
            ERROR_TEXT("incompatible type");
            RETURN_ERROR_NUMBER(ERROR_ALL);
        }
    }else{
        ERROR_TEXT("start of expression expected");
        RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_EQ);
    }

    GET_TOKEN(token);
    if(token->token_name == TOKEN_COMMA){
        if(exp_next()){
            RETURN_ERROR_NUMBER(ERROR_INTERNAL);
        }
        if(value(token)){
            RETURN_ERROR;
        }
    }else
    {
        if((check_count > ast_node->count_expression) && !ast_node->it_is_return)
        {
            ERROR_TEXT("the number of parameters on the sides of the expression do not match");
            RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS_EQ);
        }
        if(ast_node->expression->first_exp && !ast_node->it_is_return){
            ast_node->expression = ast_node->expression->first_exp;
        }
        hold_token();

    }
    return IT_IS_OK;
}

int next_id(t_token *token){ ///проверенна
    GET_TOKEN(token);
    if(token->token_name == TOKEN_COMMA){
        GET_TOKEN(token);
        if(token->token_name == TOKEN_IDENTIFIER){
            node* function_var = NULL;
            ul hash = hashcode(token->lexeme->inter->data);

            bool error_nul = false;
            function_var = check_type_stack(error_nul,hash);
            if(error_nul){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            if (!function_var) {
                ERROR_TEXT("variable was not defined");
                RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
            }

            ast_node->it_is_variable_ = true;
            ast_node->count_variable++;
            if(string_param_copy_string(ast_node->variable,token->lexeme->inter)){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
            //тип
            if(string_param_copy_string(ast_node->type_variable,function_var->data->type)){
                RETURN_ERROR_NUMBER(ERROR_INTERNAL);
            }
        }else{
            ERROR_TEXT("after the comma, a variable was expected");
            RETURN_ERROR_NUMBER(ERROR_SYN_ANALYSIS);
        }

        if(next_id(token)){
            RETURN_ERROR;
        }

    }else{
        hold_token();
        return IT_IS_OK;
    }
    return IT_IS_OK;
}


void ast_init(t_ast_node* ast){
    ast->global = NULL;
    ast->in_function = NULL;
    ast->local = NULL;
    ast->function_info = NULL;

    ast->variable = NULL;
    ast->type_variable = NULL;
    ast->count_variable = 0;

    ast->func = NULL;
//    ast->func_param = NULL;
    ast->count_func_param = 0;



    ast->expression = NULL;
    ast->count_expression = 0;
    ast->count_func_param = 0;

    ast->count_nesting = 0;
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
    ast->it_is_return = false;
    ast->it_is_return_exp = false;
    ast->it_is_start_if_loop = false;
    ast->it_is_function_end = false;
    ast->it_is_loop_or_if = false;
    ast->first_node = NULL;
    ast->next_node = NULL;
}
void ast_free(t_ast_node* ptr){
   t_ast_node* ast = ptr->next_node;


    while(ast){
        if(ast->variable) {
            string_param_free(ast->variable);
        }
        if(ast->type_variable)
            string_param_free(ast->type_variable);

        if(ast->expression) {
            if (ast->expression->first_exp) {
                ast->expression = ast->expression->first_exp;
            }
        }

        while(ast->expression){
            if(ast->expression->data){
                string_free(ast->expression->data);
                ast->expression->data = NULL;
            }
            if(ast->expression->preced_expression_tree){
                delete_ast(ast->expression->preced_expression_tree);
                ast->expression->preced_expression_tree = NULL;
            }
            t_exp_list * p = ast->expression->next_exp;
            free(ast->expression);
            ast->expression = p;
        }

        t_ast_node* p = ast->next_node;
        free(ast);
        ast = p;
    }


    if(ptr->variable) {
        string_param_free(ptr->variable);
    }
    if(ptr->type_variable)
        string_param_free(ptr->type_variable);

    if(ptr->expression) {
        if (ptr->expression->first_exp) {
            ptr->expression = ptr->expression->first_exp;
        }
    }

    while(ptr->expression){
        if(ptr->expression->data){
            string_free(ptr->expression->data);
            ptr->expression->data = NULL;
        }
        if(ptr->expression->preced_expression_tree){
            delete_ast(ptr->expression->preced_expression_tree);
            ptr->expression->preced_expression_tree = NULL;
        }
        t_exp_list * p = ptr->expression->next_exp;
        free(ptr->expression);
        ptr->expression = p;
    }

    ast_init(ptr);
}
int add_table_symbols_system_function(char* name){
    sData *function_var = NULL;
    t_str* str = NULL;
    str = malloc(sizeof (t_str));
    if(!str){
        return ERROR_INTERNAL;
    }
    if(string_init(str)){
        free(str);
        return ERROR_INTERNAL;
    }

    function_var = malloc(sizeof (sData));
    if(!function_var){
        string_free(str);
        return ERROR_INTERNAL;
    }
    s_data_init(function_var);
    function_var->name = malloc(sizeof (t_str));
    if(!function_var->name){
        string_free(str);
        free(function_var);
        return ERROR_INTERNAL;
    }
    if(string_init(function_var->name)){
        string_free(str);
        free(function_var->name);
        free(function_var);
        return ERROR_INTERNAL;
    }
    function_var->type = malloc(sizeof (t_str));
    if(!function_var->type){
        string_free(str);
        string_free(function_var->name);
        free(function_var);
        return ERROR_INTERNAL;
    }
    if(string_init(function_var->type)){
        string_free(str);
        string_free(function_var->name);
        free(function_var->type);
        free(function_var);
        return ERROR_INTERNAL;
    }
    if(string_wright_arr(function_var->name, name)){
        string_free(str);
        string_free(function_var->name);
        string_free(function_var->type);
        free(function_var);
        return ERROR_INTERNAL;
    }
    if(string_wright_arr(function_var->type, func)){
        string_free(str);
        string_free(function_var->name);
        string_free(function_var->type);
        free(function_var);
        return ERROR_INTERNAL;
    }

    function_var->declaration = true;
    function_var->define = true;
    function_var->system_function = true;
    function_var->count_returned_params = 1;


    function_var->type_returned_params = malloc(sizeof (t_str_param));
    if(!function_var->type_returned_params){
        string_free(str);
        string_free(function_var->name);
        string_free(function_var->type);
        free(function_var);
        return ERROR_INTERNAL;
    }
    if(string_param_init(function_var->type_returned_params)){
        string_free(str);
        free(function_var->type_returned_params);
        string_free(function_var->name);
        string_free(function_var->type);
        free(function_var);
        return ERROR_INTERNAL;
    }
    if(string_wright_arr(str,name)){
        string_free(str);
        string_param_free(function_var->type_returned_params);
        string_free(function_var->name);
        string_free(function_var->type);
        free(function_var);
        return ERROR_INTERNAL;
    }

    if(string_arr_cmp(str,"write")){
        function_var->count_returned_params = 0;
        function_var->system_function_infinity_param = true;
        string_param_free(function_var->type_returned_params);
        function_var->type_returned_params = NULL;
    }else if(string_arr_cmp(str,"reads")){
        if(string_wright_arr(str, strin)){
            string_free(str);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
    }else if(string_arr_cmp(str,"readi")){
        if(string_wright_arr(str, integ)){
            string_free(str);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
    }else if(string_arr_cmp(str,"readn")){
        if(string_wright_arr(str, numb)){
            string_free(str);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
    }else if(string_arr_cmp(str,"tointeger")){
        function_var->type_params = malloc(sizeof (t_str_param));
        if(!function_var->type_params){
            string_free(str);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
        if(string_param_init(function_var->type_params)){
            string_free(str);
            free(function_var->type_params);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
        if(string_wright_arr(str,numb)){
            string_free(str);
            string_param_free(function_var->type_params);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
        if(string_param_copy_string(function_var->type_params, str)){
            string_free(str);
            string_param_free(function_var->type_params);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
        function_var->count_params = 1;

        if(string_wright_arr(str, integ)){
            string_free(str);
            string_param_free(function_var->type_params);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
    }else if(string_arr_cmp(str,"substr")){
        function_var->type_params = malloc(sizeof (t_str_param));
        if(!function_var->type_params){
            string_free(str);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
        if(string_param_init(function_var->type_params)){
            string_free(str);
            free(function_var->type_params);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
        if(string_wright_arr(str, strin)){
            string_free(str);
            string_param_free(function_var->type_params);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
        if(string_param_copy_string(function_var->type_params, str)){
            string_free(str);
            string_param_free(function_var->type_params);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
        if(string_wright_arr(str, numb)){
            string_free(str);
            string_param_free(function_var->type_params);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
        if(string_param_copy_string(function_var->type_params, str)){
            string_free(str);
            string_param_free(function_var->type_params);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
        if(string_param_copy_string(function_var->type_params, str)){
            string_free(str);
            string_param_free(function_var->type_params);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
        function_var->count_params = 3;

        if(string_wright_arr(str, strin)){
            string_free(str);
            string_param_free(function_var->type_params);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
    }else if(string_arr_cmp(str,"ord")){
        function_var->type_params = malloc(sizeof (t_str_param));
        if(!function_var->type_params){
            string_free(str);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
        if(string_param_init(function_var->type_params)){
            string_free(str);
            free(function_var->type_params);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
        if(string_wright_arr(str, strin)){
            string_free(str);
            string_param_free(function_var->type_params);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
        if(string_param_copy_string(function_var->type_params, str)){
            string_free(str);
            string_param_free(function_var->type_params);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
        if(string_wright_arr(str, integ)){
            string_free(str);
            string_param_free(function_var->type_params);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
        if(string_param_copy_string(function_var->type_params, str)){
            string_free(str);
            string_param_free(function_var->type_params);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
        function_var->count_params = 2;

        if(string_wright_arr(str, integ)){
            string_free(str);
            string_param_free(function_var->type_params);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }

    }else if(string_arr_cmp(str,"chr")){
        function_var->type_params = malloc(sizeof (t_str_param));
        if(!function_var->type_params){
            string_free(str);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
        if(string_param_init(function_var->type_params)){
            string_free(str);
            free(function_var->type_params);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
        if(string_wright_arr(str, integ)){
            string_free(str);
            string_param_free(function_var->type_params);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
        if(string_param_copy_string(function_var->type_params, str)){
            string_free(str);
            string_param_free(function_var->type_params);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
        function_var->count_params = 1;

        if(string_wright_arr(str, strin)){
            string_free(str);
            string_param_free(function_var->type_params);
            string_param_free(function_var->type_returned_params);
            string_free(function_var->name);
            string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
    }
    if(function_var->type_returned_params) {
        if (string_param_copy_string(function_var->type_returned_params, str)) {
            string_free(str);
            if (function_var->type_params)
                string_param_free(function_var->type_params);
            if (function_var->type_returned_params)
                string_param_free(function_var->type_returned_params);
            if (function_var->name)
                string_free(function_var->name);
            if (function_var->type)
                string_free(function_var->type);
            free(function_var);
            return ERROR_INTERNAL;
        }
    }
    global_table = tree_insert(global_table, hashcode(function_var->name->data),function_var);
    if(!global_table){
        string_free(str);
        if(function_var->type_params)
            string_param_free(function_var->type_params);
        if(function_var->type_returned_params)
            string_param_free(function_var->type_returned_params);
        if(function_var->name)
            string_free(function_var->name);
        if(function_var->type)
            string_free(function_var->type);
        free(function_var);
        return ERROR_INTERNAL;
    }

    string_free(str);
    return IT_IS_OK;
}

int send_ast(){
    if(ast_node->expression){
        if(ast_node->expression->first_exp) {
            ast_node->expression = ast_node->expression->first_exp;
        }
    }
    if(code_generation(ast_node)){
        RETURN_ERROR;
    }
    if(ast_node->it_is_in_function || ast_node->it_is_function_define){
        sData* fn = ast_node->function_info;
        node* gl = ast_node->global;
        node* in_f = ast_node->in_function;
        ast_free(ast_node);
        ast_init(ast_node);
        ast_node->it_is_in_function = true;
        ast_node->global = gl;
        ast_node->in_function = in_f;
        ast_node->function_info = fn;
    }else{
        ast_free(ast_node);
        ast_init(ast_node);
    }
    return IT_IS_OK;
}

void exp_init(t_exp_list* exp){
    exp->tree = false;
    exp->var = false;
    exp->preced_expression_tree = NULL;
    exp->data = NULL;


    exp->str = false;
    exp->integer = false;
    exp->numb =false;
    exp->nil = false;
    exp->data_double = 0.0;


    exp->first_exp = NULL;
    exp->next_exp = NULL;

}
int if_loop_ast_next(){
    ast_node->next_node = malloc(sizeof(t_ast_node));
    if(!ast_node->next_node){
        return ERROR_INTERNAL;
    }
    ast_init(ast_node->next_node);
    t_ast_node* ptr = ast_node->next_node;
//    ast_init(ptr);
    if(!ast_node->first_node){
        ast_node->first_node = ast_node;
    }
    ptr->first_node = ast_node->first_node;
    ptr->count_nesting = ast_node->count_nesting;
    ptr->it_is_loop = false; // ast_node->it_is_loop;
    ptr->it_is_if = false; //ast_node->it_is_if;
    ptr->it_is_in_function = ast_node->it_is_in_function;
    ptr->global = ast_node->global;
    ptr->function_info = ast_node->function_info;
    ptr->in_function = ast_node->in_function;
    ptr->local = ast_node->local;
    ptr->it_is_loop_or_if = true;

    ast_node = ast_node->next_node;
    return IT_IS_OK;
}

int exp_next(){
    if(!ast_node->expression->first_exp){
        ast_node->expression->first_exp = ast_node->expression;
    }
    //пересобираем для следующего вырожения
    ast_node->expression->next_exp = malloc(sizeof (t_exp_list));
    if(!ast_node->expression->next_exp){
        return ERROR_INTERNAL;
    }
    t_exp_list* ptr = ast_node->expression->next_exp;
    exp_init(ptr);
    ptr->first_exp = ast_node->expression->first_exp;
    ast_node->expression = ast_node->expression->next_exp;
    return IT_IS_OK;
}

void error_processing(){
//    free all
    if(global_table){
        global_table = tree_delete(global_table);
        global_table = NULL;
    }

    if(stack_table){
        node* del_table = NULL;
        del_table = table_pop(stack_table);
        while(del_table){
            del_table = tree_delete(del_table);
//            if(del_table)
                del_table = table_pop(stack_table);
        }
        table_delete(stack_table);
        free(stack_table);
        stack_table = NULL;
    }

    if(ast_node){
        if(ast_node->first_node){
            ast_node = ast_node->first_node;
        }
        ast_free(ast_node);
        free(ast_node);
        ast_node = NULL;
    }
}


int fake_variable(){
    sData* fake_var = malloc(sizeof (sData));
    if(!fake_var){
        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
    }
    s_data_init(fake_var);
    fake_var->name = malloc(sizeof (t_str));
    if(!fake_var->name){
        free(fake_var);
        fake_var = NULL;
        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
    }
    if(string_init(fake_var->name)){
        free(fake_var->name);
        free(fake_var);
        fake_var = NULL;
        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
    }
    if(string_wright_char(fake_var->name, '$')){
        string_free(fake_var->name);
        free(fake_var);
        fake_var = NULL;
        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
    }

    fake_var->type = malloc(sizeof (t_str));
    if(!fake_var->type){
        free(fake_var);
        fake_var = NULL;
        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
    }
    if(string_init(fake_var->type)){
        free(fake_var->type);
        free(fake_var);
        fake_var = NULL;
        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
    }
    if(string_wright_char(fake_var->type, '$')){
        string_free(fake_var->type);
        free(fake_var);
        fake_var = NULL;
        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
    }

    node* fake_table = NULL;
    fake_table = tree_insert(fake_table, hashcode(fake_var->name->data), fake_var);
    if (table_push(stack_table,fake_table)){
        fake_table = tree_delete(fake_table);
        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
    }
    return IT_IS_OK;
}


node* check_type_stack(bool error_null, ul hash){
    node* function_var = NULL;
    int count = 0;
    s_stack* save_table = malloc(sizeof (s_stack));
    if(!save_table){
        error_null = true;
        return NULL;
    }
    table_init(save_table);
    node* check_type = NULL;
    while(1){
        check_type = table_pop(stack_table);
        count++;
        if(table_push(save_table,check_type)){
            error_null = true;
            check_type = tree_delete(check_type);
            check_type = NULL;
            if(save_table){
                node* del_table = NULL;
                del_table = table_pop(save_table);
                while(del_table){
                    del_table = tree_delete(del_table);
//                    if(del_table)
                        del_table = table_pop(save_table);
                }
                table_delete(save_table);
                free(save_table);
                save_table = NULL;
                return NULL;
            }
        }
        function_var = tree_search(check_type, hash);
        if(function_var){
            break;
        }
        if(check_type == ast_node->in_function){
            break;
        }
    }
    for(int i = 0; i < count; i++){
        check_type = table_pop(save_table);
        if(table_push(stack_table,check_type)){
            error_null = true;
            check_type = tree_delete(check_type);
            check_type = NULL;
            if(save_table){
                node* del_table = NULL;
                del_table = table_pop(save_table);
                while(del_table){
                    del_table = tree_delete(del_table);
//                    if(del_table)
                        del_table = table_pop(save_table);
                }
                table_delete(save_table);
                free(save_table);
                save_table = NULL;
                return NULL;
            }
        }
    }

    table_delete(save_table);
    free(save_table);
    if(error_null)
        return NULL;

    return function_var;
}


int tree_check(node *root,t_token* token){
    if(root == NULL)
        return IT_IS_OK;

    if(!ERROR_ALL)
        tree_check(root->right_node,token);
    if(!ERROR_ALL)
        tree_check(root->left_node,token);

    if(root->data && !ERROR_ALL){
        if(root->data->declaration && !ERROR_ALL){
            if(!root->data->define && !ERROR_ALL){
                ERROR_TEXT("the function was declared but was not defined in the program");
                RETURN_ERROR_NUMBER(ERROR_SEMANTIC_ANALYSIS);
            }
        }
    }

    return ERROR_ALL;
}

int start_analysis(t_token *token){

    ast_node = malloc(sizeof (t_ast_node));
    if(!ast_node){
        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
    }
    ast_init(ast_node);
    stack_table = malloc(sizeof (s_stack));
    if(!stack_table){
        free(ast_node);
        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
    }
    table_init(stack_table);

    if(add_table_symbols_system_function("reads")){
        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
    }
    if(add_table_symbols_system_function("readi")){
        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
    }
    if(add_table_symbols_system_function("readn")){
        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
    }
    if(add_table_symbols_system_function("write")){
        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
    }
    if(add_table_symbols_system_function("tointeger")){
        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
    }
    if(add_table_symbols_system_function("substr")){
        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
    }
    if(add_table_symbols_system_function("ord")){
        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
    }
    if(add_table_symbols_system_function("chr")){
        RETURN_ERROR_NUMBER(ERROR_INTERNAL);
    }

    start_generation();
    int global_error = 0;
    global_error = start_program(token);
    if(global_error){
        return global_error;
    }
    global_error = tree_check(global_table,token);
    if(global_error){
        return global_error;
    }
    global_error = code_assemble();
    if(global_error){
        return global_error;
    }
    error_processing();
    return global_error;

}
