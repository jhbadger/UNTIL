/*
|       FILE: SENGINE.C
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
| 	CHANGE LOG:
|	04/24/94	Made unfound tokens pass through to the output
|			buffer. [nes]
|	04/20/95	Merged into Until V2.4 code base. [nes]
*/

#include "compiler.h"

#ifdef SENGINE

#include <stdio.h>
#include <string.h>
#include <ctype.h>

#ifndef NO_STDLIB
#include <stdlib.h>
#endif

#include "until.h"
#include "functs.h"
#include "sengine.h"

/***********************+--------------+
|			| sengine_init |
|			+--------------+
|	" filename" sengine
*/
void sengine_init()
{
	sgml_file = (char*)popsp();

	sgml_in  = fopen(sgml_file,"r");
	if(!sgml_in){
		printf("%s\n",sgml_file);
		printf("Input SGML file not found!\n");
		return;
	}
}
/***********************+---------+
|			| sengine |
|			+---------+
| Note: you must call sengine_init before calling sengine!
*/
void sengine()
{
	this_field = kb_buff;
	FOREVER{
		next_token();
		exec_cmd(token_buf);
		if(QUIT){		/* set by interpreter */
			break;
		}
	}
	QUIT       = TRUE;		/* reset QUIT to stay in interp..*/
	ABORT_FLAG = FALSE;
	fclose(sgml_in);
}
/***********************+-----------+
|			| nest_open |
|			+-----------+
*/
void nest_open()
{       open_cnt++;     }
/***********************+----------------+
|			| push_sgml_file |
|			+----------------+
*/
void push_sgml_file()
{	pushsp(NUMB sgml_file);  }
/***********************+------------+
|			| nest_close |
|			+------------+
*/
void nest_close()
{
	open_cnt--;
	if(open_cnt < 0){
		printf("Error-Too many closing }\n");
	}
}
/***********************+------------+
|			| next_token |
|			+------------+
| This function extracts the next token from the input stream.
*/
void next_token()
{
	int  status;
	int  end_tok = FALSE;
	char ch;

		/*
		| Reset token_buf and token
		*/
	token_buf[0]  = '\0';
	token         = token_buf;
	attrib        = attrib_buf;
	attrib_buf[0] = '\0';
	entity        = entity_buf;
	entity_buf[0] = '\0';

	ch            = skip_leading_ws();  /* Skip leading ws for token */
	if(status    == TOK_EOF){          /* EOF is error               */
		/* error */
	}
		/*
		| Loop through the rest of the buffer, extracting chars
		| until whitespace is detected.
		*/
	FOREVER{
		end_tok = parse_char(ch);
		if(end_tok){  break;  }
		ch      = next_char();	/* Read next char	 */
	}
	/*
	| Token returned in tok_buf[]
	*/
}
/***********************+-----------+
|			| next_char |
|			+-----------+
*/
char next_char()
{
	char ch;


	ch = tib[IN++];
	if(IN > tib_len){
		X_read_next_line();
		ch = tib[IN++];
	}
	if(ch == TOK_EOF){  /* EOF is error                 */
/*		>>error<<  */
	}
	return(ch);
}
/***********************+-----------------+
|			| skip_leading_ws |
|			+-----------------+
| This function is tied to the Terminal Input Buffer (TIB) and
| the input pointer (IN). The characters that are considered
| whitespace are ' ', \t, \n. The first non-whitespace character
| is returned.
*/
char skip_leading_ws()
{
	char ch;
		/*
		| This loop skips all characters in the input stream
		| until a whitespace character is encoutered
		*/
	FOREVER{
		ch = next_char();
		whitespace = isspace(ch);
		if(!ch)        {  whitespace = TRUE;} /* must stop nulls */
		if(!whitespace){  break;  }
	}
	return(ch);
}
/***********************+------------+
|			| parse_char |
|			+------------+
|	This function reads a char from the input stream and
|	processed. Processing of the char is based on the
|	cuttent STATE. The states defined for this function are
|
|	o TEXT		Collecting text in *this_field
|	o TAG		Within a tag. Char stored in *token
|	o ATTRIB	Within an attribute. Char stored in *attrib
|	o ENTITY	Within an Entity. Char stored in *entity
|	o default	Error - should never happen
|
|	parse_char() is called once per character.
|
| Return values are:
|	FALSE = Not end of token
|	TRUE  = End of Token
|
| Blank entries in the following table are errors!
|
|         |STAGO|Whitespace|TAGC|VI  |ERO   |ERC |Other
|         +-----+----------+----+----+------+----+-----
| TEXT    |TAG  |TEXT      |    |TEXT|ENTITY|    |
| TAG     |     |ATTRIB    |TEXT|    |      |    |
| ATTRIB  |     |          |    |TAB |      |    |
| ENTITY  |     |TEXT      |    |    |      |TEXT|
| default |     |          |    |    |      |    |
|
|
| Note: state is a global variable
*/
int parse_char(char ch)
{
	char *string;
	int  len;
	int  i;
	int  found;
	int  eo_tok = FALSE;

	switch(sgml_state){
	case SE_TEXT:
		switch(ch){
		case STAGO:	/*  '<'  */
			sgml_state    = TAG;
			token    = token_buf;
			*token++ = ch;
			store_char('\n');
			store_char('\0');
			nest_open();
			break;
		case ERO:	/*  '&'  */
			sgml_state    = ENTITY;
			break;
		case '\n':	/* newline */
			new_line_flag = TRUE;
		default:
			store_char(ch);
		}
		break;
	case TAG:
			/*
			| NOTE: An alternative approach is to have this
			|       in a function or a loop
			*/
		if(ch == TAGC){		/*   '>' */
			sgml_state = SE_TEXT;
			eo_tok   = TRUE;
			*token++ = ch;	/* need > on end  */
			*token++ = '\0';
			close_tag();
			break;
		}

		whitespace = isspace(ch);
		if(whitespace){
			sgml_state = ATTRIB;
			break;
		}
		*token++ = ch;
		break;
	case ATTRIB:
		if(ch == TAGC){		/*   '>' */
			sgml_state    = SE_TEXT;
			*token++ = ch;		/* append closing > */
			*token++ = '\0';	/* close string     */
			close_attrib();
			close_tag();
			break;
		}
		if(ch == VI){		/*  '='  */
			collect_attrib_value();
			*token  = '\0';	/* close string     */
			close_attrib();
			sgml_state   = TAG;
			break;
		}
			/*
			| This should never happen...
			*/
		if(ch == '\"'){
			/*
			| do something
			*/
			break;
		}
			/*
			| Builds name portion of attribute stmt.
			| Look at doing complete loop inside
			| collect_attrib() instead of one char at
			| a time.
			*/
		collect_attrib(ch);
		break;
			/*
			| NOTE: The way Entity processing is set up,
			|	if the entity ref is found, it will be
			|	the lookup value will be copied into
			|	*this_field, otherwise the entity
			|	token from the input stream will be
			|	copied as is to *this_field.
			*/
	case ENTITY:
			/*
			| Read the next word from the input stream
			| via word. Push ';'then call word() to read
			| up to the ;. Note: This will NOT handle
			| entities that end in a blank, but they are
			| rarely used.
			*/
		pushsp(NUMB REFC);
		word();			/* Read up to ERC */
		drop();			/* We don't use return value */
			/*
			| Move word from input stream to the
			| Entity Buffer. Must make sure that the
			| leading & and trailing ; are both included
			| in the buffer because of looking up in the dict.
			*/
		strcat(pad,";");	/* make sure have ending ; */
		pad[0] = pad[0] + 1;

		counted_to_null(pad,&entity_buf[2]);
		entity_buf[0] = ERO;
		entity_buf[1] = ch;
			/*
			| Search dictionary for the Entity buffer
			*/
		strcpy(&pad[1],entity_buf);
		pad[0] = strlen(entity_buf);
		found  = prim_find();
		if(found){
			(WA->CFA)();
			popsp();
			string = (char*)R_addr;
/*			strcpy(entity_buf,(string+1));	/ * counted         */
			strcpy(entity_buf,string);	/* null terminated */
		}
			/*
			| copy entity value to this_field
			*/
		len = strlen(entity_buf);
		for(i=0;i<len;i++){
			store_char(entity_buf[i]);
		}
		sgml_state = SE_TEXT;
		entity_buf[0] = '\0';
		break;
	default:
		printf("Invalid state\n");
	}
	return(eo_tok);
}
/***********************+----------------+
|			| process_entity |
|			+----------------+
| This function processes an entity substitution. The replacement
| value is copied into *this_field.
*/

