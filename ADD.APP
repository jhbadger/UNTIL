\	FILE: ADD.APP
\
\ This is a simple filter program that adds up a the first column of
\ numbers in a file. I use this in conjunction with a Unix-like wc
\ programs to count the number of lines in the source files for
\ Until. It assumes the 

fload filter.app

variable total	0 total !

: count.it
   inbuf 1+ atol	\ call the C atol() function passing it the input buffer
   total +!	\ add it to the total
   ;

: doit " w.lis" " xxx.xxx" ['] count.it filter ;

doit
." Total: " total ? cr cr
