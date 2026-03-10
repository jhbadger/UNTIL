\ file: pcolon.txt
\
\ Norman E. Smith, CDP
\ (c) Copyright 1989 (c)
\ All Rights Reserved

.( pcolon.app ) cr

: pcolon.app ;

true null_strings                  \ This program uses null strings

variable scr-no    0 scr-no !      \ current screen number
variable page-no   0 page-no !     \ page counter
variable line-no   0 line-no !     \ line counter
variable fin
variable inbuf 100 allot
variable fname 100 allot

   58 constant line/page
   12 constant form-feed

: .heading                      ( --- )
      1 page-no +!              \ increment page count
      0 line-no !               \ reset line number
      form-feed emit  cr        \ go to new page
      ." listing of colon definitions in "
      fname dup strlen type
      ."                       page " page-no @ . cr cr ;

: .:line                        ( addr of line --- )
      line-no @ line/page >     \ line ct > 60 ?
      if ( page overflow )
	.heading                \ print page heading
      then
      1 line-no +!              \ increment line count
      scr-no ?                  \ get scr # and print it
      1- count type             \ type the line
    ;

: colon-in-screen               ( addr --- )
     dup c@ ascii : =           \ get char and compare to ":"
     if ( : )                   \ if =
       .:line                   \ print the line with the ":"
     else
       drop                     \ remove the unused addr
     then  ;

: find-colon                    ( --- )
   begin
      inbuf 80 fin @ fgets 
      1 scr-no +!
      inbuf 1+ colon-in-screen
      0= 
   until
   cr ;

: index                         ( --- )
      0  scr-no !
      0  page-no !             \ initialize page number
      99 line-no !             \ initialize line number
      1 argv[] READ fopen   fin !
      find-colon               \ find the colon definitions
      fin @ fclose
   ;


