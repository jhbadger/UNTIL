/*
|       FILE: VOCAB.C
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
|	DATE		COMMENT
|	07/05/93	Created via several cut & pastes. [nes]
|	07/18/93	Added vocs() [nes]
*/

#include "compiler.h"

#include <stdio.h>
#include <string.h>

#ifndef NO_STDLIB
#include <stdlib.h>
#endif

#ifdef ANSIC
#include <conio.h>
#include <time.h>
#endif

#ifdef OSF
#include <time.h>
#endif

#ifdef SGI_INDY
#include <time.h>
#endif

#ifdef SPARC
#include <time.h>
#endif

#include "until.h"
#include "functs.h"

/***********************+----------+
|			| dot_vocs |
|			+----------+
| This function prints a list of the vocabularies defined at
| the moment. This is equivalent to words...
*/
void dot_vocs()
{

}
/***********************+------------+
|                       | Create_voc |
|                       +------------+
|  The vocab to create should be in pad. PAD is counted in the
|  Forth tradition. It must be converted to null terminated
|  for C to digest it properly. This is the generic "create a
|  vocab header" Create_voc() is Create() with modifications
|  specific to vocabularies.
|
*/
void Create_voc()
{
	struct NameField  *nfa;
	struct DictHeader *entry;

	int  status;
	int  len;
	char string[80];

#ifdef CPP
	entry = (DictHeader *) new char [sizeof(DictHeader)];
#else
	entry = (struct DictHeader *) malloc(sizeof(struct DictHeader));
#ifdef CHECK_MALLOC
	if(!entry){
		printf("Create_voc: Could not allocate DictHeader.\n");
		abort_F();
		return;
	}
#endif
#endif
	len                   = *pad;
	strncpy(string,(pad + 1),len);
	string[len]           = '\0';

	status                = prim_find();    /* search dict for this word */
	if(status != WORD_NOT_FOUND){
		printf("%s: Vocabulary redefined...\n",string);
	}
/*
| Need to add check of VOC_LINK chain
*/
	nfa                   = CreateNFA(string);
	entry->NFA            = nfa;
	entry->NFA->smudge    = VISIBLE;
/*
	entry->NFA->immediate = NOT_IMMEDIATE;
*/
	entry->NFA->immediate = WORD_IMMEDIATE;	/* this is correct 99% of time */
	entry->CFA            = do_vocabulary;
	entry->PFA.lvalue     = (long)DP0;	/* Putting NAME0 in the
						   vocab means always have
						   at least 1 word, which
						   simplifies some code
						*/
	entry->LFA            = VOC_LINK;

	VOC_LINK              = entry;
/*
	DP                    = 0;
*/
}
/***********************+---------+
|			| set_voc |
|			+---------+
*/
void set_voc()
{
	VOC_LINK->CFA  = do_vocabulary;
	VOC_LINK->LFA  = 0;
}
/***********************+--------------------+
|                       | Compile_vocabulary |
|                       +--------------------+
|
*/
void Compile_vocabulary()
{
	next_word();
	Create_voc();
}
/***********************+---------------+
|                       | do_vocabulary |
|                       +---------------+
| This word has to search the vocabulary thread for a match on the
*/
void do_vocabulary()
{
	if(STATE        == COMPILE_MODE){
		VOC_FLAG = CONTEXT;
	}
	CONTEXT          = WA;
}
/***********************+---------------------+
|                       | Compile_definitions |
|                       +---------------------+
|
*/
void Compile_definitions()
{
	printf("Not Implemented.\n");
}
/***********************+----------------+
|                       | do_definitions |
|                       +----------------+
|
*/
void do_definitions()
{
	CURRENT  = CONTEXT;
	LATEST   = CONTEXT->PFA.DHaddr;
	DP       = LATEST;
}
/***********************+-------------------+
|			| make_initial_vocs |
|			+-------------------+
*/
void make_initial_vocs()
{
		/*
		| Create initial word in the dictionary
		*/
	DP0                = (struct DictHeader *) malloc(sizeof(struct DictHeader));
#ifdef CHECK_MALLOC
	if(!DP0){
		printf("make_initial_vocs: Could not allocate DictHeader.\n");
		abort_F();
		return;
	}
#endif
	NAME0              = CreateNFA("--Name0--");
	DP0->NFA           = NAME0;
	DP0->CFA           = do_variable;
	DP0->PFA.lvalue    = 0;
	DP0->LFA           = 0;
	DP                 = DP0;
	VOC_LINK           = DP0;
	CONTEXT            = DP0;
	CURRENT            = DP0;
	CONTEXT->PFA.DHaddr= DP0;
		/*
		| Create the root vocabulary.
		*/
	FORTH_WA           = 0;
	strcpy(pad+1,"root");
	*pad               = 4;
	Create_voc();
	CURRENT            = VOC_LINK;
	CONTEXT            = VOC_LINK;
		/*
		| Create the Forth vocabulary
		*/
	strcpy(pad+1,"forth");
	*pad               = 5;
	Create_voc();
	CURRENT            = VOC_LINK;
	CONTEXT            = VOC_LINK;
	FORTH_WA           = VOC_LINK;
}

