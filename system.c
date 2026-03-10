/*
|	FILE: SYSTEM.C
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
|	03/27/95	Broke out from IO.C [nes]
*/

#include "compiler.h"

#include <stdio.h>
#include <string.h>

#ifndef NO_STDLIB
#include <stdlib.h>
#endif

#ifdef ANSIC
#include <dir.h>
#endif

#ifdef VAX
#include <file.h>
#endif

#include "until.h"
#include "functs.h"

/***********************************************************************
|***********************************************************************
|	These are application development type words.
*/
/***********************+------+
|			| edit |
|			+------+
| " filename " edit
|
|	The filename to edit ends up at here
*/
void edit()
{
	char file_name[128];

	next_word();
	strcpy(file_name,pad+1);

	strcpy(pad,editor);
	strcat(pad," ");
	strcat(pad,file_name);
	system(pad);
}
/***********************+--------+
|			| system |
|			+--------+
| { command} system
|
|	The filename to edit ends up at here
*/
void do_system()
{
	char *cmd;

	cmd = (char*)popsp();
	system(cmd);
}
/***********************+-----------+
|			| end_shell |
|			+-----------+
|
*/
void end_shell()
{
#ifndef VECTORED_C_IO
	printf("[End]");
#else
	stdout_vect("[End]");
#endif
}
/***********************+----------+
|			| do_shell |
|			+----------+
|
*/
void do_shell()
{
#ifdef UNIX_SYS
	char *shell;

#ifndef VECTORED_C_IO
	printf("Type ^d to return to Until...\n");
#else
	stdout_vect("Type ^d to return to Until...\n");
#endif
	if((shell = getenv("SHELL")) == NULL){
		shell = "sh";
	}
	system(shell);
	end_shell();
#else
#ifndef VECTORED_C_IO
	printf("Type EXIT to return to UNTIL...\n");
#else
	stdout_vect("Type EXIT to return to UNTIL...\n");
#endif
	system("");
	end_shell();
#endif
}
/***********************+--------+
|			| do_dir |
|			+--------+
|
*/
void do_dir()
{
#ifdef UNIX_SYS
	system("ls -l");
	end_shell();
#else
	system("dir");
	end_shell();
#endif
}
/***********************+-------------+
|			| prim_getenv |
|			+-------------+
|
*/
void prim_getenv()
{
	char *symbol;
	
	symbol = (char*)popsp();
	symbol = (char*)R_addr;
	pushsp((long)getenv(symbol));
}

/***********************+-----------+
|			| type_file |
|			+-----------+
|
| Change into a 'more' type operation...
*/
void type_file()
{
	FILE *fd;
	char *eof = 0;

	next_word();
	fd = fopen((pad + 1),"r");
	if(!fd){
#ifndef VECTORED_C_IO
		fprintf(stderr,"ERROR: File %s\n", (pad + 1));
#else
		sprintf(tstring,"ERROR: File %s\n", (pad + 1));
		stderr_vect(tstring);
#endif
		return;
	}
	do{
		eof = fgets(here_here,PADSIZE,fd);
#ifndef VECTORED_C_IO
		printf("%s",here_here);
#else
		stdout_vect(here_here);
#endif
	}while(eof != 0);
	fclose(fd);
}

