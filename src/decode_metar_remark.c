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
#include "remark_quality_flags.h"
#include "remark_thermal_pressure.h"
#include "remark_precip_snow.h"
#include "remark_cloud.h"
#include "remark_wind_storm.h"
#include "remark_visibility.h"

#define SKY1_len 50
float fracPart( const char * );

#ifdef DEBUGZZ
# define DEBUG_PRINTF(x) printf x
#else
# define DEBUG_PRINTF(x)
#endif

/********************************************************************/
/*                                                                  */
/*  Title:         isTS_LOC                                         */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          06 May 1996                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:   Identify the input character string as a thunder-   */
/*              storm location.  If the input string is a thunder-  */
/*              storm location, then return TRUE.  Otherwise,       */
/*              return FALSE.                                       */
/*                                                                  */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         string    - a pointer to a pointer to a charac-  */
/*                             ter string from a METAR report.      */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if the input string is a thunderstorm     */
/*                        location.                                 */
/*                 FALSE - the input string is not a thunderstorm   */
/*                         location.                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/*  Title:         isDVR                                            */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/*  Title:         isRADAT                                          */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          07 Nov 1996                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:   Determines whether or not the input string is       */
/*              the 'RADAT' group elevation indicator.  If it is,   */
/*              then skip past the 'RADAT' indicator and also the   */
/*              next group which is the RADAT elevation informa-    */
/*              tion.                                               */
/*                                                                  */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         string - the address of a pointer to a charac-   */
/*                          ter string that may or may not be the   */
/*                          RADAT group.                            */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if a RADAT group is found.                */
/*                 FALSE - if no RADAT group is found.              */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/*  Title:         isTornadicActivity                               */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:   Determines whether or not the input character       */
/*              string is signals the beginning of TORNADIC         */
/*              ACTIVITY data.  If it is, then interrogate subse-   */
/*              quent report groups for time, location, and movement*/
/*              of tornado.  Return TRUE, if TORNADIC ACTIVITY is   */
/*              found.  Otherwise, return FALSE.                    */
/*                                                                  */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         string - the address of a pointer to a charac-   */
/*                          ter string that may or may not signal   */
/*                          TORNADIC ACTIVITY.                      */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if TORNADIC ACTIVITY is found.            */
/*                 FALSE - if no TORNADIC ACTIVITY is found.        */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/*  Title:         isPartialObscuration                             */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:   Determine whether or not the input character string */
/*              is a partial obscuration phenomenon.  If a partial  */
/*              obscuration is found, then take the preceding group */
/*              as the obscuring phenomenon.  If a partial obscura- */
/*              tion is found, then return TRUE.  Otherwise, return */
/*              false.                                              */
/*                                                                  */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         string - the address of a pointer to a group     */
/*                          in a METAR report that may or may not   */
/*                          be a partial obscuration indicator.     */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if the input string is a partial obscura- */
/*                        tion.                                     */
/*                 FALSE - if the input string is not a partial ob- */
/*                         scuration.                               */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/*  Title:         isA0indicator                                    */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:   Identify the input character string as an automated */
/*              station code type.  If the input character string   */
/*              is an automated station code type, then return      */
/*              TRUE.  Otherwise, return FALSE.                     */
/*                                                                  */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         indicator - a pointer to a character string      */
/*                             that may or may not be an ASOS       */
/*                             automated station code type.         */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if the input string matches one of the    */
/*                        valid ASOS automated station indicators.  */
/*                 FALSE - the input string did not match one of the*/
/*                        valid ASOS automated station indicators.  */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/*  Title:         isPeakWind                                       */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  Determine whether or not the current and subsequent  */
/*             groups from the METAR report make up a valid report  */
/*             of peak wind.                                        */
/*                                                                  */
/*                                                                  */
/*  Input:         string - the addr of a ptr to a character string */
/*                             that may or may not be the indicator */
/*                             for a peak wind data group.          */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if the input string (and subsequent grps) */
/*                        are decoded as peak wind.                 */
/*                 FALSE - if the input string (and subsequent grps)*/
/*                         are not decoded as peak wind.            */
/*  External Functions Called:                                      */
/*                 nisdigit                                         */
/*                                                                  */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/*  Title:         isWindShift                                      */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  Determine whether or not the current and subsequent  */
/*             groups from the METAR report make up a valid report  */
/*             of wind shift and frontal passage, if included.      */
/*                                                                  */
/*                                                                  */
/*  Input:         string - the addr of a ptr to a character string */
/*                           that may or may not be the indicator   */
/*                           for a wind shift data group.           */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if the input string (and subsequent grps) */
/*                        are decoded as wind shift.                */
/*                 FALSE - if the input string (and subsequent grps)*/
/*                         are not decoded as wind shift.           */
/*  External Functions Called:                                      */
/*                 nisdigit                                         */
/*                                                                  */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/*  Title:         isTowerVisibility                                */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  Determine whether or not the current and subsequent  */
/*             groups from the METAR report make up a valid report  */
/*             of tower visibility.                                 */
/*                                                                  */
/*                                                                  */
/*  Input:         string - the addr of a ptr to a character string */
/*                          that may or may not be the indicator    */
/*                          for tower visibility.                   */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if the input string (and subsequent grps) */
/*                        are decoded as tower visibility.          */
/*                 FALSE - if the input string (and subsequent grps)*/
/*                         are not decoded as tower visibility      */
/*  External Functions Called:                                      */
/*                 nisdigit                                         */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/*  Title:         isSurfaceVisibility                              */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  Determine whether or not the current and subsequent  */
/*             groups from the METAR report make up a valid report  */
/*             of surface visibility.                               */
/*                                                                  */
/*                                                                  */
/*  Input:         string - the addr of a ptr to a character string */
/*                          that may or may not be the indicator    */
/*                          for surface visibility.                 */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if the input string (and subsequent grps) */
/*                        are decoded as surface visibility.        */
/*                 FALSE - if the input string (and subsequent grps)*/
/*                         are not decoded as surface visibility.   */
/*  External Functions Called:                                      */
/*                 nisdigit                                         */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/*  Title:         isVariableVisibility                             */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          21 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  Determine whether or not the current and subsequent  */
/*             groups from the METAR report make up a valid report  */
/*             of variable prevailing visibility.                   */
/*                                                                  */
/*                                                                  */
/*  Input:         string - the addr of a ptr to a character string */
/*                          that may or may not be the indicator    */
/*                          for variable prevailing visibility.     */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if the input string (and subsequent grps) */
/*                        are decoded as variable prevailing vsby.  */
/*                 FALSE - if the input string (and subsequent grps)*/
/*                         are not decoded as variable prevailing   */
/*                         vsby.                                    */
/*  External Functions Called:                                      */
/*                 nisdigit                                         */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/*  Title:         isVisibility2ndSite                              */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  Determine whether or not the current and subsequent  */
/*             groups from the METAR report make up a valid report  */
/*             of visibility at a secondary site.                   */
/*                                                                  */
/*                                                                  */
/*  Input:         token  - the addr of a ptr to a character string */
/*                          that may or may not be the indicator    */
/*                          for visibility at a secondary site.     */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if the input string (and subsequent grps) */
/*                        are decoded as visibility at a 2ndry site.*/
/*                 FALSE - if the input string (and subsequent grps)*/
/*                         are not decoded as visibility at a 2ndry */
/*                         site.                                    */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 nisalnum                                         */
/*                 fracPart                                         */
/*                 nisdigit                                         */
/*                                                                  */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/*  Title:         isLightningFrequency                             */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  Determine whether or not the current and subsequent  */
/*             groups from the METAR report make up a valid report  */
/*             of lightning.                                        */
/*                                                                  */
/*                                                                  */
/*  Input:        string  - the addr of a ptr to a character string */
/*                          that may or may not be the indicator    */
/*                          for lightning.                          */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the index */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the index   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if the input string (and subsequent grps) */
/*                        are decoded as lightning.                 */
/*                 FALSE - if the input string (and subsequent grps)*/
/*                         are not decoded as lightning.            */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 NONE.                                            */
/*                                                                  */
/*                                                                  */
/*  Modification History:                                           */
/*                 09 May 1996:  Software modified to properly      */
/*                               decode lightning types.            */
/*                                                                  */
/*                                                                  */
/*                                                                  */
/********************************************************************/




