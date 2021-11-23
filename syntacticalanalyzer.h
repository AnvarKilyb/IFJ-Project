#ifndef IFJ_PROJEKT_SYNTACTICALANALYZER_H
#define IFJ_PROJEKT_SYNTACTICALANALYZER_H

#define GET_TOKEN(token) if(get_token(token)) return ERROR_LEX_ANALYSIS

//typedef struct s_node{
//    // TODO Какаяниюудь стабулка ссимволов где искть
//
//    sData head;
//    sData params;
//    sData body;
//
//    bool it_is_loop;
//    bool is_is_declaration;
//    bool it_is_if;
//    bool it_is_def_function;
//    bool it_is_function;
//}t_node;
//

int chunk(t_token *token);
int function(t_token *token);
int params(t_token *token);
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
