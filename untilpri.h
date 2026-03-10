/*
|       FILE: UNTILPRIV.H
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
|       03/03/95        Made compatible with C++ and added vectoredFunction
|                       typedef     -- EFC
|	04/??/95	Broke out from UNTIL.H -- EFC
|	04/13/95	Merged in other Until.h changes
*/
/*
|			Magic settings for Param. Stack
*/

#ifndef UNTILPRIV_H_
#define UNTILPRIV_H_

#include <stdio.h>

#include "until.h"

#ifdef until

#ifdef  BCC
unsigned _stklen = 32768;		/* Increased stack size in BC++ */
#endif
/********************************************************
|               CONSTANTS FOR BUFFER SIZES AND SUCH
|
**********************************************************
|               STARTUP DATA STRUCTURE
*/
 char *comma_data = NULL;
#ifdef SENGINE
 int  show_copy          = 1;
#else
 int  show_copy          = 1;
#endif
 int  u_errno            = 0;
 int  sys_errno          = 0;
 int  Vers               = 1;
 char CALC_VERSION[]     = "2.5.2";
#ifndef SPARC 
#ifdef MIPS
 char DATE[]             = "July 1995";
#else
 char DATE[]             = __DATE__;
#endif
#else
 char DATE[]              = "July 1995";
#endif
 char boot_word[32]       = "<outer>";
 char first_word[32]      = "";
 char dict_file[80]       = "";
 char VERSION[]           = "2.5.2";
 char editor[]            = "emacs";
#ifdef BCC
 char until_directory[80] = "\until";
#else
 char until_directory[80] = "";		/* /usr/local/until maybe??? */
#endif
 char autoload[80]        = "applic.app";
#ifdef SEAL_MODULE
 char binload[128]        = "until.bin";
/* char binload[128]        = "o:\\sgml\\bin\\until.bin";
*/
#else
 char binload[128]        = "until.app";
#endif
 char arg_file[128]       = "";
 char tstring[128];

/********************************************************
|               UNTIL REGISTERS
*/
long seal_eof;
long tib_len;
long pfa_offset;
long comma_offset;
long PSP;               /* Param Stack Pointer */
long RS;                /* Return Stack Pointer */
long START = 1;         /* Startup flag */
long IN;
long STATE;             /* compile state 0=Interpret 1=compile  */
long BASE;              /* Current base                         */
long SPAN;
long hld;               /* Used in <# # #> sequences            */

/***********************+------------------+
|                       | Vocabulary Stuff |
|                       +------------------+
*/
struct DictHeader *LATEST;	/* Last word compiled          */
struct DictHeader *CURRENT;	/* Vocab words compiled into   */
struct DictHeader *CONTEXT;	/* First vocab in search order */
struct DictHeader *FORTH;
struct DictHeader *VOC_LINK;	/* Last vocab created          */
struct DictHeader *vocs[8];	/* allow 8 vocs at a time      */
struct DictHeader *VOC_FLAG;	/* Set when vocab executed in compile
				|  mode to the current value of CONTEXT.
				|  Should be used to reset CONTEXT by
				|  compiler the next time a word is found.
				|  This keeps current and context in
				|  sequence.
				*/

long              vp;		/* voc stack ptr               */
/****************************************************************
|                       Buffers and other Data Structures
*/
char *tib       = NULL;
char *here_here = NULL;
long *pstack    = NULL;
long *rstack    = NULL;
char *pad       = NULL;
void *R_addr    = NULL;

/************************************************************
|                       OTHER STUFF
*/
int I;
union cell_types cell;

long show_stack		= TRUE;		/* T=return values */
long string_return	= TRUE;		/* T=return values */
long quote_null		= FALSE;	/* F=counted T=null*/
long left_margin        = 0;
long right_margin       = 79;
long does_flag;
long ABORT_FLAG;
long QUIT;
long In_case;
/*
long ABORT;
*/
long fargc;
char **fargv;

#if SENGINE_BANNER
char PROMPT_STR[20]     = "S-Engine> ";
#else
char PROMPT_STR[20]     = "Until> ";
#endif

char the_cold_msg[]     = "Cold Start...\n";
char the_warm_msg[]     = "\n";
struct encrypt_key this_key;
/*************************************************************
|               VECTORED WORDS
*/
#ifdef __cplusplus
extern "C" {
#endif

vectoredFunction user_cold_hook   = NULL;
vectoredFunction user_abort_hook  = NULL;
vectoredFunction user_outer_hook  = NULL;

vectoredFunction FREAD_WA;
vectoredFunction READ_WA;
vectoredFunction Semi_colon;
vectoredFunction number_vect;
vectoredFunction literal_vect;

#ifdef VECTORED_IO
vectoredFunction verbatim_vect;
vectoredFunction display_vect;
vectoredFunction center_vect;
vectoredFunction dot_quote_vect;
vectoredFunction type_vect;
vectoredFunction dot_vect;
vectoredFunction g_dots_vect;
vectoredFunction dots_vect;
vectoredFunction prints_vect;
vectoredFunction printf_vect;
vectoredFunction emit_vect;
vectoredFunction space_vect;
vectoredFunction spaces_vect;
vectoredFunction query_terminal_vect;
vectoredFunction cr_vect;
vectoredFunction printf_vect;
vectoredFunction key_vect;
#endif

#ifdef VECTORED_C_IO
vectoredIOFunction stderr_vect;
vectoredIOFunction stdout_vect;
vectoredWordFunction stdin_vect;
#endif

#ifdef __cplusplus
}
#endif

/*************************************************************
|
*/
struct DictHeader *pfa_list[PFA_SIZE];  /* Temp area to compile list of pfa's */
struct DictHeader **IP;         	/* Instrcution Pointer */
struct DictHeader *DP;                  /* Dictionary Pointer */
struct DictHeader *WA;                  /* Word Address Register */
struct DictHeader *LIT_WA;              /* lit Word Address */
struct DictHeader *CASE_WA;
struct DictHeader *OF_WA;
struct DictHeader *ENDCASE_WA;
struct DictHeader *ZERO_BRAN_WA;
struct DictHeader *BRANCH_WA;
struct DictHeader *QUOTE_WA;
struct DictHeader *DOT_QUOTE_WA;
struct DictHeader *LOOP_WA;
struct DictHeader *DO_WA;
struct DictHeader *DO_FOR_WA;
struct DictHeader *I_WA;
struct DictHeader *NAME0_WA;
struct DictHeader *ABORT_QUOTE_WA;
struct DictHeader *PLUS_LOOP_WA;
struct DictHeader *J_WA;
struct DictHeader *K_WA;
struct DictHeader *TICK_WA;
struct DictHeader *DO_COMPILE_WA;
struct DictHeader *DO_DOES_WA;
struct DictHeader *COMPILE_DO_DOES_WA;
struct DictHeader *DO_VOC_WA;
struct DictHeader *DO_DEF_WA;
struct DictHeader *DP0;
struct DictHeader *FORTH_WA;

struct NameField  *NAME0;


#endif



#endif
