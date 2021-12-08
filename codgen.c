/**
 * Project: Implementace překladače imperativního jazyka IFJ21
 *
 * File:     codgen.c
 * Subject:  IFJ2021
 *
 * @author:  Vladislav Mikheda  	xmikhe00
 * @author:  Khrisanov Vladislav    xkhris00
 * @author:  Kilybayev Anvar        xkilyb00
 * @author:  Gazizov Zhasdauren     xgaziz00
 */



#include "codgen.h"
#include "lexicalanalysis.h"
#include "string_param.h"

#define ADD(string) string_add_arr(code, string);
#define ADD_STRING(string) string_add_string(string, code);

#define ADD_MAIN(string) string_add_arr(main_b, string);
#define ADD_STRING_MAIN(string) string_add_string(string, main_b);

#define ADD_BUILTIN(string) string_add_arr(builtin, string);

#define ADD_TMP(string) string_add_arr(tmp, string);
#define ADD_STRING_TMP(string) string_add_string(string, tmp);

#define ADD_VARS(string) string_add_arr(variables, string);
#define ADD_STRING_VARS(string) string_add_string(string, variables);

#define WHILE_ID_VARS() ADD_VARS("&"); ADD_VARS(int_to_str(while_quantity)); ADD_VARS("%"); ADD_VARS(int_to_str(cnt_while));
#define IF_ID_VARS() ADD_VARS("!"); ADD_VARS(int_to_str(if_quantity)); ADD_VARS("%"); ADD_VARS(int_to_str(cnt_if));

#define WHILE_ID_TMP() ADD_TMP("&"); ADD_TMP(int_to_str(while_quantity)); ADD_TMP("%"); ADD_TMP(int_to_str(cnt_while));
#define IF_ID_TMP() ADD_TMP("!"); ADD_TMP(int_to_str(if_quantity)); ADD_TMP("%"); ADD_TMP(int_to_str(cnt_if));

static t_str *code;
static t_str *main_b;
static t_str *builtin;
static t_str *tmp;
static t_str *variables;

int cnt_if = 0;
int if_quantity = 0;
int cnt_while = 0;
int while_quantity = 0;

int concat_flag = 0;
int strlen_flag = 0;

int reads_flag = 0;
int readi_flag = 0;
int readn_flag = 0;
int write_flag = 0;
int tointeger_flag = 0;
int substr_flag = 0;
int ord_flag = 0;
int chr_flag = 0;

char* int_to_str(int a){
    char *buffer;
    size_t size;
    size = snprintf(NULL, 0, "%d", a);
    if((buffer = (char *)malloc(size + 1)) == NULL){
        return 0;
    }
    snprintf(buffer, size + 1, "%d", a);
    return buffer;
}

char* float_converter(double a){
    char *buffer;
    size_t size;
    size = snprintf(NULL, 0, "%a", a);
    if((buffer = (char *)malloc(size + 1)) == NULL){
        return 0;
    }
    snprintf(buffer, size + 1, "%a", a);
    return buffer;
}

/*
int make_(t_ast_node *ast_tree){

    return IT_IS_OK;
}
*/

/* built-in functions:
*/

int make_reads(){
    ADD_BUILTIN("LABEL reads\n");
    ADD_BUILTIN("PUSHFRAME\n");
    ADD_BUILTIN("DEFVAR LF@retval\n");
    ADD_BUILTIN("READ LF@retval string\n");
    //return:
    ADD_BUILTIN("PUSHS LF@retval\n");
    ADD_BUILTIN("POPFRAME\n");
    ADD_BUILTIN("RETURN\n");
    return IT_IS_OK;
}

int make_readi(){
    ADD_BUILTIN("LABEL readi\n");
    ADD_BUILTIN("PUSHFRAME\n");
    ADD_BUILTIN("DEFVAR LF@retval\n");
    ADD_BUILTIN("READ LF@retval int\n");
    //return:
    ADD_BUILTIN("PUSHS LF@retval\n");
    ADD_BUILTIN("POPFRAME\n");
    ADD_BUILTIN("RETURN\n");
    return IT_IS_OK;
}

int make_readn(){
    ADD_BUILTIN("LABEL readn\n");
    ADD_BUILTIN("PUSHFRAME\n");
    ADD_BUILTIN("DEFVAR LF@retval\n");
    ADD_BUILTIN("READ LF@retval float\n");
    //return:
    ADD_BUILTIN("PUSHS LF@retval\n");
    ADD_BUILTIN("POPFRAME\n");
    ADD_BUILTIN("RETURN\n");
    return IT_IS_OK;
}

int make_write(){
    ADD_BUILTIN("LABEL write\n");
    ADD_BUILTIN("PUSHFRAME\n");
    //parametr:
    ADD_BUILTIN("DEFVAR LF@to_write\n");
    ADD_BUILTIN("POPS LF@to_write\n");
    //check for nil:
    ADD_BUILTIN("DEFVAR LF@type\n");
    ADD_BUILTIN("TYPE LF@type LF@to_write\n");
    ADD_BUILTIN("JUMPIFNEQ write%print LF@type string@nil\n");
    ADD_BUILTIN("WRITE string@nil\n");
    ADD_BUILTIN("LABEL write%print\n");
    //write:
    ADD_BUILTIN("WRITE LF@to_write\n");
    ADD_BUILTIN("POPFRAME\n");
    ADD_BUILTIN("RETURN\n");
    return IT_IS_OK;
}

int make_tointeger(){
    ADD_BUILTIN("LABEL tointeger\n");
    ADD_BUILTIN("PUSHFRAME\n");
    //params:
    ADD_BUILTIN("DEFVAR LF@retval\n");
    ADD_BUILTIN("POPS LF@retval\n");
    //check for nil
    ADD_BUILTIN("DEFVAR LF@type\n");
    ADD_BUILTIN("TYPE LF@type LF@retval\n");
    ADD_BUILTIN("JUMPIFEQ tointeger%end LF@type string@nil\n");
    ADD_BUILTIN("FLOAT2INT LF@retval LF@retval\n");
    ADD_BUILTIN("LABEL tointeger%end\n");
    ADD_BUILTIN("PUSHS LF@retval\n");
    ADD_BUILTIN("POPFRAME\n");
    ADD_BUILTIN("RETURN\n");
    return IT_IS_OK;
}

