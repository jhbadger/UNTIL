/*
|       FILE: UNTIL.H
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
|       10/07/92        Added "hld". [nes]
|       12/21/92        Broke function prototypes out into a
|                       separate header file.
|	03/11/93	Added FREAD_WA [nes].
|	06/28/93	Changed tib, pstack, rstack, & here_here from
|			arrays to a char ptrs. [nes]
|	06/29/93	Changed pad from array to ptrs. [nes]
|	07/05/93	Added vocabulary stack. [nes]
|	05/30/94	Added flags to control C string words. [nes]
|	07/06/94	Added new File Descriptor structure. [nes].
|	08/04/94	Made Beta Distribution compile changes. [nes].
|	08/31/94	Expanded C Stack to 32k.
|	12/30/94	Started V2.1.1
|	01/20/95	Moved file descriptor stack to io.h. [nes] 
|       03/03/95        Made compatible with C++ and added vectoredFunction
|                       typedef     -- EFC
|	04/??/95	Broke out UNTILPRI.H into a separate file.
|	04/23/95	Added view_location structure. [nes]
*/
/*
|			Magic settings for Param. Stack
*/

#ifndef UNTIL_H_
#define UNTIL_H_

#include <stdio.h>

#define COMPILE_ADDR(W) (pfa_list[pfa_offset++] = (struct DictHeader*) W)
#define ADDR            (long)(void*)
#define NUMB            (long)

#define SP PSP


/*
|			 Buffer sizes
*/
#define MAX_NAME         32
#define TIBSIZE          256
#define PADSIZE          128
#define PSTACKSIZE       64
#define RSTACKSIZE       32
#define PFA_SIZE         100

#define NUM_FILES        16
#define BIN              0
#define SOURCE           1
#define OPENED           2
#define CLOSED           4
#define STANDARD_IN      5

#ifdef TRUE
#undef TRUE
#endif
#define TRUE             1

#define FALSE            0
#define BLANK            ' '
#define ESC              '\033'
				/* Values for file I/O */
#define READ            "r"
#define WRITE           "w"
#define APPEND          "a"
#define BINARY          "b"
#define TEXT		"t"
#define UPDATE          "r+"
#define NEW_UPDATE      "w+"
#define APPEND_UPDATE	"a+"

#define FOREVER         for(;;)
#define WORD_FOUND      1
#define WORD_NOT_FOUND  0
#define WORD_IMMEDIATE  -1
#define NOT_IMMEDIATE   0
#define COMPILE_MODE    1
#define EXECUTION_MODE  0
#define FIRST_CELL	1
				/* Values for smudge 		*/
#define VISIBLE		0
#define HIDDEN		1
				/* Values for view types 	*/
#define PRIM		0
#define HIGH_LEVEL	1

/********************************************************
|               UNTIL STRUCTURES
*/
union cell_types{
	char  C;
	short S;
	int   I;
	long  L;
	void  *P;
};

struct view_location{
	char *filename;
	int  offset;
	int  type;		/* prim/high level  */
};

struct NameField{
	int  len;
	int  system;
	int  immediate;
	int  smudge;
	char *name;
	struct view_location *view;
};

union pfa_type{
	struct DictHeader **Waddr;
	struct DictHeader *DHaddr;
	long              lvalue;
	char              cvalue;
	long              *l_ptr;
	char              *c_ptr;
	double            *d_ptr;
};

struct DictHeader{
	void              (*CFA)();     /* ptr to primitive function */
	union  pfa_type   PFA;          /* ptr to list of words to execute */
	struct NameField  *NFA; /* ptr to name string */
	struct DictHeader *LFA; /* points to next word in dict. */
};

struct encrypt_key{
	char crypt_key[80];
	long crypt_offset;
	long crypt_len;
} ;

#ifdef __cplusplus
extern "C" {
#endif

typedef void  (*vectoredWordFunction)(void);
typedef char* (*vectoredFunction)(void);
typedef void  (*vectoredIOFunction)(char*);

#ifdef __cplusplus
}
#endif


struct file_descriptor{
	FILE *fd;		/* File descriptor   */
	vectoredFunction read_wa;	/* Read function for this file */
	long type;		/* BIN or SOURCE               */
	char *crypt;		/* Encryption key for BIN files*/
	long status;		/* OPEN or CLOSED              */
	long position;		/* Save ftell() return value   */
	char *name;		/* Filename                    */
	char *tib;		/* TIB for this file           */
	long in;		/* IN for this file            */
} ;
/********************************************************
|               UNTIL REGISTERS
*/
extern long seal_eof;
extern long tib_len;
extern long pfa_offset;
extern long comma_offset;
extern long PSP;         /* Param Stack Pointer */
extern long RS;                /* Return Stack Pointer */
extern long START;              /* Startup flag */
extern long IN;
extern long STATE;              /* compile state 0=Interpret 1=compile */
extern long INPUT_SOURCE;       /* Where the compiler reads from */
extern long BASE;               /* Current base                        */
extern long SPAN;
extern long hld;               /* Used in <# # #> sequences            */
/**********************************************************
|               STARTUP DATA STRUCTURE
*/
extern char *comma_data;
extern int  u_errno;
extern int  show_copy;
extern int  sys_errno;
extern char CALC_VERSION[];
extern int  Vers;
extern char DATE[];
extern char boot_word[];
extern char first_word[];
extern char dict_file[];
extern char VERSION[];
extern char editor[];
extern char until_directory[];
extern char autoload[];
extern char binload[];
extern char arg_file[];
extern char tstring[];

