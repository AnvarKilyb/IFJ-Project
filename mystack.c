#include <string.h>
#include <stdlib.h>
//#include "error.h"

#define STACK_IS_EMPTY 0

typedef enum {
    $,
    PROG,
    CHUNK,
    FUNCTION,
}e_rule;


typedef struct s_stack_unit{
    e_rule data; //TODO вместо инта дополнить что нужно
    struct s_stack_unit* down_element;
}t_stack_element;

//typedef struct s_stack_data{
//
//}t_stack_data;

typedef struct s_stack{
    t_stack_element* stack_top;
    int amount_of_elements;
}t_stack;

int stack_init(t_stack* stack){
    if(!stack)
        return ERROR_INTERNAL;

    stack->stack_top = NULL;
    stack->amount_of_elements = STACK_IS_EMPTY;
    return IT_IS_OK;
}


static int stack_is_empty(t_stack* stack){
    return stack->amount_of_elements == 0;
}

int stack_push(t_stack* stack, e_rule a /* TODO заменит на то что надо вставить в стек*/){
    if(!stack)
        return ERROR_INTERNAL;

    t_stack_element *ptr = (t_stack_element *) malloc(sizeof(t_stack_element));
    if (!ptr)
        return ERROR_INTERNAL;

    ptr->data = a;
    ptr->down_element = stack->stack_top;

    stack->stack_top = ptr;
    stack->amount_of_elements++;

    return IT_IS_OK;
}

int/*TODO поменять на то что долно лежать в стеке*/ stack_top(t_stack *stack){
    if(!stack)
        return ERROR_INTERNAL;
    return stack->stack_top->data;
}

int stack_pop(t_stack* stack){
    if(!stack)
        return ERROR_INTERNAL;
    if(stack_is_empty(stack))
        return ERROR_INTERNAL;

    t_stack_element* ptr = stack->stack_top;
    stack->stack_top = stack->stack_top->down_element;
    stack->amount_of_elements--;
    free(ptr);
    return IT_IS_OK;
}

int stack_free(t_stack* stack){
    if(!stack)
        return IT_IS_OK;
    while(stack->amount_of_elements != 0){
        stack_pop(stack);
    }
    return IT_IS_OK;
}