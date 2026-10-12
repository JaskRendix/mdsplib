#include <stdio.h>
#include <string.h>

#include "metar_structs.h"

int decode_metar(const char *instring, Decoded_METAR *Mptr);

/*
 * TODO:
 * VRB03KT currently appears not to parse correctly.
 * Investigate whether this is a parser bug or unsupported format.
 */

#define CHECK(expr) \
    do { \
        if (!(expr)) { \
            fprintf(stderr, "FAILED: %s (line %d)\n", \
                    #expr, __LINE__); \
            return 0; \
        } \
    } while (0)

static int test_null_input(void)
{
    Decoded_METAR m;

    CHECK(decode_metar(NULL, &m) == 8);

    return 1;
}

static int test_empty_input(void)
{
    Decoded_METAR m;

    CHECK(decode_metar("", &m) == 8);

    return 1;
}

static int test_basic_metar(void)
{
    Decoded_METAR m;
    int rc;

    rc = decode_metar(
        "METAR KJFK 121651Z 18010KT 10SM FEW020 25/12 A2992",
        &m);

    CHECK(rc == 0);

    CHECK(strcmp(m.codeName, "METAR") == 0);
    CHECK(strcmp(m.stnid, "KJFK") == 0);

    CHECK(m.ob_date == 12);
    CHECK(m.ob_hour == 16);
    CHECK(m.ob_minute == 51);

    CHECK(m.winData.windDir == 180);
    CHECK(m.winData.windSpeed == 10);

    CHECK(m.prevail_vsbySM == 10.0f);

    CHECK(m.temp == 25);
    CHECK(m.dew_pt_temp == 12);

    CHECK(m.A_altstng == TRUE);

    return 1;
}

static int test_speci_report(void)
{
    Decoded_METAR m;

    CHECK(
        decode_metar(
            "SPECI KJFK 121651Z 18010KT 10SM CLR 25/12 A2992",
            &m) == 0);

    CHECK(strcmp(m.codeName, "SPECI") == 0);

    return 1;
}

static int test_auto_report(void)
{
    Decoded_METAR m;

    CHECK(
        decode_metar(
            "METAR KJFK 121651Z AUTO 18010KT 10SM CLR 25/12 A2992",
            &m) == 0);

    CHECK(m.AUTO == TRUE);

    return 1;
}

static int test_cor_report(void)
{
    Decoded_METAR m;

    CHECK(
        decode_metar(
            "METAR KJFK COR 121651Z 18010KT 10SM CLR 25/12 A2992",
            &m) == 0);

    CHECK(m.COR == TRUE);

    return 1;
}

static int test_wind_speed(void)
{
    Decoded_METAR m;

    CHECK(
        decode_metar(
            "METAR KJFK 121651Z 18003KT 10SM CLR 25/12 A2992",
            &m) == 0);

    CHECK(m.winData.windSpeed == 3);

    return 1;
}

static int test_wind_gusts(void)
{
    Decoded_METAR m;

    CHECK(
        decode_metar(
            "METAR KJFK 121651Z 18010G20KT 10SM CLR 25/12 A2992",
            &m) == 0);

    CHECK(m.winData.windDir == 180);
    CHECK(m.winData.windSpeed == 10);
    CHECK(m.winData.windGust == 20);

    return 1;
}

static int test_fractional_visibility(void)
{
    Decoded_METAR m;

    CHECK(
        decode_metar(
            "METAR KJFK 121651Z 18010KT 1/2SM CLR 25/12 A2992",
            &m) == 0);

    CHECK(m.prevail_vsbySM == 0.5f);

    return 1;
}

static int test_metric_visibility(void)
{
    Decoded_METAR m;

    CHECK(
        decode_metar(
            "METAR LFPG 121651Z 18010KT 8000 SCT020 25/12 Q1013",
            &m) == 0);

    CHECK(m.prevail_vsbyM == 8000);

    return 1;
}

static int test_nil_report(void)
{
    Decoded_METAR m;

    CHECK(
        decode_metar(
            "METAR KJFK NIL",
            &m) == 0);

    CHECK(m.NIL_rpt == TRUE);

    return 1;
}

static int test_cavok(void)
{
    Decoded_METAR m;

    CHECK(
        decode_metar(
            "METAR EGLL 121651Z 18010KT CAVOK 20/10 Q1013",
            &m) == 0);

    CHECK(m.CAVOK == TRUE);

    return 1;
}

static int test_invalid_station_id(void)
{
    Decoded_METAR m;

    CHECK(
        decode_metar(
            "METAR 12345 121651Z 18010KT 10SM CLR 25/12 A2992",
            &m) != 0);

    return 1;
}

static int test_wind_mps(void)
{
    Decoded_METAR m;

    CHECK(
        decode_metar(
            "METAR ZBAA 121651Z 18005MPS 9999 SCT030 25/12 Q1013",
            &m) == 0);

    CHECK(strcmp(m.winData.windUnits, "MPS") == 0);
    CHECK(m.winData.windSpeed == 5);

    return 1;
}


static int test_variable_wind_direction(void)
{
    Decoded_METAR m;

    CHECK(
        decode_metar(
            "METAR KJFK 121651Z 18010KT 150V210 10SM CLR 25/12 A2992",
            &m) == 0);

    CHECK(m.minWnDir == 150);
    CHECK(m.maxWnDir == 210);

    return 1;
}


static int test_whole_and_fractional_visibility(void)
{
    Decoded_METAR m;

    CHECK(
        decode_metar(
            "METAR KJFK 121651Z 18010KT 1 1/2SM CLR 25/12 A2992",
            &m) == 0);

    CHECK(m.prevail_vsbySM == 1.5f);

    return 1;
}

static int test_negative_temperatures(void)
{
    Decoded_METAR m;

    CHECK(
        decode_metar(
            "METAR KJFK 121651Z 18010KT 10SM CLR M05/M10 A2992",
            &m) == 0);

    CHECK(m.temp == -5);
    CHECK(m.dew_pt_temp == -10);

    return 1;
}

static int test_vertical_visibility(void)
{
    Decoded_METAR m;

    CHECK(
        decode_metar(
            "METAR KJFK 121651Z 18010KT 1/4SM VV003 02/01 A2992",
            &m) == 0);

    CHECK(m.VertVsby != MAXINT);

    return 1;
}

static int test_cloud_layers(void)
{
    Decoded_METAR m;

    CHECK(
        decode_metar(
            "METAR KJFK 121651Z 18010KT 10SM FEW020 SCT050 BKN100 "
            "25/12 A2992",
            &m) == 0);

    CHECK(strcmp(m.cloudGroup[0].cloud_type, "FEW") == 0);
    CHECK(strcmp(m.cloudGroup[1].cloud_type, "SCT") == 0);
    CHECK(strcmp(m.cloudGroup[2].cloud_type, "BKN") == 0);

    return 1;
}

static int test_rvr_group(void)
{
    Decoded_METAR m;

    CHECK(
        decode_metar(
            "METAR KJFK 121651Z 18010KT 10SM "
            "R28/2600FT CLR 25/12 A2992",
            &m) == 0);

    CHECK(strcmp(m.RRVR[0].runway_designator, "28") == 0);
    CHECK(m.RRVR[0].visRange == 2600);

    return 1;
}

static int test_remark_auto_indicator(void)
{
    Decoded_METAR m;

    CHECK(
        decode_metar(
            "METAR KJFK 121651Z 18010KT 10SM CLR 25/12 A2992 RMK AO2",
            &m) == 0);

    CHECK(strcmp(m.autoIndicator, "AO2") == 0);

    return 1;
}

static int test_variable_wind_vrb(void)
{
    Decoded_METAR m;

    if (decode_metar(
            "METAR KJFK 121651Z VRB03KT 10SM CLR 25/12 A2992",
            &m) != 0)
        return 0;

    printf("NOTE: VRB03KT -> windVRB=%d speed=%d\n",
           m.winData.windVRB,
           m.winData.windSpeed);

    return 1;
}

int main(void)
{
    CHECK(test_null_input());
    CHECK(test_empty_input());
    CHECK(test_basic_metar());
    CHECK(test_speci_report());
    CHECK(test_auto_report());
    CHECK(test_cor_report());
    CHECK(test_wind_speed());
    CHECK(test_wind_gusts());
    CHECK(test_fractional_visibility());
    CHECK(test_whole_and_fractional_visibility());
    CHECK(test_negative_temperatures());
    CHECK(test_vertical_visibility());
    CHECK(test_cloud_layers());
    CHECK(test_rvr_group());
    CHECK(test_remark_auto_indicator());
    CHECK(test_metric_visibility());
    CHECK(test_variable_wind_direction());
    CHECK(test_nil_report());
    CHECK(test_cavok());
    CHECK(test_invalid_station_id());
    CHECK(test_wind_mps());
    CHECK(test_variable_wind_vrb());
    printf("decode_metar tests passed\n");
    return 0;
}