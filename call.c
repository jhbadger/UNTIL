/*
|       FILE: CALL.C
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
|       Date written: 30-SEPT-1991
|
|	DATE 		CHANGE
|	12/10/92	Modified all malloc calls to explicitly
|			cast the return value. This was done to correct
|			problems with DEC C on DECstations.
|	12/22/92	Added ['] and corrected '.
|	12/22/92	Modified allot to include length and offset
|			as frist two bytes of the malloc'd array.
|			This is the first step to allowing , and c,
|			to work. [nes]
|	12/24/92	Re-write : and ; to make ] work. [nes]
|	12/29/92	Corrected bug in [']. Made initial coding
|			of [compile]. [nes]
|	12/30/92        Tested [compile], fixed bug in immediate.
|			Coded compile; it appears to work, but should
|			be tested more. [nes]
|	02/21/92	Added and debugged does>. [nes]
|	07/05/93	Moved vocabulary words to VOCAB.C. [nes]
|	07/18/93	Made many minor changes to try & fix vocab
|			problems. [nes]
|	06/23/94	Changed Compile_quote to save string as Null
|			instead of being counted... [nes]
|	06/27/94	Added system flag to NFA. [nes]
|	01/16/95	Made cleanup changes suggested by Akira Kida,
|			akida@isd.hin.konica.co.jp. [nes]
|	05/06/95	Added CHECK_MALLOC code. [nes]
*/

#include "compiler.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#if defined( SGI_INDY ) || defined( SPARC ) || defined( OSF )
#include <malloc.h>
#endif


#ifdef ANSIC
#include <alloc.h>

#endif

#ifdef VAX
#include <file.h>
#endif

#include "until.h"
#include "functs.h"

/***********************+-----------+
|                       | CreateNFA |
|                       +-----------+
| Needs to be changed to insert filename/position info instead of 0
| for view...
*/
struct NameField *CreateNFA(char *wordname)
{
	int    len;
	void   *temp_name;
	struct NameField *newword;

	newword            = (struct NameField*) malloc(sizeof(struct NameField));
#ifdef CHECK_MALLOC
	if(!newword){
#ifndef VECTORED_C_IO
		fprintf(stderr,"CreateNFA: Could not allocate NameField.\n");
#else
		stderr_vect("CreateNFA: Could not allocate NameField.\n");
#endif
		abort_F();
		return(0);
	}
#endif

	len                = strlen(wordname);
	newword->len       = len;
	temp_name          = (void *)malloc(len+2);
#ifdef CHECK_MALLOC
	if(!temp_name){
#ifndef VECTORED_C_IO
		fprintf(stderr,"CreateNFA: Could not allocate wordname.\n");
#else
		stderr_vect("CreateNFA: Could not allocate wordname.\n");
#endif
		abort_F();
		return(0);
	}
#endif
	newword->name      = (char*)temp_name;
	newword->view      = 0;

	strcpy(newword->name,wordname);
	newword->smudge    = -1;
	newword->immediate = NOT_IMMEDIATE;
	return(newword);
}
/***********************+--------+
|                       | Create |
|                       +--------+
|  The word to create should be in pad. PAD is counted in the
|  Forth tradition. It must be converted to null terminated
|  for C to digest it properly. This is the generic "create a
|  word header"
|
*/
void Create()
{
	struct NameField  *nfa;
	struct DictHeader *entry;

	int  status;
	int  len;
	char string[80];

	entry = (struct DictHeader *) malloc(sizeof(struct DictHeader));
#ifdef CHECK_MALLOC
	if(!entry){
#ifndef VECTORED_C_IO
		fprintf(stderr,"Create: Could not allocate DictHeader.\n");
#else
		stderr_vect("Create: Could not allocate DictHeader.\n");
#endif
		abort_F();
		return;
	}
#endif

	len                   = *pad;
	strncpy(string,(pad + 1),len);
	string[len]           = '\0';
		/*
		| Is this call really needed? It is already done
		| immediately before several calls to Create().
		| >>Need to research all calls and see if this
		| call is redundant...
		*/
	status                = prim_find();    /* search dict for this word */
	if(status == 1){
#ifndef VECTORED_C_IO
		fprintf(stderr,"%s: Word redefined...\n",string);
#else
		sprintf(tstring,"%s: Word redefined...\n",string);
		stderr_vect(tstring);
#endif
	}

	nfa                   = CreateNFA(string);
	entry->NFA            = nfa;
	entry->NFA->smudge    = HIDDEN;
	entry->NFA->system    = VISIBLE;
	entry->NFA->immediate = NOT_IMMEDIATE;
	entry->CFA            = do_variable;
	entry->PFA.lvalue     = 0;
	entry->LFA            = DP;
	DP                    = entry;
	LATEST                = DP;
	CURRENT->PFA.DHaddr   = DP;
}
/***********************+-------------+
|                       | forget_word |
|                       +-------------+
| This word removes the word whose address is passed.
|
|
|
|
*/
#define VARIABLE        1
#define CONSTANT        2
#define COLON           3
#define ALLOT           4
#define VOCAB		5

