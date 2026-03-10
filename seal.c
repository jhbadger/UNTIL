/*
|	FILE: seal.c
|
|	This module 'seals' a Until source file. The following steps
|	must be performed:
|
|	1) wrap the source code
|	2) Encrypt the wrapped lines
|	3) Write it to disk
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
|	01/15/95	Corrected bug in unseal so last line of file
|			does not print twice. [nes]
|	01/16/95	Made cleanup changes suggested by Akira Kida,
|			akida@isd.hin.konica.co.jp. [nes]
|	01/21/95	Added code to handle ." and " correctly. [nes]
|	04/16/95	Merged Skip Carter's vectored I/O changes and
|			added code to handle abort" like ." and " [nes]
*/
#include "compiler.h"

#include <stdio.h>
#include <string.h>

#ifndef NO_STDLIB
#include <stdlib.h>
#endif

#include "until.h"
#include "functs.h"

#define WRAP_MARGIN	70

#ifdef SEAL_MODULE

FILE *fd_out;
FILE *fd_in;

/***********************+------------+
|			| Sread_line |
|			+------------+
*/
void Sread_line()
{
	char *status;

	status     = (READ_WA)();
	if(status == 0){
		seal_eof = TRUE;
	}
}
/***********************+-------+
|                       | Sword |
|                       +-------+
|  This word extracts the next word from the input stream (tib)
|  and places it in pad. The parameter stack is used to pass
|  and return arguments.
|
|  NOTE: tib_len must have been set by the input function prior
|        to calling word().
|
|       word()          ( delim --- 'pad )
*/
void Sword()
{
	char ch;
	char delim;             /* word delimiter */
	int  i;
	long X;

	if(IN  >= tib_len){
		Sread_line();       		/* read next line from stream */
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
			Sread_line();  		/* read next line from stream */
		}
	}
		/*
		|  Collect chars up to a match on delim. Chars are
		|  loaded into pad starting at pos. 1. pad[0] is
		|  reserved for the length byte.
		*/
	i = 1;
