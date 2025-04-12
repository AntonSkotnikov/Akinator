#ifndef TREE_H
#define TREE_H
#include <stdio.h>
//typedef int elem_t;
typedef char* elem_t;

struct TreeNode {
    elem_t value = nullptr;
    struct TreeNode* left = nullptr;
    struct TreeNode* right = nullptr;

};

enum errors {
    SUCCESS_DONE = 0,
    ALLOC_ERROR = 1,
    NODE_NULLPTR = 2,
    FILE_OPENING_ERR = -1,
    INPUT_ERR = 3

};

const int INDICATION = 51;
const int OBJECT = 50;
const int TREE_SEPARATOR_SIZE = 100;
const int INPUT_SIZE = 100;

int my_strcmp_begin(const char* str1, const char* str2);
int CreateNode(TreeNode** Node, elem_t value);
int FreeTree(TreeNode** Node);
int InsertNode(TreeNode** Root, elem_t value);
TreeNode* NodeFind(TreeNode* Root, elem_t value);
int TreeTraversal(TreeNode* Node);
int CreateGraphvizFile(TreeNode* Root, const char* filename);
int NodeType(TreeNode* Root, elem_t value);
void remove_curly_braces(char* dest, const char* src);
int SaveTree(TreeNode* node, FILE* file, int depth);

int SaveTreeToFile(TreeNode* root, const char* filename);

TreeNode* LoadTree(FILE* file);

TreeNode* LoadTreeFromFile(const char* filename);

int string_separator(char* str);


#endif //TREE_H
