(  UNTIL.APP                                        06/28/95                   )
(                                                                              )
(  This is the official UNTIL.APP file. It gets loaded automatically at        )
(  startup in the form of UNTIL.BIN. It is part of the distribution for        )
(  Until.                                                                      )
(                                                                              )

: hello
   cr ."  Welcome to UNTIL V2.5" cr cr
   ;

-1 constant true
 0 constant false

: argv[]		( n --- 'argv[n] )
   4 * argv + @
   ;
: nip			( n1 n2 --- n2 )
   swap drop ;
: tuck			( n1 n2 --- n2 n1 n2 )
   swap over
   ;
: msg			( 'string --- )
   dup strlen type ;
: d.            ( d --- )
   drop .
   ;
: d+			( d1 d2 --- dsum ) \ per Wil Baden
   rot + >r   over +    dup rot
   u< if		\ Test for carry
     r> 1+
   else
     r>
   endif
   ;
: d*			( d1 d2 --- dproduct ) \ per Wil Baden
   >r 
   tuck * >r
   um*
   r> r> swap r> * + +
   ;
: >here                 ( 'string --- )  ( copy string to here )
   here 20 0 fill
   count here swap cmove
   ;

: )   cr ."   Unpaired )" cr ;
: }   cr ."   Unpaired }" cr ;

true show_copy
true null_strings
false string_returns

hello

