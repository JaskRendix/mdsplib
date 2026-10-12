#include <string.h>
#include <stdlib.h>

#include "remark_thermal_pressure.h"

MDSP_BOOL isPRESFR( char *string, Decoded_METAR *Mptr, int *NDEX)
{

	if( string == NULL )
		return FALSE;

	if( strcmp(string, "PRESFR") != 0 )
		return FALSE;
	else {
		Mptr->PRESFR = TRUE;
		(*NDEX)++;
		return TRUE;
	}

}

MDSP_BOOL isPRESRR( char *string, Decoded_METAR *Mptr, int *NDEX)
{

	if( string == NULL )
		return FALSE;

	if( strcmp(string, "PRESRR") != 0 )
		return FALSE;
	else {
		Mptr->PRESRR = TRUE;
		(*NDEX)++;
		return TRUE;
	}

}

MDSP_BOOL isSLP( char **token, Decoded_METAR *Mptr, int *NDEX )
{
	int pressure;

	if( *token == NULL )
		return FALSE;

	if( strcmp(*token, "SLPNO") == 0 ) {
		Mptr->SLPNO = TRUE;
		(*NDEX)++;
		return TRUE;
	}


	if( strncmp(*token, "SLP", 3) != 0 )
		return FALSE;
	else
	{
		if( strncmp(*token, "SLP", 3) == 0 &&
		        strcmp(*token,"SLP") != 0 )
		{
			if( nisdigit( *token+3, 3) )
			{
				pressure = atoi(*token+3);

				if(pressure >= 550 )
					Mptr->SLP = ((float) pressure)/10. + 900.;
				else
					Mptr->SLP = ((float) pressure)/10. + 1000.;
				(*NDEX)++;
				return TRUE;
			}
			else
				return FALSE;
		}
		else
		{
			(++token);

			if( *token == NULL )
				return FALSE;

			if( nisdigit( *token, 3) )
			{
				pressure = atoi(*token);

				if(pressure >= 550 )
					Mptr->SLP = ((float) pressure)/10. + 900.;
				else
					Mptr->SLP = ((float) pressure)/10. + 1000.;

				(*NDEX)++;
				(*NDEX)++;
				return TRUE;
			}
			else
				return FALSE;
		}

	}

}

MDSP_BOOL isTTdTenths( char *token, Decoded_METAR *Mptr, int *NDEX)
{
	MDSP_BOOL returnFlag = FALSE;
	float sign;

	if( token == NULL )
		return FALSE;

	if( *token != 'T' )
		return FALSE;
	else if( !(strlen(token) == 5 || strlen(token) == 9) )
		return FALSE;
	else
	{
		if( (*(token+1) == '0' || *(token+1) == '1') &&
		        nisdigit(token+2,3) )
		{
			if( *(token+1) == '0' )
				sign = 0.1;
			else
				sign = -0.1;

			Mptr->Temp_2_tenths = sign * ((float) antoi(token+2,3));
			returnFlag = TRUE;
		}
		else
			return FALSE;

		if( (*(token+5) == '0' || *(token+5) == '1') &&
		        nisdigit(token+6,3) )
		{
			if( *(token+5) == '0' )
				sign = 0.1;
			else
				sign = -0.1;

			Mptr->DP_Temp_2_tenths = sign * ((float) atoi(token+6));
			(*NDEX)++;
			return TRUE;

		}
		else
		{
			if( returnFlag )
			{
				(*NDEX)++;
				return TRUE;
			}
			else
				return FALSE;
		}
	}
}

MDSP_BOOL isMaxTemperature(char *string, Decoded_METAR *Mptr, int *NDEX)
{
	char buf[ 32 ];

	if( string == NULL )
		return FALSE;

	if(strlen(string) != 5 )
		return FALSE;
	else if(*string == '1' && (*(string+1) == '0' ||
	                           *(string+1) == '1' ||
	                           *(string+1) == '/'   ) &&
	        (nisdigit((string+2),3) ||
	         strncmp(string+2,"///",3) == 0) )
	{
		if(nisdigit(string+2,3))
		{
			memset(buf, '\0', sizeof(buf));
			strncpy(buf,string+2,3);
			Mptr->maxtemp = ( (float) atoi(buf))/10.;

			if( *(string+1) == '1' )
				Mptr->maxtemp *= (-1.0);

			(*NDEX)++;
			return TRUE;
		}
		else
		{
			Mptr->maxtemp = (float) MAXINT;
			(*NDEX)++;
			return TRUE;
		}
	}
	else
		return FALSE;

}

