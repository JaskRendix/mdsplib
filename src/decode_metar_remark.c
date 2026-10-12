/*
METAR Decoder Software Package Library: Parses Aviation Routine Weather Reports
Copyright (C) 2003  Eric McCarthy

This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public
License along with this library; if not, write to the Free Software
Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "metar_structs.h"
#include "remark_cloud.h"
#include "remark_precip_snow.h"
#include "remark_quality_flags.h"
#include "remark_recent_weather.h"
#include "remark_thermal_pressure.h"
#include "remark_visibility.h"
#include "remark_wind_storm.h"

#define SKY1_len 50
float fracPart(const char *);

#ifdef DEBUGZZ
#define DEBUG_PRINTF(x) printf x
#else
#define DEBUG_PRINTF(x)
#endif

#include "metar_structs.h" /* standard header file */

/********************************************************************/
/*                                                                  */
/*  Title:         decode_metar_remark                              */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      decode_metar_remark takes a pointer to a METAR   */
/*                 report and parses/decodes data elements from     */
/*                 the remarks section of the report.               */
/*                                                                  */
/*                                                                  */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         token - the address of a pointer to a METAR      */
/*                         report character string.                 */
/*                 Mptr  - a pointer to a structure of the vari-    */
/*                         able type Decoded_METAR.                 */
/*                                                                  */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

