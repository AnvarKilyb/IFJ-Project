
#include "codgen.h"
char *header = ".IFJcode21";
static t_str *program_body;
static t_str *program_header;


void print_tree(AST_leaf* tree){
    if(tree){
        print_tree(tree->left);
        printf("%s",tree->token->lexeme->inter->data);
        print_tree(tree->right);
    }
}


int code_header(){
    program_header = malloc(sizeof (t_str));
    if(string_init(program_header))
        return ERROR_INTERNAL;

    string_wright_arr(program_header, header);
    return IT_IS_OK;
}
//int code_expression_node(t_ast_node *ast_tree){
//    return IT_IS_OK;
//}
//int code_loop(t_ast_node *ast_tree){
//    return IT_IS_OK;
//}
//int code_if(t_ast_node *ast_tree){
//    return IT_IS_OK;
//}
//int code_declaration_variable(t_ast_node *ast_tree){
//    return IT_IS_OK;
//}
//
//int code_function(t_ast_node *ast_tree){
//    return IT_IS_OK;
//}
int code_gen(t_ast_node *ast_tree){
    t_ast_node* p = ast_tree;
    printf("***********************************************************************************\n");
    while(p){
        printf("-------------------------------------------------------------------------------\n");
        if (p->variable || p->type_variable) {
            printf("variable: ");
            if (p->type_variable) {
                for (int i = 0; i < p->count_variable; i++) {
                    printf("%s  ", p->variable->data[i]->data);
                }
            }
            if (p->type_variable) {
                if (p->type_variable) {
                    for (int i = 0; i < p->count_variable; i++) {
                        printf("%s  ", p->type_variable->data[i]->data);
                    }
                    printf("\n");
                }
            }
        }
        if (p->count_variable != 0) {
            printf("count_variable: %d\n", p->count_variable);
        }
        if (p->func) {
            printf("%s  ", p->func->name->data);
            printf("%s  ", p->func->type->data);
            printf("\t");
            if(p->func->params) {
                for (int i = 0; i < p->func->count_params; i++) {
                    printf("%s  ", p->func->params->data[i]->data);
                }
            }
            if(p->func->type_params) {
                for (int i = 0; i < p->func->count_params; i++) {
                    printf("%s  ", p->func->type_params->data[i]->data);
                }
            }

            if (p->func->type_returned_params) {
                printf(":");
                for (int i = 0; i < p->func->count_returned_params; i++) {
                    printf("%s  ", p->func->type_returned_params->data[i]->data);
                }
            }
            printf("\n");
        }
        printf("count_expression: %d\n",p->count_expression);
        printf("count_func_param: %d\n",p->count_func_param);
        if(p->func) {
            if(p->func->type_params) {
                printf("type1: %s \n", p->func->type_params->data[0]->data);
                printf("type1: %s \n", p->func->type_params->data[1]->data);
            }
        }
        if (p->expression) {
            printf("IN EXP:\n");
            t_exp_list *ptr = p->expression;
            while (ptr) {
                if (ptr->preced_expression_tree) {
                    print_tree(ptr->preced_expression_tree);
                    printf("\n");
                }
                if (ptr->var) {
                    printf("variable: %s\n", ptr->data->data);
                }
                if (ptr->str) {
                    printf("str: %s\n", ptr->data->data);
                }
                if (ptr->integer) {
                    printf("integer: %s\n", ptr->data->data);
                }
                if (ptr->numb) {
                    printf("double: %s\n", ptr->data->data);
                }
                if (ptr->nil) {
                    printf("NIL\n");
                }

                ptr = ptr->next_exp;
            }
        }


        if (p->it_is_variable_) {
            printf("it_is_variable_\n");
        }
        if (p->it_is_variable_expression) {
            printf("it_is_variable_expression\n");
        }
        if (p->it_is_variable_call_function) {
            printf("it_is_variable_call_function\n");
        }
        if (p->it_is_declaration_variable) {
            printf("it_is_declaration_variable\n");
        }
        if (p->it_is_loop) {
            printf("it_is_loop\n");
        }
        if (p->it_is_if) {
            printf("it_is_if\n");
        }
        if (p->it_is_function_define) {
            printf("it_is_function_define\n");
        }
        if (p->it_is_call_function) {
            printf("it_is_call_function\n");
        }
        if (p->if_loop_end) {
            printf("if_loop_end\n");
        }
        if (p->if_else) {
            printf("if_else\n");
        }
        if (p->it_is_in_function) {
            printf("it_is_in_function\n");
        }
        if (p->it_is_return) {
            printf("it_is_return\n");
        }
        if (p->it_is_return_exp) {
            printf("it_is_return_exp\n");
        }
        if (p->it_is_start_if_loop) {
            printf("it_is_start_if_loop\n");
        }
        if (p->it_is_function_end) {
            printf("it_is_function_end\n");
        }
        p = p->next_node;
    }



//
//    if(!program_body){
//       if(string_init(program_body)){
//           return ERROR_INTERNAL;
//       }
//    }
//
//    if(ast_tree->it_is_expression_node){
//        if(code_expression_node(ast_tree))
//            return ERROR_INTERNAL;
//    }
//    else if(ast_tree->it_is_loop){
//        if(code_loop(ast_tree))
//            return ERROR_INTERNAL;
//    }
//    else if(ast_tree->it_is_if){
//        if(code_if(ast_tree))
//            return ERROR_INTERNAL;
//    }
//    else if(ast_tree->it_is_declaration_variable){
//        if(code_declaration_variable(ast_tree))
//            return ERROR_INTERNAL;
//    }
//    else if(ast_tree->it_is_function){
//        if(code_function(ast_tree))
//            return ERROR_INTERNAL;
//    }

    return IT_IS_OK;
}

