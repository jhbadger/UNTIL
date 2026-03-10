/*
|	FILE: IO.C
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
|	13-Mar-1993	Added open_source() and associated changes.
|			Also added binary_input_mode flag. [nes]
|	30-JUN-1993	Changed file words to take file names etc
|			from pstack instead of here. [nes]
|	27-JUN-1994	Changed read_include() to close the current
|			include file when the ABORT_FLAG is set. This
|			kicks out of reading CALC.APP when a compile
|			error occurs. [nes]
|	06-JUL-1994	Changed file words to use new file descriptor
|			structure. These are hooks for files on cmd line.[nes]
|	27-AUG-1994	Split the FILE.C off. [nes]
|	25-NOV-1994	Modified do_autoload() so autoloading will work
|			on Unix systems.
|	15-JAN-1995	Changed gets() call to fgets() in get_stdin(). [nes]
|	01/16/95	Made cleanup changes suggested by Akira Kida,
|			akida@isd.hin.konica.co.jp. [nes]
|       03-MAR-1995     Fixed READ_WA linked list in open/close include
|                       functions    -- EFC
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
#include "io.h"
/***********************+----------------+
|			| read_next_line |
|			+----------------+
*/
void read_next_line()
{
	char *status;
ReadAgain:
	status     = (READ_WA)();
	if(status == 0){
/*		if(INPUT_SOURCE){	/ * EOF on include file, back to stdin */
		if(this_file){	/* EOF on include file, back to stdin */
			close_include();
			goto ReadAgain;
		}else{			/* EOF on stdin, so exit */
#ifndef VECTORED_C_IO
			fprintf(stderr,"read_next_line: End of File on STDIN\n");
#else
			stderr_vect("read_next_line: End of File on STDIN\n");
#endif
			exit(0);
		}
	}
}
/***********************+----------------+
|			| read_file_line |
|			+----------------+
| This function is the same as read_next_line(), except that it
| does not go back to the beginning and try to read again on
| EOF.
*/
void read_file_line()
{
	char *status;

	status     = (READ_WA)();
	if(status == 0){
/*		if(INPUT_SOURCE){	/ * EOF on include file, back to stdin */
		if(this_file){	/* EOF on include file, back to stdin */
			close_include();
		}else{			/* EOF on stdin, so exit */
#ifndef VECTORED_C_IO
			fprintf(stderr,"read_file_line: End of File on STDIN\n");
#else
			stderr_vect("read_file_line: End of File on STDIN\n");
#endif
			exit(0);
		}
	}
}
/***********************+----------------+
|			| tabs_to_blanks |
|			+----------------+
|
*/
void tabs_to_blanks(char* string)
{
	char temp[TIBSIZE*2];
	char * orig;
	char ch;
	int  len;
	int  i;
	int  j;

	temp[0]  = '\0';
	orig     = string;
	len      = strlen(string);
	for(i=0,j=0;i<len;i++){
		ch = *string++;
		if(ch == '\t'){
			temp[j++] = ' ';
		}else{
			temp[j++] = ch;
		}
		if(j > (TIBSIZE*2)){
#ifndef VECTORED_C_IO
			fprintf(stderr,"Too many TABS in input line \n");
#else
			stderr_vect("Too many TABS in input line \n");
#endif
		}
	}
	temp[j] = '\0';
	strcpy(orig,temp);
}
/***********************+------------+
|			| read_stdin |
|			+------------+
*/
char *read_stdin()
{
	char *status;

	tib_len = 0;
	while(!tib_len){
		prompt();
		status = fgets(tib, TIBSIZE, stdin);
		if(!status){			/* test for I/O error */
/*
#ifndef VECTORED_C_IO
			fprintf(stderr,"Read Error\n");
#else
			stderr_vect("Read Error\n");
#endif
*/
			QUIT = TRUE;
		}
		IN = 0;
		tabs_to_blanks(tib);		/* basically ignore tabs */
		tib_len = strlen(tib);
		tib_len--;
		tib[tib_len] = '\0';
	}
	return(status);
}
/***********************+--------------+
|			| read_include |
|			+--------------+
*/
char *read_include()
{
	char *status;
		/*
		| If a compile error occurs, the abort flag will be set.
		| Stop compilation in the file by closing the current
		| include file and returning to the caller. 06/27/94 [nes]
		*/
	if(ABORT_FLAG){
		close_include();
		return((char*)-1);
	}
	file_eof = FALSE;
	tib_len  = 0;
	while(!tib_len){
		IN      = 0;
		*tib    = '\0';
		status  = fgets(tib,TIBSIZE,source[this_file].fd);
		if(status){
			tabs_to_blanks(tib);	/* basically ignore tabs */
			tib_len = strlen(tib);
			if(tib_len){
				tib_len--;  	/* try to trash the \n */
				*(tib + tib_len) = '\0';
			}
		}else{				/* this is really EOF */
#ifdef CPP
#ifndef VECTORED_C_IO
			fprintf(stderr,"EOF on include file.\n");
#else
			stderr_vect("EOF on include file.\n");
#endif
#endif
			break;
		}
	}
	return(status);
}
/***********************+-------------+
|			| read_binary |
|			+-------------+
*/
#ifdef SEAL_MODULE
char *read_binary()
{
	char *status;

	file_eof = FALSE;
	tib_len  = 0;
	while(!tib_len){
		IN      = 0;
		*tib    = '\0';
		status  = fgets(tib,TIBSIZE,source[this_file].fd);
		if(status){
			crypt_string(&this_key,tib);
			tabs_to_blanks(tib);	/* basically ignore tabs */
			tib_len = strlen(tib);
			if(tib_len){
				tib_len--;  	/* try to trash the \n */
				*(tib + tib_len) = '\0';
			}
		}else{				/* this is really EOF */
			break;
		}
	}
	return(status);
}
#endif
/***********************+-------------+
|			| open_source |
|			+-------------+
| This function is used to open all input source files. That way
| files may be nested. The READ_WA function is also stored in the
| file descriptor structure. It is used for restoring the proper read
| function after a nested include.
*/
long open_source(char *file_name, long type, char *crypt_key)
{
	long truth = TRUE;

	this_file                  = INPUT_SOURCE;
	INPUT_SOURCE++;
	source[this_file].fd = fopen(file_name,"r");
	if(!source[this_file].fd){		/* null means error */
		INPUT_SOURCE--;			/* reset input counter */
		tib_len  = 0;
		IN       = 0;
                this_file--;
		truth    = FALSE;
		return(truth);
	}
	if(INPUT_SOURCE > (NUM_FILES - 1)){
#ifndef VECTORED_C_IO
		fprintf(stderr,"Too many files open\n");
#else
		stderr_vect("Too many files open\n");
#endif
		abort_F();
	}
	source[this_file].type     = type;
	source[this_file].status   = OPENED;
	source[this_file].position = 0;
	source[this_file].tib      = tib;
	source[this_file].in       = 0;
	source[this_file].name     = (char*) malloc(strlen(file_name)+2);
	strcpy(source[this_file].name,file_name);
	source[this_file].crypt    = (char*) malloc(strlen(crypt_key)+2);
	strcpy(source[this_file].crypt,crypt_key);
	switch(type){
#ifdef SEAL_MODULE
	case BIN:
		READ_WA                   = read_binary ; /* Set current reader */
		source[this_file].read_wa = READ_WA ;
		break;
#endif
	case SOURCE:
		READ_WA                   = read_include ; /* Set current reader */
		source[this_file].read_wa = READ_WA ;
		break;
	default:
#ifndef VECTORED_C_IO
		fprintf(stderr,"ERROR-File: %s, Invalid Type: %ld\n",file_name,type);
#else
		sprintf(tstring,"ERROR-File: %s, Invalid Type: %ld\n",file_name,type);
		stderr_vect(tstring);
#endif
		abort_F();
	}
	tib_len  = 0;
	IN       = 0;
	return(truth);
}
/***********************+-------------+
|			| do_autoload |
|			+-------------+
| After the file is opened, the next time through the outer interpreter
| will read from the autoload file.
|
*/
long do_autoload(char *filename, long type)
{
	long truth;
	char name[64];
	char ext[8];

	base_fn(filename,name,ext);

	truth = type;
	strupr(ext);
	if(!strcmp(ext,"BIN")){		/* false means match */
		truth = open_source(filename,BIN,name);
		binary_input_mode = TRUE;
	}else{
		truth = open_source(filename,SOURCE,"");
		binary_input_mode = FALSE;
	}
	return(truth);
}
/***********************+------------+
|			| open_stdin |
|			+------------+
| This function is used to open stdin as the first item on the file
| stack. 
| 
| Assumes stdin comes for free and does not open it.
*/
void open_stdin()
{
	this_file             = INPUT_SOURCE++;
	source[this_file].fd  = stdin;
	if(!source[this_file].fd){		/* null means error */
		INPUT_SOURCE--;			/* reset input counter */
		tib_len  = 0;
		IN       = 0;
	}
	source[this_file].type     = SOURCE;
	source[this_file].status   = OPENED;
	source[this_file].position = 0;
	source[this_file].tib      = tib;
	source[this_file].in       = 0;
	source[this_file].name     = 0;
	source[this_file].crypt    = 0;
	source[this_file].read_wa  = read_stdin;

	tib_len  = 0;
	IN       = 0;
}
/***********************+---------------+
|			| close_include |
|			+---------------+
| INPUT_SOURCE points to the next entry in the table to open. this_file
| points to the currently open file.
*/
void close_include()
{
	char key[32];
		/*
		| Go back to stdin after all files are closed
		*/
 	if(INPUT_SOURCE  <= 1){
#ifndef VECTORED_C_IO
		fprintf(stderr,"close_include: EOF on STDIN\n");
#else
		stderr_vect("close_include: EOF on STDIN\n");
#endif
		file_eof          = TRUE;
		abort_F();			/* bail out... */
		exit(0);
		return;
	}
	fclose(source[this_file].fd);
		/*
		| Clean up structure for the file just closed...
		*/
	source[this_file].fd          = 0;
	source[this_file].status      = CLOSED;
	source[this_file].position    = 0;
	source[this_file].in          = 0;
	free(source[this_file].name);

	source[this_file].name        = 0;
		/*
		| Adjust the world to use the previous entry in
		| the file table.
		*/
	INPUT_SOURCE--;				/* decrement file #        */
	this_file--;
	IN                = 1;
	tib_len           = 0;
	binary_input_mode = source[this_file].type;
	READ_WA           = source[this_file].read_wa;
	file_eof          = TRUE;
#ifdef SEAL_MODULE
	if(source[this_file].crypt){
		strcpy(key,source[this_file].crypt);
		set_crypt_key(&this_key,key);
	}
#endif
}
/***********************+---------+
|			| base_fn |
|			+---------+
| Be sure that name and ext resolve back to arrays and are not
| just pointers!! Also note that this works only with file names,
| NOT full path names.
|
*/
void base_fn(char *full, char *name, char *ext)
{
		/*
		| This loop extracts the file name portion of the full
		| name string into name.
		*/
	while(*full){
		if(*full != '.'){
			*name++ = *full++;	/* copy char to name */
		}else{
			*name++ = '\0';
			break;
		}
	}
	if(!full){				/* got end of string */
		return;
	}
		/*
		| Now loop through the rest to get the extension...
		*/
	full++;
	while(*full){
		*ext++ = *full++;
	}
	*ext = '\0';
}
/***********************+----------+
|			| finclude |
|			+----------+
|	include source-file
|
| Should be the only command on the input line!!!!
*/
void finclude()
{
	long success;

	next_word();
	success = open_source((pad + 1),SOURCE, "");
	if(!success){
#ifndef VECTORED_C_IO
		fprintf(stderr,"finclude() Error-Could not open %s\n",pad+1);
#else
		sprintf(tstring,"finclude() Error-Could not open %s\n",pad+1);
		stderr_vect(tstring);
#endif
	}
}
/***********************+--------+
|			| U_load |
|			+--------+
|	load ( n --- )
| This word provided for compatability with 'normal' Forth I/O.
*/
void U_load()
{
	drop();
#ifndef VECTORED_C_IO
	fprintf(stderr,"Not implemented.\n");
#else
	stderr_vect("Not implemented.\n");
#endif
}
/***********************+-------+
|			| fload |
|			+-------+
|	fload file.bin
|
| This function is designed to load an Until binary image file.
| The file must have been saved using 'seal' first.
*/
void fload()
{
	long success;
	char name[16];
	char ext[8];

	next_word();
	base_fn((pad + 1),name,ext);
#ifdef SEAL_MODULE
	if(!strcmp(ext,"bin")){		/* false means match */
		set_crypt_key(&this_key,name);
		success = open_source((pad + 1),BIN, name);
#else
	if(!strcmp(ext,"bin")){		/* false means match */
#ifndef VECTORED_C_IO
		fprintf(stderr,
			"fload() Error-Could not open binary file: %s\n",pad+1);
#else
		sprintf(tstring,"fload() Error-Could not open binary file: %s\n",pad+1);
		stderr_vect(tstring);
#endif
#endif
	}else{
		success = open_source((pad + 1),SOURCE, "");
	}
	if(!success){
#ifndef VECTORED_C_IO
		fprintf(stderr,"fload() Error-Could not open %s\n",pad+1);
#else
		sprintf(tstring,"fload() Error-Could not open %s\n",pad+1);
		stderr_vect(tstring);
#endif
	}
}
/***********************+-----------+
|			| span_prim |
|			+-----------+
|	span	( --- addr )
*/
void span_prim()
{
	pushsp((long) &SPAN);
}
/***********************+-------+
|			| u_dot |
|			+-------+
|	u.	( u --- )
*/
void u_dot()
{
	unsigned long value;

	value = (unsigned long) popsp();
#ifndef VECTORED_C_IO
	printf("%ld", value);
#else
	sprintf(tstring,"%ld", value);
	stdout_vect(tstring);
#endif
}
/***********************+-------------+
|			| prim_expect |
|			+-------------+
|	expect	( addr len --- )
*/
void prim_expect(void)
{
	int  i;
	long len;
	char *addr;
	char ch;

	len   = popsp();
	addr  = (char*)popsp();

	for(i=0;i<len;i++){
#ifdef VECTORED_IO
	        key_vect();
#else
		key();
#endif
		ch = (char)popsp();
		if((ch == '\n') || (ch == '\r')){
			*addr = '\0';	/* NULL; */
			SPAN  = i;
			return;
		}
		*addr++ = ch;
#ifdef BCC
#ifndef VECTORED_C_IO
		printf("%c",ch);
#else
		sprintf(tstring,"%c",ch);
		stdout_vect(tstring);
#endif
#endif
	}
	*addr = 0;			/* NULL; */
	SPAN  = i;
		/*
		| flush rest of input line!!
		*/
	FOREVER{
#ifdef VECTORED_IO
		key_vect();		/* return a char to the stack... */
#else
		key();
#endif
		ch = (char)popsp();
		if((ch == '\n') || (ch == '\r')){
			break;
		}
	}
}
/***********************************************************************
|***********************************************************************
|	These are application development type words.
*/
