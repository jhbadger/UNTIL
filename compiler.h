/*
|	COMPILER.H
|
|	This file attempts to automatically choose the C compiler
|	you are using.
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

#ifndef COMPILER_H_
#define COMPILER_H_

#undef VAX
#undef ANSIC
#undef OSF
#undef SPARC
#undef GCC

#undef  DEBUG_PROG	/* Compile C debugging functions    */
#define C_STRINGS	/* Compile C String functions       */
#define CIO_MODULE	/* Compile C I/O functions          */
#define SEAL_MODULE	/* Compile seal module              */
#define SEARCH_MODULE	/* Compile search & repl module     */
#define READABLE	/* generate human readable :-) code */
#define FLOAT_POINT	/* Include floating point math      */
#define FFLOAT		/* Use F type for printing          */
#undef  EFLOAT		/* Use E type for printing          */
#undef  GFLOAT		/* Use G type for printing          */
#undef  VECTORED_IO	/* Vector Forth I/O words           */
#undef  VECTORED_C_IO   /* all I/O is vectored */
#undef  REPORT_STARTUP_ERRS /* Reports missing until.bin & applic.app */
#define CHECK_MALLOC	/* Include code to check for memory allocation errors */
#undef  NEED_MEMMOVE	/* Define this symbol if your C library does  */
			/* not include memmove().                     */
#define SENGINE		/* build SGML document interpreter            */
#undef  SENGINE_BANNER	/* Use S-Engine or Until opening banner       */
#undef  ANSI_CONSOLE	/* Use ANSI escape sequences for screen stuff */
#define SHARP_WORDS	/* Include Forth <# ... #> formatting code    */
#define CMD_LINE_FLAG	/* Designate Forth source files by -f on cmd line */
#undef  POS_DO		/* Do loop indices must be positive           */

/*		VAX/OpenVMS running VAX-11C */
#ifdef vax11c
#define VAX
#endif

/*		Alpha-AXP running OpenVMS */
#ifdef __DECC
#define VAX
#endif

#ifdef MIPS
#define UNIX_SYS
#include <malloc.h>
extern char* getenv(char*);

#ifndef __GCC__         /* actually only if NOT using gcc */
#define NO_STDLIB
#endif

#else

/*		DECstation running OSF/1  */

#ifdef mips
#define OSF
#define UNIX_SYS
#endif

#endif

/*		SGI Indy ANS C compiler */
#ifdef __sgi 
#define SGI_INDY
#define UNIX_SYS
#endif

/*		AIX ANS C compiler */
#ifdef __AIX
#define IBM_AIX
#define UNIX_SYS
#endif
/*		Sequent running Dynix */
#ifdef _SEQUENT_
#define DYNIX
#define UNIX_SYS
#endif

/*		Sparc C compiler */
#ifdef sun
#define SPARC
#define UNIX_SYS
#endif
/*		GCC on a Sparc */
/*		GCC on a Linux */
#ifdef __unix__
#define GCC
#define UNIX_SYS
#endif
/*		Alpha running OSF/1  */
#ifdef __osf__
#define OSF
#define UNIX_SYS
#endif

/*		PDP-11 with DEC C and RT-11  */
#ifdef __pdp11c
#define OSF
#endif

/*		Borland C++ - MS-DOS */
#ifdef __BORLANDC__
#define SEG_ADDRESSING
#define ANSIC
#define BCC
#endif

/*            Symantec C++ 6.1 - MS-DOS */
#ifdef __SC__
#define SEG_ADDRESSING
#define ANSIC
#define MSDOS
#endif

/*		Turbo C V1.5 - MS-DOS */
#ifdef __TURBOC__
#define SEG_ADDRESSING
#define ANSIC
#define BCC
#endif

/*		Define NEED_FAR_PTRS for PC compilers that support
		'far' memory references. Smaller versions of Until
		may not need it. The symptom is 'help' does not work.
		I expect this list to expand...

		NOTE: need to do something about farmalloc and farfree.
		      both have different calling sequences than malloc
		      and free...
*/
#ifdef NEED_FAR_PTRS
#define strnset _fstrnset
#define strcmp _fstrcmp
#define strncmp _fstrncmp
#define strcpy _fstrcpy
#define strncpy _fstrncpy
#define strcat _fstrcat
#define strncat _fstrncat
#define strlen _fstrlen
#define memcpy _fmemcpy
#define memmove _fmemmove
#endif

#ifdef DEBUG_PROG
void debug(char*);
void debugs(char*,char*);
void debugi(char*,long);
#endif

#endif