//const char start[] = ".IFJcode21";
//
//void start_generet(/*tree*/){
//
//}

/**/
/* FJcode21
 *
 * Function start:
 *      PUSHFRAME
 *
 *
 *  While:
 *      ls: -- loop start
 *      LT bool LF@a LF@b -- <  |  JUMPIFEQ lb LF@a LF@b //  == |  JUMPIFNEQ lb LF@a LF@b //  != | GT bool LF@a LF@b -- >
 *      JUMPIFEQ lb bool true   |                               |                                | JUMPIFEQ lb bool true
 *      JUMP lend
 *      lb:
 *
 *      JUMP ls:
 *      lend:
 *
 *  IF:
 *      LT bool LF@a LF@b -- <   |  JUMPIFEQ ift LF@a LF@b //  == |  JUMPIFNEQ ift LF@a LF@b //  != | GT bool LF@a LF@b -- >
 *      JUMPIFEQ ift bool true   |                                |                                 | JUMPIFEQ ift bool true
 *
 *
 *      JUMP ifend
 *      ift:
 *
 *      ifend:
 *
 *
 * ///// function
 *  reads -> LF@_string
 *           READ LF@_string string
 *
 *  write -> WRITE LF@w
 *
 *  substr -> POP LF@str
 *            POP LF@1p
 *            POP LF@2p
 *            DEFVAR LF@char
 *            DEFVAR LF@count_1
 *            MOV LF@count_1 LF@1p
 *            DEFVAR LF@count_2
 *            MOV LF@count_2 0
 *
 *            ls:
 *            LT bool LF@count_1 LF@2p
 *            JUMPIFEQ lb bool true
 *            JUMPIFEQ lb LF@count_1 LF@2p
 *            JUMP lend:
 *            lb:
 *            GETCHAR LF@char LF@str LF@count_1
 *            SETCHAR LF@char LF@count_2 LF@char
 *
 *            ADD LF@count_1 1
 *            ADD LF@count_2 1
 *            JUMP ls
 *            lend:
 *
 */