/***********************+------------+
|			| store_char |
|			+------------+
|	Stores the character in the next location in the current
|	input buffer.
*/
void store_char(char ch)
{
	*this_field++ = ch;
}
/***********************+-----------+
|			| close_tag |
|			+-----------+
|	The pointer to this_field must be reset to the start of
|	the buffer so the next set of characters will start at
|	the beginning of the buffer, not just tacked onto the
|	end of whatever is already there.
*/
void close_tag()
{
/*
	append 0 to *token
	set eo_tok to TRUE
	nest_close();
*/
	*token++ = '\0';
	nest_close();
}
/***********************+--------------+
|			| close_attrib |
|			+--------------+
| Move the string collected in attrib_buf to the variable that
| is the attribute.
|
|	NOTE: ptrs must be reset to collect the next value...
|
| NOTE:	I found out that SGML requires each attribute name be
|	unique, so prepejnding the element name is not absolutely
|	necessary. It is convention now in this system and shall
|	remain for the time being... [nes]
*/
void close_attrib()
{
	int  status;
	int  len;
	char att_name[80];
		/*
		| Construct field name
		*/
	*attrib++     = '\0';
	strcpy(att_name,&token_buf[1]);
	strcat(att_name,".");
	strcat(att_name,attrib_buf);
	strcpy(&pad[1],att_name);
	pad[0]        = strlen(att_name);
		/*
		| Lookup field name in dictionary
		*/
	status    = prim_find();
	if(status){			
/*		attrib_string = WA->PFA.c_ptr;  */
		attrib_string = WA->PFA.c_ptr + 8;
			/*
			| Move the work field to the real location.
			| >> need to make sure CFA is do_allot... <<
			*/
		strcpy(attrib_string,attrib_value);
	}else{
		/*
		| error, word not found
		*/
		printf("Attribute name not found: %s\n",att_name);
	}
		/*
		| reset ptrs here
		*/
	attrib        = attrib_buf;
	attrib_buf[0] = '\0';
}
/***********************+---------------+
|			|collect_attrib |
|			+---------------+
*/
void collect_attrib(char ch)
{
	*attrib++ = ch;
}
/***********************+----------------------+
|			| collect_attrib_value |
|			+----------------------+
|	At this point, we know that we are at the starting point of an
|	attribute value. This function must collect the value, then
|	store it in the appropriate value. word() returns the token
|	in pad[] in counted form.
*/
void collect_attrib_value()
{
		/*
		| NOTE: The next three lines effectively read up to
		|       the closing " into pad[].
		*/
	pushsp((long)DQUOTE);
	word();
	drop();
		/*
		| Copy the attribute value from pad[] to attrip_value
		| while converting from counted to null terminated
		| form.
		*/
	counted_to_null(pad,attrib_value);
}
/***********************+----------+
|			| exec_cmd |
|			+----------+
|
|
*/
void exec_cmd(char* cmd)
{
	int    len;
	int    i;
	int    found;
	static int started;

	if(!started){
		started = TRUE;
	}
	QUIT = 0;

	strcpy(&pad[1],cmd);
	pad[0] = strlen(cmd);
	pushsp(NUMB pad);
	minus_find();           /* Look it up in the dictionary */
	found  = popsp();
	if(found){              /* found the word */
		WA = (struct DictHeader*) popsp();
		WA = (struct DictHeader*) R_addr;
		(*WA->CFA)();
	}else{
			/*
			|  Not a valid tag, so append it to
			|  the input buffer.
			*/
		this_field--;	/* Back up past \n\0 appended by token proc.*/
		this_field--;
		len = strlen(cmd);
		for(i=0;i<len;i++){
			store_char(cmd[i]);
		}
		drop();		/* Get rid of WA */
	}
	ABORT_FLAG = 0;
}
/***********************+----------------------+
|			| dummy read_next_line |
|			+----------------------+
*/
void X_read_next_line()
{
	char *status;

	tib_len = 0;
	while(!tib_len){
		IN      = 0;
		tib[0]  = '\0';
		status  = fgets(tib,TIBSIZE,sgml_in);
		if(status){
			tabs_to_blanks(tib);    /* basically ignore tabs */
			tib_len = strlen(tib);
			if(tib_len){
			/*
			| If I leave the \n, does WORD have to be changed too?
			| ANS: No, WORD is not call this, Next_char()
			| calls it.
			*/
				new_line_flag    = FALSE;
				return;
			}
			/*
			| Looks like here is the place to stick the <cr><cr>
			| as a para code....
			*/
			new_line_flag = FALSE;
		}else{
/* >>>>>> must change this so that we end up back in Forth on EOF..... */
			printf("EOF\n");
			QUIT = TRUE;
			exit(0);
		}

	}
}
/***********************+--------------------+
|			| push_new_line_flag |
|			+--------------------+
*/
void push_new_line_flag()
{
	pushsp(NUMB &new_line_flag);
}
/***********************+---------------+
|			| push_fmt_flag |
|			+---------------+
*/
void push_fmt_flag()
{
	pushsp(NUMB &fmt_flag);
}
/***********************+-------------+
|			| append_this |
|			+-------------+
| This function is connected to the macro "append_this".
*/
void append_this()
{
	int  i;
	int  len;
	char *string;

	string = (char*) popsp();
			/*
			| copy entity value to this_field
			*/
	len = strlen(string);
	for(i=0;i<len;i++){
		store_char(entity_buf[i]);
	}
}
/***********************+----------+
|			| set_this |
|			+----------+
| This function is connected to the macro "read_into".
*/
void set_this()
{
	this_field = (char*) popsp();
	start_this = this_field;
}
/***********************+------------+
|			| clear_this |
|			+------------+
*/
void clear_this()
{
	this_field = kb_buff;
	start_this = this_field;
}
/***********************+-----------+
|			| push_this |
|			+-----------+
*/
void push_this()
{
	pushsp((long) &this_field);
}
/***********************+---------+
|			| between |
|			+---------+
*/
#ifdef IGNORE
int between(int lo, int hi, int value)
{
	int truth;

	truth = ((value >= lo) && (value <= hi));
	return(truth);
}
#endif
/***********************+--------------------+
|			| sengine_make_prims |
|			+--------------------+
| Need to add the functs listed below... and probably move
| this funct to another file...
*/
void sengine_make_prims()
{
	build_primitive("sengine",sengine);
	build_primitive("sengine_init",sengine_init);
	build_primitive("read_into",set_this);
	build_primitive("this_field",push_this);
	build_primitive("clear_this",clear_this);
	build_primitive("append_this",append_this);
	build_primitive("nest_open",nest_open);
	build_primitive("nest_close",nest_close);
/*
	build_primitive("new_line_flag",push_new_line_flag);
	build_primitive("fmt_flag",push_fmt_flag);
	build_primitive("verbatim",exec_verbatim);
	build_primitive("display",exec_display);
	build_primitive("center",exec_center);
*/
}

#endif
