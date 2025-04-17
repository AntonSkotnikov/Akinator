#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "tree.h"
#include "akinator.h"


void create_ptr_arr(char *buffer, int strings_num, char **address) {
    int j = 0;
    address[0] = buffer;

    for (int i = 1; i < strings_num; i++) {
        while (buffer[j] != '\0') {
            j++;
        }
        j++;
        address[i] = buffer + j;
    }
}


#include <stdio.h>
#include <assert.h>
#include "tree.h"
#include "akinator.h"

void WriteIndent(FILE* file, int depth) {
    fprintf(file, "%*s", depth * 4, "");
}

int SaveTree(TreeNode* node, FILE* file, int depth) {
    assert(file != NULL);
    assert(node != NULL);

    WriteIndent(file, depth);
    fprintf(file, "{\n");

    WriteIndent(file, depth + 1);
    fprintf(file, "%s\n", node->value);

    if (node->left) {
        SaveTree(node->left, file, depth + 1);
    }

    if (node->right) {
        SaveTree(node->right, file, depth + 1);
    }

    WriteIndent(file, depth);
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

    SaveTree(root, file, 0);
    fclose(file);
    return SUCCESS_DONE;
}


TreeNode* LoadTreeFromLines(char** lines, int* pos, int total_lines) {
    if (*pos >= total_lines) return NULL;

    if (strchr(lines[*pos], '{') == NULL) return NULL;
    (*pos)++;

    if (*pos >= total_lines) return NULL;

    TreeNode* node = NULL;
    CreateNode(&node, lines[*pos]);
    (*pos)++;

    if (*pos < total_lines && strchr(lines[*pos], '{')) {
        node->left = LoadTreeFromLines(lines, pos, total_lines);
    }

    if (*pos < total_lines && strchr(lines[*pos], '{')) {
        node->right = LoadTreeFromLines(lines, pos, total_lines);
    }

    while (*pos < total_lines && strchr(lines[*pos], '}') == 0) {
        (*pos)++;
    }
    if (*pos < total_lines) (*pos)++;

    return node;
}


TreeNode* LoadTreeFromFile(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Ошибка при открытии файла");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    long file_size = ftell(file);
    rewind(file);

    char* buffer = (char*)calloc((size_t)file_size + 1, sizeof(char));
    if (!buffer) {
        perror("Ошибка выделения памяти");
        fclose(file);
        return NULL;
    }

    fread(buffer, sizeof(char), (size_t)file_size, file);
    fclose(file);

    int strings_num = 0;
    for (long i = 0; i < file_size; i++) {
        if (buffer[i] == '\n') strings_num++;
    }
    strings_num++;

    for (int i = 0; i < file_size; i++) {
        if (buffer[i] == '\n') {
            buffer[i] = '\0';
        }
    }

    char** lines = (char**)calloc((size_t)strings_num, sizeof(char*));
    if (!lines) {
        perror("Ошибка выделения памяти под массив строк");
        free(buffer);
        return NULL;
    }

    create_ptr_arr(buffer, strings_num, lines);

    // Чистим строки от \n и пробелов
    for (int i = 0; i < strings_num; i++) {
        string_separator(lines[i]);
    }

    int pos = 0;
    TreeNode* root = LoadTreeFromLines(lines, &pos, strings_num);

    free(buffer);
    free(lines);
    return root;
}
