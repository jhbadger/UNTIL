/*
|       FILE: PRIM.C
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
|       11/20/92        Modified dot() to output hex and octal numbers
|                       as well as decimal numbers. [nes]
|	12/17/92	Corrected behavior of ascii. It now works in
|			a colon definition. [nes]
|	02/11/93	Added , and c, and fixed fill(). [nes]
|	03/11/93	Made read_next_line() vectored to FREAD_WA
|	07/05/93	Moved vocabulary stuff to VOCAB.C. [nes]
|	07/18/93	Made many minor changes to try and fix VOCAB
|			and DOES> problems. [nes]
|       02/05/94        Modified pushsp() to not crash on stack overflow.
|                       The suggestions for this changes came from RCS. [nes]
|	06/24/94	Started folding in changes from S-Engine. pushsp()
|			and popsp()... [nes]
|	06/24/94	Modified dump to not put out "system" words. [nes]
|	11/13/94	Added version of word() that calls strtok()
|			instead of doing it all by hand. [nes]
|	01/16/95	Made cleanup changes suggested by Akira Kida,
|			akida@isd.hin.konica.co.jp. [nes]
|	04/16/95	Folded Skip Carter's changes into base code. [nes]
*/

#include "compiler.h"
#include <time.h>
#include <errno.h>

#include <stdio.h>
#include <string.h>

#ifndef NO_STDLIB
#include <stdlib.h>
#endif

#ifdef ANSIC
#include <conio.h>
#include <time.h>
#endif

#ifdef GCC
#include <time.h>
#endif

#ifdef MIPS
#include <sys/types.h>
#endif

#if defined( SGI_INDY ) || defined( MIPS ) || defined( SPARC ) || defined( OSF )
#include <time.h>
#include <errno.h>
#endif


#ifdef VAX
#include <time.h>
#include <errno.h>
#endif


#include "until.h"
#include "functs.h"

#define   SP   PSP

