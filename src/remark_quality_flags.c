#include <string.h>

#include "remark_quality_flags.h"

MDSP_BOOL isNOSPECI (char *string, Decoded_METAR *Mptr, int *NDEX)
{
	if (string == NULL) {
		return FALSE;
	}

	if (strcmp(string,"NOSPECI") != 0) {
		return FALSE;
	} else {
		Mptr->NOSPECI = TRUE;
		(*NDEX)++;
		return TRUE;
	}
}

MDSP_BOOL isLAST( char *string, Decoded_METAR *Mptr, int *NDEX )
{
	if (string == NULL) {
		return FALSE;
	}

	if (strcmp(string,"LAST") != 0) {
		return FALSE;
	} else {
		Mptr->LAST = TRUE;
		(*NDEX)++;
		return TRUE;
	}
}

MDSP_BOOL isPWINO( char *string, Decoded_METAR *Mptr, int *NDEX)
{

	if( string == NULL )
		return FALSE;


	if( strcmp(string, "PWINO") != 0 )
		return FALSE;
	else {
		Mptr->PWINO = TRUE;
		(*NDEX)++;
		return TRUE;
	}

}

MDSP_BOOL isPNO( char *string, Decoded_METAR *Mptr, int *NDEX)
{


	if( string == NULL )
		return FALSE;

	if( strcmp(string, "PNO") != 0 )
		return FALSE;
	else {
		Mptr->PNO = TRUE;
		(*NDEX)++;
		return TRUE;
	}

}

MDSP_BOOL isRVRNO( char *string, Decoded_METAR *Mptr, int *NDEX)
{

	if( string == NULL )
		return FALSE;

	if( strcmp(string, "RVRNO") != 0 )
		return FALSE;
	else {
		Mptr->RVRNO = TRUE;
		(*NDEX)++;
		return TRUE;
	}

}

MDSP_BOOL isCHINO( char **string, Decoded_METAR *Mptr, int *NDEX)
{


	if( *string == NULL )
		return FALSE;

	if( strcmp(*string, "CHINO") != 0 )
		return FALSE;
	else
		string++;

	if( *string == NULL )
		return FALSE;

	if( strlen(*string) <= 2 ) {
		(*NDEX)++;
		return FALSE;
	}
	else {
		if( strncmp( *string, "RY", 2 ) == 0 &&
		        nisdigit(*string+2,strlen(*string+2)) ) {
			Mptr->CHINO = TRUE;
			strcpy(Mptr->CHINO_LOC, *string);
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

MDSP_BOOL isVISNO( char **string, Decoded_METAR *Mptr, int *NDEX)
{

	if( *string == NULL )
		return FALSE;

	if( strcmp(*string, "VISNO") != 0 )
		return FALSE;
	else
		string++;

	if( *string == NULL )
		return FALSE;

	if( strlen(*string) <= 2 ) {
		(*NDEX)++;
		return FALSE;
	}
	else {
		if( strncmp( *string, "RY", 2 ) == 0 &&
		        nisdigit(*string+2,strlen(*string+2))) {
			Mptr->VISNO = TRUE;
			strcpy(Mptr->VISNO_LOC, *string);
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

MDSP_BOOL isFZRANO( char *string, Decoded_METAR *Mptr, int *NDEX)
{


	if( string == NULL )
		return FALSE;

	if( strcmp(string, "FZRANO") != 0 )
		return FALSE;
	else {
		Mptr->FZRANO = TRUE;
		(*NDEX)++;
		return TRUE;
	}

}

MDSP_BOOL isTSNO( char *string, Decoded_METAR *Mptr, int *NDEX)
{

	if( string == NULL )
		return FALSE;

	if( strcmp(string, "TSNO") != 0 )
		return FALSE;
	else {
		Mptr->TSNO = TRUE;
		(*NDEX)++;
		return TRUE;
	}

}

MDSP_BOOL isDollarSign( char *indicator, Decoded_METAR *Mptr,
                       int *NDEX )
{

	if( indicator == NULL )
		return FALSE;

	if( strcmp(indicator,"$") != 0 )
		return FALSE;
	else
	{
		(*NDEX)++;
		Mptr->DollarSign = TRUE;
		return TRUE;
	}
}
