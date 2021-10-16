#include <stdio.h>
#include <stdlib.h>

#include "binary_tree.h"

node *tree_search(node *root, int key){
    if(root == NULL || root->key == key)
        return root; // Возвращает нужный узел
    else if(key > root->key) // Ищет в правом поддереве нужый узел, пока ключи не совпадут
        return tree_search(root->right_node, key);
    else // ИЩет в левом поддереве нужный узел, пока ключи не совпадут
        return tree_search(root->left_node, key);
}

node *tree_min(node *root){
    if(root == NULL)
        return NULL;
    else if(root->left_node != NULL)
        return tree_min(root->left_node);
    return root;
}
// Создает дерево, уже с первым узлом
node *tree_init(int key, sData data){
    node *root;
    root = malloc(sizeof(node));
    root->data = data;
    root->key = key;
    root->left_node = NULL;
    root->right_node = NULL;

    return root;
}
// Добавление узла в дерево, ищет место куда вставить новый узел
node *tree_insert(node *root, int key, sData data){
    if(root == NULL)
        return tree_init(key, data);
    else if(key > root->key) // Ищет место вставки в правом поддереве, если ключ больше ключа узла
        root->right_node = tree_insert(root->right_node, key, data);
    else // Ищет место вставки в левом поддереве, если ключ меньше ключа узла
        root->left_node = tree_insert(root->left_node, key, data);
    return root;
}

node *tree_delete(node *root){
    if(root == NULL)
        return NULL;
    // Проходит все дерево
    root->right_node = tree_delete(root->right_node);
    root->left_node = tree_delete(root->left_node);

    // Тут будет освобождение памяти data

    // Освобождение памяти узлов
    free(root);
    root = NULL;
    return root;
}

//Провера работоспособности дерева

// void tree_inOrder(node *root){
//     if(root != NULL){
//         tree_inOrder(root->left_node);
//         printf(" %d|%d ", root->key, root->data.params);
//         tree_inOrder(root->right_node);
//     }
// }

// int main(){
//     node *root;
//     sData data;
//     data.params = 110;
//     root = tree_init(20, data);
//     tree_insert(root, 5, data);
//     tree_insert(root, 1, data);
//     tree_insert(root, 15, data);
//     tree_insert(root, 9, data);
//     tree_insert(root, 7, data);
//     tree_insert(root, 12, data);
//     tree_insert(root, 30, data);
//     tree_insert(root, 25, data);
//     tree_insert(root, 40, data);
//     tree_insert(root, 45, data);
//     tree_insert(root, 42, data);

//     tree_inOrder(root);
//     printf("\n");

//     root = tree_delete(root);

// }