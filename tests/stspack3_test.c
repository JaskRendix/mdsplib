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

static int test_stspack3(void)
{
    char s1[] = "***ABC";
    char s2[] = "123ABC";
    char s3[] = "\t\nA";
    char s4[] = "abcDEF";
    char s5[] = "!!!abc";
    char s6[] = "   abc";
    char s7[] = "XYZabc";
    char s8[] = "++A9";

    /* NULL handling */
    if (nxtalpha(NULL) != NULL)
        return 0;

    if (nxtdigit(NULL) != NULL)
        return 0;

    /* nxtalnum */
    if (strcmp(nxtalnum(s1), "ABC") != 0)
        return 0;

    /* nxtalpha */
    if (strcmp(nxtalpha(s1), "ABC") != 0)
        return 0;

    /* nxtdigit */
    if (strcmp(nxtdigit(s2), "123ABC") != 0)
        return 0;

    /* nxtcntrl */
    if (nxtcntrl(s3) != s3)
        return 0;

    /* nxtlower */
    if (strcmp(nxtlower(s4), "abcDEF") != 0)
        return 0;

    /* nxtupper */
    if (strcmp(nxtupper(s4), "DEF") != 0)
        return 0;

    /* nxtpunct */
    if (strcmp(nxtpunct(s5), "!!!abc") != 0)
        return 0;

    /* nxtspace */
    if (nxtspace(s6) != s6)
        return 0;

    /* nxtgraph */
    if (strcmp(nxtgraph(s6), "abc") != 0)
        return 0;

    /* nxtprint */
    if (strcmp(nxtprint(s6), "abc") != 0)
        return 0;

    /* nxtxdigi */
    if (strcmp(nxtxdigi(s8), "A9") != 0)
        return 0;

    /* not found */
    {
        char no_digits[] = "ABCDE";

        if (nxtdigit(no_digits) != NULL)
            return 0;
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
