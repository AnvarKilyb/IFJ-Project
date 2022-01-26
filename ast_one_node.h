/**
 * Project: Implementace překladače imperativního jazyka IFJ21
 *
 * File:     ast_one_node.h
 * Subject:  IFJ2021
 *
 * @author:  Vladislav Mikheda  	xmikhe00
 * @author:  Khrisanov Vladislav    xkhris00
 * @author:  Kilybayev Anvar        xkilyb00
 * @author:  Gazizov Zhasdauren     xgaziz00
 */

#ifndef IFJ_PROJEKT_AST_ONE_NODE_H
#define IFJ_PROJEKT_AST_ONE_NODE_H
#include "stdbool.h"
#include "precedence_tree.h"
#include "string.h"
#include "symtable.h"
typedef unsigned long ul;

typedef struct s_exp_list{

    bool tree;
    bool var;
    AST_leaf* preced_expression_tree;

//    t_str* variable;
    t_str* data;


    bool str;
    bool integer;
    bool numb;
    bool nil;
//    int data_int;
    double data_double;
//    t_str* data_string;

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


    t_exp_list* expression;
    int count_expression;
    int count_func_param;

    int count_nesting;
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
    bool it_is_loop_or_if;
    struct s_ast_node *first_node;
    struct s_ast_node *next_node;
}t_ast_node;
#endif //IFJ_PROJEKT_AST_ONE_NODE_H
