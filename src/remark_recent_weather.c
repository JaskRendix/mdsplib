#include <stdlib.h>
#include <string.h>

#include "remark_recent_weather.h"

MDSP_BOOL isRecentWX(char **token, Decoded_METAR *Mptr, int *NDEX) {
  static char *phenom[] = {
      "FCB",    "-DZB",    "DZB",    "+DZB",    "FZDZB",  "-RAB",    "RAB",
      "+RAB",   "SHRAB",   "TSRAB",  "FZRAB",   "-SNB",   "SNB",     "+SNB",
      "DRSNB",  "BLSNB",   "SHSNB",  "TSSNB",   "-SGB",   "SGB",     "+SGB",
      "ICB",    "-PEB",    "PEB",    "+PEB",    "SHPEB",  "TSPEB",   "GRB",
      "SHGRB",  "TSGRB",   "GSB",    "SHGSB",   "TSGSB",  "-GSB",    "+GSB",
      "TSB",    "VCTSB",   "-TSRAB", "TSRAB",   "+TSRAB", "-TSSNB",  "TSSNB",
      "+TSSNB", "-TSPEB",  "TSPEB",  "+TSPEB",  "-TSGSB", "TSGSB",   "+TSGSB",
      "VCSHB",  "-SHRAB",  "+SHRAB", "-SHSNB",  "+SHSNB", "-SHPEB",  "+SHPEB",
      "-SHGSB", "+SHGSB",  "-FZDZB", "+FZDZB",  "-FZRAB", "+FZRAB",  "FZFGB",
      "+FZFGB", "BRB",     "FGB",    "VCFGB",   "MIFGB",  "PRFGB",   "BCFGB",
      "FUB",    "VAB",     "DUB",    "DRDUB",   "BLDUB",  "SAB",     "DRSAB",
      "BLSAB",  "HZB",     "BLPYB",  "BLSNB",   "+BLSNB", "VCBLSNB", "BLSAB",
      "+BLSAB", "VCBLSAB", "+BLDUB", "VCBLDUB", "POB",    "VCPOB",   "SQB",
      "FCB",    "+FCB",    "VCFCB",  "SSB",     "+SSB",   "VCSSB",   "DSB",
      "+DSB",   "VCDSB",

      "FCE",    "-DZE",    "DZE",    "+DZE",    "FZDZE",  "-RAE",    "RAE",
      "+RAE",   "SHRAE",   "TSRAE",  "FZRAE",   "-SNE",   "SNE",     "+SNE",
      "DRSNE",  "BLSNE",   "SHSNE",  "TSSNE",   "-SGE",   "SGE",     "+SGE",
      "ICE",    "-PEE",    "PEE",    "+PEE",    "SHPEE",  "TSPEE",   "GRE",
      "SHGRE",  "TSGRE",   "GSE",    "SHGSE",   "TSGSE",  "-GSE",    "+GSE",
      "TSE",    "VCTSE",   "-TSRAE", "TSRAE",   "+TSRAE", "-TSSNE",  "TSSNE",
      "+TSSNE", "-TSPEE",  "TSPEE",  "+TSPEE",  "-TSGSE", "TSGSE",   "+TSGSE",
      "VCSHE",  "-SHRAE",  "+SHRAE", "-SHSNE",  "+SHSNE", "-SHPEE",  "+SHPEE",
      "-SHGSE", "+SHGSE",  "-FZDZE", "+FZDZE",  "-FZRAE", "+FZRAE",  "FZFGE",
      "+FZFGE", "BRE",     "FGE",    "VCFGE",   "MIFGE",  "PRFGE",   "BCFGE",
      "FUE",    "VAE",     "DUE",    "DRDUE",   "BLDUE",  "SAE",     "DRSAE",
      "BLSAE",  "HZE",     "BLPYE",  "BLSNE",   "+BLSNE", "VCBLSNE", "BLSAE",
      "+BLSAE", "VCBLSAE", "+BLDUE", "VCBLDUE", "POE",    "VCPOE",   "SQE",
      "FCE",    "+FCE",    "VCFCE",  "SSE",     "+SSE",   "VCSSE",   "DSE",
      "+DSE",   "VCDSE",   "4-Zs"};

  int i, beg_hour, beg_min, end_hour, end_min;

  char *temp, *free_temp, *numb_char, *C_char;

  if (*token == NULL)
    return FALSE;

  if ((free_temp = temp =
           (char *)malloc(sizeof(char) * (strlen(*token) + 1))) == NULL) {
    return FALSE;
  } else
    strcpy(temp, *token);

  while (*temp != '\0') {
    /*
    printf("isRecentWX:  JUST inside while-loop, *NDEX = %d\n",*NDEX);
    printf("isRecentWX:  JUST inside while-loop, temp = %s\n",temp);
    */
    i = 0;

    beg_hour = beg_min = end_hour = end_min = MAXINT;

    while (strncmp(temp, phenom[i], strlen(phenom[i])) != 0 &&
           strcmp(phenom[i], "4-Zs") != 0)
      i++;

    if (strcmp(phenom[i], "4-Zs") != 0) {

      C_char = (strlen(phenom[i]) - 1) + temp;
      numb_char = C_char + 1;

      if (*numb_char == '\0')
        return FALSE;

      if (nisdigit(numb_char, 4) && strlen(numb_char) >= 4) {
        if (*C_char == 'B') {
          beg_hour = antoi(numb_char, 2);
          beg_min = antoi(numb_char + 2, 2);
          temp = numb_char + 4;

          if (*NDEX < 3) {
            Mptr->ReWx[*NDEX].Bmm = beg_min;
            Mptr->ReWx[*NDEX].Bhh = beg_hour;
          }

          temp = numb_char + 4;

          if (*(numb_char + 4) == 'E') {
            numb_char += 5;
            if (nisdigit(numb_char, 4) && strlen(numb_char) >= 4) {
              end_hour = antoi(numb_char, 2);
              end_min = antoi(numb_char + 2, 2);
              temp = numb_char + 4;

              if (*NDEX < 3) {
                Mptr->ReWx[*NDEX].Emm = end_min;
                Mptr->ReWx[*NDEX].Ehh = end_hour;
              }

              temp = numb_char + 4;

            } else if (nisdigit(numb_char, 2) && strlen(numb_char) >= 2) {
              end_min = antoi(numb_char, 2);

              if (*NDEX < 3) {
                Mptr->ReWx[*NDEX].Emm = end_min;
              }
              temp = numb_char + 2;
            }
          }

          if (*NDEX < 3) {
            strncpy(Mptr->ReWx[*NDEX].Recent_weather, phenom[i],
                    (strlen(phenom[i]) - 1));
            (*NDEX)++;
          }
          if (*temp == '\0') {
            free(free_temp);
            return TRUE;
          }

        } else {
          end_hour = antoi(numb_char, 2);
          end_min = antoi(numb_char + 2, 2);

          temp = numb_char + 4;

          if (*NDEX < 3) {
            Mptr->ReWx[*NDEX].Emm = end_min;
            Mptr->ReWx[*NDEX].Ehh = end_hour;
          }

          temp = numb_char + 4;

          if (*(numb_char + 4) == 'B') {
            numb_char += 5;
            if (nisdigit(numb_char, 4) && strlen(numb_char) >= 4) {

              beg_hour = antoi(numb_char, 2);
              beg_min = antoi(numb_char + 2, 2);
              temp = numb_char + 4;

              if (*NDEX < 3) {
                Mptr->ReWx[*NDEX].Bmm = beg_min;
                Mptr->ReWx[*NDEX].Bhh = beg_hour;
              }

              temp = numb_char + 4;
            } else if (nisdigit(numb_char, 2) && strlen(numb_char) >= 2) {
              beg_min = antoi(numb_char, 2);

              if (*NDEX < 3) {
                Mptr->ReWx[*NDEX].Bmm = beg_min;
              }

              temp = numb_char + 2;
            }
          }

          if (*NDEX < 3) {
            strncpy(Mptr->ReWx[*NDEX].Recent_weather, phenom[i],
                    (strlen(phenom[i]) - 1));
            (*NDEX)++;
          }

          if (*temp == '\0') {
            free(free_temp);
            return TRUE;
          }
        }

      } else if (nisdigit(numb_char, 2) && strlen(numb_char) >= 2) {
        if (*C_char == 'B') {
          beg_min = antoi(numb_char, 2);

          if (*NDEX < 3) {
            Mptr->ReWx[*NDEX].Bmm = beg_min;
          }

          temp = numb_char + 2;

          if (*(numb_char + 2) == 'E') {
            numb_char += 3;
            if (nisdigit(numb_char, 4) && strlen(numb_char) >= 4) {
              end_hour = antoi(numb_char, 2);
              end_min = antoi(numb_char + 2, 2);

              if (*NDEX < 3) {
                Mptr->ReWx[*NDEX].Emm = end_min;
                Mptr->ReWx[*NDEX].Ehh = end_hour;
              }

              temp = numb_char + 4;
            } else if (nisdigit(numb_char, 2) && strlen(numb_char) >= 2) {
              end_min = antoi(numb_char, 2);

              if (*NDEX < 3)
                Mptr->ReWx[*NDEX].Emm = end_min;

              temp = numb_char + 2;
            }
          }
          if (*NDEX < 3) {
            strncpy(Mptr->ReWx[*NDEX].Recent_weather, phenom[i],
                    (strlen(phenom[i]) - 1));

            (*NDEX)++;
          }
          if (*temp == '\0') {
            free(free_temp);
            return TRUE;
          }
        } else {
          end_min = antoi(numb_char, 2);

          if (*NDEX < 3)
            Mptr->ReWx[*NDEX].Emm = end_min;

          temp = numb_char + 2;

          if (*(numb_char + 2) == 'B') {
            numb_char += 3;
            if (nisdigit(numb_char, 4) && strlen(numb_char) >= 4) {
              beg_hour = antoi(numb_char, 2);
              beg_min = antoi(numb_char + 2, 2);

              if (*NDEX < 3) {
                Mptr->ReWx[*NDEX].Bmm = beg_min;
                Mptr->ReWx[*NDEX].Bhh = beg_hour;
              }

              temp = numb_char + 4;
            } else if (nisdigit(numb_char, 2) && strlen(numb_char) >= 2) {
              beg_min = antoi(numb_char, 2);

              if (*NDEX < 3)
                Mptr->ReWx[*NDEX].Bmm = beg_min;

              temp = numb_char + 2;
            }
          }
          if (*NDEX < 3) {
            strncpy(Mptr->ReWx[*NDEX].Recent_weather, phenom[i],
                    (strlen(phenom[i]) - 1));
            (*NDEX)++;
          }
          if (*temp == '\0') {
            free(free_temp);
            return TRUE;
          }
        }

      } else {
        free(free_temp);

        if (*NDEX > 0 && *NDEX < 3)
          return TRUE;
        else
          return FALSE;
      }

    } else {
      free(free_temp);
      return FALSE;
    }
  }
  // should not reach here
  return FALSE;
}
