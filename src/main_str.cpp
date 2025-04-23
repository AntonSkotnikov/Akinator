#include <stdio.h>
#include "tree.h"
#include "akinator.h"
#include <stdlib.h>
#include <string.h>
#include "stack.h"
#include "colors.h"

int main() {
    TreeNode *Root = NULL;
    Root = LoadTreeFromFile("akinator_tree.txt");

    const int CAPACITY = 20;
    if (!Root) {
        printf("Файл не найден, создаём новое дерево:\n");
        CreateNode(&Root, strdup("человек?"));            //TODO: лучше сделать отдельный файлик, откуда
        CreateNode(&(Root->right), strdup("полторашка")); //бы оно доставало эти три строчки и выполнялось, фактически как база для тестов(то есть примеров)
        CreateNode(&(Root->left), strdup("Овчинкин"));    //
    }
    //TODO: все это обернуть в функцию
    char choice[GAME_INPUT] = {};
    int running = 1;
    int cleaning_val = 0;
    while (running) {
        printf(RED "\nВыбери режим:\n" RED);
        printf(CYAN "T - Traversal\n" CYAN);
        printf(CYAN "D - Difference\n" CYAN);
        printf(CYAN "M - Meaning (Definition)\n" CYAN);
        printf(CYAN "Q - Quit\n" CYAN);
        printf(CYAN ">> " CYAN);   //TODO: и это тоже обернуть в функцию

        if (!fgets(choice, sizeof(choice), stdin)) {
            break;
        }

        switch (choice[0]) {
            case 'T': case 't': {
                AkinatorTraversal(&Root);
                break;
            }
            case 'D': case 'd': {
                char word_1[INPUT_SIZE] = {};
                char word_2[INPUT_SIZE] = {};
                printf(YELLOW "Введи два объекта для сравнения (через пробел): " YELLOW);
                if (scanf("%s %s", word_1, word_2) == 2) {

                    while ((cleaning_val = getchar()) != '\n' && cleaning_val != EOF);  // чистим остаток строки //TODO: в функцию обернуть

                    Stack stk1 = {}, stk2 = {};
                    StackConstructor(&stk1, CAPACITY);
                    StackConstructor(&stk2, CAPACITY);

                    AkinatorDifference(Root, &stk1, &stk2, word_1, word_2);

                    StackDestructor(&stk1);
                    StackDestructor(&stk2);
                } else {
                    printf(YELLOW "Неверный ввод.\n" YELLOW);
                    while ((cleaning_val = getchar()) != '\n' && cleaning_val != EOF);
                }
                break;
            }
            case 'M': case 'm': {
                char target[INPUT_SIZE] = {};
                printf(GREEN "Введи объект для определения: " GREEN);
                if (fgets(target, sizeof(target), stdin) != NULL) {
                    break;
                }
                target[strcspn(target, "\n")] = '\0';

                Stack stk = {};
                StackConstructor(&stk, CAPACITY);
                AkinatorDefinition(Root, &stk, target);
                StackDestructor(&stk);

                break;
            }
            case 'Q': case 'q': {
                running = 0;
                break;
            }
            default:
                printf(MAGENTA "Неверный выбор — попробуй ещё раз.\n" MAGENTA);
        }

        if (running) {
            SaveTreeToFile(Root, "akinator_tree.txt");
            CreateGraphvizFile(Root, "graph.dot");
            system("/opt/homebrew/bin/dot graph.dot -T png -o graph.png");
            //TODO: лучше обновить PATH до bin(добавить /opt/homebrew/bin/dot в директорию)
            //А также проверять, существует ли dot
            //system(...dot --version)
        }
    }

    FreeTree(&Root);
    return 0;
}
