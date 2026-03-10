/*
|       FILE: MATH.C
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
|       10/7/92         Added code for number formatting words <#, #,
|                       #>, hold, sign, and #S. [nes]
|       11/20/92        Fixed base_number() to handle hex numbers
|                       correctly. [nes]
|       12/10/92        Initial coding of double number set. See
|                       comment in d+. Corrected um* and um* /mod
|                       problems. [nes]
|	10/04/94	Fixed bug in sharp() when value is 0. [nes]
|	01/16/95	Added <ctype.h> per suggestion of Akira Kida,
|			akida@isd.hin.konica.co.jp. [nes]
|	04/23/95	Moved <# ... #> code to SHARP.C. [nes]
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

/***********************+-------+
|                       | times |
|                       +-------+
|       *       ( n1 n2 --- product )
*/
void multiply()
{
#ifdef READABLE
	long product,mult1,mult2;

	mult2   = popsp();
	mult1   = popsp();
	product = mult1 * mult2;
	pushsp(product);
#else
	pushsp((popsp()) * (popsp()));
#endif
}
/***********************+--------+
|                       | divide |
|                       +--------+
|       /       ( n1 n2 --- quot )
|
| quot is n1/n2.
*/
void divide()
{
	long quotent,n1,n2;

	n2      = popsp();
	n1      = popsp();
	if(!n2){
#ifndef VECTORED_C_IO
		fprintf(stderr,"Divide by zero!\n");
#else
		stderr_vect("Divide by zero!\n");
#endif
		ABORT_FLAG = TRUE;
	}else{
		quotent = n1/n2;
		pushsp(quotent);
	}
}
/***********************+------+
|                       | plus |
|                       +------+
|       +       ( n1 n2 --- sum )
*/
void plus()
{
	long n1,n2,sum;

	n2  = popsp();
	n1  = popsp();
	sum = n1 + n2;
	pushsp(sum);
}
/***********************+-------+
|                       | minus |
|                       +-------+
|       -       ( n1 n2 --- n3)
|
*/
void minus()
{
	long n1, n2, diff;

	n2   = popsp();
	n1   = popsp();
	diff = n1 - n2;
	pushsp(diff);
}
/***********************+-----------+
|                       | less_than |
|                       +-----------+
|       <       ( n1 n2 --- tf)  t=n1<n2
*/
void less_than()
{
	long n1, n2, truth;

	n2 = popsp();
	n1 = popsp();
	if(n1 < n2){
		truth = TRUE;
	}else{
		truth = FALSE;
	}
	pushsp(truth);
}
/***********************+-----------------+
|                       | less_than_equal |
|                       +-----------------+
|       <=       ( n1 n2 --- tf)  t=n1<=n2
*/
void less_than_equal()
{
	long n1, n2, truth;

	n2 = popsp();
	n1 = popsp();
	if(n1 <= n2){
		truth = TRUE;
	}else{
		truth = FALSE;
	}
	pushsp(truth);
}
/***********************+--------------+
|                       | greater_than |
|                       +--------------+
|       >       ( n1 n2 --- tf )  t=n1>n2
*/
void greater_than()
{
	long n1,n2,truth;

	n2 = popsp();
	n1 = popsp();
	if(n1 > n2){
		truth = TRUE;
	}else{
		truth = FALSE;
	}
	pushsp(truth);
}
/***********************+--------------------+
|                       | greater_than_equal |
|                       +--------------------+
|       >=       ( n1 n2 --- tf )  t=n1=>n2
*/
void greater_than_equal()
{
	long n1,n2,truth;

	n2 = popsp();
	n1 = popsp();
	if(n1 >= n2){
		truth = TRUE;
	}else{
		truth = FALSE;
	}
	pushsp(truth);
}
/***********************+--------+
|                       | equals |
|                       +--------+
|       =       ( n1 n2 --- tf )
*/
void equals()
{
#ifdef READABLE
	long n1,n2,truth;

	n2 = popsp();
	n1 = popsp();
	if(n1 == n2){
		truth = TRUE;
	}else{
		truth = FALSE;
	}
	pushsp(truth);
#else
	pushsp(popsp() == popsp());
#endif
}
/***********************+-----+
|                       | mod |
|                       +-----+
|       rem     ( n1 n2 --- mod )
*/
void mod()
{
	long n1;
	long n2;

	n2 = popsp();
	n1 = popsp();

	if(!n2){
#ifndef VECTORED_C_IO
		fprintf(stderr,"Divide by zero!\n");
#else
		sprintf(tstring,"Divide by zero!\n");
		stderr_vect(tstring);
#endif
		ABORT_FLAG = TRUE;
	}else{
		pushsp(n1 % n2);
	}
}
/***********************+-------------+
|                       | u_less_than |
|                       +-------------+
|       U<      ( n1 n2 --- tf)
*/
void u_less_than()
{
	unsigned long n1,n2,truth;

	n2 = popsp();
	n1 = popsp();
	if(n1 < n2){
		truth = TRUE;
	}else{
		truth = FALSE;
	}
	pushsp(truth);
}
/***********************+----------------+
|                       | u_greater_than |
|                       +----------------+
|       U>      ( n1 n2 --- truth)
*/
void u_greater_than()
{
	unsigned long n1,n2,truth;

	n2 = popsp();
	n1 = popsp();
	if(n1 > n2){
		truth = TRUE;
	}else{
		truth = FALSE;
	}
	pushsp(truth);
}
/***********************+-------------+
|                       | base_number |
|                       +-------------+
|       This function handles number conversion when the base is
|       other than decimal. The built-in C function is much faster,
|       so call this one only when necessaary.
|
|       11/20/92        Fixed problem with A-F not being handled in hex.
|                       [nes]
*/
void base_number()
{
	long value;
	int  i;
	long digit;
	int  range;
	char *addr;
	int  len;
	long negative = FALSE;

	addr  = (char*)popsp();
	len   = *addr++;

	value = 0;
	range = BASE - 1;
	for(i=0;i<len;i++){
		digit  = *addr++;
		digit  = toupper(digit);
		if(digit > '9'){           /* take into account A-Z */
			digit -= 7;
		}
		if(digit == '-'){
			negative = TRUE;
			continue;
		}
		digit -= '0';           /* ascii to int conversion */
		if((digit < 0) || (digit > range)){
#ifndef VECTORED_C_IO
			fprintf(stderr,"Digit out of range.\n");
#else
			sprintf(tstring,"Digit out of range.\n");
			stderr_vect(tstring);
#endif
			abort_F();
			return;
		}
		value  = BASE * value + digit;
	}
	if(negative){
		pushsp((long)0 - value);
	}else{
		pushsp((long)value);
	}

}
/***********************+-----------+
|                       | zero_less |
|                       +-----------+
|       0<      ( n --- tf )
*/
void zero_less()
{
#ifdef READABLE
	long value;

	value = popsp();
	if(value < 0){
		pushsp(TRUE);
	}else{
		pushsp(FALSE);
	}
#else
	pushsp((long)(popsp())<0);
#endif 
}
/***********************+--------------+
|                       | zero_greater |
|                       +--------------+
|       0>      ( n --- tf )
*/
void zero_greater()
{
/*
	long value;

	value = popsp();
	if(value > 0){
		pushsp(TRUE);
	}else{
		pushsp(FALSE);
	}
*/
	pushsp((popsp()) > 0);
}
/***********************+------------+
|                       | zero_equal |
|                       +------------+
|       0=      ( n --- tf )
*/
void zero_equal()
{
	long value;

	value = popsp();
	if(value == 0){
		pushsp((long)TRUE);
	}else{
		pushsp((long)FALSE);
	}
}
/***********************+----------+
|                       | one_plus |
|                       +----------+
|       1+      ( n --- n+1 )
*/
void one_plus()
{
	long value;

	value = popsp();
	pushsp(value + 1);
}
/***********************+-----------+
|                       | one_minus |
|                       +-----------+
|       1-      ( n --- n-1 )
*/
void one_minus()
{
	long value;

	value = popsp();
	pushsp(value - 1);
}
/***********************+----------+
|                       | two_plus |
|                       +----------+
|       2+      ( n --- n+2 )
*/
void two_plus()
{
	long value;

	value = popsp();
	pushsp(value + 2);
}
/***********************+-----------+
|                       | two_minus |
|                       +-----------+
|       2-      ( n --- n-2 )
*/
void two_minus()
{
	long value;

	value = popsp();
	pushsp(value - 2);
}
/***********************+------------+
|                       | two_divide |
|                       +------------+
|       2/      ( n --- n/2 )
*/
void two_divide()
{
	long value;

	value = popsp();
	pushsp(value / 2);
}
/***********************+----------------+
|                       | star_slash_mod |
|                       +----------------+
|       * /mod  ( n1 n2 n3 --- n4 n5 )
*/
void star_slash_mod()
{
	long n1;
	long n2;
	long n3;
	long n4;
	long n5;

	n3 = popsp();
	n2 = popsp();
	n1 = popsp();

	n5 = n1 * n2;
	n4 = n5 / n3;
	n5 = n5 % n3;

	pushsp(n4);
	pushsp(n5);
}
/***********************+-----------+
|                       | slash_mod |
|                       +-----------+
|       /mod    ( n1 n2 --- mod )
*/
void slash_mod()
{
	long n1;
	long n2;

	n2 = popsp();
	n1 = popsp();

	pushsp(n1 % n2);
	pushsp(n1 / n2);
}
/***********************+---------+
|                       | decimal |
|                       +---------+
|       decimal ( --- )
*/
void decimal()
{
	BASE = 10;
}
/***********************+-----+
|                       | hex |
|                       +-----+
|       hex     ( --- )
*/
void hex()
{
	BASE = 16;
}
/***********************+--------+
|                       | binary |
|                       +--------+
|       binary  ( --- )
*/
void binary()
{
	BASE = 2;
}
/***********************+----------+
|                       | absolute |
|                       +----------+
|       abs     ( n --- |n| )
*/
void absolute()
{
/*
	long n;
*/
	int  n;

	n = popsp();
	n = abs(n );
	pushsp((long)n);
}
/***********************+----------+
|                       | prim_max |
|                       +----------+
|       max     ( n1 n2 --- n3 )
*/
void prim_max()
{
	long n1;
	long n2;

	n2 = popsp();
	n1 = popsp();

	if(n1 > n2){
		pushsp(n1);
	}else{
		pushsp(n2);
	}
}
/***********************+----------+
|                       | prim_min |
|                       +----------+
|       min     ( n1 n2 --- n3 )
*/
void prim_min()
{
	long n1;
	long n2;

	n2 = popsp();
	n1 = popsp();

	if(n1 < n2){
		pushsp(n1);
	}else{
		pushsp(n2);
	}
}
/***********************+--------+
|                       | negate |
|                       +--------+
|       negate  ( n1 --- -n1 )
| -->I don't have any idea that this is the correct code for
|    negate...
*/
void negate()
{
	long n1;

	n1 = popsp();
	pushsp(0 - n1);
}
/***********************+-----+
|                       | not |
|                       +-----+
|       not     ( n1 n2 --- n3 )
*/
void not()
{
	long n1;

	n1 = popsp();
	pushsp(~n1);
}
/***********************+----+
|                       | or |
|                       +----+
|       or      ( n1 n2 --- n1|n2 )
*/
void or()
{
	long n1;
	long n2;

	n2 = popsp();
	n1 = popsp();

	pushsp(n1 | n2);
}
/***********************+-----+
|                       | and |
|                       +-----+
|       and     ( n1 n2 --- n3 )
*/
void and()
{
	long n1;
	long n2;
	long n3;

	n2 = popsp();
	n1 = popsp();

	n3 = n1 & n2;
	pushsp(n3);
}
/***********************+---------+
|                       | um_star |
|                       +---------+
|       um*     ( u1 u2 --- d )
| unsigned multiply, returns double
*/
void um_star()
{
	unsigned long u1;
	unsigned long u2;

	u2 = popsp();
	u1 = popsp();
	pushsp(u1 * u2);
	pushsp((long)0);
}
/***********************+--------------+
|                       | um_slash_mod |
|                       +--------------+
|       um/mod  ( d u2 --- mod quot )
|
| This word implement unsigned division with remainder.
|
|NOTE: um/mod does not work on negative numbers...
*/
void um_slash_mod()
{
	unsigned long u1;
	unsigned long u2;

	u2 = abs(popsp());
	u1 = abs(popsp());              /* get rid of placeholder 0*/
	u1 = popsp();

	pushsp(u1 % u2);
	pushsp(u1 / u2);
}
/***********************+-----+
|                       | xor |
|                       +-----+
|       xor     ( n1 n2 --- n3 )
*/
void xor()
{
	long n1;
	long n2;

	n2 = popsp();
	n1 = popsp();
	pushsp(n1 ^ n2);
}
/***********************+------------+
|                       | star_slash |
|                       +------------+
|       * /     ( n1 n2 n3 --- n4 )
*/
void star_slash()
{
	long n1;
	long n2;
	long n3;
	long n4;

	n3 = popsp();
	n2 = popsp();
	n1 = popsp();

	n4 = n1 * n2 / n3;
	pushsp(n4);
}
/***********************+------------+
|                       | plus_store |
|                       +------------+
|       +!      ( n addr --- )
*/
void plus_store()
{
	long *addr;
	long n;

	addr   = (long*)popsp();
	n      = popsp();
	*addr += n;
}
/***********************+-----------+
|                       | base_prim |
|                       +-----------+
|       base ( --- 'BASE )
*/
void base_prim()
{
	pushsp((long)&BASE);
}
/********************************************************************
 ********************************************************************
 * This section of code is related to double numbers.               *
 ********************************************************************
 ********************************************************************/

