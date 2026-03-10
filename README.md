                                  UNTIL V2.5.2

FILE: 00READ.ME DATE: 07/30/95

                >>> BACK UP THIS DISK BEFORE USING!!! <<<

Welcome to UNTIL, the UNconventional Threaded Interpretative Language.
It is a Forth-like language written in C for extreme portability.
Versions of Until run on at least four operating systems, compiled with
five different C compilers. Until has about 95% of the functionality of
a full Forth system; plus some features available in few if any other
Forth's. It was designed from the ground up to be embedded in other
programs and used as a 'macro' language. Both calling other C functions
and being called by other C functions is extremely easy with Until.

This disk comprises the UNTIL Language Materials and is Copyrighted. See
the file LICENSE.TXT or the documentation (DOC.LIS) for details. The
distribution requirements are very liberal.

Files included in this package are:

  FILE               DESCRIPTION
  ------------------ ------------------------------------------------------------
  00READ.ME          This file.
  ADD.APP            This is a simple filter program that adds up a the
                     first column of numbers in a file. I used this in
                     conjunction with a Unix-like wc programs to count
                     the number of lines in the source files for Until.  
  ADDR1.APP          A version of the S-Engine program to generate a Roff5
                     output for the SGML address book application.
  ADDRESS.APP        The S-Engine program to process the address book file
                     and produce a browsable HTML output file.
  APPLIC.APP         A simple default application file.
  CALL.C             The Compiler module source code.
  CASE.APP           Simple test file for the case statement.
  CASE.C             Code for the 'case' words.
  COMPILER.H         Header file for selecting the compiler being used.
  DEBUG.C            Source for simple C-level debugging functions.
  FILE.C             The source code for the C I/O library words.
  FILEXAMP.APP       The File Word example code from the Ref Manual.
  FILTER.APP         A skeleton file filter program. It reads a file, and
                     executes a user specified word on each line. When used
                     with the search and replace words, FILTER provides a 
					 very convenient text filter.
  FILTEREX.APP       A file filter example that floads FILTER.APP.
  FIXENT.APP         This filter translates the default entities into their
                     printable form. It was used in creating the printable
                     Until Reference Manual from the base HTML doc file.
  FLOAT.C            Source code for floating point words.
  FUNCTS.H           Function prototypes.
  G2HELP.APP         An Until filter program that generates help.app from the
                     base HTML word glossary. Note the HTML version of the
                     file is included when you purchase the printed manual.
  HELP.APP           The help file used inside Until.
  HELP.C             Source for the help file module.
  IO.C               File IO module source code.
  IO.H               Include file for IO.H.
  LICENSE.TXT        Until License text.
  MAKEFILE.UNX       Unix makefile. Works with Sun Sparc ANS C, GCC on a Sparc,
                     Silicon Graphics ANS C, and Linux GCC.
  MAKEFILE.BCC       A makefile for Turbo C++/Borland C++.
  MATH.C             Math module source code.
  OUTER.C            The Outer Interpreter source code.
  PCOLON.APP         Sample Until program to print all colon definitions
                     in a source file.
  PRIM.C             Until primitive words source code.
  QUOTE.C            Source code that implements quoted constant strings.
  SCREEN.C           Screen output code.
  SEAL.C             Seal module source code.
  SEARCH.C           An experimental module that contains simple, brute
                     force search and replace code. Note that while this code
                     has had good results from initial testing, it is not
                     documented. Use the words defined in it at your own risk!
  SEARCH.H           Include file used by SEARCH.C.
  SEARCH1.APP        A simple file filter demonstrating the search and replace
                     word set.
  SENGINE.C          The source code for the SGML document intrepreter named
                     S-Engine.
  SENGINE.H          Include file for SENGINE.C.
  SHARP.C            The Forth formatting words set, \<#, #, #\>, etc.
  SHOWBIT.C          The source code for .bin.
  STOIC.APP          The Stoic String package.
  STREXAMP.APP       C String example from the documentation.
  STRING.APP         Example of the C String word set.
  STRING.C           String module source code.
  SYSTEM.C           Source code for words that call C system functions.
  UNTIL.APP          Default, minimal startup code. It is sealed
                     into UNTIL.BIN.
  UNTIL.BIN          Sealed version of UNTIL.APP.
  UNTIL.C            Main program source code.
  UNTIL.EXE          Executable Until.
  UNTIL.H            Until data structures and global extern variables.
  UNTIL25.DOC        Until V2.5 Reference Manual.
  UNTILPRI H         Untill data structures and global extern variables.
  USER.C             Primitive C words interfaced here.
  VECTORED.C         Source code for vectored code.
  VOCAB.C            Vocabulary module source code.

