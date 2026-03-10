/*
|       FILE: QUOTE.C
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
|	DATE		CHANGE
|	----		------
|	04/02/94	Broke module out from prim.c and compile.c. [nes]
*/

#include "compiler.h"

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#ifndef NO_STDLIB
#include <stdlib.h>
#endif 

#ifdef ANSIC
#include <conio.h>
#endif

#ifdef VAX
#include <file.h>
#endif

#include "until.h"
#include "functs.h"

/***********************+-------+
|                       | quote |
|                       +-------+
|	" string"
|
|  This is the run-time portion of " and is valid only
|  in compile mode.
|  Counted or Null form of the string is determined at compile time...
*/
void quote()
{
	unsigned long addr;
	addr = (unsigned long) *IP++;
	if(quote_null){
		addr++;
	}
	pushsp((long)addr);
}
/***********************+------------+
|			| quote_char |
|			+------------+
*/
char quote_char(char ch)
{
	switch(ch){
	case 'n':
		ch = '\n';
		break;
	case 't':
		ch = '\t';
		break;
	case '"':
		ch = '"';
		break;
	case 'a':
		ch = 7;			/* ^G or bell char */
		break;
	case 'b':
		ch = '\b';
		break;
	case 'f':
		ch = '\f';
		break;
	case 'r':
		ch = '\r';
		break;
	case '\\':
		ch = '\\';
		break;
	case 'v':
		ch = '\v';
		break;
	case '\'':
		ch = '\'';
		break;
	case '0':
		ch = '\0';
		break;
	case '?':
		ch = '\?';
		break;
	default:
		;
	}
	return(ch);
}
/***********************+---------------+
|                       | Compile_quote |
|                       +---------------+
|       " xxxxx"
|
| This word can be used only inside of a colon definition
| because it allocates space in a dictionary entry. Also note
| that it is identical to Compile_dot_quote, except for the
| run-time address compiled into the dictionary.
|
*/
#ifdef IGNORE
void Compile_quote()
{
	char *const_string;
	long len;
	long i;
	char ch;

#ifdef IGNORE
	pushsp('\"');
	word();
#endif
		/*
		|  Suck up chars until " is found
		*/
	i         = 1;
	pad[1]    = '\0';
	ch        = *(tib + IN);
	while(ch != '"'){
		*(pad + i++) = ch;
		IN++;
		if(IN > tib_len){
			(FREAD_WA)();       	/* read next line from stream */
		}
		ch      = *(tib + IN);
	}
	*(pad + i++) = '\0';
	*(pad + i)   = '"';
	*pad         = i - 2;
	IN++;
		/*
		| Check for state. If not in compile mode, push addr
		| and get out, else continue
		*/
	if(STATE == EXECUTION_MODE){
		const_string = pad;
		if(quote_null){ const_string++; }
		strcpy(here_here,const_string);
		pushsp((long)here_here);
		return;
	}
		/*
		| Execution from here on is done in compile
		| mode only...
		*/
	len = *pad;
	len+= 2;
	const_string = (char*)malloc(len+2);
#ifdef CHECK_MALLOC
	if(!const_string){
#ifndef VECTORED_C_IO
		printf("Could not allocate memory for a quote string\n");
#else
		stderr_vect("Could not allocate memory for a quote string\n");
#endif
		abort_F();
		return;
	}
#endif
			/*
			| If quote_null flag is true, save string as a
			| null terminated string. Otherwise, store it
			| as a counted string. (This is done at run-time
                        | now...)
			*/
	memcpy(const_string,pad,len);
	*(const_string + len - 1) = 0;		/* NULL terminate string */
	pfa_list[pfa_offset++] = QUOTE_WA;
	pfa_list[pfa_offset++] = (struct DictHeader*) const_string;
	WA = 0;
}
#endif
void Compile_quote()
{
	long len;
	long i;
	char *const_string;
	char ch;

		/*
		|  Suck up chars until " is found
		*/
	i     = 1;
	FOREVER{
		ch = *(tib + IN++);
		if(IN > tib_len){
			(FREAD_WA)();
		}
			/*
			| get out when a " is found...
			*/
		if(ch == '"'){
			break;
		}
			/*
			| Check for quote character
			*/
		if(ch == '\\'){
			ch = *(tib + IN++);
			ch = quote_char(ch);
		}
		*(pad + i++) = ch;
	}
	*(pad + i++) = '\0';
	*(pad + i)   = '"';
	*pad         = i - 2;
		/*
		| Check for state. If not in compile mode, push addr
		| and get out, else continue
		*/
	if(STATE == EXECUTION_MODE){
		const_string = pad;
		if(quote_null){ const_string++; }
		strcpy(here_here,const_string);
		pushsp((long)here_here);
		return;
	}
		/*
		| Execution from here on is done in compile
		| mode only...
		*/
	len = *pad;
	len+= 2;
	const_string = (char*)malloc(len+2);
#ifdef CHECK_MALLOC
	if(!const_string){
#ifndef VECTORED_C_IO
		printf("Could not allocate memory for a quote string\n");
#else
		stderr_vect("Could not allocate memory for a quote string\n");
#endif
		abort_F();
		return;
	}
#endif
		/*
		| If quote_null flag is true, save string as a
		| null terminated string. Otherwise, store it
		| as a counted string. (This is done at run-time
		| now...)
		*/
	memcpy(const_string,pad,len);
	*(const_string + len - 1) = 0;		/* NULL terminate string */
	pfa_list[pfa_offset++] = QUOTE_WA;
	pfa_list[pfa_offset++] = (struct DictHeader*) const_string;
	WA = 0;
}
/***********************+------------+
|                       | do_cstring |
|                       +------------+
|       { xxxxx}
|
| This word can be used only interactively. The temp string is
| left at here.
*/
void do_cstring()
{
	long len;

	pushsp('}');
	word();
	drop();
	len = *pad;
	memcpy(here_here,pad+1,len);
	*(here_here + len) = 0;		/* NULL; */
}
/******************************************************************
 ******************************************************************
 * This section of code impelements printf                        *
 ******************************************************************
 ******************************************************************/
