#ifndef DUMP_H
#define DUMP_H

#include "tree.h"

static const char *DOT_FILE_NAME = "dump.dot";
const int CMD_LEN = 40;
const int SVG_FILE_LEN = 18;

errors_t DumpTree(Tree_t *tree, const char* filename, const char *funcname, int line);
errors_t DrawTree(Tree_t *tree, FILE *dot_file);
errors_t DrawNode(Node_t *node, FILE *dot_file);

#define DUMP_TREE(TREE) DumpTree(TREE, __FILE__, __func__, __LINE__)

#endif
