!       		( n addr --- )
       "store". Store the value, "n", at address specified by "addr".
              5 trash !
        moves 5 into the variable trash.
"       		( --- 'string )
        "double quote". This word compiles the string delimited by a trailing
        " into the dictionary as part of the current word definition. " is
        state smart so it will work in either interactive or compile mode.
              : aword   " This is a string" count type ;
#       		(ud1 --- ud2)
        "sharp". Convert the next digit in "ud1" to ASCII and add
        it to the numeric string being built. Used with "<#".
#>       		(d --- addr len)
        "sharp-greater-than". Ends a number formatting sequence. See 
        <#.
#s       		(ud --- 00)
        "numbers". Convert the unsigned double number to numeric
        digits.
#tib       		( --- 'tib )
        "number-tib". This word returns the address of the Terminal Input
        Buffer (tib).
'       		( --- 'word ) 
        "tick". Tick looks up the next blank delimited token in the input
        stream in the Dictionary and leaves its address on the stack. 
               ' aword
        leaves the word address of "aword" on the data stack. The operation of
        tick is slightly different from Forth-83 because it actually leaves the
        address of the word entry (WA) rather than the CFA. The effect is the
        same because Until's "execute" works that way.
        It is easy to go from the word address to the fields, but was not easy
        to go from one header field to another. 
