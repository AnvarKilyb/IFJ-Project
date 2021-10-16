#include <stdio.h>
#include <stdlib.h>

typedef struct{
    int params;
    //TODO
} sData;

typedef struct tree{
    sData data;
    int key;
    struct tree *right_node; //Указатель на правый узел
    struct tree *left_node; // Указатель на левый узел
}node;
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