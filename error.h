//#include <string.h>
//#include <stdio.h>
//#include <stdlib.h>
//#include <stdbool.h>
//

typedef enum {
    IT_IS_OK = 0,
    ERROR_LEX_ANALYSIS = 9,
    ERROR_SYN_ANALYSIS = 10, //TODO kod
    ERROR_SEMANTIC_ANALYSIS = 3,
    ERROR_SEMANTIC_ANALYSIS_EQ = 4,// в присвоение
    ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC = 5,
    ERROR_SEMANTIC_ANALYSIS_EXPR = 6,
    ERROR_SEMANTIC_ANALYSIS_ALL = 7,
    ERROR_INTERNAL = 99,
}e_error_message;
