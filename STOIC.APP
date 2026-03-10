\ file: string.txt
.( string.txt ) cr
: string.txt ;

\ misc utility words not in f83                         2 /6 /89
: 1+!                          ( addr --- )
    1 swap +! ;
: msg                       ( 'string --- )
    count type ;
: b<-                        ( addr b --- )
    swap c! ;
: c<-                        ( addr b --- )
    swap c! ;
: <-                         ( addr n --- )
    swap ! ;
: b!                         ( b addr --- )
    c! ;
: b@                         ( addr --- b )
    c@ ;

: 0<-                        ( addr --- )
   0 swap ! ;

: minus                      ( n --- -n )
   0 swap - ;

: $length                    ( addr --- len )
    dup dup
    begin c@ while 1+ dup repeat
    swap - 
    ;
: shift-left                 ( addr len --- )
    2dup over 1+ -rot        \ start at addr+1
    cmove                    \ move the bytes
    + 32 swap c!             \ tack blank on end
    ;
: string+                    ( to-addr fr-addr --- )
    dup $length                 \ get length of string
    rot dup $length + swap      \ to string length
    cmove                       \ append it
    ;

: get-str-chars              ( a1 a2 --- a1 a2 c1 c2 )
    over c@                  \ get c1
    over c@ ;                \ get c2

: incr-str-addrs             ( a1 a2 --- a1+1 a2+2 )
    1+ swap 1+ swap ;

: comp-len                   ( a1 a2 --- len )
    $length swap $length       ( get lengths )
    2dup >                   \ which is shorter?
    if ( s1 is shorter )
      swap drop              \ trash len of s1
    else
      drop                   \ trash len of s2
    then ;

				\ works on null terminated strings only
: string=               ( a1 a2 --- flag )
    2dup comp-len 0             \ # chars to test
    swap 0
    do
      drop get-str-chars        \ get next chars to test
      - dup                     \ test for =
      if ( c1 = c2 ) exit ( leave )      \ found =, get out
      else >r incr-str-addrs r> \ set up for next test
      then
    loop swap drop swap drop ;
 \    0 = s1=s2
 \    - = s1<s2
 \    + = s1>s2

: null->counted              ( addr --- )
    dup 1+ $length            \ compute length to null byte
    swap c! ;                \ save string length

: stab                  ( c 'string --- )
    swap over count +           \ calc end of string
    c!                          \ append it to the end
    1+! ;                       \ incr length byte by 1

: move_string                ( 'from 'to --- )
    over count 1+ swap drop  \ set up move
    cmove ;                  \ move-it

: .move_string               ( addr len 'string --- )
    dup >r 1+                \ save destination address
    over >r                  \ save length
    swap cmove               \ move string
    r> r>                    \ get addr & length
    c! ;                     \ update byte count

: strap                      ( 'from 'to --- )
    2dup count +             \ calc append addr
    swap count               \ 'fr & len
    swap -rot                \ adjust stack
    cmove                    \ move the bytes
    swap count swap drop     \ get # bytes added to string
    over c@ +                \ get length & add
    swap c! ;                \ update string length

: anykey                     ( --- )
    ." Press any key to continue"
    key drop
    ;

: .strap                      ( addr len 'string --- )
    over >r dup >r           \ save count and 'string
    count +                  \ calc append addr
    swap                     \ adjust stack
    cmove                    \ move the bytes
    r> r>                    \ get # bytes added to string
    over c@ +                \ calc new length
    swap c!                  \ update string length
    ;
