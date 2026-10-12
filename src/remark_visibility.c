#include <stdlib.h>
#include <string.h>

#include "remark_visibility.h"

float fracPart(const char *);

MDSP_BOOL isDVR(char *token, Decoded_METAR *Mptr, int *NDEX) {
  char *slashPtr, *FT_ptr;
  char *vPtr;
  int length;

  if (token == NULL)
    return FALSE;

  if ((length = strlen(token)) < 4)
    return FALSE;

  if (strncmp(token, "DVR", 3) != 0)
    return FALSE;

  if (*(slashPtr = token + 3) != '/') {
    (*NDEX)++;
    return FALSE;
  }

  if (strcmp(token + (strlen(token) - 2), "FT") != 0)
    return FALSE;
  else
    FT_ptr = token + (strlen(token) - 2);

  if (strchr(slashPtr + 1, 'P') != NULL)
    Mptr->DVR.above_max_DVR = TRUE;

  if (strchr(slashPtr + 1, 'M') != NULL)
    Mptr->DVR.below_min_DVR = TRUE;

  if ((vPtr = strchr(slashPtr, 'V')) != NULL) {
    Mptr->DVR.vrbl_visRange = TRUE;
    Mptr->DVR.Min_visRange = antoi(slashPtr + 1, (vPtr - (slashPtr + 1)));
    Mptr->DVR.Max_visRange = antoi(vPtr + 1, (FT_ptr - (vPtr + 1)));
    (*NDEX)++;
    return TRUE;
  } else {
    if (Mptr->DVR.below_min_DVR || Mptr->DVR.above_max_DVR)
      Mptr->DVR.visRange = antoi(slashPtr + 2, (FT_ptr - (slashPtr + 2)));
    else
      Mptr->DVR.visRange = antoi(slashPtr + 1, (FT_ptr - (slashPtr + 1)));

    (*NDEX)++;
    return TRUE;
  }
}

