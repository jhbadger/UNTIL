\ FILE: FIXENT.APP
\ 
\ This filter translates the default entities into their
\ printable form. It was used in creating the printable 
\ Until Reference Manual from the base HTML doc file.
\

fload filter.app

inbuf 1+ constant data

: fix.ents
   data " &lt;"     " <"   gsub drop
   data " &gt;"     " >"   gsub drop
   data " &amp;"    " &"   gsub drop
   data " &action;" " ---" gsub drop
   data " &numb;"   " #"   gsub drop
   data  fout @ fputs drop
   ;

: doit 1 argv[] 2 argv[] ['] fix.ents filter ;

\ doit

