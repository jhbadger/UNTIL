/*
|	FILE: UNTIL.C
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
*/

#include "compiler.h"

#include <stdio.h>
#include <string.h>

#ifdef ANSIC
#include <process.h>
#include <conio.h>
#endif

#include "until.h"
#include "functs.h"

/***********************+------+
|			| main |
|			+------+
*/
main(int argc, char **argv)
{
/*
	clrscr();
*/
	startup(argc,argv);
}
/***********************+-------+
|			| Until |
|			+-------+
*/
void XX_Until(char *word)
{
	long len;
		/*
		| Boot to application word
		*/
	len = strlen(word);
	if(len){
		QUIT   = FALSE;
		strcpy((pad + 1),word);
		*pad   = len;
		exec_word();
	}
}
