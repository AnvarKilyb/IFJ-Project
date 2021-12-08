/**
 * Project: Implementace překladače imperativního jazyka IFJ21
 *
 * File:     symtable.c
 * Subject:  IFJ2021
 *
 * @author:  Vladislav Mikheda  	xmikhe00
 * @author:  Khrisanov Vladislav    xkhris00
 * @author:  Kilybayev Anvar        xkilyb00
 * @author:  Gazizov Zhasdauren     xgaziz00
 */


#include <stdio.h>
#include <stdlib.h>

#include "symtable.h"
#include "error.h"

void s_data_init(sData* node){
    node->name = NULL;
    node->type = NULL;

    node->declaration = false;
    node->define = false;
    node->it_is_define = false;

    node->count_params = 0;
    node->params = NULL;
    node->type_params = NULL;

    node->was_return = false;
    node->count_returned_params = 0;
    node->type_returned_params = NULL;

    node->data_int = 0;
    node->data_double = 0.0;
    node->counter_in = 0;
    node->data_string = NULL;
    node->data_valid = false;
    node->system_function = false;
    node->system_function_infinity_param = false;

    node->help_count = 0;
}


node *tree_search(node *root, unsigned long key){
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

//static void copy_data(sData *data, node *root){
//    if(!root->data.name) {
//        root->data.name = malloc(sizeof(t_str));
//        string_init(root->data.name);
//    }
//    if(!root->data.string_params) {
//        root->data.string_params = malloc(sizeof(t_str));
//        string_init(root->data.string_params);
//    }
//
//    root->data.type = data->type;
//    string_copy(data->string_params, root->data.string_params);
//    root->data.define = data->define;
//    root->data.params = data->params;
//    string_copy(data->string_params, root->data.string_params);
//}
// Создает дерево, уже с первым узлом
node *tree_init(unsigned long  key, sData *data){
    node *root;
    root = malloc(sizeof(node));
    if(!root){
        return NULL;
    }

    root->key = key;
    root->left_node = NULL;
    root->right_node = NULL;
    root->data = data;
    return root;
}
// Добавление узла в дерево, ищет место куда вставить новый узел
node *tree_insert(node *root, unsigned long  key, sData *data){
    if(root == NULL)
        return tree_init(key, data);
    else if(key > root->key) // Ищет место вставки в правом поддереве, если ключ больше ключа узла
        root->right_node = tree_insert(root->right_node, key, data);
    else if(key < root->key)// Ищет место вставки в левом поддереве, если ключ меньше ключа узла
        root->left_node = tree_insert(root->left_node, key, data);
    else
        //copy_data(&data, root);
       root->data = data;
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
    if(root->data){
        if(root->data->name){
            string_free(root->data->name);
        }
        if(root->data->type){
            string_free(root->data->type);
        }
        if(root->data->params){
            string_param_free(root->data->params);
        }
        if(root->data->type_params){
            string_param_free(root->data->type_params);
        }
        if(root->data->type_returned_params){
            string_param_free(root->data->type_returned_params);
        }
        if(root->data->data_string){
            string_free(root->data->data_string);
        }
        free(root->data);
    }
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

int table_push(s_stack *stack, node *tree){
    if(stack == NULL) return IT_IS_OK;

    stack->top_index++;
    if((stack->size - 1) <= stack->top_index){
        stack->size += STACK_CHUNK;
        stack->ptr = realloc(stack->ptr, sizeof(node *) * stack->size);
        if(!stack->ptr){
            return ERROR_INTERNAL;
        }
    }

    stack->ptr[stack->top_index] = tree;
    return IT_IS_OK;
}

node* table_pop(s_stack *stack){
    if(stack == NULL || stack->top_index < 0)
        return NULL;
    
    return stack->ptr[stack->top_index--];
}

node* table_top(s_stack *stack){
    if(stack == NULL || stack->top_index < 0)
        return NULL;

    return stack->ptr[stack->top_index];
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
        printf("Key(%ld)\n", tree->key);
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

unsigned long hashcode(/*unsigned*/ char *str){
    unsigned long hash = 0, x = 0;
    
    for(char c = *str; c != '\0'; c = *(++str)){
        hash = (hash << 4) + c;
        if((x = hash & 0xF0000000L) != 0){
            hash ^= (x >> 24);
        }
        hash &= ~x;
    }
    return hash;
}

//int main(){
    // char arr1[] = "return";
    // char arr2[] = "stop";
    // char arr3[] = "while";
    // t_str *string1 = string_init();
    // t_str *string2 = string_init();
    // t_str *string3 = string_init();

    // string_wright_arr(string1, arr1);
    // string_wright_arr(string2, arr2);
    // string_wright_arr(string3, arr3);

    // int key1 = 0, key2 = 0, key3 = 0;
    // key1 = hashcode(string1->data);
    // key2 = hashcode(string2->data);
    // key3 = hashcode(string3->data);
    // sData data1 = {string1->data, FUNC, 0, 0};
    // sData data2 = {string2->data, VAR, 0, 0};
    // sData data3 = {string3->data, VAR, 0, 0};

    // node *tree;
    // tree = tree_init(key1, data1);
    // tree_insert(tree, key2, data2);
    // tree_insert(tree, key3, data3);

    // table_preOrder(tree);

    // tree = tree_delete(tree);
//    return 0;
// }
