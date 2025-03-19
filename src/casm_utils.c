#include "casm_utils.h"
#include <string.h>
#include <ctype.h>
#include <stdio.h>

/* ============================================================================================================
                                           Local functions
============================================================================================================ */


/* ============================================================================================================
                                           Global functions
============================================================================================================ */

void trim_whitespace(char *str)
{
    char *start = str;
    while (isspace((unsigned char)*start)) start++;

    char *end = str + strlen(str) - 1;
    while (end > start && isspace((unsigned char)*end)) end--;

    memmove(str, start, end - start + 1);
    str[end - start + 1] = '\0';
}

void print_binary(uint16_t num)
{
    for (int i = sizeof(num) * 8 - 1; i >= 0; i--) {
        putchar((num & (1 << i)) ? '1' : '0');
    }
}
