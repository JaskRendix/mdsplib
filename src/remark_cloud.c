#include <stdlib.h>
#include <string.h>

#include "remark_cloud.h"

#define SKY1_len 50

MDSP_BOOL isPartialObscuration(char **string, Decoded_METAR *Mptr, int ndex,
                               int *NDEX) {
  int i;

  static char *phenom[] = {
      "-DZ",    "DZ",    "+DZ",   "FZDZ",   "-RA",   "RA",     "+RA",   "SHRA",
      "TSRA",   "FZRA",  "-SN",   "SN",     "+SN",   "DRSN",   "SHSN",  "TSSN",
      "-SG",    "SG",    "+SG",   "IC",     "-PE",   "PE",     "+PE",   "SHPE",
      "TSPE",   "GR",    "SHGR",  "TSGR",   "GS",    "SHGS",   "TSGS",  "-GS",
      "+GS",    "TS",    "VCTS",  "-TSRA",  "TSRA",  "+TSRA",  "-TSSN", "TSSN",
      "+TSSN",  "-TSPE", "TSPE",  "+TSPE",  "-TSGS", "TSGS",   "+TSGS", "VCSH",
      "-SHRA",  "+SHRA", "-SHSN", "+SHSN",  "-SHPE", "+SHPE",  "-SHGS", "+SHGS",
      "-FZDZ",  "+FZDZ", "-FZRA", "+FZRA",  "FZFG",  "+FZFG",  "BR",    "FG",
      "VCFG",   "MIFG",  "PRFG",  "BCFG",   "FU",    "VA",     "DU",    "DRDU",
      "BLDU",   "SA",    "DRSA",  "BLSA",   "HZ",    "BLPY",   "BLSN",  "+BLSN",
      "VCBLSN", "BLSA",  "+BLSA", "VCBLSA", "+BLDU", "VCBLDU", "PO",    "VCPO",
      "SQ",     "FC",    "+FC",   "VCFC",   "SS",    "+SS",    "VCSS",  "DS",
      "+DS",    "VCDS",  NULL};

#ifdef DEBUGXX
  printf("isPartialObscuration:  Routine Entered...\n");
  printf("isPartialObscuration:  *string = %s\n", *string);
  if (Mptr->PartialObscurationAmt[ndex][0] != '\0') {
    printf("PartialObscurationAmt = %s\n", Mptr->PartialObscurationAmt[ndex]);
    if (strcmp(*string, "FEW///") == 0 || strcmp(*string, "SCT///") == 0 ||
        strcmp(*string, "BKN///") == 0 || strcmp(*string, "FEW000") == 0 ||
        strcmp(*string, "SCT000") == 0 || strcmp(*string, "BKN000") == 0) {

      --string;
      printf("isPartialObscuration:  Preceding group = %s\n", *string);
      ++string;
    }
  }
#endif

  if (*string == NULL)
    return FALSE;

  if (strcmp(*string, "FEW///") == 0 || strcmp(*string, "SCT///") == 0 ||
      strcmp(*string, "BKN///") == 0 || strcmp(*string, "FEW000") == 0 ||
      strcmp(*string, "SCT000") == 0 || strcmp(*string, "BKN000") == 0) {
    if (Mptr->PartialObscurationAmt[ndex][0] == '\0') {
      (*NDEX)++;
      return FALSE;
    } else {
      if (strcmp(*string, Mptr->PartialObscurationAmt[ndex]) == 0) {
        --string;

        if (*string == NULL)
          return FALSE;

        i = 0;
        while (phenom[i] != NULL) {
          if (strcmp(*string, phenom[i]) == 0) {
            strcpy(Mptr->PartialObscurationPhenom[ndex], *string);

            (*NDEX)++;
            return TRUE;
          } else
            i++;
        }

        (*NDEX)++;
        return FALSE;

      } else {
        (*NDEX)++;
        return FALSE;
      }
    }

  } else
    return FALSE;
}

MDSP_BOOL isVariableCIG(char **token, Decoded_METAR *Mptr, int *NDEX) {
  char *V_char;

  if (*token == NULL)
    return FALSE;

  if (strcmp(*token, "CIG") != 0)
    return FALSE;
  else
    (++token);

  if (*token == NULL)
    return FALSE;

  if ((V_char = strchr(*token, 'V')) != NULL) {
    if (nisdigit(*token, (V_char - *token)) &&
        nisdigit(V_char + 1, strlen(V_char + 1))) {
      Mptr->minCeiling = antoi(*token, (V_char - *token));
      Mptr->maxCeiling = atoi(V_char + 1);

      (*NDEX)++;
      (*NDEX)++;
      return TRUE;
    } else
      return FALSE;
  } else
    return FALSE;
}