#include "metar_structs.h"     /* standard header file */

/********************************************************************/
/*                                                                  */
/*  Title:         isRecentWeather                                  */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:  Determine whether or not the current and subsequent  */
/*             groups from the METAR report make up a valid report  */
/*             recent weather.                                      */
/*                                                                  */
/*  Input:         token  - the addr of a ptr to a character token  */
/*                          that may or may not be a recent weather */
/*                          group.                                  */
/*                                                                  */
/*                 Mptr - a pointer to a structure that has the     */
/*                        data type Decoded_METAR.                  */
/*                                                                  */
/*                 NDEX - a pointer to an integer that is the i*NDEX */
/*                        into an array that contains the indi-     */
/*                        vidual groups of the METAR report being   */
/*                        decoded.  Upon entry, NDEX is the i*NDEX   */
/*                        of the current group of the METAR report  */
/*                        that is to be indentified.                */
/*                                                                  */
/*  Output:        TRUE - if the input token (and possibly subse-  */
/*                        quent groups) are decoded as recent wx.   */
/*                 FALSE - if the input token (and possibly subse- */
/*                         quent groups) are not decoded as recent  */
/*                         wx.                                      */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 nisdigit                                         */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

static MDSP_BOOL isRecentWX( char **token, Decoded_METAR *Mptr,
                             int *NDEX )
{
	static char *phenom[] = {"FCB", "-DZB", "DZB", "+DZB",
	                         "FZDZB", "-RAB", "RAB", "+RAB",
	                         "SHRAB", "TSRAB", "FZRAB", "-SNB",
	                         "SNB", "+SNB", "DRSNB", "BLSNB",
	                         "SHSNB", "TSSNB", "-SGB", "SGB",
	                         "+SGB", "ICB", "-PEB", "PEB", "+PEB",
	                         "SHPEB", "TSPEB", "GRB", "SHGRB",
	                         "TSGRB", "GSB", "SHGSB", "TSGSB", "-GSB",
	                         "+GSB", "TSB", "VCTSB", "-TSRAB",
	                         "TSRAB", "+TSRAB", "-TSSNB", "TSSNB",
	                         "+TSSNB", "-TSPEB", "TSPEB", "+TSPEB",
	                         "-TSGSB", "TSGSB", "+TSGSB",
	                         "VCSHB", "-SHRAB", "+SHRAB", "-SHSNB",
	                         "+SHSNB", "-SHPEB", "+SHPEB",
	                         "-SHGSB", "+SHGSB", "-FZDZB", "+FZDZB",
	                         "-FZRAB", "+FZRAB", "FZFGB",
	                         "+FZFGB", "BRB", "FGB", "VCFGB", "MIFGB",
	                         "PRFGB", "BCFGB", "FUB",
	                         "VAB", "DUB", "DRDUB", "BLDUB", "SAB",
	                         "DRSAB", "BLSAB", "HZB",
	                         "BLPYB", "BLSNB", "+BLSNB", "VCBLSNB",
	                         "BLSAB", "+BLSAB",
	                         "VCBLSAB", "+BLDUB", "VCBLDUB", "POB",
	                         "VCPOB", "SQB", "FCB", "+FCB",
	                         "VCFCB", "SSB", "+SSB", "VCSSB", "DSB",
	                         "+DSB", "VCDSB",


	                         "FCE", "-DZE", "DZE", "+DZE",
	                         "FZDZE", "-RAE", "RAE", "+RAE",
	                         "SHRAE", "TSRAE", "FZRAE", "-SNE",
	                         "SNE", "+SNE", "DRSNE", "BLSNE",
	                         "SHSNE", "TSSNE", "-SGE", "SGE",
	                         "+SGE", "ICE", "-PEE", "PEE", "+PEE",
	                         "SHPEE", "TSPEE", "GRE", "SHGRE",
	                         "TSGRE", "GSE", "SHGSE", "TSGSE", "-GSE",
	                         "+GSE", "TSE", "VCTSE", "-TSRAE",
	                         "TSRAE", "+TSRAE", "-TSSNE", "TSSNE",
	                         "+TSSNE", "-TSPEE", "TSPEE", "+TSPEE",
	                         "-TSGSE", "TSGSE", "+TSGSE",
	                         "VCSHE", "-SHRAE", "+SHRAE", "-SHSNE",
	                         "+SHSNE", "-SHPEE", "+SHPEE",
	                         "-SHGSE", "+SHGSE", "-FZDZE", "+FZDZE",
	                         "-FZRAE", "+FZRAE", "FZFGE",
	                         "+FZFGE", "BRE", "FGE", "VCFGE", "MIFGE",
	                         "PRFGE", "BCFGE", "FUE",
	                         "VAE", "DUE", "DRDUE", "BLDUE", "SAE",
	                         "DRSAE", "BLSAE", "HZE",
	                         "BLPYE", "BLSNE", "+BLSNE", "VCBLSNE",
	                         "BLSAE", "+BLSAE",
	                         "VCBLSAE", "+BLDUE", "VCBLDUE", "POE",
	                         "VCPOE", "SQE", "FCE", "+FCE",
	                         "VCFCE", "SSE", "+SSE", "VCSSE", "DSE",
	                         "+DSE", "VCDSE", "4-Zs"
	                        };

	int i,
	    beg_hour,
	    beg_min,
	    end_hour,
	    end_min;

	char *temp,
	     *free_temp,
	     *numb_char,
	     *C_char;


	if( *token == NULL )
		return FALSE;


	if( (free_temp = temp = (char *) malloc(sizeof(char) *
	                                        (strlen(*token) + 1))) == NULL ) {
		return FALSE;
	}
	else
		strcpy(temp,*token);



	while ( *temp != '\0' ) {
		/*
		printf("isRecentWX:  JUST inside while-loop, *NDEX = %d\n",*NDEX);
		printf("isRecentWX:  JUST inside while-loop, temp = %s\n",temp);
		*/
		i = 0;

		beg_hour = beg_min = end_hour = end_min = MAXINT;

		while( strncmp(temp, phenom[i],strlen(phenom[i])) != 0 &&
		        strcmp(phenom[i],"4-Zs") != 0 )
			i++;

		if( strcmp(phenom[i],"4-Zs") != 0 ) {

			C_char = (strlen(phenom[i]) - 1) + temp;
			numb_char = C_char + 1;

			if( *numb_char == '\0')
				return FALSE;

			if( nisdigit(numb_char,4) && strlen(numb_char) >= 4) {
				if( *C_char == 'B' ) {
					beg_hour = antoi( numb_char, 2 );
					beg_min = antoi( numb_char+2,2 );
					temp = numb_char+4;

					if( *NDEX < 3 ) {
						Mptr->ReWx[*NDEX].Bmm = beg_min;
						Mptr->ReWx[*NDEX].Bhh = beg_hour;
					}

					temp = numb_char + 4;

					if( *(numb_char+4) == 'E' ) {
						numb_char += 5;
						if( nisdigit(numb_char,4) &&
						        strlen(numb_char) >= 4 ) {
							end_hour = antoi( numb_char, 2 );
							end_min = antoi( numb_char+2,2 );
							temp = numb_char+4;

							if( *NDEX < 3 ) {
								Mptr->ReWx[*NDEX].Emm = end_min;
								Mptr->ReWx[*NDEX].Ehh = end_hour;
							}

							temp = numb_char + 4;

						}
						else if( nisdigit(numb_char,2) &&
						         strlen(numb_char) >= 2 ) {
							end_min = antoi( numb_char,2 );

							if( *NDEX < 3 ) {
								Mptr->ReWx[*NDEX].Emm = end_min;
							}
							temp = numb_char+2;
						}

					}

					if( *NDEX < 3 ) {
						strncpy(Mptr->ReWx[*NDEX].Recent_weather,
						        phenom[i], (strlen(phenom[i])-1) );
						(*NDEX)++;
					}
					if( *temp == '\0' ) {
						free( free_temp );
						return TRUE;
					}

				}
				else {
					end_hour = antoi( numb_char, 2 );
					end_min = antoi( numb_char+2,2 );

					temp = numb_char + 4;

					if( *NDEX < 3 ) {
						Mptr->ReWx[*NDEX].Emm = end_min;
						Mptr->ReWx[*NDEX].Ehh = end_hour;

					}

					temp = numb_char+4;

					if( *(numb_char+4) == 'B' ) {
						numb_char += 5;
						if( nisdigit(numb_char,4) &&
						        strlen(numb_char) >= 4 ) {

							beg_hour = antoi(numb_char,2);
							beg_min  = antoi(numb_char+2,2);
							temp = numb_char + 4;

							if( *NDEX < 3 ) {
								Mptr->ReWx[*NDEX].Bmm = beg_min;
								Mptr->ReWx[*NDEX].Bhh = beg_hour;

							}

							temp = numb_char+4;
						}
						else if( nisdigit(numb_char,2) &&
						         strlen(numb_char) >= 2 ) {
							beg_min = antoi( numb_char,2 );

							if( *NDEX < 3 ) {
								Mptr->ReWx[*NDEX].Bmm = beg_min;
							}

							temp = numb_char+2;
						}

					}

					if( *NDEX < 3 ) {
						strncpy(Mptr->ReWx[*NDEX].Recent_weather,
						        phenom[i], (strlen(phenom[i])-1) );
						(*NDEX)++;
					}

					if( *temp == '\0' ) {
						free( free_temp );
						return TRUE;
					}

				}

			}
			else if(nisdigit(numb_char,2) && strlen(numb_char) >= 2 ) {
				if( *C_char == 'B' ) {
					beg_min = antoi( numb_char,2 );

					if( *NDEX < 3 ) {
						Mptr->ReWx[*NDEX].Bmm = beg_min;


					}

					temp = numb_char+2;

					if( *(numb_char+2) == 'E' ) {
						numb_char += 3;
						if( nisdigit(numb_char,4) &&
						        strlen(numb_char) >= 4 ) {
							end_hour = antoi( numb_char,2 );
							end_min = antoi( numb_char+2,2 );

							if( *NDEX < 3 ) {
								Mptr->ReWx[*NDEX].Emm = end_min;
								Mptr->ReWx[*NDEX].Ehh = end_hour;


							}

							temp = numb_char+4;
						}
						else if( nisdigit(numb_char,2) &&
						         strlen(numb_char) >= 2 ) {
							end_min = antoi( numb_char,2 );

							if( *NDEX < 3 )
								Mptr->ReWx[*NDEX].Emm = end_min;


							temp = numb_char+2;
						}
					}
					if( *NDEX < 3 ) {
						strncpy( Mptr->ReWx[*NDEX].Recent_weather,
						         phenom[i], (strlen(phenom[i])-1) );

						(*NDEX)++;
					}
					if( *temp == '\0' ) {
						free( free_temp );
						return TRUE;
					}
				}
				else {
					end_min = antoi( numb_char, 2 );

					if( *NDEX < 3 )
						Mptr->ReWx[*NDEX].Emm = end_min;

					temp = numb_char+2;

					if( *(numb_char+2) == 'B' ) {
						numb_char += 3;
						if( nisdigit(numb_char,4) &&
						        strlen(numb_char) >= 4 ) {
							beg_hour = antoi( numb_char,2 );
							beg_min = antoi( numb_char+2,2 );

							if( *NDEX < 3 ) {
								Mptr->ReWx[*NDEX].Bmm = beg_min;
								Mptr->ReWx[*NDEX].Bhh = beg_hour;

							}

							temp = numb_char+4;
						}
						else if( nisdigit(numb_char,2) &&
						         strlen(numb_char) >= 2 ) {
							beg_min = antoi( numb_char,2 );

							if( *NDEX < 3 )
								Mptr->ReWx[*NDEX].Bmm = beg_min;


							temp = numb_char+2;
						}

					}
					if( *NDEX < 3 ) {
						strncpy( Mptr->ReWx[*NDEX].Recent_weather,
						         phenom[i], (strlen(phenom[i])-1) );
						(*NDEX)++;
					}
					if( *temp == '\0' ) {
						free( free_temp );
						return TRUE;
					}

				}

			}
			else {
				free( free_temp );

				if( *NDEX > 0 && *NDEX < 3 )
					return TRUE;
				else
					return FALSE;
			}

		}
		else {
			free( free_temp );
			return FALSE;
		}

	}
	// should not reach here
	return FALSE;
}


