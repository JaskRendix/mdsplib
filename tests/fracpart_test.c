#include <stdio.h>
#include <math.h>

#include "metar_structs.h"

float fracPart(const char *instring);

static int test_fracpart(void)
{
    /* integer values */
    if (fabs(fracPart("5") - 5.0f) > 1e-6f)
        return 0;

    if (fabs(fracPart("0") - 0.0f) > 1e-6f)
        return 0;

    /* normal fractions */
    if (fabs(fracPart("1/2") - 0.5f) > 1e-6f)
        return 0;

    if (fabs(fracPart("3/4") - 0.75f) > 1e-6f)
        return 0;

    if (fabs(fracPart("1/4") - 0.25f) > 1e-6f)
        return 0;

    /* zero numerator */
    if (fabs(fracPart("0/5") - 0.0f) > 1e-6f)
        return 0;

    /* division by zero */
    if (fracPart("1/0") != (float)MAXINT)
        return 0;

    /* NULL input */
    if (fracPart(NULL) != (float)MAXINT)
        return 0;

    /* malformed fractions use atoi semantics */
    if (fabs(fracPart("abc/2") - 0.0f) > 1e-6f)
        return 0;

    if (fabs(fracPart("2/abc") - 0.0f) > 1e-6f)
        return 0;

    /* buffer overflow protection */
    {
        const char *long_fraction =
            "1234567890123456789012345678901234567890/2";

        if (fracPart(long_fraction) != (float)MAXINT)
            return 0;
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
