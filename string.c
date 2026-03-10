/*
|	FILE: string.c
|
|	This module contains the bindings to C string functions. It
|	is not all inclusive. I am adding them as I need them...
|
|	This module contains copyrighted source code for part the 
|	UNTIL Language Materials. 
|
|	Written by:
|		Norman E. Smith, CDP
|		Copyright 1992, 1994
|		All Rights Reserved
|
|	Right to use, copy, and modify this code is granted
|	for personal non-commercial use, provided that this
|	copyright disclosure remains on ALL copies. Any other
|	use, reproduction, or distribution is covered in the
|	License Agreement with the Until Language Maeterials
|	documentation and in the file LICENSE.TXT.
|
|	DATE		Change
|	06/30/93	Added C_strcat, C_strcpy, C_strlen, etc. [nes]
|	06/05/94	Started the modification to be able to
|			turn on/off C string words leaving return
|			values. [nes]
|	01/16/95	Made cleanup changes suggested by Akira Kida,
|			akida@isd.hin.konica.co.jp. [nes]
*/
#include "compiler.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#ifndef NO_STDLIB
#include <stdlib.h>
#endif

#ifdef BCC
#pragma intrinsic
#endif

#include "until.h"
#include "functs.h"

/***********************+------------+
|			| str_return |
|			+------------+
|	( --- value ) strret
|
| This function returns the return value from the last string word
| call. The string return value is cleared.
*/
void str_return()
{
	pushsp((long)u_errno);
	u_errno = 0;
}
/***********************+----------------+
|			| set_str_return |
|			+----------------+
|	( tf --- ) set_string_return
|
| This function takes Truth from the stack and sets the global 
| flag, ??. This has the effect of turning on/off return values for 
| other C string words...
*/
void set_str_return()
{
	long truth;

	truth         = popsp();
	string_return = truth;
}
/***********************+--------------+
|			| set_str_type |
|			+--------------+
|	( tf --- ) set_str_type
|
| This function takes Truth from the stack and sets the global 
| flag, null_strings. This has the effect of switching string types
| from counted to null
*/
void set_str_type()
{
	long truth;

	truth      = popsp();
	quote_null = truth;
}
/********************************************************************
*********************************************************************
The following code is compiled only when the symbol C_STRINGS
is defined.
*********************************************************************
*********************************************************************/

#ifdef C_STRINGS

/***********************+-----------+
|			| C_strncmp |
|			+-----------+
|	( s1 s2 len --- flag ) strncmp
*/
void C_strncmp()
{
	char *string1;
	char *string2;
	long len;
	long result;

	len     = popsp();
	string2 = (char*) popsp();
	string1 = (char*) popsp();
	result  = strncmp(string1,string2,len);
	u_errno = result;
	pushsp(result);
}
/***********************+----------+
|			| C_strcmp |
|			+----------+
|	( 's1 's2 --- flag ) strcmp
*/
void C_strcmp()
{
	char *string1;
	char *string2;
	long result;

	string2 = (char*) popsp();
	string1 = (char*) popsp();
	result  = strcmp(string1,string2);
	u_errno = result;
	pushsp(result);
}
/***********************+----------+
|			| C_strcat |
|			+----------+
|	( 's1 's2 --- 's1 ) strcat
*/
void C_strcat()
{
	char *string1;
	char *string2;
	char *result;

	string2 = (char*) popsp();
	string1 = (char*) popsp();
	result  = strcat(string1,string2);
	u_errno = (long)result;
	if(string_return){ pushsp((long)result);  }
}
/***********************+-----------+
|			| C_strncat |
|			+-----------+
|	( 's1 's2 n --- 's1 ) strncat
*/
void C_strncat()
{
	long len;
	char *string1;
	char *string2;
	char *result;

	len     = popsp();
	string2 = (char*) popsp();
	string1 = (char*) popsp();
	result  = strncat(string1,string2,len);
	u_errno = (long)result;
	if(string_return){ pushsp((long)result);  }
}
/***********************+----------+
|			| C_strcpy |
|			+----------+
|	( 's1 's2 --- 's1 ) strcpy
*/
void C_strcpy()
{
	char *string1;
	char *string2;
	char *result;

	string2 = (char*) popsp();
	string1 = (char*) popsp();
	result  = strcpy(string1,string2);
	u_errno = (long)result;
	if(string_return){ pushsp((long)result);  }
}
/***********************+-----------+
|			| C_strncpy |
|			+-----------+
|	( 's1 's2 n --- 's1 ) strncpy
*/
void C_strncpy()
{
	long len;
	char *string1;
	char *string2;
	char *result;

	len     = popsp();
	string2 = (char*) popsp();
	string1 = (char*) popsp();
	result  = strncpy(string1,string2,len);
	u_errno = (long)result;
	if(string_return){ pushsp((long)result);  }
}
/***********************+--------+
|			| C_atol |
|			+--------+
|	( s --- n ) atol
*/
void C_atol()
{
	char *string;
	long result;

	string  = (char*) popsp();
	result  = atol(string);
	pushsp(result);
}
/***********************+----------+
|			| C_strlen |
|			+----------+
|	( s --- n ) strlen
*/
void C_strlen()
{
	char *string;
	long len;

	string  = (char*) popsp();
	len     = strlen(string);
	pushsp(len);
}
/***********************+--------+
|			| strupr |
|			+--------+
*/
#ifndef __TURBOC__
void strupr(char *string)
{
	while(*string){
		*string = toupper(*string);
		string++;
	}
}
#endif
/***********************+----------+
|			| C_strupr |
|			+----------+
|	( s --- n ) strupr
*/
void C_strupr()
{
	char *string;
/*
	long len;
*/
	string  = (char*) popsp();
	strupr(string);
}

#endif