/********************************************************************/
/*                                                                  */
/*  Title:         isVariableCIG                                    */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          21 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      isVariableCIG determines whether or not the      */
/*                 current group in combination with the next       */
/*                 one or more groups is a report of variable       */
/*                 ceiling.                                         */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 nisdigit                                         */
/*                                                                  */
/*  Input:         token - a pointer to an array of METAR report    */
/*                           groups.                                */
/*                 Mptr - a pointer to a decoded_METAR structure    */
/*                 NDEX - the index value of the current METAR      */
/*                        report group array element.               */
/*                                                                  */
/*  Output:        TRUE, if the token is currently pointing to      */
/*                 METAR report group(s) that a report of vari-     */
/*                 ble ceiling.                                     */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/*  Title:         isCeil2ndSite                                    */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      isCeil2ndSite determines whether or not the      */
/*                 current group in combination with the next       */
/*                 one or more groups is a report of a ceiling      */
/*                 at a secondary site.                             */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 nisdigit                                         */
/*                                                                  */
/*  Input:         token - a pointer to an array of METAR report    */
/*                           groups.                                */
/*                 Mptr - a pointer to a decoded_METAR structure    */
/*                 NDEX - the index value of the current METAR      */
/*                        report group array element.               */
/*                                                                  */
/*  Output:        TRUE, if the token is currently pointing to      */
/*                 METAR report group(s) that are reporting         */
/*                 ceiling at a secondary site.                     */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 nisdigit                                         */
/*                                                                  */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/*  Title:         isPRESFR                                         */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Input:         x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

