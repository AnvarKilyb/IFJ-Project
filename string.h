/**
 * Project: Implementace překladače imperativního jazyka IFJ21
 *
 * File:     string.h
 * Subject:  IFJ2021
 *
 * @author:  Vladislav Mikheda  	xmikhe00
 * @author:  Khrisanov Vladislav    xkhris00
 * @author:  Kilybayev Anvar        xkilyb00
 * @author:  Gazizov Zhasdauren     xgaziz00
 */



#ifndef IFJ_PROJEKT_STRING_H
#define IFJ_PROJEKT_STRING_H

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define STRING_LEN 16  // стандартная длин строки
#define STRING_START 0  // начало строки
#define NUL '\0'        // конец строки


typedef unsigned long long ull;

typedef struct s_string{
    char* data;
    ull lenght;
    ull how_occupied;
}t_str;


/*
 * Создание строки
 */
int string_init(t_str* str);

/*
 * Очишение строки
 */
void string_free(t_str* str);

/*
 * Увелисиавает строку
 */
int string_expansion(t_str* str);

int string_copy(t_str* str1, t_str* str2);



/*
 * Вкладвает символ в строку
 */
int string_wright_char(t_str* str, char symbol);

/*
 * Переписывает масив в строку
 */
int string_wright_arr(t_str* str, char* arr);


/*
 * Возвращает длинну строки
 */
ull string_lenght(t_str *str);
/*
 * Узнает соответсвует ли первая строка второй строке
 */
bool string_cmp(t_str* str1, t_str* str2);

bool string_arr_cmp(t_str* str,const char* arr);
void string_add_string(t_str* str1, t_str* str2);

void string_init_state(t_str* str );
void string_add_arr(t_str* str1, char* arr);

#endif //IFJ_PROJEKT_STRING_H