int make_substr(){
//    ADD_BUILTIN("LABEL substr\n");
//    ADD_BUILTIN("PUSHFRAME\n");
//    //params:
//    ADD_BUILTIN("DEFVAR LF@string\n");
//    ADD_BUILTIN("DEFVAR LF@position_1\n");
//    ADD_BUILTIN("DEFVAR LF@position_2\n");
//    ADD_BUILTIN("POPS LF@position_2\n");
//    ADD_BUILTIN("POPS LF@position_1\n");
//    ADD_BUILTIN("POPS LF@string\n");
//    ADD_BUILTIN("DEFVAR LF@new_string\n");
//    //check if the given positions are valid
//    ADD_BUILTIN("DEFVAR LF@cond\n");
//    ADD_BUILTIN("GT LF@cond LF@position_1 LF@position_2\n");
//    ADD_BUILTIN("JUMPIFEQ substr%out LF@cond bool@true\n");
//    ADD_BUILTIN("JUMPIFEQ substr%out LF@position_1 int@0\n");
//    ADD_BUILTIN("JUMPIFEQ substr%out LF@position_2 int@0\n");
//    ADD_BUILTIN("DEFVAR LF@lenght\n");
//    ADD_BUILTIN("STRLEN LF@lenght LF@string\n");
//    ADD_BUILTIN("DEFVAR LF@cond_a\n");
//    ADD_BUILTIN("DEFVAR LF@cond_b\n");
//    ADD_BUILTIN("GT LF@cond_a LF@position_1 LF@lenght\n");
//    ADD_BUILTIN("GT LF@cond_b LF@position_2 LF@lenght\n");
//    ADD_BUILTIN("OR LF@cond_a LF@cond_a LF@cond_b\n");
//    ADD_BUILTIN("JUMPIFEQ substr%valid LF@cond_a bool@false\n");
//    ADD_BUILTIN("DEFVAR LF@cond\n");
//    ADD_BUILTIN("GT LF@cond LF@position_1 LF@position_2\n");
//    ADD_BUILTIN("JUMPIFEQ substr%out LF@cond bool@true\n");
//    ADD_BUILTIN("DEFVAR LF@lenght\n");
//    ADD_BUILTIN("STRLEN LF@lenght LF@string\n");
//    ADD_BUILTIN("DEFVAR LF@cond_a\n");
//    ADD_BUILTIN("DEFVAR LF@cond_b\n");
//    ADD_BUILTIN("LT LF@cond_a LF@position_1 int@1\n");
//    ADD_BUILTIN("GT LF@cond_b LF@position_1 LF@lenght\n");
//    ADD_BUILTIN("OR LF@cond_a LF@cond_a LF@cond_b\n");
//    ADD_BUILTIN("JUMPIFEQ substr%out LF@cond_a bool@true\n");
//    ADD_BUILTIN("LT LF@cond_a LF@position_2 int@1\n");
//    ADD_BUILTIN("GT LF@cond_b LF@position_2 LF@lenght\n");
//    ADD_BUILTIN("OR LF@cond_a LF@cond_a LF@cond_b\n");
//    ADD_BUILTIN("JUMPIFEQ substr%valid LF@cond_a bool@false\n");
//    //write an empty string:
//    ADD_BUILTIN("LABEL substr%out\n");
//    ADD_BUILTIN("MOVE LF@new_string string@\n");
//    ADD_BUILTIN("JUMP substr%end\n");
//    //valid:
//    ADD_BUILTIN("LABEL substr%valid\n");
//    ADD_BUILTIN("DEFVAR LF@char\n");
//    ADD_BUILTIN("MOVE LF@new_string string@\n");
//    ADD_BUILTIN("SUB LF@position_1 LF@position_1 int@1\n");
//    ADD_BUILTIN("LABEL substr%while\n");
//    ADD_BUILTIN("JUMPIFEQ substr%end LF@position_1 LF@position_2\n");
//    ADD_BUILTIN("GETCHAR LF@char LF@string LF@position_1\n");
//    ADD_BUILTIN("CONCAT LF@new_string LF@new_string LF@char\n");
//    ADD_BUILTIN("ADD LF@position_1 LF@position_1 int@1\n");
//    ADD_BUILTIN("JUMP substr%while\n");
//    ADD_BUILTIN("LABEL substr%end\n");
//    ADD_BUILTIN("PUSHS LF@new_string\n");
//    ADD_BUILTIN("POPFRAME\n");
//    ADD_BUILTIN("RETURN\n");
//    return IT_IS_OK;
//


    ADD_BUILTIN("LABEL substr\n");
    ADD_BUILTIN("PUSHFRAME\n");
    ADD_BUILTIN("DEFVAR LF@string\n");
    ADD_BUILTIN("DEFVAR LF@position_1\n");
    ADD_BUILTIN("DEFVAR LF@position_2\n");
    ADD_BUILTIN("FLOAT2INTS\n");
    ADD_BUILTIN("POPS LF@position_2\n");
    ADD_BUILTIN("FLOAT2INTS\n");
    ADD_BUILTIN("POPS LF@position_1\n");
    ADD_BUILTIN("POPS LF@string\n");
    ADD_BUILTIN("DEFVAR LF@new_string\n");
    ADD_BUILTIN("DEFVAR LF@cond\n");
    ADD_BUILTIN("GT LF@cond LF@position_1 LF@position_2\n");
    ADD_BUILTIN("JUMPIFEQ substr%out LF@cond bool@true\n");
    ADD_BUILTIN("DEFVAR LF@lenght\n");
    ADD_BUILTIN("STRLEN LF@lenght LF@string\n");
    ADD_BUILTIN("DEFVAR LF@cond_a\n");
    ADD_BUILTIN("DEFVAR LF@cond_b\n");
    ADD_BUILTIN("LT LF@cond_a LF@position_1 int@1\n");
    ADD_BUILTIN("GT LF@cond_b LF@position_1 LF@lenght\n");
    ADD_BUILTIN("OR LF@cond_a LF@cond_a LF@cond_b\n");
    ADD_BUILTIN("JUMPIFEQ substr%out LF@cond_a bool@true\n");
    ADD_BUILTIN("LT LF@cond_a LF@position_2 int@1\n");
    ADD_BUILTIN("GT LF@cond_b LF@position_2 LF@lenght\n");
    ADD_BUILTIN("OR LF@cond_a LF@cond_a LF@cond_b\n");
    ADD_BUILTIN("JUMPIFEQ substr%valid LF@cond_a bool@false\n");
    ADD_BUILTIN("LABEL substr%out\n");
    ADD_BUILTIN("MOVE LF@new_string string@\n");
    ADD_BUILTIN("JUMP substr%end\n");
    ADD_BUILTIN("LABEL substr%valid\n");
    ADD_BUILTIN("DEFVAR LF@char\n");
    ADD_BUILTIN("MOVE LF@new_string string@\n");
    ADD_BUILTIN("SUB LF@position_1 LF@position_1 int@1\n");
    ADD_BUILTIN("LABEL substr%while\n");
    ADD_BUILTIN("JUMPIFEQ substr%end LF@position_1 LF@position_2\n");
    ADD_BUILTIN("GETCHAR LF@char LF@string LF@position_1\n");
    ADD_BUILTIN("CONCAT LF@new_string LF@new_string LF@char\n");
    ADD_BUILTIN("ADD LF@position_1 LF@position_1 int@1\n");
    ADD_BUILTIN("JUMP substr%while\n");
    ADD_BUILTIN("LABEL substr%end\n");
    ADD_BUILTIN("PUSHS LF@new_string\n");
    ADD_BUILTIN("POPFRAME\n");
    ADD_BUILTIN("RETURN\n");
    return IT_IS_OK;
}

