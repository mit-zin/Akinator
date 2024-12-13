#include "../../common/include/errors.h"
#include "../../common/include/colors.h"
#include "../include/tree.h"
#include "../include/tree_funcs.h"
#include "../include/dump.h"
#include "../include/game.h"

int main()
{
    Tree_t tree = {};

    unsigned int res = 0;

    WRITE_ER(CreateTree(&tree));
    WRITE_ER(ReadBase(&tree));

    WRITE_ER(PlayGame(&tree));


    //Definition(&tree);

    WRITE_ER(WriteBase(&tree));

    WRITE_ER(DestroyTree(&tree));

    PrintErr(res);

    PRINT_COL(ITALICS, "End of program.\n");


    return 0;
}
