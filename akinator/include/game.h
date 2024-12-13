#ifndef GAME_H
#define GAME_H

#include "../../common/include/errors.h"
#include "../../common/include/colors.h"
#include "tree.h"
#include "tree_funcs.h"
#include "dump.h"

const int INP_LEN = 101;

errors_t PlayGame(Tree_t *tree);
errors_t Ask(Node_t *node);

typedef enum
{
    WRONG_SUBTREE = 0,
    RIGHT_SUBTREE = 1
} rec_res_t;

typedef enum
{
    WORD_1_FOUND,
    WORD_2_FOUND,
    BOTH_FOUND,
    NOTHING_FOUND
} print_cmp_res_t;

enum choice_t
{
    ANS_1 = 1,
    ANS_2,
    ANS_3,
    ANS_4
};

//errors_t Suggest(Tree_t *tree);

errors_t  Definition(Tree_t *tree);
rec_res_t PrintDef(Node_t *node, const char *word);

errors_t CompareCharacters(Tree_t *tree);
print_cmp_res_t PrintCmp(Node_t *node, const char *word1, const char *word2);

errors_t GetWord(char *word, int max_size);
const int SCAN_STRING_LEN = 30;

errors_t AddAnswer(Node_t *node);

#endif
