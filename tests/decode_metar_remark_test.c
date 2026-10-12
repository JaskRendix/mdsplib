#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "metar_structs.h"

int decode_metar(const char *instring, Decoded_METAR *Mptr);
void decode_metar_remark(char **token, Decoded_METAR *Mptr);

#define CHECK(expr) \
    do { \
        if (!(expr)) { \
            fprintf(stderr, "FAILED: %s (line %d)\n", \
                    #expr, __LINE__); \
            return 0; \
        } \
    } while (0)

static char **make_tokens(const char *text)
{
    static char *tokens[64];
    static char buffer[512];
    char *tok;
    int i = 0;

    strncpy(buffer, text, sizeof(buffer) - 1);
    buffer[sizeof(buffer) - 1] = '\0';

    tok = strtok(buffer, " ");

    while (tok != NULL && i < 63) {
        tokens[i++] = tok;
        tok = strtok(NULL, " ");
    }

    tokens[i] = NULL;

    return tokens;
}

static int test_no_rmk(void)
{
    Decoded_METAR m;

    decode_metar(
        "METAR KJFK 121651Z 18010KT 10SM CLR 25/12 A2992",
        &m);

    decode_metar_remark(
        make_tokens("METAR KJFK 121651Z"),
        &m);

    return 1;
}

static int test_ao2(void)
{
    Decoded_METAR m;

    decode_metar(
        "METAR KJFK 121651Z 18010KT 10SM CLR 25/12 A2992",
        &m);

    decode_metar_remark(
        make_tokens("METAR KJFK RMK AO2"),
        &m);

    CHECK(strcmp(m.autoIndicator, "AO2") == 0);

    return 1;
}

static int test_presfr(void)
{
    Decoded_METAR m;

    decode_metar(
        "METAR KJFK 121651Z 18010KT 10SM CLR 25/12 A2992",
        &m);

    decode_metar_remark(
        make_tokens("METAR KJFK RMK PRESFR"),
        &m);

    CHECK(m.PRESFR == TRUE);

    return 1;
}

static int test_presrr(void)
{
    Decoded_METAR m;

    decode_metar(
        "METAR KJFK 121651Z 18010KT 10SM CLR 25/12 A2992",
        &m);

    decode_metar_remark(
        make_tokens("METAR KJFK RMK PRESRR"),
        &m);

    CHECK(m.PRESRR == TRUE);

    return 1;
}

static int test_pwino(void)
{
    Decoded_METAR m;

    decode_metar(
        "METAR KJFK 121651Z 18010KT 10SM CLR 25/12 A2992",
        &m);

    decode_metar_remark(
        make_tokens("METAR KJFK RMK PWINO"),
        &m);

    CHECK(m.PWINO == TRUE);

    return 1;
}

static int test_fzrano(void)
{
    Decoded_METAR m;

    decode_metar(
        "METAR KJFK 121651Z 18010KT 10SM CLR 25/12 A2992",
        &m);

    decode_metar_remark(
        make_tokens("METAR KJFK RMK FZRANO"),
        &m);

    CHECK(m.FZRANO == TRUE);

    return 1;
}

static int test_tsno(void)
{
    Decoded_METAR m;

    decode_metar(
        "METAR KJFK 121651Z 18010KT 10SM CLR 25/12 A2992",
        &m);

    decode_metar_remark(
        make_tokens("METAR KJFK RMK TSNO"),
        &m);

    CHECK(m.TSNO == TRUE);

    return 1;
}

static int test_pno(void)
{
    Decoded_METAR m;

    decode_metar(
        "METAR KJFK 121651Z 18010KT 10SM CLR 25/12 A2992",
        &m);

    decode_metar_remark(
        make_tokens("METAR KJFK RMK PNO"),
        &m);

    CHECK(m.PNO == TRUE);

    return 1;
}

static int test_rvrno(void)
{
    Decoded_METAR m;

    decode_metar(
        "METAR KJFK 121651Z 18010KT 10SM CLR 25/12 A2992",
        &m);

    decode_metar_remark(
        make_tokens("METAR KJFK RMK RVRNO"),
        &m);

    CHECK(m.RVRNO == TRUE);

    return 1;
}

static int test_dollar_sign(void)
{
    Decoded_METAR m;

    decode_metar(
        "METAR KJFK 121651Z 18010KT 10SM CLR 25/12 A2992",
        &m);

    decode_metar_remark(
        make_tokens("METAR KJFK RMK $"),
        &m);

    CHECK(m.DollarSign == TRUE);

    return 1;
}

static int test_duplicate_flag_reset(void)
{
    Decoded_METAR m;

    decode_metar(
        "METAR KJFK 121651Z 18010KT 10SM CLR 25/12 A2992",
        &m);

    decode_metar_remark(
        make_tokens("METAR KJFK RMK PWINO PWINO"),
        &m);

    CHECK(m.PWINO == FALSE);

    return 1;
}

int main(void)
{
    printf("test_no_rmk\n");
    fflush(stdout);
    CHECK(test_no_rmk());

    printf("test_ao2\n");
    fflush(stdout);
    CHECK(test_ao2());

    printf("test_presfr\n");
    fflush(stdout);
    CHECK(test_presfr());

    printf("test_presrr\n");
    fflush(stdout);
    CHECK(test_presrr());

    printf("test_pwino\n");
    fflush(stdout);
    CHECK(test_pwino());

    printf("test_fzrano\n");
    fflush(stdout);
    CHECK(test_fzrano());

    printf("test_tsno\n");
    fflush(stdout);
    CHECK(test_tsno());

    printf("test_pno\n");
    fflush(stdout);
    CHECK(test_pno());

    printf("test_rvrno\n");
    fflush(stdout);
    CHECK(test_rvrno());

    printf("test_dollar_sign\n");
    fflush(stdout);
    CHECK(test_dollar_sign());

    printf("test_duplicate_flag_reset\n");
    fflush(stdout);
    CHECK(test_duplicate_flag_reset());

    printf("decode_metar_remark tests passed\n");

    return 0;
}
