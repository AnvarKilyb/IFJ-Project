/**
 * Project: Implementace překladače imperativního jazyka IFJ21
 *
 * File:     precedence_sa.c
 * Subject:  IFJ2021
 *
 * @author:  Vladislav Mikheda  	xmikhe00
 * @author:  Khrisanov Vladislav    xkhris00
 * @author:  Kilybayev Anvar        xkilyb00
 * @author:  Gazizov Zhasdauren     xgaziz00
 */


#include <stdio.h>
#include <stdlib.h>

#include "precedence_sa.h"
AST_leaf *init_leaf(){
    AST_leaf *new_leaf = (AST_leaf *) malloc(sizeof(struct A_leaf));
    if(new_leaf == NULL)
        return NULL;
    new_leaf->token = NULL;
    new_leaf->left = NULL;
    new_leaf->right = NULL;
    return new_leaf;
}
AST_leaf *create_leaf(t_token *token){
    AST_leaf *new_leaf = (AST_leaf *) malloc(sizeof(struct A_leaf));
    if(new_leaf == NULL){
        return NULL;
    }
    new_leaf->token = malloc(sizeof (t_token));
    new_leaf->token->lexeme = malloc((sizeof (t_lexeme)));
    new_leaf->token->lexeme->inter = malloc(sizeof (t_str));
    new_leaf->token->str = token->str;
    string_init(new_leaf->token->lexeme->inter);

    new_leaf->token->token_name = token->token_name;
    new_leaf->token->lexeme->keyword = token->lexeme->keyword;
    new_leaf->token->lexeme->integer = token->lexeme->integer;
    new_leaf->token->lexeme->number = token->lexeme->number;
    string_copy(token->lexeme->inter, new_leaf->token->lexeme->inter);
    new_leaf->left = NULL;
    new_leaf->right = NULL;

    return new_leaf;
}

AST_leaf *create_tree(AST_leaf *leaf_1, AST_leaf *leaf_2, t_token *token){
    AST_leaf *new_leaf = (AST_leaf *) malloc(sizeof(struct A_leaf));
    if(new_leaf == NULL){
        return NULL;
    }
    new_leaf->token = malloc(sizeof (t_token));
    new_leaf->token->lexeme = malloc((sizeof (t_lexeme)));
    new_leaf->token->lexeme->inter = malloc(sizeof (t_str));
    new_leaf->token->str = token->str;
    string_init(new_leaf->token->lexeme->inter);

    new_leaf->token->token_name = token->token_name;
    new_leaf->token->lexeme->keyword = token->lexeme->keyword;
    new_leaf->token->lexeme->integer = token->lexeme->integer;
    new_leaf->token->lexeme->number = token->lexeme->number;
    string_copy(token->lexeme->inter, new_leaf->token->lexeme->inter);
    new_leaf->left = leaf_1;
    new_leaf->right = leaf_2;
    return new_leaf;
}

//void token_ast_free(t_token* token){
//    if(token){
//        if(token->lexeme){
//            if (token->lexeme->inter) {
//                string_free(token->lexeme->inter);
//            }
//            free(token->lexeme);
//            token->lexeme = NULL;
//            free(token);
//            token = NULL;
//        }
//    }
//}

void delete_ast(AST_leaf *leaf){
    if(leaf != NULL){
//        while(leaf != NULL){
            delete_ast(leaf->left);
            delete_ast(leaf->right);

            if(leaf->token){
                if(leaf->token->lexeme){
                    if (leaf->token->lexeme->inter) {
                        string_free(leaf->token->lexeme->inter);
                    }
                    free(leaf->token->lexeme);
                    leaf->token->lexeme = NULL;
                    free(leaf->token);
                    leaf->token = NULL;
                }
            }
//            free(leaf->token);
            leaf->token = NULL;
            free(leaf);
            leaf = NULL;
//        }
    }

}
#define STACK_IS_EMPTY 0
int stack_init(t_stack* stack){
    if(!stack)
        return ERROR_INTERNAL;

    stack->stack_top = NULL;
    stack->amount_of_elements = STACK_IS_EMPTY;
    return IT_IS_OK;
}


static int stack_is_empty(t_stack* stack){
    return stack->amount_of_elements == 0;
}

int stack_push(t_stack* stack, AST_leaf *root, prec_symbol symbol){
    if(!stack)
        return ERROR_INTERNAL;

    t_stack_element *ptr = (t_stack_element *) malloc(sizeof(t_stack_element));
    if (!ptr)
        return ERROR_INTERNAL;

    ptr->symbol = symbol;
    ptr->root =  root;
    ptr->down_element = stack->stack_top;

    stack->stack_top = ptr;
    stack->amount_of_elements++;

    return IT_IS_OK;
}


t_stack_element* stack_top(t_stack *stack){
    if(!stack)
        return NULL;
    return stack->stack_top;
}

int stack_pop(t_stack* stack){
    if(!stack)
        return ERROR_INTERNAL;
    if(stack_is_empty(stack))
        return ERROR_INTERNAL;

    t_stack_element* ptr = stack->stack_top;
    stack->stack_top = stack->stack_top->down_element;
    stack->amount_of_elements--;
    free(ptr);
    return IT_IS_OK;
}

int stack_free(t_stack* stack){
    if(!stack)
        return IT_IS_OK;
    while(stack->amount_of_elements != 0){
        stack_pop(stack);
    }
    return IT_IS_OK;
}

//     1 <- SHIFT <
//     0 <- EQUAL =
//     2 <- REDUCE >
//    -1 <- ERROR

