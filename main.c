#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

int main(int argc, char** argv){
    //TODO обработка ошибок с читением имени файла
    FILE* file_name = fopen(argv[1],"r");
    printf("%s",argv[1]);
    return 0;
}
