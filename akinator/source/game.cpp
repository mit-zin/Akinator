#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "../include/game.h"

errors_t PlayGame(Tree_t *tree)
{
    MY_ASSERT(tree, "Null pointer given as argument.", return NULL_PTR);

    DUMP_TREE(tree);

    int choice = 0;
    do
    {
        printf("\nЧтобы начать игру введите 1\nЧтобы узнать определение введите 2\n"
                  "Чтобы закончить введите 3\nЧтобы получить сравнение введите 4\n\n");

        SET_COL(ORNG);
        while (!scanf("%d", &choice) || choice < ANS_1 || choice > ANS_4)
        {
            PRINT_COL(PALE_GRN, "\nМожно вводить только 1, 2, 3 или 4.\n\n");
            SET_COL(ORNG);
            while (getchar() != '\n');
        }

        switch (choice)
        {
            case ANS_1:
                CHECK_ER(Ask(tree->root));
                break;
            case ANS_2:
                CHECK_ER(Definition(tree));
                break;
            case ANS_3:
                PRINT_COL(PALE_GRN, "\nСпасибо за игру!\n\n");
                break;
            case ANS_4:
                CHECK_ER(CompareCharacters(tree));
                break;
            default:
                return UNKNOWN_ERROR;
        }
        if (choice != ANS_3)
        {
            getchar();
            getchar();
        }

        DUMP_TREE(tree);

    } while (choice != ANS_3);

    return SUCCESS;
}

errors_t Ask(Node_t *node)
{
    MY_ASSERT(node, "Null pointer given as argument.", return NULL_PTR);

    if (!node->left && !node->right)
    {
        PRINT_COL(PALE_GRN, "\nЭто %s! Угадал?\n\n", node->data);

        char input[INP_LEN] = {};

        CHECK_ER(GET_WORD(input, INP_LEN));

        while (strcmp(input, "да") && strcmp(input, "нет"))
        {
            PRINT_COL(PALE_GRN, "\nПрости, не понимаю тебя.\n\n");
            CHECK_ER(GET_WORD(input, INP_LEN));
        }

        if (strcmp("да", input))
            CHECK_ER(AddAnswer(node));
        else
            PRINT_COL(PALE_GRN, "\nЭто было легче лёгкого └(^▼^)┘\n\n");

        return SUCCESS;
    }

    PRINT_COL(PALE_GRN, "\nЭто %s?\n\n", node->data);

    char input[INP_LEN] = {};

    CHECK_ER(GET_WORD(input, INP_LEN));
    while (strcmp(input, "да") && strcmp(input, "нет"))
    {
        PRINT_COL(PALE_GRN, "\nПрости, не понимаю тебя.\n\n");

        CHECK_ER(GET_WORD(input, INP_LEN));
    }

    if (!strcmp(input, "да"))
        CHECK_ER(Ask(node->right));
    else
        CHECK_ER(Ask(node->left));

    return SUCCESS;
}

errors_t AddAnswer(Node_t *node)
{
    MY_ASSERT(node, "Null pointer given as argument.", return NULL_PTR);

    PRINT_COL(PALE_GRN, "\nЧто тогда ты загадал?\n\n");

    if (node->parent->left == node)
        node->parent = node->parent->left = CreateNode(node, CreateNode(NULL, NULL, NULL), node->parent);
    else
        node->parent = node->parent->right = CreateNode(node, CreateNode(NULL, NULL, NULL), node->parent);

    node = node->parent;
    node->right->parent = node;
    CHECK_ER(GET_WORD(node->right->data, DATA_SIZE));

    PRINT_COL(PALE_GRN, "\nЧем %s отличается от %s?\n\n", node->right->data, node->left->data);

    CHECK_ER(GET_WORD(node->data, DATA_SIZE));

    PRINT_COL(PALE_GRN, "\nОк, я запомнил.\n\n");

    return SUCCESS;
}


