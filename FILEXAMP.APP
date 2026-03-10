(  file.app )

variable fin
variable inbuf 100 allot
variable fout
variable outbuf 100 allot

: list.file             ( --- )
   " filexamp.app" READ fopen    fin !
   begin
     inbuf 80 fin @ fgets
     if
       inbuf count type
       false
     else
       true
     then
   until
   fin @ fclose
   ;

: copy.file
   " filexamp.app" READ  fopen   fin  !
   " xxx.xxx"      WRITE fopen   fout !
   begin
       fin @ fgetc
       dup
       true = 0=
       if ( not eof )
         fout @ fputc drop
         false
       else
         true
       then
   until
   fin @ fclose
   fout @ fclose
   ;

