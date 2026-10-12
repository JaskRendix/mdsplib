#include <stdlib.h>
#include <string.h>

#include "remark_precip_snow.h"

float fracPart(const char *);

MDSP_BOOL isGR( char **string, Decoded_METAR *Mptr, int *NDEX)
{
	char *slash;

	if( *string == NULL )
		return FALSE;

	if( strcmp(*string, "GS") == 0 ) {
		Mptr->GR = TRUE;
		(*NDEX)++;
		return TRUE;
	}


	if( strcmp(*string, "GR") != 0 )
		return FALSE;
	else {
		(++string);

		if( *string == NULL )
			return FALSE;

		if( (slash = strchr( *string, '/' )) != NULL ) {
			if( strcmp( *string, "M1/4" ) == 0 ) {
				Mptr->GR_Size = 1./8.;
				Mptr->GR = TRUE;
				(*NDEX)++;
				(*NDEX)++;
				return TRUE;
			}
			else if( nisdigit( *string, (slash - *string) ) &&
			         nisdigit( slash+1, strlen(slash+1)) ) {
				Mptr->GR_Size = fracPart( *string );
				Mptr->GR = TRUE;
				(*NDEX)++;
				(*NDEX)++;
				return TRUE;
			}
			else {
				Mptr->GR = TRUE;
				(*NDEX)++;
				return TRUE;
			}
		}
		else if( nisdigit( *string, strlen(*string) ) ) {
			Mptr->GR_Size = antoi( *string, strlen(*string) );
			Mptr->GR = TRUE;

			(++string);

			if( *string == NULL )
				return FALSE;

			if( (slash = strchr( *string, '/' )) != NULL ) {
				if( nisdigit( *string, (slash - *string) ) &&
				        nisdigit( slash+1, strlen(slash+1)) ) {
					Mptr->GR_Size += fracPart( *string );
					(*NDEX)++;
					(*NDEX)++;
					(*NDEX)++;
					return TRUE;
				}
				else {
					(*NDEX)++;
					(*NDEX)++;
					return TRUE;
				}
			}
			else {
				(*NDEX)++;
				(*NDEX)++;
				return TRUE;
			}
		}
		else {
			Mptr->GR = TRUE;
			(*NDEX)++;
			return TRUE;
		}
	}
}

MDSP_BOOL isVIRGA( char **string, Decoded_METAR *Mptr, int *NDEX)
{

	if( *string == NULL )
		return FALSE;

	if( strcmp(*string, "VIRGA") != 0 )
		return FALSE;
	else {
		Mptr->VIRGA = TRUE;
		(*NDEX)++;

		(++string);

		if( *string == NULL )
			return TRUE;


		if( strcmp( *string, "N" ) == 0 ||
		        strcmp( *string, "S" ) == 0 ||
		        strcmp( *string, "E" ) == 0 ||
		        strcmp( *string, "W" ) == 0 ||
		        strcmp( *string, "NE" ) == 0 ||
		        strcmp( *string, "NW" ) == 0 ||
		        strcmp( *string, "SE" ) == 0 ||
		        strcmp( *string, "SW" ) == 0    ) {
			strcpy(Mptr->VIRGA_DIR, *string);
			(*NDEX)++;
		}
		return TRUE;
	}

}

MDSP_BOOL isSNINCR( char **string, Decoded_METAR *Mptr, int *NDEX )
{

	char *slash;

	if( *string == NULL )
		return FALSE;

	if( strcmp( *string, "SNINCR") != 0 )
		return FALSE;
	else {
		(++string);

		if( *string == NULL )
			return FALSE;


		if( (slash = strchr(*string,'/')) == NULL ) {
			(*NDEX)++;
			return FALSE;
		}
		else if( nisdigit (*string,(slash-*string)) &&
		         nisdigit(slash+1,strlen(slash+1)) ) {
			Mptr->SNINCR = antoi(*string,(slash-*string));
			Mptr->SNINCR_TotalDepth = antoi(slash+1,strlen(slash+1));
			(*NDEX)++;
			(*NDEX)++;
			return TRUE;
		}
		else {
			(*NDEX)++;
			return FALSE;
		}

	}

}