MDSP_BOOL isTowerVisibility(char **token, Decoded_METAR *Mptr, int *NDEX) {
  char *slash;
  float T_vsby;

  /* IF THE CURRENT AND NEXT GROUPS ARE "TWR VIS", THEN DETERMINE */
  /* WHETHER OR NOT THE GROUP(S) THAT FOLLOWS IS(ARE) A VALID     */
  /* TOWER VISIBILITY  GROUP.  IF IT IS, THEN DECODE THE GROUP    */
  /* GROUP AND RETURN TRUE.  OTHERWISE, RETURN FALSE.             */

  if (*token == NULL)
    return FALSE;

  if (strcmp(*token, "TWR") != 0)
    return FALSE;
  else
    (++token);

  if (*token == NULL)
    return FALSE;

  if (strcmp(*token, "VIS") != 0) {
    (*NDEX)++;
    return FALSE;
  } else
    (++token);

  if (*token == NULL)
    return FALSE;

  if (nisdigit(*token, strlen(*token))) {
    Mptr->TWR_VSBY = (float)atoi(*token);
    (++token);
    if (*token != NULL) {
      if ((slash = strchr(*token, '/')) != NULL) {
        if (nisdigit(slash + 1, strlen(slash + 1)) &&
            nisdigit(*token, (slash - *token))) {
          T_vsby = fracPart(*token);
          Mptr->TWR_VSBY += T_vsby;
          (*NDEX)++;
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
        (*NDEX)++;
        (*NDEX)++;
        (*NDEX)++;
        return TRUE;
      }
    } else {
      (*NDEX)++;
      (*NDEX)++;
      (*NDEX)++;
      return TRUE;
    }

  } else if ((slash = strchr(*token, '/')) != NULL) {
    if (nisdigit(slash + 1, strlen(slash + 1)) &&
        nisdigit(*token, (slash - *token))) {
      Mptr->TWR_VSBY = fracPart(*token);
      (*NDEX)++;
      (*NDEX)++;
      (*NDEX)++;
      return TRUE;
    } else {
      (*NDEX)++;
      (*NDEX)++;
      return FALSE;
    }
  } else {
    (*NDEX)++;
    (*NDEX)++;
    return FALSE;
  }
}

MDSP_BOOL isSurfaceVisibility(char **token, Decoded_METAR *Mptr, int *NDEX) {
  char *slash;
  float S_vsby;

  /* IF THE CURRENT AND NEXT GROUPS ARE "SFC VIS", THEN DETERMINE */
  /* WHETHER OR NOT THE GROUP(S) THAT FOLLOWS IS(ARE) A VALID     */
  /* SURFACE VISIBILITY  GROUP.  IF IT IS, THEN DECODE THE GROUP  */
  /* GROUP AND RETURN TRUE.  OTHERWISE, RETURN FALSE.             */

  if (*token == NULL)
    return FALSE;

  if (strcmp(*token, "SFC") != 0)
    return FALSE;
  else
    (++token);

  if (*token == NULL)
    return FALSE;

  if (strcmp(*token, "VIS") != 0) {
    (*NDEX)++;
    return FALSE;
  } else
    (++token);

  if (*token == NULL)
    return FALSE;

  if (nisdigit(*token, strlen(*token))) {
    Mptr->SFC_VSBY = (float)atoi(*token);
    (++token);
    if (*token != NULL) {
      if ((slash = strchr(*token, '/')) != NULL) {
        if (nisdigit(slash + 1, strlen(slash + 1)) &&
            nisdigit(*token, (slash - *token))) {
          S_vsby = fracPart(*token);
          Mptr->SFC_VSBY += S_vsby;
          (*NDEX)++;
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
        (*NDEX)++;
        (*NDEX)++;
        (*NDEX)++;
        return TRUE;
      }
    } else {
      (*NDEX)++;
      (*NDEX)++;
      (*NDEX)++;
      return TRUE;
    }

  } else if ((slash = strchr(*token, '/')) != NULL) {
    if (nisdigit(slash + 1, strlen(slash + 1)) &&
        nisdigit(*token, (slash - *token))) {
      Mptr->SFC_VSBY = fracPart(*token);
      (*NDEX)++;
      (*NDEX)++;
      (*NDEX)++;
      return TRUE;
    } else {
      (*NDEX)++;
      (*NDEX)++;
      return FALSE;
    }
  } else {
    (*NDEX)++;
    (*NDEX)++;
    return FALSE;
  }
}

MDSP_BOOL isVariableVisibility(char **string, Decoded_METAR *Mptr, int *NDEX) {
  char *slash, *slash1, *slash2, buf[5], *V_char;
  float minimumVsby, maximumVsby;

  slash = slash1 = slash2 = V_char = NULL;

  /* IF THE CURRENT GROUP IS  "VIS", THEN DETERMINE  */
  /* WHETHER OR NOT THE GROUPS THAT FOLLOW ARE VALID */
  /* VARIABLE PREVAILING VSBY.  IF THEY ARE, THEN    */
  /* DECODE THE GROUPS AND RETURN TRUE.  OTHERWISE,  */
  /* RETURN FALSE.                                   */

  if (*string == NULL)
    return FALSE;

  if (strcmp(*string, "VIS") != 0)
    return FALSE;
  else
    (++string);

  if (*string == NULL)
    return FALSE;

  if (!((V_char = strchr(*string, 'V')) != NULL ||
        nisdigit(*string, strlen(*string))))
    return FALSE;
  else if (nisdigit(*string, strlen(*string))) {
    minimumVsby = (float)atoi(*string);
    (++string);

    if (*string == NULL)
      return FALSE;

    if ((V_char = strchr(*string, 'V')) == NULL)
      return FALSE;
    else {
      if ((slash = strchr(*string, '/')) == NULL)
        return FALSE;
      else {
        if (nisdigit(*string, (slash - *string)) &&
            nisdigit(slash + 1, (V_char - (slash + 1))) &&
            nisdigit(V_char + 1, strlen(V_char + 1))) {
          if ((V_char - *string) > 4)
            return FALSE;
          else {
            memset(buf, '\0', 5);
            strncpy(buf, *string, (V_char - *string));
            Mptr->minVsby = minimumVsby + fracPart(buf);
            maximumVsby = (float)atoi(V_char + 1);
          }

          (++string);

          if (*string == NULL)
            return FALSE;

          if ((slash = strchr(*string, '/')) == NULL) {
            Mptr->maxVsby = maximumVsby;
            (*NDEX)++;
            (*NDEX)++;
            (*NDEX)++;
            return TRUE;
          } else if (nisdigit(*string, (slash - *string)) &&
                     nisdigit(slash + 1, strlen(slash + 1))) {
            Mptr->maxVsby = maximumVsby + fracPart(*string);
            (*NDEX)++;
            (*NDEX)++;
            (*NDEX)++;
            (*NDEX)++;
            return TRUE;
          } else {
            Mptr->maxVsby = maximumVsby;
            (*NDEX)++;
            (*NDEX)++;
            (*NDEX)++;
            return TRUE;
          }
        } else
          return FALSE;
      }
    }
  } else {
    if ((V_char = strchr(*string, 'V')) == NULL)
      return FALSE;
    if (nisdigit(*string, (V_char - *string)) &&
        nisdigit(V_char + 1, strlen(V_char + 1))) {
      Mptr->minVsby = (float)antoi(*string, (V_char - *string));
      maximumVsby = (float)atoi(V_char + 1);

      (++string);

      if (*string == NULL)
        return FALSE;

      if ((slash = strchr(*string, '/')) == NULL) {
        Mptr->maxVsby = maximumVsby;
        (*NDEX)++;
        (*NDEX)++;
        return TRUE;
      } else if (nisdigit(*string, (slash - *string)) &&
                 nisdigit(slash + 1, strlen(slash + 1))) {
        Mptr->maxVsby = maximumVsby + fracPart(*string);
        (*NDEX)++;
        (*NDEX)++;
        (*NDEX)++;
        return TRUE;
      } else {
        Mptr->maxVsby = maximumVsby;
        (*NDEX)++;
        (*NDEX)++;
        return TRUE;
      }
    } else {
      if ((slash2 = strchr(V_char + 1, '/')) == NULL &&
          (slash1 = strchr(*string, '/')) == NULL)
        return FALSE;
      else if (slash1 == NULL)
        return FALSE;
      else if (slash == slash2)
        return FALSE;
      else if (nisdigit(*string, (slash1 - *string)) &&
               nisdigit((slash1 + 1), (V_char - (slash1 + 1)))) {
        if ((V_char - *string) > 4)
          return FALSE;
        else {
          memset(buf, '\0', 5);
          strncpy(buf, *string, (V_char - *string));
          minimumVsby = fracPart(buf);
        }
        if (slash2 == NULL) {
          if (nisdigit(V_char + 1, strlen(V_char + 1))) {
            maximumVsby = (float)atoi(V_char + 1);

            (++string);

            if (*string == NULL)
              return FALSE;

            if ((slash = strchr(*string, '/')) == NULL) {
              Mptr->minVsby = minimumVsby;
              Mptr->maxVsby = maximumVsby;
              (*NDEX)++;
              (*NDEX)++;
              return TRUE;
            } else if (nisdigit(*string, (slash - *string)) &&
                       nisdigit((slash + 1), strlen(slash + 1))) {
              Mptr->minVsby = minimumVsby;
              Mptr->maxVsby = maximumVsby + fracPart(*string);
              (*NDEX)++;
              (*NDEX)++;
              (*NDEX)++;
              return TRUE;
            } else {
              Mptr->minVsby = minimumVsby;
              Mptr->maxVsby = maximumVsby;
              (*NDEX)++;
              (*NDEX)++;
              return TRUE;
            }
          } else
            return FALSE;
        } else {
          if (nisdigit(V_char + 1, (slash2 - V_char + 1)) &&
              nisdigit((slash2 + 1), strlen(slash2 + 1))) {
            Mptr->minVsby = minimumVsby;
            Mptr->maxVsby = fracPart(V_char + 1);
            (*NDEX)++;
            (*NDEX)++;
            return TRUE;
          } else
            return FALSE;
        }
      }
    }
  }
  return FALSE;
}

MDSP_BOOL isVisibility2ndSite(char **token, Decoded_METAR *Mptr, int *NDEX) {
  char *slash;
  float S_vsby, VSBY_2ndSite;

  /* IF THE CURRENT GROUP IS  "VIS", THEN DETERMINE  */
  /* WHETHER OR NOT THE GROUPS THAT FOLLOW ARE VALID */
  /* VSBILITY AT A 2NDRY SITE.  IF THEY ARE, THEN    */
  /* DECODE THE GROUPS AND RETURN TRUE.  OTHERWISE,  */
  /* RETURN FALSE.                                   */

  if (*token == NULL)
    return FALSE;

  if (strcmp(*token, "VIS") != 0)
    return FALSE;
  else
    (++token);

  if (*token == NULL)
    return FALSE;

  if (nisdigit(*token, strlen(*token))) {
    VSBY_2ndSite = (float)atoi(*token);
    (++token);
    if (*token != NULL) {
      if ((slash = strchr(*token, '/')) != NULL) {
        if (nisdigit(slash + 1, strlen(slash + 1)) &&
            nisdigit(*token, (slash - *token))) {
          S_vsby = fracPart(*token);

          (++token);

          if (*token == NULL)
            return FALSE;

          if (strncmp(*token, "RMY", 3) == 0) {
            if (nisalnum(*token, strlen(*token))) {
              strcpy(Mptr->VSBY_2ndSite_LOC, *token);
              Mptr->VSBY_2ndSite = VSBY_2ndSite + S_vsby;
              (*NDEX)++;
              (*NDEX)++;
              (*NDEX)++;
              (*NDEX)++;
              return TRUE;
            } else
              return FALSE;
          } else
            return FALSE;
        } else {
          if (strncmp(*token, "RMY", 3) == 0) {
            if (nisalnum(*token, strlen(*token))) {
              strcpy(Mptr->VSBY_2ndSite_LOC, *token);
              Mptr->VSBY_2ndSite = VSBY_2ndSite;
              (*NDEX)++;
              (*NDEX)++;
              (*NDEX)++;
              return TRUE;
            } else
              return FALSE;
          } else
            return FALSE;
        }

      } else {
        if (strncmp(*token, "RMY", 3) == 0) {
          if (nisalnum(*token, strlen(*token))) {
            strcpy(Mptr->VSBY_2ndSite_LOC, *token);
            Mptr->VSBY_2ndSite = VSBY_2ndSite;
            (*NDEX)++;
            (*NDEX)++;
            (*NDEX)++;
            return TRUE;
          } else
            return TRUE;
        } else
          return FALSE;
      }
    } else {
      if (*token != NULL && strncmp(*token, "RMY", 3) == 0) {
        if (nisalnum(*token, strlen(*token))) {
          strcpy(Mptr->VSBY_2ndSite_LOC, *token);
          Mptr->VSBY_2ndSite = VSBY_2ndSite;
          (*NDEX)++;
          (*NDEX)++;
          (*NDEX)++;
          return TRUE;
        } else
          return FALSE;
      } else
        return FALSE;
    }

  } else if ((slash = strchr(*token, '/')) != NULL) {
    if (nisdigit(slash + 1, strlen(slash + 1)) &&
        nisdigit(*token, (slash - *token))) {
      VSBY_2ndSite = fracPart(*token);

      (++token);

      if (*token == NULL)
        return FALSE;

      if (strncmp(*token, "RMY", 3) == 0) {
        if (nisalnum(*token, strlen(*token))) {
          Mptr->VSBY_2ndSite = VSBY_2ndSite;
          strcpy(Mptr->VSBY_2ndSite_LOC, *token);
          (*NDEX)++;
          (*NDEX)++;
          (*NDEX)++;
          return TRUE;
        } else
          return FALSE;
      } else
        return FALSE;
    } else
      return FALSE;
  } else
    return FALSE;
}

MDSP_BOOL isSectorVsby(char **string, Decoded_METAR *Mptr, int *NDEX) {
  int result, tempstrlen = 20;

  float vsby;
  vsby = FLT_MAX;
  char dd[3], temp[20], *slash;

  if (*string == NULL)
    return FALSE;

  memset(dd, '\0', sizeof(dd));

  if (strcmp(*string, "VIS") != 0)
    return FALSE;
  else {
    ++string;

    if (*string == NULL)
      return FALSE;

    if (strncmp(*string, "NE", 2) == 0)
      strncpy(dd, *string, 2);
    else if (strncmp(*string, "SE", 2) == 0)
      strncpy(dd, *string, 2);
    else if (strncmp(*string, "NW", 2) == 0)
      strncpy(dd, *string, 2);
    else if (strncmp(*string, "SW", 2) == 0)
      strncpy(dd, *string, 2);
    else if (strncmp(*string, "N", 1) == 0)
      strncpy(dd, *string, 1);
    else if (strncmp(*string, "E", 1) == 0)
      strncpy(dd, *string, 1);
    else if (strncmp(*string, "S", 1) == 0)
      strncpy(dd, *string, 1);
    else if (strncmp(*string, "W", 1) == 0)
      strncpy(dd, *string, 1);
    else
      return FALSE;

    (++string);
    if (*string == NULL)
      return FALSE;
    /*
    printf("DCDMTRMK result = %d\n",
                     strspn(*string,"0123456789/M"));
    */
    if ((result = strspn(*string, "0123456789/M")) == 0)
      return FALSE;
    else if (nisdigit(*string, result))
      vsby = antoi(*string, result);
    else if (result >= tempstrlen - 1)
      return FALSE;
    else {
      memset(temp, '\0', tempstrlen);
      strncpy(temp, *string, result);
      /*
      printf("DCDMTRMK temp = %s\n",temp);
      */
      if (strcmp(temp, "M1/4") == 0) {
        strcpy(Mptr->SectorVsby_Dir, dd);
        Mptr->SectorVsby = 0.0;
        (*NDEX)++;
        (*NDEX)++;
        (*NDEX)++;
        return TRUE;
      }
      if (strchr(temp, 'M') != NULL)
        return FALSE;
      if ((slash = strchr(temp, '/')) == NULL)
        return FALSE;
      else if (nisdigit(temp, (slash - temp)) &&
               nisdigit(slash + 1, strlen(slash + 1))) {
        vsby = fracPart(temp);
        if (vsby > 0.875)
          return FALSE;
        else {
          Mptr->SectorVsby = vsby;
          strcpy(Mptr->SectorVsby_Dir, dd);
          (*NDEX)++;
          (*NDEX)++;
          (*NDEX)++;
          return TRUE;
        }
      }
    }

    (++string);
    if (*string == NULL) {
      Mptr->SectorVsby = vsby;
      strcpy(Mptr->SectorVsby_Dir, dd);
      (*NDEX)++;
      (*NDEX)++;
      (*NDEX)++;
      return TRUE;
    } else if ((result = strspn(*string, "0123456789/")) == 0) {
      Mptr->SectorVsby = vsby;
      strcpy(Mptr->SectorVsby_Dir, dd);
      (*NDEX)++;
      (*NDEX)++;
      (*NDEX)++;
      return TRUE;
    } else if ((slash = strchr(*string, '/')) == NULL) {
      Mptr->SectorVsby = vsby;
      strcpy(Mptr->SectorVsby_Dir, dd);
      (*NDEX)++;
      (*NDEX)++;
      (*NDEX)++;
      return TRUE;
    } else {
      if (fracPart(*string) > 0.875) {
        Mptr->SectorVsby = vsby;
        strcpy(Mptr->SectorVsby_Dir, dd);
        (*NDEX)++;
        (*NDEX)++;
        (*NDEX)++;
        return TRUE;
      } else {
        vsby += fracPart(*string);
        Mptr->SectorVsby = vsby;
        strcpy(Mptr->SectorVsby_Dir, dd);
        (*NDEX)++;
        (*NDEX)++;
        (*NDEX)++;
        (*NDEX)++;
        return TRUE;
      }
    }
  }
}
