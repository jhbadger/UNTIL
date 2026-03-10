/*
|	FILE: SEARCH.H
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
/*
|		Function Prototypes
*/
char *search(char *, char*);
char *insert(char*, char*);
char *replace(char*, char*, char*);
char *search_insert(char*, char *, char *);
char *search_replace(char*, char *, char *);
void clear_Temporary_String();

void call_search();
void call_insert();
void call_replace();
void call_search_insert();
void call_search_replace();
void call_gsearch_replace();
void call_clear_temp();