/***********************+-------+
|                       | popsp |
|                       +-------+
*/
long popsp()
{
	long value;

	SP--;
	if(SP < 0L){
#ifndef VECTORED_C_IO
		fprintf(stderr,"P Stack underflow.");
#else
		stderr_vect("P Stack underflow.");
#endif
		SP = 0L;
	}
	value  = *(pstack + SP);
	R_addr = (vectoredFunction)value;
	return(value);
}
/***********************+--------+
|                       | pushsp |
|                       +--------+
|
| This word pushes a value on the parameter stack. The stack
| is 32-bits. Stack is tested for overflow.
*/
void pushsp(long value)
{
	long last;

	if(SP>PSTACKSIZE){
#ifndef VECTORED_C_IO
	       fprintf(stderr,"P Stack Overflow");
#else
	       stderr_vect("P Stack Overflow");
#endif
		last = popsp();
#ifdef PC_FIX
		last = (long)R_addr;
#endif
#ifndef VECTORED_C_IO
		fprintf(stderr,"Last value on stack was %ld.\n",last);
#else
		sprintf(tstring,"Last value on stack was %ld.\n",last);
		stderr_vect(tstring);
#endif
#ifndef VECTORED_C_IO
		fprintf(stderr,"Stack cleared\n\n");
#else
		stderr_vect("Stack cleared\n\n");
#endif
		SP = -1L;
	}else{
		*(pstack + SP) = value;
		SP++;
	}
}
/***********************+-------+
|                       | poprs |
|                       +-------+
*/
long poprs()
{
	long value;

	RS--;
	if(RS < 0L){
#ifndef VECTORED_C_IO
		fprintf(stderr,"R Stack underflow.");
#else
		sprintf(tstring,"R Stack underflow.");
		stderr_vect(tstring);
#endif
		RS = 0L;
	}
	value = (long)*(rstack + RS);
	return(value);
}
/***********************+--------+
|                       | pushrs |
|                       +--------+
*/
void pushrs(long value)
{
	if(RS>RSTACKSIZE){
#ifndef VECTORED_C_IO
		fprintf(stderr,"R Stack Overflow");
#else
		sprintf(tstring,"R Stack Overflow");
		stderr_vect(tstring);
#endif
		RS       = (long)RSTACKSIZE;
	}
	*(rstack + RS++) = value;
}
/***********************+------+
|                       | word |
|                       +------+
|  This word extracts the next word from the input stream (tib)
|  and places it in pad. The parameter stack is used to pass
|  and return arguments.
|
|  NOTE: tib_len must have been set by the input function prior
|        to calling word().
|
|       word()          ( delim --- 'pad )
*/
void word()
{
	char ch;
	char delim;             /* word delimiter */
	int  i;
	long X;

	if(IN  >= tib_len){
		(FREAD_WA)();       		/* read next line from stream */
	}
	X       = popsp();
	delim   = (char) X;
	ch      = *(tib + IN);
	IN++;
		/*
		|  Skip leading delimiters
		*/
	while(ch == delim){
		ch      = *(tib + IN);
		IN++;
		if(IN > tib_len){
			ch = delim;             /* force back thru 1 more time */
			(FREAD_WA)();       	/* read next line from stream */
		}
	}
		/*
		|  Collect chars up to a match on delim. Chars are
		|  loaded into pad starting at pos. 1. pad[0] is
		|  reserved for the length byte.
		*/
	i = 1;

	while(ch != delim){
		*(pad + i++) = ch;
		ch       = tib[IN++];
		if(IN > tib_len){		/* assume EOL is end of current word */
			break;
		}
	}
	*(pad + i++) = '\0';
	*(pad + i)   = delim;
	*pad         = i - 2;
	pushsp((long)pad);
}
/*
| This version does not append the delimiter character on the
| end of the string
*/
#ifdef IGNORE
void X_word()
{
	static int flag;
	static char *tib_ptr;
	static not_first_time;

	int  i;
	long X;
	char *string;
	char ch;
	char delim[8];             /* word delimiter */

	X        = popsp();
	delim[0] = (char) X;
	delim[1] = '\0';
	if(!not_first_time){
		(FREAD_WA)();       		/* read next line from stream */
		not_first_time = TRUE;
	}
Start_Over:
	if(flag){
		string = strtok((char*)0,delim);
	}else{
		flag   = TRUE;
		string = strtok(tib,delim);
	}
	if(!string){	/* read a line */
		(FREAD_WA)();       		/* read next line from stream */
		flag   = FALSE;
		goto Start_Over;
	}
	strcpy(&pad[1],string);
/*	strcat(pad,delim); */
	*pad         = strlen(string);
	pushsp((long)pad);
}
#endif
/***********************+------------+
|			| prim_token |
|			+------------+
| Token is just like word, except that it takes a string of
| delimiters instead of a character. This makes it easier to
| write SGML-type applications that use both > and blank as a
| token delimiter.
|
|	string " > "  token    ( 'string 'delimiters --- 'string1 )
|
| The returned string is null terminated.
|
| Note: This works, but I am not sure it is how I really want
|       it to.
*/
void prim_token()
{
	static int flag;

	long X;
	char *temp;
	char *string;
	char *delim;

	X        = popsp();
	delim    = (char*) X;
	X        = popsp();
	temp     = (char*) X;
/*
Start_Over:
*/
	if(flag){
		string = strtok((char*)0,delim);
	}else{
		flag   = TRUE;
		string = strtok(temp,delim);
	}
	if(!string){	/* read a line */
		flag   = FALSE;
	}
	pushsp((long)string);
}
/***********************+--------+
|			| erputs |
|			+--------+
*/
static void erputs(char* s)
{
        fputs(s,stderr);
}
/***********************+-------+
|			| oputs |
|			+-------+
*/
static void oputs(char* s)
{
        fputs(s,stdout);
}
/***********************+------------+
|			| init_vects |
|			+------------+
|  This function initializes the vectored I/O words.
*/
void init_vects()
{
	READ_WA             = (vectoredFunction)read_stdin;
	FREAD_WA            = (vectoredFunction)read_next_line;
	Semi_colon          = (vectoredFunction)normal_semi;
	number_vect         = (vectoredFunction)number;
	literal_vect        = (vectoredFunction)literal;
	open_stdin();

#ifdef VECTORED_IO
/* Add vector for printf  */
	dot_vect            = (vectoredFunction)dot;
	dot_quote_vect      = (vectoredFunction)dot_quote;
	g_dots_vect         = (vectoredFunction)gdot_s;
	dots_vect           = (vectoredFunction)dot_s;
	type_vect           = (vectoredFunction)type;
	emit_vect           = (vectoredFunction)emit;
	center_vect         = (vectoredFunction)center;
	prints_vect         = (vectoredFunction)prints;
	printf_vect         = (vectoredFunction)printf;
	space_vect          = (vectoredFunction)space;
	spaces_vect         = (vectoredFunction)spaces;
	cr_vect             = (vectoredFunction)cr;
	key_vect            = (vectoredFunction)key;
	query_terminal_vect = (vectoredFunction)query_terminal;
#endif
#ifdef VECTORED_C_IO
	stderr_vect         = (vectoredIOFunction)erputs;
	stdout_vect         = (vectoredIOFunction)oputs;
	stdin_vect          = (vectoredWordFunction)read_stdin;
#endif
}
/***********************+------+
|                       | cold |
|                       +------+
|	Note: If cold executed a second time, all memory from
|	      the first call is still allocated. Also the .APP file
|	      is not loaded again. This can cause real problems!!
*/
void cold()
{
	char string[80];

		/*
		| Initial allocation of buffers
		*/
	if ( tib )       /* assume if one exists, then they all do */
	  {              /* from previous call, clean up by removing them first */
                         /*  Skip */
	    free( tib );
            free( here_here );
	    free( pad );
	    free( pstack );
            free( rstack );
	  }

	tib               = (char *) malloc(TIBSIZE);
#ifdef CHECK_MALLOC
	if(!tib){
		printf("cold: Could not allocate tib.\n");
		abort_F();
		return;
	}
#endif
	here_here         = (char *) malloc(PADSIZE);
#ifdef CHECK_MALLOC
	if(!here_here){
		printf("cold: Could not allocate here.\n");
		abort_F();
		return;
	}
#endif
	pad               = (char *) malloc(PADSIZE);
#ifdef CHECK_MALLOC
	if(!pad){
		printf("cold: Could not allocate pad.\n");
		abort_F();
		return;
	}
#endif
	pstack            = (long *) malloc(PSTACKSIZE*sizeof(long));
#ifdef CHECK_MALLOC
	if(!pstack){
		printf("cold: Could not allocate Parameter Stack.\n");
		abort_F();
		return;
	}
#endif
	rstack            = (long *) malloc(RSTACKSIZE*sizeof(long));
#ifdef CHECK_MALLOC
	if(!rstack){
		printf("cold: Could not allocate Return Stack.\n");
		abort_F();
		return;
	}
#endif
		/*
		| Set flags
		*/
	ABORT_FLAG        = 0L;
	QUIT              = 0L;
	START             = FALSE;
	STATE             = EXECUTION_MODE;
		/*
 		| Initialize variables
		*/
	tib_len           = 0L;
	IN                = 0L;
	SP                = 0L;
	RS                = 0L;
	BASE              = 10;
	comma_data        = (char*)pfa_list;
		/*
		| Set up initial dictionaries and build primitives
		*/
	make_initial_vocs();
	init_vects();
#ifdef FLOAT_POINT
	float_init();
#endif
#ifdef SENGINE
	sengine_make_prims();
#endif
	make_prims();
        if ( user_cold_hook )
        	user_cold_hook();

#ifdef BCC_XXX
	clrscr();
#endif
#ifdef SENGINE_BANNER
	if(show_copy){
		center("S-Engine\n");
		center("Copyright 1994, 1995\n");
		center("by Norman E. Smith,\n");
		center("All Rights Reserved.\n\n");
		sprintf(string,"Version %s\n",CALC_VERSION);
		center(string);
		sprintf(string,"%s\n",DATE);
		center(string);
	}
#else
	if(show_copy){
		center("Until\n");
		center("Copyright 1995\n");
		center("by Norman E. Smith,\n");
		center("All Rights Reserved.\n\n");
		sprintf(string,"Version %s\n",CALC_VERSION);
		center(string);
		sprintf(string,"%s\n",DATE);
		center(string);
	}
#endif
	prompt();
}
/***********************+-------------+
|                       | do_variable |
|                       +-------------+
*/
void do_variable()
{
	pushsp((long) &WA->PFA.lvalue);
}
/***********************+-------------+
|                       | do_constant |
|                       +-------------+
*/
void do_constant()
{
	pushsp(WA->PFA.lvalue);
}
/***********************+---------+
|                       | do_word |
|                       +---------+
*/
void do_word(struct DictHeader *entry)
{
	(*entry->CFA)();
}
/********************+----------+
|                    | do_colon |
|                    +----------+
| This function loops through a PFA that contains a list of
| Word Addresses (WA) to execute. The last entry must be a
| NULL to flag end of addresses.
|
| This version of do_colon is executed from inside a Forth
| word. The word address is picked up from the global variable
| WA. I did it this way because I never figured out a way to
| stick a function in a table with arguments.
*/
void do_colon()
{
	register struct DictHeader *word;
	struct DictHeader **old_IP;

	old_IP = IP;                    /* Save curr IP+1 */
	IP     = WA->PFA.Waddr;
	WA     = 0;
	word   = *IP++;

	while(word){
		WA   = word;
		(*word->CFA)();
		word = *IP++;
	}
	IP = old_IP;
}
/***********************+------+
|                       | drop |
|                       +------+
|       drop    ( n --- )
*/
void drop()
{
	SP--;
	if(SP<0){
#ifndef VECTORED_C_IO
		fprintf(stderr,"P Stack underflow.");
#else
		sprintf(tstring,"P Stack underflow.");
		stderr_vect(tstring);
#endif
		SP = 0;
	}
}
/***********************+-----------+
|                       | exec_word |
|                       +-----------+
|
|  Execute a forth word from C. The word name is passed in as
|  an argument. Pad cannot be used because it gets trashed in
|  the null_to_counted conversion. So, string is used an an
|  intermediate area.
|
|  prim_find() uses pad to look up the word, and sets WA when
|  it is found. The pad string must be counted.
*/
void exec_word()
{
	int  status;

	status = prim_find();
	if(status){
		(*WA->CFA)();
	}else{
		pushsp((long)pad);
		count();
		type();
#ifndef VECTORED_C_IO
		fprintf(stderr," Word not found...\n");
#else
		sprintf(tstring," Word not found...\n");
		stderr_vect(tstring);
#endif
	}
}
/***********************+-------------------+
|                       | voc_thread_search |
|                       +-------------------+
|  This word searches the vocabulary passed via "vocab" for a word
|  name match with the counted string in pad. When a match occurs,
|  WA is set to the address of the word that matches. truth is
|  returned to indicate wether or not a match was found.
|
|  When the smudge flag is set, there is no match, even if the
|  string compare works. This prevents Forth from compiling
|  words that are not fully compiled.
|
|  "string" is assumed to be null terminated
|
|  On exit:
|	No match: WA     = 0
|		  Return = True
|	match:	  WA     = WA of matching word
|		  Return = False
*/
int voc_thread_search(struct DictHeader *vocab, char *string, long len)
{
	struct DictHeader *temp;
	int  truth = WORD_NOT_FOUND;
	int  status;
			/*
			| Search the dictionary starting at vocab
			| looking for a match.
			*/
	WA   = 0;
	temp = vocab;
	while(temp->LFA){
	   if((len    == temp->NFA->len)&&(temp->NFA->smudge != HIDDEN)){
		status = strncmp(string,temp->NFA->name,temp->NFA->len);
		if(status == 0){                    /* 0 means match */
			if(temp->NFA->smudge == HIDDEN){ /* Can't use smudged word */
				truth = WORD_NOT_FOUND;
				break;
			}else{
				truth = WORD_FOUND;
				break;
			}
		}
	   }
	   temp   = temp->LFA;
	}
	WA        = temp;
		/*
		| Change WORD_FOUND to WORD_IMMEDIATE for immediate
		| words.
		*/
	if(truth == WORD_FOUND){              /* set immediate flag */
		if(WA->NFA->immediate != NOT_IMMEDIATE){
			truth          = WA->NFA->immediate;
		}
	}
	return(truth);
}
/***********************+-----------+
|                       | prim_find |
|                       +-----------+
|  This word searches the dictionary for a word name match with
|  the counted string in pad. When a match occurs, WA is set to
|  the address of the word that matches. truth is returned to
|  indicate wether or not a match was found.
|
|  FALSE means a match.
|
|  When the smudge flag is set, there is no match, even if the
|  string compare works. This prevents Forth from compiling
|  words that are not fully compiled.
|
|  This function needs to match up on return values with minus_find():
|
|	Return: 0 = No match
|	        1 = Found, non immediate
|	       -1 = Found, immediate
|
|  Note: This version does not match the way Forth does is. It
|        should be used internally by primitives for increased
|        execution speed.
*/
int prim_find()
{
	int  truth = FALSE;
	int  len;
	char string[80];
			/*
			| Copy the word text from pad to string
			*/
	counted_to_null(pad,string);
	len = strlen(string);
			/*
			| Search the three vocabs.
			| CONTEXT, then CURRENT, and finally
			| FORTH vocabularies.
			*/
		/*
		| Search CONTEXT only if words already exist
		*/
	if(CONTEXT){
		truth = voc_thread_search(CONTEXT->PFA.DHaddr,string,len);
	}
		/*
		| Search CURRENT only if there is:
		|   o no match in CONTEXT
		|   o CURRENT contains words
		|   o CURRENT is not the same as CONTEXT
		*/
	if(!truth && (CONTEXT != CURRENT) && (CURRENT->PFA.DHaddr)){
		truth = voc_thread_search(CURRENT->PFA.DHaddr,string,len);
	}
		/*
		| Search FORTH only as a last resort.
		|   o No match on prev lookups
		|   o FORTH contains words
		|   o CURRENT != CONTEXT != FORTH
		*/
	if((!truth) && FORTH_WA
		    && ((CONTEXT != FORTH_WA)
			&&
		       (FORTH_WA != CURRENT))){
		truth = voc_thread_search(FORTH_WA->PFA.DHaddr,string,len);
	}
		/*
		| Still no match, check the vocabs...
		*/
	if(!truth){
		truth = voc_thread_search(VOC_LINK,string,len);
		if(truth){	/* found the vocab, so search it */
			VOC_FLAG = CONTEXT;
			CONTEXT  = WA;
		}
	}
	return(truth);
}
/***********************+---------------+
|                       | thread_search |
|                       +---------------+
|
|  This word searches a dictionary thread for a word name match.
|  The WA of a found word is pushed onto the parameter stack.
|
|  When the smudge flag is set, there is no match, even if the
|  string compare works. This prevents Forth from compiling
|  words that are not fully compiled.
|
|  Returns are:
|	WORD_FOUND     = Word was found
|	WORD_IMMEDIATE = Word found is immediate
|	WORD_NOT_FOUND = Not found
|
|  	The WA is also pushed onto the stack when a match is found.
*/
int thread_search(struct DictHeader *vocab, char *string, long len)
{
	struct DictHeader *temp;
	int  truth = WORD_NOT_FOUND;
	int  status;
			/*
			| Search the dictionary starting at DP
			| looking for a match. The first time through
			| searches CONTEXT, then CURRENT, and finally
			| FORTH vocabularies.
			*/
	temp = vocab;
			/*
			| Search the dictionary starting at DP
			| looking for a match.
			*/
	while(temp->LFA){
		if((len == temp->NFA->len) && (temp->NFA->smudge != HIDDEN)){
			status     = strncmp(string,temp->NFA->name,temp->NFA->len);
			if(status == 0){                /* 0 means match */
				truth = WORD_FOUND;
				pushsp((long) temp);
				if(temp->NFA->immediate != NOT_IMMEDIATE){
					truth = WORD_IMMEDIATE;	/* CANNOT use TRUE!! */
				}
				break;
			}
		}
		temp = temp->LFA;
	}
	return((long)truth);
}
/***********************+------------+
|                       | minus_find |
|                       +------------+
|       minus_find              ( 'string --- 'string 0)  Not found
|                               ( 'string --- WA -1 )     Found, normal
|                               ( 'string --- WA 1  )     Found, immediate
|
|  This word searches the dictionary for a word name match with
|  the counted string in pad.
|
|  When the smudge flag is set, there is no match, even if the
|  string compare works. This prevents Forth from compiling
|  words that are not fully compiled.
*/
void minus_find()
{
	char *addr;
	long len;
	int  truth = WORD_NOT_FOUND;
	char string[80];

	addr   = (char *)popsp();
	addr   = (char*)R_addr;
	counted_to_null(addr,string);
	len    = strlen(string);
		/*
		| Search CONTEXT only if words already exist
		*/
	if(CONTEXT->PFA.DHaddr){
		truth = thread_search(CONTEXT->PFA.DHaddr,string,len);
	}
		/*
		| Search CURRENT only if there is:
		|   o no match in CONTEXT
		|   o CURRENT contains words
		|   o CURRENT is not the same as CONTEXT
		*/
	if(!truth && (CONTEXT != CURRENT) && (CURRENT->PFA.DHaddr)){
		truth = thread_search(CURRENT->PFA.DHaddr,string,len);
	}
		/*
		| Search FORTH only as a last resort.
		*/
	if((!truth) && FORTH_WA
		    && (CONTEXT != FORTH_WA)
		    && (FORTH_WA != CURRENT)){
		truth = thread_search(FORTH_WA->PFA.DHaddr,string,len);
	}
		/*
		| Still no match, check the vocabs...
		*/
	if(!truth){
		truth = thread_search(VOC_LINK,string,len);
	}
	if(!truth){                     /* need orig addr if no match */
		pushsp((long) addr);
	}
	pushsp((long)truth);
}
/***********************+-----------------+
|                       | counted_to_null |
|                       +-----------------+
|
|  This word converts a string from counted form to null
|  terminated form.
*/
void counted_to_null(char *counted, char* null)
{
	int  len;

	len = *counted++;
	strncpy(null,counted,len);
	*(null+len) = '\0';
}
/***********************+--------+
|                       | rfetch |
|                       +--------+
|
|       The value of RS is not checked, so if there is nothing
|       on the rstack, a 0 is returned rather than an error. A
|       check will be added later if this turns out to be a
|       problem.
*/
void rfetch()
{
	long value;

	value = *(rstack + RS - 1);		/* get addr of cell */
	pushsp(value);                          /* put it on the stack */
}
/***********************+------+
|                       | to_r |
|                       +------+
| See about setting up a flag to show that >r was called. It might
| help cleaning it up in come cases...
*/
void to_r()
{
#ifdef READABLE
	long value;
	value = popsp();
	pushrs(value);                          /* put it on the stack */
#else
	pushrs(popsp());
#endif
}
/***********************+--------+
|                       | r_from |
|                       +--------+
|
|       r>      ( --- value)
*/
void r_from()
{
#ifdef READABLE
	long value;

	value = poprs();
	pushsp(value);                          /* put it on the stack */
#else
	pushsp(poprs());
#endif
}
/***********************+-----------------+
|                       | null_to_counted |
|                       +-----------------+
|
|  This word converts a string from null to counted form
|  terminated form. This function is called from other C functions
|  only.
*/
void null_to_counted(char *null, char* counted)
{
	int  len;
	char *original;

	original  = counted++;
	len       = strlen(null);
	strcpy(counted,null);
	*original = len;
}
/***********************+------------+
|                       | n_to_count |
|                       +------------+
|	'null 'counted null->counted
|
|  This word converts a string from null to counted form
|  terminated form.
*/
void n_to_count()
{
	char *null;
	char *counted;

	counted   = (char*) popsp();
	counted   = (char*) R_addr;
	null      = (char*) popsp();
	null      = (char*) R_addr;
	null_to_counted(null,counted);
}
/***********************+------------+****************************
|                       | to_counted |
|                       +------------+
|       >counted        ( addr --- addr len )
|
| This word converts a null terminated string to counted form
| on the stack.The original string is not modified.
*/
void to_counted()
{
	long   len;
	char   *string;

	string = (char*)popsp();
	string = (char*)R_addr;
	len    = strlen(string);
	pushsp((long)string);
	pushsp(len);
}
/***********************+---------+
|                       | to_null |
|                       +---------+
|       >null   ( 'counted 'null --- )
|
|  This word converts a string from counted form to null
|  terminated form.
*/
void to_null()
{
	int  i;
	int  len;
	char *counted;
	char *null;

	null    = (char*)popsp();
	counted = (char*)popsp();
	len     = *counted++;
	for(i=0;i<len;i++){
		*null++ = *counted++;
	}
	*null   = '\0';
}
/***********************+-------+
|                       | count |
|                       +-------+
|       count   ( addr --- addr len )
*/
void count()
{
	char *addr;
	int  len;

	addr = (char *)popsp();
	addr = (char*) R_addr;
	len  = *addr++;
	pushsp((long)addr);
	pushsp(len);
}
/***********************+-------+
|                       | fetch |
|                       +-------+
|
|       @       ( addr --- val )
*/
void fetch()
{
#ifdef READABLE
	long value;
	long *addr;

	addr  = (long*) popsp();               /* get addr of cell */
	addr  = (long*) R_addr;
	value = *addr;                          /* then the value */
	pushsp(value);                          /* put it on the stack */
#else
	pushsp(*(popsp()));
#endif
}
/***********************+-------+
|                       | store |
|                       +-------+
|
|       !       ( val addr --- )
*/
void store()
{
#ifdef READABLE
	long *addr;
	long value;

	addr  = (long*) popsp();
	addr  = (long*) R_addr;
	value = popsp();
	value = (long) R_addr;
	*addr = value;
#else
	long *addr;

	*addr  = (long*)popsp(popsp());
#endif
}
/***********************+----------+
|                       | prim_dup |
|                       +----------+
|       dup     ( n --- n n )
|
| This function is not named dup because there is a predefined C
| system function of that name.
|
*/
void prim_dup()
{
	long value;

	value = *(pstack + SP - 1);
	pushsp(value);
}
/***********************+---------+
|                       | two_dup |
|                       +---------+
|       2dup     ( n1 n2 --- n1 n2 n1 n2 )
|
| This function is not named dup because there is a predefined C
| system function of that name.
|
*/
void two_dup()
{
	over();
	over();
}
/***********************+------+
|                       | swap |
|                       +------+
|       swap    ( n1 n2 --- n2 n1 )
*/
void swap()
{
	long value;

	if(SP < 2){
#ifndef VECTORED_C_IO
		fprintf(stderr,"P Stack: not enough entries.");
#else
		sprintf(tstring,"P Stack: not enough entries.");
		stderr_vect(tstring);
#endif
		return;
	}
	value              = *(pstack + SP - 1);
	*(pstack + SP - 1) = *(pstack + SP - 2);
	*(pstack + SP - 2) = value;
}
/***********************+-----+
|                       | dot |
|                       +-----+
|       .       ( n1 --- )
|
|       11/20/92        dot() modified to output digits in bases
|                       8, 10, and 16. It should be re-written to
|                       output in any base automatically! [nes]
*/
void dot()
{
	long n1;

	n1 = popsp();
	switch(BASE){
	case 16:
#ifndef VECTORED_C_IO
		printf("%lx ",n1);
#else
		sprintf(tstring,"%lx ",n1);
		stdout_vect(tstring);
#endif
		break;
	case 8:
#ifndef VECTORED_C_IO
		printf("%lo ",n1);
#else
		sprintf(tstring,"%lo ",n1);
		stdout_vect(tstring);
#endif
		break;
	case 10:
	default:
#ifndef VECTORED_C_IO
		printf("%ld ",n1);
#else
		sprintf(tstring,"%ld ",n1);
		stdout_vect(tstring);
#endif
	}
}
/***********************+------+
|                       | zero |
|                       +------+
*/
void zero()
{
	pushsp((long)0);
}
/***********************+-----+
|                       | one |
|                       +-----+
*/
void one()
{
	pushsp((long)1);
}
/***********************+-----+
|                       | two |
|                       +-----+
*/
void two()
{
	pushsp((long)2);
}
/***********************+-------+
|                       | three |
|                       +-------+
*/
void three()
{
	pushsp((long)3);
}
/***********************+-------+
|                       | words |
|                       +-------+
| Type list of words in the current vocabulary.
| >> Note: Modify to look at system margin variables to figure
|          out how many columns to output.
*/
void words()
{
#ifdef BCC
	char       ch;
#endif
	char       string[80];
	struct DictHeader *temp;
	int        length;
	int        wnum = 0;
	int        wds  = 0;

	temp = CONTEXT->PFA.DHaddr;
#ifndef VECTORED_C_IO
	printf("\n");
#else
	stdout_vect("\n");
#endif
	while(temp->LFA){
		/* put while looking for a key press */
#ifdef BCC
		if(kbhit()){
#ifdef VECTORED_IO
		        key_vect();
		        ch       = (char)popsp();
                        if ( ch == ESC ) { return; }
			key_vect();  
                        ch       = (char)popsp();
#else
			key();   
			ch     = (char)popsp();
			if(ch == ESC){  return; }
			key();
			ch     = (char)popsp();
#endif
		}
#endif
		if(temp->NFA->smudge){          /* don't show smudged wds*/
			temp           = temp->LFA;
			continue;
		}
		if(temp->NFA->system){          /* don't show system only wds*/
			temp           = temp->LFA;
			continue;
		}
		length         = temp->NFA->len;
		strncpy(string,temp->NFA->name,length);
		string[length] = '\0';
#ifndef VECTORED_C_IO
		printf("%16s ",string);
#else
		sprintf(tstring,"%16s ",string);
		stdout_vect(tstring);
#endif
		wnum++;
		wds++;
		if(wnum>3){
			wnum = 0;
#ifndef VECTORED_C_IO
			printf("\n");
#else
			sprintf(tstring,"\n");
			stdout_vect(tstring);
#endif
		}
		temp           = temp->LFA;
	}
#ifndef VECTORED_C_IO
	printf("\n\n%d words\n",wds);
#else
	sprintf(tstring,"\n\n%d words\n",wds);
	stdout_vect(tstring);
#endif
}
/***********************+--------+
|                       | prompt |
|                       +--------+
*/
void prompt()
{
#ifndef VECTORED_C_IO
	printf("\n%s",PROMPT_STR);
#else
	sprintf(tstring,"\n%s",PROMPT_STR);
	stdout_vect(tstring);
#endif
}
/***********************+-----+
|                       | bye |
|                       +-----+
| Check outer loops to be sure this causes the main loop to fall
| through properly...
*/
void bye()
{
	QUIT       = 1;
	ABORT_FLAG = 1;
}
/***********************+---------+
|                       | abort_F |
|                       +---------+
|       abort   ( --- )
|
| Look at using setjmp()/longjmp() to accomplish this.
| In the mean time, close the current input file if reading from a
| file.
*/
void abort_F()
{
	ABORT_FLAG = 1;
	QUIT       = 0;
/* >> code here to check to see if a file is open  */
#ifndef VECTORED_C_IO
	fprintf(stderr,"Aborted\n");
#else
	sprintf(tstring,"Aborted\n");
	stderr_vect(tstring);
#endif
	  	/*
	  	| Supply the user with a way to cleanup if aborting
	  	*/
	if(user_abort_hook){
		user_abort_hook();
	}
	fflush(stderr);
}
/***********************+------+
|                       | warm |
|                       +------+
*/
void warm()
{
	tib_len      = 0L;
	IN           = 0L;
	SP           = 0L;
	RS           = 0L;
	STATE        = EXECUTION_MODE;
	QUIT         = 0L;
#ifdef FLOAT_POINT
	init_float_stack();
#endif
#ifdef CPP
/* Why not do this for all???  */
//	INPUT_SOURCE = 0L;
#endif
#ifndef VECTORED_C_IO
	printf("%s",the_warm_msg);
#else
	stdout_vect(the_warm_msg);
#endif
}
/***********************+----+
|                       | cr |
|                       +----+
|       cr      ( --- )
| Look at making this a macro instead of a function...
*/
void cr()
{
#ifndef VECTORED_C_IO
	printf("\n");
#else
	stdout_vect("\n");
#endif
}
/***********************+-------+
|                       | dot_s |
|                       +-------+
|       .s      ( --- )
*/
void dot_s()
{
	int  TSP;                        /* temp stack ptr */
	long n1;

	TSP = SP - 1;
	if(TSP < 0){
#ifndef VECTORED_C_IO
		printf("Stack Empty\n");
#else
		stdout_vect("Stack Empty\n");
#endif
		return;
	}
	while(TSP >= 0){
		n1 = *(pstack + TSP);
		TSP--;
#ifndef VECTORED_C_IO
		switch(BASE){
		case 16:
			printf("%lx ",n1);
			break;
		case 8:
			printf("%lo ",n1);
			break;
		case 10:
		default:
			printf("%ld ",n1);
		}
#else
		switch(BASE){
		case 16:
			sprintf(tstring,"%lx ",n1);
			stdout_vect(tstring);
			break;
		case 8:
			sprintf(tstring,"%lo ",n1);
			stdout_vect(tstring);
			break;
		case 10:
		default:
			sprintf(tstring,"%ld ",n1);
			stdout_vect(tstring);
		}
#endif
	}
}
/***********************+--------+
|                       | gdot_s |
|                       +--------+
|       .g      ( --- )
*/
#ifdef BCC
void gdot_s()
{
	static tdepth;                  /* Use to blank out prev display */
	int TSP;                        /* temp stack ptr */
	int row = 2;
	int col = 70;
	int orig_row;
	int orig_col;
		/*
		|  Save current screen position
		*/
	orig_row = wherey();
	orig_col = wherex();
		/*
		|  blank out prev display
		*/
	while(tdepth){
		gotoxy(col,row++);
#ifndef VECTORED_C_IO
		printf("          ");
#else
		stdout_vect("          ");
#endif
		tdepth--;
	}
		/*
		|  Do the stack print
		*/
	row    = 2;
	TSP    = SP - 1;
	tdepth = SP + 1;
	if(TSP < 0){
		gotoxy(col,row);
#ifndef VECTORED_C_IO
		printf("Stack Empty");
#else
		stdout_vect("Stack Empty");
#endif
		gotoxy(orig_col,orig_row);      /* restore cursor */
		tdepth = 0;
		return;
	}
	gotoxy(col,row++);
#ifndef VECTORED_C_IO
	printf("Stack      ");
#else
	stdout_vect("Stack      ");
#endif
	while(TSP >= 0){
		gotoxy(col,row++);
#ifndef VECTORED_C_IO
		printf("%ld\n",*(pstack + TSP--));
#else
		sprintf(tstring,"%ld\n",*(pstack + TSP--));
		stdout_vect(tstring);
#endif
	}
	if(ABORT_FLAG){
		row = 2;
		while(tdepth){
			gotoxy(col,row++);
#ifndef VECTORED_C_IO
			printf("          ");
#else
			stdout_vect("          ");
#endif
			tdepth--;
		}
	}
	gotoxy(orig_col,orig_row);              /* restore cursor */
}
#else
void gdot_s()
{
}
#endif
/***********************+-----------+
|                       | next_word |
|                       +-----------+
|
| This word is intended for use inside primitives only.
|
| ->Should go back later and code it all in low level C for
|   speed.
*/
void next_word()
{
	pushsp((long)BLANK);
	word();
	drop();
}
/***********************+--------+
|                       | do_lit |
|                       +--------+
|
| This is the run-time portion of literal.
*/
void do_lit()
{
	pushsp((long) *IP);
	IP++;
}
/***********************+------------+
|                       | do_literal |
|                       +------------+
|       literal ( n --- )
| NOTE: This looks like compile time action. Check USER.C. If this
|	is compile time, then change name to Compile_literal() and
|	move into CALL.C.
*/
void do_literal()
{
	pfa_list[pfa_offset++] = LIT_WA;
	WA                     = (struct DictHeader*)popsp();
	WA                     = (struct DictHeader*)R_addr;
	pfa_list[pfa_offset++] = WA;
}
/***********************+-----------+
|                       | dot_quote |
|                       +-----------+
|       ." string"
|
|  This is the run-time portion of ." and is valid only
|  in compile mode.
|
*/
void dot_quote()
{
	char *const_string;

	const_string = (char *)*IP++;
	const_string++;
#ifndef VECTORED_C_IO
	printf("%s",const_string);
#else
	stdout_vect(const_string);
#endif
}
/***********************+-------+
|                       | do_do |
|                       +-------+
| This function is the run-time for 'do'. 
*/
void do_do()
{
	to_r();
	if(*(pstack + SP - 1) > 0){		/* nes 05/24/95 */
		one_minus();
	}
	if(SP < 1){
#ifndef VECTORED_C_IO
		fprintf(stderr,"Stack is empty; do loops require FROM TO\n");
#else
		stderr_vect("Stack is empty; do loops require FROM TO\n");
#endif
		abort_F();
		return;
	}
#ifdef POS_DO
	if(*(pstack + SP - 1) < 0){		/* nes 02/01/95 */
#ifndef VECTORED_C_IO
		fprintf(stderr,"Loop arg must be > 0\n");
		fprintf(stderr,"arg is %d\n",(*(pstack + SP - 1)) + 1);
#else
		stderr_vect("Loop arg must be > 0\n");
		sprintf(tstring,"arg is %d\n",(*(pstack + SP - 1)) + 1);
		stderr_vect(tstring);
#endif
		abort_F();
		return;
	}
#endif
	to_r();
}
/***********************+------------+
|                       | do_zero_do |
|                       +------------+
| This function is the run-time for 'do'.
|
| >>>> Use for NEXT ... FOR
*/
void do_zero_do()
{
	pushrs(0);
	one_minus();
	if(SP < 1){
#ifndef VECTORED_C_IO
		fprintf(stderr,"Stack is empty; for requires a count\n");
#else
		stderr_vect("Stack is empty; for requires a count\n");
#endif
		abort_F();
		return;
	}
	if(*(pstack + SP - 1) < 1){
#ifndef VECTORED_C_IO
		fprintf(stderr,"Loop arg must be > 0\n");
#else
		stderr_vect("Loop arg must be > 0\n");
#endif
		abort_F();
		return;
	}
	to_r();
}
/***********************+---------+
|                       | do_loop |
|                       +---------+
| Part of do ... loop. This version of loop tests for exit at
| the bottom of the loop rather than at the top.
*/
void do_loop()
{
	long end;
	long index;

	end       = poprs();
	index     = poprs();
	if(index == end){
		IP++;
	}else{
		index++;
		pushrs(index);
		pushrs(end);
		branch();
	}
}
/***********************+--------------+
|                       | do_plus_loop |
|                       +--------------+
*/
void do_plus_loop()
{
	long end;
	long index;

	end       = poprs();
	index     = poprs();
	if(index == end){
		popsp();		/* 05/24/95 [nes] */
		IP++;
	}else{
		index += popsp();
		pushrs(index);
		pushrs(end);
		branch();
	}
}
/***********************+-------+
|                       | leave |
|                       +-------+
| Force do ... loop to exit the next time through.
|
| >>> Check to see if there is a way to leave immediately! Maybe
|     setjmp()/longjmp().
*/
void leave()
{
	long end;

	end   = poprs();
	poprs();
	pushrs(end);
	pushrs(end);
}
/***********************+-----------+
|                       | zero_bran |
|                       +-----------+
*/
void zero_bran()
{
	long truth;
	long offset;

	truth = popsp();                /* get truth from pstack */
	if(truth){
		IP++;                   /* skip branch offset */
		return;                 /* no branch, get out */
	}
	offset = (long)*IP;             /* mark branch offset point */
	IP    += offset;                /* jump to new code */
}
/***********************+--------+
|                       | branch |
|                       +--------+
*/
void branch()
{
	long offset;

	offset = (long)*IP;             /* mark branch offset point */
	IP    += offset;                /* jump to new code */
}
/***********************+-------+
|                       | quote |
|                       +-------+
|	" string"
|
|  This is the run-time portion of " and is valid only
|  in compile mode.
|  Counted or Null form of the string is determined at compile time...
*/
#ifdef IGNORE
void X_quote()
{
	unsigned long addr;
	addr = (unsigned long) *IP++;
	if(quote_null){
		addr++;
	}
	pushsp((long)addr);
}
#endif
/***********************+----------+
|                       | question |
|                       +----------+
|       ?       ( addr --- )
|
|  Same as @ .
*/
void question()
{
	fetch();
	dot();
}
/***********************+-----+
|                       | rot |
|                       +-----+
|       rot ( n1 n2 n3 --- n2 n3 n1)
*/
void rot()
{
	long n1, n2, n3;

	n3 = popsp();
	n2 = popsp();
	n1 = popsp();

	pushsp(n2);
	pushsp(n3);
	pushsp(n1);
}
/***********************+------+
|                       | do_i |
|                       +------+
*/
void do_i()
{
	pushsp((long)*(rstack + RS - 2));
}
/***********************+------+
|                       | do_j |
|                       +------+
*/
void do_j()
{
	if(RS < 4){
#ifndef VECTORED_C_IO
		fprintf(stderr,"J: Do loop not nested enough\n");
#else
		stderr_vect("J: Do loop not nested enough\n");
#endif
	}else{
		pushsp((long)*(rstack + RS - 4));
	}
}
/***********************+------+
|                       | do_k |
|                       +------+
*/
void do_k()
{
	if(RS < 6){
#ifndef VECTORED_C_IO
		fprintf(stderr,"K: DO LOOP NOT NESTED ENOUGH\n");
#else
		stderr_vect("K: DO LOOP NOT NESTED ENOUGH\n");
#endif
	}else{
		pushsp((long)*(rstack + RS - 6));
	}
}
/***********************+------+
|                       | type |
|                       +------+
| This word types a string to the screen.
|
|       type            ( addr len --- )
|
| NOTE: type() & ctype()are executed thru vector type_vec and
|	executed in exec_type().
*/
void type()
{
	int  i;
	int  len;
	char ch;
	char *string;

	len    = popsp();               /* get length */
	string = (char*) popsp();      /* get string addr */
	string = (char*) R_addr;
	for(i=0;i<len;i++){
		ch = *string++;
#ifndef VECTORED_C_IO
		printf("%c",ch);
#else
		sprintf(tstring,"%c",ch);
		stdout_vect(tstring);
#endif
	}
}
/***********************+--------+
|                       | c_type |
|                       +--------+
| This word types a string to the screen. It uses the console I/O
| functs so bold on/bold off, etc works. The vector is type_vec.
|
|       type            ( addr len --- )
*/
#ifdef IGNORE
void c_type()
{
	int  i;
	int  len;
	char ch;
	char *string;

	len    = popsp();               /* get length */
	string = (char *) popsp();      /* get string addr */
	string = (char*) R_addr;
	for(i=0;i<len;i++){
		ch = *string++;
#ifndef VECTORED_C_IO
		printf("%c",ch);
#else
		sprintf(tstring,"%c",ch);
		stdout_vect(tstring);
#endif
	}
}
#endif
/***********************+--------+
|			| prints |
|			+--------+
|	prints	( 'string --- )
|
| print a counted string.
*/
void prints()
{
	count();
	type();
}
/***********************+-----------+
|                       | do_smudge |
|                       +-----------+
|       smudge  ( WA --- )
|
| Typical usage is:
|       ' xxx smudge
*/
void do_smudge()
{
	struct DictHeader *the_word;

	the_word = (struct DictHeader*)popsp();
	the_word = (struct DictHeader*)R_addr;
	if(the_word->NFA->smudge){
		the_word->NFA->smudge = VISIBLE;
	}else{
		the_word->NFA->smudge = HIDDEN;
	}
}
/***********************+--------+
|                       | smudge |
|                       +--------+
| This is the C-level function that looks up the word in the dict
| then sets the smudge flag.
*/
void smudge(char *token)
{
	int not_found;

	null_to_counted(token,pad);
	not_found     = prim_find();
	if(not_found == 0){
		if(WA->NFA->smudge){
			WA->NFA->smudge = VISIBLE;
		}else{
			WA->NFA->smudge = HIDDEN;
		}
	}
}
/***********************+------+
|                       | hide |
|                       +------+
| This is the C-level function that looks up the word in the dict
| then sets the system flag.
*/
void hide(char *token)
{
	int found;

	null_to_counted(token,pad);
	found     = prim_find();
	if(found){
		WA->NFA->system = HIDDEN;
	}
}
/***********************+--------+
|                       | cfetch |
|                       +--------+
|
|       c@      ( addr --- char )
*/
void cfetch()
{
#ifdef READABLE
	char ch;
	char *addr;

	addr  = (char *) popsp();               /* get addr of cell    */
	ch    = *addr;                          /* then the value      */
	pushsp((long)ch);                       /* put it on the stack */
#else
	pushsp((char*)*(popsp()));
#endif
}
/***********************+--------+
|                       | cstore |
|                       +--------+
|
|       c!      ( val addr --- )
*/
void cstore()
{
	char *addr;
	char ch;

	addr  = (char *) popsp();
	ch    = popsp();
	*addr = ch;
}
/***********************+-----------+
|                       | minus_rot |
|                       +-----------+
|       -rot    ( n1 n2 n3 --- n3 n1 n2 )
*/
void minus_rot()
{
	long value1, value2, value3;

	value3 = popsp();
	value2 = popsp();
	value1 = popsp();

	pushsp(value3);
	pushsp(value1);
	pushsp(value2);
}
/***********************+-----+
|                       | key |
|                       +-----+
|       key     ( --- c )
|
| >>> Check out RCS changes...
*/
void key()
{
	char ch;

#ifdef BCC
	ch     = getch();
#endif
#if defined( SGI_INDY ) || defined( MIPS ) || defined( OSF ) || defined( SPARC )
	ch     = getchar();
#endif
#if defined( GCC ) || defined ( VAX )
	ch     = getchar();
#endif

	if(ch == EOF){                   /* test for eof  */
#ifndef VECTORED_C_IO
		fprintf(stderr,"key: EOF on STDIN");
#else
		stderr_vect("key: EOF on STDIN");
#endif
	}
	pushsp((long)ch);
}
/***********************+----------------+
|                       | query_terminal |
|                       +----------------+
|	?terminal  ( --- tf)
|
|>>> Check out RCS changes...
*/
void query_terminal()
{
#ifdef BCC
	pushsp(kbhit());
#else
	pushsp(NUMB 1);
#endif
}
/***********************+------+
|                       | emit |
|                       +------+
|       emit    ( ch --- )
|
| >>> Make vectored word
*/
void emit()
{
	char ch;

	ch = (char)popsp();
#ifndef VECTORED_C_IO
	printf("%c",ch);
#else
	sprintf(tstring,"%c",ch);
	stdout_vect(tstring);
#endif
}
/***********************+------+
|                       | dump |
|                       +------+
|       dump    ( addr len --- )
*/
void dump()
{
	int  i;
	unsigned long addr;
#ifdef SEG_ADDRESSING
	unsigned long seg_addr;
#endif
	unsigned long end_addr;
	unsigned long prt_addr;

	int  len;
	int  dump[20];

	unsigned char *buffer;
	unsigned char *buf2;

#ifndef VECTORED_C_IO
	printf("\n");
#else
	stdout_vect("\n");
#endif
#ifndef VECTORED_C_IO
	printf("Address   00 01 02 03 04 05 06 07");
#else
	stdout_vect("Address   00 01 02 03 04 05 06 07");
#endif
#ifndef VECTORED_C_IO
	printf("  08 09 0a 0b 0c 0d 0e 0f  ********  ********\n");
#else
	stdout_vect("  08 09 0a 0b 0c 0d 0e 0f  ********  ********\n");
#endif
	len      = popsp();
	addr     = (unsigned long)popsp();
	addr     = (unsigned long)R_addr;
	end_addr = addr + len;
	buffer   = (unsigned char *) addr;
	buf2     = buffer;

	for(;addr<end_addr;addr+=16){
		for(i=0;i<16;i++){
			dump[i] = (*buffer++ & 0xff);
		}
/*
| Need a working algorithm for this
*/
#ifdef SEG_ADDRESSING
		seg_addr = (addr & 0x0000ffff) << 4;
		prt_addr = addr & 0xffff;
		seg_addr = (addr | seg_addr);
		prt_addr = seg_addr;
#else
		prt_addr = addr;
#endif
#ifndef VECTORED_C_IO
		printf("%08lx> %02x %02x %02x %02x %02x %02x %02x %02x  %02x %02x %02x %02x %02x %02x %02x %02x",
			prt_addr, dump[0],  dump[1],  dump[2],  dump[3],
			      dump[4],  dump[5],  dump[6],  dump[7],
			      dump[8],  dump[9],  dump[10], dump[11],
			      dump[12], dump[13], dump[14], dump[15]);
#else
		sprintf(tstring,"%08lx> %02x %02x %02x %02x %02x %02x %02x %02x  %02x %02x %02x %02x %02x %02x %02x %02x",
			prt_addr, dump[0],  dump[1],  dump[2],  dump[3],
			      dump[4],  dump[5],  dump[6],  dump[7],
			      dump[8],  dump[9],  dump[10], dump[11],
			      dump[12], dump[13], dump[14], dump[15]);
		stdout_vect(tstring);
#endif

		for(i=0;i<16;i++){
			dump[i] = *buf2++;
			if(dump[i] < ' ' || dump[i] > 126){
				dump[i] = '.';
			}
		}
#ifndef VECTORED_C_IO
		printf("  %c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c\n",
			dump[0],  dump[1],  dump[2],   dump[3],
			dump[4],  dump[5],  dump[6],   dump[7],
			dump[8],  dump[9],  dump[10],  dump[11],
			dump[12], dump[13], dump[14],  dump[15]);
#else
		sprintf(tstring,"  %c%c%c%c%c%c%c%c  %c%c%c%c%c%c%c%c\n",
			dump[0],  dump[1],  dump[2],   dump[3],
			dump[4],  dump[5],  dump[6],   dump[7],
			dump[8],  dump[9],  dump[10],  dump[11],
			dump[12], dump[13], dump[14],  dump[15]);
		stdout_vect(tstring);
#endif
       }
}
/***********************+------+
|                       | tick |
|                       +------+
|       ' name  ( --- WA )
*/
void tick()
{
	int status;

	pushsp((long)BLANK);
	word();
	minus_find();
	status = popsp();
	if(status == 0){
		abort_F();
	}
}
/***********************+-------+
|                       | depth |
|                       +-------+
|       depth   ( --- n )
*/
void depth()
{
	pushsp(SP);
}
/***********************+----------+
|                       | addr_pad |
|                       +----------+
|       pad     ( --- 'pad )
*/
void addr_pad()
{
	pushsp((long) pad);
}
/***********************+-------+
|                       | to_in |
|                       +-------+
|       >in     ( --- 'IN )
*/
void to_in()
{
	pushsp((long)&IN);
}
/***********************+----------+
|                       | addr_tib |
|                       +----------+
|       tib     ( --- 'tib )
*/
void addr_tib()
{
	pushsp((long)tib);
}
/***********************+--------------+
|                       | display_word |
|                       +--------------+
| -->Look at a horizontal verstion of this word...
|    This word is useless on the PC without address decoding...
*/
void display_word()
{
	struct DictHeader *temp;
	int        status;

	pushsp(BLANK);
	word();
	minus_find();
	status = popsp();
	if(status){
		temp = (struct DictHeader *)popsp();
#ifndef VECTORED_C_IO
		printf("Word       Addr     CFA      LFA      PFA      NFA      smud  immed\n");
#else
		stderr_vect("Word       Addr     CFA      LFA      PFA      NFA      smud  immed\n");
#endif
/*
#ifndef VECTORED_C_IO
		printf("%-10s %12x %12x %12x %12x %12x %1d %1d\n",
			temp->NFA->name,temp,*temp->CFA,temp->PFA,
			temp->NFA, temp->NFA->smudge,temp->NFA->immediate);
#else
		sprintf(tstring,"%-10s %12x %12x %12x %12x %12x %1d %1d\n",
			temp->NFA->name,temp,*temp->CFA,temp->PFA,
			temp->NFA, temp->NFA->smudge,temp->NFA->immediate);
		stdout_vect(tstring);
#endif
*/
#ifndef VECTORED_C_IO
		printf("%-10s %12lx %1d\n", temp->NFA->name,&temp,temp->NFA->immediate);
#else
		sprintf(tstring,"%-10s %12lx %1d\n", temp->NFA->name,&temp,temp->NFA->immediate);
		stdout_vect(tstring);
#endif
	}
}
/***********************+--------+
|                       | v_word |
|                       +--------+
| This is messy, clean up!!!
*/
void v_word()
{
	struct DictHeader *temp;
	int        status;

	pushsp(BLANK);
	word();
	minus_find();
	status = popsp();
	if(status){
		temp = (struct DictHeader *)popsp();
#ifndef VECTORED_C_IO
		printf("Word Address: %ld\n",(unsigned long)temp);
#else
		sprintf(tstring,"Word Address: %ld\n",(unsigned long)temp);
		stdout_vect(tstring);
#endif
#ifndef VECTORED_C_IO
		printf("NFA         : %s\n",temp->NFA->name);
#else
		sprintf(tstring,"NFA         : %s\n",temp->NFA->name);
		stderr_vect(tstring);
#endif
#ifndef VECTORED_C_IO
		printf("  smudge    : %d\n",temp->NFA->smudge);
#else
		sprintf(tstring,"  smudge    : %d\n",temp->NFA->smudge);
		stdout_vect(tstring);
#endif
#ifndef VECTORED_C_IO
		printf("  immediate : %d\n",temp->NFA->immediate);
#else
		sprintf(tstring,"  immediate : %d\n",temp->NFA->immediate);
		stdout_vect(tstring);
#endif
#ifndef VECTORED_C_IO
		printf("CFA         : %ld\n",(unsigned long)temp->CFA);
#else
		sprintf(tstring,"CFA         : %ld\n",(unsigned long)temp->CFA);
		stdout_vect(tstring);
#endif
#ifndef VECTORED_C_IO
		printf("LFA         : %ld\n",(unsigned long)temp->LFA);
#else
		sprintf(tstring,"LFA         : %ld\n",(unsigned long)temp->LFA);
		stdout_vect(tstring);
#endif
#ifndef VECTORED_C_IO
		printf("PFA         : %ld\n",temp->PFA.lvalue);
#else
		sprintf(tstring,"PFA         : %ld\n",temp->PFA.lvalue);
		stdout_vect(tstring);
#endif
#ifndef VECTORED_C_IO
		printf("\n");
#else
		stdout_vect("\n");
#endif
	}
}
/***********************+----------+
|                       | do_comma |
|                       +----------+
|	NOTE: This implementation of , is not particuarily normal
|	for a Forth system. It is one of the tradeoffs made for
|	having a non-contigious dictionary. The value is compiled
|	into the temp compile buffer, pfa_list. When , is used
|	in non-compile mode, i.e. for building a table, something
|	must be done to copy pfa_list back into the dictionary.
|	I think >>> ] ; <<< will do it, but must do some heavy
|	duty testing to check in out...
|
|	>>> Also, the CFA needs to be changed from Variable to
|	Constant...
|
|	>>> Is there a Compile_comma()???
*/
void do_comma()
{

	long value;

		/*
		| It is not particularily efficient to set
		| the CFA to Constant every time, but effective...
		*/
	DP->CFA                = do_constant;  /* <- is this correct?? */
			/* The above line is not desirable 
			|  when , is used inside a definition or
			|  a <builds does> construct. NEEDS FURTHER STUDY
			*/
	value                  = popsp();
	pfa_list[pfa_offset++] = (struct DictHeader *)value;
		/*
		| >>> Where is comma_offset initialized???
		*/
	comma_offset          += sizeof(struct DictHeader *); /* keep in sync */
}
/***********************+------------+
|                       | do_c_comma |
|                       +------------+
|	NOTE: This implementation is the same as do_comma. This
|	is temporary. I have to figure out how to align byte values
|	in an array that is inherently addresses.
*/
void do_c_comma()
{
	long value;

		/*
		| It is not particularily efficient to set
		| the CFA to Constant every time, but effective...
		*/
	DP->CFA = do_constant;

	value = popsp();
	*(comma_data + comma_offset) = (char)value;
	comma_offset++;

	value = comma_offset % sizeof(struct DictHeader *);
	if(!value){		/* incr pfa_offset each mod 0 */
		pfa_offset++;
	}

}
/***********************+----------+
|			| do_align |
|			+----------+
|	This word performs alignment of pfa_offset in conjunction
|	with c_comma.
*/
void do_align()
{
	long value;

	pfa_offset = comma_offset / sizeof(struct DictHeader *);
	value      = comma_offset % sizeof(struct DictHeader *);
	if(value){
		pfa_offset++;
		comma_offset = pfa_offset * sizeof(struct DictHeader *);
	}
}
/***********************+---------------+
|			| pfa_list_addr |
|			+---------------+
*/
void pfa_list_addr()
{	pushsp((long)pfa_list);      }
/***********************+------------------+
|			| pfa_offset_value |
|			+------------------+
*/
void pfa_offset_value()
{	pushsp((long)pfa_offset);    }
/***********************+--------------------+
|			| comma_offset_value |
|			+--------------------+
*/
void comma_offset_value()
{	pushsp((long)comma_offset);  }
/***********************+------------+
|                       | number_tib |
|                       +------------+
*/
void number_tib()
{
	pushsp((long)tib_len);
}
/***********************+-----------+
|                       | dot_paren |
|                       +-----------+
|       .( message )
|
| Redo this in C
*/
void dot_paren()
{
	pushsp((long)')');
	word();
	count();
	type();
}
/***********************+-----------+
|                       | backslash |
|                       +-----------+
|       \ Comment to end of line
|
| >> This needs to be modified to work with the version of word that
|	calls strtok()...
*/
void backslash()
{
	IN = tib_len + 1;
}
/***********************+-----------+
|                       | query_dup |
|                       +-----------+
|       ?dup    ( n --- n n ) or ( 0 --- 0 )
*/
void query_dup()
{
	if(*(pstack + SP - 1)){
		pushsp(*(pstack + SP - 1));
	}
}
/***********************+-------+
|                       | query |
|                       +-------+
|       ?       ( addr --- )

*/
void query()
{
#ifdef READABLE
	long *addr;

	addr = (long *)popsp();
	addr = (long *)R_addr;
#ifndef VECTORED_C_IO
	printf("%ld ",*addr);
#else
	sprintf(tstring,"%ld ",*addr);
	stdout_vect(tstring);
#endif
#else
#ifndef VECTORED_C_IO
	printf("%ld ",*(popsp()));
#else
	sprintf(tstring,"%ld ",*(popsp()));
	stdout_vect(tstring);
#endif
#endif
}
/***********************+-------+
|                       | cmove |
|                       +-------+
|       cmove   ( from to n --- )
*/
void cmove()
{
	unsigned char *from;
	unsigned char *to;
	long          n;

	n    = popsp();
	to   = (unsigned char*) popsp();
	from = (unsigned char*) popsp();

	memcpy(to,from,n);
}
/***********************+---------+
|			| memmove |
|			+---------+
|	This version of memmove() supplied bye Michael Mundrane, 01/95.
|	The version of gcc he uses on a Sparc does not include a 
|	memmove() function.
|
|	This code supplied by Michael Mundrane.
*/
#ifdef NEED_MEMMOVE
char *memmove (char *d, char *s, int len)
{
   char *dptr = d;
   char *sptr = s;

   if (sptr < dptr && dptr < sptr + len)  /* test for overlap */
   {
      dptr += len;
      sptr += len;

      while (len--)                       /* copy backwards!  */
	 *--dptr = *--sptr;
   }
   else
   {
      while (len--)                       /* copy forwards    */
	 *dptr++ = *sptr++;
   }

   return d;
}
#endif
/***********************+----------+
|                       | cmove_up |
|                       +----------+
|       cmove>  ( from to n --- )
*/
void cmove_up()
{
	unsigned char *from;
	unsigned char *to;
	long          n;

	n    = popsp();
	to   = (unsigned char*) popsp();
	from = (unsigned char*) popsp();

	memmove(to,from,n);
}
/***********************+------+
|                       | fill |
|                       +------+
|       fill    ( addr n byte --- )
|
| Fill-memory beginning at ADDR with the character, CHAR, for #
| BYTES. FILL emulates the equivalent Forth word.
*/
void fill()
{
	char* string;
	int   bytes;
	char  character;
	int   i;

	character = (char) popsp();
	bytes     = popsp();
	string    = (char*)popsp();

	for(i=0;i<bytes;i++){
		*string++ = character;
	}
}
/***********************+------------+
|                       | prim_forth |
|                       +------------+
|       forth   ( --- )
*/
void prim_forth()
{  }
/***********************+----------+
|                       | forth_83 |
|                       +----------+
|       forth-83 ( --- )
*/
void forth_83()
{
#ifndef VECTORED_C_IO
	printf("Until is almost Forth-83 Compatable.\n");
#else
	stdout_vect("Until is almost Forth-83 Compatable.\n");
#endif
}
/***********************+------+
|                       | here |
|                       +------+
|       here    ( --- 'here )
*/
void here()
{
	pushsp((long)here_here);
}
/***********************+------+
|                       | over |
|                       +------+
|       over    ( n1 n2 --- n1 n2 n1 )
|
| >>> Do in C!!
*/
void over()
{
	long n1;
	long n2;

	n2 = popsp();
	n1 = popsp();

	pushsp(n1);
	pushsp(n2);
	pushsp(n1);
}
/***********************+------+
|                       | pick |
|                       +------+
|       pick    ( n --- x )
| dup nth pstack item to the top. 0 pick is dup. 1 pick is over.
|
| This version supplied by Michael Mundrane.
*/
void pick (void)
{
	unsigned long n = popsp();

	pushsp(pstack[SP-n-1]);
}
/***********************+------+
|                       | roll |
|                       +------+
|       roll    ( n --- x )
| Bring tne nth pstack item to the top. For example:
| 1 roll is swap and 2 roll is rot.
*/
void roll (void)
{
		/* This version supplied by Michael Mundrane */
   unsigned long n = popsp();

   if (SP > n)
   {
      long *sptr = &pstack[SP-n-1];
      long *dptr = sptr;
      long  tmp  = *sptr++;

      while (n--)                    /* roll me away */
         *dptr++ = *sptr++;

      *dptr = tmp;
   }
   else
   {
#ifndef VECTORED_C_IO
      fprintf(stderr,"P Stack underflow.");
#else
      stderr_vect("P Stack underflow.");
#endif
	abort_F();
   }
}
/***********************+-------+
|                       | space |
|                       +-------+
|       space   ( --- )
! >>> make space and spaces fectored to use cprintf
*/
void space()
{
#ifndef VECTORED_C_IO
	printf(" ");
#else
	stdout_vect(" ");
#endif
}
/***********************+--------+
|                       | spaces |
|                       +--------+
|       spaces  ( n --- )
*/
void spaces()
{
	long n;
	int  i;

	n = popsp();
	for(i=0;i<n;i++){
#ifndef VECTORED_C_IO
		printf(" ");
#else
		stdout_vect(" ");
#endif
	}
}
/***********************+----------------+
|                       | minus_trailing |
|                       +----------------+
| This word removes trailing blanks from the string.
| -TRAILING starts at the end of the string and 
| searches for the first non-space character.
*/
void minus_trailing()
{
	int   i;
	int   len;
	char *addr;
	char *end;

	len  = popsp();
	addr = (char*)popsp();

	end  = addr + len - 1;
	for(i=0;i<len;i++){
		if(*end == BLANK){
			end--;
		} else {
			pushsp((long) addr);
			pushsp(end - addr);
			return;
		}
	}
}
/***********************+------------+
|                       | prim_ftime |
|                       +------------+
*/
void prim_ftime()
{
#ifndef VAX
	time_t t;
	char   *string;

	t      = time(NULL);
	string = ctime(&t);
#ifndef VECTORED_C_IO
	printf("%s",string);
#else
	stdout_vect(string);
#endif

#else

/*
  "time_t" etc. comes from the file SYS$LIBRARY:TIME.H;  
  mktime() is prototyped in that file and has an entry 
  in the vaxcrtl, however it is NOT documented in the 
  VAX C Run-Time Library reference manual -- rcs    
*/

	struct tm *time_structure;
	char      *ascii_time;
	time_t    time_in_seconds;

	time(&time_in_seconds);
	time_structure = localtime(&time_in_seconds);
	ascii_time     = asctime(time_structure);
#ifndef VECTORED_C_IO
	printf ("%s",ascii_time);
#else
	stdout_vect(ascii_time);
#endif

#endif
}
/***********************+-------+
|                       | ascii |
|                       +-------+
|
*/
void ascii()
{
	char ch;

	next_word();
	ch = *(pad + 1);		/* extract the char */

	pushsp((long)ch);
	if(STATE==COMPILE_MODE){	/* in compile mode  */
		literal();		/* Add char to pfa_list */
		pfa_list[pfa_offset++] = WA;
	}
}
/***********************+--------+
|                       | forget |
|                       +--------+
| This was moved to CALL.C
*/
/***********************+------+
|                       | exit |
|                       +------+
| >>> See if there is a way to point IP to a null WA so it
|     will stop the current def...
*/
void f_exit()
{
/*      IP = NAME0_WA; */
}
/***********************+------------+
|                       | force_exit |
|                       +------------+
|
|	exit()
|
| Force immediate exit to the OS...
*/
void force_exit()
{
	long value;

	value = popsp();
	exit(value);
}
/***********************+--------+***************************
|                       | number |
|                       +--------+
|       number  ( addr --- n )
|
|       This is the easy way to implement number(). It does not
|       exactly adhere to traditional Forth, but should not cause
|       a problem 99.9% of the time because 0 is a word in the
|       dictionary. 0 should be found in the dict before getting
|       to number.  It may also not work correctly for bases
|       other than decimal.
|
|       The bottom line is this way is very efficient with minimal
|       possibility of incorrect side effects.
|
|       addr is a counted string.
|
|       NOTE: The global variable ABORT is set if an error in
|             conversion occurs.
|
|       ALTERNATIVE:    *(addr+len) = NULL;
|       This should work, but needs to be tested before being
|       put into the code.
*/
void number()
{
	char *addr;
	char nstring[32];
	long value;

	if((BASE != 10)){
		base_number();
		return;
	}
		/*
		|  Decimal numbers are fastest!! Uses C's base 10 number
		|  conversion.
		*/
	addr     = (char *) popsp();
	counted_to_null(addr,nstring);          /* Look at alternative  */
/*
|  Problem with C function is 2dup return 2!!!!
|  Need to add a check...
*/
	value    = atol(nstring);
	pushsp(value);
	if(value == 0){                         /* bad conversion       */
#ifndef VECTORED_C_IO
		fprintf(stderr,"%s ? \n",nstring);
		fprintf(stderr,"TIB: %s\n",tib);
#else
		sprintf(tstring,"%s ? \n",nstring);
		stderr_vect(tstring);
		sprintf(tstring,"TIB: %s\n",tib);
		stderr_vect(tstring);
#endif
		abort_F();                      /* should be abort_quote*/
	}
}
/***********************+------------+
|                       | do_cstring |
|                       +------------+
|       { xxxxx}
|
| This word can be used only interactively. The temp string is
| left at here.
*/
void X_do_cstring()
{
	long len;

	pushsp('}');
	word();
	drop();
	len = *pad;
	memcpy(here_here,pad+1,len);
	*(here_here + len) = 0;		/* NULL; */
}
/***********************+-----------+
|			| do_malloc |
|			+-----------+
| Call malloc() to allocate n bytes of memory. Use do_free() to
| return the memory to the OS.
*/
void do_malloc()
{
	long len;
	char *memory;

	len    = popsp();
	memory = (char*) malloc(len);
	pushsp((long)memory);
}
/***********************+------------+
|			| do_realloc |
|			+------------+
*/
void do_realloc()
{
	char* status;
	char* ptr;
	long  size;

	size   = (long)popsp();
	ptr    = (char*)popsp();

	status = (char*)realloc(ptr,size);
	if(!status){
#ifndef VECTORED_C_IO
		fprintf(stderr,"Could not realloc memory!\n");
#else
		stderr_vect("Could not realloc memory!\n");
#endif
	}
	pushsp((long)status);
}
/***********************+---------+
|			| do_free |
|			+---------+
| Free memory allocated by do_malloc().
*/
void do_free()
{
	char *memory;

	memory = (char*)popsp();
	free(memory);
}
/***********************+-------+
|                       | xargc |
|                       +-------+
*/
void xargc()
{
	pushsp(fargc);
}
/***********************+-------+
|                       | xargv |
|                       +-------+
*/
void xargv()
{
	pushsp((long)fargv);
}
/***********************+-------+
|                       | fence |
|                       +-------+
| This is a dummy word to stop forget().
*/
void fence()
{}
/***********************+--------------+
|			| return_errno |
|			+--------------+
|	( --- value ) errno
|
| This function returns the value of the C variable errno.
*/
void return_errno()
{
	pushsp((long)errno);
}
/***********************+---------------+
|			| set_show_copy |
|			+---------------+
|	( tf --- ) copyright
|
*/
void set_show_copy()
{
	long truth;

	truth      = popsp();
	show_copy  = truth;
}
