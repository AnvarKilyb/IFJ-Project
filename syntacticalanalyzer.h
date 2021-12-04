#ifndef IFJ_PROJEKT_SYNTACTICALANALYZER_H
#define IFJ_PROJEKT_SYNTACTICALANALYZER_H

#define GET_TOKEN(token) if(get_token(token)) return ERROR_LEX_ANALYSIS

#define RETURN_ERROR_NUMBER(ERR) ERROR_ALL = ERR; error_processing(); return ERROR_ALL
#define RETURN_ERROR  return ERROR_ALL
#define ERROR_TEXT(string_text) if(!error_check) {printf("ERROR: in string #%d ", token->str); printf("[in "); printf("--> %s <-- ",token->lexeme->inter->data); printf("%s]",string_text);} error_check = true
#include "symbol_table.h"
#include "lexicalanalysis.h"
#include "string_param.h"
#include "precefence_tree.h"

typedef unsigned long ul;

typedef struct s_exp_list{

    bool tree;
    bool var;
    AST_leaf* preced_expression_tree;

    t_str* variable;


    bool str;
    bool integer;
    bool numb;
    bool nil;
    int data_int;
    double data_double;
    t_str* data_string;

    struct s_exp_list* first_exp;
    struct s_exp_list* next_exp;
}t_exp_list;

typedef struct s_ast_node{
    node* global;
    node* in_function;
    node* local;
    sData* function_info;

    t_str_param* variable;
    t_str_param* type_variable;
    int count_variable;

    sData* func;
//    t_str_param* func_param;
    int count_func_param;


    t_exp_list* expression;
    int count_expression;

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
    bool it_is_return;
    bool it_is_return_exp;
    bool it_is_start_if_loop;
    bool it_is_function_end;
    struct s_ast_node *first_node;
    struct s_ast_node *next_node;
}t_ast_node;


int start_analysis(t_token* token);
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
int send_ast();
void exp_init(t_exp_list* exp);
int exp_next();
int if_loop_ast_next();
int return_exp(t_token* token);
void error_processing();
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
