/**
 * Project: Implementace překladače imperativního jazyka IFJ21
 *
 * File:     codgen.h
 * Subject:  IFJ2021
 *
 * @author:  Vladislav Mikheda  	xmikhe00
 * @author:  Khrisanov Vladislav    xkhris00
 * @author:  Kilybayev Anvar        xkilyb00
 * @author:  Gazizov Zhasdauren     xgaziz00
 */



#ifndef IFJ_PROJEKT_CODGEN_H
#define IFJ_PROJEKT_CODGEN_H

#include "ast_one_node.h"
#include "precedence_tree.h"
#include "syntacticalanalyzer.h"
//#include "symbol_table.h"
//#include "string_param.h"
//#include "string.h"
//#include "error.h"
//#include <stdlib.h>
//#include "lexicalanalysis.h"


int code_assemble();
int start_generation();
int code_generation(t_ast_node* ast_tree);
//int code_expression_node(t_ast_node *ast_tree);
//int code_loop(t_ast_node *ast_tree);
//int code_if(t_ast_node *ast_tree);
//int code_declaration_variable(t_ast_node *ast_tree);

//int code_function(t_ast_node *ast_tree);


#endif //IFJ_PROJEKT_CODGEN_H
