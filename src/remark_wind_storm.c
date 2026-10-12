#include <stdlib.h>
#include <string.h>

#include "remark_wind_storm.h"

MDSP_BOOL isRADAT( char **string, Decoded_METAR *Mptr,
                          int *NDEX )
{
	if( strcmp( *string, "RADAT" ) != 0 )
		return FALSE;
	else {

		(*NDEX)++;
		(++string);

		if( *string == NULL )
			return TRUE;
		else {
			(*NDEX)++;
			(++string);

			return TRUE;
		}

	}

}

MDSP_BOOL isTS_LOC( char **string, Decoded_METAR *Mptr,
                           int *NDEX )
{

	/* COMPARE THE INPUT CHARACTER STRING WITH */
	/* VALID AUTOMATED STATION CODE TYPE.  IF  */
	/* A MATCH IS FOUND, RETURN TRUE.  OTHER-  */
	/*           WISE, RETURN FALSE            */

	if( *string == NULL )
		return FALSE;


	if( strcmp( *string, "TS") != 0 )
		return FALSE;
	else {
		string++;

		if( *string == NULL )
			return FALSE;

		if(    strcmp(*string,"N")  == 0  ||
		        strcmp(*string,"NE") == 0  ||
		        strcmp(*string,"NW") == 0  ||
		        strcmp(*string,"S")  == 0  ||
		        strcmp(*string,"SE") == 0  ||
		        strcmp(*string,"SW") == 0  ||
		        strcmp(*string,"E")  == 0  ||
		        strcmp(*string,"W")  == 0   ) {
			strcpy( Mptr->TS_LOC, *string );
			(*NDEX)++;
			(*NDEX)++;
			string++;

			if( *string == NULL )
				return TRUE;

			if( strcmp( *string, "MOV" ) == 0 ) {
				string++;

				if( *string == NULL ) {
					(*NDEX)++;
					return TRUE;
				}

				if(    strcmp(*string,"N")  == 0  ||
				        strcmp(*string,"NE") == 0  ||
				        strcmp(*string,"NW") == 0  ||
				        strcmp(*string,"S")  == 0  ||
				        strcmp(*string,"SE") == 0  ||
				        strcmp(*string,"SW") == 0  ||
				        strcmp(*string,"E")  == 0  ||
				        strcmp(*string,"W")  == 0   ) {
					strcpy( Mptr->TS_MOVMNT, *string );
					(*NDEX)++;
					(*NDEX)++;
					string++;
					return TRUE;
				}
			}
			else
				return TRUE;

		}
		else
			return FALSE;

	}
	return FALSE;

}