/********************************************************************/
/*                                                                  */
/*  Title:         isPRESRR                                         */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Input:         x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

/********************************************************************/
/*                                                                  */
/*  Title:         isSLP                                            */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/




/********************************************************************/
/*                                                                  */
/*  Title:         isGR                                             */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Input:         x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/*  Title:         isVIRGA                                          */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Input:         x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/











/********************************************************************/
/*                                                                  */
/*  Title:         isSynopticClouds                                 */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/






/********************************************************************/
/*                                                                  */
/*  Title:         isSnowDepth                                      */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/








/********************************************************************/
/*                                                                  */
/*  Title:         isHourlyPrecipitation                            */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Input:         x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/*  Title:         isP6Precipitation                                */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Input:         x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/*  Title:         isP24Precip                                      */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Input:         x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/



/********************************************************************/
/*                                                                  */
/*  Title:         isTTdTenths                                      */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          16 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Input:         x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

/********************************************************************/
/*                                                                  */
/*  Title:         isMaxTemperature                                 */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

/********************************************************************/
/*                                                                  */
/*  Title:         isMinTemperature                                 */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

/********************************************************************/
/*                                                                  */
/*  Title:         isT24MaxMinTemp                                  */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

/********************************************************************/
/*                                                                  */
/*  Title:         isPtendency                                      */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

