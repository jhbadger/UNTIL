/*
|	FILE: HELP.C
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
| CHANGES:
| DATE		DESCRIPTION
| 01/12/95	Replaced help() with version contributed by
|		Michael Mundrane of Rutgers University.
| 01/16/95	Made cleanup changes suggested by Akira Kida,
|		akida@isd.hin.konica.co.jp. [nes]
*/

#include "compiler.h"

#include <stdio.h>
#include <string.h>

#ifndef NO_STDLIB
#include <stdlib.h>
#endif

/*
#ifdef ANSIC
#include <conio.h>
#include <time.h>
#endif

#if defined( OSF ) || defined( SPARC )
#include <time.h>
#endif
*/

#include "until.h"

extern void next_word();

/***********************+------+
|			| help |
|			+------+
*/
#ifdef IGNORE
void X_help()
{
	int  i;
	int  j;
	int  match;
	int  truth;
	int  help_file;
	FILE *FD;

	char *status;
	char line[132];
	char string[32];

/*
	help_file	  = INPUT_SOURCE;
*/
	FD = fopen("HELP.APP","r");
	if(!FD){
#ifndef VECTORED_C_IO
		fprintf(stderr,"Help file not found\n");
#else
		stderr_vect("Help file not found\n");
#endif
		return;
	}
	truth  = FALSE;
/*
	for(i=0;i<32;i++){
		*(pad + i) = 0;
	}
*/
	next_word();
	status = fgets(line,132,FD);
	while(status){				/* do until eof */
		if(line[0] != BLANK){
			j = 0;
			i = 0;
			while(line[i] != BLANK){
				string[j++] = line [i++];
			}
			string[j] = '\0';
			match     = strcmp(string,&pad[1]);
			if(match == 0){
				truth  = TRUE;
#ifndef VECTORED_C_IO
				printf("%s",line);
#else
				stderr_vect(line);
#endif
				status = fgets(line,132,FD);
				if(!status) goto All_done;
				while(line[0] == BLANK){
#ifndef VECTORED_C_IO
					printf("%s",line);
#else
					stderr_vect(line);
#endif
					status = fgets(line,132,FD);
				}
				goto All_done;
			}else{
				status = fgets(line,132,FD);
				if(!status) goto All_done;
			}
		}else{
			status = fgets(line,132,FD);
			if(!status) goto All_done;
		}
	}
All_done:
	fclose(FD);
	if(!truth){
#ifndef VECTORED_C_IO
		printf("Macro not found in Help file.\n");
#else
		stderr_vect("Macro not found in Help file.\n");
#endif
	}
}
#endif
/*
| NOTE: This function contributed by Michael Mundrane of
| 	Rutgers University, Dept. of Mechanical & Aerospace
|	Engineering.
*/
void help (void)
{
   FILE *FD;
   
   next_word();

   if ((FD = fopen("help.app", "r")) != NULL)
   {
      FOREVER{
         char line[132];
         
         if (fgets(line, 132, FD) == NULL)
         {
#ifndef VECTORED_C_IO
            fprintf(stderr,"Macro not listed in help file.\n");
#else
            stderr_vect("Macro not listed in help file.\n");
#endif
            break;
         }
         
         if (line[0]                      == pad[1] &&
             strcspn(line, " ")           == pad[0] &&
             strncmp(line, pad+1, pad[0]) == 0         )
         {
            do
            {
#ifndef VECTORED_C_IO
		fprintf(stderr,"%s", line);
#else
		stderr_vect(line);
#endif
            } while (fgets(line, 132, FD) && line[0] == ' ');
            break;
         }
      }
      
      fclose(FD);
   }
   else
#ifndef VECTORED_C_IO
      fprintf(stderr,"Help file not found\n");      
#else
      stderr_vect("Help file not found\n");      
#endif
}


