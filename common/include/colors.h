#ifndef COLORS_H
#define COLORS_H

#define ORNG     "\033[0;38;5;214m"
#define PALE_GRN "\033[0;38;5;151m"
#define NORMAL   "\033[0m"
#define ITALICS  "\033[0;3m"

#define PRINT_COL(COL, ...) do {printf(COL __VA_ARGS__); printf(NORMAL);}while(0)
#define SCAN_COL(COL, ...)  do       \
    {                                \
        printf(COL);                 \
        scanf(__VA_ARGS__);          \
        printf(NORMAL);              \
    } while(0)


#define SET_COL(COL) printf(COL)

#endif
