//#include "coden.h"
//#include "lexicalanalysis.h"
//#include "string_param.h"
//
//#define ADD(string) string_add_arr(code, string);
//#define ADD_STRING(string) string_add_string(string, code);
//
//#define ADD_MAIN(string) string_add_arr(main, string);
//#define ADD_STRING_MAIN(string) string_add_string(string, code);
//
//#define ADD_BUILTIN(string) string_add_arr(builtin, string);
//
//#define WHILE_ID() ADD("%"); ADD(int_to_str(while_quantity)); ADD("%"); ADD(int_to_str(cnt_while)); ADD("%while");
//
//#define IF_ID() ADD("%"); ADD(int_to_str(while_quantity)); ADD("%"); ADD(int_to_str(cnt_while)); ADD("%if");
//
//
//static t_str *code;
//static t_str *main;
//static t_str *builtin;
//
//int cnt_if = 0;
//int if_quantity = 0;
//int cnt_while = 0;
//int while_quantity = 0;
//
//int concat_flag = 0;
//int strlen_flag = 0;
//
//int reads_flag = 0;
//int readi_flag = 0;
//int readn_flag = 0;
//int write_flag = 0;
//int tointeger_flag = 0;
//int substr_flag = 0;
//int ord_flag = 0;
//int chr_flag = 0;
//
//char* int_to_str(int a){
//   char *buffer;
//   size_t size;
//   size = snprintf(NULL, 0, "%d", a);
//   if((buffer = (char *)malloc(size + 1)) == NULL){
//      return 0;
//   }
//   snprintf(buffer, size + 1, "%d", a);
//   return buffer;
//}
//
///*
//int make_(t_ast_node *ast_tree){
//
//    return IT_IS_OK;
//}
//*/
//
///* built-in functions:
//*/
//
//int make_reads(){
//    ADD_BUILTIN("LABEL reads\n");
//    ADD_BUILTIN("PUSHFRAME\n");
//    ADD_BUILTIN("DEFVAR LF@retval\n");
//    ADD_BUILTIN("READ LF@retval string\n");
//    //return:
//    ADD_BUILTIN("PUSHS LF@retval\n");
//    ADD_BUILTIN("POPFRAME\n");
//    ADD_BUILTIN("RETURN\n");
//    return IT_IS_OK;
//}
//
//int make_readi(){
//    ADD_BUILTIN("LABEL readi\n");
//    ADD_BUILTIN("PUSHFRAME\n");
//    ADD_BUILTIN("DEFVAR LF@retval\n");
//    ADD_BUILTIN("READ LF@retval int\n");
//    //return:
//    ADD_BUILTIN("PUSHS LF@retval\n");
//    ADD_BUILTIN("POPFRAME\n");
//    ADD_BUILTIN("RETURN\n");
//    return IT_IS_OK;
//}
//
//int make_readn(){
//    ADD_BUILTIN("LABEL readn\n");
//    ADD_BUILTIN("PUSHFRAME\n");
//    ADD_BUILTIN("DEFVAR LF@retval\n");
//    ADD_BUILTIN("READ LF@retval float\n");
//    //return:
//    ADD_BUILTIN("PUSHS LF@retval\n");
//    ADD_BUILTIN("POPFRAME\n");
//    ADD_BUILTIN("RETURN\n");
//    return IT_IS_OK;
//}
//
//int make_write(){
//    ADD_BUILTIN("LABEL write\n");
//    ADD_BUILTIN("PUSHFRAME\n");
//    //parametr:
//    ADD_BUILTIN("DEFVAR LF@to_write\n");
//    ADD_BUILTIN("POPS LF@to_write\n");
//    //check for nil:
//    ADD_BUILTIN("DEFVAR LF@type\n");
//    ADD_BUILTIN("TYPE LF@type LF@to_write\n");
//    ADD_BUILTIN("JUMPIFNEQ write%print LF@type string@nil\n");
//    ADD_BUILTIN("WRITE string@nil\n");
//    ADD_BUILTIN("LABEL write%print\n");
//    //write:
//    ADD_BUILTIN("WRITE LF@to_write\n");
//    ADD_BUILTIN("POPFRAME\n");
//    ADD_BUILTIN("RETURN\n");
//    return IT_IS_OK;
//}
//
//int make_tointeger(){
//    ADD_BUILTIN("LABEL tointeger\n");
//    ADD_BUILTIN("PUSHFRAME\n");
//    //params:
//    ADD_BUILTIN("DEFVAR LF@retval\n");
//    ADD_BUILTIN("POPS LF@retval\n");
//    //check for nil
//    ADD_BUILTIN("DEFVAR LF@type\n");
//    ADD_BUILTIN("TYPE LF@type LF@retval\n");
//    ADD_BUILTIN("JUMPIFEQ tointeger%end LF@type string@nil\n");
//    ADD_BUILTIN("FLOAT2INT LF@retval LF@retval\n");
//    ADD_BUILTIN("LABEL tointeger%end\n");
//    ADD_BUILTIN("PUSHS LF@retval\n");
//    ADD_BUILTIN("POPFRAME\n");
//    ADD_BUILTIN("RETURN\n");
//    return IT_IS_OK;
//}
//
//int make_substr(){
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
//}
//
//int make_ord(){
//    ADD_BUILTIN("LABEL ord\n");
//    ADD_BUILTIN("PUSHFRAME\n");
//    //params:
//    ADD_BUILTIN("DEFVAR LF@string\n");
//    ADD_BUILTIN("DEFVAR LF@position\n");
//    ADD_BUILTIN("POPS LF@position\n");
//    ADD_BUILTIN("POPS LF@string\n");
//    ADD_BUILTIN("DEFVAR LF@lenght\n");
//    ADD_BUILTIN("STRLEN LF@lenght LF@string\n");
//    //if index > lenght return nil
//    ADD_BUILTIN("DEFVAR LF@cond_0\n");
//    ADD_BUILTIN("DEFVAR LF@cond_1\n");
//    ADD_BUILTIN("GT LF@cond_0 LF@position LF@lenght\n");
//    ADD_BUILTIN("EQ LF@cond_1 LF@position int@0\n");
//    ADD_BUILTIN("OR LF@cond_0 LF@cond_0 LF@cond_1\n");
//    ADD_BUILTIN("JUMPIFEQ ord%valid LF@cond_0 bool@false\n");
//    ADD_BUILTIN("MOVE LF@string nil@nil\n");
//    ADD_BUILTIN("JUMP ord%end\n");
//    ADD_BUILTIN("LABEL ord%valid\n");
//    ADD_BUILTIN("SUB LF@position LF@position int@1\n");
//    ADD_BUILTIN("STRI2INT LF@string LF@string LF@position\n");
//    ADD_BUILTIN("LABEL ord%end\n");
//    ADD_BUILTIN("PUSHS LF@string\n");
//    ADD_BUILTIN("POPFRAME\n");
//    ADD_BUILTIN("RETURN\n");
//    return IT_IS_OK;
//}
//
//int make_chr(){
//    ADD_BUILTIN("LABEL chr\n");
//    ADD_BUILTIN("PUSHFRAME\n");
//    //parametr:
//    ADD_BUILTIN("DEFVAR LF@retval\n");
//    ADD_BUILTIN("DEFVAR LF@ascii_value\n");
//    ADD_BUILTIN("POPS LF@ascii_value\n");
//    //check if the value is <= 255
//    ADD_BUILTIN("DEFVAR LF@cond\n");
//    ADD_BUILTIN("GT LF@cond LF@ascii_value int@255\n");
//    ADD_BUILTIN("JUMPIFEQ chr%valid LF@cond bool@false\n");
//    ADD_BUILTIN("MOVE LF@retval nil@nil\n");
//    ADD_BUILTIN("JUMP chr%end\n");
//    //if valid:
//    ADD_BUILTIN("LABEL chr%valid\n");
//    ADD_BUILTIN("INT2CHAR LF@retval LF@ascii_value\n");
//    ADD_BUILTIN("LABEL chr%end\n");
//    ADD_BUILTIN("PUSHS LF@retval\n");
//    ADD_BUILTIN("POPFRAME\n");
//    ADD_BUILTIN("RETURN\n");
//    return IT_IS_OK;
//}
//
////spotting built-in functions and adding them to the code:
//int spot_builtin_function(t_ast_node *ast_tree){
//    if((strcmp(ast_tree->func->name->data, "reads") == 0 && reads_flag == 0)){
//        make_reads();
//        reads_flag = 1;
//    }
//    else if((strcmp(ast_tree->func->name->data, "readi") == 0) && readi_flag == 0){
//        make_readi();
//        readi_flag = 1;
//    }
//    else if((strcmp(ast_tree->func->name->data, "readn") == 0) && readn_flag == 0){
//        make_readn();
//        readn_flag = 1;
//    }
//    else if((strcmp(ast_tree->func->name->data, "write") == 0) && write_flag == 0){
//        make_write();
//        write_flag = 1;
//    }
//    else if((strcmp(ast_tree->func->name->data, "substr") == 0) && substr_flag == 0){
//        make_substr();
//        substr_flag = 1;
//    }
//    else if((strcmp(ast_tree->func->name->data, "ord") == 0) && ord_flag == 0){
//        make_ord();
//        ord_flag = 1;
//    }
//    else if((strcmp(ast_tree->func->name->data, "chr") == 0) && chr_flag == 0){
//        make_chr();
//        chr_flag = 1;
//    }
//    return IT_IS_OK;
//}
//
///* modules for calling functions:
//*/
//
//int make_call_params(t_ast_node *ast_tree){
//    if(ast_tree->expression->integer){
//        ADD("int@");
//        ADD_STRING(ast_tree->expression->data); ADD("\n");
//    }
//    else if(ast_tree->expression->numb){
//        ADD("float@");
//        ADD_STRING(ast_tree->expression->data); ADD("\n");
//    }
//    else if(ast_tree->expression->str){
//        ADD("string@");
//        ADD_STRING(ast_tree->expression->data); ADD("\n");
//    }
//    else if(ast_tree->expression->nil){
//        ADD("nil@nil\n");
//    }
//    else if(ast_tree->expression->var){
//        ADD_STRING(ast_tree->expression->data); ADD("\n");
//    }
//    return IT_IS_OK;
//}
//
//int make_call_params_main(t_ast_node *ast_tree){
//    if(ast_tree->expression->integer){
//        ADD("int@");
//        ADD_STRING_MAIN(ast_tree->expression->data); ADD("\n");
//    }
//    else if(ast_tree->expression->numb){
//        ADD("float@");
//        ADD_STRING_MAIN(ast_tree->expression->data); ADD("\n");
//    }
//    else if(ast_tree->expression->str){
//        ADD("string@");
//        ADD_STRING_MAIN(ast_tree->expression->data); ADD("\n");
//    }
//    else if(ast_tree->expression->nil){
//        ADD("nil@nil\n");
//    }
//    else if(ast_tree->expression->var){
//        ADD_STRING_MAIN(ast_tree->expression->data); ADD("\n");
//    }
//    return IT_IS_OK;
//}
//
////calling a function inside of a function:
//int make_call(t_ast_node *ast_tree){
//   if(strcmp(ast_tree->func->name->data, "write") == 0){
//       for (int i = 0; i < ast_tree->func->count_params; i++){
//           ADD("CREATEFRAME\n");
//           ADD("PUSHS ");
//           make_call_params(ast_tree);
//           ADD("CALL write\n");
//       }
//   }
//   else{
//       ADD("CREATEFRAME\n");
//       for (int i = 0; i < ast_tree->func->count_params; i++){
//           ADD("PUSHS ");
//           make_call_params(ast_tree);
//       }
//       ADD("CALL "); ADD_STRING(ast_tree->func->name->data); ADD("\n");
//   }
//   spot_builtin_function(ast_tree);
//   return IT_IS_OK;
//}
//
////calling a function in a main body
//int make_main_body_call(t_ast_node *ast_tree){
//   if(strcmp(ast_tree->func->name->data, "write") == 0){
//       for (int i = 0; i < ast_tree->func->count_params; i++){
//           ADD_MAIN("CREATEFRAME\n");
//           ADD_MAIN("PUSHS ");
//           make_call_params_main(ast_tree);
//           ADD_MAIN("CALL write\n");
//       }
//   }
//   else{
//       ADD_MAIN("CREATEFRAME\n");
//       for (int i = 0; i < ast_tree->func->count_params; i++){
//           ADD_MAIN("PUSHS ");
//           make_call_params_main(ast_tree);
//       }
//       ADD_MAIN("CALL "); ADD_STRING_MAIN(ast_tree->func->name->data); ADD_MAIN("\n");
//   }
//   spot_builtin_function(ast_tree);
//   return IT_IS_OK;
//}
//
///* function structure:
//*/
//
//int make_function_define(t_ast_node *ast_tree){
////    ADD("LABEL "); ADD_STRING(ast_tree->func->name->data); ADD("\n");
////    ADD("PUSHFRAME\n");
////    for (int i = 0; i < ast_tree->func->count_params; i++){
////        ADD("DEFVAR LF@"); ADD_STRING(ast_tree->func->params[i]->data); ADD("\n");
////    }
////    for (int i = ast_tree->func->count_params - 1; i >= 0; i--){
////        ADD("POPS LF@"); ADD_STRING(ast_tree->func->params); ADD("\n");
////    }
//    return IT_IS_OK;
//}
//
//int make_function_end(t_ast_node *ast_tree){
////    ADD("LABEL "); ADD_STRING(ast_tree->func->name->data); ADD("%end") ADD("\n");
////    ADD("POPFRAME\n");
////    ADD("RETURN\n");
//    return IT_IS_OK;
//}
//
///* expressions:
//*/
//
//int make_expression(t_ast_node *ast_tree, char *location){
//    static AST_leaf *tree;
//    tree = ast_tree->expression->preced_expression_tree;
//    if(tree){
//        //recursion:
////        expression(tree->left);
////        expression(tree->right);
//        //do:
//        if(tree->token->token_name == TOKEN_PLUS){
//            ADD("PUSHS ADDS\n");
//        }
//        else if(tree->token->token_name == TOKEN_MINUS){
//            ADD("PUSHS SUBS\n");
//        }
//        else if(tree->token->token_name == TOKEN_MULTIPLICATION){
//            ADD("PUSHS MULS\n");
//        }
//        else if(tree->token->token_name == TOKEN_DIVISION){
//            ADD("PUSHS DIVS\n");
//        }
//        else if(tree->token->token_name == TOKEN_INT_DIVISION){
//            ADD("PUSHS IDIVS\n");
//        }
//        //concatenation and strlen:
//        else if(tree->token->token_name == TOKEN_CONCATENATION){
//            if(concat_flag == 0){
//                ADD("DEFVAR LF@%str_1\n");
//                ADD("DEFVAR LF@%str_2\n");
//            }
//            ADD("POPS LF@str_2\n");
//            ADD("POPS LF@str_1\n");
//            ADD("CONCAT LF@str_1 LF@str_1 LF@str_2\n");
//            ADD("PUSHS LF@str_1\n");
//            concat_flag = 1;
//        }
//        else if(tree->token->token_name == TOKEN_LENGTH){
//            if(strlen_flag == 0){
//               ADD("DEFVAR LF@%str_len\n");
//            }
//            ADD("POPS LF@str_len\n");
//            ADD("STRLEN LF@str_len LF@str_len\n");
//            ADD("PUSHS LF@str_len\n");
//            strlen_flag = 1;
//        }
//        //relational operators:
//        if(tree->token->token_name == TOKEN_GREATER){
//            if(strcmp(location, "while") == 0){
//                ADD("DEFVAR LF@exp_1"); WHILE_ID(); ADD("\n");
//                ADD("DEFVAR LF@exp_2"); WHILE_ID(); ADD("\n");
//                ADD("POPS LF@exp_2"); WHILE_ID(); ADD("\n");
//                ADD("POPS LF@exp_1"); WHILE_ID(); ADD("\n");
//                ADD("DEFVAR LF@cond"); WHILE_ID(); ADD("\n");
//                ADD("GT LF@cond"); WHILE_ID(); ADD("LF@exp_1 "); ADD("LF@exp_2") ADD("\n");
//            }
//            if(strcmp(location, "if") == 0){
//                ADD("DEFVAR LF@exp_1"); IF_ID(); ADD("\n");
//                ADD("DEFVAR LF@exp_2"); IF_ID(); ADD("\n");
//                ADD("POPS LF@exp_2"); IF_ID(); ADD("\n");
//                ADD("POPS LF@exp_1"); IF_ID(); ADD("\n");
//                ADD("DEFVAR LF@cond"); IF_ID(); ADD("\n");
//                ADD("GT LF@cond"); IF_ID(); ADD("LF@exp_1 "); ADD("LF@exp_2") ADD("\n");
//            }
//        }
//        else if(tree->token->token_name == TOKEN_GREATER_OR_EQUAL){
//          //add  char *id + char *part
//          if(strcmp(location, "while") == 0){
//            ADD("DEFVAR LF@%cond_0\n");
//            ADD("DEFVAR LF@%cond_1\n");
//            //ADD(int_to_str(while_quantity)); ADD("%"); ADD(int_to_str(cnt_while)); ADD("%while\n");
//            //while stuff
//          }
//          if(strcmp(location, "if") == 0){
//            ADD("DEFVAR LF@%cond_0\n");
//            ADD("DEFVAR LF@%cond_1\n");
//            //if stuff
//          }
////          GT LF@%cond_0 LF@expression_1 LF@expression_2
////          EQ LF@%cond_1 LF@expression_1 LF@expression_2
//          ADD("OR LF@%cond_0 LF@%cond_0 LF@%cond_1\n");
//        }
//        else if(tree->token->token_name == TOKEN_LESS){
//          if(strcmp(location, "while") == 0){
//            ADD("DEFVAR LF@%cond_0\n");
//
//            //ADD(int_to_str(while_quantity)); ADD("%"); ADD(int_to_str(cnt_while)); ADD("%while\n");
//            //while stuff
//          }
//            if(strcmp(location, "if") == 0){
//            ADD("DEFVAR LF@%cond_0\n");
//            //if stuff
//            }
////          LT LF@%cond_0 LF@expression_1 LF@expression_2
//        }
//        else if(tree->token->token_name == TOKEN_LESS_OR_EQUAL){
//          //add  char *id + char *part
//          if(strcmp(location, "while") == 0){
//            ADD("DEFVAR LF@%cond_0\n");
//            ADD("DEFVAR LF@%cond_1\n");
//            //ADD(int_to_str(while_quantity)); ADD("%"); ADD(int_to_str(cnt_while)); ADD("%while\n");
//            //while stuff
//          }
//          if(strcmp(location, "if") == 0){
//            ADD("DEFVAR LF@%cond_0\n");
//            ADD("DEFVAR LF@%cond_1\n");
//            //if stuff
//          }
////          LT LF@%cond_0 LF@expression_1 LF@expression_2
////          EQ LF@%cond_1 LF@expression_1 LF@expression_2
//          ADD("OR LF@%cond_0 LF@%cond_0 LF@%cond_1\n");
//        }
//        else if(tree->token->token_name == TOKEN_EQUALS){
//          if(strcmp(location, "while") == 0){
//            ADD("DEFVAR LF@%cond_0\n");
//
//            //ADD(int_to_str(while_quantity)); ADD("%"); ADD(int_to_str(cnt_while)); ADD("%while\n");
//            //while stuff
//          }
//            if(strcmp(location, "if") == 0){
//            ADD("DEFVAR LF@%cond_0\n");
//            //if stuff
//            }
////          EQ LF@%cond_0 LF@expression_1 LF@expression_2
//
//        }
//        else if(tree->token->token_name == TOKEN_NOT_EQUALS){
//          if(strcmp(location, "while") == 0){
//            ADD("DEFVAR LF@%cond_0\n");
//
//            //ADD(int_to_str(while_quantity)); ADD("%"); ADD(int_to_str(cnt_while)); ADD("%while\n");
//            //while stuff
//          }
//            if(strcmp(location, "if") == 0){
//            ADD("DEFVAR LF@%cond_0\n");
//            //if stuff
//            }
////          EQ LF@%cond_0 LF@expression_1 LF@expression_2
//          ADD("NOT LF@%cond_0 LF@%cond_0\n");
//
//        }
//        //variables and constants:
//        else if(tree->token->token_name == TOKEN_INTEGER){
//            ADD("PUSHS int@"); ADD_STRING(tree->token->lexeme); ADD("\n");
//        }
//        else if(tree->token->token_name == TOKEN_NUMBER){
//            //todo conversion
//            ADD("PUSHS number@"); ADD_STRING(tree->token->lexeme); ADD("\n");
//        }
//        else if(tree->token->token_name == TOKEN_NUMBER_EXPONENT){
//            //todo conversion
//            ADD("PUSHS number@"); ADD_STRING(tree->token->lexeme); ADD("\n");
//        }
//        else if(tree->token->token_name == TOKEN_STRING){
//            ADD("PUSHS @"); ADD_STRING(tree->token->lexeme); ADD("\n");
//        }
//        else if(tree->token->token_name == NONE_T){
//            ADD("PUSHS nil@nil\n");
//            //is that nil?
//        }
//        else if(tree->token->token_name == TOKEN_IDENTIFIER){
//            //how to get this?
//        }
//    }
//    return IT_IS_OK;
//}
//
///* variables:
//*/
//
//int make_declaration_variable(t_ast_node *ast_tree){
////    DEFVAR var_name
//    return IT_IS_OK;
//}
//
//int make_variable_call_function(t_ast_node *ast_tree){
//
//    return IT_IS_OK;
//}
//
//int make_variable_expression(t_ast_node *ast_tree){
//
//    return IT_IS_OK;
//}
//
//int make_variable_declaration_expression(t_ast_node *ast_tree){
////    DEFVAR var_name
//    //how to get expression values?
//    return IT_IS_OK;
//}
//
//int make_variable_declaration_call_function(t_ast_node *ast_tree){
////    DEFVAR var_name
////    make_call(ast_tree);
////    POPS our return to var_name
//    return IT_IS_OK;
//}
//
// /* returning from a function:
// */
//
///*
//int make_return(t_ast_node *ast_tree){
//    ADD("JUMP "); ADD_STRING(ast_tree->func->name->data); ADD("%end\n")
//    return IT_IS_OK;
//}
//
//int make_return_expression(t_ast_node *ast_tree){
//    for (int i = 0; i < ast_tree->func->count_returned_params; i++) {
//        ast_tree->func->type_returned_params->data[i]->data
//
//    }
//    make_return(ast_tree);
//    return IT_IS_OK;
//}
//
//int make_get_retval(t_ast_node *ast_tree){
//    //we can only put return into a variable(s) so:
//    DEFVAR LF@special_name
//    POPS LF@special_name
//    for(amount of params){
//        MOVE param[i] LF@special_name
//    }
//    return IT_IS_OK;
//}
//*/
//
///* if and while structure:
//*/
//
//int make_if_start(t_ast_node *ast_tree){
//
//    return IT_IS_OK;
//}
//
//int make_if_else(t_ast_node *ast_tree){
//    //JUMP /*add count % or $*/if_end
//    //LABEL /*add count % or $*/if_else
//    return IT_IS_OK;
//}
//
//int make_if_end(t_ast_node *ast_tree){
//    //LABEL /*add count % or $*/if_end
//    return IT_IS_OK;
//}
//
//int make_while_start(t_ast_node *ast_tree){
//  // ADD FUNCTION NAME ALSO TO THE LABEL
////  we can use stack to check variables
////    ADD("LABEL "); ADD(int_to_str(while_quantity)); ADD("%"); ADD(int_to_str(cnt_while)); ADD("%while\n");
////    make_expression(ast_tree, "while");
//    //ADD("JUMPIFEQ "); ADD(int_to_str(cnt_while)); ADD("%"); ADD(int_to_str(while_quantity)); ADD("%while%end");
//    //
//    return IT_IS_OK;
//}
//
//int make_while_end(t_ast_node *ast_tree){
//    ADD("JUMP "); ADD(int_to_str(cnt_while)); ADD("%"); ADD(int_to_str(while_quantity)); ADD("%while\n");
//    ADD("LABEL "); ADD(int_to_str(cnt_while)); ADD("%"); ADD(int_to_str(while_quantity)); ADD("%while%end\n");
//    return IT_IS_OK;
//}
//
///* dealing with duplicates:
//*/
//
///*
//int remember_variable(t_ast_node *ast_tree){
//
//    return IT_IS_OK;
//}
//
//int process_duplicates_in_exp(t_ast_node *ast_tree){
//
//   return IT_IS_OK;
//}
//
//int process_duplicates_in_params(t_ast_node *ast_tree){
//
//   return IT_IS_OK;
//}
//
//int process_duplicates_in_vars(t_ast_node *ast_tree){
//
//   return IT_IS_OK;
//}
//
//int process_duplicates_in_conditions(t_ast_node *ast_tree){
//
//   return IT_IS_OK;
//}
//*/
//
///* executing code generation:
//*/
//
//int start_generation(){
////    if((code = malloc(sizeof (t_str))) == NULL){
////        return ERROR_INTERNAL;
////    }
////    if((main = malloc(sizeof (t_str))) == NULL){
////        return ERROR_INTERNAL;
////    }
////    if((builtin = malloc(sizeof (t_str))) == NULL){
////        return ERROR_INTERNAL;
////    }
////    string_init(code);
////    string_init(main);
////    string_init(builtin);
////    string_init(unique_id);
////    ADD(".IFJcode21\nJUMP $$main\n");
//    return IT_IS_OK;
//}
//
//int code_generation(t_ast_node *ast_tree){
//    //main body:
//    if(!(ast_tree->it_is_in_function)){
//        if(ast_tree->it_is_call_function){
//            make_main_body_call(ast_tree);
//        }
//    }
//    //function body:
//    else if(ast_tree->it_is_in_function){
//        //function parts, calls and returns:
//        if(ast_tree->it_is_call_function){
//            make_call(ast_tree);
//        }
//        else if(ast_tree->it_is_function_define){
//            make_function_define(ast_tree);
//        }
//        else if(ast_tree->it_is_return && !(ast_tree->it_is_return_exp)){
////            make_return(ast_tree);
//        }
//        else if(ast_tree->it_is_return_exp){
////            make_return_expression(ast_tree);
//        }
//        else if(ast_tree->it_is_function_end){
//            make_function_end(ast_tree);
//            concat_flag = 0;
//            strlen_flag = 0;
//        }
//        //variables:
//        else if(ast_tree->it_is_declaration_variable && ast_tree->it_is_variable_expression){
////            make_variable_declaration_expression();
//        }
//        else if(ast_tree->it_is_declaration_variable && ast_tree->it_is_variable_call_function){
////            make_variable_declaration_call_function();
//        }
//        else if(ast_tree->it_is_declaration_variable){
////            make_declaration_variable();
//        }
//        else if(ast_tree->it_is_variable_call_function){
////            make_variable_call_function();
//        }
//        else if(ast_tree->it_is_variable_expression){
////            make_variable_expression();
//        }
//
//        //if:
//        else if(ast_tree->it_is_if && ast_tree->it_is_start_if_loop){
//            cnt_if++;
//            make_if_start(ast_tree);
//        }
//        else if(ast_tree->if_else){
//            make_if_else(ast_tree);
//        }
//        else if(ast_tree->it_is_if && ast_tree->if_loop_end){
//            make_if_end(ast_tree);
//            cnt_if--;
//            if(cnt_while == 0){
//                if_quantity++;
//            }
//        }
//        //while:
//        else if(ast_tree->it_is_loop && ast_tree->it_is_start_if_loop){
//            cnt_while++;
////            make_while_start();
//        }
//
//        else if(ast_tree->it_is_loop && ast_tree->if_loop_end){
////            make_while_end();
//            cnt_while--;
//            if(cnt_while == 0){
//                while_quantity++;
//            }
//        }
//    }
//    return IT_IS_OK;
//}
//
//int code_assemble(){
//    ADD_STRING(builtin);
//    ADD("LABEL $$main\n");
//    ADD_STRING(main);
//    return IT_IS_OK;
//}
//
///* freeing dynamic strings:
//*/
//
////string_free(code);
////string_free(main);
////string_free(builtin);
