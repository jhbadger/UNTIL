/*
|	FILE:	DEBUG,C
|
|	This module provides several C-level debugging functions
|	for programmer convenience,
|
|	DATE		CHANGE
|	01/16/95	Made cleanup changes suggested by Akira Kida,
|			akida@isd.hin.konica.co.jp. [nes]
*/
#include "compiler.h"

#include <stdio.h>

#ifdef DEBUG_PROG

/***********************+-------+
|			| debug |
|			+-------+
*/
void debug(char *msg)
#ifndef VECTORED_C_IO
{	fprintf(stderr,"%s\n",msg);	}
#else
{	sprintf(tstring,"%s\n",msg);	
	(stderr_vect*)(tstring);
}
#endif
/***********************+--------+
|			| debugs |
|			+--------+
*/
void debugs(char *msg,char *value)
#ifndef VECTORED_C_IO
{	fprintf(stderr,"%s: %s\n",msg,value);	}
#else
{	sprintf(tstring,"%s: %s\n",msg,value);	
	(stderr_vect*)(tstring);
}
#endif
/***********************+--------+
|			| debugi |
|			+--------+
*/
void debugi(char *msg,int value)
#ifndef VECTORED_C_IO
{	fprintf(stderr,"%s: %d\n",msg,value);	}
#else
{	sprintf(tstring,"%s: %d\n",msg,value);
	(stderr_vect*)(tstring);
}
#endif
/***********************+--------+
|			| debuga |
|			+--------+
*/
void debuga(char *msg,void *value)
#ifndef VECTORED_C_IO
{	fprintf(stderr,"%s: %ld\n",msg,(unsigned long)value);	}
#else
{	sprintf(tstring,"%s: %ld\n",msg,(unsigned long)value);
	(stderr_vect*)(tstring);
}
#endif

#endif
