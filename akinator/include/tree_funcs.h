#ifndef TREE_FUNCS_H
#define TREE_FUNCS_H

#include <stdio.h>

#include "../../common/include/errors.h"
#include "../../common/include/colors.h"
#include "tree.h"

static const char *HTML_FILE_NAME = "dump.html";
static const char *BASE_FILE_NAME = "base.txt";

Node_t  *CreateNode    (Node_t *left, Node_t *right, Node_t *parent);
errors_t DestroySubTree(Node_t *node);
errors_t ReadBase      (Tree_t *tree);
errors_t WriteBase     (Tree_t *tree);
errors_t WriteNode     (Node_t *node, FILE *base_file);
errors_t CreateTree    (Tree_t *tree);
errors_t DestroyTree   (Tree_t *tree);

errors_t fGetWord(char *word, int max_size, FILE *stream);
const int SCAN_STRING_LEN = 30;

#define GET_WORD(WORD, MAX_SIZE) fGetWord(WORD, MAX_SIZE, stdin)

#endif