int make_ord(){
    ADD_BUILTIN("LABEL ord\n");
    ADD_BUILTIN("PUSHFRAME\n");
    //params:
    ADD_BUILTIN("DEFVAR LF@string\n");
    ADD_BUILTIN("DEFVAR LF@position\n");
    ADD_BUILTIN("POPS LF@position\n");
    ADD_BUILTIN("POPS LF@string\n");
    ADD_BUILTIN("DEFVAR LF@lenght\n");
    ADD_BUILTIN("STRLEN LF@lenght LF@string\n");
    //if index > lenght return nil
    ADD_BUILTIN("DEFVAR LF@cond_0\n");
    ADD_BUILTIN("DEFVAR LF@cond_1\n");
    ADD_BUILTIN("GT LF@cond_0 LF@position LF@lenght\n");
    ADD_BUILTIN("LT LF@cond_1 LF@position int@1\n");
    ADD_BUILTIN("OR LF@cond_0 LF@cond_0 LF@cond_1\n");
    ADD_BUILTIN("JUMPIFEQ ord%valid LF@cond_0 bool@false\n");
    ADD_BUILTIN("MOVE LF@string nil@nil\n");
    ADD_BUILTIN("JUMP ord%end\n");
    ADD_BUILTIN("LABEL ord%valid\n");
    ADD_BUILTIN("SUB LF@position LF@position int@1\n");
    ADD_BUILTIN("STRI2INT LF@string LF@string LF@position\n");
    ADD_BUILTIN("LABEL ord%end\n");
    ADD_BUILTIN("PUSHS LF@string\n");
    ADD_BUILTIN("POPFRAME\n");
    ADD_BUILTIN("RETURN\n");
    return IT_IS_OK;
}

int make_chr(){
    ADD_BUILTIN("LABEL chr\n");
    ADD_BUILTIN("PUSHFRAME\n");
    //parametr:
    ADD_BUILTIN("DEFVAR LF@retval\n");
    ADD_BUILTIN("DEFVAR LF@ascii_value\n");
    ADD_BUILTIN("POPS LF@ascii_value\n");
    //check if the value is in range (0, 255)
    ADD_BUILTIN("DEFVAR LF@cond_a\n");
    ADD_BUILTIN("DEFVAR LF@cond_b\n");
    ADD_BUILTIN("LT LF@cond_a LF@ascii_value int@0\n");
    ADD_BUILTIN("GT LF@cond_b LF@ascii_value int@255\n");
    ADD_BUILTIN("OR LF@cond_a LF@cond_a LF@cond_b\n");
    ADD_BUILTIN("JUMPIFEQ chr%valid LF@cond_a bool@false\n");
    ADD_BUILTIN("MOVE LF@retval nil@nil\n");
    ADD_BUILTIN("JUMP chr%end\n");
    //if valid:
    ADD_BUILTIN("LABEL chr%valid\n");
    ADD_BUILTIN("INT2CHAR LF@retval LF@ascii_value\n");
    ADD_BUILTIN("LABEL chr%end\n");
    ADD_BUILTIN("PUSHS LF@retval\n");
    ADD_BUILTIN("POPFRAME\n");
    ADD_BUILTIN("RETURN\n");
    return IT_IS_OK;
}

//spotting built-in functions and adding them to the code:
int spot_builtin_function(t_ast_node *ast_tree){
    if((strcmp(ast_tree->func->name->data, "reads") == 0 && reads_flag == 0)){
        make_reads();
        reads_flag = 1;
    }
    else if((strcmp(ast_tree->func->name->data, "readi") == 0) && readi_flag == 0){
        make_readi();
        readi_flag = 1;
    }
    else if((strcmp(ast_tree->func->name->data, "readn") == 0) && readn_flag == 0){
        make_readn();
        readn_flag = 1;
    }
    else if((strcmp(ast_tree->func->name->data, "tointeger") == 0) && tointeger_flag == 0){
        make_tointeger();
        tointeger_flag = 1;
    }
    else if((strcmp(ast_tree->func->name->data, "write") == 0) && write_flag == 0){
        make_write();
        write_flag = 1;
    }
    else if((strcmp(ast_tree->func->name->data, "substr") == 0) && substr_flag == 0){
        make_substr();
        substr_flag = 1;
    }
    else if((strcmp(ast_tree->func->name->data, "ord") == 0) && ord_flag == 0){
        make_ord();
        ord_flag = 1;
    }
    else if((strcmp(ast_tree->func->name->data, "chr") == 0) && chr_flag == 0){
        make_chr();
        chr_flag = 1;
    }
    return IT_IS_OK;
}

/* modules for calling functions:
*/

int make_call_params(t_ast_node *ast_tree){
    if(ast_tree->expression->integer && !ast_tree->expression->var){
        ADD_TMP("int@");
        ADD_STRING_TMP(ast_tree->expression->data); ADD_TMP("\n");
    }
    else if(ast_tree->expression->numb && !ast_tree->expression->var){
        ADD_TMP("float@");
        ADD_TMP(float_converter(ast_tree->expression->data_double)); ADD_TMP("\n");
    }
    else if(ast_tree->expression->str && !ast_tree->expression->var){
        ADD_TMP("string@");
        ADD_STRING_TMP(ast_tree->expression->data); ADD_TMP("\n");
    }
    else if(ast_tree->expression->nil){
        ADD_TMP("nil@nil\n");
    }
    else if(ast_tree->expression->var){
        ADD_TMP("LF@"); ADD_STRING_TMP(ast_tree->expression->data); ADD_TMP("\n");
    }
    return IT_IS_OK;
}

int make_call_params_main(t_ast_node *ast_tree){
    if(ast_tree->expression->integer && !ast_tree->expression->var){
        ADD_MAIN("int@");
        ADD_STRING_MAIN(ast_tree->expression->data); ADD_MAIN("\n");
    }
    else if(ast_tree->expression->numb && !ast_tree->expression->var){
        ADD_MAIN("float@");
        ADD_MAIN(float_converter(ast_tree->expression->data_double)); ADD_MAIN("\n");
    }
    else if(ast_tree->expression->str && !ast_tree->expression->var){
        ADD_MAIN("string@");
        ADD_STRING_MAIN(ast_tree->expression->data); ADD_MAIN("\n");
    }
    else if(ast_tree->expression->nil){
        ADD_MAIN("nil@nil\n");
    }
    else if(ast_tree->expression->var){
        ADD_STRING_MAIN(ast_tree->expression->data); ADD_MAIN("\n");
    }
    return IT_IS_OK;
}