static int prec_table[][TABLE_SIZE] = {
//	| # | */|// |+- |.. | < | > |<= |=> |== |~= | ( | ) | i | $ |
        { 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 1 , 2 , 1 , 2 }, // #
        { 1 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 1 , 2 , 1 , 2 }, // */
        { 1 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 1 , 2 , 1 , 2 }, // //
        { 1 , 1 , 1 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 1 , 2 , 1 , 2 }, // +-
        { 1 , 1 , 1 , 1 , 1 , 2 , 2 , 2 , 2 , 2 , 2 , 1 , 2 , 1 , 2 }, // ..
        { 1 , 1 , 1 , 1 , 1 , 2 , 2 , 2 , 2 , 2 , 2 , 1 , 2 , 1 , 2 }, // <
        { 1 , 1 , 1 , 1 , 1 , 2 , 2 , 2 , 2 , 2 , 2 , 1 , 2 , 1 , 2 }, // >
        { 1 , 1 , 1 , 1 , 1 , 2 , 2 , 2 , 2 , 2 , 2 , 1 , 2 , 1 , 2 }, // <=
        { 1 , 1 , 1 , 1 , 1 , 2 , 2 , 2 , 2 , 2 , 2 , 1 , 2 , 1 , 2 }, // >=
        { 1 , 1 , 1 , 1 , 1 , 2 , 2 , 2 , 2 , 2 , 2 , 1 , 2 , 1 , 2 }, // ==
        { 1 , 1 , 1 , 1 , 1 , 2 , 2 , 2 , 2 , 2 , 2 , 1 , 1 , 1 , 2 }, // ~=
        { 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 0 , 1 ,-1 }, // (
        {-1 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 ,-1 , 2 ,-1 , 2 }, // )
        { 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 , 2 ,-1 , 2 ,-1 , 2 }, // i
        { 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 , 1 ,-1 , 1 ,-1 }  // $
};

static prec_index get_prec_index(prec_symbol symbol){

    switch (symbol)
    {
        case PREC_PLUS:
        case PREC_MINUS:
            return I_PLUS_MINUS;

        case PREC_MUL:
        case PREC_DIV:
            return I_MUL_DIV;

        case PREC_IDIV:
            return I_IDIV;

        case PREC_CON:
            return I_CON;
        case PREC_LENGTH:
            return I_LENGTH;

        case PREC_EQ:
            return I_EQ;
        case PREC_NEQ:
            return I_NEQ;
        case PREC_LEQ:
            return I_LEQ;
        case PREC_LTN:
            return I_LTN;
        case PREC_MEQ:
            return I_MEQ;
        case PREC_MTN:
            return I_MTN;

        case PREC_LEFT_BRACKET:
            return I_LEFT_BRACKET;

        case PREC_RIGHT_BRACKET:
            return I_RIGHT_BRACKET;

        case PREC_IDENTIFIER:
        case PREC_INT_NUMBER:
        case PREC_DOUBLE_NUMBER:
        case PREC_STRING:
            return I_DATA;

        default:
            return I_DOLLAR;
    }
}

static prec_symbol get_symbol_from_token(t_token *token){
    switch(token->token_name)
    {
        case TOKEN_PLUS:
            return PREC_PLUS;
        case TOKEN_MINUS:
            return PREC_MINUS;
        case TOKEN_MULTIPLICATION:
            return PREC_MUL;
        case TOKEN_DIVISION:
            return PREC_DIV;
        case TOKEN_INT_DIVISION:
            return PREC_IDIV;
        case TOKEN_EQUALS:
            return PREC_EQ;
        case TOKEN_CONCATENATION:
            return PREC_CON;
        case TOKEN_LENGTH:
            return PREC_LENGTH;
        case TOKEN_NOT_EQUALS:
            return PREC_NEQ;
        case TOKEN_LESS_OR_EQUAL:
            return PREC_LEQ;
        case TOKEN_LESS:
            return PREC_LTN;
        case TOKEN_GREATER_OR_EQUAL:
            return PREC_MEQ;
        case TOKEN_GREATER:
            return PREC_MTN;
        case TOKEN_LEFT_BRACKET:
            return PREC_LEFT_BRACKET;
        case TOKEN_RIGHT_BRACKET:
            return PREC_RIGHT_BRACKET;
        case TOKEN_IDENTIFIER:
        case TOKEN_INTEGER:
        case TOKEN_NUMBER:
        case TOKEN_STRING:
            return PREC_IDENTIFIER;
        default:
            return PREC_DOLLAR;
    }
}

int find_operation(t_token *token, t_stack *stack){
    prec_index input, terminal;
    prec_symbol tmp;
    tmp = get_symbol_from_token(token);
    input = get_prec_index(tmp);

    t_stack tmp_stack = *stack;
    for(int i = 0; i < tmp_stack.amount_of_elements; i++){
        if(tmp_stack.stack_top->symbol != PREC_NON_TERM){
            terminal = get_prec_index(tmp_stack.stack_top->symbol);
            return prec_table[terminal][input];
        }
        tmp_stack.stack_top = tmp_stack.stack_top->down_element;
    }
    return ERROR_INTERNAL;
}

