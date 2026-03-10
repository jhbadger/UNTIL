/*
|	FILE: SHOWBIT.C
|
|	This module contains copyrighted source code for part the 
|	UNTIL Language Materials. 
|
|	Written by:
|		Richard Secrist
|		Copyright 1994
|		All Rights Reserved
|
|	Right to use, copy, and modify this code is granted
|	for personal non-commercial use, provided that this
|	copyright disclosure remains on ALL copies. Any other
|	use, reproduction, or distribution is covered in the
|	License Agreement with the Until Language Maeterials
|	documentation and in the file LICENSE.TXT.
*/

#include "compiler.h"

#include <stdio.h>
#include "until.h"
#include "functs.h"

/***********************+----------+
|			| showbits |
|			+----------+
|	Used by dot_bin.
*/
void showbits(long value)
{
	long i;
	unsigned long window = 2147483648;
	for (i=1; i<=32; i++){
	  putchar((value & window) ? '1':'0');
	  window >>= 1;
	}
}
/***********************+---------+
|			| dot_bin |
|			+---------+
|	.bin	( n --- )
*/
void dot_bin()
{
	long value;
	void showbits(long value);

	value = popsp();
	showbits(value);
}
