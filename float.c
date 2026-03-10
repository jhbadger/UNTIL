/*
|       FILE: FLOAT.C
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
|	DATE		DESCRIPTION
|	03/27/95	Initial Coding. [nes]
*/

#include "compiler.h"

#ifdef FLOAT_POINT

#include <stdio.h>
#include <string.h>

#ifndef NO_STDLIB
#include <stdlib.h>
#endif 

#include "until.h"
#include "functs.h"

#define FSTACKSIZE	32

long   FP;
double *fstack;
struct DictHeader *F_LIT_WA;

/***********************+------------------+
|			| init_float_stack |
|			+------------------+
*/
void init_float_stack()
{
	FP = 0;
}
/***********************+------+
|                       | fpop |
|                       +------+
*/
double fpop()
{
	double value;

	FP--;
	if(FP<0){
#ifndef VECTORED_C_IO
		fprintf(stderr,"F Stack underflow.");
#else
		stderr_vect("F Stack underflow.");
#endif
		FP = 0;
	}
	value  = *(fstack + FP);
	return(value);
}
/***********************+-------+
|                       | fpush |
|                       +-------+
|
| This word pushes a value on the floating point stack. The stack
| is a double. Stack is tested for overflow.
*/
void fpush(double value)
{
	double last;

	if(FP > FSTACKSIZE){
#ifndef VECTORED_C_IO
		printf("F Stack Overflow");
		last = fpop();
		printf("Last value on stack was %g.\n",last);
		printf("Stack cleared\n\n");
		FP = (double)-1;
#else
		stderr_vect("F Stack overflow.");
		last = fpop();
		sprintf(tstring,"Last value on stack was %g.\n",last);
		stderr_vect(tstring);
		stderr_vect("Stack cleared\n\n");
		FP = (double)-1;
#endif
	}else{
		*(fstack + FP) = value;
		FP++;
	}
}
/***********************+--------+
|                       | f_drop |
|                       +--------+
|       fdrop    ( fn --- )
*/
void f_drop()
{
	FP--;
	if(FP<0){
#ifndef VECTORED_C_IO
		fprintf(stderr,"F Stack underflow.");
#else
		stderr_vect("F Stack underflow.");
#endif
		FP = 0;
	}
}
/***********************+--------+
|                       | f_swap |
|                       +--------+
|       fswap    ( f1 f2 --- f2 f1 )
*/
void f_swap()
{
	double value;

	if(FP < 2){
#ifndef VECTORED_C_IO
		fprintf(stderr,"Float Stack: not enough entries.");
#else
		stderr_vect("Float Stack: not enough entries.");
#endif
		return;
	}
	value              = *(fstack + FP - 1);
	*(fstack + FP - 1) = *(fstack + FP - 2);
	*(fstack + FP - 2) = value;
}
/***********************+-------+
|                       | f_rot |
|                       +-------+
|       frot ( f1 f2 f3 --- f2 f3 f1)
*/
void f_rot()
{
	double n1, n2, n3;

	n3 = fpop();
	n2 = fpop();
	n1 = fpop();

	fpush(n2);
	fpush(n3);
	fpush(n1);
}
/***********************+-------+
|                       | f_dup |
|                       +-------+
|       fdup     ( f --- f f )
|
*/
void f_dup()
{
	double value;

	value = *(fstack + FP - 1);
	fpush(value);
}
/***********************+--------+
|                       | f_over |
|                       +--------+
|       fover    ( f1 f2 --- f1 f2 f1 )
*/
void f_over()
{
	double n1;
	double n2;

	n2 = fpop();
	n1 = fpop();

	fpush(n1);
	fpush(n2);
	fpush(n1);
}
/***********************+---------+
|                       | f_store |
|                       +---------+
|
|       f!       ( val addr --- )
*/
void f_store()
{
	double *addr;
	double value;

	addr  = (double*) popsp();
	value = fpop();
	*addr = value;
}
/***********************+---------+
|                       | f_fetch |
|                       +---------+
|
|       f@       ( addr --- f )
*/
void f_fetch()
{
	double value;
	double *addr;

	addr  = (double*)popsp();
	value = *addr; 
	fpush(value);  
}
/***********************+---------------+
|                       | do_f_variable |
|                       +---------------+
| Push the address of a double onto the parameter stack.
*/
void do_f_variable()
{
	pushsp(ADDR &(*WA->PFA.d_ptr));
}
/***********************+---------------+
|                       | do_f_constant |
|                       +---------------+
*/
void do_f_constant()
{
	fpush(*WA->PFA.d_ptr);
}
/***********************+--------------------+
|                       | Compile_f_constant |
|                       +--------------------+
|  The word to create should be in pad. PAD is counted in the
|  Forth tradition. It must be converted to null terminated
|  for C to digest it properly.
|
|  Compile_variable takes a list of DictHeader pointers and inserts
|  it into the PFA. Create() is called first to allocate a skeleton
|  word. The value to initialize the constant with is on the stack.
|
*/
void compile_f_constant()
{
	double *d_addr;
			/*
			| Create skeleton word header
			*/
	next_word();
	Create();
			/*
			| Fill in the rest of the word.
			*/
	DP->NFA->smudge    = VISIBLE;
	DP->NFA->immediate = NOT_IMMEDIATE;
	DP->CFA            = do_f_constant;
	d_addr             = (double*)malloc(sizeof(double));
#ifdef CHECK_MALLOC
	if(!d_addr){
#ifndef VECTORED_C_IO
		fprintf(stderr,"Could not allocate float constant\n");
#else
		stderr_vect("Could not allocate float constant\n");
#endif
		abort_F();
		return;
	}
#endif
	DP->PFA.d_ptr      = d_addr;
	*DP->PFA.d_ptr     = fpop();
}
/***********************+--------------------+
|                       | Compile_f_variable |
|                       +--------------------+
|  The word to create should be in pad. PAD is counted in the
|  Forth tradition. It must be converted to null terminated
|  for C to digest it properly.
|
|  Compile_variable takes a list of DictHeader pointers and inserts
|  it into the PFA. Create() is called first to allocate a skeleton
|  word.
|
*/
void compile_f_variable()
{
	double *d_addr;
			/*
			| Create skeleton word header
			*/
	next_word();
	Create();
			/*
			| Fill in the rest of the word.
			*/
	DP->NFA->smudge    = VISIBLE;
	DP->NFA->immediate = NOT_IMMEDIATE;
	DP->CFA            = do_f_variable;
	d_addr             = (double*)malloc(sizeof(double));
#ifdef CHECK_MALLOC
	if(!d_addr){
#ifndef VECTORED_C_IO
		fprintf(stderr,"Could not allocate float constant\n");
#else
		stderr_vect("Could not allocate float constant\n");
#endif
		abort_F();
		return;
	}
#endif
	DP->PFA.d_ptr      = d_addr;
	*DP->PFA.d_ptr     = 0;
			/*
			| Should these go into Create() instead of here???
			*/
	comma_offset       = FIRST_CELL;
	pfa_offset         = FIRST_CELL;
}
/***********************+-----------+
|                       | f_literal |
|                       +-----------+
|       fliteral ( n --- )
*/
void f_literal()
{
	pfa_list[pfa_offset++] = F_LIT_WA;
	WA                     = (struct DictHeader*)popsp();
	literal_vect           = (vectoredFunction) literal;
}