#define PSTK	0
#define FSTK	1

long num_params;
char *out;
/***********************+--------------+
|			| count_params |
|			+--------------+
*/
long count_params(char *format)
{
	int i=0;
	
	while(*format){
		if(*format++ == '%'){
			if(*format++ == '%'){
				continue;
			}else{
				i++;
			}
		}
	}
	return(i);
}
/***********************+-------------------+
|			| printf_conversion |
|			+-------------------+
| First pass does not handle %2d etc. That will come later...
*/
int printf_conversion(char *format)
{
	int  i;
	int  len;
	long value;
	char temp[32];
	char result[256];
	char str[8];
	char ch;
	
	strcpy(temp,"%");
	str[0] = '\0';
	i      = 0;
	ch     = *format++;
	len    = 1;
	
	while(ch){
		if(!isdigit(ch) && (ch != '.')){
			break; }
		str[0]    = ch;
		strcat(temp,str);
		ch        = *format++;
		len++;
	}
	switch(ch){
	case '%':
		*out++ = '%';
		break;
	case 'd':
		strcat(temp,"d");
		value = pstack[SP - num_params];
		num_params--;
		sprintf(result,temp,value);
		break;
	case 's':
		strcat(temp,"s");
		value = pstack[SP - num_params];
		num_params--;
		sprintf(result,temp,value);
		break;
	case 'c':
		strcat(temp,"c");
		value = pstack[SP - num_params];
		num_params--;
		sprintf(result,temp,value);
		break;
	case 'i':
		strcat(temp,"i");
		value = pstack[SP - num_params];
		num_params--;
		sprintf(result,temp,value);
		break;
	default:
		;
	}
	while(result[i]){
		*out++ = result[i++];
	}
	return(len);
}
/***********************+-------------+
|                       | prim_format |
|                       +-------------+
| This version does not test stack depth, which it should. The fact that
| args may come from multiple stacks (param, fp, big int, etc)
| complicates checking it immensly. Also floating point is not
| covered yet.
*/
void prim_format()
{
	int  i;
	int  save_params;
	char *format;
	char ch;
	
	out         = pad;
	format      = (char*)popsp();
	if(!quote_null){			/* for null terminated strings only*/
		format++;			/* skip count byte */
	}
	num_params  = count_params(format);
	save_params = num_params;
/* test stack depth...   */
	while(*format){
		ch = *format++;
		if(ch == '"'){
			break;
		}
		if(ch == '%'){
			i       = printf_conversion(format);
			format += i;		/* account for # bytes eaten */
		}else{
			*out++ = ch;
		}
	}
	*out = '\0';
	SP  -= save_params;
}
/***********************+-------------+
|                       | prim_printf |
|                       +-------------+
| Usage:
|	args... " format string" printf
|
*/
void prim_printf()
{
	char *string;
	
	prim_format();		/* Temp string built at pad */
#ifndef VECTORED_C_IO
	printf("%s",pad);
#else
	sprintf(tstring,"%s",pad);
	stderr_vect(tstring);
#endif
}
/***********************+--------------+
|                       | prim_sprintf |
|                       +--------------+
| Usage:
|	args... " format string" 'string sprintf
|
*/
void prim_sprintf()
{
	char *result;

	result = (char*) popsp();
	prim_format();
	strcpy(result,pad);
}
/***********************+--------------+
|                       | prim_fprintf |
|                       +--------------+
| Usage:
|	args... " format string" fd sprintf
|
*/
void prim_fprintf()
{
	FILE *fd;

	fd     = (FILE*)popsp();
	prim_format();
	fprintf(fd,"%s",pad);
}
#ifdef FLOAT_POINT