errors_t Definition(Tree_t *tree)
{
    MY_ASSERT(tree, "Null pointer given as argument.", return NULL_PTR);

    char *word = (char *) calloc(DATA_SIZE, sizeof(char));
    if (!word)
        return NULL_PTR;

    CHECK_ER(GET_WORD(word, DATA_SIZE));

    while (!PrintDef(tree->root, word))
    {
        PRINT_COL(PALE_GRN, "\nЯ не знаю, что это :(\n\n");

        CHECK_ER(GET_WORD(word, DATA_SIZE));
    }

    printf("\n\n");

    free(word);

    return SUCCESS;
}

rec_res_t PrintDef(Node_t *node, const char *word)
{
    MY_ASSERT(node, "Null pointer given as argument.", exit(EXIT_FAILURE));
    MY_ASSERT(word, "Null pointer given as argument.", exit(EXIT_FAILURE));

    if (node->left && PrintDef(node->left, word))
    {
        PRINT_COL(PALE_GRN, "не %s ", node->data);
        return RIGHT_SUBTREE;
    }
    if (node->right && PrintDef(node->right, word))
    {
        PRINT_COL(PALE_GRN, "%s ", node->data);
        return RIGHT_SUBTREE;
    }

    if (!node->left && !node->right)
    {
        if (!strcmp(node->data, word))
        {
            PRINT_COL(PALE_GRN, "\n%s это ", word);
            return RIGHT_SUBTREE;
        }
    }

    return WRONG_SUBTREE;
}

errors_t CompareCharacters(Tree_t *tree)
{
    MY_ASSERT(tree, "Null pointer given as argument.", return NULL_PTR);

    char *word1 = (char *) calloc(DATA_SIZE, sizeof(char));
    char *word2 = (char *) calloc(DATA_SIZE, sizeof(char));
    if (!word2 || !word1)
        return NULL_PTR;

    CHECK_ER(GET_WORD(word1, DATA_SIZE));
    CHECK_ER(GET_WORD(word2, DATA_SIZE));

    while (PrintCmp(tree->root, word1, word2) != BOTH_FOUND)
    {
        PRINT_COL(PALE_GRN, "\nЯ не всё из этого знаю :(\n\n");

        CHECK_ER(GET_WORD(word1, DATA_SIZE));
        CHECK_ER(GET_WORD(word2, DATA_SIZE));
    }
    printf("\n\n");

    free(word1);
    free(word2);

    return SUCCESS;
}

print_cmp_res_t PrintCmp(Node_t *node, const char *word1, const char *word2)
{
    MY_ASSERT(node, "Null pointer given as argument.", exit(EXIT_FAILURE));
    MY_ASSERT(word1, "Null pointer given as argument.", exit(EXIT_FAILURE));
    MY_ASSERT(word2, "Null pointer given as argument.", exit(EXIT_FAILURE));

    if (!node->left && !node->right)
    {
        if (!strcmp(node->data, word1))
            return WORD_1_FOUND;
        if (!strcmp(node->data, word2))
            return WORD_2_FOUND;
        return NOTHING_FOUND;
    }

    switch(PrintCmp(node->left, word1, word2))
    {
        case WORD_1_FOUND :
            if (PrintCmp(node->right, word1, word2) == WORD_2_FOUND && node->parent)
            {
                PRINT_COL(PALE_GRN, "\n%s похож на %s тем, что %s ", word1, word2, word1);
                return BOTH_FOUND;
            }
            else
                return WORD_1_FOUND;
        case WORD_2_FOUND :
            if (PrintCmp(node->right, word1, word2) == WORD_1_FOUND && node->parent)
            {
                PRINT_COL(PALE_GRN, "\n%s похож на %s тем, что %s ", word1, word2, word1);
                return BOTH_FOUND;
            }
            else
                return WORD_2_FOUND;
        case BOTH_FOUND :
            PRINT_COL(PALE_GRN, "не %s ", node->data);
            return BOTH_FOUND;
        case NOTHING_FOUND :
        {
            print_cmp_res_t res = PrintCmp(node->right, word1, word2);
            if (res == BOTH_FOUND)
            {
                PRINT_COL(PALE_GRN, "%s ", node->data);
            }
            return res;
        }
        default :
            fprintf(stderr, "Default case\nFile: %s\nFunction: %s\nLine: %d\n",
                    __FILE__, __func__, __LINE__);
            exit(EXIT_FAILURE);
    }

    return NOTHING_FOUND;
}