//calling a function inside of a function:
int make_call(t_ast_node *ast_tree){
    t_ast_node* ptr = malloc(sizeof (t_ast_node));
    ast_init(ptr);
    ptr->expression = ast_tree->expression;
    if(strcmp(ast_tree->func->name->data, "write") == 0){
        for (int i = 0; i < ast_tree->count_func_param; i++){
            ADD_TMP("CREATEFRAME\n");
            ADD_TMP("PUSHS ");
            make_call_params(ptr);
            ADD_TMP("CALL write\n");
            ptr->expression = ptr->expression->next_exp;
        }
    }
    else{
        ADD_TMP("CREATEFRAME\n");
        for (int i = 0; i < ast_tree->func->count_params; i++){
            ADD_TMP("PUSHS ");
            make_call_params(ptr);
            if(string_arr_cmp(ast_tree->func->type_params->data[i],"number")){
                if(ptr->expression->integer){
                    ADD_TMP("INT2FLOATS\n")
                }
            }
            ptr->expression = ptr->expression->next_exp;
        }
        ADD_TMP("CALL "); ADD_TMP(ast_tree->func->name->data); ADD_TMP("\n");
    }
    free(ptr);
    spot_builtin_function(ast_tree);
    return IT_IS_OK;
}

//calling a function in a main body
int make_main_body_call(t_ast_node *ast_tree){
    t_ast_node* ptr = malloc(sizeof (t_ast_node));
    ast_init(ptr);
    ptr->expression = ast_tree->expression;
    if(strcmp(ast_tree->func->name->data, "write") == 0){
        for (int i = 0; i < ast_tree->count_func_param; i++){
            ADD_MAIN("CREATEFRAME\n");
            ADD_MAIN("PUSHS ");
            make_call_params_main(ptr);
            ADD_MAIN("CALL write\n");
            ptr->expression = ptr->expression->next_exp;
        }
    }
    else{
        ADD_MAIN("CREATEFRAME\n");
        for (int i = 0; i < ast_tree->func->count_params; i++){
            ADD_MAIN("PUSHS ");
            make_call_params_main(ptr);
            if(string_arr_cmp(ast_tree->func->params->data[i],"number")) {
                if (ptr->expression->integer) {
                    ADD_MAIN("INT2FLOATS\n");
                }
            }
            ptr->expression = ptr->expression->next_exp;
        }
        ADD_MAIN("CALL "); ADD_STRING_MAIN(ast_tree->func->name); ADD_MAIN("\n");
    }
    free(ptr);
    spot_builtin_function(ast_tree);
    return IT_IS_OK;
}

/* function structure:
*/

int make_function_define(t_ast_node *ast_tree){
    ADD("LABEL "); ADD_STRING(ast_tree->func->name); ADD("\n");
    ADD("PUSHFRAME\n");
    for (int i = 0; i < ast_tree->func->count_params; i++){
        ADD_TMP("DEFVAR LF@"); ADD_STRING_TMP(ast_tree->func->params->data[i]); ADD_TMP("\n");
    }
    for (int i = ast_tree->func->count_params - 1; i >= 0; i--){
        ADD_TMP("POPS LF@"); ADD_STRING_TMP(ast_tree->func->params->data[i]); ADD_TMP("\n");
    }
    return IT_IS_OK;
}

int make_function_end(t_ast_node *ast_tree){
    ADD("LABEL "); ADD_STRING(ast_tree->function_info->name); ADD("%end") ADD("\n");
    ADD("POPFRAME\n");
    ADD("RETURN\n");
    return IT_IS_OK;
}

/* expressions:
*/

int int_to_number(){
    ADD_BUILTIN ("LABEL inttonumber\n");
    ADD_BUILTIN ("CREATEFRAME\n");
    ADD_BUILTIN ("PUSHFRAME\n")
    ADD_BUILTIN ("DEFVAR LF@%check_1%\n")
    ADD_BUILTIN ("DEFVAR LF@%check_2%\n")
    ADD_BUILTIN ("DEFVAR LF@%cond_numb_1%\n")
    ADD_BUILTIN ("DEFVAR LF@%cond_numb_2%\n");
    ADD_BUILTIN ("DEFVAR LF@%type_check_1%\n");
    ADD_BUILTIN ("DEFVAR LF@%type_check_2%\n");
    ADD_BUILTIN ("DEFVAR LF@%cond_numb%\n");
    ADD_BUILTIN ("POPS LF@%check_2%\n");
    ADD_BUILTIN ("POPS LF@%check_1%\n");
    ADD_BUILTIN ("TYPE LF@%type_check_1% LF@%check_1%\n");
    ADD_BUILTIN ("TYPE LF@%type_check_2% LF@%check_2%\n");
    ADD_BUILTIN ("JUMPIFEQ $ok$ LF@%type_check_1% LF@%type_check_2%\n");
    ADD_BUILTIN ("EQ LF@%cond_numb_1% LF@%type_check_1% string@int\n");
    ADD_BUILTIN ("EQ LF@%cond_numb_2% LF@%type_check_2% string@float\n");
    ADD_BUILTIN ("AND LF@%cond_numb% LF@%cond_numb_1% LF@%cond_numb_2%\n");
    ADD_BUILTIN ("JUMPIFEQ $first$ LF@%cond_numb% bool@true\n");
    ADD_BUILTIN ("EQ LF@%cond_numb_1% LF@%type_check_1% string@float\n");
    ADD_BUILTIN ("EQ LF@%cond_numb_2% LF@%type_check_2% string@int\n");
    ADD_BUILTIN ("AND LF@%cond_numb% LF@%cond_numb_1% LF@%cond_numb_2%\n");
    ADD_BUILTIN ("JUMPIFEQ $second$ LF@%cond_numb% bool@true\n");
    ADD_BUILTIN ("LABEL $first$\n");
    ADD_BUILTIN ("INT2FLOAT LF@%check_1% LF@%check_1%\n");
    ADD_BUILTIN ("JUMP $ok$\n");
    ADD_BUILTIN ("LABEL $second$\n");
    ADD_BUILTIN ("INT2FLOAT LF@%check_2% LF@%check_2%\n");
    ADD_BUILTIN ("LABEL $ok$\n");
    ADD_BUILTIN ("PUSHS LF@%check_1%\n");
    ADD_BUILTIN ("PUSHS LF@%check_2%\n");
    ADD_BUILTIN ("POPFRAME\n");
    ADD_BUILTIN ("RETURN\n");
    return IT_IS_OK;
}

