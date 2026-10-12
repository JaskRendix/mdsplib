#include <stdio.h>

#include "local.h"

MDSP_BOOL charcmp(char *str, char *pattern);

static int test_charcmp(void)
{
    /* NULL checks */
    if (charcmp(NULL, "a") != FALSE)
        return 0;

    if (charcmp("A", NULL) != FALSE)
        return 0;

    /* a = alphabetic */
    if (charcmp("A", "a") != TRUE)
        return 0;

    if (charcmp("1", "a") != FALSE)
        return 0;

    /* d = digit */
    if (charcmp("5", "d") != TRUE)
        return 0;

    if (charcmp("A", "d") != FALSE)
        return 0;

    /* c = alphanumeric */
    if (charcmp("A", "c") != TRUE)
        return 0;

    if (charcmp("5", "c") != TRUE)
        return 0;

    if (charcmp("@", "c") != FALSE)
        return 0;

    /* i = lowercase */
    if (charcmp("a", "i") != TRUE)
        return 0;

    if (charcmp("A", "i") != FALSE)
        return 0;

    /* u = uppercase */
    if (charcmp("A", "u") != TRUE)
        return 0;

    if (charcmp("a", "u") != FALSE)
        return 0;

    /* s = literal space */
    if (charcmp(" ", "s") != TRUE)
        return 0;

    if (charcmp("A", "s") != FALSE)
        return 0;

    /* w = any whitespace */
    if (charcmp("\t", "w") != TRUE)
        return 0;

    if (charcmp("A", "w") != FALSE)
        return 0;

    /* m = one or more whitespace */
    if (charcmp("   ", "m") != TRUE)
        return 0;

    if (charcmp("\t\t", "m") != TRUE)
        return 0;

    if (charcmp("A", "m") != FALSE)
        return 0;

    /* t = punctuation */
    if (charcmp("!", "t") != TRUE)
        return 0;

    if (charcmp("A", "t") != FALSE)
        return 0;

    /* p = printable */
    if (charcmp("A", "p") != TRUE)
        return 0;

    /* g = graphical */
    if (charcmp("A", "g") != TRUE)
        return 0;

    if (charcmp(" ", "g") != FALSE)
        return 0;

    /* quoted literal */
    if (charcmp("X", "'X") != TRUE)
        return 0;

    if (charcmp("Y", "'X") != FALSE)
        return 0;

    /* multiple pattern elements */
    if (charcmp("Ab3", "aad") != TRUE)
        return 0;

    if (charcmp("A33", "aad") != FALSE)
        return 0;

    /* short string */
    if (charcmp("", "a") != FALSE)
        return 0;

    /* invalid pattern */
    if (charcmp("A", "z") != FALSE)
        return 0;

    return 1;
}

int main(void)
{
    if (!test_charcmp()) {
        fprintf(stderr, "charcmp test failed\n");
        return 1;
    }

    return 0;
}
