
#ifndef IFJ_PROJEKT_STRING_PARAM_H
#define IFJ_PROJEKT_STRING_PARAM_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "string.h"

#define STRING_PARAM_LEN 3  // стандартная длин строки

typedef struct s_param_string{
    t_str** data;
    ull lenght;
    ull how_occupied;
}t_str_param;

int string_param_init(t_str_param* str);
void string_param_free(t_str_param* str);
bool string_param_cmp_string_param(t_str_param* str1, ull numb_p1, t_str_param * str2, ull numb_p2);
bool string_param_cmp_string(t_str_param* str_p, ull numb_p, t_str* str);
#endif //IFJ_PROJEKT_STRING_PARAM_H
