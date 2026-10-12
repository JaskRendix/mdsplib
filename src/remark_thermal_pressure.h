#ifndef REMARK_THERMAL_PRESSURE_H
#define REMARK_THERMAL_PRESSURE_H

#include "metar_structs.h"

MDSP_BOOL isPRESFR(char *string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isPRESRR(char *string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isSLP(char **token, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isTTdTenths(char *token, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isMaxTemperature(char *string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isMinTemperature(char *string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isT24MaxMinTemp(char *string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isPtendency(char *string, Decoded_METAR *Mptr, int *NDEX);

#endif
