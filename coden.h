#ifndef IFJ_PROJEKT_CODEN_H
#define IFJ_PROJEKT_CODEN_H

#include "ast_one_node.h"
#include "precefence_tree.h"
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


#endif //IFJ_PROJEKT_CODEN_H