/***********************+------------------+
|                       | Vocabulary Stuff |
|                       +------------------+
*/
extern struct DictHeader *LATEST;
extern struct DictHeader *CURRENT;
extern struct DictHeader *CONTEXT;
extern struct DictHeader *FORTH;
extern struct DictHeader *VOC_LINK;
extern struct DictHeader *vocs[8];
extern struct DictHeader *VOC_FLAG;
extern long              vp;

/****************************************************************
|                       Buffers and other Data Structures
*/
extern char *tib;
extern char *here_here;
extern long *pstack;
extern long *rstack;
extern char *pad;
extern void *R_addr;

/************************************************************
|                       OTHER STUFF
*/
extern long show_stack;
extern long string_return;
extern long quote_null;

extern long left_margin;
extern long right_margin;

extern long does_flag;
extern long ABORT_FLAG;
extern long QUIT;
extern long In_case;

extern long fargc;
extern char **fargv;
extern char PROMPT_STR[];
extern char the_cold_msg[];
extern char the_warm_msg[];
extern struct encrypt_key this_key;

/*************************************************************
|               VECTORED WORDS
*/

#ifdef __cplusplus
extern "C" {
#endif

extern vectoredFunction user_cold_hook;
extern vectoredFunction user_abort_hook;
extern vectoredFunction user_outer_hook;

extern vectoredFunction FREAD_WA;
extern vectoredFunction READ_WA;
extern vectoredFunction Semi_colon;
extern vectoredFunction number_vect;
extern vectoredFunction literal_vect;

#ifdef VECTORED_IO
extern vectoredFunction verbatim_vect;
extern vectoredFunction emit_vect;
extern vectoredFunction display_vect;
extern vectoredFunction center_vect;
extern vectoredFunction dot_quote_vect;
extern vectoredFunction type_vect;
extern vectoredFunction dot_vect;
extern vectoredFunction g_dots_vect;
extern vectoredFunction dots_vect;
extern vectoredFunction prints_vect;
extern vectoredFunction printf_vect;
extern vectoredFunction space_vect;
extern vectoredFunction spaces_vect;
extern vectoredFunction query_terminal_vect;
extern vectoredFunction cr_vect;
extern vectoredFunction printf_vect;
extern vectoredFunction key_vect;
#endif

#ifdef VECTORED_C_IO
extern vectoredIOFunction stderr_vect;
extern vectoredIOFunction stdout_vect;
extern vectoredWordFunction stdin_vect;
#endif

#ifdef __cplusplus
}
#endif

/*************************************************************
|
*/
extern int I;
extern union  cell_types  cell;

extern struct DictHeader **IP;                  /* Instrcution Pointer */
extern struct DictHeader *DP;                   /* Dictionary Pointer */
extern struct DictHeader *WA;                   /* Word Address Register */
extern struct DictHeader *pfa_list[];	/* Temp area to compile list of pfa's */
extern struct DictHeader *LIT_WA;               /* lit Word Address */
extern struct DictHeader *CASE_WA;
extern struct DictHeader *OF_WA;
extern struct DictHeader *ENDCASE_WA;
extern struct DictHeader *ZERO_BRAN_WA;
extern struct DictHeader *BRANCH_WA;
extern struct DictHeader *QUOTE_WA;
extern struct DictHeader *DOT_QUOTE_WA;
extern struct DictHeader *LOOP_WA;
extern struct DictHeader *DO_WA;
extern struct DictHeader *DO_FOR_WA;
extern struct DictHeader *I_WA;
extern struct DictHeader *NAME0_WA;
extern struct DictHeader *ABORT_QUOTE_WA;
extern struct DictHeader *PLUS_LOOP_WA;
extern struct DictHeader *J_WA;
extern struct DictHeader *K_WA;
extern struct DictHeader *TICK_WA;
extern struct DictHeader *DO_COMPILE_WA;
extern struct DictHeader *DO_DOES_WA;
extern struct DictHeader *COMPILE_DO_DOES_WA;
extern struct DictHeader *DO_VOC_WA;
extern struct DictHeader *DO_DEF_WA;
extern struct DictHeader *DP0;
extern struct DictHeader *FORTH_WA;

extern struct NameField  *NAME0;

#endif

