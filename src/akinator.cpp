#include "akinator.h"
#include "tree.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include "stack.h"
#include <assert.h>

bool is_question(const char* str) {
    return strchr(str, '?') != NULL;
}

bool is_yes(const char* str) {
    return strcmp(str, "да") == 0 || strcmp(str, "yes") == 0 ||
           strcmp(str, "Yes") == 0 || strcmp(str, "Да") == 0;
}
bool is_no(const char* str) {
    return strcmp(str, "нет") == 0 || strcmp(str, "no") == 0 || strcmp(str, "No") == 0 || strcmp(str, "Нет") == 0;
}



TreeNode** processing_bad_input(char* input, TreeNode** node) {

    while (true) {
        if (is_yes(input)) {
            return &((*node)->left);
        }
        else if (is_no(input)) {
            return &((*node)->right);
        }
        else {
            printf("Не понял. Введи еще раз yes/no или да/нет. Это %s\n", (*node)->value);
            fgets(input, INPUT_SIZE, stdin);
            string_separator(input);
            //printf("input = |%s|, sizeof = %lu, is_yes = %d, is_no = %d\n", input, sizeof(input), is_yes(input), is_no(input) );
        }
    }


}


TreeNode** processing_no_input(char* input, TreeNode** node) {
    if (*node == NULL) {
        printf("Не угадал. Добавим новый объект.\n");

        char* new_object = (char*)calloc(INPUT_SIZE, sizeof(char));
        printf("Кто это был?\n");
        fgets(new_object, INPUT_SIZE, stdin);
        string_separator(new_object);

        TreeNode* new_node = NULL;
        CreateNode(&new_node, new_object);

        *node = new_node;

        free(new_object);
        free(input);

        printf("Записал. Игра окончена\n");
        return NULL;
    }

    // Узел есть -> это объект
    printf("Не угадал. Добавим новый объект.\n");

    char* new_object = (char*)calloc(INPUT_SIZE, sizeof(char));
    char* new_question = (char*)calloc(INPUT_SIZE, sizeof(char));

    printf("Кто это был?\n");
    fgets(new_object, INPUT_SIZE, stdin);
    string_separator(new_object);

    printf("Чем %s отличается от %s?\n", new_object, (*node)->value);
    fgets(new_question, INPUT_SIZE, stdin);
    string_separator(new_question);

    if (!strchr(new_question, '?')) {
        strcat(new_question, "?");
    }

    TreeNode* old_node = NULL;
    CreateNode(&old_node, (*node)->value);

    TreeNode* new_node = NULL;
    CreateNode(&new_node, new_object);

    free((*node)->value);
    (*node)->value = strdup(new_question);
    (*node)->left = new_node;
    (*node)->right = old_node;

    free(new_object);
    free(new_question);
    free(input);

    printf("Записал. Игра окончена\n");
    return NULL;
}

int AkinatorTraversal(TreeNode** node) {
    if (node == NULL || *node == NULL) return NODE_NULLPTR;

    while (true) {
        char* input = (char*)calloc(INPUT_SIZE, sizeof(char));

        if (is_question((*node)->value)) {
            while (true) {
                printf("Это %s\n", (*node)->value);
                fgets(input, INPUT_SIZE, stdin);
                string_separator(input);

                if (is_yes(input)) {
                    if ((*node)->left == NULL) {
                        printf("Угадал!\n");
                        free(input);
                        return SUCCESS_DONE;
                    }
                    node = &((*node)->left);
                    break;
                }
                else if (is_no(input)) {
                    if ((*node)->right == NULL) {
                        TreeNode** new_node = processing_no_input(input, node);
                        if (new_node == NULL) return SUCCESS_DONE;
                        node = new_node;
                    } else {
                        node = &((*node)->right);
                    }
                    break;
                }
                else {
                    printf("Не понял. Введи ещё раз yes/no или да/нет.\n");
                }
            }
        }
        else {
            //объект
            while (true) {
                printf("Это %s?\n", (*node)->value);
                fgets(input, INPUT_SIZE, stdin);
                string_separator(input);

                if (is_yes(input)) {
                    printf("Ура! Я победил!\n");
                    free(input);
                    return SUCCESS_DONE;
                }
                else if (is_no(input)) {
                    TreeNode** new_node = processing_no_input(input, node);
                    if (new_node == NULL) return SUCCESS_DONE;
                    node = new_node;
                    break;
                }
                else {
                    printf("Не понял. Введи ещё раз yes/no или да/нет.\n");
                }
            }
        }

        free(input);
    }

    return SUCCESS_DONE;
}


