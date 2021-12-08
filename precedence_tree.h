/**
 * Project: Implementace překladače imperativního jazyka IFJ21
 *
 * File:     precedence_tree.h
 * Subject:  IFJ2021
 *
 * @author:  Vladislav Mikheda  	xmikhe00
 * @author:  Khrisanov Vladislav    xkhris00
 * @author:  Kilybayev Anvar        xkilyb00
 * @author:  Gazizov Zhasdauren     xgaziz00
 */




#ifndef IFJ_PROJEKT_PRECEDENCE_TREE_H
#define IFJ_PROJEKT_PRECEDENCE_TREE_H
#include "lexicalanalysis.h"
typedef enum
{
    PREC_PLUS,			// +
    PREC_MINUS,			// -
    PREC_MUL,			// *
    PREC_DIV,			// /
    PREC_IDIV,			// //
    PREC_LENGTH,		// #
    PREC_CON,			// ..
    PREC_EQ,			// ==
    PREC_NEQ,			// ~=
    PREC_LEQ,			// <=
    PREC_LTN,			// <
    PREC_MEQ,			// >=
    PREC_MTN,			// >
    PREC_LEFT_BRACKET,	// (
    PREC_RIGHT_BRACKET,	// )
    PREC_IDENTIFIER,	// ID
    PREC_INT_NUMBER,	// int
    PREC_DOUBLE_NUMBER,	// double
    PREC_STRING,		// string
    PREC_DOLLAR,		// $
    PREC_STOP,			// stop symbol used when reducing
    PREC_NON_TERM		// non-terminal
}prec_symbol;

typedef enum{
    I_LENGTH,
    I_MUL_DIV,
    I_IDIV,
    I_PLUS_MINUS,
    I_CON,
    I_LTN,
    I_MTN,
    I_LEQ,
    I_MEQ,
    I_EQ,
    I_NEQ,
    I_LEFT_BRACKET,
    I_RIGHT_BRACKET,
    I_DATA,
    I_DOLLAR
}prec_index;

typedef struct A_leaf{
    // bool processed;
    t_token *token;
    struct A_leaf *left;
    struct A_leaf *right;
}AST_leaf;

typedef struct s_stack_unit{
    // e_rule data;
    prec_symbol symbol;
    AST_leaf *root;
    struct s_stack_unit* down_element;
}t_stack_element;

typedef struct{
    t_stack_element* stack_top;
    int amount_of_elements;
}t_stack;

#endif //IFJ_PROJEKT_PRECEDENCE_TREE_H
