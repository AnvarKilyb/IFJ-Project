
#include "codgen.h"
char *header = ".IFJcode21";
static t_str *program_body;
static t_str *program_header;


void print_tree(AST_leaf* tree){
    if(tree){
        print_tree(tree->left);
        print_tree(tree->right);
        printf("%s",tree->token->lexeme->inter->data);
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
    printf("-------------------------------------------------------------------------------\n");
    if(ast_tree->variable || ast_tree->type_variable) {
        printf("variable: ");
       if(ast_tree->type_variable) {
            for (int i = 0; i < ast_tree->count_variable; i++) {
                printf("%s  ", ast_tree->variable->data[i]->data);
            }
        }
        if(ast_tree->type_variable) {
            if (ast_tree->type_variable) {
                for (int i = 0; i < ast_tree->count_variable; i++) {
                    printf("%s  ", ast_tree->type_variable->data[i]->data);
                }
                printf("\n");
            }
        }
    }
    if(ast_tree->count_variable != 0){
        printf("count_variable: %d\n",ast_tree->count_variable);
    }
    if(ast_tree->func){
        printf("%s  ", ast_tree->func->name->data);
        printf("%s  ", ast_tree->func->type->data);
        printf("\t");
        for (int i = 0; i < ast_tree->func->count_params; i++) {
            printf("%s  ", ast_tree->func->params->data[i]->data);
        }
        for (int i = 0; i < ast_tree->func->count_params; i++) {
            printf("%s  ", ast_tree->func->type_params->data[i]->data);
        }

        if(ast_tree->func->type_returned_params){
            printf(":");
            for (int i = 0; i < ast_tree->func->count_returned_params; i++) {
                printf("%s  ", ast_tree->func->type_returned_params->data[i]->data);
            }
        }
        printf("\n");
    }

    if(ast_tree->expression){
        printf("IN EXP:\n");
        t_exp_list* ptr = ast_tree->expression;
        while(ptr){
            if(ptr->preced_expression_tree){
                print_tree(ptr->preced_expression_tree);
                printf("\n");
            }
            if(ptr->variable){
                printf("variable: %s\n",ptr->variable->data);
            }
            if(ptr->str){
                printf("str: %s\n",ptr->data_string->data);
            }
            if(ptr->integer){
                printf("integer: %d\n",ptr->data_int);
            }
            if(ptr->numb){
                printf("double: %f\n",ptr->data_double);
            }
            if(ptr->nil){
                printf("NIL\n");
            }

            ptr = ptr->next_exp;
        }
    }


    if(ast_tree->it_is_variable_){
        printf("it_is_variable_\n");
    }
    if(ast_tree->it_is_variable_expression){
        printf("it_is_variable_expression\n");
    }
    if(ast_tree->it_is_variable_call_function){
        printf("it_is_variable_call_function\n");
    }
    if(ast_tree->it_is_declaration_variable){
        printf("it_is_declaration_variable\n");
    }
    if(ast_tree->it_is_loop){
        printf("it_is_loop\n");
    }
    if(ast_tree->it_is_if){
        printf("it_is_if\n");
    }
    if(ast_tree->it_is_function_define){
        printf("it_is_function_define\n");
    }
    if(ast_tree->it_is_call_function){
        printf("it_is_call_function\n");
    }
    if(ast_tree->if_loop_end){
        printf("if_loop_end\n");
    }
    if(ast_tree->if_else){
        printf("if_else\n");
    }
    if(ast_tree->it_is_in_function){
        printf("it_is_in_function\n");
    }
    if(ast_tree->it_is_return){
        printf("it_is_return\n");
    }
    if(ast_tree->it_is_return_exp){
        printf("it_is_return_exp\n");
    }
    if(ast_tree->it_is_start_if_loop){
        printf("it_is_start_if_loop\n");
    }
    if(ast_tree->it_is_function_end){
        printf("it_is_function_end\n");
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
