/*
|	FILE: SENGINE.H
*/

#define FORWARD		1
#define BACKWARD	0

#define SE_TEXT    1
#define TAG     2
#define ATTRIB	3
#define TOK_EOF	4
#define ENTITY  5

#define TAGC    '>'
#define STAGO   '<'
#define VI      '='
#define REFC	';'
#define ERO	'&'
#define NET	'/'
#define PIC	'>'

#define DQUOTE  '"'
#define EQUAL   '='
#define TAB     '\t'
#define NEWLINE '\n'

#define MAX_ATTRIB	32
#define MAX_VALUE	80
#define MAX_TOKEN	64


/**********************************************************
|		S-ENGINE STUFF
*/

int  open_cnt   = 0;
int  whitespace;
int  sgml_state = SE_TEXT;

char *this_field;			/* Curr input buffer		*/
char *start_this;
char *token;				/* Goes with tokeb_buf[]	*/
char *entity;				/* Goes with entity_buf[]	*/
char *attrib;				/* Goes with attrib_buf[]	*/
char *attrib_string;			/* Goes with attrib_value[]	*/
char kb_buff[512];			/* Default input buffer		*/
char token_buf[MAX_TOKEN];
char entity_buf[MAX_TOKEN];
char attrib_buf[MAX_ATTRIB];
char attrib_value[MAX_VALUE];
char next_ch;

char *sgml_file;
FILE *sgml_in;
long new_line_flag;
long fmt_flag;
