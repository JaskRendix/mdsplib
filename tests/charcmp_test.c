#include <stdio.h>

#include "local.h"

MDSP_BOOL charcmp(char *str, char *pattern);

#define CHECK(expr) \
    do { \
        if (!(expr)) { \
            fprintf(stderr, "FAILED: %s (line %d)\n", \
                    #expr, __LINE__); \
            return 0; \
        } \
    } while (0)

static int test_charcmp(void)
{
    CHECK(charcmp(NULL, "a") == FALSE);
    CHECK(charcmp("A", NULL) == FALSE);

    CHECK(charcmp("A", "a") == TRUE);
    CHECK(charcmp("1", "a") == FALSE);

    CHECK(charcmp("5", "d") == TRUE);
    CHECK(charcmp("A", "d") == FALSE);

    CHECK(charcmp("A", "c") == TRUE);
    CHECK(charcmp("5", "c") == TRUE);
    CHECK(charcmp("@", "c") == FALSE);

    CHECK(charcmp("a", "i") == TRUE);
    CHECK(charcmp("A", "i") == FALSE);

    CHECK(charcmp("A", "u") == TRUE);
    CHECK(charcmp("a", "u") == FALSE);

    CHECK(charcmp(" ", "s") == TRUE);
    CHECK(charcmp("A", "s") == FALSE);

    CHECK(charcmp("\t", "w") == TRUE);
    CHECK(charcmp("A", "w") == FALSE);

    CHECK(charcmp("   ", "m") == TRUE);
    CHECK(charcmp("\t\t", "m") == TRUE);
    CHECK(charcmp("A", "m") == FALSE);

    CHECK(charcmp("!", "t") == TRUE);
    CHECK(charcmp("A", "t") == FALSE);

    CHECK(charcmp("A", "p") == TRUE);

    CHECK(charcmp("A", "g") == TRUE);
    CHECK(charcmp(" ", "g") == FALSE);

    CHECK(charcmp("X", "'X") == TRUE);
    CHECK(charcmp("Y", "'X") == FALSE);

    CHECK(charcmp("Ab3", "aad") == TRUE);
    CHECK(charcmp("A33", "aad") == FALSE);

    CHECK(charcmp("", "a") == FALSE);

    CHECK(charcmp("A", "z") == FALSE);

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