AST_leaf *reduce_by_rule(t_stack *tmp_stack, t_stack *stack, AST_leaf *tree, e_error_message *e_check){
    AST_leaf *top_item = stack_top(tmp_stack)->root;
    AST_leaf *new_tree;
    prec_symbol check_symbol = stack_top(tmp_stack)->symbol;
    //E -> id
    if(check_symbol == PREC_IDENTIFIER || check_symbol == PREC_INT_NUMBER || check_symbol == PREC_DOUBLE_NUMBER || check_symbol == PREC_STRING){
        stack_push(stack, top_item, PREC_NON_TERM);
        stack_pop(tmp_stack);
        new_tree = top_item;
    }
        //E -> (E)
    else if(check_symbol == PREC_LEFT_BRACKET){
        AST_leaf* ptr = stack_top(tmp_stack)->root; //todo ???
        delete_ast(ptr);//todo ???
        stack_pop(tmp_stack);
        top_item = stack_top(tmp_stack)->root;
//        AST_leaf *tmp_top_item;
        if(stack_top(tmp_stack)->symbol == PREC_NON_TERM){
            stack_pop(tmp_stack);
//            tmp_top_item = stack_top(tmp_stack)->root;
            if(stack_top(tmp_stack)->symbol == PREC_RIGHT_BRACKET){
                AST_leaf* p = stack_top(tmp_stack)->root; //todo ???
                delete_ast(p);//todo ???
                stack_pop(tmp_stack);
                stack_push(stack, top_item, PREC_NON_TERM);
                new_tree = top_item;
            }
            else{
                stack_free(tmp_stack);
                *e_check = ERROR_SEMANTIC_ANALYSIS;
                new_tree = NULL;
            }
        }
        else{
            *e_check = ERROR_SEMANTIC_ANALYSIS;
            new_tree = NULL;
        }
        return new_tree;
    }
        // E -> #E
    else if(check_symbol == PREC_LENGTH){
        new_tree = create_tree(NULL, tree, top_item->token);
        stack_push(stack, new_tree, PREC_NON_TERM);
    }
    else if(check_symbol == PREC_NON_TERM && tmp_stack->amount_of_elements == 3){
        if(stack_top(tmp_stack)->down_element->down_element->symbol != PREC_NON_TERM){
            *e_check = ERROR_SEMANTIC_ANALYSIS;
            delete_ast(tree);
            new_tree = NULL;
            return new_tree;
        }
        AST_leaf *op_item;
        op_item = stack_top(tmp_stack)->down_element->root;
        AST_leaf *left_leaf;
        // E -> E+E
        if(get_symbol_from_token(op_item->token) == PREC_PLUS){
            left_leaf = top_item;
            new_tree = create_tree(left_leaf, tree, op_item->token);
            delete_ast(op_item);
            stack_push(stack, new_tree, PREC_NON_TERM);
        }
            // E -> E-E
        else if(get_symbol_from_token(op_item->token) == PREC_MINUS){
            left_leaf = top_item;
            new_tree = create_tree(left_leaf, tree, op_item->token);
            delete_ast(op_item);
            stack_push(stack, new_tree, PREC_NON_TERM);
        }
            // E -> E*E
        else if(get_symbol_from_token(op_item->token) == PREC_MUL){
            left_leaf = top_item;
            new_tree = create_tree(left_leaf, tree, op_item->token);
            delete_ast(op_item);
            stack_push(stack, new_tree, PREC_NON_TERM);
        }
            // E -> E/E
        else if(get_symbol_from_token(op_item->token) == PREC_DIV){
            left_leaf = top_item;
            new_tree = create_tree(left_leaf, tree, op_item->token);
            delete_ast(op_item);
            stack_push(stack, new_tree, PREC_NON_TERM);
        }
            // E -> E//E
        else if(get_symbol_from_token(op_item->token) == PREC_IDIV){
            left_leaf = top_item;
            new_tree = create_tree(left_leaf, tree, op_item->token);
            delete_ast(op_item);
            stack_push(stack, new_tree, PREC_NON_TERM);
        }
            // E -> E<E
        else if(get_symbol_from_token(op_item->token) == PREC_LTN){
            left_leaf = top_item;
            new_tree = create_tree(left_leaf, tree, op_item->token);
            delete_ast(op_item);
            stack_push(stack, new_tree, PREC_NON_TERM);
        }
            // E -> E>E
        else if(get_symbol_from_token(op_item->token) == PREC_MTN){
            left_leaf = top_item;
            new_tree = create_tree(left_leaf, tree, op_item->token);
            delete_ast(op_item);
            stack_push(stack, new_tree, PREC_NON_TERM);
        }
            // E -> E<=E
        else if(get_symbol_from_token(op_item->token) == PREC_LEQ){
            left_leaf = top_item;
            new_tree = create_tree(left_leaf, tree, op_item->token);
            delete_ast(op_item);
            stack_push(stack, new_tree, PREC_NON_TERM);
        }
            // E -> E>=E
        else if(get_symbol_from_token(op_item->token) == PREC_MEQ){
            left_leaf = top_item;
            new_tree = create_tree(left_leaf, tree, op_item->token);
            delete_ast(op_item);
            stack_push(stack, new_tree, PREC_NON_TERM);
        }
            // E -> E==E
        else if(get_symbol_from_token(op_item->token) == PREC_EQ){
            left_leaf = top_item;
            new_tree = create_tree(left_leaf, tree, op_item->token);
            delete_ast(op_item);
            stack_push(stack, new_tree, PREC_NON_TERM);
        }
            // E -> E~=E
        else if(get_symbol_from_token(op_item->token) == PREC_NEQ){
            left_leaf = top_item;
            new_tree = create_tree(left_leaf, tree, op_item->token);
            delete_ast(op_item);
            stack_push(stack, new_tree, PREC_NON_TERM);
        }
            // E -> E..E
        else if(get_symbol_from_token(op_item->token) == PREC_CON){
            left_leaf = top_item;
            new_tree = create_tree(left_leaf, tree, op_item->token);
            delete_ast(op_item);
            stack_push(stack, new_tree, PREC_NON_TERM);
        }
        else{
            *e_check = ERROR_SEMANTIC_ANALYSIS;
            delete_ast(tree);
            new_tree = NULL;
        }
    }
    else{
        *e_check = ERROR_SEMANTIC_ANALYSIS;
        delete_ast(tree);
        new_tree = NULL;
    }


    stack_free(tmp_stack); //todo ???
    return new_tree;

}

AST_leaf *get_expression(t_token *token, t_stack *stack, AST_leaf *tree, e_error_message *e_check){
    AST_leaf *tmp_item = create_leaf(token);
    AST_leaf *top_item = NULL;
    int operation = find_operation(token, stack);
    if(operation == ERROR_INTERNAL){
        printf("Error\n");
        return NULL;
    }



    if(operation == 0){ // =
        stack_push(stack, tmp_item, get_symbol_from_token(token));
    }
    if(operation == 1){ // <
        if(stack->stack_top->symbol != PREC_NON_TERM){
            stack_push(stack, NULL, PREC_STOP);
            stack_push(stack, tmp_item, get_symbol_from_token(token));
        }
        else{
            top_item = stack_top(stack)->root;
            stack_pop(stack);
            stack_push(stack, NULL, PREC_STOP);
            stack_push(stack, top_item, PREC_NON_TERM);
            stack_push(stack, tmp_item, get_symbol_from_token(token));
        }
    }
    if(operation == 2){ // >
        t_stack tmp_stack;
        stack_init(&tmp_stack);
        delete_ast(tmp_item);// todo ????
        tmp_item = NULL;
        prec_symbol top_symbol;
        do{
            tmp_item = stack_top(stack)->root;
            stack_push(&tmp_stack, tmp_item, stack_top(stack)->symbol);
            stack_pop(stack);
            top_symbol = stack_top(stack)->symbol;
        }while(top_symbol != PREC_STOP);
        stack_pop(stack);
        tree = reduce_by_rule(&tmp_stack, stack, tree, e_check);

    }else if(operation == -1){
        delete_ast(tmp_item); //todo ???
    }
    return tree;
}

