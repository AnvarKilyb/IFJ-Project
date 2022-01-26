/**
 * Project: Implementace překladače imperativního jazyka IFJ21
 *
 * File:     symtable.h
 * Subject:  IFJ2021
 *
 * @author:  Vladislav Mikheda  	xmikhe00
 * @author:  Khrisanov Vladislav    xkhris00
 * @author:  Kilybayev Anvar        xkilyb00
 * @author:  Gazizov Zhasdauren     xgaziz00
 */



#ifndef IFJ_PROJEKT_SYMTABLE_H
#define IFJ_PROJEKT_SYMTABLE_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "string.h"
#include "string_param.h"

#define STACK_CHUNK 100 // Размер стэка, если перевалит то выделит доп. память

typedef struct{
//    sType type; // Тип идентификатора
    t_str* name;
    t_str* type;

    bool declaration;
    bool define; // Был ли дифинован
    bool it_is_define;

    int count_params; // Кол-во параметров
    t_str_param* params; // Параметры
    t_str_param* type_params;

    bool was_return;
    int count_returned_params;
    t_str_param* type_returned_params; // Возвращаемы параметры

    bool data_valid;
    int data_int;
    double data_double;
    t_str* data_string;
    bool system_function;
    bool system_function_infinity_param;
    int counter_in;

    int help_count;
} sData;


typedef struct tree{
    sData* data;
    unsigned long  key;
    struct tree *right_node; //Указатель на правый узел
    struct tree *left_node; // Указатель на левый узел
}node;

typedef struct{
    int size;
    int top_index;
    node **ptr; // Список указателей на дерево
} s_stack;
node* table_top(s_stack *stack);
void s_data_init(sData* node);
//Находит нужный узел в дереве
node* tree_search(node *root, unsigned long key);
//Находит узел с минимальным ключом, незнаю если нужен но пусть будет
node* tree_min(node *root);
//Инициализирует дерево и уже создает первый узел(корень)
node *tree_init(unsigned long  key, sData *data);
//Добавления узла в дерево ищет куда вставить с помощью ключа
node *tree_insert(node *root, unsigned long  key, sData *data);
//Полное удаление дерева
node *tree_delete(node *root);
//Проверочная функция(не нужна)
void tree_inOrder(node *root);
//Инициализация стэка табулки
void table_init(s_stack *stack);
//Пушит дерево до стэка
int table_push(s_stack *stack, node *tree);
//Вытаскивает дерево из стэка
node* table_pop(s_stack *stack);
//Выписывает все дерево с помощью preOrder: 
/*
                   20
                 /    \
                /      \
               5       30
                 \     /\
                  \   /  \
                  15 25   40
                /           \
               /             \
              9              45
            /   \           /
           /     \         /
          7      12       42
    PRINT: 20  5  15  9  7  12  30  25  40  45  42
*/
void table_leftMostPre(s_stack *stack, node *tree);
void table_preOrder(node *tree);
// Удаляет табулку
void table_delete(s_stack *stack);
//Кодирует строку в ключ
unsigned long hashcode(/*unsigned*/ char *str);

#endif //IFJ_PROJEKT_SYMTABLE_H