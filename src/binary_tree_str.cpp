#include <stdio.h>
#include <assert.h>
#include "tree.h"
#include <string.h>
#include <ctype.h>
#include <stdlib.h>


void remove_curly_braces(char* dest, const char* src) {
    while (*src) {
        if (*src != '{' && *src != '}') {
            *dest++ = *src;
        }
        src++;
    }
    *dest = '\0';
}

int DumpTreeToDot(TreeNode* node, FILE* file) {
    assert(file != NULL);
    if (node == NULL) return SUCCESS_DONE;

    char cleaned_value[INPUT_SIZE];
    remove_curly_braces(cleaned_value, node->value);
    string_separator(cleaned_value);

    fprintf(file, "    node%p [label=\"%s\"]\n", node, cleaned_value);
    fprintf(file, "    node%p [shape= \"Mrecord\", label= \"{value: %s | current: %p  |  {left: %p | right: %p}}\", style = \"filled\", fillcolor= \"#FFD7D3\" ];\n ",
            node, cleaned_value, node, node->left, node->right);

    if (node->left) {
        fprintf(file, "    node%p -> node%p\n", node, node->left);
        DumpTreeToDot(node->left, file);
    }

    if (node->right) {
        fprintf(file, "    node%p -> node%p\n", node, node->right);
        DumpTreeToDot(node->right, file);
    }

    return SUCCESS_DONE;
}




int CreateGraphvizFile(TreeNode* Root, const char* filename) {
    assert(filename != NULL);
    FILE* file = fopen(filename, "w");
    if (file == NULL) {
        perror("Ошибка при открытии файла");
        return FILE_OPENING_ERR;
    }

    fprintf(file, "digraph BinaryTree {\n");
    fprintf(file, "    node [shape=circle, style=filled, fillcolor=lightblue];\n");

    DumpTreeToDot(Root, file);

    fprintf(file, "}\n");
    fclose(file);
    return SUCCESS_DONE;
}

int CreateNode(TreeNode** Node, elem_t value) {
    *Node = (TreeNode*)calloc(1, sizeof(TreeNode));
    if (*Node == NULL) return ALLOC_ERROR;
    (*Node)->value = strdup(value);
    if ((*Node)->value == NULL) {
        free(*Node);
        return ALLOC_ERROR;
    }
    (*Node)->left = NULL;
    (*Node)->right = NULL;
    return SUCCESS_DONE;

}
int FreeTree(TreeNode **Node) {
    if (*Node == NULL) return SUCCESS_DONE;


    FreeTree(&(*Node)->left);
    FreeTree(&(*Node)->right);

    free((*Node)->value);
    free(*Node);
    *Node = NULL;

    return SUCCESS_DONE;

}

int InsertNode(TreeNode **Root, elem_t value) {
    if (*Root == NULL) return CreateNode(Root, value);

    if (strcmp(value, (*Root)->value) < 0) {
        return InsertNode(&((*Root)->left), value);
    }
    else {
        return InsertNode(&((*Root)->right), value);
    }

    return SUCCESS_DONE;
}



TreeNode* NodeFind(TreeNode* Root, elem_t value) {
    if (Root == NULL) {
        return NULL;
    }
    if (strcmp(Root->value, value) == 0) return Root;

    if (strcmp(value, Root->value) < 0) {
        return NodeFind(Root->left, value);
    }
    else {
        return NodeFind(Root->right, value);
    }
}

int NodeType(TreeNode* Root, elem_t value) {
    assert(Root != NULL);
    printf("value in NodeFind = %s\n", value);
    TreeNode* found_node = NodeFind(Root, value);
    if (found_node) {
        if (strstr(value, "?") &&  (found_node->left != NULL && found_node != NULL)) {
            printf("left node = %p, right node = %p\n", found_node->left, found_node->right);
            printf("indication with value %s identified\n", value);
            return INDICATION;
        }
        else {
            printf("object with value %s identified\n", value);
            return OBJECT;
        }
    }
    else {
        printf("error in NodeType, \n");
        return NODE_NULLPTR;
    }
}


int TreeTraversal(TreeNode* Node) {

    if (Node == NULL) {
        return NODE_NULLPTR;

    }

    TreeTraversal(Node->left);
    printf("%s ", Node->value);
    TreeTraversal(Node->right);
    return SUCCESS_DONE;
}


int string_separator(char* str) {
    assert(str != NULL);
    size_t len = strlen(str);

    if (len > 0 && str[len - 1] == '\n') {
        str[len - 1] = '\0';
        len--;
    }

    char *src = str;
    while (*src == ' ') {
        src++;
    }

    if (src != str) {
        char *dst = str;
        while (*src != '\0') {
            *dst++ = *src++;
        }
        *dst = '\0';
    }

    return SUCCESS_DONE;
}