int make_expression(AST_leaf *tree ,char *location){
//    static AST_leaf *tree;
//    tree = ast_tree;
    if(tree){
        //recursion:
        make_expression(tree->left, location);
        make_expression(tree->right, location);
        //do:
        if(tree->token->token_name == TOKEN_PLUS){
            ADD_TMP("CREATEFRAME\n");
            ADD_TMP("CALL inttonumber\n");
            ADD_TMP("ADDS\n");
        }
        else if(tree->token->token_name == TOKEN_MINUS){
            ADD_TMP("CREATEFRAME\n");
            ADD_TMP("CALL inttonumber\n");
            ADD_TMP("SUBS\n");
        }
        else if(tree->token->token_name == TOKEN_MULTIPLICATION){
            ADD_TMP("CREATEFRAME\n");
            ADD_TMP("CALL inttonumber\n");
            ADD_TMP("MULS\n");
        }
        else if(tree->token->token_name == TOKEN_DIVISION){
            ADD_TMP("DIVS\n");
        }
        else if(tree->token->token_name == TOKEN_INT_DIVISION){
            ADD_TMP("IDIVS\n");
        }
            //concatenation and strlen:
        else if(tree->token->token_name == TOKEN_CONCATENATION){
            if(concat_flag == 0){
                ADD_VARS("DEFVAR LF@%str_1\n");
                ADD_VARS("DEFVAR LF@%str_2\n");
            }
            ADD_TMP("POPS LF@%str_2\n");
            ADD_TMP("POPS LF@%str_1\n");
            ADD_TMP("CONCAT LF@%str_1 LF@%str_1 LF@%str_2\n");
            ADD_TMP("PUSHS LF@%str_1\n");
            concat_flag = 1;
        }
        else if(tree->token->token_name == TOKEN_LENGTH){
            if(strlen_flag == 0){
                ADD_VARS("DEFVAR LF@%str_len\n");
            }
            ADD_TMP("POPS LF@%str_len\n");
            ADD_TMP("STRLEN LF@%str_len LF@%str_len\n");
            ADD_TMP("PUSHS LF@%str_len\n");
            strlen_flag = 1;
        }
        //relational operators:
        if(tree->token->token_name == TOKEN_GREATER){
            if(strcmp(location, "while") == 0){
                ADD_TMP("CREATEFRAME\n");
                ADD_TMP("CALL inttonumber\n");
                ADD_VARS("DEFVAR LF@*exp_a*"); WHILE_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*exp_b*"); WHILE_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*cond*"); WHILE_ID_VARS(); ADD_VARS("\n");
                ADD_TMP("POPS LF@*exp_b*"); WHILE_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("POPS LF@*exp_a*"); WHILE_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("GT LF@*cond*"); WHILE_ID_TMP(); ADD_TMP(" LF@*exp_a*"); WHILE_ID_TMP(); ADD_TMP(" LF@*exp_b*") WHILE_ID_TMP(); ADD_TMP("\n");
            }
            else if(strcmp(location, "if") == 0){
                ADD_TMP("CREATEFRAME\n");
                ADD_TMP("CALL inttonumber\n");
                ADD_VARS("DEFVAR LF@*exp_a*"); IF_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*exp_b*"); IF_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*cond*"); IF_ID_VARS(); ADD_VARS("\n");
                ADD_TMP("POPS LF@*exp_b*"); IF_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("POPS LF@*exp_a*"); IF_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("GT LF@*cond*"); IF_ID_TMP(); ADD_TMP(" LF@*exp_a*"); IF_ID_TMP(); ADD_TMP(" LF@*exp_b*") IF_ID_TMP(); ADD_TMP("\n");
            }
        }
        else if(tree->token->token_name == TOKEN_GREATER_OR_EQUAL){
            if(strcmp(location, "while") == 0){
                ADD_TMP("CREATEFRAME\n");
                ADD_TMP("CALL inttonumber\n");
                ADD_VARS("DEFVAR LF@*exp_a*"); WHILE_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*exp_b*"); WHILE_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*cond_a*"); WHILE_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*cond_b*"); WHILE_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*cond*"); WHILE_ID_VARS(); ADD_VARS("\n");
                ADD_TMP("POPS LF@*exp_b*"); WHILE_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("POPS LF@*exp_a*"); WHILE_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("GT LF@*cond_a*"); WHILE_ID_TMP(); ADD_TMP(" LF@*exp_a*"); WHILE_ID_TMP(); ADD_TMP(" LF@*exp_b*") WHILE_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("EQ LF@*cond_b*"); WHILE_ID_TMP(); ADD_TMP(" LF@*exp_a*"); WHILE_ID_TMP(); ADD_TMP(" LF@*exp_b*") WHILE_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("OR LF@*cond*"); WHILE_ID_TMP(); ADD_TMP(" LF@*cond_a*"); WHILE_ID_TMP(); ADD_TMP(" LF@*cond_b*") WHILE_ID_TMP(); ADD_TMP("\n");
            }
            else if(strcmp(location, "if") == 0){
                ADD_TMP("CREATEFRAME\n");
                ADD_TMP("CALL inttonumber\n");
                ADD_VARS("DEFVAR LF@*exp_a*"); IF_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*exp_b*"); IF_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*cond_a*"); IF_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*cond_b*"); IF_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*cond*"); IF_ID_VARS(); ADD_VARS("\n");
                ADD_TMP("POPS LF@*exp_b*"); IF_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("POPS LF@*exp_a*"); IF_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("GT LF@*cond_a*"); IF_ID_TMP(); ADD_TMP(" LF@*exp_a*"); IF_ID_TMP(); ADD_TMP(" LF@*exp_b*") IF_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("EQ LF@*cond_b*"); IF_ID_TMP(); ADD_TMP(" LF@*exp_a*"); IF_ID_TMP(); ADD_TMP(" LF@*exp_b*") IF_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("OR LF@*cond*"); IF_ID_TMP(); ADD_TMP(" LF@*cond_a*"); IF_ID_TMP(); ADD_TMP(" LF@*cond_b*") IF_ID_TMP(); ADD_TMP("\n");
            }
        }
        else if(tree->token->token_name == TOKEN_LESS){
            if(strcmp(location, "while") == 0){
                ADD_TMP("CREATEFRAME\n");
                ADD_TMP("CALL inttonumber\n");
                ADD_VARS("DEFVAR LF@*exp_a*"); WHILE_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*exp_b*"); WHILE_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*cond*"); WHILE_ID_VARS(); ADD_VARS("\n");
                ADD_TMP("POPS LF@*exp_b*"); WHILE_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("POPS LF@*exp_a*"); WHILE_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("LT LF@*cond*"); WHILE_ID_TMP(); ADD_TMP(" LF@*exp_a*"); WHILE_ID_TMP(); ADD_TMP(" LF@*exp_b*") WHILE_ID_TMP(); ADD_TMP("\n");
            }
            else if(strcmp(location, "if") == 0){
                ADD_TMP("CREATEFRAME\n");
                ADD_TMP("CALL inttonumber\n");
                ADD_VARS("DEFVAR LF@*exp_a*"); IF_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*exp_b*"); IF_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*cond*"); IF_ID_VARS(); ADD_VARS("\n");
                ADD_TMP("POPS LF@*exp_b*"); IF_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("POPS LF@*exp_a*"); IF_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("LT LF@*cond*"); IF_ID_TMP(); ADD_TMP(" LF@*exp_a*"); IF_ID_TMP(); ADD_TMP(" LF@*exp_b*") IF_ID_TMP(); ADD_TMP("\n");
            }
        }
        else if(tree->token->token_name == TOKEN_LESS_OR_EQUAL){
            if(strcmp(location, "while") == 0){
                ADD_TMP("CREATEFRAME\n");
                ADD_TMP("CALL inttonumber\n");
                ADD_VARS("DEFVAR LF@*exp_a*"); WHILE_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*exp_b*"); WHILE_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*cond_a*"); WHILE_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*cond_b*"); WHILE_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*cond*"); WHILE_ID_VARS(); ADD_VARS("\n");
                ADD_TMP("POPS LF@*exp_b*"); WHILE_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("POPS LF@*exp_a*"); WHILE_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("LT LF@*cond_a*"); WHILE_ID_TMP(); ADD_TMP(" LF@*exp_a*"); WHILE_ID_TMP(); ADD_TMP(" LF@*exp_b*") WHILE_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("EQ LF@*cond_b*"); WHILE_ID_TMP(); ADD_TMP(" LF@*exp_a*"); WHILE_ID_TMP(); ADD_TMP(" LF@*exp_b*") WHILE_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("OR LF@*cond*"); WHILE_ID_TMP(); ADD_TMP(" LF@*cond_a*"); WHILE_ID_TMP(); ADD_TMP(" LF@*cond_b*") WHILE_ID_TMP(); ADD_TMP("\n");
            }
            else if(strcmp(location, "if") == 0){
                ADD_TMP("CREATEFRAME\n");
                ADD_TMP("CALL inttonumber\n");
                ADD_VARS("DEFVAR LF@*exp_a*"); IF_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*exp_b*"); IF_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*cond_a*"); IF_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*cond_b*"); IF_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*cond*"); IF_ID_VARS(); ADD_VARS("\n");
                ADD_TMP("POPS LF@*exp_b*"); IF_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("POPS LF@*exp_a*"); IF_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("LT LF@*cond_a*"); IF_ID_TMP(); ADD_TMP(" LF@*exp_a*"); IF_ID_TMP(); ADD_TMP(" LF@*exp_b*") IF_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("EQ LF@*cond_b*"); IF_ID_TMP(); ADD_TMP(" LF@*exp_a*"); IF_ID_TMP(); ADD_TMP(" LF@*exp_b*") IF_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("OR LF@*cond*"); IF_ID_TMP(); ADD_TMP(" LF@*cond_a*"); IF_ID_TMP(); ADD_TMP(" LF@*cond_b*") IF_ID_TMP(); ADD_TMP("\n");
            }
        }
        else if(tree->token->token_name == TOKEN_EQUALS){
            if(strcmp(location, "while") == 0){
                ADD_TMP("CREATEFRAME\n");
                ADD_TMP("CALL inttonumber\n");
                ADD_VARS("DEFVAR LF@*exp_a*"); WHILE_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*exp_b*"); WHILE_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*cond*"); WHILE_ID_VARS(); ADD_VARS("\n");
                ADD_TMP("POPS LF@*exp_b*"); WHILE_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("POPS LF@*exp_a*"); WHILE_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("EQ LF@*cond*"); WHILE_ID_TMP(); ADD_TMP(" LF@*exp_a*"); WHILE_ID_TMP(); ADD_TMP(" LF@*exp_b*") WHILE_ID_TMP(); ADD_TMP("\n");
            }
            else if(strcmp(location, "if") == 0){
                ADD_TMP("CREATEFRAME\n");
                ADD_TMP("CALL inttonumber\n");
                ADD_VARS("DEFVAR LF@*exp_a*"); IF_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*exp_b*"); IF_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*cond*"); IF_ID_VARS(); ADD_VARS("\n");
                ADD_TMP("POPS LF@*exp_b*"); IF_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("POPS LF@*exp_a*"); IF_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("EQ LF@*cond*"); IF_ID_TMP(); ADD_TMP(" LF@*exp_a*"); IF_ID_TMP(); ADD_TMP(" LF@*exp_b*") IF_ID_TMP(); ADD_TMP("\n");
            }
        }
        else if(tree->token->token_name == TOKEN_NOT_EQUALS){
            if(strcmp(location, "while") == 0){
                ADD_TMP("CREATEFRAME\n");
                ADD_TMP("CALL inttonumber\n");
                ADD_VARS("DEFVAR LF@*exp_a*"); WHILE_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*exp_b*"); WHILE_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*cond*"); WHILE_ID_VARS(); ADD_VARS("\n");
                ADD_TMP("POPS LF@*exp_b*"); WHILE_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("POPS LF@*exp_a*"); WHILE_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("EQ LF@*cond*"); WHILE_ID_TMP(); ADD_TMP(" LF@*exp_a*"); WHILE_ID_TMP(); ADD_TMP(" LF@*exp_b*") WHILE_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("NOT LF@*cond*"); WHILE_ID_TMP(); ADD_TMP(" LF@*cond*"); WHILE_ID_TMP(); ADD_TMP("\n");
            }
            else if(strcmp(location, "if") == 0){
                ADD_TMP("CREATEFRAME\n");
                ADD_TMP("CALL inttonumber\n");
                ADD_VARS("DEFVAR LF@*exp_a*"); IF_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*exp_b*"); IF_ID_VARS(); ADD_VARS("\n");
                ADD_VARS("DEFVAR LF@*cond*"); IF_ID_VARS(); ADD_VARS("\n");
                ADD_TMP("POPS LF@*exp_b*"); IF_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("POPS LF@*exp_a*"); IF_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("EQ LF@*cond*"); IF_ID_TMP(); ADD_TMP(" LF@*exp_a*"); IF_ID_TMP(); ADD_TMP(" LF@*exp_b*") IF_ID_TMP(); ADD_TMP("\n");
                ADD_TMP("NOT LF@*cond*"); IF_ID_TMP(); ADD_TMP(" LF@*cond*"); IF_ID_TMP(); ADD_TMP("\n");
            }
        }
            //variables and constants:
        else if(tree->token->token_name == TOKEN_INTEGER){
            ADD_TMP("PUSHS int@"); ADD_STRING_TMP(tree->token->lexeme->inter); ADD_TMP("\n");
        }
        else if(tree->token->token_name == TOKEN_NUMBER){
            ADD_TMP("PUSHS float@"); ADD_TMP(float_converter(tree->token->lexeme->number)); ADD_TMP("\n");
        }
        else if(tree->token->token_name == TOKEN_NUMBER_EXPONENT){
            ADD_TMP("PUSHS float@"); ADD_TMP(float_converter(tree->token->lexeme->number)); ADD_TMP("\n");
        }
        else if(tree->token->token_name == TOKEN_STRING){
            ADD_TMP("PUSHS string@"); ADD_STRING_TMP(tree->token->lexeme->inter); ADD_TMP("\n");
        }
        else if(tree->token->lexeme->keyword == KEYWORD_NIL){
            ADD_TMP("PUSHS nil@nil\n");
        }
        else if(tree->token->token_name == TOKEN_IDENTIFIER){
            ADD_TMP("PUSHS LF@"); ADD_STRING_TMP(tree->token->lexeme->inter); ADD_TMP("\n");
        }
    }
    return IT_IS_OK;
}
int make_expression_not_tree(t_exp_list* exp_tree){
    if(exp_tree->integer && !exp_tree->var){
        ADD_TMP("PUSHS int@"); ADD_STRING_TMP(exp_tree->data); ADD_TMP("\n");
    }
    else if(exp_tree->numb && !exp_tree->var){
        ADD_TMP("PUSHS float@"); ADD_TMP(float_converter(exp_tree->data_double)); ADD_TMP("\n");
    }
    else if(exp_tree->str && !exp_tree->var){
        ADD_TMP("PUSHS string@"); ADD_STRING_TMP(exp_tree->data); ADD_TMP("\n");
    }
    else if(exp_tree->nil){
        ADD_TMP("PUSHS nil@nil\n");
    }
    else if(exp_tree->var){
        ADD_TMP("PUSHS LF@"); ADD_STRING_TMP(exp_tree->data); ADD_TMP("\n");
    }
    return IT_IS_OK;
}

