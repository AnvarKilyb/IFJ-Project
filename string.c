/**
 * Project: Implementace překladače imperativního jazyka IFJ21
 *
 * File:     string.c
 * Subject:  IFJ2021
 *
 * @author:  Vladislav Mikheda  	xmikhe00
 * @author:  Khrisanov Vladislav    xkhris00
 * @author:  Kilybayev Anvar        xkilyb00
 * @author:  Gazizov Zhasdauren     xgaziz00
 */



#include "string.h"
#include "error.h"

/*
 * Создание строки
 */
int string_init(t_str* str)
{
//    t_str* str = malloc(sizeof (t_str));
    str->data = malloc(STRING_LEN * sizeof (char));
    if(!str->data){
        return ERROR_INTERNAL;
    }

    str->data[STRING_START] = NUL;
    str->lenght = STRING_LEN;
    str->how_occupied = 0;
    return IT_IS_OK;
}

/*
 * Очишение строки
 */
void string_free(t_str* str){
    if(str->data)
        free(str->data);
    free(str);
}


/*
 * Увелисиавает строку
 */
int string_expansion(t_str* str){
    t_str string;
    string.data = realloc(str->data, (str->lenght * 2) * sizeof(char));
    if(!string.data){
        return 99;
    }

    str->data = string.data;
    str->lenght = str->lenght * 2;
    return 0;
}
/*str1 - откуда str2 - куда копируется*/
int string_copy(t_str* str1, t_str* str2){
    while(str1->how_occupied > str2->lenght){
        if(string_expansion(str2)){
            return ERROR_INTERNAL;
        }
    }
    str2->how_occupied = STRING_START;
    for(ull slider = 0; str1->data[slider] != NUL; slider++){
        str2->data[slider] = str1->data[slider];
        str2->how_occupied++;
    }

    str2->data[str2->how_occupied++] = NUL;
    return IT_IS_OK;
}

void string_add_string(t_str* str1, t_str* str2){
    for(int slider = 0; str1->data[slider] != NUL; slider++){
        string_wright_char(str2,str1->data[slider]);
    }

}

void string_add_arr(t_str* str1, char* arr){
    for(int slider = 0; arr[slider] != NUL; slider++){
        string_wright_char(str1,arr[slider]);
    }

}

//*
// * Вкладвает символ в начало строки
// */
//void string_wright_char_begin(t_str* str, const char symbol){
//
//    str->how_occupied = STRING_START;
//    str->data[str->how_occupied++] = symbol;
//    str->data[str->how_occupied++] = NUL;
//
//}

/*
 * Вкладвает символ в строку
 */
int string_wright_char(t_str* str, char symbol){
    if(str->lenght <= str->how_occupied) {
        if(string_expansion(str)){
            return ERROR_INTERNAL;
        }
    }
    if(str->how_occupied == 0){
        str->data[str->how_occupied++] = symbol;
        str->data[str->how_occupied++] = NUL;
    }else {
        str->data[--str->how_occupied] = symbol; // str->how_occupied показывает сколько символов влодина считая и конец строки
        str->data[++str->how_occupied] = NUL;
        str->how_occupied++;
    }
    return IT_IS_OK;
}

/*
 * Переписывает масив в строку
 */
int string_wright_arr(t_str* str, char* arr){
    while (str->lenght <= strlen(arr)+1) {
        if (string_expansion(str))
        {
            return ERROR_INTERNAL;
        }
    }   //увеличения буфера строки

    str->how_occupied = STRING_START;
    for(ull slider = 0; arr[slider] != NUL; slider++){
        str->data[slider] = arr[slider];
        str->how_occupied++;
    }

    str->data[str->how_occupied++] = NUL; // сначало вкладываем 0 потом приболяем

    return IT_IS_OK;

}


/*
 * Возвращает длинну строки
 */
ull string_lenght(t_str *str){
    return str->lenght;
}

/*
 * Узнает соответсвует ли первая строка второй строке
 */
bool string_cmp(t_str* str1, t_str* str2){
    if (str1->how_occupied != str2->how_occupied)
        return false;

    for (ull slider = 0; slider < str1->how_occupied ; slider++){
        if(str1->data[slider] != str2->data[slider])
            return false;
    }
    return true;
}

bool string_arr_cmp(t_str* str,const char* arr){
    if(strcmp(str->data,arr) == 0)
        return true;
    else
        return false;
}

void string_init_state(t_str* str ){
    str->data[STRING_START] = NUL;
    str->how_occupied = 0;
}



//int main(){
//    char arr1[] = "123456789sadyftjgggggggggggggg";
//    char arr2[] = "123456789sadyftggggggggggggggg";
//    char s1 = 'a';
//    char s2 = 'b';
//
//    t_str* string = string_init();
//    t_str* string2 = string_init();
//
//    string_wright_arr(string, arr1);
//    printf("string = %s\nhow_occupied = %lld \nlenght = %lld \n \n",string->data, string->how_occupied, string->lenght);
//
//    string_wright_arr(string2, arr1);
//    printf("string = %s\nhow_occupied = %lld \nlenght = %lld \n \n",string2->data, string2->how_occupied, string2->lenght);
//
//    if(string_equales(string, string2)){
//        printf("True\n\n");
//    }else{
//        printf("False\n\n");
//    }
//
//    string_wright_arr(string, arr1);
//    printf("string = %s\nhow_occupied = %lld \nlenght = %lld \n \n",string->data, string->how_occupied, string->lenght);
//
//    string_wright_arr(string2, arr2);
//    printf("string = %s\nhow_occupied = %lld \nlenght = %lld \n \n",string2->data, string2->how_occupied, string2->lenght);
//
//    if(string_equales(string, string2)){
//        printf("True\n\n");
//    }else{
//        printf("False\n\n");
//    }
//
//    string_wright_char_begin(string,s1);
//    printf("string = %s\nhow_occupied = %lld \nlenght = %lld \n \n",string->data, string->how_occupied, string->lenght);
//
//    string_wright_char_begin(string,s2);
//    printf("string = %s\nhow_occupied = %lld \nlenght = %lld \n \n",string->data, string->how_occupied, string->lenght);
//
//    string_wright_char_behind(string,s1);
//    printf("string = %s\nhow_occupied = %lld \nlenght = %lld \n \n",string->data, string->how_occupied, string->lenght);
//
//    string_wright_arr(string,arr2);
//    printf("string = %s\nhow_occupied = %lld \nlenght = %lld \n \n",string->data, string->how_occupied, string->lenght);
//
//
//
//    string_wright_char_begin(string,s2);
//    printf("string = %s\nhow_occupied = %lld \nlenght = %lld \n \n",string->data, string->how_occupied, string->lenght);
//
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    string_wright_char_behind(string,s1);
//    printf("string = %s\nhow_occupied = %lld \nlenght = %lld \n \n",string->data, string->how_occupied, string->lenght);
//
//
//    string_free(string);
//
//    return 0;
//}