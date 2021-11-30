#ifndef IFJ_PROJEKT_SYNTACTICALANALYZER_H
#define IFJ_PROJEKT_SYNTACTICALANALYZER_H

#define GET_TOKEN(token) if(get_token(token)) return ERROR_LEX_ANALYSIS
#include "symbol_table.h"
#include "lexicalanalysis.h"
#include "string_param.h"

typedef unsigned long ul;

typedef struct s_ast_node{
    node* global;
    node* in_function;
    node* local;

    t_str_param* variable;
    t_str_param* type_variable;
    int count_variable;

    sData* func;
    t_str_param* func_param;
    int count_func_param;
    t_str_param* return_param;
    int count_return_param;


    //что то там что то там expression
    bool it_is_variable_;
    bool it_is_variable_expression;
    bool it_is_variable_call_function;
    bool it_is_declaration_variable;
    bool it_is_loop;
    bool it_is_if;
    bool it_is_function_define;
    bool it_is_call_function;
    bool if_loop_end;
    bool if_else;
    bool it_is_in_function;
    struct s_ast_node *active_node;
    struct s_ast_node *next_node;
}t_ast_node;


int chunk(t_token *token);
int function(t_token *token);
int params(t_token *token, ul hash);
int return_types(t_token *token, ul hash);
int statement(t_token *token);
int function_call(t_token *token);
int args(t_token *token);
int data_type(t_token *token, node* function_node, bool ret_param);
int next_data_type(t_token *token,node* function_node, bool  ret_param);
int next_param(t_token *token, node* function_node, bool ret_param);
int value(t_token *token);
int expression(t_token *token);
int next_expression(t_token *token);
int next_id(t_token *token);
int global_params(t_token *token, ul hash);
void ast_init(t_ast_node* ast);
void ast_free(t_ast_node* ast);
int next_args(t_token* token);
void send_ast();

/*
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 *
 * */


#endif //IFJ_PROJEKT_SYNTACTICALANALYZER_H
