(  FILE: filter.app                                                           )
(                                                                             )
(  This is a skeketon file filter process                                     )

32 constant blank

variable fin
variable fout
variable inbuf  2050 allot
variable outbuf 2050 allot
variable process 

: prints
   fout @ fputs drop  
   ;
: \n
   fout @ fcr drop 
   ;
: read.next.line 	( --- EOF|#bytes )
   inbuf 2048 fin @ fgets
   ;
: init.filter  ( 'file-in 'file-out 'process --- )
   process !   
   WRITE fopen  dup fout !
   0= if ( error)
      ." Could not open output file" cr
      bye
   then
   READ fopen  dup  fin !
   0= if ( error)
      abort" Could not open input file" cr
   then
   ;
: exit.filter		( --- )
   fin @ fclose
   fout @ fclose
   ;

: filter		( 'file-in 'file-out 'process --- )
   init.filter
   begin
     read.next.line
     if ( Not EOF )
       process @ execute
       false
     else
       true
     then
   until
   exit.filter
   ;


( Sample call    )
\ : process.it  inbuf count type ;
\ : doit " report.tag" " xxx.xxx" ['] process.it filter ;
\

\ GENERATE NEW HELP FILE FROM THE GLOSSARY RAW FILE

inbuf 1+ constant line
128 string trash

: look.for.<HR>
   begin
     line " <HR>" search
     if ( got a match )
\       inbuf count type
       true
     else
       read.next.line
       if ( not EOF )
         false
       else
         true
         bye
       endif
     endif
   until
   ;
	\ Extract word and put it in trash
: extract.word		( 'start 'end --- )
   over - 		\ calc length
   dup >r
   0 do			\ move word to trash
       dup c@		\ get next byte
       trash i + c!	\ save it
       1+
   loop 
   0 r> trash + c!	\ NULL terminate it
   drop
\ trash dup strlen type cr
   ;
: pad.word		( --- )
   trash dup strlen +		\ calc starting address
   14 trash strlen -	 	\ calc # spaces
   0 do
       blank over c!		\ append blank
       1+			\ increment address
   loop
   0 swap c!			\ null terminate it
   ;
: process.word		( --- )
		\ extract word name
   read.next.line
   0= if ( not EOF )
     true
     bye
   endif
   line " <B>" search dup
   if ( found bold )
     3 + dup		\ calc address to start of word
     " </B>" search dup	\ Now search for end of word
     if ( got all addresses )
       extract.word	\ extract word from input line
       pad.word		\ pad with trailing blanks for alignment
     else		\ no match, clean up stack
       drop
     endif
   else			\ no match, clean up stack
     drop
   endif
   ;
: extract.stack		( --- )
   read.next.line
   0= if ( not EOF )
     true
     bye
   endif
   0 inbuf count 5 - + c!	\ truncate <br> off line
   trash line strcat
   ;
: read.line.and.do.checks	( --- tf)
     false
     read.next.line
     0= if ( not EOF )
       drop true
       bye
     endif
		\ Test to get out completely
     line " </BODY>" strcmp
     0= if ( found it, time to quit )
       drop true
       bye
     endif
     inbuf c@ 2 <
     if ( empty line )
       drop true
     endif
     line " <PRE>" search
     if ( found it, time to quit )
       drop true
     endif
   ;
: process.description
   begin
     read.line.and.do.checks
		\ test for blank line
     dup 0= if
       trash "               " strcpy
       trash line strcat
       trash fout @ fputs drop	\ write the line
     else drop
     endif
   until
   ;
: process.it
   look.for.<HR>	\ read until an <HR> is found
   process.word		\ The word ends up in trash
   extract.stack	\ pull out stack comment
   trash fout @ fputs drop	\ write the line
   fout @ fcr drop
trash dup strlen type cr
   process.description	\ read and write description lines
   ;

: do.help " g2.raw" " help.xxx" ['] process.it filter ;
