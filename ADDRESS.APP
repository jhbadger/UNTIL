( Address book - S-Engine Demo                                       )
(                                                                    )
\ ====================================================================
\				SECTION 1
\ ====================================================================

variable fd.out

10   constant <nl>

4096 string out.buf

\ ====================================================================
\				SECTION 2
\ ====================================================================
80 string name
32 string bphone
32 string aphone
32 string hphone
32 string fax
80 string title
80 string category
80 string company
80 string addr1
80 string addr2
80 string city
80 string st
16 string zip
4096 string note
\ ====================================================================
\				SECTION 3
\ ====================================================================
: clear		( address --- )
   0 swap c!
   ;
: -newline		( 'string --- )
   dup strlen + 1- 		\ Calc address
   dup c@ <nl> =		\ Compare last char to newline
   if
     0 swap c!			\ yes, trash it
   else
     drop
   endif
   ;
: output.line		( ?? 'string --- )
   fd.out @ fprintf  \ drop 
\   fd.out @ fputline drop
   ;
: msg 		 ( 'string --- )
   dup strlen type
   ;
\ ====================================================================
\				SECTION 4
\ ====================================================================
: clear.fields		( --- )
   name clear      bphone clear 
   aphone clear    hphone clear 
   fax clear       title clear 
   category clear  company clear 
   addr1 clear     addr2 clear 
   city clear      st clear 
   zip clear       note clear 
   ;
: setup.HTML
   " <HTML>\n"   output.line
   " <HEAD><TITLE>SGML Address Book</TITLE></HEAD>\n"  output.line
   " </HEAD>\n"  output.line
   " <BODY>\n"   output.line
   ;
: process.name
   " <HR>\n" output.line
   name strlen 0>
   if    ( There is a name field )
      out.buf name strcpy
   else  ( use the company name  )
      out.buf company strcpy
      company clear
   endif 
   out.buf output.line
   ;
: process.phones
   " <DL>\n" output.line
   bphone strlen 0>
   if    bphone " <DT>Bus.: <DD>%s\n" output.line  endif
   hphone strlen 0>
   if    hphone " <DT>Home: <DD>%s\n" output.line  endif
   fax strlen 0>
   if    fax    " <DT>Fax : <DD>%s\n" output.line  endif
   aphone strlen 0>
   if    aphone " <DT>Alt : <DD>%s\n" output.line  endif
   " </DL>\n" output.line
   ;
: process.the.rest
   " <P>\n"   output.line
   title   strlen 0>   if title   " %s<BR>\n" output.line endif
   company strlen 0>   if company " %s<BR>\n" output.line endif
   addr1   strlen 0>   if addr1   " %s<BR>\n" output.line endif
   addr2   strlen 0>   if addr2   " %s<BR>\n" output.line endif
   city    strlen 0>
   if 
     city st zip " %s, %s  %s<BR>\n" output.line
   endif
   note    strlen 0>   if note    " <P>\n%s<BR>\n" output.line endif
   clear.fields
   ;
\ ====================================================================
\				SECTION 5
\ ==================================================================== 
: <ADDRESS>
   cr ." The SGML-based Address Book" cr cr
   " address.htm"  WRITE fopen
   fd.out !
   setup.HTML
   clear.fields
   ;
: </ADDRESS> 
   " </BODY>\n"  output.line
   " </HTML>\n"  output.line
   fd.out @ fclose
   ;

: <ENTRY> ;
: </ENTRY>
   process.name
   process.phones
   process.the.rest
   ;

\ ====================================================================
\				SECTION 6
\ ====================================================================
: <NAME>	name read_into    ;
: </NAME>	name -newline   
name msg cr ;

: <BPHONE>	bphone read_into  ;
: </BPHONE>	bphone -newline ;

: <APHONE>	aphone read_into  ;
: </APHONE>	aphone -newline ;

: <HPHONE>	hphone read_into  ;
: </HPHONE>	hphone -newline ;

: <FAX> 	fax read_into     ;
: </FAX>	fax -newline    ;

: <TITLE>	title read_into   ;
: </TITLE>	title -newline  ;

: <CATEGORY>	category read_into  ;
: </CATEGORY>	category -newline ;

: <COMPANY>	company read_into   ;
: </COMPANY>	company -newline  ;

: <ADDR1>	addr1 read_into     ;
: </ADDR1>	addr1 -newline    ;

: <ADDR2>	addr2 read_into     ;
: </ADDR2>	addr2 -newline    ;

: <CITY>	city read_into      ;
: </CITY>	city -newline     ;

: <ST>		st read_into        ;
: </ST>		st -newline       ;

: <ZIP>		zip read_into       ;
: </ZIP>	zip -newline      ;

: <NOTE>	note read_into      ;
: </NOTE>	note -newline     ;
\ ====================================================================
\				SECTION 7
\ ====================================================================
: &amp;		" &" ;
: &lt;		" <" ;
: &gt;		" >" ;

" address.sgm" sengine_init
sengine
