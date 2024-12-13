#ifndef TREE_FUNCS_H
#define TREE_FUNCS_H

#include "../../common/include/errors.h"
#include "tree.h"

static const char *HTML_FILE_NAME = "dump.html";

Node_t  *CreateNode    (Node_t *left, Node_t *right, Node_t *parent);
errors_t DestroySubTree(Node_t *node);
errors_t ReadBase      (Tree_t *tree);
errors_t WriteBase     (Tree_t *tree);
errors_t WriteNode     (Node_t *node, FILE *base_file);
errors_t CreateTree    (Tree_t *tree);
errors_t DestroyTree   (Tree_t *tree);

#endif
