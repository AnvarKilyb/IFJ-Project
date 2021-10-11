#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "lexicalanalysis.c"

int main(int argc, char** argv){
    //TODO обработка ошибок с читением имени файла
    FILE* file_name;
    file_name = fopen( argv[1],"r");
    printf("%s\n",argv[1]);
    t_token* token;
    token = malloc(sizeof (t_token));
    token->lexeme = malloc((sizeof (t_lexeme)));
    token->lexeme->inter = malloc(sizeof (t_str));
    string_init(token->lexeme->inter);
    file_ptr(file_name);

    get_token(token);
    printf("%d\n",token->token_name);
    get_token(token);
    printf("%d\n",token->token_name);
    get_token(token);
    printf("%d\n",token->token_name);
    get_token(token);
    printf("%d\n",token->token_name);
    get_token(token);
    printf("%d\n",token->token_name);

    string_free(token->lexeme->inter);
    free(token->lexeme);
    free(token);

    return 0;
}
