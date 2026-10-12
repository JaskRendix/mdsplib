#ifndef REMARK_WIND_STORM_H
#define REMARK_WIND_STORM_H

#include "metar_structs.h"

MDSP_BOOL isRADAT(char **string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isTS_LOC(char **string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isTornadicActivity(char **string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isA0indicator(char *indicator, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isPeakWind(char **string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isWindShift(char **string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isLightningFrequency(char **string, Decoded_METAR *Mptr, int *NDEX);

#endif