The absolute minimum you need to run with is UNTIL.EXE. It's a good idea
to have an UNTIL.BIN for words you always use or for a complete
application if you distribute one. The file APPLIC.APP is always loaded
if it exists. This is a convenient place to put code in development.
Other source files may be specified on the command line via the -f
option:

    UNTIL -fpcolon.app

will run Until and compile pcolon.app.

The default editor from within Until is EMACS. I use MicroEMACS. The
editor name can be changed in UNTIL.H, then recompile Until or rename
your normal editor to EMACS.EXE.

## DOCUMENTATION

A plain ASCII version of the Until Reference Manual is contained on this
disk. Typeset manual and electronic HTML document versions are
available. A minimal version of Until is also described in "Write Your
Own Programming Language Using C++", ISBN \# 1-55622-264-5. Examples of
using Until as embedded macro languages can be found in "Programming
Output Drivers Using Borland C++", ISBN \# 1-55622-309-9 and in "Getting
the Most From Your HP LaserJet Printer", ISGN \# 1-55622-441-9. The
books are published by Wordware Publishing (214-423-0090) and can be
ordered from FIG or any bookstore.

The ASCII manual has been revised. It is now up to date. A new section
on the string was added along with an updated word glossary. HELP.APP
has also been regenerated and includes all of the words in the glossary.

A browsable HTML version of the Reference Manual with numerous
hyperlinks is available on the World Wide Web at
http://taygeta.oc.nps.navy.mil/until.html. A link to it is included on
the Forth Page on Taygeta.

## RELEASE NOTES

2.5.2 Corrected several compile errors for compilers I don't have access
to. Corrected bug in fconstant. 2.5 Public release of the changes since
V2.2.2 including updated documentation. Release prepared for the 1995
Rochester Forth Conference. Special thanks to Skip Carter, Richard
Secrist, and Michael Mundrane who have all made signficant contributions
to Until since V2.2.

2.4 Merged V2.3 with Skip Carter's C-level I/O redirection code, added
error checking for all malloc() calls, backslash quoted characters in "
..." string constants, getenv, case, and the S-Engine SGML document
interpreter. Treated as a working update and not released.

2.3 Initial floating point and printf implementations. Not released.

2.2.2 Bug fix where stacks were not being malloc()'d properly.

V2.2 Reworked UNTIL.C to make Until easier to embed and wrote
instructions. Replaced help() with code contributed by Michael Mundrane.
Modified get_stdin() to call fgets() instead of gets() and make EOF from
the keyboard exit Until instead of generating an error per a suggestion
by Michael Mundrane. Moved file descriptor stack from UNTIL.H to IO.H.

    Cleaned up code by removing unreferenced variables per
    suggestions by Akira Kida. This eleminates most of the warning
    messages generated while compiling Until. Akira also
    contributed a working version of shell() for Unix systems. 

    Corrected bug in SEAL.C. The code to leave blanks embedded
    blanks in ." and " had not been written previously. 

    Corrected coding errors in SEARCH.C. The string search and
    replace words are now functional. Michael Mundrane supplied
    me with a revised SEARCH.C as part of beta testing, which
    will probably be included in a future release. The words
    have also been included in the word glossary.

    Compiled Until under VAX/VMS. There are several compile errors
    that should be fixed. Until seems to run OK, but has not
    been extensively tested yet. The DCL command files to compile
    and link Until are now included in the distribution.

