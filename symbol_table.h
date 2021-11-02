#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define STACK_CHUNK 100 // Размер стэка, если перевалит то выделит доп. память

typedef enum{
    var,
    func
} sType;

typedef struct{
    sType type; // Тип идентификатора
    bool flag; // Был ли идентификатор уже определен в табулке
    int params; // Кол-во параметров
} sData;

typedef struct tree{
    sData data;
    int key;
    struct tree *right_node; //Указатель на правый узел
    struct tree *left_node; // Указатель на левый узел
}node;

typedef struct{
    int size;
    int top_index;
    node **ptr; // Список указателей на дерево
} s_stack;

//Находит нужный узел в дереве
node* tree_search(node *root, int key);
//Находит узел с минимальным ключом, незнаю если нужен но пусть будет
node* tree_min(node *root);
//Инициализирует дерево и уже создает первый узел(корень)
node *tree_init(int key, sData data);
//Добавления узла в дерево ищет куда вставить с помощью ключа
node *tree_insert(node *root, int key, sData data);
//Полное удаление дерева
node *tree_delete(node *root);
//Проверочная функция(не нужна)
void tree_inOrder(node *root);
//Инициализация стэка табулки
void table_init(s_stack *stack);
//Пушит дерево до стэка
void table_push(s_stack *stack, node *tree);
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