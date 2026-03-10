/*
|       FILE: CASE.C
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
|	Note: The word size is assumed to be 32-bits in several
|	      calls to malloc. This is not portable... Watch out!
|	      And do_allot too!!
|
|       Date written: 05/14/95
|
|	DATE 		CHANGE
|	05/14/95	Extracted portions from call.c to build CASE
|			code from. [nes]
*/

#include "compiler.h"

#include <stdio.h>
#include <string.h>

#include "until.h"
#include "functs.h"


/*
=============================================================
=== This module impelements a simple case statement. 
=== 
=============================================================
*/
/***********************+----------------+
|			| resolve_branch |
|			+----------------+
*/
void resolve_branch()
{
	long offset;
	struct DictHeader **branch;
	
	r_from();
	offset  = popsp();
	branch  = (struct DictHeader**) offset;
	offset  = (long) &pfa_list[pfa_offset] - offset;
	offset  = offset / sizeof(struct DictHeader*);
	*branch = (struct DictHeader*) offset;
	WA = 0;
}
/***********************+-------------------+
|			| resolve_of_branch |
|			+-------------------+
*/
void resolve_of_branch()
{
	long offset;
	struct DictHeader **branch;
	
	r_from();
	offset  = popsp();
	branch  = (struct DictHeader**) offset;
	offset  = (long) &pfa_list[pfa_offset] - offset;
	offset  = (offset / sizeof(struct DictHeader*)) + 1;
	*branch = (struct DictHeader*) offset;
	WA = 0;
}
/*
=============================================================
=== This section of code impelemnts the case statement in
=== Until. The words are:
===	case		( n --- )
===	of		( n --- )
===	endof
===	endcase
===
===
=============================================================*/

/***********************+--------------+
|			| Compile_case |
|			+--------------+
| Push a 0 on the return stack to mark beginning of case work data.
| There is no run-time action to perform.
*/
void Compile_case()
{
	In_case = TRUE; 
	pushrs((long)0);			/* mark start of case on RS */
	WA      = 0;				/* reset WA                 */
}
/***********************+---------+
|			| do_case |
|			+---------+
*/
void do_case()
{
}
/***********************+------------+
|			| Compile_of |
|			+------------+
| 1. Compile run-time function of do_of() into the dict.
| 2. Compile run-time function of zero_branch() into dict.
| 3. Put address to resolve offset to on return stack.
*/
void Compile_of()
{
	COMPILE_ADDR(OF_WA);			/* Set runtime addr for do_of */
	COMPILE_ADDR(ZERO_BRAN_WA);		/* Set runtime addr to branch to next of */
	pushrs((long)&pfa_list[pfa_offset++]);	/* Leave room for offset      */
	WA = 0;
}
/***********************+-------+
|			| do_of |
|			+-------+
| Run-time portion of the OF statement
|
|   ( value --- value|0 ) Value left when no match
*/
void do_of()
{
	long caseof;
	long value;
	
	caseof = popsp();		/* Get compiled value to test */
	value  = popsp();		/* Get run-time case value    */
		/*
		| If the case value doesn't match the compiled
		| of value, put the value back on the stack and
		| branch to the next OF
		*/
	if(value != caseof){
		pushsp(value);
		pushsp((long)0);
	}else{
		pushsp((long)TRUE);
	}
}
/***********************+---------------+
|			| Compile_endof |
|			+---------------+
*/
void Compile_endof()
{
	COMPILE_ADDR(BRANCH_WA);		/* Set up branch to ENDCASE */
	resolve_of_branch();			/* resolve 0branch from OF */
	pushrs((long) &pfa_list[pfa_offset++]);	/* Push address to resolve later */
}
/***********************+----------+
|			| do_endof |
|			+----------+
| separate runtime not needed
*/
#ifdef IGNORE
void do_endof()
{ }
#endif
/***********************+-----------------+
|			| Compile_endcase |
|			+-----------------+
*/
void Compile_endcase()
{
		/*
		| Resolve all ENDOF branches to the end
		*/
	COMPILE_ADDR(ENDCASE_WA);
	while(*(rstack + RS-1)){	/* while there is data... */
		resolve_branch();
	}
	In_case = FALSE;
}
/***********************+------------+
|			| do_endcase |
|			+------------+
*/
void do_endcase()
{
	drop();
}
