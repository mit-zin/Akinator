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
    //MY_ASSERT(node, "Null pointer given as argument.", exit(EXIT_FAILURE));

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

    fprintf(base_file, "\"%s\"\n", node->data);

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

    FILE *base_file = fopen("base.txt", "wb");
    if (!base_file)
        return FILE_NULL_PTR;

    CHECK_ER(WriteNode(tree->root, base_file));

    fclose(base_file);

    return SUCCESS;
}

errors_t ReadBase(Tree_t *tree)
{
    MY_ASSERT(tree, "Null pointer given as argument.", return NULL_PTR);

    FILE *base_file = fopen("base.txt", "rb");
    if (!base_file)
        return FILE_NULL_PTR;

    tree->root = CreateNode(NULL, NULL, NULL);

    fscanf(base_file, "{ \"%100[^\"]\"", tree->root->data);
    Node_t *node = tree->root;

    while (fscanf(base_file, "%*[^{}]") != EOF)
    {
        char bracket = (char) fgetc(base_file);

        if (bracket == '{')
        {
            CHECK_ER(DUMP_TREE(tree));

            char *string = (char *) calloc(DATA_SIZE, sizeof(char));
            if (!string)
                return NULL_PTR;
            fscanf(base_file, " \"%100[^\"]\" ", string);

            if (!node->left)
            {
                node->left = CreateNode(NULL, NULL, node);//TODO rename

                memcpy(node->left->data, string, DATA_SIZE);
                node = node->left;
            }
            else
            {
                node->right = CreateNode(NULL, NULL, node);

                memcpy(node->right->data, string, DATA_SIZE);
                node = node->right;
            }

            free(string);
        }
        else
        {
            /*printf("%p ", node);
            printf("%p ", node->parent);
            printf("%p ", node->left);
            printf("%p\n", node->right);*/
            if (node == tree->root)
                break;
            node = node->parent;
        }
    }

    fclose(base_file);

    return SUCCESS;
}