MDSP_BOOL isCIG2ndSite(char **token, Decoded_METAR *Mptr, int *NDEX) {
  int CIG2ndSite;

  if ((*token) == NULL)
    return FALSE;

  if (strcmp(*token, "CIG") != 0)
    return FALSE;
  else
    (++token);

  if ((*token) == NULL)
    return FALSE;

  if (strlen(*token) != 3)
    return FALSE;

  if (nisdigit(*token, 3)) {
    CIG2ndSite = atoi(*token) * 10;

    ++token;

    if (*token == NULL)
      return FALSE;

    if (strncmp(*token, "RY", 2) != 0)
      return FALSE;
    else {
      strcpy(Mptr->CIG_2ndSite_LOC, *token);
      Mptr->CIG_2ndSite_Meters = CIG2ndSite;
      (*NDEX)++;
      (*NDEX)++;
      (*NDEX)++;
      return TRUE;
    }
  } else
    return FALSE;
}

MDSP_BOOL isSurfaceObscuration(char *string, Decoded_METAR *Mptr, int *NDEX) {
  static char *WxSymbols[] = {
      "BCFG", "BLDU",   "BLSA",   "BLPY",   "DRDU", "DRSA", "DRSN", "DZ",
      "DS",   "FZFG",   "FZDZ",   "FZRA",   "FG",   "FC",   "FU",   "GS",
      "GR",   "HZ",     "IC",     "MIFG",   "PE",   "PO",   "RA",   "SHRA",
      "SHSN", "SHPE",   "SHGS",   "SHGR",   "SN",   "SG",   "SQ",   "SA",
      "SS",   "TSRA",   "TSSN",   "TSPE",   "TSGS", "TSGR", "TS",   "VCSH",
      "VCPO", "VCBLDU", "VCBLSA", "VCBLSN", "VCFG", "VCFC", "VA",   NULL};
  int i, ndex;
  char *numLoc, ww[12], *temp;

  MDSP_BOOL IS_NOT_FOUND;

  if (string == NULL)
    return FALSE;

  memset(ww, '\0', sizeof(ww));

  if (strlen(string) < 4)
    return FALSE;

  if (strncmp(string, "-X", 2) != 0)
    return FALSE;

  if (!(nisdigit(string + (strlen(string) - 1), 1)))
    return FALSE;
  else {
    temp = string + 2;
    strncpy(ww, temp, (strlen(string) - 2));

    ndex = 0;
    temp = ww;
    numLoc = temp + (strlen(temp) - 1);

    while (temp < numLoc && ndex < MAX_SURFACE_OBSCURATIONS) {
      i = 0;

      IS_NOT_FOUND = TRUE;

      while (WxSymbols[i] != NULL && IS_NOT_FOUND) {
        if (strncmp(WxSymbols[i], temp, strlen(WxSymbols[i])) != 0)
          i++;
        else
          IS_NOT_FOUND = FALSE;
      }

      if (WxSymbols[i] == NULL) {
        (*NDEX)++;
        return FALSE;
      } else {
        strcpy(Mptr->SfcObscuration[ndex], WxSymbols[i]);
        temp += strlen(WxSymbols[i]);
        ndex++;
      }
    }

    if (ndex > 0) {
      Mptr->Num8thsSkyObscured = antoi(numLoc, 1);
      (*NDEX)++;
      return TRUE;
    } else {
      (*NDEX)++;
      return FALSE;
    }
  }
}

MDSP_BOOL isCeiling(char *string, Decoded_METAR *Mptr, int *NDEX) {

  if (string == NULL)
    return FALSE;

  if (!(strncmp(string, "CIG", 3) == 0 && strlen(string) >= 5))
    return FALSE;
  else {
    if (strcmp(string, "CIGNO") == 0) {
      Mptr->CIGNO = TRUE;
      (*NDEX)++;
      return TRUE;
    } else if (strlen(string + 3) == 3) {
      if (nisdigit(string + 3, strlen(string + 3)) && strlen(string + 3) == 3) {
        Mptr->Ceiling = atoi(string + 3) * 100;
        (*NDEX)++;
        return TRUE;
      } else
        return FALSE;
    } else if (strlen(string + 3) == 4) {
      if (*(string + 3) == 'E' && nisdigit(string + 4, 3)) {
        Mptr->Estimated_Ceiling = antoi(string + 4, 3) * 100;
        (*NDEX)++;
        return TRUE;
      } else
        return FALSE;
    } else
      return FALSE;
  }
}