//TODO: Для Определения нужно: воспользоваться стэком, когда мы нашли конкретный объект, мы должны делать постфиксный обход: возвращаться на уровень выше и пушить этот объект



//TODO: сделать бесконечный цикл с нексколькими режимами: угадать, определение, и выйти

//TODO: для разницы между объектами: для объекта 1 свой стек, для объекта 2 - свой стэк.
//мы должны попить их, и искать момент, когда у них произойдет первое различие. На этом моменте
//мы должны будем вывести остаток тех определений для каждого объекта, которые и различают их


int FindObjectWithPath(TreeNode* Node, Stack* stk, const char* target, char* path) {
    if (Node == NULL) {
        return NODE_NULLPTR;
    }

    if (strcmp(Node->value, target) == 0) {
        printf("Объект найден: %s\n", Node->value);
        printf("Путь к объекту: %s\n", path);
        return SUCCESS_DONE;
    }

    if (strchr(Node->value, '?')) {
        char temp_path[INPUT_SIZE];
        strcpy(temp_path, path);

        strcat(temp_path, "да -> ");
        if (FindObjectWithPath(Node->left, stk, target, temp_path) == SUCCESS_DONE) {
            StackPush(stk, Node->left);
            printf("pointer да = %p\n", Node->left);
            return SUCCESS_DONE;
        }
        temp_path[strlen(temp_path) - 8] = '\0';  // Убираем "да -> " из пути
        strcat(temp_path, "нет -> ");
        if (FindObjectWithPath(Node->right, stk, target, temp_path) == SUCCESS_DONE) {
            StackPush(stk, Node->right);
            printf("pointer нет = %p\n", Node->right);
            return SUCCESS_DONE;
        }
    }

    return NODE_NULLPTR;
}


int AkinatorDefinition(Stack* stk, elem_t word) {
    assert(stk != NULL);
    StackDump(stk);
    TreeNode* current_node = nullptr;
    TreeNode* parent_node = nullptr;
    char temp_val[INPUT_SIZE] = {};
    printf("%s - ", word);

    current_node = StackPop(stk);
    if (current_node == NULL) {
        printf("Ошибка: стек пуст или путь неверен\n");
        return NODE_NULLPTR;
    }

    //printf("CUR NODE = %p\n", current_node);
    //printf("stack size = %zu\n", stk->service_size);

    while (StackGetSize(stk) > 0)
    {
        parent_node = current_node;
        current_node = StackPop(stk);
        if (current_node == NULL) {
            printf("Ошибка: стек пуст или путь неверен\n");
            return NODE_NULLPTR;
        }
        //printf("parent_node = %p, cur node = %p\n", parent_node, current_node);
        //printf("parent node left = %p, parent node right = %p\n", parent_node->left, parent_node->right);
        if (parent_node->left == current_node) {

            strcpy(temp_val, parent_node->value);
            temp_val[strlen(temp_val) - 1] = '\0';

            printf("%s | ", temp_val);
        }
        else if (parent_node->right == current_node) {
            strcpy(temp_val, parent_node->value);
            temp_val[strlen(temp_val) - 1] = '\0';
            printf("не %s |", temp_val);
        }
        else {
            printf("ошибка: неверный путь\n ");
        }
    }

    printf("\n");
    return SUCCESS_DONE;
}
