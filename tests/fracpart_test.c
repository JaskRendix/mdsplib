#include <stdio.h>
#include <math.h>

#include "metar_structs.h"

float fracPart(const char *instring);

#define CHECK(expr) \
    do { \
        if (!(expr)) { \
            fprintf(stderr, "FAILED: %s (line %d)\n", \
                    #expr, __LINE__); \
            return 0; \
        } \
    } while (0)

static int test_fracpart(void)
{
    CHECK(fabs(fracPart("5") - 5.0f) < 1e-6f);
    CHECK(fabs(fracPart("0") - 0.0f) < 1e-6f);

    CHECK(fabs(fracPart("1/2") - 0.5f) < 1e-6f);
    CHECK(fabs(fracPart("3/4") - 0.75f) < 1e-6f);
    CHECK(fabs(fracPart("1/4") - 0.25f) < 1e-6f);

    CHECK(fabs(fracPart("0/5") - 0.0f) < 1e-6f);

    CHECK(fracPart("1/0") == (float)MAXINT);

    CHECK(fracPart(NULL) == (float)MAXINT);

    CHECK(fabs(fracPart("abc/2") - 0.0f) < 1e-6f);
    CHECK(fracPart("2/abc") == (float)MAXINT);

    {
        const char *long_fraction =
            "1234567890123456789012345678901234567890/2";

        CHECK(fracPart(long_fraction) == (float)MAXINT);
    }

    return 1;
}

int main(void)
{
    if (!test_fracpart()) {
        fprintf(stderr, "fracPart test failed\n");
        return 1;
    }

    return 0;
}
