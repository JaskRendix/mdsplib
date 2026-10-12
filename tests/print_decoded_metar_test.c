#include <stdio.h>
#include <string.h>

#include "metar.h"

#define CHECK(expr) \
    do { \
        if (!(expr)) { \
            fprintf(stderr, "FAILED: %s (line %d)\n", \
                    #expr, __LINE__); \
            return 0; \
        } \
    } while (0)

static void init_metar(Decoded_METAR *metar)
{
    memset(metar, 0, sizeof(*metar));

    metar->ob_date = MAXINT;
    metar->ob_hour = MAXINT;
    metar->ob_minute = MAXINT;

    metar->minCeiling = MAXINT;
    metar->maxCeiling = MAXINT;

    metar->BTornadicHour = MAXINT;
    metar->BTornadicMinute = MAXINT;
    metar->ETornadicHour = MAXINT;
    metar->ETornadicMinute = MAXINT;
    metar->TornadicDistance = MAXINT;

    metar->RRVR[0].visRange = MAXINT;
    metar->RRVR[0].Max_visRange = MAXINT;
    metar->RRVR[0].Min_visRange = MAXINT;

    metar->DVR.visRange = MAXINT;
    metar->DVR.Max_visRange = MAXINT;
    metar->DVR.Min_visRange = MAXINT;
}

static int test_basic_output(void)
{
    Decoded_METAR metar;
    char buffer[5000];

    init_metar(&metar);

    sprint_metar(buffer, &metar);

    CHECK(strstr(buffer,
                 "THE DECODED METAR REPORT FOLLOWS") != NULL);

    return 1;
}

static int test_max_ceiling_label(void)
{
    Decoded_METAR metar;
    char buffer[5000];

    init_metar(&metar);

    metar.maxCeiling = 5000;

    sprint_metar(buffer, &metar);

    CHECK(strstr(buffer,
                 "MAX VRBL_CIG (FT)   : 5000") != NULL);

    CHECK(strstr(buffer,
                 "MAX VRBL_CIG (FT))") == NULL);

    return 1;
}

static int test_runway_visibility_feet(void)
{
    Decoded_METAR metar;
    char buffer[5000];

    init_metar(&metar);

    strcpy(metar.RRVR[0].runway_designator, "09");
    metar.RRVR[0].distance_unit = DIST_FEET;
    metar.RRVR[0].visRange = 2400;

    sprint_metar(buffer, &metar);

    CHECK(strstr(buffer,
                 "R_WAY VIS RANGE (FT): 2400") != NULL);

    return 1;
}

static int test_runway_visibility_meters(void)
{
    Decoded_METAR metar;
    char buffer[5000];

    init_metar(&metar);

    strcpy(metar.RRVR[0].runway_designator, "09");
    metar.RRVR[0].distance_unit = DIST_METERS;
    metar.RRVR[0].visRange = 800;

    sprint_metar(buffer, &metar);

    CHECK(strstr(buffer,
                 "R_WAY VIS RANGE (M): 800") != NULL);

    return 1;
}

static int test_tornadic_output(void)
{
    Decoded_METAR metar;
    char buffer[5000];

    init_metar(&metar);

    strcpy(metar.TornadicType, "TORNADO");
    metar.BTornadicHour = 12;
    metar.BTornadicMinute = 34;

    sprint_metar(buffer, &metar);

    CHECK(strstr(buffer,
                 "TORNADIC ACTVTY TYPE: TORNADO") != NULL);

    CHECK(strstr(buffer,
                 "TORN. ACTVTY BEGHOUR: 12") != NULL);

    CHECK(strstr(buffer,
                 "TORN. ACTVTY BEGMIN : 34") != NULL);

    return 1;
}

int main(void)
{
    CHECK(test_basic_output());
    CHECK(test_max_ceiling_label());
    CHECK(test_runway_visibility_feet());
    CHECK(test_runway_visibility_meters());
    CHECK(test_tornadic_output());

    return 0;
}
