#ifndef REMARK_QUALITY_FLAGS_H
#define REMARK_QUALITY_FLAGS_H

#include "metar_structs.h"

MDSP_BOOL isNOSPECI(char *string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isLAST(char *string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isPWINO(char *string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isPNO(char *string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isRVRNO(char *string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isCHINO(char **string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isVISNO(char **string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isFZRANO(char *string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isTSNO(char *string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isDollarSign(char *indicator, Decoded_METAR *Mptr, int *NDEX);

#endif
