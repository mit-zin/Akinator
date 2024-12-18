#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../include/tree.h"
#include "../include/tree_funcs.h"
#include "../include/dump.h"
#include "../../common/include/errors.h"

errors_t CreateTree(Tree_t *tree)
{
    MY_ASSERT(tree, "Null pointer given as argument.", return NULL_PTR);

    tree->html_file = fopen(HTML_FILE_NAME, "w");
    if (!tree->html_file)
        return FILE_NULL_PTR;

    fprintf(tree->html_file, "<DOCTYPE html>\n<html>\n\t<head>\n\t\t<title>Dump</title>\n\t"
                              "</head>\n\t<body>\n");

    return SUCCESS;
}

Node_t *CreateNode(Node_t *left, Node_t *right, Node_t *parent)
{
    Node_t *node = (Node_t *) calloc(1, sizeof(Node_t));
    if (!node)
        return NULL;

    node->data = (Data_t) calloc(DATA_SIZE, sizeof(char));
    if (!node->data)
        exit(EXIT_FAILURE);

    node->left = left;
    node->right = right;
    node->parent = parent;

    return node;
}

errors_t DestroySubTree(Node_t *node)
{
    MY_ASSERT(node, "Null pointer given as argument.", return NULL_PTR);

    if (node->left)
        CHECK_ER(DestroySubTree(node->left));

    if (node->right)
        CHECK_ER(DestroySubTree(node->right));

    free(node->data);
    free(node);

    return SUCCESS;
}

errors_t DestroyTree(Tree_t *tree)
{
    MY_ASSERT(tree, "Null pointer given as argument.", return NULL_PTR);

    CHECK_ER(DUMP_TREE(tree));

    fprintf(tree->html_file, "\t</body>\n</html>\n");

    fclose(tree->html_file);

    CHECK_ER(DestroySubTree(tree->root));

    return SUCCESS;
}

errors_t WriteNode(Node_t *node, FILE *base_file)
{
    MY_ASSERT(node, "Null pointer given as argument.", return NULL_PTR);
    MY_ASSERT(base_file, "Null pointer given as argument.", return NULL_PTR);

    static int depth = 0;

    for (int i = 0; i < depth; i++)
        fputc('\t', base_file);

    fprintf(base_file, "{\n");
    for (int i = 0; i < depth + 1; i++)
        fputc('\t', base_file);

    fprintf(base_file, "%s\n", node->data);

    depth++;

    if (node->left)
        CHECK_ER(WriteNode(node->left, base_file));
    if (node->right)
        CHECK_ER(WriteNode(node->right, base_file));

    depth--;

    for (int i = 0; i < depth; i++)
        fputc('\t', base_file);

    fprintf(base_file, "}\n");


    return SUCCESS;
}

errors_t WriteBase(Tree_t *tree)
{
    MY_ASSERT(tree, "Null pointer given as argument.", return NULL_PTR);

    FILE *base_file = fopen(BASE_FILE_NAME, "wb");
    if (!base_file)
        return FILE_NULL_PTR;

    CHECK_ER(WriteNode(tree->root, base_file));

    fclose(base_file);

    return SUCCESS;
}

errors_t ReadBase(Tree_t *tree)
{
    MY_ASSERT(tree, "Null pointer given as argument.", return NULL_PTR);

    FILE *base_file = fopen(BASE_FILE_NAME, "rb");
    if (!base_file)
        return FILE_NULL_PTR;

    tree->root = CreateNode(NULL, NULL, NULL);

    fgetc(base_file);
    CHECK_ER(fGetWord(tree->root->data, DATA_SIZE, base_file));

    Node_t *node = tree->root;

    while (fscanf(base_file, "%*[^{}]") != EOF)
    {
        char bracket = (char) fgetc(base_file);

        if (bracket == '{')
        {
            CHECK_ER(DUMP_TREE(tree));

            if (!node->left)
            {
                node->left = CreateNode(NULL, NULL, node);

                CHECK_ER(fGetWord(node->left->data, DATA_SIZE, base_file));
                node = node->left;
            }
            else
            {
                node->right = CreateNode(NULL, NULL, node);

                CHECK_ER(fGetWord(node->right->data, DATA_SIZE, base_file));
                node = node->right;
            }
        }
        else
        {
            if (node == tree->root)
                break;
            node = node->parent;
        }
    }

    fclose(base_file);

    return SUCCESS;
}

errors_t fGetWord(char *word, int max_size, FILE *stream)
{
    MY_ASSERT(word, "Null pointer given as argument.", return NULL_PTR);

    char scan_string[SCAN_STRING_LEN] = {};
    sprintf(scan_string, "%%*[ \t\n\r]%%%d[^\n]", max_size - 1);

    FSCAN_COL(ORNG, stream, scan_string, word);

    return SUCCESS;
}