/* variables:
*/

int make_declaration_variable(t_ast_node *ast_tree){
    //store the name and change string to tmp
    ADD_VARS("DEFVAR LF@"); ADD_STRING_VARS(ast_tree->variable->data[0]); ADD_VARS("\n");
    ADD_VARS("MOVE LF@") ADD_STRING_VARS(ast_tree->variable->data[0]);  ADD_VARS(" nil@nil\n ")
    return IT_IS_OK;
}

int make_variable_declaration_expression(t_ast_node *ast_tree){
    //store the name and change string to tmp
    ADD_VARS("DEFVAR LF@"); ADD_STRING_VARS(ast_tree->variable->data[0]); ADD_VARS("\n");
    if(ast_tree->expression->tree){
        AST_leaf* ptr = ast_tree->expression->preced_expression_tree;
        make_expression(ptr, "return");
    }else{
        make_expression_not_tree(ast_tree->expression);
    }
        ADD_TMP("POPS LF@"); ADD_TMP(ast_tree->variable->data[0]->data); ADD_TMP("\n");
    return IT_IS_OK;
}

int make_variable_declaration_call_function(t_ast_node *ast_tree){
    //store the name and change string to tmp
    ADD_VARS("DEFVAR LF@"); ADD_VARS(ast_tree->variable->data[0]->data); ADD_VARS("\n");
    make_call(ast_tree);
    ADD_TMP("POPS LF@"); ADD_TMP(ast_tree->variable->data[0]->data); ADD_TMP("\n");
    return IT_IS_OK;
}

