#include <limits.h>
#include <stdio.h>

int antoi(const char *string, int len);

#define CHECK(expr) \
    do { \
        if (!(expr)) { \
            fprintf(stderr, "FAILED: %s (line %d)\n", \
                    #expr, __LINE__); \
            return 0; \
        } \
    } while (0)

static int test_antoi(void)
{
    CHECK(antoi("12345", 5) == 12345);
    CHECK(antoi("12345abc", 5) == 12345);
    CHECK(antoi("-42xyz", 3) == -42);
    CHECK(antoi("123", 0) == 0);
    CHECK(antoi(NULL, 5) == INT_MIN);
    CHECK(antoi("123", -1) == INT_MIN);

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
