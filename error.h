/**
 * Project: Implementace překladače imperativního jazyka IFJ21
 *
 * File:     error.h
 * Subject:  IFJ2021
 *
 * @author:  Vladislav Mikheda  	xmikhe00
 * @author:  Khrisanov Vladislav    xkhris00
 * @author:  Kilybayev Anvar        xkilyb00
 * @author:  Gazizov Zhasdauren     xgaziz00
 */

typedef enum {
    IT_IS_OK = 0,
    ERROR_LEX_ANALYSIS = 1,
    ERROR_SYN_ANALYSIS = 2,
    ERROR_SEMANTIC_ANALYSIS = 3,
    ERROR_SEMANTIC_ANALYSIS_EQ = 4,// в присвоение
    ERROR_SEMANTIC_ANALYSIS_PARAM_IN_FUNC = 5,
    ERROR_SEMANTIC_ANALYSIS_EXPR = 6,
    ERROR_SEMANTIC_ANALYSIS_ALL = 7,
    ERROR_UNEXPECTED_ZERO = 8,
    ERROR_INTERNAL = 99,
}e_error_message;
