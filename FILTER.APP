(  FILE: filter.app                                                           )
(                                                                             )
(  This is a skeketon file filter process                                     )

variable fin
variable fout
variable inbuf  2050 allot
variable outbuf 2050 allot
variable process 

inbuf 1+ constant data

: prints
   fout @ fputs drop  
   ;
: \n
   fout @ fcr drop 
   ;
: read.line		( --- EOF|n )
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
     read.line
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
\
\ : doit " report.tag" " xxx.xxx" ['] process.it filter ;
\ doit