(       		(--- ) 
        "left paren". Text between the parenthesis is treated as a comment. 
        Left parens starts a comment. A parens comment may span multiple lines.
               ( This is a comment )
        Use \ to comment to the end of the current line.
*       		(n1 n2 --- n1*n2 )
        "star". Multiply n1 times n2.
              2 2 *
        The result, 4, is placed on the top of the stack.
*/       		(n1 n2 n3 --- quot )
        "star-slash". Multiply n1 times n2, then divide by n3. (n1*n2/n3). The
        quotient is left on the stack.
              3 3 2 */
        leaves 4 on the stack.
*/mod       		( n1 n2 n3 --- quot rem )
        "star-slash-mod". Multiply n1 * n2, then divide by n3, leaving
        the quotient and remainder on the stack. (n1*n2/n3).
              4 4 2 */mod
        results in 8 0 on the stack. (4*4/2).
+       		( n1 n2 --- n1+n2 )
        "plus". Add n1 to n2. The sum is placed on the top
        of the stack.
              1 2 +
        The result, 3, is left on the top of the stack.
+!       		( n addr --- )
        "plus-store". Add "n" to the value at address "addr".
        Assume trash contains 27. 
              10 trash +!
        trash contains now contains 37.
+loop       		( n --- )
        "plus loop". The end of a do loop. The loop index is incremented by 'n'
        each time through the loop.
              : xxx 10 0 do i . 2 +loop ;
        This do loop will execute 5 times. +loop can be used only inside a word
        definition. See "do".
,       		( n --- )
        "comma". Not implemented. This word compiles a 32-bit number into
        the dictionary.
-       		( n1 n2 --- n3 )
        "minus". Subtract n2 from n1 and place the result 
        on the top of the stack. (n1-n2=n3).
              2 1 -
        The result, 1, is left on the stack.
-find       		( 'string --- addr flag )
        "minus-find". Search the dictionary for the word name in "string". The
        return values are the address of the word and a flag. The flag values
        are:
         0=not found
         1=normal
        -1=immediate
        The address is valid only when flag is non-zero. A typical usage of
        -find is:
              { aword} here -find 
        The return value could be tested before further processing.
-rot       		( n1 n2 n3 --- n3 n1 n2)
        "minus-rot". Move the value on the top of the parameter stack to the
        third item. 
              1 2 3  -rot
        becomes 3 1 2. 
-trailing       		( addr len --- addr' len')
        "minus-trailing". Remove trailing blanks from the counted string
        at "addr". The length, "len", is adjusted accordingly.
        If trash contains ``This is a test   ''. The stack would have
        68642 17. 
              trash count -trailing
        results in 68642 14. (Trust me, there are three trailing
        blanks!)
.       		( n --- )
        "dot". Display the number at the top of the parameter
        stack to the screen. This is a stack destructive operation.
        Note: Only base 8, 10, and 16 are supported by this operation.
."       		( --- )
        "dot quote". Type a string to the screen. Used in the form:
               ." This goes to the screen."
        This word can be used only inside of a word definition. Use ".("
        for interactive or stream messages.
.(       		( --- )
        "dot-parens". Type a constant string to the terminal screen. The
        string is terminated with a right parens. This word is used during
        interpretive mode only. 
              .( This is displayed.)
        The typical usage is writing messages to the screen when
        compiling from a file.
.bin       		( n --- )
        "dot-bin". Type the 32-bit binary representation of number on the
        top of the stack. 
.move_string       		High level (addr len 'destination  --- )
        "dot move string". This is a variation of move_string that uses an address
        and length to specify the source string. "destination" specifies the
        address of the counted string that receives the source. No provision is
        made for overlapping strings.
.quote       		( --- )
        This is an internal word that should not be used. ".quote" is
        the run-time code for "."".
.s       		dot_s ( --- )
        "dot s". Non-destructive Data Stack display to the screen. Until 
        also displays the stack contents each time through the main loop in the
        outer interpreter.
.strap       		.strap High level (addr len 'destination  --- )
        "dot strap". end the string at addr to destination for len bytes. 
        This word is a variation of "strap".
/       		( n1 n2 --- quot )
        "slash". Divide n1 by n2. (n1/n2).
              4 2 /
        Leaves 2 on the top of the stack. There is no remainder. 
              9 2 / 
        computes to 4. Until does not deal with floored division vs.
        normal division. It uses the standard C division operator.
        And that's that!
/mod       		( n1 n2 --- quotient remainder )
        "slash-mod". Divide n1 by n2 leaving the quotient and remainder on
        the parameter stack. (n1/n2).
              10 3 /mod
        results in (3 1) being left on the stack.
0       		( --- 0 )
        "zero". Push a constant of 0 onto the parameter stack. Several small
        integers are defined as words instead of being converted to numbers
        because the compile time is quicker. 
0<       		( n --- tf )
        "zero-less-than". Compare the number at the top of the parameter stack
        to zero to determine if it is less than zero. TRUE means n is less than
        zero. FALSE indicates n is greater than or equal zero.
               -1 0<   ( returns true )
               00 0<   ( returns false )
               99 0<   ( returns false )
        This primitive executes faster than executing 0 < as two separate
        words.
0=       		( n --- tf )
        "zero-equal". Test the number on the top of the parameter stack 
        for equal to zero. Return TRUE when n is zero, else return false.
               99 0=   ( returns FALSE )
               00 0=    ( returns TRUE )
0>	       	( n --- tf )
        "zero-greater-than". Compares "n" to zero and replaces n with
        TRUE if it greater than zero or FALSE when less than or equal zero.
               99 0>   ( returns TRUE )
               00 0>   ( returns FALSE )
               -1 0>   ( returns FALSE )
0bran	       	Internal Use Only
        This word is used internally by the compiler. It branches to the address
        in the next cell in the dictionary when the value on the top of
        the parameter stack is zero. "0bran" is typically used in conditional
        and looping constructs.
1       		( --- 1 )
        "one". Push a constant 1 onto the parameter stack. Several small
        integers (0, 1, 2, and 3) are defined as words instead of being
        converted to numbers because the compile time is quicker. 
1+       		( n --- n+1 )
        "one-plus". Add one to the number on the top of the parameter stack.
1+!       		( addr --- )
        "one-plus-store". Add one to the contents of the cell located at
        addr.
1-       		( n --- n-1)
        "one-minus". Subtract one from the number on the top of the 
        parameter stack.
2       		( --- 2 )
        "two". Push a constant two onto the parameter stack. Several small
        integers (0, 1, 2, and 3) are defined as words instead of being
        converted to numbers because the compile time is quicker.
2+       		( n --- n+2 )
        "two plus". Add two to the number on the top of the parameter stack.
2-       		( n --- n-2 )
        "two-minus". Subtract two from the number on the top of
        the stack.
2/       		( n --- n/2)
        "two-divide". Divide the number on the top of the stack.
2dup       		( n1 n2 --- n1 n2 n1 n2 )
        "two dup". Duplicate the top two numbers on the stack. This word
        is also used for duplicating a double length number.
              1 2 2dup 
        results in a stack of 1 2 1 2.
3       		( --- 3 )
        "three". Push a constant value three onto the parameter stack. Several
        small integers (0, 1, 2, and 3) are defined as words instead of being
        converted to numbers because the compile time is quicker.
:       		( --- )
        "colon". Start a new word definition. The word name is the next word in the
        input stream:
              : new macro  ;
        See ';'.
;       		( --- )
        "semi". Finishes a word definition and exits
        compile mode. Must be paired with :. See ':'.
<       		( n1 n2 --- tf )
        "less than". Compare the two numbers on the top of the stack
        and return TRUE or FALSE.
              5 6 <                 ( returns true  )
              5 5 <                 ( returns false )
              6 5 <                 ( returns false )
<#       		( d --- d )
        "less than sharp". Initialize a numeric output string. Digits
        are converted right to left within the output picture.
              1234 0 <# # # #> type
        outputs 34 to the user terminal.
<builds       		( --- )
        Not implemented.
<outer>       		Internal Use Only
        This is the word that is called during startup to begin execution
        of the outer interpreter. It is bound to a C function.
=       		( n1 n2 --- tf )
        "equals". Compare the top two values on the stack for equality.
              5 6 =                ( returns false )
              5 5 =                ( returns true  )
              6 5 =                ( returns false )
>       		( n1 n2 --- tf )
        "greater than". Compare the top two stack entries. (n1>n2).
              5 6 >                 ( returns false )
              5 5 >                 ( returns false )
              6 5 >                 ( returns true  )
>counted       		( 'null --- addr len)
        "to-counted". Convert the null terminated string at "'null" to
        counted form with the address and length left on the stack.
        This word is useful in converting
        a string returned from a C function to the form normally expected by
        Forth.
>here       		High level ( 'string --- )
        "to-here". Copy the string whose address is on the top of the stack
        to "here". Several system/file words expect a string at
"here". For example:
               " xxx.xxx" >here   here 20 dump
>in	       	( --- 'IN)
        "to-in". Return the address of the system variable that contains
        the offset to the current byte in TIB.
>null       		( 'null 'counted --- )
        "to-null". Convert the null terminated string whose address is "'null"
        to a counted string at "'counted". Forth normally uses
        counted strings and C uses null terminated strings. This word is
        typically used to get a string in the correct form for passing
        to a C function call.
>r       		( n --- )
        "to-r". Move the top number, "n", on the parameter stack to the
        return stack. This word cannot be used in Until to alter the processing
        path taken by the inner interpreter. It is possible to implement GOTO
        in some Forths using this technique.
?       		( addr --- )
        "query". Type the contents of the address that is on the
        top of the stack.
              trash ?
        types the value stored in the variable trash. This is
        logically equivalent to "@ .".
?dup       		( n --- n | n n )
        "query-dup". Duplicate the top number on the stack if it not equal
        to zero. "?dup" is useful in situations were a number will be used
        when it contains a value. It prevents having to explicitly have an "else"
        statement to clean up the stack. For example, the following word
        types the value of the number on the top of the stack using "dup":
               : aword  dup if ." The number is: " . cr
                            else drop
                            then ;
        Now, consider the same word using "?dup":
               : aword  ?dup if ." The number is: " . cr then ;
        The second form of "aword" is not only cleaner to read, it
        executes faster.
?terminal       		( --- tf)
        "query-terminal". Return TRUE when a character is waiting in the input
        buffer. The action of this word is OS dependent. It always returns TRUE in
        VMS and Coherent (including other Unix variants). 
        Borland C++ includes a function, "keyin()", that is equivalent
        to "?terminal". Most other C compilers do not. In addition, "getc()"
        normally does not return a character until <CR> is pressd. This causes
        slight differences in the operation of not only "?terminal", but
        "key" as well.
        The typical usage is polling the keyboard for a key press:
              : wait-for-key   begin ?terminal until key drop ;
@       		( addr --- n )
        "fetch". Fetch the value from address and place on the top
        of the stack.
              trash @
        retrieves the value stored at trash and places it on the top
        of the stack.
abort       		( --- )
        "abort". Abort the execution of the current word, do a warm start,
        then return to the outer interpreter outer-most loop. Until differs
        from most Forth implementations in that execution is not stopped
        immediately. The interpreter loop continues until the bottom at which
        time a warm start is done.
abort"       		( ? --- )
        "abort-quote". Abort the outer interpreter after typing the message
        ended by a ". This word does not terminate the outer interpreter
        immediately, but waits until the end of the current loop to perform
        the warm start.
abort_quote       		Internal Use Only
abs		       ( n --- |n| )
        "a-b-s". Return the absolute value of "n".
              99 abs          ( returns 99 )
              -1 abs          ( returns 1 )
align       		( --- ) 
        "align". "align" will be used with "c," when it is
        implemented.
allot       		( n --- )
        "allot". Allocate "n" bytes in the dictionary. This is another place
        where Until departs from normal Forth implementations. The memory
        allocation is made and the following actions performed:
           - The address returned from malloc() put in the PFA
           - The CFA is changed to "do_allot"
        The effect is that the allocation is applied to most recently defined
        word in the dictionary. This is indeed what is desired in most cases.
        Forth simply increments DP to allow for "n" bytes in the dictionary.
        It works well with a flat, contigious dictionary, but not in the
        linked list of structures used by Until. Defining a string or array is
        typical usage of allot:
              variable any_string 80 allot
              create any_string_too 80 allot
        This statement creates a string with 80 bytes allocated to it. This
        operation is not identical to the ANS Forth standard, but matches
        Forth-83.
and       		( n1 n2 --- n3 )
        "and". This is a bitwise and operation. (n1 & n2).
               1111 1111 - n1
               0000 1111 - n2
               0000 1111 - result n1&amp;n2
        Only bits that are on in both n1 and n2 are on after the operation.
APPEND       		( --- 'APPEND )
        "append". This word leaves the address of the "a" string used by fopen.
              { anyfile.dat} APPEND fopen
        opens "anyfile.dat" for output with <p>end access.
argc       		( --- argc )
        "a-r-g-c". argc leaves the number of arguments from the command line on the
        parameter stack. This makes passing command line arguments to Until
        words easy.
argv       		( --- argv )
        "a-r-g-v". argv leaves the pointer to the arguments on the command
        line. This makes passing command line arguments to Until words easy.
ascii       		( --- c )
        "ascii". Convert the character following in the input stream to its
        ASCII-equivalent integer number. 
              ascii A
        leaves decimal 65 on the stack. 
atol       		( 'string --- n )
        "a-to-l". Convert the null terminated string whose address is
        on the top of the parameter stack to a long integer. "atol" calls
        the C atol() function.
base       		( --- base )
        "base". Fetch the current base to the top of the stack. The current
        base can be typed by:
              base @ dup decimal . base !
begin       		( --- )
        "begin". "begin" marks the starting point for a
        "begin ... until "
        loop. The compiler uses begin to mark the branch point for
        the "until" statement. Typical usage:
              : aword   begin 1+ dup 10 > until ;
              0 aword
        "aword" expects a value on the stack. It loops until the counter on
        the stack is greater than ten. See "until".
binary       		( --- )
        "binary". Sets the current base to binary. 
BINARY       		( --- 'BINARY )
        "binary". Sets the file type for "fopen" to binary. This is necessary
        to process a file that contains non-Ascii data. Typical usage:
              " anyfile.dat" BINARY fopen
        See the File I/O word glossary for
        complete examples.
bold_off       		( --- )
        "bold off". Set screen video to normal. This specific to Borland C++
        at the present time.
bold_on       		( --- )
        "bold on". Set screen video to bold. This word is specific to Borland
        C++ at the present time.
branch       		Internal Use Only"
        "branch". "branch" is an unconditional branch compiled into the
        dictionary to cause program branching. The number compiled with branch
        is the relative offset within the parameter field to branch to
        by setting IP.
bye       		( --- )
        "bye". Exit Until and return to the calling function. Returns to DOS if
        stand alone program.
c!       		( c addr --- )
        "c-store". Store the character, "c", at address "addr". This is
        a byte oriented operation.
              ascii A here c!
        stores the integer value for 'A' (65 decimal) at the address of here.
c,       		( c --- )
        "c comma". Not implemented. Normal Forths compile the character into 
        the next available byte in the dictionary. There are several issues
        that must be resolved in my mind before I add "c," to Until. The
        problem is caused by the non-contigious dictionary. Does the character
        get compiled into the last definition? Should an allot be required 
        before c,? The problems are compatibility related not technical...
c@       		( addr --- c )
        "c-fetch". Fetch the character located at the address, "addr". The
        following statement fetches the character stored at here:
              here c@ .
        Assuming that here contains a `A', the number 65 will be typed.
center       		( 'string --- )
        "center".
clear_temp       		(  --- )
        "clear-temp". The search and replace C functions use a dynamically
        allocated temprorary work string. Calling clear_temp will free()
        the memory.
cls       		( --- )
        "c-l-s". Clear screen. Note: This word is functional only in the
        PC version of Until.
cmove       		( 'from 'to n --- )
        "c-move". Move "n" bytes starting at address "'from" to
        the destination address, "'to". The source and string should
        not overlap. Use "cmove>" for overlapping strings.
              source destination 20 cmove
        moves 20 bytes from "source" to "destination". 
cmove>       		( 'from 'to n --- )
        "c-move-up". Move "n" bytes starting at "'from" to "'to".
        This move operations starts at the end of the buffer and works toward
        the front. "cmove>" should be used when source and destination
        strings overlap. Use "cmove" for non-overlapping strings.
cold       		( --- )
        "cold". Perform a cold start of the system. The stacks are cleared
        and the primitives recreated.
comma_offset       		( --- 'comma_offset)
        "comma offset". Push the address of the C variable "comma_offset"
        onto the parameter stack.
compile       		( --- )
        "compile". Compile the following word into the current definition. It
        is typically used in immediate words to compile an address into
        the definition. Usage:
              : immed-word   compile cr ; immediate
        Since "immed-word" executes at compile time to include "cr"
        in the word definition. Generally, the definition is an immediate
        word that contains a non-immediate word that is to be compiled rather
        than executed. Use "compile" only inside a colon definition.
constant       		( --- n )
        "constant". Create a constant. The compile time stack comment is
        ( n --- ). Typical usage is:
              5 constant five
        When "five" is referenced, the integer 5 is pushed onto
        the parameter stack:
              five    ( --- 5 )
convert       		( d 'numeric_string --- d2 'end )
        "convert". Convert the numeric Ascii string to a double number. The
        end address is the address of the first non-numeric character in
        the numeric string. NOT IMPLEMENTED.
count       		( 'addr --- addr+1 len )
        "count". Convert a counted string address, "addr", to the 
        first byte address and length. 
              a_string count type
        "count" is used in conjunction with many words that require
        an address and a length to manipulate a counted string.
cr       		( --- )
        "c-r". Type a carriage return to the screen. This word is vectored.
create       		( --- )
        "create". Create an empty word definition. The word name assigned 
        is the following blank delimited token in the input stream. Usage: 
              create name     \ run-time ( --- 'name )
        The run-time execution of name leaves its address on the top of the
        stack. "create" can be used to build arrays and such.
d+       		( d1 d2 --- d1+d2 )
        "d-plus". Double number addition. The high order cell contains a
        dummy zero for a place holder for compatibility with other
        Forth implementations. Both d+ and + operate with 32-bit integer
        values.
d-       		( d1 d2 --- d1-d2 )
        "d-minus". Double number subtraction. (d1-d2). The high order cell contains a
        dummy zero for a place holder for compatibility with other
        Forth implementations. Both d- and - operate with 32-bit integer
        values.
d<       		( d1 d2 --- tf )
        "d-less-than". Compare two double numbers. TRUE is left on the stack
        if "d1" is less than "d2". The high order cell contains a
        dummy zero for a place holder for compatibility with other
        Forth implementations. 
decimal       		( --- )
        "decimal". Set current base to 10.
definitions       		( --- )
        "definitions".
depth       		( --- n )
        "depth". Leave the current parameter stack depth on the stack.
dir       		( --- )
        Executes a "dir" command to the OS. This works fine in DOS, CP/M, and
        VAX/VMS. In Unix and its' clones, you must create a shell script named dir.
dnegate       		( d --- d2 )
        "d-negate". Leave the two's complement of a double number on the
        top of the stack. The high order cell contains a
        dummy zero for a place holder for compatibility with other
        Forth implementations. 
do       		( limit start --- )
        "do". Begin a do loop. Can be used only in a macro definition.
              : ten-dots 10 0 do " ." loop ;
        See "loop".
does>       		( --- )
        "does". Not implemented, but not 100% necessary
        in Until because the same functionality can be achieved via
        C. I do plan to implement "does>" someday.
do_compile       		( --- )
        "do compile". This is an internal compiler word that should not
        be used!
do_definitions       		( --- )
        "do definitions". This is an internal compiler word that is the
        run-time portion of "definitions".
do_do       		Internal Use Only
        "do-do". This is the run-time portion of do.
do_loop       		Internal Use Only
        "do-loop". This is the run-time portion of loop.
do_vocabulary       		( --- )
        "do vocabulary". This is the run-time portion of "vocabulary".
drop       		( n --- )
        "drop". Remove the top number on the stack.
dump       		( addr len --- )
        "dump". Type a hex dump starting at "addr" for a length of "len".
        The actual number of bytes displayed is rounded up to the
        next 16-byte boundary to insure the complete last line in the dump prints.
dup       		( n --- n n )
        "dup".  Duplicate the top stack entry:
              1 dup
        Results in two 1s on the stack.
edit       		( 'filename --- )
        This word shells to the OS to execute the editor specified in
        the global C variable "editor". The default is EMACS, except for
        Coherent where the name is "me". Usage is:
              { filename} here edit
        or
              " filename" edit
        Once the filename has been specified in the first edit, simply use
        "here" to edit the same file again. Use "system" to use
        another editor at run-time.
else       		( --- )
        "else". Else is the starting execution point for the FALSE portion
        of an if statement. Its usage is:
              : aword   if ." true" else ." false" then ;
        Use only within a colon definition. See "if".
emit       		( c --- )
        "emit". Type the character on the top of the stack to the
        screen. 
              65 emit
        displays an `A'.
endif       		( --- )
        "end-if". A synonym for "then". 
        Terminate an "if" statement. "endif" is the branch
        point for exiting the conditional. See "if". It
        is used in the
        form"
              : aword    if true_words else false_words endif ;
              : bword    if true_words endif ;
        The "if" statement and its parts can only be used in a colon
        definition.
execute       		( 'word --- )
        "execute". Execute the word whose address, "'word", is on the
        top of the stack. Used in the form:
               ' aword execute
        Some Forths expect the CFA and others expect the PFA. Until uses the
        WA, which is the address of the dictionary entry structure. In the
        case where ' returns the CFA and "expect" requires the PFA, the
        word ">body" must
        be used to convert the CFA to the PFA. This is
        not required by Until.
exit       		( --- )
        "exit". This word presently does nothing at the present. Someday, it 
        will exit from the currently executing word. 
expect       		( addr len --- )
        "expect". Read up to "len" characters from the keyboard 
        into the buffer pointed to by "addr". Input is terminated either
        by a <CR> or "len" bytes being read. The number of bytes input
        can be obtained from the system variable "span".
fclose       		( fd --- )
        "f-close". Close the file whose file descriptor is "fd".
fcr       		( fd --- status )
        "f-c-r". Write a carriage return (well actually a newline) to the output
        file specified by "fd".
              descriptor @ fcr
feof       		( fd --- 0|eof )
        "f-e-o-f". Test the specified stream, "fd", for end of file. A
        zero return means not at EOF. Eof is returned if the last file operation
        encountered end of file.
           descriptor @ feof
                if   ." Got eof"
                else ." Not eof" then
ferror       		( fd --- tf )
        "f-error". This word tests the file stream for read or write error. FALSE
        means no error and TRUE indicates an error has occurred.
                descriptor @ ferror
                if   ." File error"
                else ." No error" endif
fence       		( --- )
        "fence". This is a special word that "forget" will not forget
        past. It protects the default word set from being accidentally
        deleted.
fflush       		( fd --- )
        "f-flush". This word flushes the system output buffer for the file
        specified by "fd". The file must have been opened previously by a call
        to "fopen". fflush only makes sense for output files.
        descriptor @ fflush
fgetc       		( fd --- c )
        "f-get-c". Read the next character from the file specified by
        the file descriptor fd. The character is true (-1) on End of File.
                descriptor @ fgetc
fgets       		( addr size fd --- count )
        "f-get-s". Read the next string of a max length of "size"
        into the buffer at
        "addr" from file "fd". The count is the number of bytes actually read
        or 0 for End of File. A "string" is logically a line in the file. The
        trailing newline character is included in the result string. "fgets"
        reads up to the new line character.
                buffer size descriptor @ fgets
                0= if ." EOF" then
        Empty lines have a length of one because "fgets" reads the trailing
        <newline> as part of the buffer.
fill       		( addr len byte --- )
        "fill". Fill memory starting at "addr" for a length of "len"
        with the value specified by "byte".
              trash 20 0 fill
        will copy 20 zeros into the memory starting at the address of "trash".
fload       		( --- )
        "f-load". "fload" loads a .BIN file. 
               fload anyfile.bin
fnsplit	       	( 'filename  --- 'drive 'dir 'file 'ext )
        "f-n-split". Break the filename contained in the string whose address
        is on the top of the stack into its component parts. This function
        is specific to Borland C++. A generic C version is under development.
                " c:\until\outer.c" fnsplit
                .( Extension> ) dup strlen type cr
                .( File     > ) dup strlen type cr
                .( Dir      > ) dup strlen type cr
                .( Drive    > ) dup strlen type cr
fopen       		( 'filename 'fmode --- fd )
        "f-open". Open the file whose name is stored in "'filename" and has a
        valid C file mode string. The predefined file modes in Until are
        BINARY, READ, WRITE, or APPEND. The file descriptor,
        "fd", should be saved and is used other file words to
        refer to the file.
              : test " filename" READ fopen
        The predefined modes are:
           - READ
           - WRITE
           - APPEND
           - BINARY
forget       		( --- )
        "forget". Forget the word following in the input stream:
              forget aword
        Forget stops at "fence". It
        calls abort if the word is not found
        in the dictionary. The term "forget" means to remove all
        word definitions from the dictionary starting with the latest
        word back to and including the word to forget.
forth       		( --- )
        "forth". This is the base vocabulary in this system.
forth-83       		( --- )
        "forth-83". Types the message "Until is almost Forth-83 compatible."
fputc       		( ch fd --- status)
        "f-put-c". Write the character, "ch", to the file whose descriptor is
        "fd". The status of the write is returned. The status is either
        the character on a successful write or EOF on error.
        ascii A fd@ fputc drop
fputline       		( 'string fd --- status )
        "f-put-line". Write the string whose address is next to the top of the
        parameter stack to the file whose file descriptor, "fd", is
        on the top of the stack. A <CR> is appended to the string
        before being written. "status" is EOF when an error occurs.
fputs       		( 'buffer fd --- status )
        "f-put-s". Write the string whose address is specified by "buffer"
        to the file descriptor, "fd". "status" is the last character
        written on success or EOF on error.
                string fd @ fputs drop
fread       		( addr len fd --- status )
        "f-read". Read a block of data from a disk file. This word eventually
        calls fread(). It reads "len" bytes into the buffer that 
        starts at "addr". The status is the number of bytes read on
        success or zero for end of file.
free       		( addr --- )
        "free". Return memory allocated by "malloc" to the system. 
        See "malloc" for an example.
fseek       		( whence offset fd --- status)
        "f-seek". This word positions the file, "fd", to the offset
        specified by "offset". The offset is not necessarily the 
        relative number of bytes from the beginning of the file. "whence"
        can be:
           0 Beginning of the file (SEEK_SET)
           1 From the current file position (SEEK_CUR)
           2 End of the file (SEEK_END)
        The status is zero on success and non-zero on error. DOS only
        returns an error when attempting to position a file that has
        not been opened. Other operating systems may return additional
        errors.
ftell       		( fd --- offset)
        "f-tell". Return the file offset for the file specified by "fd".
        An offset of -1 indicates an error. Most C compilers return the number
        of bytes from the beginning of the file, but this is not
        guaranteed!
fwrite       		( addr len fd --- status )
        "f-write". Calls the C function "fwrite()". It writes the
        buffer at "addr" for a length of "len" bytes to the
        file specified by "fd". Status is the number of bytes
        written or zero for an error.
gotoxy       		( col row --- )
        "go to x-y". Position the screen cursor to column "col" and
        row "row". This word uses a Borland C++ specific function and
        may need modification with other compilers.
gsub       		( 'string 'search 'replace --- #matches )
        "g-sub". The global search and replace word searches "string"
        for "search" and replaces it with "replace" when a
        match occurs. This version performs multiple replacements in
        the string. The number of replacements is returned.
           inbuf 1+ " is" " is not" gsub
        Searches <inbuf> for occurrances of "is" and replaces each with
        "is not". 
help       		( --- )
        "help". Looks up the specified word name in HELP.APP
        and prints the entry to the screen. Usage:
              help do
        prints the help entry for "do". Use the words command to type the
        words defined.
here       		( --- 'here )
        "here". Push the address contained in the system variable, "here"
        onto the parameter stack. In Until,
        "here" is allocated at startup. Most other 
        Forths allow it to float at the end of the dictionary. "here"
        is initially 128 bytes. "here" and "pad" are the
        same size.
hex       		( --- )
        "hex". Set the current base to 16.
hold       		( char --- )
        "hold". Add "char" to the numeric string. "hold" is used
        with <# ... #> to format numbers.
i       		( --- i )
        "i". Push the current do loop index onto the Data Stack. When loops
        are nested, i always refers to the current loop. 
           : test_i
             10 0 do i loop ;
if       		( tf --- )
        "if". Test the value on the top of the stack for True or False.
              5 5 =
              if   " True comparison"
              else " False comparison"
              endif
        The function do_if is executed at run-time.
immediate       		( --- )
        "immediate". Flag the last word in the dictionary as immediate. An
        immediate word executes at compile time rather than being compiled
        into the dictionary. Words such as "if" and "do"
        are immediate. Until
        primitives built with "build_iprim()" are immediate.
include       		( --- )
        "include". Compiles the file specified as the next token in the
        input stream. 
              include source.fth
        Compiles the file, source.fth, then returns to normal interpretation of
        the console. "include" is nestable.
insert       		( 'string 'new-string --- 'string )
        "insert". Insert "new-string" at the beginning of "string".
        The address of the original string is returned. 
j       		( --- j )
        "j". Return the index of the next outer do loop.
              : aword   10 0 do i . 5 0 do i . j . loop loop ;
        i will range from 0 to 4 and j will range from 0 to 9 in the
        inner loop. i in the 
        outer do loop is the same as j in the inner loop.
k       		( --- k )
        "k". Return the index of the second outer do loop.
           : aword   10 0 do i . 5 0 do 2 0 do i . k . loop loop loop ;
        i will range from 0 to 1 and k will range from 0 to 9
        in the inner loop. i in the 
        outer-most do loop is the same as k in the inner loop.
key       		( --- c )
        "key". Read a character from the keyboard. It eventually calls the
        C function "getch()". Its operation may vary slightly depending
        on the C compiler used. For example, some compilers may return the
        character immediately when typed and others not return a character
        until the <ENTER> key is pressed. 
              : aword    begin other-words ?terminal until key ;
        "?terminal" is typically called
        before key to ensure that a
        key is indeed ready to be read before calling "key".
list       		( --- )
        "list". Type the contents of a file. The usage is:
              list filename
lit       		( --- n ) Internal??
        "l-i-t". Fetch the value immediately following "lit" in the
        dictionary to the data stack. This is the run-time portion of
        "literal".
literal       		( n --- )
        "literal". Compile a literal number into the dictionary at compile
        time along with the WA of "lit". This word is the compile-time
        portion of "lit".
load       		( --- )
        "load". Not implemented. Use "include" to compile a text file
        of source code.
loop       		( --- )
        "loop". The end of a do construct.
              : xxx 10 0 do i . 2 +loop ;
        This do loop will execute 10 times. loop can be used only inside a macro
        definition. See "do".
malloc       		( n --- addr )
        "malloc". Allocate n bytes of system memory. The C "malloc()" function
        is called. Use "free" to return the memory to the system when done
        with it. Usage:
              100 malloc
              mem_ptr !
              ( some words )
              mem_ptr @
              free
        Use "realloc" to resize an array
        originally allocated by "malloc".
max       		( n1 n2 --- n3 )
        "max". Return the larger of n1 or n2.
              99 66 max
        returns 99. A good way to return a non-negative number is "0 max".
min       		( n1 n2 --- n3 )
        "min". Return the lesser of n1 and n2.
              99 66 min
        returns 66.
mod       		( n1 n2 --- mod )
        "mod". Calculate the remainder of n1 divided by n2:
              4 2 mod
        Leaves 0 on the stack.
msg       		High Level ( 'string --- )
        "message". Type a message to the console. The definition is:
              : msg   count type ;
move_string       		High level ('source 'destination  --- )
        "move string". Move the contents of "source" to "destination".
        Both addresses are counted strings. The destination string must be
        large enough to hold the source string. No check is made for
        string overflow.
negate       		( n1 --- n2 )
        "negate". Calculate the two's complement of a number.
not       		( n1 --- n2 )
        "not". Calculate the one's complement of a number.
null_strings       		( tf --- )
        "null strings". Until treats strings as counted by default for
        """. This word allows the programmer to toggle the operation of
        """ to switch to null terminated representation. 
                true  null_strings    \ Null terminated strings
                false null_strings    \ Counted strings
        This is a convenience word generally used with the C string words. 
        "null_strings" affects run-time string representation.
null->counted       		('null 'counted --- )
        "null-to-counted". Copy string from "null" to "counted",
        converting from null terminated to counted form. Typical interactive
        usage is:
              { string} here inbuf null->counted
              inbuf count type
or       		( n1 n2 --- tf)
        "or". Calculate the bitwise or of "n1" and "n2".
              1010 1010  - n1
              0101 0101  - n2
              1111 1111  - Result
over       		( n1 n2 --- n1 n2 n1 )
        "over". Copy "n1" to the top of the stack.
              1 2 over
        results in ( 1 2 1 ).
pad       		( --- 'pad )
        "pad". Push the address of "pad" onto the parameter stack. "pad"
        is a memory buffer allocated at start up for use as a scratch memory
        area for some string operations. It is used as 
        a temporary work area by many words. "pad" is allocated as a
        128 byte buffer at startup in Until. It does NOT float at the end
        of the dictionary as in conventional Forths.
pfa_list       		( --- 'pfa_list )
        "p-f-a list". Push the address of the C array "pfa_list" onto
        the parameter stack. pfa_list is used to accumulate temporary definitions
        during compilation. 
pfa_offset       		( --- 'pfa_offset )
        "p-f-a offset". Push the address of the C variable "pfa_offset"
        onto the stack. It is the current offset into "pfa_list".
pick       		( n --- value )
        "pick". Pick the value "n" items from the top of the stack and place
        a copy of it on the top of the stack. Assume the top five numbers 
        on the top of the stack are ( 1 2 3 4 5 ):
              5 pick
        copies the fifth item, 1, to the top of the stack ( 1 2 3 4 5 1 ).
        1 pick is the same as dup. 2 pick is the same as over.
plus_loop       		Internal Use Only
        "plus-loop". This is the internal word that is executed by "+loop".
        Do not use it in application code.
quote       		Internal Use Only
        "quote". This is the internal word called when " executes. 
r>       		( --- n )
        "r-from". Move the number on the top of the return stack to the top
        of the parameter stack. "r>" must be paired with a previous
        ">r". The Return Stack is used only for loop indices in Until.
r@       		( --- n )
        "r-fetch". Fetch the value from the top of the return stack. The return
        stack is unaffected.
READ       		( --- 'READ )
        "read". This word leaves the address of the "r" string used by fopen.
              " anyfile.dat" READ fopen
        opens "anyfile.dat" with read access.
        The Return Stack is used only for loop indices in Until.
realloc       		( 'memory-block --- addr )
        "re-alloc". Call the C function "realloc()" to reallocate a
        block of memory. "memory-block" is the address of a previously
        malloc'd block of memory. The return, "addr", is either the
        address of the newly allocated memory block or 0 on error.
repeat       		( --- )
        "repeat". Terminate a:
              begin ... while ... repeat
        loop. "repeat" branches to "begin". The exit test is
        done by the "while".
             : test
               0            \ initialize counter
               begin
                 dup 1+     \ increment count
                 dup 10 <   \ is count < 10
               while
                 ." ."      \ type a .
               repeat
               cr ." Done, 9 dots, but count is: " . cr
               ;
replace       		( 'string 'new_str 'sub_str --- 'string )
        "replace". Replace the substring, "sub_str",  starting at
        "string" with the new string, "new_str". The original
        address is returned. No check is made to see that the new
        string fits.
roll       		( n --- )
        "roll". Move the nth value on the stack to the top entry. All 
        entries are pushed down a cell. "n" does not include itself
        in the count. 
              3 roll
        is equivalent to "rot". The value
        of "n" must be greater than zero.
rot       		( n1 n2 n3 --- n2 n3 n1 )
        "rote". Rotate the third value on the parameter stack to the 
        top. The other values are pushed down one cell.
              1 2 3 rot
        results in (2 3 1).
seal       		( --- )
        "seal". Converts an Until source file into 'binary' form for later
        loading. It allows Until applications to be distributed without
        allowing users access to source code. The following line seals
        CALC.APP:
              seal calc.app
search       		( 'string 'search --- f|'match )
        "search". Search "string" for the substring "search". The
        address of the match is returned when "search" is found or
        0 for no match. For example:
               inbuf " xxx" search
        returns the match address or 0 when no match occurs.
search&insert       		( 'string 'search 'insert --- f|'match )
        "search and insert". Search "string" for the substring, "search",
        and insert "insert" after the match point. The address of the end of
        the match is returned on match or 0 for no match. For example:
           inbuf " is" "  not" search&insert
        will change an occurrance of "is" into "is not". 
SEEK_CUR       		( --- 1 )
        "seek-cur". Sets seek mode to relative to the current file position. Use
        with "fseek".
SEEK_END       		( --- 2 )
        "seek-end". Sets seek mode to relative to end of file. Use 
        with "fseek".
SEEK_SET       		( --- 0 )
        "seek-set". Sets seek mode to beginning of the file. Used
        with "fseek".
shell       		( --- )
        "shell". Spawn/fork/whatever a shell to the OS. 
              shell
        Log off to exit the shell and return to Until. This is "exit"
        in DOS, Control-D in **ix, and "logout" in VMS.
show_stack       		( tf --- )
        "show-stack". Turn on/off the stack display in the upper right corner
        of the screen.
sign       		( n --- )
        "sign". The number on the top of the stack is compared to zero. A
        `-' is added to the current output number string if "n" is 
        negative. 
              <# # # sign #>
        will generate a two digit numeric string with leading sign. The 
        numeric string is constructed in "pad".
        "sign" is used with the number formatting words <# ... #>.
             : 2digits    <# sign # # #> type ;
               -1234 2digits
               34-
smudge       		( 'word --- )
        "smudge". Hide the word whose address is on the top of the stack
        from the system. Used in the form:
               ' aword smudge
        It is generally used during compilation to prevent the word currently
        being define from being found. This prevents accidental recurrsion.
        "smudge" expects the word address (WA) in Until.
space       		( --- )
        "space". Output a space to the terminal.
spaces       		( n --- )
        "spaces". Output "n" spaces to the terminal. Logically "spaces"
        is:
              : spaces  0 do space loop ;   ( n --- )
        The line:
              0 spaces
        outputs no spaces. This is slightly different from normal Forth behavior,
        so beware.
span       		( --- 'span )
        "span". Returns the address of "span". It contains the number of
        characters read in the last "expect" call.
state       		( --- 'state)
        "state". Leave the address of the variable containing the
        current compilation state on the stack. "state" is zero during
        interactive mode and non-zero during compilation.
stab       		High level (char 'destination --- )
        "stab". Store a byte, or stab, appends a byte to the end of the counted
        string whose address is specified by "destination". The count
        byte is automatically incremented. "stab" only works with
        counted strings and is high level code found in STOIC.APP.
strap       		High level ('source 'destination  --- )
        "strap". The string append word appends the contents of the source
        string, "'source", to the destination string, "'destination".
        Both source and destination string must be a counted string.
        "strap" only works with
        counted strings and is high level code found in STOIC.APP.
strcat       		( 'dest 'source --- 'dest )
        "s-t-r cat". Concatenate the source string to the destination string.
        "strcat" only works with null terminated strings.
strcmp       		( 'string1 'string2 --- <1|0|>1 )
        "s-t-r c-m-p". Compare two strings. Zero means the strings match. If
        the return value is less than zero, string1 is less than string2. If
        the return value is positive, string1 is greater than string2.
        "strcat" only works with null terminated strings.
strcpy       		( 'dest 'source --- 'dest )
        "s-t-r c-p-y". Copy the string whose address is "source"
        into the string at "dest". No check is made for string overflow.
        "strcat" only works with null terminated strings.
strncat       		( 'dest 'source len --- )
        "s-t-r-n-c-a-t". Concatenate n bytes of the source string to
        the destination string.
        "strcat" only works with null terminated strings.
strncmp       		( 'string1 'string2 len --- )
        "s-t-r-n-c-m-p". Compare two strings for "n" bytes.
        "strcat" only works with null terminated strings.
strncpy       		( 'dest 'source len --- )
        "s-t-r-n-c-p-y". Copy "n" bytes of one string to another.
        "strcat" only works with null terminated strings.
string       		( len ---  )
        "string". Define a string array of "len" bytes. At run-time,
        the address of the count byte is left on the stack.
              100 string text         ( len --- )
              text                    ( --- 'text )
        This is equivalent to:
              variable text 100 allot
string_returns       		( tf --- )
        "string return". C string functions have the nasty habit of
        returning values that most C programmers ignore. This causes code
        that is a mess in Until to explicitly "drop" unused function values.
        This word controls whether the C string words return values to 
        the parameter stack.
              true  string_return   \ String words return values
              false string_return   \ String words do not return values
        This approach gives the programmer a choice. The words affected
        by "string_returns" are:
           - strcat
           - strcpy
           - strncat
           - strncpy
strret       		( --- return-value )
        "s-t-r-r-e-t". Return the value of the last C string word call.
strupr       		( 'string --- 'STRING )
        Convert the null terminated string whose address is "string"
        to all upper case. 
sub       		( 'string 'search 'replace --- tf )
        "sub". The global search and replace word searches "string"
        for "search" and replaces it with "replace" when a
        match occurs. This version performs only a single replacement 
        per call.
swap       		( n1 n2 --- n2 n1 )
        "swap". Swap the top two entries on the top of the stack.
              1 2 swap
        results in the top stack entry being 2 and the second entry
        being 1.
system       		( --- )
        "system". Used in the form:
              " command" system
        This command shells out to the operating system and executes the
        command, such as dir. 
then       		( --- )
        "then". Terminate an "if" statement. "then" is theanch
        point for exiting the conditional. See "if". It
        is used in the form:
              : aword    if true_words else false_words then ;
              : bword    if true_words then ;
        The "if" statement and its parts can only be used in a colon
        definition. "endif" is a synonym for "then".
tib       		( --- 'tib )
        "t-i-b". Leave the address of the terminal input buffer on the stack.
        "tib" is allocated a buffer of 256 bytes in Until at startup.
time       		( --- )
        "time". This word calls the operating system "time()" function,
        if available.
tolower       		( CH --- ch )
        "tolower". Convert a character to lower case.
toupper       		( ch --- CH )
        "toupper". Convert a character to upper case.
token       		( ?? --- ?? )
        "token". 
type       		( addr len --- )
        "type". Type the contents of "addr" for a length of "len"
        bytes to the terminal.
u.       		( u --- )
        "u-dot". Type the value of the unsigned number on the top of the stack
        to the terminal.
u<       		( u1 u2 --- tf)
        "u-less-than". Perform an unsigned compare of the two unsigned numbers
        on the top of the stack. Leave TRUE or FALSE on the stack. (u1<u2).
              99000 99900 u<
        will return TRUE on the stack. "u<" is typically used to compare
        very large numbers that would be treated as negative by <.
u>       		( u1 u2 --- tf)
        "u-greater-than". Perform an unsigned compare of the two unsigned
        numbers on the top of the stack. Return TRUE if u1 is greater than
        u2. Else, return FALSE. (u1>u2).
              99000 99900 u>
        will return FALSE. "u>" is typically used for large positive
        numbers.
um*       		( d1 u1 --- d1*u1 )
        "u-m-star". Unsigned mixed length multiply. This is a 32-bit multiply
        in Until. 
um/mod       		( d1 u2 --- rem quot )
        "u-m slash mod". Unsigned multiple length division. (d1/u2)
unseal       		( --- )
        "unseal".  Converts a sealed Until source file into 'ASCII' form for 
        reference. It allows sealed Until applications to be examined
        for debugging. "unseal" is the opposite of "seal".
        The following line unseals CALC.BIN:
              unseal calc.bin
until       		( tf --- )
        "until". Terminate a "begin" loop.
              : aword   begin some_words until ;
        "until" branches back to "begin" if the value on the stack
        is FALSE. TRUE causes the loop to terminate.
variable       		( --- addr )
        "variable". At compile time, creates a variable:
              variable trash
        The compile time stack comment for "variable" is ( --- ).
        At run time, push the address of the variable onto the stack:
              trash    ( --- addr.of.trash )
vocabulary       		( --- )
        "vocabulary". Create a new vocabulary branch in the dictionary. The
        root vocabulary is "forth". Usage is:
              vocabulary  my.vocab immediate
        All vocabularies should be declared immediate so they can be used
        inside of colon definitions. Using "my.vocab" sets the vocabulary
        pointer, "context", to search my.vocab.
while       		( tf --- )
        "while". Test for exit of a "begin ... while ... repeat"
        loop. When the truth flag is TRUE, execution continues to "repeat"
        then branches back to "begin". When the truth flag is FALSE, 
        execution branches immediately past "repeat" to exit the loop.
        See "repeat".
             : test
               0            \ initialize counter
               begin
                 dup 1+     \ increment count
                 dup 10 <   \ is count < 10
               while
                 ." ."      \ type a .
               repeat
               cr ." Done, 9 dots, but count is: " . cr
               ;
word       		( delimiter --- 'token )
        "word". Parse the next token from the input stream delimited by
        the character, "delimiter". 
              ascii } constant brace
              : {   brace word drop ;
        The address of the token is left on the stack. In Until, "word"
        always leaves the token at "pad". 
words       		( --- )
        "words". Display all of the currently defined macro names on the computer
        screen.
WRITE       		( --- 'WRITE )
        "write". Leave the address of the write file open modifier, "w"
        on the top of the stack.
              " anyfile.dat" WRITE fopen
xor       		( n1 n2 --- n3 )
        "x-or". Return the exclusive OR of n1 and n2.
[       		( --- )
        "left-bracket". Turn compilation mode off. Words following [ in the
        input stream are executed rather than being compiled. [ is generally
        paired with ] in a colon definition.
              : aword   compiled_words [ executed_words ] compilation words ;
        See ].
[']       		( --- )
        "bracket-tick". A special form of tick that is immediate. 
        It is generally used to retrieve the WA of an immediate word rather
        than execute it. For example:
              : got.true   ." True..." ;  immediate
              : got.false  ." False..." ; immediate
              : test       ( tf --- )
                           if   ['] got.true
                           else ['] got.false
                           then
                           execute;
[compile]       		( --- )
        "bracket-compile". Compile an immediate word into the dictionary.
        The word following in the input stream is used.
              : aword   [compile] ['] ;
        The effect is the same as "['] aword execute". This
        is used in traditional Forths to build looping words such
        as "while" and "repeat".
\       		backslash ( --- )
        "backslash". Comment to end of line. The rest of the line is
        ignored by the compiler.
               some words   \ this portion is a comment
]       		( --- )
        "right bracket". Turn on compilation mode. Text from the input
        stream is now compiled instead of executed. ] is usually
        paired with [.
             : five   [ ascii 5 ] literal emit ;
        When "five" executes, the digit 5 is displayed on the
        terminal.
{       		( --- )
        "left brace". Leave a {} delimited string at "here". The string
        is stored null terminated. Use { interactively for specifying 
        parameter strings to operating system type words such as "edit".
        A typical call is:
              { filename} here edit
        This { cannot be used inside a colon definition.

Note:

