#include <stdio.h>
#include "tree.h"
#include "akinator.h"
#include <stdlib.h>
#include <string.h>
#include "stack.h"

int main() {
    TreeNode *Root = NULL;
    Root = LoadTreeFromFile("akinator_tree.txt");

    const int CAPACITY = 20;
    if (!Root) {
        printf("Файл не найден, создаём новое дерево:\n");
        CreateNode(&Root, strdup("человек?"));
        CreateNode(&(Root->right), strdup("полторашка"));
        CreateNode(&(Root->left), strdup("Овчинкин"));
    }

    char choice[GAME_INPUT];
    int running = 1;
    int cleaning_val = 0;
    while (running) {
        printf("\nВыбери режим:\n");
        printf("T - Traversal\n");
        printf("D - Difference\n");
        printf("M - Meaning (Definition)\n");
        printf("Q - Quit\n");
        printf(">> ");

        if (!fgets(choice, sizeof(choice), stdin)) {
            break;
        }

        switch (choice[0]) {
            case 'T': case 't': {
                AkinatorTraversal(&Root);
                break;
            }
            case 'D': case 'd': {
                char word_1[INPUT_SIZE], word_2[INPUT_SIZE];
                printf("Введи два объекта для сравнения (через пробел): ");
                if (scanf("%s %s", word_1, word_2) == 2) {

                    while ((cleaning_val = getchar()) != '\n' && cleaning_val != EOF);  // чистим остаток строки

                    Stack stk1 = {}, stk2 = {};
                    StackConstructor(&stk1, CAPACITY);
                    StackConstructor(&stk2, CAPACITY);

                    AkinatorDifference(Root, &stk1, &stk2, word_1, word_2);

                    StackDestructor(&stk1);
                    StackDestructor(&stk2);
                } else {
                    printf("Неверный ввод.\n");
                    while ((cleaning_val = getchar()) != '\n' && cleaning_val != EOF);
                }
                break;
            }
            case 'M': case 'm': {
                char target[INPUT_SIZE] = {};
                printf("Введи объект для определения: ");
                if (fgets(target, sizeof(target), stdin)) {
                    target[strcspn(target, "\n")] = '\0';
                    Stack stk = {};
                    StackConstructor(&stk, CAPACITY);
                    AkinatorDefinition(Root, &stk, target);
                    StackDestructor(&stk);
                }
                break;
            }
            case 'Q': case 'q': {
                running = 0;
                break;
            }
            default:
                printf("Неверный выбор — попробуй ещё раз.\n");
        }

        if (running) {
            SaveTreeToFile(Root, "akinator_tree.txt");
            CreateGraphvizFile(Root, "graph.dot");
            system("/opt/homebrew/bin/dot graph.dot -T png -o graph.png");
        }
    }

    FreeTree(&Root);
    return 0;
}
