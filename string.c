#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define STRING_LEN 16  // стандартная длин строки
#define STRING_START 0  // начало строки
#define EOL '\0'        // конец строки


typedef unsigned long long ull;

typedef struct s_string{
    char* data;
    ull lenght;
    ull how_occupied;
}t_str;

/*
 * Создание строки
 */
t_str* string_init()
{
    t_str* str = (t_str*)malloc(sizeof (t_str));
    str->data = malloc(STRING_LEN * sizeof (char));
    if(!str->data){
        //TODO вызов функции ошибки
    }

    str->data[STRING_START] = EOL;
    str->lenght = STRING_LEN;
    str->how_occupied = 0;

    return str;
}

/*
 * Очишение строки
 */
void string_free(t_str* str){
    free(str->data);
    free(str);
}


/*
 * Увелисиавает строку
 */
void string_expansion(t_str* str){
    t_str string;
    string.data = realloc(str->data, (str->lenght * 2) * sizeof(char));
    if(!string.data){
        //TODO добавить вывод ошибки
    }

    str->data = string.data;
    str->lenght = str->lenght * 2;
}

/*
 * Вкладвает символ в начало строки
 */
void string_wright_char_begin(t_str* str, char symbol){

    str->how_occupied = STRING_START;
    str->data[str->how_occupied++] = symbol;
    str->data[str->how_occupied++] = EOL;

}

/*
 * Вкладвает символ за уже вложеными символами
 */
void string_wright_char_behind(t_str* str, char symbol){
    if(str->lenght < str->how_occupied + 1)
        string_expansion(str);

    str->data[--str->how_occupied] = symbol; // str->how_occupied показывает сколько символов влодина считая и конец строки
    str->data[++str->how_occupied] = EOL;
    str->how_occupied++;

}

/*
 * Переписывает масив в строку
 */
void string_wright_arr(t_str* str, char* arr){
    while (str->lenght <= strlen(arr)+1)
        string_expansion(str);   //увеличения буфера строки

    str->how_occupied = STRING_START;
    for(ull slider = 0; arr[slider] != EOL; slider++){
        str->data[slider] = arr[slider];
        str->how_occupied++;
    }

    str->data[str->how_occupied++] = EOL; // сначало вкладываем 0 потом приболяем

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
bool string_equales(t_str* str1, t_str* str2){
    if (str1->how_occupied != str2->how_occupied)
        return false;

    for (ull slider = 0; slider < str1->how_occupied ; slider++){
        if(str1->data[slider] != str2->data[slider])
            return false;
    }
    return true;
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