void decode_metar_remark(char **token, Decoded_METAR *Mptr) {
  int TornadicActvty = 0, A0indicator = 0, peakwind = 0, windshift = 0,
      towerVsby = 0, surfaceVsby = 0, variableVsby = 0, LTGfreq = 0, TS_LOC = 0,
      recentWX = 0, variableCIG = 0, PRESFR = 0, Vsby2ndSite = 0,
      CIG2ndSite = 0, PRESRR = 0, SLP = 0, PartObscur = 0, SectorVsby = 0,
      GR = 0, Virga = 0, SfcObscur = 0, Ceiling = 0, VrbSkyCond = 0,
      ObscurAloft = 0, NoSPECI = 0, Last = 0, SynopClouds = 0, Snincr = 0,
      SnowDepth = 0, WaterEquivSnow = 0, SunshineDur = 0, hourlyPrecip = 0,
      P6Precip = 0, P24Precip = 0, TTdTenths = 0, MaxTemp = 0, MinTemp = 0,
      T24MaxMinTemp = 0, Ptendency = 0, PWINO = 0, FZRANO = 0, TSNO = 0,
      maintIndicator = 0, CHINO = 0, RVRNO = 0, VISNO = 0, PNO = 0, DVR = 0;

  int NDEX, i;

  MDSP_BOOL IS_NOT_RMKS;

  NDEX = 0;

  // locate the start of the metar remarks section

  IS_NOT_RMKS = TRUE;

  while (token[NDEX] != NULL && IS_NOT_RMKS) {
#ifdef DEBUGZZ
    printf("decode_metar_remark:  token[%d] = %s\n", NDEX, token[NDEX]);
#endif
    if (strcmp(token[NDEX], "RMK") != 0) {
      NDEX++;
    } else {
      IS_NOT_RMKS = FALSE;
    }
  }

  // if the metar report contains no remarks section, return

  if (token[NDEX] != NULL) {
#ifdef DEBUGZZ
    printf("decode_metar_remark:  RMK found, token[%d] = %s\n", NDEX,
           token[NDEX]);
#endif
    NDEX++;
#ifdef DEBUGZZ
    printf("decode_metar_remark:  Bump NDEX, token[%d] = %s\n", NDEX,
           token[NDEX]);
#endif
  } else {
#ifdef DEBUGZZ
    printf("decode_metar_remark:  No RMK found.  NULL ptr encountered\n");
#endif
    return;
  }

  // identify and validate remarks section
  //   data groups for parsing/decoding

  while (token[NDEX] != NULL) {

#ifdef DEBUGZZ
    printf("decode_metar_remark:  decode RMK: token[%d] = %s\n", NDEX,
           token[NDEX]);
#endif

    isRADAT(&(token[NDEX]), Mptr, &NDEX);

    if (isTornadicActivity(&(token[NDEX]), Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: tornadic activity\n"));

      // NB temp
#if 0
			char stupid[100];
			extern void sprint_tornadic_info (char * string, Decoded_METAR *Mptr);
			stupid[0] = '\0';
			sprintf_tornadic_info(stupid,Mptr);
			puts(stupid);
#endif
      // end temp

      TornadicActvty++;
      if (TornadicActvty > 1) {
        memset(Mptr->TornadicType, '\0', sizeof(Mptr->TornadicType));
        memset(Mptr->TornadicLOC, '\0', sizeof(Mptr->TornadicLOC));
        memset(Mptr->TornadicDIR, '\0', sizeof(Mptr->TornadicDIR));
        Mptr->BTornadicHour = MAXINT;
        Mptr->BTornadicMinute = MAXINT;
        Mptr->ETornadicHour = MAXINT;
        Mptr->ETornadicMinute = MAXINT;
      }
    } else if (isA0indicator(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: A0 indicator\n"));
      A0indicator++;
      if (A0indicator > 1) {
        memset(Mptr->autoIndicator, '\0', sizeof(Mptr->autoIndicator));
      }
    } else if (isPeakWind(&(token[NDEX]), Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: peak wind\n"));
      peakwind++;
      if (peakwind > 1) {
        Mptr->PKWND_dir = MAXINT;
        Mptr->PKWND_speed = MAXINT;
        Mptr->PKWND_hour = MAXINT;
        Mptr->PKWND_minute = MAXINT;
      }
    } else if (isWindShift(&(token[NDEX]), Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: wind shift\n"));
      windshift++;
      if (windshift > 1) {
        Mptr->WshfTime_hour = MAXINT;
        Mptr->WshfTime_minute = MAXINT;
      }
    } else if (isTowerVisibility(&(token[NDEX]), Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: tower visibility\n"));
      towerVsby++;
      if (towerVsby > 1) {
        Mptr->TWR_VSBY = (float)MAXINT;
      }
    } else if (isSurfaceVisibility(&(token[NDEX]), Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: surface visibility\n"));
      surfaceVsby++;
      if (surfaceVsby > 1) {
        Mptr->SFC_VSBY = (float)MAXINT;
      }
    } else if (isVariableVisibility(&(token[NDEX]), Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: variable visibility\n"));
      variableVsby++;
      if (variableVsby > 1) {
        Mptr->minVsby = (float)MAXINT;
        Mptr->maxVsby = (float)MAXINT;
      }
    } else if (isVisibility2ndSite(&(token[NDEX]), Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: 2nd site visibility\n"));
      Vsby2ndSite++;
      if (Vsby2ndSite > 1) {
        Mptr->VSBY_2ndSite = (float)MAXINT;
        memset(Mptr->VSBY_2ndSite_LOC, '\0', sizeof(Mptr->VSBY_2ndSite_LOC));
      }
    } else if (isLightningFrequency(&(token[NDEX]), Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: lightning frequency\n"));
      LTGfreq++;
      if (LTGfreq > 1) {
        Mptr->OCNL_LTG = FALSE;
        Mptr->FRQ_LTG = FALSE;
        Mptr->CNS_LTG = FALSE;
        Mptr->CG_LTG = FALSE;
        Mptr->IC_LTG = FALSE;
        Mptr->CC_LTG = FALSE;
        Mptr->CA_LTG = FALSE;
        Mptr->DSNT_LTG = FALSE;
        Mptr->OVHD_LTG = FALSE;
        Mptr->VcyStn_LTG = FALSE;
        Mptr->LightningVCTS = FALSE;
        Mptr->LightningTS = FALSE;
        memset(Mptr->LTG_DIR, '\0', sizeof(Mptr->LTG_DIR));
      }
    } else if (isTS_LOC(&(token[NDEX]), Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: TS_LOC\n"));
      TS_LOC++;
      if (TS_LOC > 1) {
        memset(Mptr->TS_LOC, '\0', sizeof(Mptr->TS_LOC));
        memset(Mptr->TS_MOVMNT, '\0', sizeof(Mptr->TS_MOVMNT));
      }
    } else if (isRecentWX(&(token[NDEX]), Mptr, &recentWX)) {
      DEBUG_PRINTF(("decode_metar_remark: recent WX\n"));
      NDEX++;
    } else if (isVariableCIG(&(token[NDEX]), Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: variable CIG\n"));
      variableCIG++;
      if (variableCIG > 1) {
        Mptr->minCeiling = MAXINT;
        Mptr->maxCeiling = MAXINT;
      }
    } else if (isCIG2ndSite(&(token[NDEX]), Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: 2ndsite CIG\n"));
      CIG2ndSite++;
      if (CIG2ndSite > 1) {
        Mptr->CIG_2ndSite_Meters = MAXINT;
        memset(Mptr->CIG_2ndSite_LOC, '\0', sizeof(Mptr->CIG_2ndSite_LOC));
      }
    } else if (isPRESFR(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: PRESFR\n"));
      PRESFR++;
      if (PRESFR > 1) {
        Mptr->PRESFR = FALSE;
      }
    } else if (isPRESRR(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: PRESRR\n"));
      PRESRR++;
      if (PRESRR > 1) {
        Mptr->PRESRR = FALSE;
      }
    } else if (isSLP(&(token[NDEX]), Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: SLP\n"));
      SLP++;
      if (SLP > 1) {
        Mptr->SLP = (float)MAXINT;
      }
    } else if (isPartialObscuration(&(token[NDEX]), Mptr, PartObscur, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: partial obscuration\n"));
      PartObscur++;
      if (PartObscur > 2) {
        memset(Mptr->PartialObscurationAmt[0], '\0',
               sizeof(Mptr->PartialObscurationAmt[0]));
        memset(Mptr->PartialObscurationPhenom[0], '\0',
               sizeof(Mptr->PartialObscurationPhenom[0]));

        memset(Mptr->PartialObscurationAmt[1], '\0',
               sizeof(Mptr->PartialObscurationAmt[1]));
        memset(Mptr->PartialObscurationPhenom[1], '\0',
               sizeof(Mptr->PartialObscurationPhenom[1]));
      }
    } else if (isSectorVsby(&(token[NDEX]), Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: sector visibility\n"));
      SectorVsby++;
      if (SectorVsby > 1) {
        Mptr->SectorVsby = (float)MAXINT;
        memset(Mptr->SectorVsby_Dir, '\0', sizeof(Mptr->SectorVsby_Dir));
      }
    } else if (isGR(&(token[NDEX]), Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: GR\n"));
      GR++;
      if (GR > 1) {
        Mptr->GR_Size = (float)MAXINT;
        Mptr->GR = FALSE;
      }
    } else if (isVIRGA(&(token[NDEX]), Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: VIRGA\n"));
      Virga++;
      if (Virga > 1) {
        Mptr->VIRGA = FALSE;
        memset(Mptr->VIRGA_DIR, '\0', sizeof(Mptr->VIRGA_DIR));
      }
    } else if (isSurfaceObscuration(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: surface obscuration\n"));
      SfcObscur++;
      if (SfcObscur > 1) {
        for (i = 0; i < MAX_SURFACE_OBSCURATIONS; i++) {
          memset(&(Mptr->SfcObscuration[i][0]), '\0',
                 sizeof(Mptr->SfcObscuration[i]));
          Mptr->Num8thsSkyObscured = MAXINT;
        }
      }
    } else if (isCeiling(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: ceiling\n"));
      Ceiling++;
      if (Ceiling > 1) {
        Mptr->CIGNO = FALSE;
        Mptr->Ceiling = MAXINT;
        Mptr->Estimated_Ceiling = FALSE;
      }
    } else if (isVariableSky(&(token[NDEX]), Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: variable sky\n"));
      VrbSkyCond++;
      if (VrbSkyCond > 1) {
        memset(Mptr->VrbSkyBelow, '\0', sizeof(Mptr->VrbSkyBelow));
        memset(Mptr->VrbSkyAbove, '\0', sizeof(Mptr->VrbSkyAbove));
        Mptr->VrbSkyLayerHgt = MAXINT;
      }
    } else if (isObscurationAloft(&(token[NDEX]), Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: obscuration aloft\n"));
      ObscurAloft++;
      if (ObscurAloft > 1) {
        Mptr->ObscurAloftHgt = MAXINT;
        memset(Mptr->ObscurAloft, '\0', sizeof(Mptr->ObscurAloft));
        memset(Mptr->ObscurAloftSkyCond, '\0',
               sizeof(Mptr->ObscurAloftSkyCond));
      }
    } else if (isNOSPECI(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: NOSPECI\n"));
      NoSPECI++;
      if (NoSPECI > 1) {
        Mptr->NOSPECI = FALSE;
      }
    } else if (isLAST(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: LAST\n"));
      Last++;
      if (Last > 1) {
        Mptr->LAST = FALSE;
      }
    } else if (isSynopticClouds(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: Synoptic Clouds\n"));
      SynopClouds++;
      if (SynopClouds > 1) {
        memset(Mptr->synoptic_cloud_type, '\0',
               sizeof(Mptr->synoptic_cloud_type));
        Mptr->CloudLow = '\0';
        Mptr->CloudMedium = '\0';
        Mptr->CloudHigh = '\0';
      }
    } else if (isSNINCR(&(token[NDEX]), Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: SNINCR\n"));
      Snincr++;
      if (Snincr > 1) {
        Mptr->SNINCR = MAXINT;
        Mptr->SNINCR_TotalDepth = MAXINT;
      }
    } else if (isSnowDepth(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: snow depth\n"));
      SnowDepth++;
      if (SnowDepth > 1) {
        memset(Mptr->snow_depth_group, '\0', sizeof(Mptr->snow_depth_group));
        Mptr->snow_depth = MAXINT;
      }
    } else if (isWaterEquivSnow(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: water equivalent snow\n"));
      WaterEquivSnow++;
      if (WaterEquivSnow > 1) {
        Mptr->WaterEquivSnow = (float)MAXINT;
      }
    } else if (isSunshineDuration(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: sunshine duration\n"));
      SunshineDur++;
      if (SunshineDur > 1) {
        Mptr->SunshineDur = MAXINT;
        Mptr->SunSensorOut = FALSE;
      }
    } else if (isHourlyPrecipitation(&(token[NDEX]), Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: hourly precipitation\n"));
      hourlyPrecip++;
      if (hourlyPrecip > 1) {
        Mptr->hourlyPrecip = (float)MAXINT;
      }
    } else if (isP6Precipitation(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: P6 precipitation\n"));
      P6Precip++;
      if (P6Precip > 1) {
        Mptr->precip_amt = (float)MAXINT;
      }
    } else if (isP24Precip(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: P24 precipitation\n"));
      P24Precip++;
      if (P24Precip > 1) {
        Mptr->precip_24_amt = (float)MAXINT;
      }
    } else if (isTTdTenths(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: TTdTenths\n"));
      TTdTenths++;
      if (TTdTenths > 1) {
        Mptr->Temp_2_tenths = (float)MAXINT;
        Mptr->DP_Temp_2_tenths = (float)MAXINT;
      }
    } else if (isMaxTemperature(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: max temperature\n"));
      MaxTemp++;
      if (MaxTemp > 1) {
        Mptr->maxtemp = (float)MAXINT;
      }
    } else if (isMinTemperature(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: min temperature\n"));
      MinTemp++;
      if (MinTemp > 1) {
        Mptr->mintemp = (float)MAXINT;
      }
    } else if (isT24MaxMinTemp(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: T24 max min temp\n"));
      T24MaxMinTemp++;
      if (T24MaxMinTemp > 1) {
        Mptr->max24temp = (float)MAXINT;
        Mptr->min24temp = (float)MAXINT;
      }
    } else if (isPtendency(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: P tendency\n"));
      Ptendency++;
      if (Ptendency > 1) {
        Mptr->char_prestndcy = MAXINT;
        Mptr->prestndcy = (float)MAXINT;
      }
    } else if (isPWINO(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: PWINO\n"));
      PWINO++;
      if (PWINO > 1) {
        Mptr->PWINO = FALSE;
      }
    } else if (isFZRANO(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: FZRANO\n"));
      FZRANO++;
      if (FZRANO > 1) {
        Mptr->FZRANO = FALSE;
      }
    } else if (isTSNO(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: TSNO\n"));
      TSNO++;
      if (TSNO > 1) {
        Mptr->TSNO = FALSE;
      }
    } else if (isDollarSign(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: maint indicator\n"));
      maintIndicator++;
      if (maintIndicator > 1) {
        Mptr->DollarSign = FALSE;
      }
    } else if (isRVRNO(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: RVRNO\n"));
      RVRNO++;
      if (RVRNO > 1) {
        Mptr->RVRNO = FALSE;
      }
    } else if (isPNO(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: PNO\n"));
      PNO++;
      if (PNO > 1) {
        Mptr->PNO = FALSE;
      }
    } else if (isVISNO(&(token[NDEX]), Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: VISNO\n"));
      VISNO++;
      if (VISNO > 1) {
        Mptr->VISNO = FALSE;
        memset(Mptr->VISNO_LOC, '\0', sizeof(Mptr->VISNO_LOC));
      }
    } else if (isCHINO(&(token[NDEX]), Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: CHINO\n"));
      CHINO++;
      if (CHINO > 1) {
        Mptr->CHINO = FALSE;
        memset(Mptr->CHINO_LOC, '\0', sizeof(Mptr->CHINO_LOC));
      }
    } else if (isDVR(token[NDEX], Mptr, &NDEX)) {
      DEBUG_PRINTF(("decode_metar_remark: DVR\n"));
      DVR++;
      if (DVR > 1) {
        Mptr->DVR.Min_visRange = MAXINT;
        Mptr->DVR.Max_visRange = MAXINT;
        Mptr->DVR.visRange = MAXINT;
        Mptr->DVR.vrbl_visRange = FALSE;
        Mptr->DVR.below_min_DVR = FALSE;
        Mptr->DVR.above_max_DVR = FALSE;
      }
    } else {
#ifdef DEBUGZZ
      printf("decode_metar_remark:  punting on RMKs: token[%d] = %s\n", NDEX,
             token[NDEX]);
#endif
      NDEX++;
    }
  }

  return;
}
