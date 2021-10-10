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
    t_token token;

    file_ptr(file_name);
    get_token(&token);
    printf("%d\n",token.token_name);
    get_token(&token);
    printf("%d\n",token.token_name);
    get_token(&token);
    printf("%d\n",token.token_name);
    get_token(&token);
    printf("%d\n",token.token_name);
    get_token(&token);
    printf("%d\n",token.token_name);

    return 0;
}
