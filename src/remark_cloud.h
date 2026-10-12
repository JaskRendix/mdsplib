#ifndef REMARK_CLOUD_H
#define REMARK_CLOUD_H

#include "metar_structs.h"

MDSP_BOOL isPartialObscuration(char **string, Decoded_METAR *Mptr, int ndex, int *NDEX);
MDSP_BOOL isVariableCIG(char **token, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isCIG2ndSite(char **token, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isSurfaceObscuration(char *string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isCeiling(char *string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isVariableSky(char **string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isObscurationAloft(char **string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isSynopticClouds(char *token, Decoded_METAR *Mptr, int *NDEX);

#endif
