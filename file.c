/*
|	FILE: FILE.C
|
|	This module contains the binding of C File/I/O library
|	functions into Until.
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
|	27-AUG-1994	Split FILE.C off from IO.C. [nes]
|	28-AUG-1994	Wrote initial generic version of FN_SPLIT(). [nes]
|	31-AUG-1994	Final version of fn_split().[nes]
*/

#include "compiler.h"

#ifdef CIO_MODULE

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

#define MAX_DRIVE	32
#define MAX_DIR		128
#define MAX_FILE	128
#define MAX_EXT		128

/***********************************************************************
|***********************************************************************
|		These are file I/O words
*/
#ifdef BCC
void file_fnsplit()
{
	char *fname;
	static char drive[MAX_DRIVE];
	static char dir[MAX_DIR];
	static char file[MAX_FILE];
	static char ext[MAX_EXT];

	fname = (char*)popsp();
	fnsplit(fname,drive,dir,file,ext);
	pushsp((long)ext);
	pushsp((long)file);
	pushsp((long)dir);
	pushsp((long)drive);
}
#else

#define EXT     0
#define FNAME	1
#define DIR	2
#define DDRIVE	3
#define SOS     4
#define DONE	5

/***********************+-------------+
|			| reverse_str |
|			+-------------+
*/
int reverse_str(char *string)
{
    int  i;
    int  len;
    int  truth = FALSE;
    char *orig = string;
    char temp[260];      /* 255 plus slop */

    len = strlen(string);
    if(len >255){
        truth = TRUE;
        return(truth);
    }
    string += len - 1;
    for(i=0;i<len;i++){
        temp[i] = *string--;
    }
    temp[i] = '\0';
    strcpy(orig,temp);
    return(truth);
}
/***********************+------------+
|			| proc_fname |
|			+------------+
*/
int proc_fname(char *f_name,char ch)
{
	int  F_state = FNAME;

	if(ch == '\\' || ch == '/' || ch == ']' || ch == '['){
		*f_name   = '\0';	/* close off string          */
		F_state   = DONE;
		return(F_state);
	}
	if(ch == ':'){
		*f_name   = '\0';	/* close off string          */
		F_state   = DDRIVE;
		return(F_state);
	}
	*f_name = ch;
	return(F_state);
}
/***********************+----------+
|			| proc_ext |
|			+----------+
*/
int proc_ext(char *ext,char ch)
{
	int  F_state = EXT;

	if(ch == '.'){
		F_state = DONE;
		*ext++  = ch;
		*ext    = '\0';
		return(F_state);
	}
	if(ch == '\\' || ch == '/' || ch == ']' || ch == '['){
		*ext     = '\0';	/* close off string          */
		F_state  = DIR;
		return(F_state);
	}
	if(ch == ':'){
		*ext     = '\0';	/* close off string          */
		F_state  = DDRIVE;
		return(F_state);
	}
	*ext = ch;
	return(F_state);
}
/***********************+--------------+
|			| file_fnsplit |
|			+--------------+
*/
void file_fnsplit()
{
	int  i;
	int  len;
	int  done;
	int  F_state;
	char *fname;
	char ch;

	char *ptr_drive;
	char *ptr_dir;
	char *ptr_file;
	char *ptr_ext;

	static char drive[MAX_DRIVE];
	static char dir[MAX_DIR];
	static char file[MAX_FILE];
	static char ext[MAX_EXT];

	ptr_drive = drive;
	ptr_dir   = dir;
	ptr_file  = file;
	ptr_ext   = ext;

	fname     = (char*)popsp();
	drive[0]  = '\0';
	dir[0]    = '\0';
	file[0]   = '\0';
	ext[0]    = '\0';
	len       = strlen(fname);
	i         = 0;
	done      = FALSE;
	F_state   = EXT;
        len--;				/* backup to last char in string */
		/*
		| Verify that something was passed....
		*/
	if(len < 0){
#ifndef VECTORED_C_IO
		fprintf(stderr,"fn_split: Empty input string \n");
#else
		sprintf(tstring,"fn_split: Empty input string \n");
		stderr_vect(tstring);
#endif
		pushsp((long)0);
		pushsp((long)0);
		pushsp((long)0);
		pushsp((long)0);
		return;
	}
/*******************************************************************
|			Handle File Extension
|*******************************************************************
*/
		/*
		| Loop looking for file extension
		*/
	FOREVER{
		ch = *(fname + len);		/* Get next char  */

		switch(F_state){
		case EXT:
			F_state          = proc_ext(ptr_ext,ch);
			ptr_ext++;
			if(F_state      == DONE){
				F_state  = FNAME;
			}
			if(F_state      == DIR){
				strcpy(file,ext);	/* no extension-> file name  */
			}
			len--;
			if(len < 0){		/* Test for SOS              */
				F_state  = SOS;
				*ptr_ext = '\0';	/* close off string          */
				strcpy(file,ext);	/* no extension-> file name  */
				ptr_ext  = ext;
				ext[0]   = '\0';
			}
			break;
/*******************************************************************
|			Handle File Name
|*******************************************************************
*/
		case FNAME:
			F_state = proc_fname(ptr_file,ch);
			ptr_file++;
			if(F_state      == DONE){
				F_state  = DIR;
				len++;
			}
			len--;
			if(len < 0){		/* Test for SOS              */
				F_state  = SOS;
				*ptr_file = '\0';	/* close off string          */
				ptr_file  = ext;
			}
			break;
/*******************************************************************
|			Handle Directory Name
|*******************************************************************
*/
		case DIR:
			if(ch == ':'){
				dir[i]   = '\0';	/* close off string          */
				done     = TRUE;
				i        = 0;
				F_state  = DDRIVE;
			}else{
				dir[i++] = ch;
				len--;
				if(len < 0){		/* Test for SOS              */
					F_state  = SOS;
					dir[i]   = '\0';	/* close off string          */
				}
			}
			break;
/*******************************************************************
|			Handle Disk Drive Name
|*******************************************************************
*/
		case DDRIVE:
			drive[i++] = ch;
			len--;
			if(len < 0){		/* Test for SOS              */
				F_state    = SOS;
				drive[i]   = '\0';	/* close off string          */
			}
			break;
/*******************************************************************
|			Handle Start of String
|*******************************************************************
*/
		case SOS:
#ifndef VECTORED_C_IO
			fprintf(stderr,"fn_split: Invalid F_state value!!\n");
#else
			sprintf(tstring,"fn_split: Invalid F_state value!!\n");
			stderr_vect(tstring);
#endif
		}
		if(F_state == SOS){
			break;
		}
	}
		/*
		| Strings must be mirrored before done...
		*/
	reverse_str(ext);
	reverse_str(file);
	reverse_str(dir);
	reverse_str(drive);

	pushsp((long)ext);
	pushsp((long)file);
	pushsp((long)dir);
	pushsp((long)drive);

}
#endif
/***********************+------------+
|			| file_fopen |
|			+------------+
|	" filename" READ fopen
|
| This function does an fopen.
*/
void file_fopen()
{
	long fd;
	char *fmode;
	char *file_name;

	fmode     = (char*)popsp();
	file_name = (char*)popsp();

	fd    = (long) fopen(file_name,fmode);
	pushsp(fd);
}
/***********************+-------------+
|			| file_fclose |
|			+-------------+
|	fclose	    ( FD --- )
|
| This function does an fclose
*/
void file_fclose()
{
	FILE *fd;

	fd = (FILE *)popsp();
	fclose(fd);
}
/***********************+------------+
|			| file_fgetc |
|			+------------+
|	fgetc	( fd --- c )
*/
void file_fgetc()
{
	FILE *fd;
	char ch;

	fd = (FILE*) popsp();
	ch = fgetc(fd);
	pushsp(ch);
}
/***********************+------------+
|			| file_fgets |
|			+------------+
|	fgets	( addr size fd --- count )
|
| count of 0 means EOF
*/
void file_fgets()
{
	FILE *fd;
	char *start;
	char *buffer;
	char *status;
	long count;
	long size;

	count  = 0;
	fd     = (FILE*)popsp();
	size   = popsp();
	buffer = (char*)popsp();
	start  = buffer;
	status = fgets(++buffer,size,fd);
	if(status){
		count  = strlen(buffer);
		*start = (char)count;
	}
	pushsp(count);
}
/***********************+------------+
|			| file_fputc |
|			+------------+
|	fputc	( ch fd --- status )
*/
void file_fputc()
{
	char ch;
	FILE *fd;
	long status;

	fd     = (FILE*)popsp();
	ch     = (char)popsp();
	status = fputc(ch,fd);
	pushsp(status);
}
/***********************+------------+
|			| file_fputs |
|			+------------+
|	fputs	( buffer fd --- status )
*/
void file_fputs()
{
	char *buffer;
	FILE *fd;
	long status;

	fd     = (FILE*)popsp();
	buffer = (char*)popsp();
	status = fputs(buffer,fd);
	pushsp(status);
}
/***********************+----------+
|			| file_fcr |
|			+----------+
|	fcr	( fd --- status )
*/
void file_fcr()
{
	FILE *fd;
	long status;

	fd     = (FILE*)popsp();
	status = fputc('\n',fd);
	pushsp(status);
}
/***********************+-------------+
|			| file_fflush |
|			+-------------+
*/
void file_fflush()
{
	FILE *fd;

	fd = (FILE*)popsp();
	fflush(fd);
}
/***********************+------------+
|			| file_ftell |
|			+------------+
|	ftell	( fd --- file_offset )
*/
void file_ftell()
{
	FILE *fd;
	long offset;

	fd     = (FILE*) popsp();
	offset = ftell(fd);
	pushsp(offset);
}
/***********************+------------+
|			| file_fseek |
|			+------------+
|	fseek	( whence file_offset fd --- status )
*/
void file_fseek()
{
	FILE *fd;
	long whence;
	long offset;
	long status;

	fd     = (FILE*) popsp();
	offset = popsp();
	whence = popsp();
	status = fseek(fd,offset,whence);
	pushsp(status);
}
/***********************+------------+
|			| file_fread |
|			+------------+
|	fread	( addr len fd --- status )
*/
void file_fread()
{
	FILE *fd;
	char *buffer;
	long number;
	long status;

	fd     = (FILE*) popsp();
	number = popsp();
	buffer = (char*) popsp();

	status = fread(buffer,1,number,fd);
	pushsp(status);
}
/***********************+-------------+
|			| file_fwrite |
|			+-------------+
|	fwrite	( addr number fd --- status )
*/
void file_fwrite()
{
	FILE *fd;
	char *buffer;
	long number;
	long status;

	fd     = (FILE*) popsp();
	number = popsp();
	buffer = (char*) popsp();

	status = fwrite(buffer,1,number,fd);
	pushsp(status);
}
/***********************+---------------+
|			| file_fputline |
|			+---------------+
|	fputline ( string fd --- status )
*/
void file_fputline()
{
	FILE *fd;

	fd = (FILE*)popsp();
	pushsp((long)fd);
	file_fputs();
	pushsp((long)fd);
	file_fcr();
}
/***********************+---------+
|			| ft_read |
|			+---------+
*/
void ft_read()
{  pushsp((long)READ); }
/***********************+----------+
|			| ft_write |
|			+----------+
*/
void ft_write()
{  pushsp((long)WRITE); }
/***********************+-----------+
|			| ft_append |
|			+-----------+
*/
void ft_append()
{  pushsp((long)APPEND); }
/***********************+-----------+
|			| ft_binary |
|			+-----------+
*/
void ft_binary()
{  pushsp((long)BINARY); }
/***********************+---------+
|			| ft_text |
|			+---------+
*/
void ft_text()
{  pushsp((long)TEXT); }
/***********************+-----------+
|			| ft_update |
|			+-----------+
*/
void ft_update()
{  pushsp((long)UPDATE); }
/***********************+---------------+
|			| ft_new_update |
|			+---------------+
*/
void ft_new_update()
{  pushsp((long)NEW_UPDATE); }
/***********************+------------------+
|			| ft_append_update |
|			+------------------+
*/
void ft_append_update()
{  pushsp((long)APPEND_UPDATE); }

/***********************+------------+
|			| file_stdin |
|			+------------+
| Usage:
|	buffer STDIN fgets
|
| This function supplied by Michael Mundrane
*/
void file_stdin (void)
{
   pushsp((long)stdin);
}
/***********************+-------------+
|			| file_stdout |
|			+-------------+
| This function supplied by Michael Mundrane
*/
void file_stdout (void)
{
   pushsp((long)stdout);
}
/***********************+-------------+
|			| file_stderr |
|			+-------------+
| This function supplied by Michael Mundrane
*/
void file_stderr (void)
{
   pushsp((long)stderr);
}
#endif