/***********************+-----------+
|                       | fmultiply |
|                       +-----------+
|       *       ( f1 f2 --- fproduct )
*/
void fmultiply()
{
	double product,mult1,mult2;

	mult2   = fpop();
	mult1   = fpop();
	product = mult1 * mult2;
	fpush(product);
}
/***********************+--------+
|                       | divide |
|                       +--------+
|       /       ( n1 n2 --- quot )
|
| quot is n1/n2.
*/
void fdivide()
{
	double quotent,n1,n2;

	n2      = fpop();
	n1      = fpop();
	if(!n2){
#ifndef VECTORED_C_IO
		fprintf(stderr,"Divide by zero!\n");
#else
		stderr_vect("Divide by zero!\n");
#endif
		ABORT_FLAG = TRUE;
	}else{
		quotent = n1/n2;
		fpush(quotent);
	}
}
/***********************+-------+
|                       | fplus |
|                       +-------+
|       f+       ( n1 n2 --- sum )
*/
void fplus()
{
	double n1,n2,sum;

	n2  = fpop();
	n1  = fpop();
	sum = n1 + n2;
	fpush(sum);
}
/***********************+--------+
|                       | fminus |
|                       +--------+
|       f-       ( n1 n2 --- n3)
|
*/
void fminus()
{
	double n1, n2, diff;

	n2   = fpop();
	n1   = fpop();
	diff = n1 - n2;
	fpush(diff);
}
/***********************+-------------+
|                       | f_less_than |
|                       +------------+
|       f<       ( n1 n2 --- tf)  t=n1<n2
*/
void f_less_than()
{
	double n1, n2;
	long truth;

	n2 = fpop();
	n1 = fpop();
	if(n1 < n2){
		truth = TRUE;
	}else{
		truth = FALSE;
	}
	pushsp(truth);
}
/***********************+-------------------+
|                       | f_less_than_equal |
|                       +-------------------+
|       f<=       ( n1 n2 --- tf)  t=n1<=n2
*/
void f_less_than_equal()
{
	double n1, n2;
	long truth;

	n2 = fpop();
	n1 = fpop();
	if(n1 <= n2){
		truth = TRUE;
	}else{
		truth = FALSE;
	}
	pushsp(truth);
}
/***********************+----------------+
|                       | f_greater_than |
|                       +----------------+
|       f>       ( n1 n2 --- tf )  t=n1>n2
*/
void f_greater_than()
{
	double n1,n2;
	long truth;

	n2 = fpop();
	n1 = fpop();
	if(n1 > n2){
		truth = TRUE;
	}else{
		truth = FALSE;
	}
	pushsp(truth);
}
/***********************+----------------------+
|                       | f_greater_than_equal |
|                       +----------------------+
|       f>=       ( n1 n2 --- tf )  t=n1=>n2
*/
void f_greater_than_equal()
{
	double n1,n2;
	long truth;

	n2 = fpop();
	n1 = fpop();
	if(n1 >= n2){
		truth = TRUE;
	}else{
		truth = FALSE;
	}
	pushsp(truth);
}
/***********************+----------+
|                       | f_equals |
|                       +----------+
|      f=       ( n1 n2 --- tf )
*/
void f_equals()
{
	pushsp((long)fpop() == fpop());
}
/***********************+-----------+
|                       | prim_fmax |
|                       +-----------+
|       fmax     ( n1 n2 --- n3 )
*/
void prim_fmax()
{
	double n1;
	double n2;

	n2 = fpop();
	n1 = fpop();

	if(n1 > n2){
		fpush(n1);
	}else{
		fpush(n2);
	}
}
/***********************+-----------+
|                       | prim_fmin |
|                       +-----------+
|       fmin     ( n1 n2 --- n3 )
*/
void prim_fmin()
{
	double n1;
	double n2;

	n2 = fpop();
	n1 = fpop();

	if(n1 < n2){
		fpush(n1);
	}else{
		fpush(n2);
	}
}
/***********************+-------------+
|                       | fstar_slash |
|                       +-------------+
|       * /     ( n1 n2 n3 --- n4 )
*/
void fstar_slash()
{
	double n1;
	double n2;
	double n3;
	double n4;

	n3 = fpop();
	n2 = fpop();
	n1 = fpop();

	n4 = n1 * n2 / n3;
	fpush(n4);
}
/***********************+---------+
|                       | fnumber |
|                       +---------+
|       fnumber  ( addr --- n )
|
*/
void fnumber()
{
	double value;
	double *d_addr;
	char   *addr;
	char   nstring[32];
		/*
		|  This is the interactive code. Compiled operation
		|  follows.
		*/
	addr        = (char *) popsp();
	counted_to_null(addr,nstring);          /* Look at alternative  */
	value       = atof(nstring);
	number_vect = (vectoredFunction)number;	/* restore normal number */
	if(STATE   == EXECUTION_MODE){
		fpush(value);
		if(value == 0){                 /* bad conversion       */
#ifndef VECTORED_C_IO
		fprintf(stderr,"? \n");
#else
		stderr_vect("? \n");
#endif
			abort_F();                      /* should be abort_quote*/
		}
		return;
	}
		/*
		| Compile time action from here on out.
		*/
	d_addr = (double*)malloc(sizeof(double));
#ifdef CHECK_MALLOC
	if(!d_addr){
#ifndef VECTORED_C_IO
		fprintf(stderr,"Could not allocate float literal\n");
#else
		stderr_vect("Could not allocate float literal\n");
#endif
		abort_F();
		return;
	}
#endif
	*d_addr      = value;
	pushsp((long)d_addr);
	literal_vect = (vectoredFunction)f_literal;
}
/***********************+--------------+
|                       | float_number |
|                       +--------------+
|	f# 1234
*/
void float_number()
{
	number_vect = (vectoredFunction)fnumber;
}
/***********************+---------+
|                       | f_dot_s |
|                       +---------+
|       f.s      ( --- )
*/
void f_dot_s()
{
	int    TSP;                        /* temp stack ptr */
	double n1;

	TSP = FP - 1;
	if(TSP < 0){
#ifndef VECTORED_C_IO
		fprintf(stderr,"Float Stack Empty\n");
#else
		stderr_vect("Float Stack Empty\n");
#endif
		return;
	}
	while(TSP >= 0){
		n1 = *(fstack + TSP);
		TSP--;
#ifdef EFLOAT
#ifndef VECTORED_C_IO
		fprintf(stderr,"%e ",n1);
#else
		sprintf(tstring,"%e ",n1);
		stderr_vect(tstring);
#endif
#endif
#ifdef GFLOAT
		printf("%g ",n1);
#ifndef VECTORED_C_IO
		fprintf(stderr,"%g ",n1);
#else
		sprintf(tstring,"%g ",n1);
		stderr_vect(tstring);
#endif
#endif
#ifdef FFLOAT
#ifndef VECTORED_C_IO
		fprintf(stderr,"%f ",n1);
#else
		sprintf(tstring,"%f ",n1);
		stderr_vect(tstring);
#endif
#endif
	}
}
/***********************+-------+
|                       | f_dot |
|                       +-------+
|       f.      ( --- )
*/
void f_dot()
{
	double n1;

	n1 = fpop();
#ifdef EFLOAT
#ifndef VECTORED_C_IO
	fprintf(stderr,"%e ",n1);
#else
	sprintf(tstring,"%e ",n1);
	stdout_vect(tstring);
#endif
#endif
#ifdef FFLOAT
#ifndef VECTORED_C_IO
	fprintf(stderr,"%f ",n1);
#else
	sprintf(tstring,"%f ",n1);
	stdout_vect(tstring);
#endif
#endif
#ifdef GFLOAT
#ifndef VECTORED_C_IO
	fprintf(stderr,"%g ",n1);
#else
	sprintf(tstring,"%g ",n1);
	stdout_vect(tstring);
#endif
#endif
}
/***********************+----------+
|			| to_float |
|			+----------+
*/
void to_float()
{
	fpush((double)popsp());
}
/***********************+------------+
|			| from_float |
|			+------------+
*/
void from_float()
{
	pushsp((long)fpop());
}
/***********************+----------+
|                       | do_f_lit |
|                       +----------+
|
| This is the run-time portion of literal.
*/
void do_f_lit()
{
	double *d_addr;
	
	d_addr = (double*)*IP++;
	fpush(*d_addr);
}
/***********************+--------------+
|                       | do_f_literal |
|                       +--------------+
|       literal ( n --- )
*/
void do_f_literal()
{
	pfa_list[pfa_offset++] = LIT_WA;
	WA                     = (struct DictHeader*)popsp();
	WA                     = (struct DictHeader*)R_addr;
	pfa_list[pfa_offset++] = WA;
}
/***********************+------------+
|			| float_init |
|			+------------+
*/
void float_init()
{
	fstack = (double*)malloc(FSTACKSIZE);	
	init_float_stack();

	build_primitive("f_lit",	do_f_lit);
	build_primitive("f+",		fplus);
	build_primitive("f-",		fminus);
	build_primitive("f*",		fmultiply);
	build_primitive("f/",		fdivide);
	build_primitive("f.s",		f_dot_s);
	build_primitive("f.",		f_dot);
	build_primitive("f<",		f_less_than);
	build_primitive("f<=",		f_less_than_equal);
	build_primitive("f>",		f_greater_than);
	build_primitive("f>=",		f_greater_than_equal);
	build_primitive("fmax",		prim_fmax);
	build_primitive("fmin",		prim_fmin);
	build_primitive("f*/",		fstar_slash);
	build_primitive("fdrop",	f_drop);
	build_primitive("fswap",	f_swap);
	build_primitive("frot",		f_rot);
	build_primitive("fdup",		f_dup);
	build_primitive("fover",	f_over);
	build_primitive("f!",		f_store);
	build_primitive("f@",		f_fetch);
	build_primitive("fconstant",	compile_f_constant);
	build_primitive("fvariable",	compile_f_variable);
	build_primitive(">float",	to_float);
	build_primitive("float>",	from_float);

	build_iprim("f#",		float_number);

	F_LIT_WA             = set_WA("f_lit");

	hide("f_lit");

/*
	build_primitive("f",		f_);
	build_primitive("f",		f_);
	build_primitive("f",		f_);
*/
}
#endif
