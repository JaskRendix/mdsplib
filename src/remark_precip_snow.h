#ifndef REMARK_PRECIP_SNOW_H
#define REMARK_PRECIP_SNOW_H

#include "metar_structs.h"

MDSP_BOOL isGR(char **string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isVIRGA(char **string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isSNINCR(char **string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isSnowDepth(char *token, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isWaterEquivSnow(char *string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isSunshineDuration(char *string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isHourlyPrecipitation(char **string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isP6Precipitation(char *string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isP24Precip(char *string, Decoded_METAR *Mptr, int *NDEX);

#endif
