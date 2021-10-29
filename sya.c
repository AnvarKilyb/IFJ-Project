#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <limits.h>
#include <errno.h>
#include <math.h>
#include "lexicalanalysis.c"
#include "mystack.c"
//#include "error.h"

static t_stack stack;

int token_analysis(t_token *token){
    switch (stack_top(&stack)) {
        case PROG:
            get_token(token);
            if(token->lexeme->keyword == KEYWORD_REQUIRE) {
                get_token(token);
                if (string_arr_cmp(token->lexeme->inter, "\"ifj21\"")) {
                    stack_pop(&stack);
                    stack_push(&stack, CHUNK);
                    printf("IT is OK\n");
                } else
                    return ERROR_SYM_ANALYSIS;
            }
            else
                return ERROR_SYM_ANALYSIS;
    }
}


int  start_analysis(t_token *token){
//    get_token(token);

    stack_init(&stack);
    stack_push(&stack,$);
    stack_push(&stack, PROG);

    token_analysis(token);


}