int make_variable_call_function(t_ast_node *ast_tree){//todo сохраняет не туда
    make_call(ast_tree);
//    ADD_VARS("DEFVAR LF@%return_to_var\n");
//    ADD_TMP("POPS LF@%return_to_var\n");
    for(int i = ast_tree->count_variable -1; i > -1; i--){
        ADD_VARS("DEFVAR LF@%return_to_var"); ADD_VARS(ast_tree->variable->data[i]->data); ADD_VARS("\n");
        ADD_TMP("POPS LF@%return_to_var");ADD_TMP(ast_tree->variable->data[i]->data);  ADD_TMP("\n");
        ADD_TMP("MOVE LF@"); ADD_TMP(ast_tree->variable->data[i]->data); ADD_TMP(" LF@%return_to_var");ADD_TMP(ast_tree->variable->data[i]->data); ADD_TMP("\n");
    }
    return IT_IS_OK;
}

int make_variable_expression(t_ast_node *ast_tree){ //todo поменял
//    t_exp_list* ptr = ast_tree->expression; //todo ???
    t_ast_node* ptr = malloc(sizeof (t_ast_node));
    ast_init(ptr);
    ptr->expression = ast_tree->expression;
    while(ptr->expression) {
        if (ast_tree->expression->tree) {
            make_expression(ptr->expression->preced_expression_tree, "exp");
        }else{
            make_expression_not_tree(ptr->expression);
        }
        ptr->expression = ptr->expression->next_exp;
    }
    free(ptr);
    for(int i = ast_tree->count_variable-1; i > -1; i--){
        ADD_TMP("POPS LF@"); ADD_TMP(ast_tree->variable->data[i]->data); ADD_TMP("\n");
    }
    return IT_IS_OK;
}

/* returning from a function:
*/

int make_return(t_ast_node *ast_tree){
    ADD_TMP("JUMP "); ADD_STRING_TMP(ast_tree->function_info->name); ADD_TMP("%end\n")
    return IT_IS_OK;

}

