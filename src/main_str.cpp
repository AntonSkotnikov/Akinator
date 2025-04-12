#include <stdio.h>
#include "tree.h"
#include "akinator.h"
#include <stdlib.h>
#include <string.h>
#include "stack.h"
int main() {
    TreeNode *Root = NULL;

    Root = LoadTreeFromFile("akinator_tree.txt");
    Stack stk = {};
    const int CAPACITY = 20;
    StackConstructor(&stk, CAPACITY);
    if (!Root) {
        printf("Файл не найден, создаём новое дерево:\n");
        CreateNode(&Root, strdup("человек?"));
        CreateNode(&(Root->right), strdup("полторашка"));  // Пустой левый потомок
        CreateNode(&(Root->left), strdup("Овчинкин")); // Пустой правый потомок
    }

    //AkinatorTraversal(&Root);
    SaveTreeToFile(Root, "akinator_tree.txt");

    printf("Введи объект, который хочешь найти: ");
    char target[INPUT_SIZE] = {};
    scanf("%s", target);
    char path[50] = {};
    FindObjectWithPath(Root, &stk, target, path);
    StackPush(&stk, Root);
    StackDump(&stk);
    //TreeNode* smth = StackPop(&stk);
    //printf("val = %s\n", smth->value);
    AkinatorDefinition(&stk, target);
    CreateGraphvizFile(Root, "graph.dot");
    system("/opt/homebrew/bin/dot graph.dot -T png -o graph.png");

    FreeTree(&Root);
    StackDestructor(&stk);
    return 0;
}

