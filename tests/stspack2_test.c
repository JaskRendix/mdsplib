#include <stdio.h>

int nisalnum(char *, int);
int nisalpha(char *, int);
int niscntrl(char *, int);
int nisdigit(char *, int);
int nisgraph(char *, int);
int nislower(char *, int);
int nisprint(char *, int);
int nispunct(char *, int);
int nisspace(char *, int);
int nisupper(char *, int);
int nisxdigi(char *, int);

#define CHECK(expr) \
    do { \
        if (!(expr)) { \
            fprintf(stderr, "FAILED: %s (line %d)\n", \
                    #expr, __LINE__); \
            return 0; \
        } \
    } while (0)

static int test_stspack2(void)
{
    CHECK(nisalnum(NULL, 1) == 0);
    CHECK(nisalpha(NULL, 1) == 0);
    CHECK(nisdigit(NULL, 1) == 0);

    CHECK(nisalnum("Ab12", 4));
    CHECK(!nisalnum("Ab$2", 4));

    CHECK(nisalpha("AbCd", 4));
    CHECK(!nisalpha("Ab1d", 4));

    CHECK(nisdigit("1234", 4));
    CHECK(!nisdigit("12A4", 4));

    CHECK(nislower("abcd", 4));
    CHECK(!nislower("abCd", 4));

    CHECK(nisupper("ABCD", 4));
    CHECK(!nisupper("ABCd", 4));

    CHECK(nisxdigi("ABCDEF012345", 12));
    CHECK(!nisxdigi("ABCXYZ", 6));

    CHECK(nispunct("!?.", 3));
    CHECK(!nispunct("!A.", 3));

    CHECK(nisspace(" \t\n", 3));
    CHECK(!nisspace(" A ", 3));

    CHECK(nisprint("ABC123", 6));

    CHECK(nisgraph("ABC123", 6));
    CHECK(!nisgraph("ABC 123", 7));

    {
        char ctrl[] = "\n\r\t";
        CHECK(niscntrl(ctrl, 3));
    }

    CHECK(nisdigit("123A", 3));

    return 1;
}

int main(void)
{
    if (!test_stspack2()) {
        fprintf(stderr, "stspack2 test failed\n");
        return 1;
    }

    return 0;
}