MDSP_BOOL isTornadicActivity( char **string, Decoded_METAR *Mptr,
                                     int *NDEX )
{
	int saveNdex,
	    TornadicTime;
	MDSP_BOOL Completion_flag;
	char *B_stringPtr,
	     *E_stringPtr;

	saveNdex = *NDEX;

	B_stringPtr = NULL;
	E_stringPtr = NULL;

	if (*string == NULL) {
		return FALSE;
	}

	if (!( strcmp(*string, "TORNADO")         == 0 ||
	        strcmp(*string, "TORNADOS")        == 0 ||
	        strcmp(*string, "TORNADOES")       == 0 ||
	        strcmp(*string, "WATERSPOUT")      == 0 ||
	        strcmp(*string, "WATERSPOUTS")     == 0 ||
	        strcmp(*string, "FUNNEL")     == 0  ) ) {
		return FALSE;
	}

	if (strcmp(*string, "FUNNEL") == 0) {
		++string;

		if (*string == NULL) {
			return FALSE;
		}

		if (!(strcmp(*string,"CLOUD") == 0 || strcmp(*string,"CLOUDS") == 0)) {
			(*NDEX)++;
			return FALSE;
		} else {
			strncpy(Mptr->TornadicType, "FUNNEL CLOUD", sizeof(Mptr->TornadicType));
		}
	} else {
		strncpy(Mptr->TornadicType, *string, sizeof(Mptr->TornadicType));
		(*NDEX)++;
		++string;
	}

	Completion_flag = FALSE;

	if (*string == NULL) {
		return FALSE;
	}

	while (!Completion_flag && *string != NULL) {

		/*       printf("isTornadicActivity:  current *string = %s\n",
				    *string);    */

		if (*(*string) =='B' || *(*string) == 'E') {
			if (*(*string) == 'B' ) {
				B_stringPtr = *string;
				E_stringPtr = strchr((*string)+1,'E');
			} else {
				B_stringPtr = strchr((*string)+1,'B');
				E_stringPtr = *string;
			}
			/*
						if (B_stringPtr != NULL) {
							printf("isTornadicActivity:  B_stringPtr = %x\n", B_stringPtr);
						} else {
							printf("isTornadicActivity:  B_stringPtr = NULL\n");
						}

						if (E_stringPtr != NULL) {
							printf("isTornadicActivity:  E_stringPtr = %x\n", E_stringPtr);
						} else {
							printf("isTornadicActivity:  E_stringPtr = NULL\n");
						}
			*/
			if (B_stringPtr != NULL && E_stringPtr == NULL) {
				if (nisdigit((*string)+1, strlen((*string)+1)) &&
				        strlen((*string)+1) <= 4 ) {
					TornadicTime = antoi((*string)+1, strlen((*string)+1));
					if (TornadicTime > 99) {
						Mptr->BTornadicHour = TornadicTime / 100;
						Mptr->BTornadicMinute = TornadicTime % 100;
						(*NDEX)++;
						++string;
					} else {
						Mptr->BTornadicHour = TornadicTime;
						(*NDEX)++;
						++string;
					}
				} else {
					(*NDEX)++;
					++string;
				}
			} else if (B_stringPtr == NULL && E_stringPtr != NULL) {
				if (nisdigit((*string) + 1, strlen((*string) + 1)) &&
				        strlen((*string) + 1) <= 4) {
					TornadicTime = antoi((*string) + 1, strlen((*string) + 1));
					if (TornadicTime > 99) {
						Mptr->ETornadicHour = TornadicTime / 100;
						Mptr->ETornadicMinute = TornadicTime % 100;
						(*NDEX)++;
						++string;
					} else {
						Mptr->ETornadicHour = TornadicTime;
						(*NDEX)++;
						++string;
					}
				} else {
					(*NDEX)++;
					++string;
				}
			} else {
				/*				printf("isTornadicActivity:  B_stringPtr != NULL"
									" and E_stringPtr != NULL\n");  */
				if (nisdigit((B_stringPtr+1),
				             (E_stringPtr - (B_stringPtr+1)))) {
					TornadicTime = antoi(( B_stringPtr + 1),
					                     (E_stringPtr - (B_stringPtr + 1)));

					if (TornadicTime > 99) {
						Mptr->BTornadicHour = TornadicTime / 100;
						Mptr->BTornadicMinute = TornadicTime % 100;
						(*NDEX)++;
						(++string);
					} else {
						Mptr->BTornadicHour = TornadicTime;
						(*NDEX)++;
						++string;
					}

					TornadicTime = antoi((E_stringPtr + 1), strlen(E_stringPtr + 1));

					if (TornadicTime > 99) {
						Mptr->ETornadicHour = TornadicTime / 100;
						Mptr->ETornadicMinute = TornadicTime % 100;
						(*NDEX)++;
						++string;
					} else {
						Mptr->ETornadicHour = TornadicTime;
						(*NDEX)++;
						++string;
					}
				} else {
					(*NDEX)++;
					++string;
				}
			}
		} else if (nisdigit(*string, strlen(*string))) {
			++string;

			if (*string == NULL) {
				return FALSE;
			}

			if  (strcmp(*string,"N")  == 0  ||
			        strcmp(*string,"NE") == 0  ||
			        strcmp(*string,"NW") == 0  ||
			        strcmp(*string,"S")  == 0  ||
			        strcmp(*string,"SE") == 0  ||
			        strcmp(*string,"SW") == 0  ||
			        strcmp(*string,"E")  == 0  ||
			        strcmp(*string,"W")  == 0   ) {
				--string;
				Mptr->TornadicDistance = antoi(*string, strlen(*string));
				(*NDEX)++;
				++string;
			} else {
				--string;

				if( saveNdex == *NDEX ) {
					return FALSE;
				} else {
					return TRUE;
				}
			}
		} else if (strcmp(*string,"DSNT")  == 0 ||
		           strcmp(*string,"VC")    == 0 ||
		           strcmp(*string,"VCY")   == 0 ) {

			if (strcmp(*string,"VCY") == 0 ||
			        strcmp(*string,"VC") == 0  ) {
				++string;

				if (*string == NULL) {
					return FALSE;
				}

				if (strcmp(*string,"STN") == 0) {
					strcpy(Mptr->TornadicLOC,"VC STN");
					(*NDEX)++;
					(*NDEX)++;
					++string;
				} else {
					strcpy(Mptr->TornadicLOC,"VC");
					(*NDEX)++;
				}
			} else {
				strcpy(Mptr->TornadicLOC,"DSNT");
				(*NDEX)++;
				++string;
			}
		} else if (strcmp(*string,"N")  == 0  ||
		           strcmp(*string,"NE") == 0  ||
		           strcmp(*string,"NW") == 0  ||
		           strcmp(*string,"S")  == 0  ||
		           strcmp(*string,"SE") == 0  ||
		           strcmp(*string,"SW") == 0  ||
		           strcmp(*string,"E")  == 0  ||
		           strcmp(*string,"W")  == 0   ) {
			strcpy(Mptr->TornadicDIR, *string);
			(*NDEX)++;
			++string;
		} else if (strcmp(*string, "MOV" ) == 0) {
			(*NDEX)++;
			++string;

			if (string == NULL) {
				return FALSE;
			}

			if (strcmp(*string, "N")  == 0  ||
			        strcmp(*string, "S")  == 0  ||
			        strcmp(*string, "E")  == 0  ||
			        strcmp(*string, "W")  == 0  ||
			        strcmp(*string, "NE")  == 0 ||
			        strcmp(*string, "NW")  == 0 ||
			        strcmp(*string, "SE")  == 0 ||
			        strcmp(*string, "SW")  == 0) {
				strcpy( Mptr->TornadicMovDir, *string );
				(*NDEX)++;
				++string;
			}
		} else {
			Completion_flag = TRUE;
		}
	}

	if (saveNdex == *NDEX) {
		return FALSE;
	} else {
		return TRUE;
	}
}