MDSP_BOOL isVariableSky(char **string, Decoded_METAR *Mptr, int *NDEX) {
  static char *cldPtr[] = {"FEW", "SCT", "BKN", "OVC", NULL};
  MDSP_BOOL IS_NOT_FOUND;
  int i;
  char SKY1[SKY1_len];

  if (*string == NULL)
    return FALSE;

  memset(SKY1, '\0', SKY1_len);
  i = 0;
  IS_NOT_FOUND = TRUE;

  while (cldPtr[i] != NULL && IS_NOT_FOUND) {
#ifdef DEBUGQQ
    printf("isVariableSky: *string = %s cldPtr[%d] = %s\n", *string, i,
           cldPtr[i]);
#endif
    if (strncmp(*string, cldPtr[i], strlen(cldPtr[i])) != 0)
      i++;
    else
      IS_NOT_FOUND = FALSE;
  }

  if (cldPtr[i] == NULL)
    return FALSE;
  else {
#ifdef DEBUGQQ
    printf("isVariableSky: *string = %s = cldPtr[%d] = %s\n", *string, i,
           cldPtr[i]);
#endif
    strcpy(SKY1, cldPtr[i]);

    (++string);

    if (*string == NULL)
      return FALSE;

    if (strcmp(*string, "V") != 0)
      return FALSE;
    else {
      (++string);

      if (*string == NULL)
        return FALSE;

      i = 0;
      IS_NOT_FOUND = TRUE;
      while (cldPtr[i] != NULL && IS_NOT_FOUND) {
#ifdef DEBUGQQ
        printf("isVariableSky: *string = %s cldPtr[%d] = %s\n", *string, i,
               cldPtr[i]);
#endif
        if (strncmp(*string, cldPtr[i], strlen(cldPtr[i])) != 0)
          i++;
        else
          IS_NOT_FOUND = FALSE;
      }

      if (cldPtr[i] == NULL) {
        (*NDEX)++;
        (*NDEX)++;
        return FALSE;
      } else {
        if (strlen(SKY1) == 6) {
          if (nisdigit(SKY1 + 3, 3)) {
            strncpy(Mptr->VrbSkyBelow, SKY1, 3);
            strcpy(Mptr->VrbSkyAbove, cldPtr[i]);
            Mptr->VrbSkyLayerHgt = antoi(SKY1 + 3, 3) * 100;
            (*NDEX)++;
            (*NDEX)++;
            (*NDEX)++;
            return TRUE;
          } else {
            (*NDEX)++;
            (*NDEX)++;
            (*NDEX)++;
            return TRUE;
          }
        } else {
          strcpy(Mptr->VrbSkyBelow, SKY1);
          strcpy(Mptr->VrbSkyAbove, cldPtr[i]);
          (*NDEX)++;
          (*NDEX)++;
          (*NDEX)++;
          return TRUE;
        }
      }
    }
  }
}

MDSP_BOOL isObscurationAloft(char **string, Decoded_METAR *Mptr, int *NDEX) {
  static char *WxSymbols[] = {
      "BCFG", "BLDU",   "BLSA",   "BLPY",   "DRDU", "DRSA", "DRSN", "DZ",
      "DS",   "FZFG",   "FZDZ",   "FZRA",   "FG",   "FC",   "FU",   "GS",
      "GR",   "HZ",     "IC",     "MIFG",   "PE",   "PO",   "RA",   "SHRA",
      "SHSN", "SHPE",   "SHGS",   "SHGR",   "SN",   "SG",   "SQ",   "SA",
      "SS",   "TSRA",   "TSSN",   "TSPE",   "TSGS", "TSGR", "TS",   "VCSH",
      "VCPO", "VCBLDU", "VCBLSA", "VCBLSN", "VCFG", "VCFC", "VA",   NULL};
  int i;
  char *saveTemp, *temp;

  if (*string == NULL) {
    return FALSE;
  }

  saveTemp = temp = *string;

  if (*temp == '\0') {
    return FALSE;
  }

  while (*temp != '\0') {
    i = 0;

    // try to match temp to one of the WxSymbols
    for (i = 0; WxSymbols[i] != NULL; i++) {
      if (strncmp(temp, WxSymbols[i], strlen(WxSymbols[i])) == 0) {
        break;
      }
    }

    // if we didn't match, bail
    if (WxSymbols[i] == NULL) {
      return FALSE;
    }

    temp += strlen(WxSymbols[i]);
  }

  ++string;

  if (*string == NULL) {
    return FALSE;
  }

  if (strlen(*string) != 6) {
    return FALSE;
  }

  if ((strncmp(*string, "FEW", 3) == 0 || strncmp(*string, "SCT", 3) == 0 ||
       strncmp(*string, "BKN", 3) == 0 || strncmp(*string, "OVC", 3) == 0) &&
      (nisdigit(*string + 3, 3) && strcmp(*string + 3, "000") != 0)) {
    strcpy(Mptr->ObscurAloft, saveTemp);
    strncpy(Mptr->ObscurAloftSkyCond, *string, 3);
    Mptr->ObscurAloftHgt = atoi(*string + 3) * 100;
    (*NDEX)++;
    // (*NDEX)++;
    // (*NDEX)++;
    return TRUE;
  }

  (*NDEX)++;
  return TRUE;
}

MDSP_BOOL isSynopticClouds(char *token, Decoded_METAR *Mptr, int *NDEX) {

  if (token == NULL)
    return FALSE;

  if (strlen(token) != 5)
    return FALSE;

  if (*token == '8' && *(token + 1) == '/' &&
      ((*(token + 2) <= '9' && *(token + 2) >= '0') || *(token + 2) == '/') &&
      ((*(token + 3) <= '9' && *(token + 3) >= '0') || *(token + 3) == '/') &&
      ((*(token + 4) <= '9' && *(token + 4) >= '0') || *(token + 4) == '/')) {
    strcpy(Mptr->synoptic_cloud_type, token);

    Mptr->CloudLow = *(token + 2);
    Mptr->CloudMedium = *(token + 3);
    Mptr->CloudHigh = *(token + 4);

    (*NDEX)++;
    return TRUE;
  } else
    return FALSE;
}