MDSP_BOOL isMinTemperature(char *string, Decoded_METAR *Mptr, int *NDEX)
{
	char buf[ 32 ];

	if( string == NULL )
		return FALSE;

	if(strlen(string) != 5 )
		return FALSE;
	else if(*string == '2' && (*(string+1) == '0' ||
	                           *(string+1) == '1' ||
	                           *(string+1) == '/'   ) &&
	        (nisdigit((string+2),3) ||
	         strncmp(string+2,"///",3) == 0) )
	{
		if(nisdigit(string+2,3))
		{
			memset(buf,'\0', sizeof(buf));
			strncpy(buf,string+2,3);
			Mptr->mintemp = ( (float) atoi(buf) )/10.;

			if( *(string+1) == '1' )
				Mptr->mintemp *= (-1.0);
			(*NDEX)++;
			return TRUE;
		}
		else
		{
			Mptr->mintemp = (float) MAXINT;
			(*NDEX)++;
			return TRUE;
		}
	}
	else
		return FALSE;

}

MDSP_BOOL isT24MaxMinTemp( char *string, Decoded_METAR *Mptr,
                           int *NDEX )
{
	char buf[ 32 ];

	if( string == NULL )
		return FALSE;

	if( strlen(string) != 9 )
		return FALSE;
	else if( (*string == '4' && (*(string+1) == '0' ||
	                             *(string+1) == '1' ||
	                             *(string+1) == '/')     &&
	          (nisdigit((string+2),3) || strncmp(string+2,"///",3)))
	         &&
	         ((*(string+5) == '0' || *(string+5) == '1' ||
	           *(string+5) == '/') &&
	          (nisdigit((string+6),3) ||
	           strncmp(string+6,"///",3) == 0 )) )
	{
		if(nisdigit(string+1,4) && (*(string+1) == '0' ||
		                            *(string+1) == '1')   )
		{
			memset(buf, '\0', sizeof(buf));
			strncpy(buf, string+2, 3);
			Mptr->max24temp = ( (float) atoi( buf ) )/10.;

			if( *(string+1) == '1' )
				Mptr->max24temp *= -1.;
		}
		else
			Mptr->max24temp = (float) MAXINT;


		if(nisdigit(string+5,4) && (*(string+5) == '0' ||
		                            *(string+5) == '1' )  )
		{
			memset(buf, '\0', sizeof(buf));
			strncpy(buf, string+6, 3);
			Mptr->min24temp = ( (float) atoi(buf) )/10.;

			if( *(string+5) == '1' )
				Mptr->min24temp *= -1.;
		}
		else
			Mptr->min24temp = (float) MAXINT;

		(*NDEX)++;
		return TRUE;

	}
	else
		return FALSE;
}

MDSP_BOOL isPtendency(char *string, Decoded_METAR *Mptr, int *NDEX)
{
	char buf[ 32 ];

	if( string == NULL )
		return FALSE;

	if(strlen(string) != 5)
		return FALSE;
	else if(*string == '5' && ('0' <= *(string+1) && *(string+1) <= '8') &&
	        (nisdigit(string+2,3) || strncmp(string+2,"///",3)
	         == 0) )
	{
		if( !(nisdigit(string+2,3)) )
		{
			memset(buf, '\0', sizeof(buf));
			strncpy(buf,(string+1),1);
			Mptr->char_prestndcy = atoi(buf);
			(*NDEX)++;
			return TRUE;
		}
		else
		{
			memset(buf, '\0', sizeof(buf));
			strncpy(buf,(string+1),1);
			Mptr->char_prestndcy = atoi(buf);

			Mptr->prestndcy = ((float) atoi(string+2)) * 0.1;

			(*NDEX)++;
			return TRUE;
		}

	}
	else
		return FALSE;

}
