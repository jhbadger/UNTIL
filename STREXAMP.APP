( C-type string example code... )

variable dest   100 allot
variable source 100 allot

false string_returns
true null_strings

: .dest
   ." dest  : " dest dup strlen type cr
   ;
: .source
   ." source: " source dup strlen type cr
   ;

: init.strings
   dest   " this is the dest string..." strcpy 
   source " The source string" strcpy 
   .dest   .source cr
   ;

: s.test1
   ." Calling strlen..." cr
   dest strlen .s cr
   .dest cr
   ;
: s.test2
   ." Calling strcat..." cr
   dest source strcat .s cr
   .dest   .source cr
   ;
: s.test3
   ." Calling strcpy..." cr
   dest source strcpy .s cr
   .dest   .source cr
   ;
: s.test4  
   ." Calling strncpy..." cr
   dest source 5 strncpy .s cr
   .dest   .source cr
   ;
: s.test5
   ." Calling strncat..." cr
   dest source 5 strncat .s cr
   .dest   .source cr
   ;
: s.test6
   ." Calling strcmp..." cr
   dest source strcmp .s drop
   .dest   .source cr

   ." Calling strncmp..." cr
   dest source 5 strncmp .s drop
   .dest   .source cr 

   ;

: test.argv
   argv       20 dump cr cr
   argv @     20 dump cr cr 	( argv[1] )
   argv 4 + @ 20 dump cr cr	( argv[2] )
   ;