AST_leaf *precede_expression(t_token *token, e_error_message *e_check){
    t_stack stack;
    prec_symbol token_symbol, top_symbol;
    AST_leaf *tree = NULL; //todo del init
    int operation;
    bool id_to_id = false;
    bool id_to_nil = false;
    *e_check = IT_IS_OK;
    stack_init(&stack);
    stack_push(&stack, NULL, PREC_DOLLAR);

    do{
        //Основная
        top_symbol = stack_top(&stack)->symbol;
        token_symbol = get_symbol_from_token(token);
        if(token->token_name == TOKEN_KEYWORD){
            if(token->lexeme->keyword == KEYWORD_NIL){
                token->token_name = TOKEN_IDENTIFIER;
                token_symbol = PREC_IDENTIFIER;
                id_to_nil = true;
            }
        }
        if((token_symbol == PREC_IDENTIFIER && top_symbol == PREC_IDENTIFIER) || (token_symbol == PREC_IDENTIFIER && top_symbol == PREC_RIGHT_BRACKET)) {
            token->token_name = TOKEN_KEYWORD;
            id_to_id = true;
        }

        if(top_symbol == PREC_NON_TERM)
            top_symbol = stack_top(&stack)->down_element->symbol;
        operation = find_operation(token, &stack);
        if(top_symbol == PREC_DOLLAR && token_symbol == PREC_DOLLAR)
            break;
        if(operation == 2){ // >
            while(operation == 2){
                operation = find_operation(token, &stack);
                tree = get_expression(token, &stack, tree, e_check);
            }
        }
        else if(operation == 1 || operation == 0){ // < , =
            tree = get_expression(token, &stack, tree, e_check);
        }
        else{ // empty
            delete_ast(tree);
            *e_check = ERROR_SEMANTIC_ANALYSIS;
            return NULL;
        }
        if(get_symbol_from_token(token) != PREC_DOLLAR || id_to_nil){
            if(get_token(token)) *e_check = ERROR_LEX_ANALYSIS;
            id_to_nil = false;
        }

    }
    while(1);
    if(id_to_id) {
        token->token_name = TOKEN_IDENTIFIER;
    }

    tree = convert_id_to_nil(tree);
    stack_free(&stack);
    hold_token();
    return tree;
}

AST_leaf *convert_id_to_nil(AST_leaf *tree){
    if(tree){
        if(tree->left)
            convert_id_to_nil(tree->left);
        if(tree->token->lexeme->keyword == KEYWORD_NIL){
            tree->token->token_name = TOKEN_KEYWORD;
        }
        if(tree->right)
            convert_id_to_nil(tree->right);
        return tree;
    }
    return NULL;
}

