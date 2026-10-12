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

#include "local.h"    /* standard header file */
 
/********************************************************************/
/*                                                                  */
/*  Title:          charcmp                                         */
/*  Organization:   W/OSO242 - GRAPHICS AND DISPLAY SECTION         */
/*  Date:           12 Dec 1995                                     */
/*  Programmer:     CINDY L. CHONG                                  */
/*  Language:       C/370                                           */
/*                                                                  */
/*  Abstract:       This function will compare each character in the */
/*                  string match with each character in the pattern */
/*                  which is made up of characters.  The str can    */
/*                  be longer than the pattern.                     */
/*                                                                  */
/*  External Functions Called:                                      */
/*                  None.                                           */
/*                                                                  */
/*  Input:          str is a pointer to char                        */
/*                  pattern is a pointer to char                    */
/*                                                                  */
/*  Output:         Return true if str matches pattern,             */
/*                  otherwise, return false                         */
/*                                                                  */
/*  Modification History:                                           */
/*                  None.                                           */
/*                                                                  */
/********************************************************************/
MDSP_BOOL charcmp(char *str, char *pattern)
{
    /* Validate input pointers to prevent null dereferences */
    if (str == NULL || pattern == NULL)
    {
        return FALSE;
    }

   /* Loop while str and pattern is not equal to null, then  */
   /* inscreases str and pattern by one                     */
 
   for (; *pattern != '\0'; pattern++)
   {
      if (*str == '\0')
         return FALSE;
 
      /* If pattern match str, then increase str and jump out the */
      /* case and read next char of the str and pattern           */
 
      if ( isspace((unsigned char)*pattern) )
         pattern = nxtalpha(pattern);
 
      switch( *pattern )
      {
         case 'c':
            if ( !isalnum((unsigned char)*str++) )
            {
               return FALSE;
            }
            break;
 
         case 'a':
            if ( !isalpha((unsigned char)*str) )
            {
               return FALSE;
            }
            str++;
            break;
 
         case 'n':
            if ( !iscntrl((unsigned char)*str++) )
            {
               return FALSE;
            }
            break;
 
         case 'd':
            if ( !isdigit((unsigned char)*str) )
            {
               return FALSE;
            }
            str++;
            break;
 
         case 'g':
            if ( !isgraph((unsigned char)*str++) )
            {
               return FALSE;
            }
            break;
 
         case 'i':
            if ( !islower((unsigned char)*str++) )
            {
               return FALSE;
            }
            break;
 
         case 'p':
            if ( !isprint((unsigned char)*str++) )
            {
               return FALSE;
            }
            break;
 
         case 't':
            if ( !ispunct((unsigned char)*str++) )
            {
               return FALSE;
            }
            break;
 
         case 'w':
            if ( !isspace((unsigned char)*str++) )
            {
               return FALSE;
            }
            break;
 
         case 'u':
            if ( !isupper((unsigned char)*str++) )
            {
               return FALSE;
            }
            break;
 
         case 's':
            if (*str++ != ' ')
            {
               return FALSE;
            }
            break;
 
         case 'm':
            if ( !isspace((unsigned char)*str) )
            {
               return FALSE;
            }
            else
            {
               while ( isspace((unsigned char)*str) )
                  str++;
            }
            break;
 
         case '\'':
             pattern++;
         
             if (*pattern == '\0')
             {
                 return FALSE;
             }
         
             if (*pattern != *str)
             {
                 return FALSE;
             }
         
             str++;
             break;
 
         default:
            return FALSE;
 
      } /* end switch */
 
   } /* end for */
 
   return (TRUE);
}
