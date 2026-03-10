/*
|       FILE: SHARP.C
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
| DATE CHANGED:
| 04/23/95	Broke this module out from MATH.C. [nes]
*/

#include "compiler.h"

#include <stdio.h>
#ifndef NO_STDLIB
#include <stdlib.h>
#endif 

#include <ctype.h>
#include <math.h>

#include "until.h"
#include "functs.h"

#ifdef SHARP_WORDS
/***********************+------------+
|                       | less_sharp |
|                       +------------+
|       <#      ( d --- d )
*/
long signed_number;
void less_sharp()
{
	signed_number = popsp();	/* read dummy 0 	    */
	signed_number = popsp();
	pushsp(abs(signed_number));	/* convert numb to unsigned */
	pushsp((long)0);		/* add dummy 0 for a double */

	hld = 80;
}
/***********************+-------+
|                       | sharp |
|                       +-------+
|       #       ( d --- d )
*/
void sharp()
{
	unsigned long digit;
	unsigned long quot;
	unsigned long remaind;

	digit   = popsp();
	digit   = popsp();
	if(!digit){
/*
#ifndef VECTORED_C_IO
                fprintf(stderr,"Divide by 0 in SHARP\n");
#else
                sprintf(tstring,"Divide by 0 in SHARP\n");
		stderr_vect(tstring);
#endif
                abort_F();
*/
		pushsp((long)0);
		pushsp((long)0);
		pushsp((long)'0');	/* for hold() */
		hold();
		return;
	}
/*
| look at using um/mod here to get unsigned division
*/
	quot    = digit / BASE;
	remaind = digit % BASE;
	if(remaind > 9){
		remaind += 7;
	}
	remaind += 0x30;
	pushsp(quot);
	pushsp((long)0);
	pushsp(remaind);
	hold();
}
/***********************+---------------+
|                       | sharp_greater |
|                       +---------------+
|
*/
void sharp_greater()
{
	drop();
	drop();
	pushsp((long)(pad + hld + 1));
	pushsp(80 - hld);
}
/***********************+---------+
|                       | sharp_s |
|                       +---------+
|       Convert a number to a string. Use pad as a work area. It must
|       be called between <# and #>.
|
| NOTE: The number formatting words assume single values, I am not
|       sure if the over over is necessary here...
*/
void sharp_s()
{
	long status;

	do{
		sharp();
		over();
		over();
		or();
		zero_equal();
		status = popsp();
	}while(!status);
}
/***********************+------+
|                       | hold |
|                       +------+
|
*/
void hold()
{
	long digit;

	digit          = popsp();
	*(pad + hld--) = digit;
}
/***********************+------+
|                       | sign |
|                       +------+
|
*/
void sign()
{
/*
	long sign_digit;

	sign_digit = popsp();
*/
	popsp();
		/*
		| use the signed number that was extracted by <#
		| Seems like it should work...
		*/
	if(signed_number < 0){
		pushsp((long)'-');
		hold();
	}
}

#endif
