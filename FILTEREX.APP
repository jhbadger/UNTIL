(  FILE: filterex.app                                                         )
(                                                                             )
(  This is sample file filter program that you can use to test the search     )
(  and replace functions.                                                     )

fload filter.app

: process.line		( --- )
\   0 inbuf count + 1- c!		\ trash the newline
   inbuf 1+ "  is " "  is not " gsub	\ blank is blank
   if ( the search was successful )
      inbuf 1+ dup strlen type cr       \ Type lines that match
   endif
   inbuf 1+ fout @ fputs drop
   ;

: doit " 00read.me" " xxx.xxx" ['] process.line filter  ;