extern long FP;
extern double *fstack;
/***********************+---------------------+
|			| printf_f_conversion |
|			+---------------------+
| This handles floating point only!!!!
*/
int printf_f_conversion(char *format)
{
	int  i;
	int  len;
	double value;
	char temp[32];
	char result[256];
	char str[8];
	char ch;
	
	strcpy(temp,"%");
	str[0] = '\0';
	i   = 0;
	ch  = *format++;
	len = 1;
	while(ch){
		if(!isdigit(ch) && (ch != '.')){
			break; }
		str[0]    = ch;
		strcat(temp,str);
		ch        = *format++;
		len++;
	}
	switch(ch){
	case '%':
		*out++ = '%';
		break;
	case 'e':
		strcat(temp,"e");
		value = *(fstack + (FP - num_params));
		num_params--;
		sprintf(result,temp,value);
		break;
	case 'f':
		strcat(temp,"f");
		value = *(fstack + (FP - num_params));
		num_params--;
		sprintf(result,temp,value);
		break;
	case 'g':
		strcat(temp,"g");
		value = *(fstack + (FP - num_params));
		num_params--;
		sprintf(result,temp,value);
		break;
	default:
		;
	}
	while(result[i]){
		*out++ = result[i++];
	}
	return(len);
}
/***********************+---------------+
|                       | prim_f_format |
|                       +---------------+
| This version does not test stack depth, which it should. The fact that
| args may come from multiple stacks (param, fp, big int, etc)
| complicates checking it immensly. Also floating point is not
| covered yet.
*/
void prim_f_format()
{
	int  i;
	int  save_params;
	char *format;
	char ch;
	
	out         = pad;
	format      = (char*)popsp();
	if(!quote_null){			/* for null terminated strings only*/
		format++;			/* skip count byte */
	}
	num_params  = count_params(format);
	save_params = num_params;
/* test stack depth...		*/
	while(*format){
		ch = *format++;
		if(ch == '"'){
			break;
		}
		if(ch == '%'){
			i       = printf_f_conversion(format);
			format += i;	/* account for # bytes eaten */
		}else{
			*out++ = ch;
		}
	}
	*out = '\0';
	FP  -= save_params;
}
/***********************+---------------+
|                       | prim_f_printf |
|                       +---------------+
| Usage:
|	args... " format string" printf_f
|
*/
void prim_printf_f()
{
	prim_f_format();
#ifndef VECTORED_C_IO
	printf("%s",pad);
#else
	sprintf(tstring,"%s",pad);
	stderr_vect(tstring);
#endif
}
/***********************+----------------+
|                       | prim_sprintf_f |
|                       +----------------+
| Usage:
|	args... " format string" 'string sprintf_f
|
*/
void prim_sprintf_f()
{
	char *result;

	result = (char*) popsp();
	prim_f_format();
	strcpy(result,pad);
}
/***********************+----------------+
|                       | prim_fprintf_f |
|                       +----------------+
| Usage:
|	args... " format string" fd fprintf_f
|
*/
void prim_fprintf_f()
{
	FILE *fd;

	fd     = (FILE*) popsp();
	prim_format();
	fprintf(fd,"%s",pad);
}
#endif
