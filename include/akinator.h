#ifndef AKINATOR_H
#define AKINATOR_H
#include "tree.h"
#include "stack.h"

bool is_yes(const char* str);

bool is_question(const char* str);

bool is_no(const char* str);

const int GAME_INPUT = 16;
int AkinatorTraversal(TreeNode** node);

TreeNode** processing_object_input(char* input, TreeNode** node);

TreeNode** processing_bad_input(char* input, TreeNode** node);

int FindObjectWithPath(TreeNode* Node, Stack* stk, const char* target);

int AkinatorDefinition(TreeNode* Node, Stack* stk, const char* target);

int PrintRestDefinition(Stack* stk);

int PrintDifference(Stack* stk_1, Stack* stk_2, char* temp_val, elem_t word_1, elem_t word_2);

int AkinatorDifference(TreeNode* Node, Stack* stk_1, Stack* stk_2, elem_t word_1, elem_t word_2);

#endif //AKINATOR_H
