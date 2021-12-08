/**
 * Project: Implementace překladače imperativního jazyka IFJ21
 *
 * File:     precedence_sa.h
 * Subject:  IFJ2021
 *
 * @author:  Vladislav Mikheda  	xmikhe00
 * @author:  Khrisanov Vladislav    xkhris00
 * @author:  Kilybayev Anvar        xkilyb00
 * @author:  Gazizov Zhasdauren     xgaziz00
 */


#ifndef IFJ_PROJEKT_PRECEDENCETABLE_H
#define IFJ_PROJEKT_PRECEDENCETABLE_H

#include "syntacticalanalyzer.h"

//typedef enum
//{
//    PREC_PLUS,			// +
//    PREC_MINUS,			// -
//    PREC_MUL,			// *
//    PREC_DIV,			// /
//    PREC_IDIV,			// //
//    PREC_LENGTH,		// #
//    PREC_CON,			// ..
//    PREC_EQ,			// ==
//    PREC_NEQ,			// ~=
//    PREC_LEQ,			// <=
//    PREC_LTN,			// <
//    PREC_MEQ,			// >=
//    PREC_MTN,			// >
//    PREC_LEFT_BRACKET,	// (
//    PREC_RIGHT_BRACKET,	// )
//    PREC_IDENTIFIER,	// ID
//    PREC_INT_NUMBER,	// int
//    PREC_DOUBLE_NUMBER,	// double
//    PREC_STRING,		// string
//    PREC_DOLLAR,		// $
//    PREC_STOP,			// stop symbol used when reducing
//    PREC_NON_TERM		// non-terminal
//}prec_symbol;
//
//typedef enum{
//    I_LENGTH,
//    I_MUL_DIV,
//    I_IDIV,
//    I_PLUS_MINUS,
//    I_CON,
//    I_LTN,
//    I_MTN,
//    I_LEQ,
//    I_MEQ,
//    I_EQ,
//    I_NEQ,
//    I_LEFT_BRACKET,
//    I_RIGHT_BRACKET,
//    I_DATA,
//    I_DOLLAR
//}prec_index;
//
//typedef struct A_leaf{
//    // bool processed;
//    t_token *token;
//    struct A_leaf *left;
//    struct A_leaf *right;
//}AST_leaf;
//
//typedef struct s_stack_unit{
//    // e_rule data;
//    prec_symbol symbol;
//    AST_leaf *root;
//    struct s_stack_unit* down_element;
//}t_stack_element;
//
//typedef struct{
//    t_stack_element* stack_top;
//    int amount_of_elements;
//}t_stack;

int stack_init(t_stack* stack);
int stack_push(t_stack* stack, AST_leaf *root, prec_symbol symbol);
t_stack_element* stack_top(t_stack *stack);
int stack_pop(t_stack* stack);
int stack_free(t_stack* stack);

AST_leaf *create_leaf(t_token *token);
AST_leaf *create_tree(AST_leaf *leaf_1, AST_leaf *leaf_2, t_token *token);
void delete_ast(AST_leaf *leaf);
AST_leaf *convert_id_to_nil(AST_leaf *tree);
#define TABLE_SIZE 15
//static prec_index get_prec_index(prec_symbol symbol);
//static prec_symbol get_symbol_from_token(t_token *token);
int find_operation(t_token *token, t_stack *stack);
AST_leaf *reduce_by_rule(t_stack *tmp_stack, t_stack *stack, AST_leaf *tree, e_error_message *e_check);
AST_leaf *get_expression(t_token *token, t_stack *stack, AST_leaf *tree, e_error_message *e_check);
AST_leaf *precede_expression(t_token *token, e_error_message *e_check);
int check_expression(AST_leaf *tree, t_ast_node *ast_node);
#endif