void forget_word(struct DictHeader *word)
{
	struct DictHeader *next_word;
	int  code_fld;
	char string[80];
		/*
		| Save link to next word; it will have to be
		| modified at the end...
		*/
	next_word = word->LFA;
		/*
		| Figure out what type of word this is...
		*/
	if(word->CFA == do_variable)    { code_fld = VARIABLE; } else
	if(word->CFA == do_constant)    { code_fld = CONSTANT; } else
	if(word->CFA == do_colon)       { code_fld = COLON;    } else
	if(word->CFA == do_allot)       { code_fld = ALLOT;    } else
	if(word->CFA == do_vocabulary)  { code_fld = VOCAB;    } else{
#ifndef VECTORED_C_IO
		fprintf(stderr,"Cannot forget %s!!\n",word->NFA->name);
#else
		sprintf(tstring,"Cannot forget %s!!\n",word->NFA->name);
		stderr_vect(tstring);
#endif
		abort_F();
		return;
	}
		/*
		| Free up the memory from the Name Field Address
		| first.
		*/
	free(word->NFA->name);
	free(word->NFA);
		/*
		| Now, handle the PFA. It can be value or ptr.
		| Check the word type. Variables and constants are
		| values, colon defs and allots are arrays.
		| (This figures out whether or not to free an array.)
		*/
	switch(code_fld){
	case VARIABLE:
	case CONSTANT:
			/*
			| Nothing to free, simply a value
			*/
		break;
	case COLON:     /* This may need to be Waddr */
	case ALLOT:
			/*
			| These are ptrs that must be freed...
			*/
		free(word->PFA.c_ptr);
		break;
	case VOCAB:
			/*
			| Need to add this code....
			*/
	default:
#ifndef VECTORED_C_IO
		fprintf(stderr,"Error, could not free memory...\n");
#else
		stderr_vect("Error, could not free memory...\n");
#endif
	}
		/*
		| And last, but not least...
		| give back the struct...
		*/
	free(word);
		/*
		| Prune the word from the dictionary
		*/
	DP                  = next_word;
	LATEST              = DP;
	CURRENT->PFA.DHaddr = DP;
}
/***********************+--------+
|                       | forget |
|                       +--------+
|
*/
void forget()
{
	int    keep_truckin;
	struct DictHeader *the_fence;
	struct DictHeader *word_addr;

	char   the_word[MAX_NAME];
		/*
		| The first thing to do is search for the word;
		| if not found, do error, then quit
		*/
	pushsp((long)BLANK);
	word();
	drop();
	strcpy(the_word,(pad + 1));	/* word stores it at pad */
	word_addr = set_WA(the_word);
	if(!word_addr){
/*
#ifndef VECTORED_C_IO
                fprintf(stderr,"Word not found\n");
#else
                stderr_vect("Word not found\n");
#endif
*/
		abort_F();
		return;
	}
		/*
		| Next, find "fence". If not found or > word addr,
		| it is an error. You can't forget past fence...
		*/
	the_fence = set_WA("fence");
	if(!the_fence){
#ifndef VECTORED_C_IO
		fprintf(stderr,"fence not found\n");
#else
		stderr_vect("fence not found\n");
#endif
		abort_F();
		return;
	}
		/*
		| Need to be able to see when we get past fence.
		| and therefore not forget any words. Testing
		| the addresses does not seem to do the trick!
		*/

		/*
		| Now that everything is hunky-dory, loop forgetting
		| each word until the one to forget...
		*/
	FOREVER{
		keep_truckin = strcmp(the_word,DP->NFA->name);
		if(keep_truckin){       /* no match, trash it */
			forget_word(DP);
			if(ABORT_FLAG){ break; }
		}else{          /*
				| Forget the word, then get out of loop
				*/
			forget_word(DP);
			break;
		}
	}
}
/***********************+---------------+
|                       | Compile_colon |
|                       +---------------+
|  The word to create should be in pad. PAD is counted in the
|  Forth tradition. It must be converted to null terminated
|  for C to digest it properly.
|
|  Compile_colon takes a list of DictHeader pointers and inserts
|  it into the PFA. Create() is called first to allocate a skeleton
|  word. This word then fills in the CFA and PFA to make it a
|  high level Forth word.
|
|  This word essentually creates a high level Forth Word.
*/
void Compile_colon()
{
			/*
			| Create skeleton word header
			*/
	if(STATE == COMPILE_MODE){
#ifndef VECTORED_C_IO
		fprintf(stderr,"ERROR->Already in Compile Mode.\n");
#else
		stderr_vect("ERROR->Already in Compile Mode.\n");
#endif
	}
	next_word();
	Create();
	STATE = COMPILE_MODE;	/* flag compile mode */
			/*
			| Fill in the rest of the word.
			*/
	DP->NFA->smudge    = VISIBLE;
	DP->NFA->immediate = NOT_IMMEDIATE;
	DP->CFA            = do_colon;
	DP->PFA.Waddr      = (struct DictHeader**)0;

	pfa_offset         = FIRST_CELL;
	comma_offset       = FIRST_CELL;
	build_pfa_list();       /* lookup words; build pfa list */
}
/***********************+------------------+
|                       | Compile_variable |
|                       +------------------+
|  The word to create should be in pad. PAD is counted in the
|  Forth tradition. It must be converted to null terminated
|  for C to digest it properly.
|
|  Compile_variable takes a list of DictHeader pointers and inserts
|  it into the PFA. Create() is called first to allocate a skeleton
|  word.
|
*/
void Compile_variable()
{
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
	DP->CFA            = do_variable;
	DP->PFA.lvalue     = 0;
			/*
			| Should these go into Create() instead of here???
			*/
	comma_offset       = FIRST_CELL;
	pfa_offset         = FIRST_CELL;
}
/***********************+------------------+
|                       | Compile_constant |
|                       +------------------+
|  The word to create should be in pad. PAD is counted in the
|  Forth tradition. It must be converted to null terminated
|  for C to digest it properly.
|
|  Compile_variable takes a list of DictHeader pointers and inserts
|  it into the PFA. Create() is called first to allocate a skeleton
|  word. The value to initialize the constant with is on the stack.
|
*/
void Compile_constant()
{
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
	DP->CFA            = do_constant;
	DP->PFA.lvalue     = popsp();
}
/***********************+----------------+
|                       | build_pfa_list |
|                       +----------------+
*/
void build_pfa_list()
{
	int  status;
	char *string;
	struct DictHeader *save_WA;

	save_WA    = WA;
		/*
		|  Get next token from input stream.
		*/
	next_word();
		/*
		|  Loop through tokens, compiling them into
		|  dictionary. Continue until state is 0;
		*/
	while(STATE){
		status = prim_find();   /* search dict for this word */
		switch(status){
		case WORD_IMMEDIATE:    /* This is an immediate word */
			(WA->CFA)();    /* execute the immed word */
				/*
				| Check to see if immed word turned off
				| compile mode. If it did, get out of loop
				*/
			if(STATE == EXECUTION_MODE){
				goto FinishDef;
			}
			break;
		case WORD_NOT_FOUND:            /* make sure can find the work */
			pushsp((long)pad);
			number();               /* Try to convert it to a # */
			if(ABORT_FLAG){         /* Nope, an error */
					/*
					| type the error msgs
					*/
				string = pad + 1;
#ifndef VECTORED_C_IO
				fprintf(stderr,
					"%s<- Word Not Found in Line\n",string);
				fprintf(stderr,"%s",tib);
#else
				sprintf(tstring,"%s<- Word Not Found in Line\n",string);
				stderr_vect(tstring);
				stderr_vect(tib);
#endif
					/*
					| Hide the bad def from the world
					*/
				WA->NFA->smudge = HIDDEN;
					/*
					| Reset Compile Mode
					*/
				STATE  = EXECUTION_MODE;
				abort_F();
				break;
			}else{                  /* Got a number */
				literal();      /* compile it into dict */
			}                       /* BREAK left out on purpose! */
		case WORD_FOUND:                /* word found... */
			pfa_list[pfa_offset++] = WA;
			if(VOC_FLAG){
				CONTEXT        = VOC_FLAG;
				VOC_FLAG       = 0;
			}
			break;
		default:
#ifndef VECTORED_C_IO
			fprintf(stderr,
				"Invalid prim_find return in build_pfa_list\n");
#else
		        stderr_vect("Invalid prim_find return in build_pfa_list\n");
#endif
		}
		if(pfa_offset > PFA_SIZE){
#ifndef VECTORED_C_IO
			fprintf(stderr,"Definition too long. \n");
#else
			stderr_vect("Definition too long. \n");
#endif
			abort_F();
			STATE = EXECUTION_MODE;
		}
		next_word();
	}
FinishDef:
	WA         = save_WA; /* restore original WA */
}
/***********************+--------+
|                       | set_WA |
|                       +--------+
| This word is used only at setup to get a WA for use
| later by the compiler.
*/
struct DictHeader* set_WA(char* string)
{
	long len;
	long status;

