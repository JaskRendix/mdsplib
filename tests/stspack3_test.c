#include <stdio.h>
#include <string.h>

char *nxtalnum(char *);
char *nxtalpha(char *);
char *nxtcntrl(char *);
char *nxtdigit(char *);
char *nxtgraph(char *);
char *nxtlower(char *);
char *nxtprint(char *);
char *nxtpunct(char *);
char *nxtspace(char *);
char *nxtupper(char *);
char *nxtxdigi(char *);

#define CHECK(expr) \
    do { \
        if (!(expr)) { \
            fprintf(stderr, "FAILED: %s (line %d)\n", \
                    #expr, __LINE__); \
            return 0; \
        } \
    } while (0)

static int test_stspack3(void)
{
    char s1[] = "***ABC";
    char s2[] = "123ABC";
    char s3[] = "\t\nA";
    char s4[] = "abcDEF";
    char s5[] = "!!!abc";
    char s6[] = "   abc";
    char s8[] = "++A9";

    CHECK(nxtalpha(NULL) == NULL);
    CHECK(nxtdigit(NULL) == NULL);

    CHECK(strcmp(nxtalnum(s1), "ABC") == 0);
    CHECK(strcmp(nxtalpha(s1), "ABC") == 0);

    CHECK(strcmp(nxtdigit(s2), "123ABC") == 0);

    CHECK(nxtcntrl(s3) == s3);

    CHECK(strcmp(nxtlower(s4), "abcDEF") == 0);
    CHECK(strcmp(nxtupper(s4), "DEF") == 0);

    CHECK(strcmp(nxtpunct(s5), "!!!abc") == 0);

    CHECK(nxtspace(s6) == s6);

    CHECK(strcmp(nxtgraph(s6), "abc") == 0);

    /*
     * isprint() returns true for spaces,
     * so nxtprint("   abc") should return s6,
     * not "abc".
     */
    CHECK(nxtprint(s6) == s6);

    CHECK(strcmp(nxtxdigi(s8), "A9") == 0);

    {
        char no_digits[] = "ABCDE";
        CHECK(nxtdigit(no_digits) == NULL);
    }

    return 1;
}

int main(void)
{
    if (!test_stspack3()) {
        fprintf(stderr, "stspack3 test failed\n");
        return 1;
    }

    return 0;
}
