
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "lexicalanalysis.h"
#include "syntacticalanalyzer.h"
//#include "main.h"

int main(int argc, char** argv){
//    char arr1[] = "aaaaaaaaaaaaaaaaaaaaaaaaaa";
//    char arr2[] = "123456789sadyftggggggggggggggg";
//    char s1 = 'a';
//    char s2 = 'b';
//
//    t_str *string = malloc(sizeof (t_str));
//    string_init(string);
//    t_str *string2 = malloc(sizeof (t_str));;
//    string_init(string2);
//
//    string_wright_arr(string2, arr2);
//
//    string_wright_arr(string, arr1);
//    string_add_string(string2,string);
//    printf("string = %s\nhow_occupied = %lld \nlenght = %lld \n \n",string->data, string->how_occupied, string->lenght);
//
//    string_wright_arr(string2, arr2);
//    printf("string = %s\nhow_occupied = %lld \nlenght = %lld \n \n",string2->data, string2->how_occupied, string2->lenght);



//    node* gl = NULL;
//    char *a = "aaaaaa";
//    char *b = "aaavaaa";
//    char *c = "aaaaaa";
//    char *d = "aaaaaa";
//    char *e = "aaaaaa";
//    sData *data = malloc(sizeof(sData));
//    data->type = FUNC;
//    data->string_params = malloc( sizeof (t_str));
//    string_init(data->string_params);
//    string_wright_arr(data->string_params, a);
//
//    gl = tree_insert(gl, hashcode(a),data);
//
//    sData *data1 = malloc(sizeof(sData));
//    data1->type = FUNC;
//    data1->string_params = malloc( sizeof (t_str));
//    string_init(data1->string_params);
//    string_wright_arr(data1->string_params, b);
//
//    s_stack *stack = malloc(sizeof (s_stack));
//    table_init(stack);
//
//
//
//    gl = tree_insert(gl, hashcode(b),data1);
//
//    node *n;
//    node *f;
//    n = tree_search(gl,hashcode(a));
//    f = tree_search(gl,hashcode(b));
//
//    printf("%s\n",n->data->string_params->data);
//    printf("%s\n",f->data->string_params->data);
//

/*
 *
 * Проверка лексического анализатора
 *
 */
//    //TODO обработка ошибок с читением имени файла
    FILE* file_name;
    file_name = fopen( argv[1],"r");
    printf("%s\n",argv[1]);
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

//   return start_analysis(token);




    while(token->token_name != TOKEN_EOF){
        token->lexeme->keyword = 100;
        token->lexeme->inter->data[0] = '\0';
        token->lexeme->inter->how_occupied = 0;
        token->lexeme->integer = 0;
        token->lexeme->number = 0.0;

        get_token(token);
        printf("...token{ %d }.....data{ %s }.....keyword{ %d }.....number_int{ %d }.....number_double{ %g }...\n",
               token->token_name,token->lexeme->inter->data,token->lexeme->keyword,token->lexeme->integer,token->lexeme->number);
    }
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

    return 0;
}