void tree_to_stack(AST_leaf *tree, t_stack *stack){
    if(tree){
        tree_to_stack(tree->left, stack);
        stack_push(stack, tree, get_symbol_from_token(tree->token));
        tree_to_stack(tree->right, stack);
    }
}
int check_expression(AST_leaf *tree, t_ast_node *ast_node){

    t_stack stack;
    bool is_len = false;
    stack_init(&stack);
    tree_to_stack(tree, &stack);
//    AST_leaf *tmp_leaf;
    t_token *token;
    if(ast_node->it_is_variable_expression){
        while(stack.amount_of_elements != 0){
            token = stack_top(&stack)->root->token;
            if(token->lexeme->keyword == KEYWORD_NIL){
                if(stack.amount_of_elements > 1){
                    if(stack_top(&stack)->down_element->root->token->token_name != TOKEN_EQUALS
                            && stack_top(&stack)->down_element->root->token->token_name != TOKEN_NOT_EQUALS){
                        stack_free(&stack);
                        return ERROR_SEMANTIC_ANALYSIS_EXPR;
                    }
                }
            }
            if (token->token_name == TOKEN_EQUALS || token->token_name == TOKEN_NOT_EQUALS || token->token_name == TOKEN_LESS_OR_EQUAL
                || token->token_name == TOKEN_LESS || token->token_name == TOKEN_GREATER_OR_EQUAL || token->token_name == TOKEN_GREATER) {
                stack_free(&stack);
                return ERROR_SEMANTIC_ANALYSIS_EXPR;
            }
            if(token->token_name == TOKEN_IDENTIFIER){
                node *function_var = NULL;
                ul hash = hashcode(token->lexeme->inter->data);
                bool error_null = false;
                function_var = check_type_stack(error_null,hash);
                if(error_null){
                    return ERROR_INTERNAL;
                }
                if(!function_var){
                    return ERROR_SEMANTIC_ANALYSIS;
                }
                if(string_arr_cmp(function_var->data->type, "string")){
                    token->lexeme->str_bool = true;
                }else  if(string_arr_cmp(function_var->data->type, "integer")){
                    token->lexeme->int_bool = true;
                }else  if(string_arr_cmp(function_var->data->type, "number")){
                    token->lexeme->number_bool = true;
                }
                if(string_arr_cmp(function_var->data->type, "nil")){
                    if(stack.amount_of_elements > 1){
                        if(stack_top(&stack)->down_element->root->token->token_name != TOKEN_EQUALS
                           && stack_top(&stack)->down_element->root->token->token_name != TOKEN_NOT_EQUALS){
                            stack_free(&stack);
                            return ERROR_SEMANTIC_ANALYSIS_EXPR;
                        }
                    }
                }
                if(string_param_cmp_arr(ast_node->type_variable, ast_node->count_expression,"integer")){
                    if(string_arr_cmp(function_var->data->type, "string")){
                        if(stack.amount_of_elements > 1){
                            if(stack_top(&stack)->down_element->root->token->token_name != TOKEN_LENGTH
                               && stack_top(&stack)->down_element->root->token->token_name != TOKEN_CONCATENATION){
                                stack_free(&stack);
                                return ERROR_SEMANTIC_ANALYSIS_EXPR;
                            }
                        }
                    }
                    if(string_arr_cmp(function_var->data->type, "number")){
                        stack_free(&stack);
                        return ERROR_SEMANTIC_ANALYSIS_EXPR;
                    }
                }
                else if(string_param_cmp_arr(ast_node->type_variable, ast_node->count_expression,"number")){
                    if(string_arr_cmp(function_var->data->type, "string")){
                        if(stack.amount_of_elements > 1){
                            if(stack_top(&stack)->down_element->root->token->token_name != TOKEN_LENGTH
                               && stack_top(&stack)->down_element->root->token->token_name != TOKEN_CONCATENATION){
                                stack_free(&stack);
                                return ERROR_SEMANTIC_ANALYSIS_EXPR;
                            }
                        }
                        else{
                            stack_free(&stack);
                            return ERROR_SEMANTIC_ANALYSIS_EXPR;
                        }
                    }
                }
                else if(string_param_cmp_arr(ast_node->type_variable, ast_node->count_expression,"string")){
                    if(!string_arr_cmp(function_var->data->type, "string")){
                        stack_free(&stack);
                        return ERROR_SEMANTIC_ANALYSIS_EXPR;
                    }
                    else{
                        if(stack.amount_of_elements > 1){
                            if(stack_top(&stack)->down_element->root->token->token_name == TOKEN_LENGTH){
                                stack_free(&stack);
                                return ERROR_SEMANTIC_ANALYSIS_EXPR;
                            }
                        }
                    }
                }
                if(stack.amount_of_elements > 1){
                    if(stack_top(&stack)->down_element->root->token->token_name == TOKEN_LENGTH
                       || stack_top(&stack)->down_element->root->token->token_name == TOKEN_CONCATENATION){
                        if(!string_arr_cmp(function_var->data->type, "string")){
                            stack_free(&stack);
                            return ERROR_SEMANTIC_ANALYSIS_EXPR;
                        }
                    }
                }
            }
            if(token->token_name == TOKEN_INTEGER){
                if(string_param_cmp_arr(ast_node->type_variable, ast_node->count_expression,"string")){
                    stack_free(&stack);
                    return ERROR_SEMANTIC_ANALYSIS_EXPR;
                }
            }
            else if(token->token_name == TOKEN_NUMBER){
                if(string_param_cmp_arr(ast_node->type_variable, ast_node->count_expression,"integer")){
                    stack_free(&stack);
                    return ERROR_SEMANTIC_ANALYSIS_EXPR;
                }
                if(string_param_cmp_arr(ast_node->type_variable, ast_node->count_expression,"string")){
                    stack_free(&stack);
                    return ERROR_SEMANTIC_ANALYSIS_EXPR;
                }
            }
            else if(token->token_name == TOKEN_STRING){
                if(!string_param_cmp_arr(ast_node->type_variable, ast_node->count_expression,"string")){
                    stack_free(&stack);
                    return ERROR_SEMANTIC_ANALYSIS_EXPR;
                }
            }
            stack_pop(&stack);
        }
    }
        //---------------------------------------------------------------------------
    else if(ast_node->it_is_return_exp) {
        while(stack.amount_of_elements != 0){
            token = stack_top(&stack)->root->token;
            if(token->lexeme->keyword == KEYWORD_NIL){
                if(stack.amount_of_elements > 1){
                    if(stack_top(&stack)->down_element->root->token->token_name != TOKEN_EQUALS
                       && stack_top(&stack)->down_element->root->token->token_name != TOKEN_NOT_EQUALS){
                        stack_free(&stack);
                        return ERROR_SEMANTIC_ANALYSIS_EXPR;
                    }
                }
            }
            if (token->token_name == TOKEN_EQUALS || token->token_name == TOKEN_NOT_EQUALS || token->token_name == TOKEN_LESS_OR_EQUAL
                || token->token_name == TOKEN_LESS || token->token_name == TOKEN_GREATER_OR_EQUAL || token->token_name == TOKEN_GREATER) {
                stack_free(&stack);
                return ERROR_SEMANTIC_ANALYSIS_EXPR;
            }
            if(token->token_name == TOKEN_IDENTIFIER){
                node *function_var = NULL;
                ul hash = hashcode(token->lexeme->inter->data);
                bool error_null = false;
                function_var = check_type_stack(error_null,hash);
                if(error_null){
                    return ERROR_INTERNAL;
                }
                if(!function_var){
                    return ERROR_SEMANTIC_ANALYSIS;
                }
                if(string_arr_cmp(function_var->data->type, "string")){
                    token->lexeme->str_bool = true;
                }else  if(string_arr_cmp(function_var->data->type, "integer")){
                    token->lexeme->int_bool = true;
                }else  if(string_arr_cmp(function_var->data->type, "number")){
                    token->lexeme->number_bool = true;
                }
                if(string_arr_cmp(function_var->data->type, "nil")){
                    if(stack.amount_of_elements > 1){
                        if(stack_top(&stack)->down_element->root->token->token_name != TOKEN_EQUALS
                           && stack_top(&stack)->down_element->root->token->token_name != TOKEN_NOT_EQUALS){
                            stack_free(&stack);
                            return ERROR_SEMANTIC_ANALYSIS_EXPR;
                        }
                    }
                }
                if(string_param_cmp_arr(ast_node->function_info->type_returned_params, ast_node->function_info->count_returned_params ,"integer")){
                    if(string_arr_cmp(function_var->data->type, "number")){
                        stack_free(&stack);
                        return ERROR_SEMANTIC_ANALYSIS_EXPR;
                    }
                    else if(string_arr_cmp(function_var->data->type, "string")){
                        if(stack.amount_of_elements > 1){
                            if(stack_top(&stack)->down_element->root->token->token_name != TOKEN_LENGTH
                               && stack_top(&stack)->down_element->root->token->token_name != TOKEN_CONCATENATION){
                                stack_free(&stack);
                                return ERROR_SEMANTIC_ANALYSIS_EXPR;
                            }
                        }
                    }
                    else{
                        if(stack.amount_of_elements > 1){
                            if(stack_top(&stack)->down_element->root->token->token_name == TOKEN_LENGTH
                               || stack_top(&stack)->down_element->root->token->token_name == TOKEN_CONCATENATION){
                                stack_free(&stack);
                                return ERROR_SEMANTIC_ANALYSIS_EXPR;
                            }
                        }
                    }
                }
                else if(string_param_cmp_arr(ast_node->function_info->type_returned_params, ast_node->function_info->count_returned_params ,"number")){
                    if(string_arr_cmp(function_var->data->type, "string")){
                        if(stack.amount_of_elements > 1){
                            if(stack_top(&stack)->down_element->root->token->token_name != TOKEN_LENGTH
                               && stack_top(&stack)->down_element->root->token->token_name != TOKEN_CONCATENATION){
                                stack_free(&stack);
                                return ERROR_SEMANTIC_ANALYSIS_EXPR;
                            }
                        }
                    }
                    else{
                        if(stack.amount_of_elements > 1){
                            if(stack_top(&stack)->down_element->root->token->token_name == TOKEN_LENGTH
                               || stack_top(&stack)->down_element->root->token->token_name == TOKEN_CONCATENATION){
                                stack_free(&stack);
                                return ERROR_SEMANTIC_ANALYSIS_EXPR;
                            }
                        }
                    }
                }
                else if(string_param_cmp_arr(ast_node->function_info->type_returned_params, ast_node->function_info->count_returned_params ,"string")){
                    if(!string_arr_cmp(function_var->data->type, "string")){
                        stack_free(&stack);
                        return ERROR_SEMANTIC_ANALYSIS_EXPR;
                    }
                    else{
                        if(stack.amount_of_elements > 1){
                            if(stack_top(&stack)->down_element->root->token->token_name == TOKEN_LENGTH){
                                stack_free(&stack);
                                return ERROR_SEMANTIC_ANALYSIS_EXPR;
                            }
                        }
                    }
                }
            }
            if(token->token_name == TOKEN_INTEGER){
                if(string_param_cmp_arr(ast_node->function_info->type_returned_params, ast_node->function_info->count_returned_params,"string")){
                    stack_free(&stack);
                    return ERROR_SEMANTIC_ANALYSIS_EXPR;
                }
            }
            else if(token->token_name == TOKEN_NUMBER){
                if(string_param_cmp_arr(ast_node->function_info->type_returned_params, ast_node->function_info->count_returned_params,"integer")){
                    stack_free(&stack);
                    return ERROR_SEMANTIC_ANALYSIS_EXPR;
                }
                if(string_param_cmp_arr(ast_node->function_info->type_returned_params, ast_node->function_info->count_returned_params,"string")){
                    stack_free(&stack);
                    return ERROR_SEMANTIC_ANALYSIS_EXPR;
                }
            }
            else if(token->token_name == TOKEN_STRING){
                if(!string_param_cmp_arr(ast_node->function_info->type_returned_params, ast_node->function_info->count_returned_params,"string")){
                    stack_free(&stack);
                    return ERROR_SEMANTIC_ANALYSIS_EXPR;
                }
            }

            stack_pop(&stack);
        }
    }
        //---------------------------------------------------------------------------
    else{
        bool sign_repeat = false;
        t_token *top_token = stack_top(&stack)->root->token;
        if(stack.amount_of_elements > 1){
            if(stack_top(&stack)->down_element->root->token->token_name == TOKEN_LENGTH
               || stack_top(&stack)->down_element->root->token->token_name == TOKEN_CONCATENATION){
                is_len = true;
            }
        }
        if(top_token->token_name == TOKEN_IDENTIFIER && !is_len){
            node *top_var = NULL;
            ul hash = hashcode(top_token->lexeme->inter->data);
            bool error_null = false;
            top_var = check_type_stack(error_null,hash);
            if(error_null){
                return ERROR_INTERNAL;
            }
            if(!top_var){
                return ERROR_SEMANTIC_ANALYSIS;
            }
            while(stack.amount_of_elements != 0){
                token = stack_top(&stack)->root->token;
                if (token->token_name == TOKEN_EQUALS || token->token_name == TOKEN_NOT_EQUALS || token->token_name == TOKEN_LESS_OR_EQUAL
                    || token->token_name == TOKEN_LESS || token->token_name == TOKEN_GREATER_OR_EQUAL || token->token_name == TOKEN_GREATER) {
                    if(sign_repeat){
                        stack_free(&stack);
                        return ERROR_SEMANTIC_ANALYSIS_EXPR;
                    }
                    sign_repeat = true;
                }
                if(token->lexeme->keyword == KEYWORD_NIL){
                    if(stack.amount_of_elements > 1){
                        if(stack_top(&stack)->down_element->root->token->token_name != TOKEN_EQUALS
                           && stack_top(&stack)->down_element->root->token->token_name != TOKEN_NOT_EQUALS){
                            stack_free(&stack);
                            return ERROR_SEMANTIC_ANALYSIS_EXPR;
                        }
                    }
                }
                if(token->token_name == TOKEN_IDENTIFIER){
                    node *function_var = NULL;
                    hash = hashcode(token->lexeme->inter->data);
                    function_var = check_type_stack(error_null,hash);
                    if(error_null){
                        return ERROR_INTERNAL;
                    }
                    if(!function_var){
                        return ERROR_SEMANTIC_ANALYSIS;
                    }
                    if(string_arr_cmp(function_var->data->type, "string")){
                        token->lexeme->str_bool = true;
                    }else  if(string_arr_cmp(function_var->data->type, "integer")){
                        token->lexeme->int_bool = true;
                    }else  if(string_arr_cmp(function_var->data->type, "number")){
                        token->lexeme->number_bool = true;
                    }
                    if(string_arr_cmp(function_var->data->type, "nil")){
                        if(stack.amount_of_elements > 1){
                            if(stack_top(&stack)->down_element->root->token->token_name != TOKEN_EQUALS
                               && stack_top(&stack)->down_element->root->token->token_name != TOKEN_NOT_EQUALS){
                                stack_free(&stack);
                                return ERROR_SEMANTIC_ANALYSIS_EXPR;
                            }
                        }
                    }
                    if(string_arr_cmp(top_var->data->type, "integer")){
                        if(string_arr_cmp(function_var->data->type, "string")){
                            if(stack.amount_of_elements > 1){
                                if(stack_top(&stack)->down_element->root->token->token_name != TOKEN_LENGTH
                                   && stack_top(&stack)->down_element->root->token->token_name != TOKEN_CONCATENATION){
                                    stack_free(&stack);
                                    return ERROR_SEMANTIC_ANALYSIS_EXPR;
                                }
                            }
                        }
                        else{
                            if(stack.amount_of_elements > 1){
                                if(stack_top(&stack)->down_element->root->token->token_name == TOKEN_LENGTH
                                   || stack_top(&stack)->down_element->root->token->token_name == TOKEN_CONCATENATION){
                                    stack_free(&stack);
                                    return ERROR_SEMANTIC_ANALYSIS_EXPR;
                                }
                            }
                        }
                    }
                    if(string_arr_cmp(top_var->data->type, "number")){
                        if(string_arr_cmp(function_var->data->type, "string")){
                            if(stack.amount_of_elements > 1){
                                if(stack_top(&stack)->down_element->root->token->token_name != TOKEN_LENGTH
                                   && stack_top(&stack)->down_element->root->token->token_name != TOKEN_CONCATENATION){
                                    stack_free(&stack);
                                    return ERROR_SEMANTIC_ANALYSIS_EXPR;
                                }
                            }
                        }
                        else{
                            if(stack.amount_of_elements > 1){
                                if(stack_top(&stack)->down_element->root->token->token_name == TOKEN_LENGTH
                                   || stack_top(&stack)->down_element->root->token->token_name == TOKEN_CONCATENATION){
                                    stack_free(&stack);
                                    return ERROR_SEMANTIC_ANALYSIS_EXPR;
                                }
                            }
                        }
                    }
                    if(string_arr_cmp(top_var->data->type, "string")){
                        if(!string_arr_cmp(function_var->data->type, "string")){
                            stack_free(&stack);
                            return ERROR_SEMANTIC_ANALYSIS_EXPR;
                        }
                        else{
                            if(stack.amount_of_elements > 1){
                                if(stack_top(&stack)->down_element->root->token->token_name == TOKEN_LENGTH){
                                    stack_free(&stack);
                                    return ERROR_SEMANTIC_ANALYSIS_EXPR;
                                }
                            }
                        }
                    }
                }
                if(token->token_name == TOKEN_INTEGER){
                    if(string_arr_cmp(top_var->data->type, "string")){
                        stack_free(&stack);
                        return ERROR_SEMANTIC_ANALYSIS_EXPR;
                    }
                }
                else if(token->token_name == TOKEN_NUMBER){
                    if(string_arr_cmp(top_var->data->type, "integer")){
                        stack_free(&stack);
                        return ERROR_SEMANTIC_ANALYSIS_EXPR;
                    }
                    if(string_arr_cmp(top_var->data->type, "string")){
                        stack_free(&stack);
                        return ERROR_SEMANTIC_ANALYSIS_EXPR;
                    }
                }
                else if(token->token_name == TOKEN_STRING){
                    if(!string_arr_cmp(top_var->data->type, "string")){
                        stack_free(&stack);
                        return ERROR_SEMANTIC_ANALYSIS_EXPR;
                    }
                }
                stack_pop(&stack);
            }
        }
        else if(is_len){
//            node *top_var = NULL;
//            ul hash = hashcode(top_token->lexeme->inter->data);
//            bool error_null = false;
//            top_var = check_type_stack(error_null,hash);
            while(stack.amount_of_elements != 0){
                token = stack_top(&stack)->root->token;
                if(token->lexeme->keyword == KEYWORD_NIL){
                    if(stack.amount_of_elements > 1){
                        if(stack_top(&stack)->down_element->root->token->token_name != TOKEN_EQUALS
                           && stack_top(&stack)->down_element->root->token->token_name != TOKEN_NOT_EQUALS){
                            stack_free(&stack);
                            return ERROR_SEMANTIC_ANALYSIS_EXPR;
                        }
                    }
                }
                if (token->token_name == TOKEN_EQUALS || token->token_name == TOKEN_NOT_EQUALS || token->token_name == TOKEN_LESS_OR_EQUAL
                    || token->token_name == TOKEN_LESS || token->token_name == TOKEN_GREATER_OR_EQUAL || token->token_name == TOKEN_GREATER) {
                    if(sign_repeat){
                        stack_free(&stack);
                        return ERROR_SEMANTIC_ANALYSIS_EXPR;
                    }
                    sign_repeat = true;
                }
                if(token->token_name == TOKEN_IDENTIFIER){
                    node *function_var = NULL;
                    ul hash = hashcode(token->lexeme->inter->data);
                    bool error_null = false;
                    function_var = check_type_stack(error_null,hash);
                    if(error_null){
                        return ERROR_INTERNAL;
                    }
                    if(!function_var){
                        return ERROR_SEMANTIC_ANALYSIS;
                    }
                    if(string_arr_cmp(function_var->data->type, "string")){
                        token->lexeme->str_bool = true;
                    }else  if(string_arr_cmp(function_var->data->type, "integer")){
                        token->lexeme->int_bool = true;
                    }else  if(string_arr_cmp(function_var->data->type, "number")){
                        token->lexeme->number_bool = true;
                    }
                    if(string_arr_cmp(function_var->data->type, "nil")){
                        if(stack.amount_of_elements > 1){
                            if(stack_top(&stack)->down_element->root->token->token_name != TOKEN_EQUALS
                               && stack_top(&stack)->down_element->root->token->token_name != TOKEN_NOT_EQUALS){
                                stack_free(&stack);
                                return ERROR_SEMANTIC_ANALYSIS_EXPR;
                            }
                        }
                    }
                    if(string_arr_cmp(function_var->data->type, "string")){
                        if(stack.amount_of_elements > 1){
                            if(stack_top(&stack)->down_element->root->token->token_name != TOKEN_LENGTH
                               && stack_top(&stack)->down_element->root->token->token_name != TOKEN_CONCATENATION){
                                stack_free(&stack);
                                return ERROR_SEMANTIC_ANALYSIS_EXPR;
                            }
                        }
                        else{
                            stack_free(&stack);
                            return ERROR_SEMANTIC_ANALYSIS_EXPR;
                        }
                    }
                    else{
                        if(stack.amount_of_elements > 1){
                            if(stack_top(&stack)->down_element->root->token->token_name == TOKEN_LENGTH
                               || stack_top(&stack)->down_element->root->token->token_name == TOKEN_CONCATENATION){
                                stack_free(&stack);
                                return ERROR_SEMANTIC_ANALYSIS_EXPR;
                            }
                        }
                    }


                }
                else if(token->token_name == TOKEN_STRING){
                    if(stack.amount_of_elements > 1){
                        if(stack_top(&stack)->down_element->root->token->token_name != TOKEN_LENGTH){
                            stack_free(&stack);
                            return ERROR_SEMANTIC_ANALYSIS_EXPR;
                        }
                    }
                    else{
                        stack_free(&stack);
                        return ERROR_SEMANTIC_ANALYSIS_EXPR;
                    }
                }
                else{
                    if(stack.amount_of_elements > 1){
                        if(stack_top(&stack)->down_element->root->token->token_name == TOKEN_LENGTH
                           || stack_top(&stack)->down_element->root->token->token_name == TOKEN_CONCATENATION){
                            stack_free(&stack);
                            return ERROR_SEMANTIC_ANALYSIS_EXPR;
                        }
                    }
                }

                stack_pop(&stack);
            }
        }
        else{
            while(stack.amount_of_elements != 0){
                token = stack_top(&stack)->root->token;
                if(token->lexeme->keyword == KEYWORD_NIL){
                    if(stack.amount_of_elements > 1){
                        if(stack_top(&stack)->down_element->root->token->token_name != TOKEN_EQUALS
                           && stack_top(&stack)->down_element->root->token->token_name != TOKEN_NOT_EQUALS){
                            stack_free(&stack);
                            return ERROR_SEMANTIC_ANALYSIS_EXPR;
                        }
                    }
                }
                if (token->token_name == TOKEN_EQUALS || token->token_name == TOKEN_NOT_EQUALS || token->token_name == TOKEN_LESS_OR_EQUAL
                    || token->token_name == TOKEN_LESS || token->token_name == TOKEN_GREATER_OR_EQUAL || token->token_name == TOKEN_GREATER) {
                    if(sign_repeat){
                        stack_free(&stack);
                        return ERROR_SEMANTIC_ANALYSIS_EXPR;
                    }
                    sign_repeat = true;
                }
                if(token->token_name == TOKEN_IDENTIFIER){
                    node *function_var = NULL;
                    ul hash = hashcode(token->lexeme->inter->data);
                    bool error_null = false;
                    function_var = check_type_stack(error_null,hash);
                    if(error_null){
                        return ERROR_INTERNAL;
                    }
                    if(!function_var){
                        return ERROR_SEMANTIC_ANALYSIS;
                    }
                    if(string_arr_cmp(function_var->data->type, "string")){
                        token->lexeme->str_bool = true;
                    }else  if(string_arr_cmp(function_var->data->type, "integer")){
                        token->lexeme->int_bool = true;
                    }else  if(string_arr_cmp(function_var->data->type, "number")){
                        token->lexeme->number_bool = true;
                    }
                    if(string_arr_cmp(function_var->data->type, "nil")){
                        if(stack.amount_of_elements > 1){
                            if(stack_top(&stack)->down_element->root->token->token_name != TOKEN_EQUALS
                               && stack_top(&stack)->down_element->root->token->token_name != TOKEN_NOT_EQUALS){
                                stack_free(&stack);
                                return ERROR_SEMANTIC_ANALYSIS_EXPR;
                            }
                        }
                    }
                    if(top_token->token_name == TOKEN_INTEGER){
                        if(string_arr_cmp(function_var->data->type, "string")){
                            if(stack.amount_of_elements > 1){
                                if(stack_top(&stack)->down_element->root->token->token_name != TOKEN_LENGTH
                                   && stack_top(&stack)->down_element->root->token->token_name != TOKEN_CONCATENATION){
                                    stack_free(&stack);
                                    return ERROR_SEMANTIC_ANALYSIS_EXPR;
                                }
                            }
                        }
                        else{
                            if(stack.amount_of_elements > 1){
                                if(stack_top(&stack)->down_element->root->token->token_name == TOKEN_LENGTH
                                   || stack_top(&stack)->down_element->root->token->token_name == TOKEN_CONCATENATION){
                                    stack_free(&stack);
                                    return ERROR_SEMANTIC_ANALYSIS_EXPR;
                                }
                            }
                        }
                    }
                    if(top_token->token_name == TOKEN_NUMBER || top_token->token_name == TOKEN_NUMBER_EXPONENT){
                        if(string_arr_cmp(function_var->data->type, "string")){
                            if(stack.amount_of_elements > 1){
                                if(stack_top(&stack)->down_element->root->token->token_name != TOKEN_LENGTH
                                   && stack_top(&stack)->down_element->root->token->token_name != TOKEN_CONCATENATION){
                                    stack_free(&stack);
                                    return ERROR_SEMANTIC_ANALYSIS_EXPR;
                                }
                            }
                        }
                        else{
                            if(stack.amount_of_elements > 1){
                                if(stack_top(&stack)->down_element->root->token->token_name == TOKEN_LENGTH
                                   || stack_top(&stack)->down_element->root->token->token_name == TOKEN_CONCATENATION){
                                    stack_free(&stack);
                                    return ERROR_SEMANTIC_ANALYSIS_EXPR;
                                }
                            }
                        }
                    }
                    if(top_token->token_name == TOKEN_STRING){
                        if(!string_arr_cmp(function_var->data->type, "string")){
                            stack_free(&stack);
                            return ERROR_SEMANTIC_ANALYSIS_EXPR;
                        }
                        else{
                            if(stack.amount_of_elements > 1){
                                if(stack_top(&stack)->down_element->root->token->token_name == TOKEN_LENGTH){
                                    stack_free(&stack);
                                    return ERROR_SEMANTIC_ANALYSIS_EXPR;
                                }
                            }
                        }
                    }
                }
                if(token->token_name == TOKEN_INTEGER){
                    if(top_token->token_name == TOKEN_STRING){
                        stack_free(&stack);
                        return ERROR_SEMANTIC_ANALYSIS_EXPR;
                    }
                }
                else if(token->token_name == TOKEN_NUMBER){
                    if(top_token->token_name == TOKEN_INTEGER){
                        stack_free(&stack);
                        return ERROR_SEMANTIC_ANALYSIS_EXPR;
                    }
                    if(top_token->token_name == TOKEN_STRING){
                        stack_free(&stack);
                        return ERROR_SEMANTIC_ANALYSIS_EXPR;
                    }
                }
                else if(token->token_name == TOKEN_STRING){
                    if(top_token->token_name != TOKEN_STRING){
                        stack_free(&stack);
                        return ERROR_SEMANTIC_ANALYSIS_EXPR;
                    }
                }


                stack_pop(&stack);
            }
        }
    }

    stack_free(&stack);
    return IT_IS_OK;
}