MDSP_BOOL isSnowDepth( char *token, Decoded_METAR *Mptr,
                              int *NDEX )
{

	if( token == NULL )
		return FALSE;

	if(strlen(token) != 5)
		return FALSE;

	if( *token == '4' &&
	        *(token+1) == '/'  &&
	        nisdigit( (token+2),3) )
	{
		strcpy(Mptr->snow_depth_group,token);
		Mptr->snow_depth = antoi(token+2,3);
		(*NDEX)++;
		return TRUE;
	}
	else
		return FALSE;
}

MDSP_BOOL isWaterEquivSnow( char *string,
                                   Decoded_METAR *Mptr,
                                   int *NDEX )
{

	if( string == NULL )
		return FALSE;

	if( strlen(string) != 6 )
		return FALSE;
	else if( !(nisdigit(string,6)) )
		return FALSE;
	else if( strncmp(string, "933", 3) != 0 )
		return FALSE;
	else {
		Mptr->WaterEquivSnow = ((float) atoi(string+3))/10.;
		(*NDEX)++;
		return TRUE;
	}

}

MDSP_BOOL isSunshineDuration( char *string, Decoded_METAR *Mptr,
                                     int *NDEX )
{

	if( string == NULL )
		return FALSE;

	if( strlen(string) != 5 )
		return FALSE;
	else if( strncmp(string, "98", 2) != 0 )
		return FALSE;
	else if(nisdigit(string+2,3)) {
		Mptr->SunshineDur = atoi(string+2);
		(*NDEX)++;
		return TRUE;
	}
	else if( strncmp(string+2, "///", 3) == 0 ) {
		Mptr->SunSensorOut = TRUE;
		(*NDEX)++;
		return TRUE;
	}
	else
		return FALSE;
}

MDSP_BOOL isHourlyPrecipitation( char **string, Decoded_METAR *Mptr,
                                        int *NDEX)
{


	if( *string == NULL )
		return FALSE;

	if( !(strcmp(*string, "P") == 0 || charcmp(*string, "'P'dddd") ||
	        charcmp(*string, "'P'ddd") ) )
		return FALSE;
	else if( strcmp(*string, "P") != 0 ) {
		if( nisdigit((*string+1), strlen(*string+1)) ) {
			Mptr->hourlyPrecip = ((float)
			                      atoi(*string+1)) * 0.01;
			(*NDEX)++;
			return TRUE;
		}
		else {
			(*NDEX)++;
			return FALSE;
		}
	}
	else {

		(++string);

		if( *string == NULL )
			return FALSE;


		if( nisdigit(*string,strlen(*string)) ) {
			Mptr->hourlyPrecip =  ((float)
			                       atoi(*string)) * 0.01;
			(*NDEX)++;
			(*NDEX)++;
			return TRUE;
		}
		else {
			(*NDEX)++;
			return FALSE;
		}
	}
}

MDSP_BOOL isP6Precipitation( char *string, Decoded_METAR *Mptr,
                                    int *NDEX )
{

	if( string == NULL )
		return FALSE;

	if( strlen(string) != 5 )
		return FALSE;


	if( charcmp(string,"'6'dddd") ||
	        charcmp(string,"'6''/''/''/''/'") ) {
		if( strcmp(string+1, "////") == 0 ) {
			Mptr->precip_amt = (float) MAXINT;
			Mptr->Indeterminant3_6HrPrecip = TRUE;
			(*NDEX)++;
			return TRUE;
		}
		else {
			Mptr->precip_amt = ((float) atoi(string+1)) / 100;
			(*NDEX)++;
			return TRUE;
		}
	}
	else
		return FALSE;

}

MDSP_BOOL isP24Precip( char *string, Decoded_METAR *Mptr,
                              int *NDEX )
{

	if( string == NULL )
		return FALSE;

	if( strlen(string) != 5 )
		return FALSE;

	if( charcmp(string,"'7'dddd") ||
	        charcmp(string,"'7''/''/''/''/'") ) {
		if( strcmp(string+1, "////") == 0 ) {
			Mptr->precip_24_amt = (float) MAXINT;
			Mptr->Indeterminant_24HrPrecip = TRUE;
			(*NDEX)++;
			return TRUE;
		}
		else {
			Mptr->precip_24_amt = ((float) atoi(string+1)) / 100.;
			(*NDEX)++;
			return TRUE;
		}
	}
	else
		return FALSE;

}
