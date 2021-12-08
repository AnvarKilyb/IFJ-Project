/**
 * Project: Implementace překladače imperativního jazyka IFJ21
 *
 * File:     syntacticlanalyzer.h
 * Subject:  IFJ2021
 *
 * @author:  Vladislav Mikheda  	xmikhe00
 * @author:  Khrisanov Vladislav    xkhris00
 * @author:  Kilybayev Anvar        xkilyb00
 * @author:  Gazizov Zhasdauren     xgaziz00
 */

#ifndef IFJ_PROJEKT_SYNTACTICALANALYZER_H
#define IFJ_PROJEKT_SYNTACTICALANALYZER_H

#define GET_TOKEN(token) ERROR_ALL = get_token(token);if(ERROR_ALL) {fprintf(stderr,"ERROR: in string #%d [lexical error]", token->str);error_check = true;} if(ERROR_ALL) return ERROR_ALL

#define RETURN_ERROR_NUMBER(ERR) ERROR_ALL = ERR; error_processing(); return ERROR_ALL
#define RETURN_ERROR error_processing(); return ERROR_ALL
#define ERROR_TEXT(string_text) if(!error_check) {fprintf(stderr, "ERROR: in string #%d ", token->str); fprintf(stderr,"[in "); fprintf(stderr,"--> %s <-- ",token->lexeme->inter->data); fprintf(stderr,"%s]",string_text);} error_check = true
#include "symtable.h"
#include "lexicalanalysis.h"
#include "string_param.h"
#include "precedence_tree.h"
#include "codgen.h"
#include "ast_one_node.h"
//typedef unsigned long ul;
//
//typedef struct s_exp_list{
//
//    bool tree;
//    bool var;
//    AST_leaf* preced_expression_tree;
//
////    t_str* variable;
//    t_str* data;
//
//
//    bool str;
//    bool integer;
//    bool numb;
//    bool nil;
////    int data_int;
////    double data_double;
////    t_str* data_string;
//
//    struct s_exp_list* first_exp;
//    struct s_exp_list* next_exp;
//}t_exp_list;
//
//typedef struct s_ast_node{
//    node* global;
//    node* in_function;
//    node* local;
//    sData* function_info;
//
//    t_str_param* variable;
//    t_str_param* type_variable;
//    int count_variable;
//
//    sData* func;
////    t_str_param* func_param;
//
//
//    t_exp_list* expression;
//    int count_expression;
//    int count_func_param;
//
//    int count_nesting;
//    bool it_is_variable_;
//    bool it_is_variable_expression;
//    bool it_is_variable_call_function;
//    bool it_is_declaration_variable;
//    bool it_is_loop;
//    bool it_is_if;
//    bool it_is_function_define;
//    bool it_is_call_function;
//    bool if_loop_end;
//    bool if_else;
//    bool it_is_in_function;
//    bool it_is_return;
//    bool it_is_return_exp;
//    bool it_is_start_if_loop;
//    bool it_is_function_end;
//    struct s_ast_node *first_node;
//    struct s_ast_node *next_node;
//}t_ast_node;


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
//int expression(t_token *token);
//int next_expression(t_token *token);
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
int fake_variable();
node* check_type_stack(bool error_null, ul hash);
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
