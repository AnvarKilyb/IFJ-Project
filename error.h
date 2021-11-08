#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef enum {
    IT_IS_OK = 0,
    ERROR_LEX_ANALYSIS = 9,
    ERROR_SYN_ANALYSIS = 10, //TODO kod
    
    ERROR_INTERNAL = 99,
}e_error_message;
