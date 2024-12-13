#ifndef TREE_H
#define TREE_H

#include <cstdio>

typedef char *Data_t;

typedef struct Node_t
{
    Data_t data;
    Node_t *left, *right, *parent;
} Node_t;

const int DATA_SIZE = 101;

typedef struct
{
    Node_t *root;
    FILE *html_file;
} Tree_t;

#endif