/*
Continue_Reading:
*/
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
/***********************+------+
|			| seal |
|			+------+
| Used in the form:	{ file.app} seal
|
| Changes that need to be made include:
| 1) Do not take whitespace out of constants
|
| Constants include ." " and {
*/
void seal()
{
	long len;
	long wlen;
	long delem;

	char file_in[80];
	char file_ext[8];
	char file_out[80];
	char save_key[80];
	char out_buf[256];
	struct encrypt_key the_key;

	delem = ' ';
		/*
		| Get input file and generate output file name from it
		*/
	next_word();
	strcpy(file_in,pad+1);
	base_fn(file_in, file_out, file_ext);
	set_crypt_key(&the_key,file_out);	/* set up encryption key */
	strcpy(save_key,file_out);
	strcat(file_out,".bin");

	open_source(file_in,(long)SOURCE,file_out);
	FREAD_WA = (vectoredFunction)read_file_line;

	fd_out   = fopen(file_out,"w");

		/*
		| start the main loop...
		*/
	len        = 0;
	out_buf[0] = '\0';
	seal_eof   = FALSE;
		/*
		| Main loop; get each word from the input stream
		| crypt it, and write it to the output file.
		| Blanks are removed, except for ." and " strings
		| and comments are dropped completely.
		*/
	FOREVER{
		if(seal_eof){
			break;  }
		pushsp(delem);
		Sword();
		drop();
		wlen = strlen(pad+1);
			/*
			| Handle " and ." and abort" strings
			*/
		if((*(pad+1) == '"') || 		/* "         */
		   (*(pad+2) == '"') ||			/* ."        */
		   (*(pad+6) == '"')){			/* abort"    */
			if(*(pad+1) == '.'){
				strcat(out_buf,".");
			}
			if(!strncmp((pad+1),"abort",5)){
				strcat(out_buf,"abort");
			}
			pushsp((long)'"');
			Sword();
			drop();
			wlen = strlen(pad+1);
			strcat(out_buf,"\" ");
			strcat(out_buf,pad+1);
			strcat(out_buf,"\" ");
			write_crypt(&the_key,out_buf);
			len = 0;
			out_buf[0] = '\0';
			continue;
		}
			/*
			| ignore \ comments
			*/
		if(wlen == 1 && *(pad +1) == '\\'){	/* comment to eol */
			IN = 999;
			continue;
		}
			/*
			| ingore ( ... ) comments
			*/
		if(wlen == 1 && *(pad + 1) == '('){	/* comment to eol */
			pushsp((long)')');
			Sword();		/* nes 01/15/94 */
			drop();
			continue;
		}
			/*
			| Normal processing
			*/
		if((len + wlen) > WRAP_MARGIN){
			write_crypt(&the_key, out_buf);
			len = 0;
			out_buf[0] = '\0';
		}
		strcat(out_buf,pad + 1);
		strcat(out_buf," ");
		len += wlen + 1;
		/* put tests for " etc here and set delem */
	}
		/*
		| flush last line of file...
		*/
	write_crypt(&the_key, out_buf);
	out_buf[0] = '\0';
	FREAD_WA   = (vectoredFunction)read_next_line;
	fclose(fd_out);
}
/***********************+---------+
|			| un_seal |
|			+---------+
| Used in the form:	{ file.app} unseal
|
| Changes that need to be made include:
| 1) Do not take whitespace out of constants
|
| Constants include ." " and {
*/
void unseal()
{
/*
	long len;
	long wlen;
	long delem;
	long i;
*/
	char file_in[80];
	char file_ext[8];
	char file_out[80];
	char save_key[80];
	char out_buf[128];
	struct encrypt_key the_key;

/*
	delem = ' ';
*/
		/*
		| Get input file and generate output file name from it
		*/
/*
	strcpy(file_in,here_here);
*/
	next_word();
	strcpy(file_in,pad+1);
	base_fn(file_in, file_out, file_ext);
	set_crypt_key(&the_key,file_out);	/* set up encryption key */
	strcpy(save_key,file_out);
	strcat(file_out,".bin");

		/*
		| start the main loop...
		*/
/*
	len        = 0;
*/
	out_buf[0] = '\0';
	seal_eof   = FALSE;
	set_crypt_key(&the_key,save_key);
	fd_in = fopen(file_out,"r");
		/*
		| This is a crude first pass...
		*/
	FOREVER{
		read_crypt(&the_key,out_buf);
		if(seal_eof){
			break;  }
#ifndef VECTORED_C_IO
		fprintf(stderr,"%s",out_buf);
#else
		sprintf(tstring,"%s",out_buf);
		stderr_vect(tstring);
#endif
		out_buf[0] = '\0';
	}
	fclose(fd_in);

}
/***********************+-------------+
|			| write_crypt |
|			+-------------+
| This function writes the contents of the string to the currently
| open output file. The output line is encrypted.
*/
void write_crypt(struct encrypt_key *the_key,char * string)
{
	char *start;
	char ch;

	start = string;
	while(*string){
		ch        = *string;
		ch        = U_crypt(the_key, ch);
		*string++ = ch;
	}
	*string++ = '\n';
	*string   = '\0';
	fputs(start,fd_out);
}
/***********************+------------+
|			| read_crypt |
|			+------------+
| This function writes the contents of the string to the currently
| open output file. The output line is encrypted.
*/
void read_crypt(struct encrypt_key *the_key,char *string)
{
	char *status;
	char buffer[128];
	char ch;

	status = fgets(buffer,128,fd_in);
	if(!status){		/* Test for EOF  */
		seal_eof = TRUE;
		return;
	}
	strcpy(string,buffer);
	while(*string){
		ch        = *string;
		ch        = U_crypt(the_key, ch);
		*string++ = ch;
	}
	string--;
	*string++ = '\n';
	*string   = '\0';
}
/***********************+-------+
|			| crypt |
|			+-------+
| This function encrypts the character passed to is based on a
| user specified encryption key. The algorithm is a simple
| character XOR funciton. The key must be specified separately.
*/
char U_crypt(struct encrypt_key *the_key,char ch)
{
	char key_ch;

	key_ch = the_key->crypt_key[the_key->crypt_offset++];
	if(the_key->crypt_offset >= the_key->crypt_len){
		the_key->crypt_offset = 0;	/* reset offset */
	}
/*
	key_ch = 255    | key_ch;
*/
	key_ch = (char)255;
	ch     = key_ch ^ ch;
	return(ch);
}
/***********************+--------------+
|			| crypt_string |
|			+--------------+
| This function encrypts the character passed to is based on a
| user specified encryption key. The algorithm is a simple
| character XOR funciton. The key must be specified separately.
*/
void crypt_string(struct encrypt_key *the_key,char* string)
{
	char ch;

	while(*string){
		ch = *string;
		ch = U_crypt(the_key, ch);
		*string++ = ch;
	}
	string--;
	*string++ = '\0';
}
/***********************+---------------+
|			| set_crypt_key |
|			+---------------+
| This function must be called prior to calling crypt() the first time
| to set up the encryption key.
*/
void set_crypt_key(struct encrypt_key *the_key, char *key)
{
	strcpy(the_key->crypt_key,key);
	the_key->crypt_offset = 0;
	the_key->crypt_len    = strlen(key);
}
#endif

