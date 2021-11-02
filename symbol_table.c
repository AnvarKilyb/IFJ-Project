#include <stdio.h>
#include <stdlib.h>

#include "symbol_table.h"
#include "error.h"

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

void table_init(s_stack *stack){
    if(stack == NULL) return;
    stack->top_index = -1;
    stack->ptr = NULL;
    stack->size = 0;
}

void table_push(s_stack *stack, node *tree){
    if(stack == NULL) return;

    stack->top_index++;
    if(stack->size <= stack->top_index){
        stack->size += STACK_CHUNK;
        stack->ptr = realloc(stack->ptr, sizeof(node *) * stack->size);
    }

    stack->ptr[stack->top_index] = tree;

}

node* table_pop(s_stack *stack){
    if(stack == NULL || stack->top_index < 0)
        return NULL;
    
    return stack->ptr[stack->top_index--];
}

void table_delete(s_stack *stack){
    if(stack == NULL) return;

    free(stack->ptr);
    stack->ptr = NULL;
    stack->size = 0;
    stack->top_index = -1;
}

void table_leftMostPre(s_stack *stack, node *tree){
    while(tree != NULL){
        table_push(stack, tree);
        printf("Key: %d\n", tree->key);
        tree = tree->left_node;
    }
}

void table_preOrder(node *tree){
    s_stack stack;
    table_init(&stack);
    table_leftMostPre(&stack, tree);
    while(stack.top_index >= 0){
        tree = table_pop(&stack);
        table_leftMostPre(&stack, tree->right_node);
    }
    table_delete(&stack);
}

// int main(){
//     sData data = {func, 1, 100};
//     node *tree;
//     tree = tree_init(2, data);
//     sData data1 = {var, 0, 101};
//     tree_insert(tree, 3, data1);
//     sData data2 = {func, 1, 102};
//     tree_insert(tree, 1, data2);

//     table_defVarPre(tree);

//     tree = tree_delete(tree);
//  }