/********************************************************************/
/*                                                                  */
/*  Title:         isPWINO                                          */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

/********************************************************************/
/*                                                                  */
/*  Title:         isPNO                                            */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

/********************************************************************/
/*                                                                  */
/*  Title:         isRVRNO                                          */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

/********************************************************************/
/*                                                                  */
/*  Title:         isCHINO                                          */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

/********************************************************************/
/*                                                                  */
/*  Title:         isVISNO                                          */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

/********************************************************************/
/*                                                                  */
/*  Title:         isFZRANO                                         */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

/********************************************************************/
/*                                                                  */
/*  Title:         isTSNO                                            */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          20 Nov 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:      x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Input:         x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                 x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

/********************************************************************/
/*                                                                  */
/*  Title:         isDollarSign                                 */
/*  Organization:  W/OSO242 - GRAPHICS AND DISPLAY SECTION          */
/*  Date:          15 Sep 1994                                      */
/*  Programmer:    CARL MCCALLA                                     */
/*  Language:      C/370                                            */
/*                                                                  */
/*  Abstract:                                                       */
/*                                                                  */
/*  External Functions Called:                                      */
/*                 None.                                            */
/*                                                                  */
/*  Input:         x                                                */
/*                                                                  */
/*  Output:        x                                                */
/*                                                                  */
/*  Modification History:                                           */
/*                 None.                                            */
/*                                                                  */
/********************************************************************/

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

