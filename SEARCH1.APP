(  FILE: filter.app                                                           )
(                                                                             )
(  This is a skeketon file filter process                                     )

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
     inbuf 2048 fin @ fgets
     if ( Not EOF )
\       inbuf 1+ prints
       process @ execute
       false
     else
       true
     then
   until
   exit.filter
   ;


: repl.DD		( --- )
   0 inbuf count + 1- c!		\ trash the newline
   inbuf 1+ "  C " "  * " sub
   if
\     inbuf 1+ prints \n
      inbuf count type cr   
   endif
   ;

: doit " ref.txt" " xxx.xxx" ['] repl.DD filter
  ;

\ doit
