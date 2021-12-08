/**
 * Project: Implementace překladače imperativního jazyka IFJ21
 *
 * File:     string_param.c
 * Subject:  IFJ2021
 *
 * @author:  Vladislav Mikheda  	xmikhe00
 * @author:  Khrisanov Vladislav    xkhris00
 * @author:  Kilybayev Anvar        xkilyb00
 * @author:  Gazizov Zhasdauren     xgaziz00
 */



#include "string_param.h"
#include "error.h"


int string_param_init(t_str_param* str)
{
//    t_str* str = malloc(sizeof (t_str));
    str->data = malloc(STRING_LEN * sizeof (t_str*));
    if(!str->data){
        //TODO вызов функции ошибки
        return 99;
    }
    str->lenght = STRING_PARAM_LEN;
    str->how_occupied = 0;
    for(int i = 0; i < STRING_PARAM_LEN; i++){
        str->data[i] = malloc(sizeof (t_str));
        if(!str->data[i]){
            //TODO error
        }
        string_init(str->data[i]);
    }
    return 0;
}

/*
 * Очишение строки
 */
void string_param_free(t_str_param* str){

    for(ull i = 0; i < str->lenght; i++){
        if(str->data[i]) {
//            if(str->data[i]->data)
//                free(str->data[i]->data);
            string_free(str->data[i]);
//            free(str->data[i]);
        }

    }
    free(str->data);
    free(str);
}


/*
 * Увелисиавает строку
 */
int string_param_expansion(t_str_param* str){
    t_str_param string;
    string.data = realloc(str->data, (str->lenght * 2) * sizeof(t_str*));
    if(!string.data){
        return ERROR_INTERNAL;
    }

    str->data = string.data;
    str->lenght = str->lenght * 2;

    for(ull i = str->how_occupied; i < str->lenght; i++){
        str->data[i] = malloc(sizeof (t_str));
        if(!str->data[i]){
            return ERROR_INTERNAL;
        }
        if(string_init(str->data[i])){
            return ERROR_INTERNAL;
        }
    }
    return IT_IS_OK;
}

int string_param_copy_string(t_str_param* str_par, t_str* str){
    if((str_par->lenght) <= str_par->how_occupied){
        if(string_param_expansion(str_par)){
            return ERROR_INTERNAL;
        }
    }
    if(string_copy(str, str_par->data[str_par->how_occupied])){
        return ERROR_INTERNAL;
    }
    str_par->how_occupied++;
    return IT_IS_OK;
}
/*
 * Узнает соответсвует ли первая строка второй строке
 */
bool string_param_cmp_string_param(t_str_param* str1, ull numb_p1, t_str_param * str2, ull numb_p2){
    if(str1->how_occupied <= (numb_p1-1)){
        return false;
    }
    return string_cmp(str1->data[numb_p1-1], str2->data[numb_p2-1]);
}

bool sting_param_uncmp_string(t_str_param* str_param, t_str* str){
    for(ull i = 1; i <= str_param->how_occupied; i++){
        if(string_param_cmp_string(str_param, i, str)){
            return false;
        }
    }
    return true;
}

bool string_param_cmp_arr(t_str_param* str_p, ull numb_p, char* arr){
    if(str_p->how_occupied <= (numb_p-1)){
        return false;
    }
    return string_arr_cmp(str_p->data[numb_p-1],arr);
}


bool string_param_cmp_string(t_str_param* str_p, ull numb_p, t_str* str){
    if(str_p->how_occupied <= (numb_p-1)){
        return false;
    }
    return string_cmp(str_p->data[numb_p-1], str);
}

