#include <stdio.h>
#include "tree.h"
#include <assert.h>
#include <string.h>
#include "akinator.h"

int SaveTree(TreeNode* node, FILE* file, int depth = 0) {
    assert(file != NULL);
    assert(node != NULL);

    fprintf(file, "%*s", depth*4, "");
    fprintf(file, "{\n");

    fprintf(file, "%*s", (depth+1)*4, "");
    fprintf(file, "%s\n", node->value);

    if (node->left) SaveTree(node->left, file, depth + 1);
    if (node->right) SaveTree(node->right, file, depth + 1);

    fprintf(file, "%*s", depth*4, "");
    fprintf(file, "}\n");

    return SUCCESS_DONE;
}

int SaveTreeToFile(TreeNode* root, const char* filename) {
    assert(root != NULL);
    assert(filename != NULL);

    FILE* file = fopen(filename, "w");
    if (!file) {
        perror("Ошибка при открытии файла");
        return FILE_OPENING_ERR;
    }
    SaveTree(root, file);
    fclose(file);
    return SUCCESS_DONE;
}


TreeNode* LoadTree(FILE* file) {
    assert(file != NULL);
    char buffer[TREE_SEPARATOR_SIZE];

    while (fgets(buffer, sizeof(buffer), file)) {
        string_separator(buffer);
        if (strchr(buffer, '{')) break;
    }

    if (fgets(buffer, sizeof(buffer), file) == NULL) return NULL;
    string_separator(buffer);
    TreeNode* node = NULL;
    CreateNode(&node, buffer);


    long pos = ftell(file); //Проверка на следующий символ
    if (fgets(buffer, sizeof(buffer), file)) {
        string_separator(buffer);
        if (strchr(buffer, '{')) {
            fseek(file, pos, SEEK_SET); // Вернулись
            node->left = LoadTree(file);
        } else {
            fseek(file, pos, SEEK_SET);
        }
    }

    pos = ftell(file);
    if (fgets(buffer, sizeof(buffer), file)) {
        string_separator(buffer);
        if (strchr(buffer, '{')) {
            fseek(file, pos, SEEK_SET);
            node->right = LoadTree(file);
        } else {
            fseek(file, pos, SEEK_SET);
        }
    }
    while (fgets(buffer, sizeof(buffer), file)) {
        if (strchr(buffer, '}')) break;
    }

    return node;
}






TreeNode* LoadTreeFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка при открытии файла");
        return NULL;
    }

    TreeNode* Root = LoadTree(file);
    fclose(file);
    return Root;
}



//TODO: ИДТИ ПО МАССИВУ ТЕКСТА, ИЗБАВИТЬСЯ ОТ КУЧИ fgets
