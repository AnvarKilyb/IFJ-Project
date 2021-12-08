/**
 * Project: Implementace překladače imperativního jazyka IFJ21
 *
 * File:     string_papram.h
 * Subject:  IFJ2021
 *
 * @author:  Vladislav Mikheda  	xmikhe00
 * @author:  Khrisanov Vladislav    xkhris00
 * @author:  Kilybayev Anvar        xkilyb00
 * @author:  Gazizov Zhasdauren     xgaziz00
 */


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
int string_param_expansion(t_str_param* str);
void string_param_free(t_str_param* str);
int string_param_copy_string(t_str_param* str_par, t_str* str);
bool string_param_cmp_string_param(t_str_param* str1, ull numb_p1, t_str_param * str2, ull numb_p2);
bool string_param_cmp_string(t_str_param* str_p, ull numb_p, t_str* str);
bool sting_param_uncmp_string(t_str_param* str_param, t_str* str);
bool string_param_cmp_arr(t_str_param* str_p, ull numb_p, char* arr);
#endif //IFJ_PROJEKT_STRING_PARAM_H
