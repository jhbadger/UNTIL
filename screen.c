/*
|	SCREEN.C
|	dummy routines to for functions not available from VAX C
|
|	DATE		COMMENT
|	13-Sep-94	rcs
|	01/16/95	Made cleanup changes suggested by Akira Kida,
|			akida@isd.hin.konica.co.jp. [nes]
*/

#include "compiler.h"

#include <stdio.h>
#include <string.h>

#ifndef NO_STDLIB
#include <stdlib.h>
#endif

#ifdef ANSIC
#include <conio.h>
#endif

#include "until.h"
#include "functs.h"

#ifndef BCC
#ifndef DUMMY
void gotoxy()  {}
void wherey()  {}
#endif
#endif

/***********************+-----+
|                       | cls |
|                       +-----+
*/
void cls()
{
	int i;

#if defined( BCC ) || defined( DUMMY ) 
	clrscr();
#else


	for(i=0;i<25;i++){
#ifndef VECTORED_C_IO
		printf("\n");
#else
		stdout_vect("\n");
#endif
	}
#endif
}
/***********************+------------------+
|			| left_margin_addr |
|			+------------------+
|	left_margin	( n --- )
| Set screen left margin so center() can calcualate screen
| width.
*/
void left_margin_addr()
{
	pushsp((long)&left_margin);
}
/***********************+-------------------+
|			| right_margin_addr |
|			+-------------------+
|	right_margin	( n --- )
| Set screen right margin so center() can calcualate screen
| width.
|
*/
void right_margin_addr()
{
	pushsp((long)&right_margin);
}
/***********************+--------+
|			| center |
|			+--------+
| This version of center is tied closely to S-Viewer. It has been
| modified considerably from the original version in Until.
*/
#ifdef BCC
void center(char *string)
{
	long pos;
	long width;

	pos    = strlen(string);
	width  = right_margin - left_margin;
	pos    = (width - pos) / 2;
	gotoxy(pos,wherey());
#ifndef VECTORED_C_IO
	printf("%s",string);
#else
	sprintf(tstring,"%s",string);
	stdout_vect(tstring);
#endif
}
#else
void center(char *string)
{
	long pos;
	long width;

	pos    = strlen(string);
	width  = right_margin - left_margin;
	pos    = (width - pos) / 2;
	for(;pos;pos--){
#ifndef VECTORED_C_IO
		printf(" ");
#else
		sprintf(tstring," ");
		stdout_vect(tstring);
#endif
	}
#ifndef VECTORED_C_IO
	printf("%s",string);
#else
	sprintf(tstring,"%s",string);
	stdout_vect(tstring);
#endif
}
#endif
/************************************************************
|************************************************************
| Note: The code here can/should be replaced with curses
|	code...
| Most screen applications can get by with these minimal
| cursor commands. 
| 
| Also note: All of these can easily be implemented in high
|	level code.
|************************************************************
*************************************************************/
#ifdef ANSI_CONSOLE

#define HOME	0
#define CLS	1
#define GOTOXY	2
#define DEL_EOL	3
#define NORMAL	4
#define BOLD	5

char *esc_seq[] = {
	"\x1b[H",	/* Home cursor     */
	"\x1b[2J",	/* Clear screen    */
	"\x1b[%d;%df",	/* Position cursor */
	"\x1b[0K",	/* Erase to EOL    */
	"\x1b[0m",	/* Normal video    */
	"\x1b[1m",	/* Bold on         */
	""
};
/***********************+---------+
|			| scr_cls |
|			+---------+
*/
void scr_cls()
{
	printf(esc_seq[CLS]);
}
/***********************+----------+
|			| scr_home |
|			+----------+
*/
void scr_home()
{
	printf(esc_seq[HOME]);
}
/***********************+---------------+
|			| scr_erase_eol |
|			+---------------+
*/
void scr_erase_eol()
{
	printf(esc_seq[DEL_EOL]);
}/***********************+-----------------+
|			| scr_normal_video |
|			+------------------+
*/
void scr_normal_video()
{
	printf(esc_seq[NORMAL]);
}/***********************+---------------+
|			| scr_bold_video |
|			+----------------+
*/
void scr_bold_video()
{
	printf(esc_seq[BOLD]);
}/***********************+-----------+
|			| scr_gotoxy |
|			+------------+
|   1 5 gotoxy  ( row1 col5 --- )
|
| Need to add checks for valid data...
*/
void scr_gotoxy()
{
	long row;
	long col;
	
	col = popsp();
	row = popsp();
	printf(esc_seq[GOTOXY],row,col);
}

#endif

