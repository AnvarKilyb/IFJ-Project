/**
 * Project: Implementace překladače imperativního jazyka IFJ21
 *
 * File:     main.c
 * Subject:  IFJ2021
 *
 * @author:  Vladislav Mikheda  	xmikhe00
 * @author:  Khrisanov Vladislav    xkhris00
 * @author:  Kilybayev Anvar        xkilyb00
 * @author:  Gazizov Zhasdauren     xgaziz00
 */



#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lexicalanalysis.h"
#include "syntacticalanalyzer.h"

//#include "main.h"

int main(){

    FILE* file_name = stdin;
//    file_name = fopen( argv[1],"r");
//    printf("%s   %d\n",argv[1],argc);
    t_token* token;
    token = malloc(sizeof (t_token));
    token->lexeme = malloc((sizeof (t_lexeme)));
    token->lexeme->inter = malloc(sizeof (t_str));
    string_init(token->lexeme->inter);
    file_ptr(file_name);

    token->lexeme->keyword = 100;
    token->lexeme->inter->data[0] = '\0';
    token->lexeme->inter->how_occupied = 0;
    token->lexeme->integer = 0;
    token->lexeme->number = 0.0;
    token->str = 1;

    int global_error = 0;

    global_error = start_analysis(token);

    string_free(token->lexeme->inter);
    free(token->lexeme);
    free(token);
    token_free();
    fclose(file_name);
    return global_error;


/////////////////////////////////////////////////////////////
//    while(token->token_name != TOKEN_EOF){
////        token->lexeme->keyword = 100;
////        token->lexeme->inter->data[0] = '\0';
////        token->lexeme->inter->how_occupied = 0;
////        token->lexeme->integer = 0;
////        token->lexeme->number = 0.0;
//
//        if(get_token(token))
//        {
//            return 222;
//        }
//        printf("string = %d",token->str);
//        printf("...token{ %d }.....data{ %s }.....keyword{ %d }.....number_int{ %d }.....number_double{ %g }...\n",
//               token->token_name,token->lexeme->inter->data,token->lexeme->keyword,token->lexeme->integer,token->lexeme->number);

////        get_old_token(token);
////        printf("...token{ %d }.....data{ %s }.....keyword{ %d }.....number_int{ %d }.....number_double{ %g }...\n",
////               token->token_name,token->lexeme->inter->data,token->lexeme->keyword,token->lexeme->integer,token->lexeme->number);
////        get_token(token);
////        printf("...token{ %d }.....data{ %s }.....keyword{ %d }.....number_int{ %d }.....number_double{ %g }...\n",
////               token->token_name,token->lexeme->inter->data,token->lexeme->keyword,token->lexeme->integer,token->lexeme->number);
//    }




//    get_token(token);
//    printf("...>%d<.....>%d<.....>%s<...\n",token->token_name,token->lexeme->keyword,token->lexeme->inter->data);
//    get_token(token);
//    printf("...>%d<.....>%d<.....>%s<...\n",token->token_name,token->lexeme->keyword,token->lexeme->inter->data);
//    get_token(token);
//    printf("...>%d<.....>%d<.....>%s<...\n",token->token_name,token->lexeme->keyword,token->lexeme->inter->data);
//    get_token(token);
//    printf("...>%d<.....>%d<.....>%s<...\n",token->token_name,token->lexeme->keyword,token->lexeme->inter->data);
//    get_token(token);
//    printf("...>%d<.....>%d<.....>%s<...\n",token->token_name,token->lexeme->keyword,token->lexeme->inter->data);

//    string_free(token->lexeme->inter);
//    free(token->lexeme);
//    free(token);


/*
 *
 * Проверка стека
 *
 */
//    t_stack stack;
//    stack_init(&stack);
//    stack_push(&stack,10);
//    printf("%d......%d\n",stack.stack_top->data,stack.amount_of_elements);
//    stack_push(&stack,11);
//    printf("%d......%d\n",stack.stack_top->data,stack.amount_of_elements);
//    stack_push(&stack,12);
//    printf("%d......%d\n",stack.stack_top->data,stack.amount_of_elements);
//    int a = stack_top(&stack);
//    printf("%d\n",a);
//    stack_pop(&stack);
//    printf("%d......%d\n",stack.stack_top->data,stack.amount_of_elements);
//    a = stack_top(&stack);
//    stack_pop(&stack);
//    printf("%d\n",a);
//    printf("%d......%d\n",stack.stack_top->data,stack.amount_of_elements);
//    a = stack_top(&stack);
//    stack_pop(&stack);
//    printf("%d\n",a);
//    stack_pop(&stack);
//    printf("%s......%d\n",stack.stack_top,stack.amount_of_elements);
//    string_free(token->lexeme->inter);
//    free(token->lexeme);
//    free(token);
//    token_free();
//    fclose(file_name);
//
//    return 0;
}
