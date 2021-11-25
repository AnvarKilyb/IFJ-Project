#ifndef IFJ_PROJEKT_SYNTACTICALANALYZER_H
#define IFJ_PROJEKT_SYNTACTICALANALYZER_H

#define GET_TOKEN(token) if(get_token(token)) return ERROR_LEX_ANALYSIS
#include "symbol_table.h"
#include "lexicalanalysis.h"
typedef unsigned long ul;
typedef struct s_node_expression{
    bool expression_variable_function;
    bool call_function;
    bool expression;

    // что то там exp
    t_str *variable;
    sData function;

}t_node_expression;

typedef struct s_ast_node{
    node global;
    node in_function;
    node local;

    sData body;
    //что то там что то там expression
    t_node_expression expression;
    bool it_is_expression_node;
    bool it_is_loop;
    bool it_is_if;
    bool it_is_declaration_variable;
    bool it_is_function;
    bool it_is_in_function;
    struct s_ast_node *next_node;
}t_ast_node;


int chunk(t_token *token);
int function(t_token *token);
int params(t_token *token, ul hash);
int return_types(t_token *token);
int statement(t_token *token);
int function_call(t_token *token);
int args(t_token *token);
int data_type(t_token *token);
int next_data_type(t_token *token);
int next_param(t_token *token);
int value(t_token *token);
int expression(t_token *token);
int new_expression(t_token *token);
int next_id(t_token *token);




#endif //IFJ_PROJEKT_SYNTACTICALANALYZER_H
