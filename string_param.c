
#include "string_param.h"


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

    for(int i = 0; i < str->lenght; i++){
        free(str->data[i]->data);
        free(str->data[i]);
    }
    free(str);
}


/*
 * Увелисиавает строку
 */
int string_param_expansion(t_str_param* str){
    t_str_param string;
    string.data = realloc(str->data, (str->lenght * 2) * sizeof(t_str*));
    if(!string.data){
        //TODO добавить вывод ошибки
    }

    str->data = string.data;
    str->lenght = str->lenght * 2;

    for(ull i = str->how_occupied; i < str->lenght; i++){
        str->data[i] = malloc(sizeof (t_str));
        if(!str->data[i]){
            //TODO error
        }
        string_init(str->data[i]);
    }
    return 0;
}

void string_param_copy_string(t_str_param* str_par, t_str* str){
    if((str_par->lenght) <= str_par->how_occupied){
        string_param_expansion(str_par);
    }
    string_copy(str, str_par->data[str_par->how_occupied]);
    str_par->how_occupied++;
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

bool string_param_cmp_string(t_str_param* str_p, ull numb_p, t_str* str){
    if(str_p->how_occupied <= (numb_p-1)){
        return false;
    }
    return string_cmp(str_p->data[numb_p-1], str);
}

//void string_add_string(t_str* str1, t_str* str2){
//    for(int slider = 0; str1->data[slider] != NUL; slider++){
//        string_wright_char(str2,str1->data[slider]);
//    }
//
//}
//
////*
//// * Вкладвает символ в начало строки
//// */
////void string_wright_char_begin(t_str* str, const char symbol){
////
////    str->how_occupied = STRING_START;
////    str->data[str->how_occupied++] = symbol;
////    str->data[str->how_occupied++] = NUL;
////
////}
//
///*
// * Вкладвает символ в строку
// */
//void string_wright_char(t_str* str, char symbol){
//    if(str->lenght <= str->how_occupied) {
//        string_expansion(str);
//    }
//    if(str->how_occupied == 0){
//        str->data[str->how_occupied++] = symbol;
//        str->data[str->how_occupied++] = NUL;
//    }else {
//        str->data[--str->how_occupied] = symbol; // str->how_occupied показывает сколько символов влодина считая и конец строки
//        str->data[++str->how_occupied] = NUL;
//        str->how_occupied++;
//    }
//
//}
//
////функция для строки в строке
//void string_wright_char(t_str* str, t_str* str_in){
//    if(str->lenght <= str->how_occupied) {
//        string_expansion(str);
//    }
//    if(str->how_occupied == 0){
//        str->data[str->how_occupied++] = str_in;
//        str->data[str->how_occupied++] = NUL;
//    }else {
//        str->data[--str->how_occupied] = str_in; // str->how_occupied показывает сколько символов влодина считая и конец строки
//        str->data[++str->how_occupied] = NUL;
//        str->how_occupied++;
//    }
//
//}
//
//
///*
// * Переписывает масив в строку
// */
//void string_wright_arr(t_str* str, char* arr){
//    while (str->lenght <= strlen(arr)+1)
//        string_expansion(str);   //увеличения буфера строки
//
//    str->how_occupied = STRING_START;
//    for(ull slider = 0; arr[slider] != NUL; slider++){
//        str->data[slider] = arr[slider];
//        str->how_occupied++;
//    }
//
//    str->data[str->how_occupied++] = NUL; // сначало вкладываем 0 потом приболяем
//
//}
//
//
///*
// * Возвращает длинну строки
// */
//ull string_lenght(t_str *str){
//    return str->lenght;
//}
//
///*
// * Узнает соответсвует ли первая строка второй строке
// */
//bool string_cmp(t_str* str1, t_str* str2){
//    if (str1->how_occupied != str2->how_occupied)
//        return false;
//
//    for (ull slider = 0; slider < str1->how_occupied ; slider++){
//        if(str1->data[slider] != str2->data[slider])
//            return false;
//    }
//    return true;
//}
//
//bool string_arr_cmp(t_str* str,const char* arr){
//    return strcmp(str->data,arr);
//}
//
//void string_init_state(t_str* str ){
//    str->data[STRING_START] = NUL;
//    str->how_occupied = 0;
//}