int make_return_expression(t_ast_node *ast_tree){ //todo поменял
    t_ast_node* ptr = malloc(sizeof (t_ast_node));
    ast_init(ptr);
    int i = 0;
    ptr->expression = ast_tree->expression;
    while(ptr->expression){
        if(ptr->expression->tree){
//            for (int i = 0; i < ast_tree->count_expression; i++) {
                make_expression(ptr->expression->preced_expression_tree, "exp");
//            }
        }
        else{
            if(ptr->expression->nil){
                ADD_TMP("PUSHS nil@nil\n");
            }
            else if(ptr->expression->var){
                ADD_TMP("PUSHS LF@"); ADD_STRING_TMP(ptr->expression->data); ADD_TMP("\n");
                if(string_arr_cmp(ast_tree->function_info->type_returned_params->data[i],"number")) {
                    if (ptr->expression->integer) {
                        ADD_TMP("INT2FLOATS\n");
                    }
                }
            }
            else if(ptr->expression->numb && !(ptr->expression->var)){
                ADD_TMP("PUSHS float@"); ADD_TMP(float_converter(ptr->expression->data_double)); ADD_TMP("\n");
            }
            else if(ptr->expression->integer && !(ptr->expression->var)){
                ADD_TMP("PUSHS int@"); ADD_STRING_TMP(ptr->expression->data); ADD_TMP("\n");
                if(string_arr_cmp(ast_tree->function_info->type_returned_params->data[i],"number")) {
                    if (ptr->expression->integer) {
                        ADD_TMP("INT2FLOATS\n");
                    }
                }
            }
            else if(ptr->expression->data && !(ptr->expression->var)){
                ADD_TMP("PUSHS string@"); ADD_STRING_TMP(ptr->expression->data); ADD_TMP("\n");
            }
        }
        ptr->expression= ptr->expression->next_exp;
        i++;
    }
    free(ptr);
    ADD_TMP("JUMP "); ADD_TMP(ast_tree->function_info->name->data); ADD_TMP("%end\n") //todo та функция в которой он написан ?
    return IT_IS_OK;

}

/* if and while structure:
*/

int make_if_start(t_ast_node *ast_tree){
    t_ast_node* ptr = ast_tree;
    make_expression(ptr->expression->preced_expression_tree, "if"); //todo 11
    ADD_TMP("JUMPIFEQ "); IF_ID_TMP(); ADD_TMP("%if%else "); ADD_TMP("LF@*cond*"); IF_ID_TMP(); ADD_TMP(" bool@false\n");
    return IT_IS_OK;
}

int make_if_else(){
    ADD_TMP("JUMP "); IF_ID_TMP(); ADD_TMP("%if%end\n");
    ADD_TMP("LABEL "); IF_ID_TMP(); ADD_TMP("%if%else\n");
    return IT_IS_OK;
}

int make_if_end(){
    ADD_TMP("LABEL "); IF_ID_TMP(); ADD_TMP("%if%end\n");
    return IT_IS_OK;
}

int make_while_start(t_ast_node *ast_tree){
    t_exp_list* ptr = ast_tree->expression;
    ADD_TMP("LABEL "); WHILE_ID_TMP(); ADD_TMP("%loop\n");
    make_expression(ptr->preced_expression_tree, "while");
    ADD_TMP("JUMPIFEQ "); WHILE_ID_TMP(); ADD_TMP("%loop%end "); ADD_TMP("LF@*cond*"); WHILE_ID_TMP(); ADD_TMP(" bool@false\n");
    return IT_IS_OK;
}

int make_while_end(){
    ADD_TMP("JUMP "); WHILE_ID_TMP(); ADD_TMP("%loop\n");
    ADD_TMP("LABEL "); WHILE_ID_TMP(); ADD_TMP("%loop%end\n");
    return IT_IS_OK;
}

/* executing code generation:
*/

int start_generation(){
    if((code = malloc(sizeof (t_str))) == NULL){
        return ERROR_INTERNAL;
    }
    if((main_b = malloc(sizeof (t_str))) == NULL){
        return ERROR_INTERNAL;
    }
    if((builtin = malloc(sizeof (t_str))) == NULL){
        return ERROR_INTERNAL;
    }
    if((variables = malloc(sizeof (t_str))) == NULL){
        return ERROR_INTERNAL;
    }
    if((tmp = malloc(sizeof (t_str))) == NULL){
        return ERROR_INTERNAL;
    }
    string_init(code);
    string_init(main_b);
    string_init(builtin);
    string_init(variables);
    string_init(tmp);
    ADD(".IFJcode21\nJUMP $$main\n");
    return IT_IS_OK;
}

int code_generation(t_ast_node *ast_tree){
    //main body:
    if(!(ast_tree->it_is_in_function)){
        if(ast_tree->it_is_call_function){
            make_main_body_call(ast_tree);
        }
    }
        //function body:
    else if(ast_tree->it_is_in_function){
        //function parts, calls and returns:
        if(ast_tree->it_is_call_function){
            make_call(ast_tree);
        }
        else if(ast_tree->it_is_function_define){
            make_function_define(ast_tree);
        }
        else if(ast_tree->it_is_return && !(ast_tree->it_is_return_exp)){
            make_return(ast_tree);
        }
        else if(ast_tree->it_is_return_exp){
            make_return_expression(ast_tree);
        }
        else if(ast_tree->it_is_function_end){
            concat_flag = 0;
            strlen_flag = 0;
            //we can deal with duplicates here
            ADD_STRING(variables);
            ADD_STRING(tmp);
            make_function_end(ast_tree);
//            printf("1111%s\n",variables->data);
//            printf("1111%s\n",tmp->data);
            string_wright_arr(variables, "###\n");
            string_wright_arr(tmp, "###\n");
        }
            //variables:
        else if(ast_tree->it_is_declaration_variable && ast_tree->it_is_variable_expression){
            make_variable_declaration_expression(ast_tree);
        }
        else if(ast_tree->it_is_declaration_variable && ast_tree->it_is_variable_call_function){
            make_variable_declaration_call_function(ast_tree);
        }
        else if(ast_tree->it_is_declaration_variable){
            make_declaration_variable(ast_tree);
        }
        else if(ast_tree->it_is_variable_call_function){
            make_variable_call_function(ast_tree);
        }
        else if(ast_tree->it_is_variable_expression){
            make_variable_expression(ast_tree);
        }
            //if:
        else if(ast_tree->it_is_if && ast_tree->it_is_start_if_loop){
            cnt_if++;
            make_if_start(ast_tree);
        }
        else if(ast_tree->if_else){
            make_if_else();
        }
        else if(ast_tree->it_is_if && ast_tree->if_loop_end){
            make_if_end();
            cnt_if--;
            if(cnt_if == 0){
                if_quantity++;
            }
        }
            //while:
        else if(ast_tree->it_is_loop && ast_tree->it_is_start_if_loop){
            cnt_while++;
            make_while_start(ast_tree);
        }

        else if(ast_tree->it_is_loop && ast_tree->if_loop_end){
            make_while_end();
            cnt_while--;
            if(cnt_while == 0){
                while_quantity++;
            }
        }
    }
//    ast_tree = ast_tree->next_node;
    if(ast_tree->next_node){
        code_generation(ast_tree->next_node);
    }
    return IT_IS_OK;
}

int code_assemble(){
    int_to_number();
    ADD_STRING(builtin);
    ADD("LABEL $$main\n");
    ADD_STRING(main_b);
    printf("%s\n",code->data);
    string_free(code);
    string_free(main_b);
    string_free(builtin);
    string_free(variables);
    string_free(tmp);

    return IT_IS_OK;
}

/* freeing dynamic strings:
*/


