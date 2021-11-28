
#include "codgen.h"
char *header = ".IFJcode21";
static t_str *program_body;
static t_str *program_header;

int code_header(){
    program_header = malloc(sizeof (t_str));
    if(string_init(program_header))
        return ERROR_INTERNAL;

    string_wright_arr(program_header, header);
    return IT_IS_OK;
}
int code_expression_node(t_ast_node *ast_tree){
    return IT_IS_OK;
}
int code_loop(t_ast_node *ast_tree){
    return IT_IS_OK;
}
int code_if(t_ast_node *ast_tree){
    return IT_IS_OK;
}
int code_declaration_variable(t_ast_node *ast_tree){
    return IT_IS_OK;
}

int code_function(t_ast_node *ast_tree){
    return IT_IS_OK;
}
int code_gen(t_ast_node *ast_tree){
    if(!program_body){
       if(string_init(program_body)){
           return ERROR_INTERNAL;
       }
    }

    if(ast_tree->it_is_expression_node){
        if(code_expression_node(ast_tree))
            return ERROR_INTERNAL;
    }
    else if(ast_tree->it_is_loop){
        if(code_loop(ast_tree))
            return ERROR_INTERNAL;
    }
    else if(ast_tree->it_is_if){
        if(code_if(ast_tree))
            return ERROR_INTERNAL;
    }
    else if(ast_tree->it_is_declaration_variable){
        if(code_declaration_variable(ast_tree))
            return ERROR_INTERNAL;
    }
    else if(ast_tree->it_is_function){
        if(code_function(ast_tree))
            return ERROR_INTERNAL;
    }

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
