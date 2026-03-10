( Address book - Short                                      11/04/93 )
(                                                                    )
variable fd.out

10   constant <nl>

4096 string out.buf

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
: output.line		( 'string --- )
   fd.out @ fputline drop drop
   ;
: output.phone#		( 'phone --- )
   dup strlen 0>
   if
     out.buf swap strcat
     out.buf output.line
   else
     drop
   endif
   ;
: clear.fields		( --- )
   name clear      bphone clear 
   aphone clear    hphone clear 
   fax clear       title clear 
   category clear  company clear 
   addr1 clear     addr2 clear 
   city clear      st clear 
   zip clear       note clear 
   ;
: .so
   " .so hplj."  output.line
   " .so macros.rof"  output.line
   ;
: .rl
   " .rl \"      L--l---l-----------------------------R\"" output.line
   ;
: setup.rof
   " .nf"  output.line
   " .pl 46"  output.line
   " .he /SGML Phone Book///"  output.line
   " .fo //-#-//"  output.line
   ;

: <ADDRESS>
   cr ." The SGML-based Address Book" cr cr
   " address.rof"  WRITE fopen
   fd.out !
   .so .rl
   setup.rof
   clear.fields
   ;
: </ADDRESS> 
   fd.out @ fclose
   ;

: <ENTRY> ;
: </ENTRY> 
   name strlen 0>
   if    ( There is a name field )
      out.buf name strcpy
   else  ( use the company name  )
      out.buf company strcpy
      company clear
   endif 
   bphone strlen 0=
   if ( Blank bphone field )		\ Use hphone if no bussiness phone
      bphone hphone strcpy 
      hphone clear
   endif
   bphone output.phone#
   out.buf "                H " strcpy
   hphone output.phone#
   out.buf "                F " strcpy
   fax output.phone#
   out.buf "                A " strcpy
   aphone output.phone#
   clear.fields
   ;

: <NAME>	name read_into    ;
: </NAME>	name -newline   ;

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

: &amp;		" &" ;

" address.sgm" sengine_init
sengine