/***********************+--------+
|                       | d_plus |
|                       +--------+
|       This is not a standard implmentation of d+. Until is already
|       32-bit. So a 0 is kept on top of the 32-bit number for
|       compatability with 16-bit Forths, but not used. If there
|       is a portable way to implement 64-bit integers in C, real
|       double words can be added.
*/
void d_plus()
{
	long n1;
	long n2;

	n1 = popsp();           /* the leading 0 */
	n1 = popsp();           /* The value     */
	n2 = popsp();           /* the leading 0 */
	n2 = popsp();           /* The value     */
	pushsp(n1+n2);          /* result        */
	pushsp((long)0);        /* placeholder 0 */
}
/***********************+---------+
|                       | d_minus |
|                       +---------+
|
*/
void d_minus()
{
	long n1;
	long n2;

	n2 = popsp();           /* the leading 0 */
	n2 = popsp();           /* The value     */
	n1 = popsp();           /* the leading 0 */
	n1 = popsp();           /* The value     */
	pushsp(n1-n2);          /* result        */
	pushsp((long)0);        /* placeholder 0 */
}
/***********************+--------+
|                       | d_less |
|                       +--------+
|
*/
void d_less()
{
	long n1;
	long n2;
	long truth;

	n2 = popsp();           /* the leading 0 */
	n2 = popsp();           /* The value     */
	n1 = popsp();           /* the leading 0 */
	n1 = popsp();           /* The value     */
	if(n1 < n2){
		truth = TRUE;
	}else{
		truth = FALSE;
	}
	pushsp(truth);
}
/***********************+---------+
|                       | dnegate |
|                       +---------+
|
*/
void dnegate()
{
	long n1;

	n1 = popsp();
	n1 = popsp();
	pushsp(0 - n1);
	pushsp(0);
}