MDSP_BOOL isA0indicator( char *indicator, Decoded_METAR *Mptr,
                                int *NDEX )
{
	char *autoFlag[] = {"A01", "A01A", "A02", "A02A", "AOA",
	                    "A0A", "AO1", "AO1A", "AO2", "AO2A", NULL
	                   };
	int i;

	/* COMPARE THE INPUT CHARACTER STRING WITH */
	/* VALID AUTOMATED STATION CODE TYPE.  IF  */
	/* A MATCH IS FOUND, RETURN TRUE.  OTHER-  */
	/*           WISE, RETURN FALSE            */

	if( indicator == NULL )
		return FALSE;

	i = 0;

	while( autoFlag[ i ] != NULL )
	{
		if( strcmp( indicator, autoFlag[ i ]) == 0 )
		{
			(*NDEX)++;
			strcpy(Mptr->autoIndicator, indicator);
			return TRUE;
		}
		i++;
	}

	return FALSE;
}

MDSP_BOOL isPeakWind( char **string, Decoded_METAR *Mptr,
                             int *NDEX )
{
	char buf[ 32 ];
	char *slash;
	int timeAsInt;
	MDSP_BOOL PK_WND_FLAG;

	/* IF THE CURRENT AND NEXT GROUPS ARE "PK WND", THEN  */
	/* DETERMINE WHETHER OR NOT THE GROUP THAT FOLLOWS IS */
	/* A VALID PK WND GROUP.  IF IT IS, THEN DECODE THE   */
	/* GROUP AND RETURN TRUE.  OTHERWISE, RETURN FALSE.   */

	PK_WND_FLAG = TRUE;

	if( *string == NULL )
		return FALSE;


	if( !(strcmp(*string,"PK") == 0 ||
	        strcmp(*string,"PKWND") == 0 ) )
		return FALSE;
	else
		(++string);

	if( *string == NULL )
		return FALSE;

	if( strcmp(*string,"WND") == 0 )
		(++string);
	else
		PK_WND_FLAG = FALSE;

	if( *string == NULL )
		return FALSE;

	if( (slash = strchr(*string,'/')) == NULL ) {
		/* INVALID PEAK WIND. BUMP PAST */
		/* PK AND WND GROUP AND RETURN  */
		/*             FALSE.           */
		(*NDEX)++;

		if( PK_WND_FLAG )
			(*NDEX)++;

		return FALSE;
	}
	// dddff(f)/(hh)mm
	else if( strlen(*string) >= 8 && strlen(*string) <= 11 &&
	         nisdigit(slash+1,strlen(slash+1)) &&
	         nisdigit(*string, (slash - *string)) &&
	         (slash - *string) >= 5 &&
	         (slash - *string) <= 6 )
	{
		memset( buf, '\0', 4);
		strncpy( buf, *string, 3 );
		Mptr->PKWND_dir = atoi( buf );

		memset( buf, '\0', 4);
		strncpy( buf, *string+3, slash-(*string+3) );
		Mptr->PKWND_speed =  atoi( buf );

		memset( buf, '\0', 5);
		strcpy( buf, slash+1 );
		timeAsInt         =  atoi( buf );

		if( strlen(buf) > 2 )
		{
			Mptr->PKWND_hour = timeAsInt/100;
			Mptr->PKWND_minute = timeAsInt % 100;
		}
		else if (timeAsInt < 60) {
			Mptr->PKWND_minute = timeAsInt;
		}
		/* VALID PEAK WIND FOUND.  BUMP */
		/* PAST PK, WND, AND PEAK WIND  */
		/* GROUPS AND RETURN TRUE.      */
		(*NDEX)++;
		(*NDEX)++;

		if( PK_WND_FLAG )
			(*NDEX)++;

		return TRUE;
	}
	else
		return FALSE;
}

