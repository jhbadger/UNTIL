/*
|	FILE: vectored.c
|
|	This module contains bindings for vectored words. Most are
|	I/O related.
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
| DATE		CHANGE
| 01/01/95	Got module integrated into rest of Until via the
|		symbol VECTORED_IO.
| 01/02/05	Added fuctions to get/set the vectors.
*/
#include "compiler.h"

#include <stdio.h>
#ifndef NO_STDLIB
#include <stdlib.h>
#endif

#include "until.h"
#include "functs.h"
/***********************************************************************
|***********************************************************************
|		The following group of functions implement vectored
|		I/O for S-Engine. They are the hooks for making
|		writing to disk as well as the screen with the same
|		words practical.
|
|		This approach slows execution slightly because every
|		execution of one of the vectored words causes two
|		function calls to be executed. The tradeoff is
|		extreme flexibility to change word behavior at
|		run-time...
*/
#ifdef VECTORED_IO
/***********************+---------------+
|			| exec_verbatim |
|			+---------------+
*/
void exec_verbatim()
{	(*verbatim_vect)();	}
/***********************+--------------+
|			| exec_display |
|			+--------------+
*/
void exec_display()
{	(*display_vect)();	}
/***********************+-------------+
|			| exec_center |
|			+-------------+
*/
void exec_center()
{	(*center_vect)();	}
/***********************+----------+
|			| exec_dot |
|			+----------+
*/
void exec_dot()
{	(*dot_vect)();	}
/***********************+----------------+
|			| exec_dot_quote |
|			+----------------+
*/
void exec_dot_quote()
{	(*dot_quote_vect)();	}
/***********************+------------+
|			| exec_dot_s |
|			+------------+
*/
void exec_dot_s()
{	(*dots_vect)();	}
/***********************+-------------+
|			| exec_g_dots |
|			+-------------+
*/
void exec_g_dots()
{	(*g_dots_vect)();	}
/***********************+-----------+
|			| exec_type |
|			+-----------+
*/
void exec_type()
{	(*type_vect)();	}
/***********************+-------------+
|			| exec_prints |
|			+-------------+
*/
void exec_prints()
{	(*prints_vect)();	}
/***********************+-------------+
|			| exec_printf |
|			+-------------+
*/
void exec_printf()
{	(*printf_vect)();	}
/***********************+-----------+
|			| exec_emit |
|			+-----------+
*/
void exec_emit()
{	(*emit_vect)();	}
/***********************+------------+
|			| exec_space |
|			+------------+
*/
void exec_space()
{	(*space_vect)();	}
/***********************+-------------+
|			| exec_spaces |
|			+-------------+
*/
void exec_spaces()
{	(*spaces_vect)();	}

/***********************+---------------------+
|			| exec_query_terminal |
|			+---------------------+
*/
void exec_query_terminal()
{	(*query_terminal_vect)();	}
/***********************+----------+
|			| exec_key |
|			+----------+
*/
void exec_key()
{	(*key_vect)();	}
/***********************+---------+
|			| exec_cr |
|			+---------+
*/
void exec_cr()
{	(*cr_vect)();	}
/***********************************************************************
|***********************************************************************
| The following section of code enables setting of the vectors.
|***********************************************************************
|***********************************************************************/
/***********************+--------------+
|			| addr_cr_vect |
|			+--------------+
*/
void addr_cr_vect()
{ pushsp((long)cr_vect);  }

/***********************+--------------------+
|			| addr_verbatim_vect |
|			+--------------------+
*/
void addr_verbatim_vect()
{ pushsp((long)verbatim_vect);  }

/***********************+-------------------+
|			| addr_display_vect |
|			+-------------------+
*/
void addr_display_vect()
{ pushsp((long)display_vect);  }

/***********************+------------------+
|			| addr_center_vect |
|			+------------------+
*/
void addr_center_vect()
{ pushsp((long)center_vect);  }

/***********************+---------------------+
|			| addr_dot_quote_vect |
|			+---------------------+
*/
void addr_dot_quote_vect()
{ pushsp((long)dot_quote_vect);  }

/***********************+----------------+
|			| addr_type_vect |
|			+----------------+
*/
void addr_type_vect()
{ pushsp((long)type_vect);  }

/***********************+---------------+
|			| addr_dot_vect |
|			+---------------+
*/
void addr_dot_vect()
{ pushsp((long)dot_vect);  }
/***********************+-----------------+
|			| addr_dot_s_vect |
|			+-----------------+
*/
void addr_dot_s_vect()
{ pushsp((long)dots_vect);  }
/***********************+------------------+
|			| addr_g_dots_vect |
|			+------------------+
*/
void addr_g_dots_vect()
{ pushsp((long)g_dots_vect);  }
/***********************+------------------+
|			| addr_prints_vect |
|			+------------------+
*/
void addr_prints_vect()
{ pushsp((long)prints_vect);  }
/***********************+------------------+
|			| addr_printf_vect |
|			+------------------+
*/
void addr_printf_vect()
{ pushsp((long)printf_vect);  }

/***********************+----------------+
|			| addr_emit_vect |
|			+----------------+
*/
void addr_emit_vect()
{ pushsp((long)emit_vect);  }

/***********************+-----------------+
|			| addr_space_vect |
|			+-----------------+
*/
void addr_space_vect()
{ pushsp((long)space_vect);  }

/***********************+------------------+
|			| addr_spaces_vect |
|			+------------------+
*/
void addr_spaces_vect()
{ pushsp((long)spaces_vect);  }

/***********************+---------------+
|			| addr_key_vect |
|			+---------------+
*/
void addr_key_vect()
{ pushsp((long)key_vect);  }

/***********************+--------------------------+
|			| addr_query_terminal_vect |
|			+--------------------------+
*/
void addr_query_terminal_vect()
{ pushsp((long)query_terminal_vect);  }

#endif

