/*
|	FILE: OUTER.C
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
|	01/16/95	Made cleanup changes suggested by Akira Kida,
|			akida@isd.hin.konica.co.jp. [nes]
|       03/03/95        setup() now uses the argument list as input
|                       parameters, and uses them as file names to do
|                       an autoload on them.
|                       The outer interpreter, outer(), now has a hook
|                       to a user defined routine that can optionally be
|			run.  -- EFC
*/

#include "compiler.h"

#include <stdio.h>

#ifdef ANSIC
#include <process.h>
#include <conio.h>
#endif

#include <string.h>

#include "until.h"
#include "functs.h"

/***********************+-----------+
|			| save_args |
|			+-----------+
| Use save_args when the application has to take information
| from the command line as well as Until source filenames. It
| only treats the -f args as source files. The convention I use
| is putting the positional parameters first and source files
| last.
*/
void save_args(int argc, char **argv)
{
	long success;
	int  i;
	char trash[128];
		/*
		| Initialization and such
		*/
	fargc = argc;
	fargv = argv;
		/*
		| Check to see if there are args.
		*/
	for(i=(argc-1);i;i--){
		strcpy(trash,argv[i]);
		if(trash[0] == '-'){		/* Got cmd line args */
			switch(trash[1]){
			case 'f':		/* File to load      */
			case 'F':
				strcpy(arg_file,&trash[2]);
				success = do_autoload(arg_file,SOURCE);
				if(!success){
					printf("Could not open %s\n",arg_file);
					getchar(); /* wait for key press */
				}
				break;
			case 'w':		/* Word to run       */
			case 'W':
				strcpy(first_word,&trash[2]);
				break;
			default:
				printf("Invalid flag; Ignored\n");
			}
		}
	}
}
/***********************+-----------+
|			| load_args |
|			+-----------+ 
| scan any remaining arguments as file names 
*/
static void load_args(int argc, char** argv)
{
        int i;
#ifdef REPORT_STARTUP_ERRS
	long success;
#endif
        
/*        INPUT_SOURCE = 0;   / * 05/04/95 [nes] */

	if ( argc > 1 )
                for (i = argc-1; i; --i)
                {
#ifdef REPORT_STARTUP_ERRS
		success = do_autoload(argv[i],SOURCE);
		if(!success){
			printf("startup() Error-default application file not found: %s\n",autoload);
		}
#else
		do_autoload(argv[i],SOURCE);
#endif
		}
}
/***********************+---------+
|                       | startup |
|                       +---------+
*/
void startup(int argc, char** argv)
{
	long len;
#ifdef REPORT_STARTUP_ERRS
	long success;
#endif
	char full_filename[128];
	
	fargc = argc;		fargv = argv;
        cold();
#ifdef CMD_LINE_FLAG		/* use -f for source files */
	save_args(argc,argv);
#else
	load_args( fargc, fargv );
#endif
		/*
		| These two file things must be done in reverse order
		| because the last autoloaded file is the first one
		| processed.
		*/
	len = strlen(autoload);
	if(len){
		strcpy(full_filename,until_directory);
		strcat(full_filename,autoload);
#ifdef REPORT_STARTUP_ERRS
		success = do_autoload(autoload,SOURCE);
/*		success = do_autoload(full_filename,SOURCE);
*/
		if(!success){
			printf("startup() Error-default application file not found: %s\n",full_filename);
		}
#else
		do_autoload(autoload,SOURCE);
/*		do_autoload(full_filename,SOURCE);
*/
#endif
	}
	len = strlen(binload);
	if(len){
		strcpy(full_filename,until_directory);
		strcat(full_filename,binload);
#ifdef REPORT_STARTUP_ERRS
		success = do_autoload(binload,BIN);
/*		success = do_autoload(full_filename,BIN);
*/
		if(!success){
			printf("startup() Error-default binary file not found: %s\n",binload);
		}
#else
		do_autoload(binload,BIN);
/*		do_autoload(full_filename,BIN);
*/
#endif
	}
		/*
		| Boot to application word
		*/
	len = strlen(boot_word);
	if(len){
		strcpy((pad + 1),boot_word);
		*pad   = len;
		exec_word();
	}else{
		outer();
	}
}
/***********************+-------+
|			| outer |
|			+-------+
|
|	This is the outer interpreter. It seems too simple.
|
*/
void outer()
{
	static started;
	int    found;

	started = TRUE;
/*
	QUIT = 0;
*/
	FOREVER{
	  if(QUIT){
		return;		        /* Initialization */
	  }else{
		warm();
	  }
	  do{
		pushsp(BLANK);
		word();			/* get next word from input stream */
		minus_find();		/* Look it up in the dictionary */
		found = popsp();
		if(found){		/* found the word */
			WA = (struct DictHeader*) popsp();
			(*WA->CFA)();
		}else{		        	/* Word not found, so try to turn */
			drop();			/*   it into a number. */
			pushsp((long)pad);	/* Word leaves string at pad */
			(*number_vect)();	/* Convert it to int. */
		}
		if(show_stack){
			gdot_s();
		}
          	/* call users function if defined */
                if ( user_outer_hook )
                	user_outer_hook();
	  }while(!ABORT_FLAG);
		/*
		| If loading from a file, back up a level. This
		| should get us back to STDIN...
		*/
	  if(INPUT_SOURCE>1){
		close_include();
	  }
	  	/*
	  	| Supply the user with a way to cleanup if aborting
	  	*/
	  if(user_abort_hook){
	  	user_abort_hook();
	  }
	  ABORT_FLAG = 0;
	}
}
/***********************+----------------+
|			| set_show_stack |
|			+----------------+
|	show_stack	( tf --- )
*/
void set_show_stack()
{
	show_stack = popsp();
}
