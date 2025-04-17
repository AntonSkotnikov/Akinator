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


int FindObjectWithPath(TreeNode* Node, Stack* stk, const char* target) {
    if (Node == NULL) {
        return NODE_NULLPTR;
    }

    if (strcmp(Node->value, target) == 0) {
        printf("Объект найден: %s\n", Node->value);
        return SUCCESS_DONE;
    }

    if (strchr(Node->value, '?')) {
        if (FindObjectWithPath(Node->left, stk, target) == SUCCESS_DONE) {
            StackPush(stk, Node->left);
            printf("pointer да = %p\n", Node->left);
            return SUCCESS_DONE;
        }
        if (FindObjectWithPath(Node->right, stk, target) == SUCCESS_DONE) {
            StackPush(stk, Node->right);
            printf("pointer нет = %p\n", Node->right);
            return SUCCESS_DONE;
        }
    }

    return NODE_NULLPTR;
}


int AkinatorDefinition(TreeNode* Node, Stack* stk, const char* target) {
    assert(stk != NULL);
    FindObjectWithPath(Node, stk, target);
    StackPush(stk, Node);
    // StackDump(stk);
    printf("%s - ", target);
    PrintRestDefinition(stk);
    return SUCCESS_DONE;
}

int AkinatorDifference(TreeNode* Node, Stack* stk_1, Stack* stk_2, elem_t word_1, elem_t word_2) {
    assert(stk_1 != NULL);
    assert(stk_2 != NULL);
    FindObjectWithPath(Node, stk_1, word_1);
    FindObjectWithPath(Node, stk_2, word_2);
    StackPush(stk_1, Node);
    StackPush(stk_2, Node);
    // StackDump(stk_1);
    // StackDump(stk_2);
    TreeNode* current_node_1 = nullptr;
    TreeNode* current_node_2 = nullptr;
    TreeNode* parent_node_1 = nullptr;
    TreeNode* parent_node_2 = nullptr;
    char* temp_val = nullptr;
    printf("Вот в чем разница: \n");
    while ((StackGetSize(stk_1) > 0) && (StackGetSize(stk_2) > 0)) {
        // StackDump(stk_1);
        // StackDump(stk_2);
        parent_node_1 = current_node_1;
        parent_node_2 = current_node_2;
        current_node_1 = StackPop(stk_1); //NOTE: глянуть StackPop, как решить вопрос со StackCheck
        current_node_2 = StackPop(stk_2);
        if ((current_node_1 == NULL) || (current_node_2 == NULL )) {
            printf("Ошибка: стек пуст\n");
            return NODE_NULLPTR;
        }
        if ((current_node_1 != current_node_2) && (parent_node_1 == parent_node_2)) {
            if (parent_node_1->left == current_node_1) {
                StackPush(stk_1, current_node_1);
                StackPush(stk_2, current_node_2);
                temp_val = strdup(parent_node_1->value);
                temp_val[strlen(temp_val) - 1] = '\0';
                PrintDifference(stk_2, stk_1, temp_val, word_2, word_1);

                return SUCCESS_DONE;

            }
            else if (parent_node_1->right == current_node_1) {
                StackPush(stk_1, current_node_1);
                StackPush(stk_2, current_node_2);
                temp_val = strdup(parent_node_1->value);
                temp_val[strlen(temp_val) - 1] = '\0';
                PrintDifference(stk_1, stk_2, temp_val, word_1, word_2);

                return SUCCESS_DONE;


            }

        }

    }

    printf("Проблема в дереве, какая-то хрень с поиском разницы\n");
    return  ERROR_OCCURED;
}


int PrintRestDefinition(Stack* stk) {

    assert(stk != NULL);
    TreeNode* current_node = nullptr;
    TreeNode* parent_node = nullptr;
    char* temp_val = nullptr;
    current_node = StackPop(stk);
    if (current_node == NULL) {
        printf("Ошибка: стек пуст\n");
        return NODE_NULLPTR;
    }


    while (StackGetSize(stk) > 0)
    {
        parent_node = current_node;
        current_node = StackPop(stk);
        if (current_node == NULL) {
            printf("Ошибка: стек пуст или путь неверен\n");
            return NODE_NULLPTR;
        }

        if (parent_node->left == current_node) {

            //strcpy(temp_val, parent_node->value); // FIXME to printf
            temp_val = strdup(parent_node->value);

            temp_val[strlen(temp_val) - 1] = '\0';

            printf("%s | ", temp_val);
        }
        else if (parent_node->right == current_node) {
            temp_val = strdup(parent_node->value);
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

int PrintDifference(Stack* stk_1, Stack* stk_2, char* temp_val, elem_t word_1, elem_t word_2) {

    printf("первое различие: %s - %s, а %s - не %s\n", word_2, temp_val, word_1, temp_val);
    printf("А вот различающие части определений объектов: \n");
    printf("%s - ", word_1);
    PrintRestDefinition(stk_1);
    printf("%s - ", word_2);
    PrintRestDefinition(stk_2);


    return SUCCESS_DONE;
}
