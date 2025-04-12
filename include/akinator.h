#ifndef AKINATOR_H
#define AKINATOR_H
#include "tree.h"
#include "stack.h"
bool is_yes(const char* str);

bool is_question(const char* str);

bool is_no(const char* str);

int AkinatorTraversal(TreeNode** node);

TreeNode** processing_no_input(char* input, TreeNode** node);

TreeNode** processing_bad_input(char* input, TreeNode** node);

int FindObjectWithPath(TreeNode* Node, Stack* stk, const char* target, char* path);

int AkinatorDefinition(Stack* stk, elem_t word);

#endif //AKINATOR_H
