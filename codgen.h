#ifndef IFJ_PROJEKT_CODGEN_H
#define IFJ_PROJEKT_CODGEN_H

#include "syntacticalanalyzer.h"
#include "symbol_table.h"

//#include "lexicalanalysis.h"


int code_header();
int code_expression_node(t_ast_node *ast_tree);
int code_loop(t_ast_node *ast_tree);
int code_if(t_ast_node *ast_tree);
int code_declaration_variable(t_ast_node *ast_tree);

int code_function(t_ast_node *ast_tree);
int code_gen(t_ast_node *ast_tree);



#endif //IFJ_PROJEKT_CODGEN_H
