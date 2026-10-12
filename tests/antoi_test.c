#include <limits.h>
#include <stdio.h>

int antoi(const char *string, int len);

static int test_antoi(void)
{
    if (antoi("12345", 5) != 12345)
        return 0;

    if (antoi("12345abc", 5) != 12345)
        return 0;

    if (antoi("-42xyz", 3) != -42)
        return 0;

    if (antoi("123", 0) != 0)
        return 0;

    if (antoi(NULL, 5) != INT_MIN)
        return 0;

    if (antoi("123", -1) != INT_MIN)
        return 0;

    return 1;
}

int main(void)
{
    if (!test_antoi()) {
        fprintf(stderr, "antoi test failed\n");
        return 1;
    }

    return 0;
}