	strcpy((pad + 1),string);
	len    = strlen(string);
	*pad   = len;
	status = prim_find();
	if(!status){
#ifndef VECTORED_C_IO
		fprintf(stderr,"%s: Word Not Found.\n",string);
#else
		sprintf(tstring,"%s: Word Not Found.\n",string);
		stderr_vect(tstring);
#endif
		return(0);
	}
	return(WA);
}
/***********************+-----------+
|                       | immediate |
|                       +-----------+
*/
void immediate()
{
	DP->NFA->immediate = WORD_IMMEDIATE;
}
/***********************+----------------+
|			| flush_pfa_list |
|			+----------------+
|	This function attaches the contents of pfa_list to
|	the latest word in the dictionary.
*/
void flush_pfa_list(long offset)
{
	void *ptr;
	int  len;

	pfa_list[pfa_offset] = 0;       /* pfa_list is null terminated */

	len   = pfa_offset;
	len  += 4;
		/*
		| Allocate memory for the pfa list. Space for
		| a trailing null is included.
		*/
#ifdef CPP
	ptr   = new DictHeader** [len*sizeof(DictHeader*)];
#else
	ptr   = (void *) malloc (len*sizeof(struct DictHeader*));
#ifdef CHECK_MALLOC
	if(!ptr){
#ifndef VECTORED_C_IO
		fprintf(stderr,"flush_pfa_list: Could not allocate DictHeader*.\n");
#else
		stderr_vect("flush_pfa_list: Could not allocate DictHeader*.\n");
#endif
		abort_F();
		return;
	}
#endif
#endif
		/* Make a var for len*sizeof...
		| move pfa list into perm location
		*/
	DP->PFA.Waddr = (struct DictHeader**)ptr;
	memcpy(ptr,&pfa_list[offset],(len*sizeof(struct DictHeader*)));
}
/***********************+------+
|                       | semi |
|                       +------+
|  NOTE: semi is vectored. Semi must perform different actions
|	 for normal words than for defining words. Defining words
|	 contain does>. The default function value is default_semi().
*/
void semi()
{
	(*Semi_colon)();
}
/***********************+-------------+
|                       | normal_semi |
|                       +-------------+
|	This is the default version of semi
*/
void normal_semi()
{
	if(!STATE){
#ifndef VECTORED_C_IO
		printf("ERROR-> Not in Compile Mode.\n");
#else
		stderr_vect("ERROR-> Not in Compile Mode.\n");
#endif
	}else{
			/*
			| offset is 1 to leave room for potential vocab
			| address in [0].
			*/
		flush_pfa_list(1);
		STATE = EXECUTION_MODE;         /* Turn off compile mode */
	}
			/*
			| Make sure the case statement was closed...
			*/
	if(In_case){
#ifndef VECTORED_C_IO
		printf("ERROR-> Case statement not closed.\n");
#else
		stderr_vect("ERROR-> Case statement not closed.\n");
#endif
	}
}
/***********************+-----------+
|                       | does_semi |
|                       +-----------+
|	This version of semi is specific to defining words. A defining
|	word contains does>.
*/
void does_semi()
{
	struct DictHeader **new_pfa;

	if(!STATE){
#ifndef VECTORED_C_IO
		printf("ERROR-> Not in Compile Mode.\n");
#else
		stderr_vect("ERROR-> Not in Compile Mode.\n");
#endif
	}else{
		STATE = EXECUTION_MODE;         /* Turn off compile mode */
		flush_pfa_list(1);
			/*
			| Search for does pointer address and put in
			| first byte
			*/
		new_pfa = DP->PFA.Waddr;
			/*
			| Search pfa_list for occurrance of do_does
			| to know where to plug offset...
			*/
		while(new_pfa++ != (struct DictHeader**)COMPILE_DO_DOES_WA){;}
		*new_pfa   = (struct DictHeader*)new_pfa + 2;
/*
|
|                    +-------+
|                    |       v
|   +-------+------+----+---+--------------------+
|   | stuff | DOES |    | 0 | DOES run-time code |
|   +-------+------+----+---+--------------------+
|                    ^
|                    +becomes an absolute addr
*/
							/* reset semi vector */
		Semi_colon = (vectoredFunction)normal_semi;
	}
}
/***********************+--------+
|                       | parens |
|                       +--------+
|       ( text )
|
| ( starts a comment that is terminated with ).
*/
void parens()
{
	pushsp(')');
	word();
	drop();
}
/***********************+---------+
|                       | literal |
|                       +---------+
|       literal ( n --- )
*/
void literal()
{
	pfa_list[pfa_offset++] = LIT_WA;
	WA                     = (struct DictHeader*)popsp();
}
/***********************+------------+
|                       | Compile_if |
|                       +------------+
*/
void Compile_if()
{
	pfa_list[pfa_offset++] = ZERO_BRAN_WA;
	pushrs((long)&pfa_list[pfa_offset++]);
	WA = 0;
}
/***********************+---------------+
|                       | Compile_endif |
|                       +---------------+
| This word 
*/
void Compile_endif()
{
	struct DictHeader **branch;
	long       offset;

	r_from();
	offset  = popsp();
	branch  = (struct DictHeader**) offset;
	offset  = (long) &pfa_list[pfa_offset] - offset;
	offset  = offset / sizeof(struct DictHeader*);
	*branch = (struct DictHeader*) offset;
	WA      = 0;
}
/***********************+--------------+*********************
|                       | Compile_then |
|                       +--------------+
| This word is identical to Compile_endif. Need to remove one
| and use the other for both then and endif...
*/
void Compile_then()
{
	struct DictHeader **branch;
	long              offset;

	r_from();
	offset  = popsp();
	branch  = (struct DictHeader**) offset;
	offset  = (long) &pfa_list[pfa_offset] - offset;
	offset  = offset / sizeof(struct DictHeader*);
	*branch = (struct DictHeader*) offset;
	WA      = 0;
}
/***********************+--------------+
|                       | Compile_else |
|                       +--------------+
*/
void Compile_else()
{
	struct DictHeader **branch;
	long       offset;
		/*
		| Branch around the ELSE part and set up
		| code to jump to the then
		*/
	r_from();                       /* save addr of the if */
	pfa_list[pfa_offset++] = BRANCH_WA;
	pushrs((long)&pfa_list[pfa_offset++]);
		/*
		| Note: This code is really THEN with the r_from() left
		|       out...
		*/
	offset  = popsp();
	branch  = (struct DictHeader**) offset;
	offset  = (long) &pfa_list[pfa_offset] - offset;
	offset  = offset / sizeof(struct DictHeader*);
	*branch = (struct DictHeader*) offset;
	WA = 0;
}
/***********************+------------+
|                       | Compile_do |
|                       +------------+
*/
void Compile_do()
{
	pfa_list[pfa_offset] = DO_WA;
	pushrs((long)&pfa_list[pfa_offset++]);
}
/***********************+-------------+
|                       | Compile_for |
|                       +-------------+
*/
void Compile_for()
{
	pfa_list[pfa_offset] = DO_FOR_WA;
	pushrs((long)&pfa_list[pfa_offset++]);
}
/***********************+--------------+
|                       | Compile_loop |
|                       +--------------+
*/
void Compile_loop()
{
	long       offset;

	pfa_list[pfa_offset++] = LOOP_WA;
	r_from();
	offset  = popsp();
	offset  = offset - (long) &pfa_list[pfa_offset];
	offset  = (offset / sizeof(struct DictHeader*)) + 1;
	pfa_list[pfa_offset++] = (struct DictHeader*) offset;
	WA      = 0;
}
/***********************+-------------------+
|			| Compile_plus_loop |
|			+-------------------+
*/
void Compile_plus_loop()
{
	long       offset;

	pfa_list[pfa_offset++] = PLUS_LOOP_WA;
	r_from();
	offset  = popsp();
	offset  = offset - (long) &pfa_list[pfa_offset];
	offset  = (offset / sizeof(struct DictHeader*)) + 1;
	pfa_list[pfa_offset++] = (struct DictHeader*) offset;
	WA      = 0;
}
/***********************+---------------+
|                       | Compile_begin |
|                       +---------------+
*/
void Compile_begin()
{
	pushrs((long)&pfa_list[pfa_offset]);
}
/***********************+---------------+
|                       | Compile_until |
|                       +---------------+
*/
void Compile_until()
{
	long offset;
	long curr_addr;

	pfa_list[pfa_offset++] = ZERO_BRAN_WA;
	r_from();
	offset                 = popsp();
	curr_addr              = (long) &pfa_list[pfa_offset];
	offset                 = offset - curr_addr;
	offset                 = offset / sizeof(struct DictHeader*);
	pfa_list[pfa_offset++] = (struct DictHeader*)offset;
	WA                     = 0;
}
/***********************+---------------+
|                       | Compile_while |
|                       +---------------+
*/
void Compile_while()
{
	pfa_list[pfa_offset++] = ZERO_BRAN_WA;
	pushrs((long)&pfa_list[pfa_offset++]);
	WA = 0;
}
/***********************+----------------+
|                       | Compile_repeat |
|                       +----------------+
*/
void Compile_repeat()
{
	struct DictHeader **branch;
	long       offset;
	long       while_addr;
	long       begin_addr;
	long       curr_addr;

	pfa_list[pfa_offset++] = BRANCH_WA;
	while_addr  = poprs();
	begin_addr  = poprs();
		/*
		| Compile offset to BEGIN into dictionary
		*/
	curr_addr              = (long) &pfa_list[pfa_offset];
	offset                 = begin_addr - curr_addr;
	offset                 = offset / sizeof(struct DictHeader*);
	pfa_list[pfa_offset++] = (struct DictHeader*)offset;
		/*
		| Comile offset for WHILE into Dict
		*/
	branch  = (struct DictHeader**) while_addr;
	offset  = (long) &pfa_list[pfa_offset] - while_addr;
	offset  = offset / sizeof(struct DictHeader*);
	*branch = (struct DictHeader*) offset;

	WA = 0;
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
void X_Compile_quote()
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
		fprintf(stderr,"Compile_quote: Could not allocate memory.\n");
#else
		stderr_vect("Compile_quote: Could not allocate memory.\n");
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
/***********************+-------------------+
|                       | Compile_dot_quote |
|                       +-------------------+
*/
void Compile_dot_quote()
{
	char *const_string;
	long len;

	pushsp('\"');
	word();
	drop();
	pfa_list[pfa_offset++] = DOT_QUOTE_WA;
	len = *pad;
	len+= 2;
#ifdef CPP
	const_string = new char [len+2];
#else
	const_string = (char*)malloc(len+2);
#ifdef CHECK_MALLOC
	if(!const_string){
#ifndef VECTORED_C_IO
		fprintf(stderr,"Compile_dot_quote: Could not allocate string.\n");
#else
		stderr_vect("Compile_dot_quote: Could not allocate string.\n");
#endif
		abort_F();
		return;
	}
#endif
#endif
	memcpy(const_string,pad,len);
	*(const_string + len - 1) = '\0';
	pfa_list[pfa_offset++]    = (struct DictHeader*) const_string;
	WA = 0;
}
/***********************+---------------------+
|                       | Compile_abort_quote |
|                       +---------------------+
*/
void Compile_abort_quote()
{
	char *const_string;
	long len;

	pushsp('\"');
	word();
	drop();
	pfa_list[pfa_offset++] = ABORT_QUOTE_WA;
	len = *pad;
	len+= 2;
#ifdef CPP
	const_string = new char [len+2];
#else
	const_string = (char*)malloc(len+2);
#ifdef CHECK_MALLOC
	if(!tib){
#ifndef VECTORED_C_IO
		fprintf(stderr,"Compile_abort_quote: Could not allocate memory.\n");
#else
		stderr_vect("Compile_abort_quote: Could not allocate memory.\n");
#endif
		abort_F();
		return;
	}
#endif
#endif
	memcpy(const_string,pad,len);
	*(const_string + len - 1) = 0;		/* NULL;  */
	pfa_list[pfa_offset++] = (struct DictHeader*) const_string;
	WA = 0;
}
/***********************+-----------+
|                       | do_create |
|                       +-----------+
|       create  ( --- )
|
|       Usage:
|               create junk
|
|       creates an empty word definition named junk
*/
void do_create()
{
	next_word();
	Create();
	DP->NFA->smudge = VISIBLE;
	pfa_offset      = FIRST_CELL;		/* added for , c, */
	comma_offset    = FIRST_CELL;
}
/***********************+-------------+
|                       | Compile_i() |
|                       +-------------+
*/
void Compile_i()
{
	pfa_list[pfa_offset++] = I_WA;
}
/***********************+-----------+
|			| Compile_j |
|			+-----------+
*/
void Compile_j()
{
	pfa_list[pfa_offset++] = J_WA;
}
/***********************+-----------------+
|                       | build_primitive |
|                       +-----------------+
|
*/
void build_primitive(char* string,void (*prim)())
{
	int not_found;
		/*
		|  The first thing to do is convert a C null terminated
		|  string into Forth counted form.
		*/
	null_to_counted(string,pad);
		/*
		|  Search the dict and flag a match for duplicate
		|  word definitions. FALSE means the word was found.
		*/
	not_found = prim_find();
		/*
		|  Create a word header, then
		|  set the CFA to prim and
		|  clear the smudge flag
		*/
	if(not_found == 0){
		Create();
		DP->CFA         = prim;
		DP->NFA->smudge = VISIBLE;
		DP->NFA->view   = PRIM;   
	}
}
/***********************+-------------+
|                       | build_iprim |
|                       +-------------+
|
*/
void build_iprim(char* string,void (*prim)())
{
	int not_found;
		/*
		|  The first thing to do is convert a C null terminated
		|  string into Forth counted form.
		*/
	null_to_counted(string,pad);
		/*
		|  Search the dict and flag a match for duplicate
		|  word definitions. FALSE means the word was found.
		*/
	not_found = prim_find();
		/*
		|  Create a word header, then
		|  set the CFA to prim and
		|  clear the smudge flag
		*/
	if(not_found == 0){
		Create();
		DP->CFA            = prim;
		DP->NFA->smudge    = VISIBLE;
		DP->NFA->immediate = WORD_IMMEDIATE;
		DP->NFA->view      = PRIM;   
	}
}
/***********************+----------+
|                       | do_allot |
|                       +----------+
| The '+8' is a hook for adding comma
*/
void do_allot()
{
	long value;

	value = (long) (WA->PFA.c_ptr +8);
	pushsp(value);
}
/***********************+-------+
|                       | allot |
|                       +-------+
|					>> RENAME TO Compile_allot()
|
|       allot   ( n --- )
|
|  This word allocates memeory in a word. Since UNTTL is not based
|  on a contigious dictionary space, allot allocates memory in the
|  most recently defined word in the dictionary. DP points to the
|  word to use. Memory is allocated and the pointer to it placed
|  in the PFA.
|
|  IDEA-> Have do_allot prim that knows there is an array of data
|         and the PFA is a pointer. This means that a variable
|         can be created with variable, then when allot is called,
|         the CFA is changed from do_variable to do_allot. It is
|         up to the application to know how many bytes were
|         alloted...
*/
void allot()
{
	char *ptr;
	long len;
			/*
			| Allocate memory.
			*/
	len                = popsp();
#ifdef CPP
	ptr                = new char [len+10];
#else
	ptr = (char*)malloc(len+10);
#ifdef CHECK_MALLOC
	if(!tib){
#ifndef VECTORED_C_IO
		fprintf(stderr,"allot: Could not allocate memory.\n");
#else
		stderr_vect("allot: Could not allocate memory.\n");
#endif
		abort_F();
		return;
	}
#endif
#endif
			/*
			| modify rest of the word.
			*/
	DP->CFA            = do_allot;
	DP->PFA.c_ptr      = ptr;
	*DP->PFA.l_ptr     = len;
	*(DP->PFA.l_ptr+1) = 00;
}
/***********************+---------+
|                       | execute |
|                       +---------+
|       execute ( WA --- ? )
|
| Usage:
|       ' .s execute
|
| will execute the word .s.
*/
void execute()
{
	WA   = (struct DictHeader*)popsp();
	(*WA->CFA)();
}
/***********************+------------+
|                       | state_prim |
|                       +------------+
|       state   ( --- state )
*/
void state_prim()
{
	pushsp(STATE);
}
/***********************+-------------+
|                       | abort_quote |
|                       +-------------+
|       abort" 	( tf --- )
| This is the run-time portion of abort". The string prints only
| if TRUE is on the top of the stack.
*/
void abort_quote()
{
	long truth;
	char *const_string;

	truth = popsp();
	if(truth){
		const_string = (char *)*IP++;
		const_string++;
#ifndef VECTORED_C_IO
		fprintf(stderr,"%s",const_string);
#else
		stderr_vect(const_string);
#endif
	}else{
		IP++;
	}
}
/***********************+--------+
|                       | builds |
|                       +--------+
|
*/
void builds()
{
#ifndef VECTORED_C_IO
	fprintf(stderr,"Not Implemented.\n");
#else
	stderr_vect("Not Implemented.\n");
#endif
}
/***********************+--------------+
|                       | Compile_does |
|                       +--------------+
|
|	This executes at defining word definition time.
|
|	+-------+-----+---+--------------+---+
|    ...| does> | pfa | 0 | run-time ... | 0 |
|	+-------+-----+---+--------------+---+
|		   |
|		   +-------^
*/
#ifdef IGNORE
void Compile_does()
{
/*	long empty_does;
*/

	pfa_list[pfa_offset++] = COMPILE_DO_DOES_WA;
	pfa_list[pfa_offset]   = (struct DictHeader *)0;  /* Leave room for DOES WA */
/*	empty_does             = pfa_offset++;
*/
	pfa_offset++;
	pfa_list[pfa_offset++] = (struct DictHeader *)0;  /* flag end of <builds */
/*
               v- Should this be FIRST_CELL?
*/
	pfa_list[0]            = (struct DictHeader*)pfa_offset;
	Semi_colon             = (void*)does_semi;	  /* change semi vector  */
}
#endif
/***********************+-----------------+
|			| Compile_do_does |
|			+-----------------+
|	This executes at instance compile time.
*/
void Compile_do_does()
{
	DP->CFA       = do_does;	/* May be able to leave as do_colon*/
		/*
		| Search for does pointer address and put in
		| first byte
		*/
	pfa_list[FIRST_CELL] = *(IP+1);
		/*
		| This call acts as semi.  >> Should FIRST_CELL be used?
		*/
	flush_pfa_list(0);
	*DP->PFA.l_ptr = (long)*(IP+2);
}
/***********************+---------+
|			| do_does |
|                       +---------+
|
| This word executes at run-time of instance word.
|
| This function loops through a PFA that contains a list of
| Word Addresses (WA) to execute. The last entry must be a
| NULL to flag end of addresses.
|
| do_does() is a modified version of do_colon().
*/
void do_does()
{
	long                        *array;
	register struct DictHeader  *word;
	struct          DictHeader **old_IP;

	old_IP = IP;
	IP     = WA->PFA.Waddr;			/* pt run-time pfa */
	array  = WA->PFA.l_ptr+1;
#ifndef VECTORED_C_IO
	printf("Array: %ld %ld %ld %ld\n",array[0],array[1],array[2],array[3]);
#else
	sprintf(tstring,"Array: %ld %ld %ld %ld\n",array[0],array[1],array[2],array[3]);
		stderr_vect(tstring);
#endif
	pushsp((long)&array[0]);
	word   = *IP++;

		/*
		| This loop executes the run-time portion of the
		| does>.
		*/
/* >>> This need to be a loop! <<<  */
	WA   = word;
	(*word->CFA)();
	IP = old_IP;			/* Restore IP to compile time 0 */
}
/***********************+--------------+
|                       | left_bracket |
|                       +--------------+
|
*/
void left_bracket()
{
	STATE = EXECUTION_MODE;
}
/***********************+---------------+
|                       | right_bracket |
|                       +---------------+
|
|
*/
void right_bracket()
{
	STATE = COMPILE_MODE;
	build_pfa_list();
}
/***********************+--------------+
|                       | bracket_tick |
|                       +--------------+
| NOTE: This is WRONG!!! It must compile the address of the
|       next word in the input stream into the dict as a literal.
|
|	: xxx ['] .s execute ;
*/
void bracket_tick()
{
	struct DictHeader *save_WA;

	save_WA                = WA;
	tick();
	literal();
	pfa_list[pfa_offset++] = WA;
	WA                     = save_WA; /* restore original WA */

}
/***********************+-----------------+
|                       | bracket_compile |
|                       +-----------------+
| This word compiles an immediate word into a colon definition.
*/
void bracket_compile()
{
	int  status;
	struct DictHeader *save_WA;

	save_WA    = WA;
		/*
		|  Get next token from input stream.
		*/
	next_word();
		/*
		|  Compile next token into dictionary.
		*/
	status = prim_find();   /* search dict for this word */
	if(status){
		pfa_list[pfa_offset++] = WA;
	}
	if(pfa_offset > PFA_SIZE){
#ifndef VECTORED_C_IO
		fprintf(stderr,"Definition too long. \n");
#else
		stderr_vect("Definition too long. \n");
#endif
		abort_F();
		STATE = EXECUTION_MODE;
	}
	WA = save_WA; /* restore original WA */
}
/***********************+------------+
|			| do_compile |
|			+------------+
| This is the run-time portion of compile();
*/
void do_compile()
{
	struct DictHeader *next_IP;

	if(STATE != COMPILE_MODE){
#ifndef VECTORED_C_IO
		fprintf(stderr,"Not in compile mode!!\n");
#else
		stderr_vect("Not in compile mode!!\n");
#endif
		abort_F();
	}else{
		next_IP                = *IP++;	/* get next word from dict, then skip its execution */
		pfa_list[pfa_offset++] = next_IP;
	}

}
/***********************+---------+
|                       | compile |
|                       +---------+
|					Change name to Compile_compile()
|
| Compile an address into a colon definition. Used in the
| form:
|	compile <name>
| Where:
|	<name> is the name of an existing word.
| The word contianing compile must be defined immediate, and
| is considered a 'compiler' word.
|
| Usage:
|	: literal     compile lit lit ; immediate
|
| The run-time portion of compile() is do_compile().
*/
void compile()
{
	int  status;
	struct DictHeader *save_WA;

	save_WA    = WA;
		/*
		|  Get next token from input stream.
		*/
	next_word();
		/*
		|  Compile next token into dictionary.
		*/
	status     =  prim_find();   /* search dict for this word */
	if(status == WORD_NOT_FOUND){
#ifndef VECTORED_C_IO
		fprintf(stderr,"Word Not Found...\n");
#else
		stderr_vect("Word Not Found...\n");
#endif
	}else{
		pfa_list[pfa_offset++] = DO_COMPILE_WA;
		pfa_list[pfa_offset++] = WA;
	}
	if(pfa_offset > PFA_SIZE){
#ifndef VECTORED_C_IO
		fprintf(stderr,"Definition too long. \n");
#else
		stderr_vect("Definition too long. \n");
#endif
		abort_F();
		STATE = EXECUTION_MODE;
	}
	WA = save_WA; /* restore original WA */
}
/***********************+----------------+
|                       | Compile_string |
|                       +----------------+
|  The word to create should be in pad. PAD is counted in the
|  Forth tradition. It must be converted to null terminated
|  for C to digest it properly.
|
*/
void Compile_string()
{
	char *ptr;
	long len;
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
			/*
			| Allocate memory.
			*/
	len                = popsp();
#ifdef CPP
	ptr                = new char [len+10];
#else
	ptr                = (char*)malloc(len+10);
#ifdef CHECK_MALLOC
	if(!ptr){
#ifndef VECTORED_C_IO
		fprintf(stderr,"Compile_string: Could not allocate memory.\n");
#else
		stderr_vect("Compile_string: Could not allocate memory.\n");
#endif
		abort_F();
		return;
	}
#endif
#endif
			/*
			| modify rest of the word.
			*/
	DP->CFA            = do_allot;
	DP->PFA.c_ptr      = ptr;
	*DP->PFA.l_ptr     = len;
	*(DP->PFA.l_ptr+1) = 0;
}
/***********************+---------+
|			| recurse |
|			+---------+
*/
void recurse()
{
	pfa_list[pfa_offset] = DP;
}