MDSP_BOOL isWindShift( char **string, Decoded_METAR *Mptr,
                              int *NDEX)
{
	int temp;

	/* IF THE CURRENT GROUP IS "WSHFT", THEN DETERMINE  */
	/* WHETHER OR NOT THE GROUP THAT FOLLOWS IS A VALID */
	/* WSHFT GROUP.  IF IT IS, THEN DECODE THE GROUP    */
	/* GROUP AND RETURN TRUE.  OTHERWISE, RETURN FALSE. */

	if( *string == NULL )
		return FALSE;

	if( strcmp( *string, "WSHFT" ) != 0 )
		return FALSE;
	else
		(++string);

	if( *string == NULL )
		return FALSE;

	if( nisdigit(*string,strlen(*string)) && strlen(*string) <= 4)
	{
		temp = atoi( *string );

		if( temp > 100 )
		{
			Mptr->WshfTime_hour = (atoi(*string))/100;
			Mptr->WshfTime_minute = (atoi(*string)) % 100;
		}
		else
			Mptr->WshfTime_minute = (atoi(*string)) % 100;

		(++string);

		if( *string == NULL )
			return FALSE;

		if( **string == '\0') {
			(*NDEX)++;
			(*NDEX)++;
			return TRUE;
		}
		else if( strcmp( *string, "FROPA") == 0 )
		{
			Mptr->Wshft_FROPA = TRUE;
			/* VALID WIND SHIFT FOUND. BUMP */
			/* PAST WSHFT, WSHFT GROUP, AND */
			/* FROPA GROUPS AND RETURN TRUE.*/
			(*NDEX)++;
			(*NDEX)++;
			(*NDEX)++;
			return TRUE;
		}
		else {
			/* VALID WIND SHIFT FOUND. BUMP */
			/* PAST WSHFT AND WSHFT GROUP   */
			/*       AND RETURN TRUE.       */
			(*NDEX)++;
			(*NDEX)++;
			return TRUE;
		}
	}
	else {
		/* INVALID WIND SHIFT FOUND. BUMP */
		/* PAST WSHFT AND RETURN FALSE.   */
		(*NDEX)++;
		return FALSE;
	}
}

