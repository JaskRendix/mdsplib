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

static int test_stspack2(void)
{
    /* NULL handling */
    if (nisalnum(NULL, 1) != 0) return 0;
    if (nisalpha(NULL, 1) != 0) return 0;
    if (nisdigit(NULL, 1) != 0) return 0;

    /* alnum */
    if (!nisalnum("Ab12", 4)) return 0;
    if (nisalnum("Ab$2", 4)) return 0;

    /* alpha */
    if (!nisalpha("AbCd", 4)) return 0;
    if (nisalpha("Ab1d", 4)) return 0;

    /* digit */
    if (!nisdigit("1234", 4)) return 0;
    if (nisdigit("12A4", 4)) return 0;

    /* lower */
    if (!nislower("abcd", 4)) return 0;
    if (nislower("abCd", 4)) return 0;

    /* upper */
    if (!nisupper("ABCD", 4)) return 0;
    if (nisupper("ABCd", 4)) return 0;

    /* hex digit */
    if (!nisxdigi("ABCDEF012345", 12)) return 0;
    if (nisxdigi("ABCXYZ", 6)) return 0;

    /* punctuation */
    if (!nispunct("!?.", 3)) return 0;
    if (nispunct("!A.", 3)) return 0;

    /* space */
    if (!nisspace(" \t\n", 3)) return 0;
    if (nisspace(" A ", 3)) return 0;

    /* printable */
    if (!nisprint("ABC123", 6)) return 0;

    /* graph */
    if (!nisgraph("ABC123", 6)) return 0;
    if (nisgraph("ABC 123", 7)) return 0;

    /* control */
    {
        char ctrl[] = "\n\r\t";
        if (!niscntrl(ctrl, 3))
            return 0;
    }

    /* verify n parameter */
    if (!nisdigit("123A", 3))
        return 0;

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