V2.1 There are one major enhancement in V2.1 and several lesser ones.
The big one is that the port to 'Unix' is now done. The supplied
makefile works with Sun Sparc ANS C and SGI ANS C unchanged. Simply
change the 'cc' to 'gcc' and the makefile works on GCC on the Sparc as
well. Hopefully, it is generic enough to work on any Unix system. Of
course TC++ and BC++ still compile Until via the project file.

    I discovered that \ does not work properly with the new version 
    of word() from V2.0.4. I have reverted back to the original
    word() for the time being. 

    A new module, SEARCH.C, and associated words, sub and gsub, were
    added. I use AWK a fair amount at work to clean up HTML files
    during document conversion. About 90% of the AWK programs are
    searching for one string and replacing it with another. sub
    and gsub words are equivalent to the AWK functions of the same
    name. These along with FILTER.APP, which contains a skeleton
    file filter program, make writing these same sorts of programs
    very convenient with Until. I have not documented the words in
    SEARCH.C yet because I want to use the code some more to be
    sure it 'feels right' in actual use. So, expect the final version
    of SEARCH.C, including documentation, in the next Until release.

    I cleaned up the internal source file handling a little so
    warning messages display when UNTIL.BIN is missing or when
    a non-existent file is specified with fload or include.

V2.0.4 Second public release. It contains a couple of minor bug fixes, a
new word called UNSEAL, and I re-wrote word() to call the C system
function strtok() instead of parsing the input stream a character at a
time. The new word() should be faster, although I never got around to
doing any timings.

    The port to Unix still has not been done. That should be in
    V2.1. The documentation has had a major overhaul and reflects
    the state of the system. The documentation is a primary reason
    for releasing this version.

V2.0.1 Until for the PC. This is the first publicly available version of
the "full" Until. As far as I know, there are no bugs. Until has been in
frequent use writing text file for the past couple of months with only a
couple of minor problems, which have been fixed.

    Code is in Until to be compiled on several other platforms.
    Be forewarned that while Until compiles on other systems,
    IT DOES NOT RUN. Porting to other platforms is my next
    priority after finishing the documentation update. An earlier
    version of Until did compile under several operating systems
    so, it should be a matter of simply finding the time.

------------------------------------------------------------------------

# THINGS YOU SHOULD KNOW

1)  The UNTIL.EXE is compiled with Turbo C++ for the 8086 with floating
    point emulation. If you use a higher processor or have a math
    coprocessor, you should recompile Until to get the best possible
    performance on your computer.

2)  When using floating point, remember that 0, 1, 2, and 3 are words;
    they are not processed by number(). So:

    f# 1

    does not do what you intend. It pushes 1 onto the parameter stack
    and leaves number() vectored to floating point. But if you type:

    f# 1.

    you will get 1.00 on the floating point stack.

    This is not a bug, but something that can trip you up.

3)  When running Until from a batch file, you will get an "EOF on STDIN"
    message when word tries to read interactively. As near as I can
    tell, the command file is treated as STDIN. This is the way it works
    with MS-DOS and OpenVMS. I haven't checked it on \*nix.

------------------------------------------------------------------------

    Norman E. Smith
    114 Claremont Road
    Oak Ridge, TN. 37830

    CompuServe: 72745,1566
    Internet:   72745.1566@compuserve.com

------------------------------------------------------------------------

ORDER FORM

                                                      Quantity     Total
                                                      --------     -----

Until Reference Manual \$25 X \_\_\_\_ \_\_\_\_\_\_\_ includes printed
and HTML versions and latest disk.

Latest Until disk alone. \$ 5 X \_\_\_\_ \_\_\_\_\_\_\_

Write Your Own Programming Language Using C++ \$15 X \_\_\_\_
\_\_\_\_\_\_\_

Programming Output Drivers Using Borland C++ \$15 X \_\_\_\_
\_\_\_\_\_\_\_

Getting the Most From Your HP LaserJet Printer \$16 X \_\_\_\_
\_\_\_\_\_\_\_ When available.

                                   Sales Tax TN only (8%)         ______

                                    Shipping and Handling           4.00

                                              Total Order     __________

Send check or money order to:

Norman Smith 114 Claremont Rd. Oak Ridge, TN. 37830