MDSP_BOOL isLightningFrequency( char **string, Decoded_METAR *Mptr, int *NDEX )
{
	MDSP_BOOL LTG_TYPE_FLAG,
	          LTG_LOC_FLAG,
	          LTG_DIR_FLAG,
	          TYPE_NOT_FOUND;
	char *temp;

	// if the current group is "LTG", then determine
	// whether or not the previous group as well as
	// groups that follow are valid lightning report
	// parameters.  if they are, then decode the
	// groups and return true.  otherwise, return
	//                   false.

	// printf("isLightningFrequency:  current *string = %s, NDEX = %d\n", *string, *NDEX);

	if( *string == NULL )
		return FALSE;

	if( strcmp(*string,"VCTS") == 0 ) {
		Mptr->LightningVCTS = TRUE;
		(++string);
		(*NDEX)++;
		return TRUE;
	}

	if( *string == NULL )
		return FALSE;

	if( strncmp( *string, "LTG", 3 ) != 0 ) {
		return FALSE;
	}
	else {

		if( *string == NULL )
			return FALSE;

		(--string);


		// check for lightning frequency
		if( strcmp( *string, "OCNL" ) == 0 ) {
			Mptr->OCNL_LTG = TRUE;
		}
		else if( strcmp( *string, "FRQ" ) == 0 ) {
			Mptr->FRQ_LTG = TRUE;
		}
		else if( strcmp( *string, "CONS" ) == 0 ) {
			Mptr->CNS_LTG = TRUE;
		}


		(++string);

		if( *string == NULL )
			return FALSE;


		if( strcmp( *string, "LTG") == 0 ) {
			(++string);

			if( *string == NULL )
				return FALSE;

			(*NDEX)++;

			LTG_LOC_FLAG = FALSE;

			// check for lightning location
			if( strcmp( *string, "DSNT" ) == 0 ) {
				Mptr->DSNT_LTG = TRUE;
				LTG_LOC_FLAG = TRUE;
			}
			else if( strcmp( *string, "AP" ) == 0 ) {
				Mptr->AP_LTG = TRUE;
				LTG_LOC_FLAG = TRUE;
			}
			else if( strcmp( *string, "VCY" ) == 0 ||
			         strcmp( *string, "VC"  ) == 0 ) {
				Mptr->VcyStn_LTG = TRUE;
				LTG_LOC_FLAG = TRUE;
			}
			else if( strcmp( *string, "OVHD" ) == 0 ||
			         strcmp( *string, "OHD"  ) == 0 ) {
				Mptr->OVHD_LTG = TRUE;
				LTG_LOC_FLAG = TRUE;
			}

			if( LTG_LOC_FLAG )
				(++string);

			if( *string == NULL ) {
				if( LTG_LOC_FLAG )
					(*NDEX)++;
				return TRUE;
			}

			LTG_DIR_FLAG = FALSE;
			// check for lightning direction
			if( strcmp( *string, "N" ) == 0 ||
			        strcmp( *string, "NE" ) == 0 ||
			        strcmp( *string, "NW" ) == 0 ||
			        strcmp( *string, "S" ) == 0 ||
			        strcmp( *string, "SE" ) == 0 ||
			        strcmp( *string, "SW" ) == 0 ||
			        strcmp( *string, "E" ) == 0 ||
			        strcmp( *string, "W" ) == 0    ) {
				strcpy( Mptr->LTG_DIR, *string);
				LTG_DIR_FLAG = TRUE;
			}


			if( LTG_LOC_FLAG )
				(*NDEX)++;
			if( LTG_DIR_FLAG )
				(*NDEX)++;

			return TRUE;
		}
		else {

			LTG_TYPE_FLAG = FALSE;
			// check for lightning type
			TYPE_NOT_FOUND = FALSE;
			temp = (*string) + 3;
			while( *temp != '\0' && !TYPE_NOT_FOUND ) {
				if( strncmp( temp, "CG", 2 ) == 0 ) {
					Mptr->CG_LTG = TRUE;
					LTG_TYPE_FLAG = TRUE;
					temp++;
					temp++;
				}
				else if( strncmp( temp, "IC", 2 ) == 0 ) {
					Mptr->IC_LTG = TRUE;
					LTG_TYPE_FLAG = TRUE;
					temp++;
					temp++;
				}
				else if( strncmp( temp, "CC", 2 ) == 0 ) {
					Mptr->CC_LTG = TRUE;
					LTG_TYPE_FLAG = TRUE;
					temp++;
					temp++;
				}
				else if( strncmp( temp, "CA", 2 ) == 0 ) {
					Mptr->CA_LTG = TRUE;
					LTG_TYPE_FLAG = TRUE;
					temp++;
					temp++;
				}
				else
					TYPE_NOT_FOUND = TRUE;
			}

			(++string);

			if( *string == NULL ) {
				(*NDEX)++;
				return TRUE;
			}
			/*       else
			            (*NDEX)++;   TURNED OFF 07-24-97  */

			LTG_LOC_FLAG = FALSE;
			// check for lightning location
			if( strcmp( *string, "DSNT" ) == 0 ) {
				Mptr->DSNT_LTG = TRUE;
				LTG_LOC_FLAG = TRUE;
			}
			else if( strcmp( *string, "AP" ) == 0 ) {
				Mptr->AP_LTG = TRUE;
				LTG_LOC_FLAG = TRUE;
			}
			else if( strcmp( *string, "VCY" ) == 0 ||
			         strcmp( *string, "VC"  ) == 0 ) {
				Mptr->VcyStn_LTG = TRUE;
				LTG_LOC_FLAG = TRUE;
			}
			else if( strcmp( *string, "OVHD" ) == 0 ) {
				Mptr->OVHD_LTG = TRUE;
				LTG_LOC_FLAG = TRUE;
			}

			if( LTG_LOC_FLAG )
				(++string);

			if( *string == NULL ) {
				if( LTG_LOC_FLAG )
					(*NDEX)++;
				if( LTG_TYPE_FLAG )
					(*NDEX)++;
				return TRUE;
			}

			LTG_DIR_FLAG = FALSE;
			// check for lightning direction
			if( strcmp( *string, "N" ) == 0 ||
			        strcmp( *string, "NE" ) == 0 ||
			        strcmp( *string, "NW" ) == 0 ||
			        strcmp( *string, "S" ) == 0 ||
			        strcmp( *string, "SE" ) == 0 ||
			        strcmp( *string, "SW" ) == 0 ||
			        strcmp( *string, "E" ) == 0 ||
			        strcmp( *string, "W" ) == 0    ) {
				strcpy( Mptr->LTG_DIR, *string);
				LTG_DIR_FLAG = TRUE;
			}


			if( LTG_TYPE_FLAG )
				(*NDEX)++;
			if( LTG_LOC_FLAG )
				(*NDEX)++;
			if( LTG_DIR_FLAG )
				(*NDEX)++;

			if( !(LTG_TYPE_FLAG) &&     /*  Added on 02/23/98 to prevent */
			        !(LTG_LOC_FLAG)  &&     /*  infinite looping when 'LTG'  */
			        !(LTG_DIR_FLAG)    )    /*  is present in the input, but */
				(*NDEX)++;               /*  all other related parameters */
			/*  are missing or invalid       */
			return TRUE;
		}
	}
}
