/*
|	FILE: SEARCH.C
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
/********************************************************************
*********************************************************************
This module adds a very basic, brute force search and replace
capability to Until. Searching is done by examining each character
of the source string for the first character of the search string. When
a match occurs, the rest of the search string is compared. This continues
until either a match or end of the string.

Memory is allocated dynamically, so be sure to call clear_temp
when done to give the memory back.

Date		Change
11/20/94	Initial coding as a stand alone program. [nes]
11/24/94	Converted to run with Until and performed initial
		testing. [nes]	
01/09/94	Changed several functions to return truth based
		on search instead of string addresses. [nes]
01/10/95	Corrected bug in search(). [nes]
01/16/95	Made cleanup changes suggested by Akira Kida,
		akida@isd.hin.konica.co.jp. [nes]
*********************************************************************
********************************************************************/
#include "compiler.h"

#include <stdio.h>
#include <string.h>

#ifndef NO_STDLIB
#include <stdlib.h>
#endif

#ifdef SEARCH_MODULE

#include "until.h"
#include "functs.h"

#include "search.h"

/********************************************************************
*********************************************************************
|		Global Variables
*/
int  Temporary_String_len = 0;
char *Temporary_String    = 0;

/********************************************************************
*********************************************************************
|	This section is the low level C function that perform the
|	various functions of the search and replace module.
*********************************************************************
********************************************************************/
/***********************+--------+
|			| search |
|			+--------+
| Returns 0 for no match or address of the match.
*/
char *search(char *string, char* sub)
{
	int  len;
	int  slen;
	int  i;
	char *truth = 0;
	char *status;
	char first;
	char ch;

	slen  = strlen(sub);
	len   = strlen(string);
	first = *sub;
	for(i=0;i<len;i++){
		ch = *string;
		if(first == ch){
			status = (char*) strncmp(string,sub,slen);
			if(!status){	/* got a match  */
				truth = string;
				break;
			}
		}
		string++;
	}
	return(truth);
}
/***********************+--------+
|			| insert |
|			+--------+
| Returns address of original string.
*/
char *insert(char* string, char* new_str)
{
	int    len;
	int    new_len;
	char   *original;

	new_len = strlen(string) + strlen(new_str);
	if(!Temporary_String_len){		/* must be malloc'd the first time */
		Temporary_String      = (char*) malloc(new_len);
#ifdef CHECK_MALLOC
		if(!Temporary_String){
			printf("insert: Could not allocate new string.\n");
			abort_F();
			return(0);
		}
#endif
		Temporary_String_len  = new_len;
	}
	if(new_len > Temporary_String_len){
		Temporary_String      = (char*) realloc(Temporary_String,new_len);
#ifdef CHECK_MALLOC
		if(!Temporary_String){
			printf("insert: Could not re-allocate new string\n");
			abort_F();
			return(0);
		}
#endif
		Temporary_String_len  = new_len;
	}
	original = string;
	strcpy(Temporary_String,new_str);
	strcat(Temporary_String,string);
	strcpy(original,Temporary_String);
	return(original);
}
/***********************+---------+
|			| replace |
|			+---------+
| string  - the replace point in the string
| new_str - the string to be inserted
| sub     - the substring being replaced
*/
char *replace(char* string, char* new_str, char* sub)
{
	int    len;
	int    new_len;
	char   *original;

	new_len  = strlen(string) + strlen(new_str) + 2;
	if(!Temporary_String_len){		/* must be malloc'd the first time */
		Temporary_String = (char*) malloc(new_len);
#ifdef CHECK_MALLOC
		if(!Temporary_String){
			printf("replace: Could not allocate temporary string\n");
			abort_F();
			return(0L);
		}
#endif
		Temporary_String_len = new_len;
	}
	if(new_len > Temporary_String_len){
		Temporary_String      = (char*) realloc(Temporary_String,new_len);
#ifdef CHECK_MALLOC
	if(!Temporary_String){
		printf("replace: Could not allocate temporary string.\n");
		abort_F();
		return(0L);
	}
#endif
		Temporary_String_len  = new_len;
	}
	original          = string;
	strcpy(Temporary_String,new_str);	/* Insert replacement */
	len               = strlen(sub);
	string           += len;		/* Skip substring */
	strcat(Temporary_String,string);	/* Append what's left of string */
	strcpy(original,Temporary_String);	/* Replace original string */
	return(original);
}
/***********************+---------------+
|			| search_insert |
|			+---------------+
*/
char *search_insert(char* string, char *srch, char *insrt)
{
	int  len;
	char *truth;

	truth = search(string,srch);
	if(truth){
/*		truth = insert(truth,insrt);
*/
		truth = truth + (strlen(srch));
		insert(truth,insrt);
	}
	return(truth);
}
/***********************+----------------+
|			| search_replace |
|			+----------------+
*/
char *search_replace(char* string, char *srch, char *repl)
{
	char *truth;

	truth = search(string,srch);
	if(truth){
/*		truth = replace(truth,repl,srch);
*/
		replace(truth,repl,srch);
	}
	return(truth);
}
/***********************+-----------------+
|			| gsearch_replace |
|			+-----------------+
*/
int gsearch_replace(char* string, char *srch, char *repl)
{
	int  matches = 0;
	int  len;
	char *truth;

	len = strlen(repl);
	FOREVER{
		truth = search(string,srch);
		if(truth){
			truth  = replace(truth,repl,srch);
			string = truth + len;
			matches++;
		}else{
			break;
		}
	}
	return(matches);
}
/***********************+------------------------+
|			| clear_Temporary_String |
|			+------------------------+
*/
void clear_Temporary_String()
{
	free(Temporary_String);
	Temporary_String_len = 0;
}
/********************************************************************
*********************************************************************
|	This section is the low level C function that perform the
|	various functions of the search and replace module.
*********************************************************************
********************************************************************/
/***********************+-------------+
|			| call_search |
|			+-------------+
*/
void call_search()
{
	char *match;	
	char *string;
	char *sub;
	
	sub    = (char*)popsp();
	sub    = (char*)R_addr;
	string = (char*)popsp();
	string = (char*)R_addr;
	match  = search(string, sub);
	pushsp((long)match);
}
/***********************+-------------+
|			| call_insert |
|			+-------------+
*/
void call_insert()
{
	char *match;	
	char *string;
	char *new_str;
	
	new_str = (char*)popsp();
	string  = (char*)popsp();
	match   = insert(string, new_str);
	pushsp((long)match);
}
/***********************+--------------+
|			| call_replace |
|			+--------------+
| ( 'string 'new_string 'replace --- tf )
*/
void call_replace()
{
	char *match;	
	char *string;
	char *new_str;
	char *sub;
	
	sub     = (char*)popsp();
	new_str = (char*)popsp();
	string  = (char*)popsp();
	match   = replace(string, new_str, sub);
	pushsp((long)match);
}
/***********************+--------------------+
|			| call_search_insert |
|			+--------------------+
| ( 'string 'search 'insert --- tf )  
*/
void call_search_insert()
{
	char *match;	
	char *string;
	char *new_str;
	char *insrt;
	
	insrt   = (char*)popsp();
	new_str = (char*)popsp();
	string  = (char*)popsp();
	match   = search_insert(string, new_str, insrt);
	pushsp((long)match);
}
/***********************+---------------------+
|			| call_search_replace |
|			+---------------------+
| ( 'string 'search 'replace --- tf )  sub  
|
| False = no match
*/
void call_search_replace()
{
	char *match;	
	char *string;
	char *srch;
	char *repl;
	
	repl    = (char*)popsp();
	srch    = (char*)popsp();
	string  = (char*)popsp();
	match   = search_replace(string, srch, repl);
	pushsp((long)match);
}
/***********************+----------------------+
|			| call_gsearch_replace |
|			+----------------------+
| ( 'string 'search 'replace --- nmatches )  gsub
*/
void call_gsearch_replace()
{
	int  match;
	char *string;
	char *srch;
	char *repl;
	
	repl    = (char*)popsp();
 	srch    = (char*)popsp();
	string  = (char*)popsp();
	match   = gsearch_replace(string, srch, repl);
	pushsp((long)match);
}
/***********************+-----------------+
|			| call_clear_temp |
|			+-----------------+
*/
void call_clear_temp()
{
	clear_Temporary_String();
}

#endif
