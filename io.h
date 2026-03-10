/*
|	FILE: IO.H
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
|	Note: Be sure to include UNTIL.H first.
|
|	DATE		DESCRIPTION
|	01/20/95	Created this file from UNTIL.H. [nes] 
*/

#ifndef IO_H_
#define IO_H_

#ifndef UNTIL_H_
#include "until.h"
#endif

struct file_descriptor source[NUM_FILES];
long INPUT_SOURCE;      /* Where the compiler reads from */
long this_file;
long file_eof;
long binary_input_mode;


#endif

