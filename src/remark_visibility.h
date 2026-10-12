#ifndef REMARK_VISIBILITY_H
#define REMARK_VISIBILITY_H

#include "metar_structs.h"

MDSP_BOOL isDVR(char *token, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isTowerVisibility(char **token, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isSurfaceVisibility(char **token, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isVariableVisibility(char **string, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isVisibility2ndSite(char **token, Decoded_METAR *Mptr, int *NDEX);
MDSP_BOOL isSectorVsby(char **string, Decoded_METAR *Mptr, int *NDEX);

#endif
