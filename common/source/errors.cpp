#include <stdio.h>

#include "errors.h"

void PrintErr(unsigned int res)
{
    if (res)
        printf("Errors:\n");
    if (res & NULL_PTR)
        printf("Null pointer.\n");
    if (res & UNKNOWN_ERROR)
        printf("Unknown error.\n");
    if (res & FILE_NULL_PTR)
        printf("File opening error.\n");
    if (res & WRONG_ARG)
        printf("Wrong argument.\n");

    printf("\n");
}