void decode_metar_remark(char **token, Decoded_METAR *Mptr)
{
	int TornadicActvty = 0, A0indicator = 0,
	    peakwind = 0, windshift = 0, towerVsby = 0, surfaceVsby = 0,
	    variableVsby = 0, LTGfreq = 0,
	    TS_LOC = 0,
	    recentWX = 0, variableCIG = 0, PRESFR = 0,
	    Vsby2ndSite = 0, CIG2ndSite = 0,
	    PRESRR = 0, SLP = 0, PartObscur = 0,
	    SectorVsby = 0, GR = 0, Virga = 0,
	    SfcObscur = 0, Ceiling = 0, VrbSkyCond = 0, ObscurAloft = 0,
	    NoSPECI = 0, Last = 0, SynopClouds = 0, Snincr = 0,
	    SnowDepth = 0, WaterEquivSnow = 0, SunshineDur = 0,
	    hourlyPrecip = 0, P6Precip = 0, P24Precip = 0,
	    TTdTenths = 0, MaxTemp = 0, MinTemp = 0, T24MaxMinTemp = 0,
	    Ptendency = 0, PWINO = 0,
	    FZRANO = 0, TSNO = 0, maintIndicator = 0, CHINO = 0, RVRNO = 0,
	    VISNO = 0, PNO = 0, DVR = 0;

	int  NDEX, i;

	MDSP_BOOL IS_NOT_RMKS;

	NDEX = 0;

	// locate the start of the metar remarks section

	IS_NOT_RMKS = TRUE;

	while (token[ NDEX ] != NULL && IS_NOT_RMKS) {
#ifdef DEBUGZZ
		printf("decode_metar_remark:  token[%d] = %s\n", NDEX, token[NDEX]);
#endif
		if (strcmp(token[ NDEX ], "RMK") != 0) {
			NDEX++;
		} else {
			IS_NOT_RMKS = FALSE;
		}
	}

	// if the metar report contains no remarks section, return

	if (token[ NDEX ] != NULL) {
#ifdef DEBUGZZ
		printf("decode_metar_remark:  RMK found, token[%d] = %s\n",
		       NDEX,token[NDEX]);
#endif
		NDEX++;
#ifdef DEBUGZZ
		printf("decode_metar_remark:  Bump NDEX, token[%d] = %s\n",
		       NDEX,token[NDEX]);
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
		printf("decode_metar_remark:  decode RMK: token[%d] = %s\n", NDEX, token[NDEX]);
#endif

		isRADAT( &(token[NDEX]), Mptr, &NDEX );

		if (isTornadicActivity (&(token[NDEX]), Mptr, &NDEX)) {
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
		} else if (isA0indicator( token[NDEX], Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: A0 indicator\n"));
			A0indicator++;
			if (A0indicator > 1) {
				memset(Mptr->autoIndicator, '\0', sizeof(Mptr->autoIndicator));
			}
		} else if (isPeakWind( &(token[NDEX]), Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: peak wind\n"));
			peakwind++;
			if (peakwind > 1) {
				Mptr->PKWND_dir = MAXINT;
				Mptr->PKWND_speed = MAXINT;
				Mptr->PKWND_hour = MAXINT;
				Mptr->PKWND_minute = MAXINT;
			}
		} else if (isWindShift( &(token[NDEX]), Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: wind shift\n"));
			windshift++;
			if (windshift > 1) {
				Mptr->WshfTime_hour = MAXINT;
				Mptr->WshfTime_minute = MAXINT;
			}
		} else if (isTowerVisibility( &(token[NDEX]), Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: tower visibility\n"));
			towerVsby++;
			if (towerVsby > 1) {
				Mptr->TWR_VSBY = (float) MAXINT;
			}
		} else if (isSurfaceVisibility( &(token[NDEX]), Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: surface visibility\n"));
			surfaceVsby++;
			if (surfaceVsby > 1) {
				Mptr->SFC_VSBY = (float) MAXINT;
			}
		} else if (isVariableVisibility( &(token[NDEX]), Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: variable visibility\n"));
			variableVsby++;
			if (variableVsby > 1) {
				Mptr->minVsby = (float) MAXINT;
				Mptr->maxVsby = (float) MAXINT;
			}
		} else if (isVisibility2ndSite(&(token[NDEX]), Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: 2nd site visibility\n"));
			Vsby2ndSite++;
			if (Vsby2ndSite > 1) {
				Mptr->VSBY_2ndSite = (float) MAXINT;
				memset(Mptr->VSBY_2ndSite_LOC, '\0', sizeof(Mptr->VSBY_2ndSite_LOC));
			}
		} else if (isLightningFrequency( &(token[NDEX]), Mptr, &NDEX)) {
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
		} else if (isTS_LOC( &(token[NDEX]), Mptr, &NDEX)) {
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
			if( variableCIG > 1) {
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
		} else if (isPRESFR( token[NDEX], Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: PRESFR\n"));
			PRESFR++;
			if (PRESFR > 1) {
				Mptr->PRESFR = FALSE;
			}
		} else if (isPRESRR( token[NDEX], Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: PRESRR\n"));
			PRESRR++;
			if (PRESRR > 1) {
				Mptr->PRESRR = FALSE;
			}
		} else if (isSLP( &(token[NDEX]), Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: SLP\n"));
			SLP++;
			if (SLP > 1) {
				Mptr->SLP = (float) MAXINT;
			}
		} else if (isPartialObscuration( &(token[NDEX]), Mptr, PartObscur,
		                                 &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: partial obscuration\n"));
			PartObscur++;
			if (PartObscur > 2) {
				memset(Mptr->PartialObscurationAmt[0], '\0', sizeof(Mptr->PartialObscurationAmt[0]) );
				memset(Mptr->PartialObscurationPhenom[0],'\0', sizeof(Mptr->PartialObscurationPhenom[0]));

				memset(Mptr->PartialObscurationAmt[1], '\0', sizeof(Mptr->PartialObscurationAmt[1]));
				memset(Mptr->PartialObscurationPhenom[1],'\0', sizeof(Mptr->PartialObscurationPhenom[1]));
			}
		} else if (isSectorVsby( &(token[NDEX]), Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: sector visibility\n"));
			SectorVsby++;
			if( SectorVsby > 1 ) {
				Mptr->SectorVsby = (float) MAXINT;
				memset(Mptr->SectorVsby_Dir, '\0', sizeof(Mptr->SectorVsby_Dir));
			}
		} else if (isGR( &(token[NDEX]), Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: GR\n"));
			GR++;
			if (GR > 1) {
				Mptr->GR_Size = (float) MAXINT;
				Mptr->GR = FALSE;
			}
		} else if (isVIRGA( &(token[NDEX]), Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: VIRGA\n"));
			Virga++;
			if (Virga > 1) {
				Mptr->VIRGA = FALSE;
				memset(Mptr->VIRGA_DIR, '\0', sizeof(Mptr->VIRGA_DIR));
			}
		} else if (isSurfaceObscuration( token[NDEX], Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: surface obscuration\n"));
			SfcObscur++;
			if (SfcObscur > 1) {
				for (i = 0; i < MAX_SURFACE_OBSCURATIONS; i++) {
					memset(&(Mptr->SfcObscuration[i][0]), '\0', sizeof(Mptr->SfcObscuration[i]));
					Mptr->Num8thsSkyObscured = MAXINT;
				}
			}
		} else if (isCeiling( token[NDEX], Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: ceiling\n"));
			Ceiling++;
			if (Ceiling > 1) {
				Mptr->CIGNO = FALSE;
				Mptr->Ceiling = MAXINT;
				Mptr->Estimated_Ceiling = FALSE;
			}
		} else if (isVariableSky( &(token[NDEX]), Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: variable sky\n"));
			VrbSkyCond++;
			if (VrbSkyCond > 1) {
				memset(Mptr->VrbSkyBelow, '\0', sizeof(Mptr->VrbSkyBelow));
				memset(Mptr->VrbSkyAbove, '\0', sizeof(Mptr->VrbSkyAbove));
				Mptr->VrbSkyLayerHgt = MAXINT;
			}
		} else if (isObscurationAloft( &(token[NDEX]), Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: obscuration aloft\n"));
			ObscurAloft++;
			if (ObscurAloft > 1) {
				Mptr->ObscurAloftHgt = MAXINT;
				memset( Mptr->ObscurAloft, '\0', sizeof(Mptr->ObscurAloft));
				memset( Mptr->ObscurAloftSkyCond, '\0', sizeof(Mptr->ObscurAloftSkyCond));
			}
		} else if (isNOSPECI( token[NDEX], Mptr, &NDEX)) {
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
		} else if (isSynopticClouds( token[NDEX], Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: Synoptic Clouds\n"));
			SynopClouds++;
			if( SynopClouds > 1 ) {
				memset( Mptr->synoptic_cloud_type, '\0', sizeof(Mptr->synoptic_cloud_type));
				Mptr->CloudLow    = '\0';
				Mptr->CloudMedium = '\0';
				Mptr->CloudHigh   = '\0';
			}
		} else if (isSNINCR( &(token[NDEX]), Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: SNINCR\n"));
			Snincr++;
			if( Snincr > 1 ) {
				Mptr->SNINCR = MAXINT;
				Mptr->SNINCR_TotalDepth = MAXINT;
			}
		} else if (isSnowDepth( token[NDEX], Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: snow depth\n"));
			SnowDepth++;
			if (SnowDepth > 1) {
				memset(Mptr->snow_depth_group, '\0', sizeof(Mptr->snow_depth_group));
				Mptr->snow_depth = MAXINT;
			}
		} else if (isWaterEquivSnow( token[NDEX], Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: water equivalent snow\n"));
			WaterEquivSnow++;
			if (WaterEquivSnow > 1) {
				Mptr->WaterEquivSnow = (float) MAXINT;
			}
		} else if (isSunshineDuration(token[NDEX], Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: sunshine duration\n"));
			SunshineDur++;
			if (SunshineDur > 1) {
				Mptr->SunshineDur = MAXINT;
				Mptr->SunSensorOut = FALSE;
			}
		} else if (isHourlyPrecipitation( &(token[NDEX]), Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: hourly precipitation\n"));
			hourlyPrecip++;
			if (hourlyPrecip > 1) {
				Mptr->hourlyPrecip = (float) MAXINT;
			}
		} else if (isP6Precipitation( token[NDEX], Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: P6 precipitation\n"));
			P6Precip++;
			if(P6Precip > 1) {
				Mptr->precip_amt = (float) MAXINT;
			}
		} else if (isP24Precip( token[NDEX], Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: P24 precipitation\n"));
			P24Precip++;
			if (P24Precip > 1) {
				Mptr->precip_24_amt = (float) MAXINT;
			}
		} else if (isTTdTenths(token[NDEX], Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: TTdTenths\n"));
			TTdTenths++;
			if( TTdTenths > 1 ) {
				Mptr->Temp_2_tenths = (float) MAXINT;
				Mptr->DP_Temp_2_tenths = (float) MAXINT;
			}
		} else if (isMaxTemperature( token[NDEX], Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: max temperature\n"));
			MaxTemp++;
			if ( MaxTemp > 1) {
				Mptr->maxtemp = (float) MAXINT;
			}
		} else if (isMinTemperature(token[NDEX], Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: min temperature\n"));
			MinTemp++;
			if (MinTemp > 1) {
				Mptr->mintemp = (float) MAXINT;
			}
		} else if (isT24MaxMinTemp( token[NDEX], Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: T24 max min temp\n"));
			T24MaxMinTemp++;
			if( T24MaxMinTemp > 1 ) {
				Mptr->max24temp = (float) MAXINT;
				Mptr->min24temp = (float) MAXINT;
			}
		} else if (isPtendency( token[NDEX], Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: P tendency\n"));
			Ptendency++;
			if (Ptendency > 1) {
				Mptr->char_prestndcy = MAXINT;
				Mptr->prestndcy = (float) MAXINT;
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
		} else if (isTSNO( token[NDEX], Mptr, &NDEX )) {
			DEBUG_PRINTF(("decode_metar_remark: TSNO\n"));
			TSNO++;
			if (TSNO > 1) {
				Mptr->TSNO = FALSE;
			}
		} else if (isDollarSign( token[NDEX], Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: maint indicator\n"));
			maintIndicator++;
			if (maintIndicator > 1) {
				Mptr->DollarSign = FALSE;
			}
		} else if( isRVRNO( token[NDEX], Mptr, &NDEX ) ) {
			DEBUG_PRINTF(("decode_metar_remark: RVRNO\n"));
			RVRNO++;
			if (RVRNO > 1) {
				Mptr->RVRNO = FALSE;
			}
		} else if (isPNO( token[NDEX], Mptr, &NDEX)) {
			DEBUG_PRINTF(("decode_metar_remark: PNO\n"));
			PNO++;
			if (PNO > 1) {
				Mptr->PNO = FALSE;
			}
		} else if (isVISNO( &(token[NDEX]), Mptr, &NDEX)) {
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
		} else if (isDVR( token[NDEX], Mptr, &NDEX)) {
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
			printf("decode_metar_remark:  punting on RMKs: token[%d] = %s\n",NDEX,token[NDEX]);
#endif
			NDEX++;
		}
	}

	return;
}
