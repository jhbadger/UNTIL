/*
|       FILE: USER.C
|
|	This module contains copyrighted source code for part the 
|	UNTIL Language Materials. 
|
|	Written by:
|		Norman E. Smith, CDP
|		Copyright 1992, 1994
|		All Rights Reserved
|
|	Right to use, copy, and modify this code is granted
|	for personal non-commercial use, provided that this
|	copyright disclosure remains on ALL copies. Any other
|	use, reproduction, or distribution is covered in the
|	License Agreement with the Until Language Maeterials
|	documentation and in the file LICENSE.TXT.
*/

#include "compiler.h"

#include <stdio.h>
#ifndef NO_STDLIB
#include <stdlib.h>
#endif

#define until

#include "untilpri.h"
#include "functs.h"

#ifdef SEARCH_MODULE
#include "search.h"
#endif

/***********************+------------+****************************
|                       | make_prims |
|                       +------------+
|
|  NOTE: The way words are linked, the dict order is "bottom-up"
|        in terms of search order. The order has no affect on
|        run-time. More fequently used words should be at the
|        end of the list to speed up compilation.
*/
void make_prims()
{
	printf("\nBuild primitives...\n");
	build_primitive( "align",         do_align);
	build_primitive( ".bin",          dot_bin);
	build_primitive( "help",          help);
	build_primitive( "system",        do_system);
	build_primitive( "do_compile",    do_compile);
	build_primitive( "shell",         do_shell);
	build_primitive( "list",          type_file);
	build_primitive( "dir",           do_dir);
	build_primitive( "create",        do_create);
	build_primitive( "edit",          edit);
	build_primitive( "forget",        forget);

	build_primitive( "printf",        prim_printf);
	build_primitive( "sprintf",       prim_sprintf);
	build_primitive( "fprintf",       prim_fprintf);
#ifdef FLOAT_POINT
	build_primitive( "printf_f",      prim_printf_f);
	build_primitive( "sprintf_f",     prim_sprintf_f);
	build_primitive( "fprintf_f",     prim_fprintf_f);
#endif
#ifdef SHARP_WORDS
	build_primitive( "<#",            less_sharp);
	build_primitive( "#",             sharp);
	build_primitive( "#>",            sharp_greater);
	build_primitive( "#s",            sharp_s);
	build_primitive( "hold",          hold);
	build_primitive( "sign",          sign);
#endif
	build_primitive( "lit",           do_lit);
	build_primitive( "<outer>",       outer);
	build_primitive( "base",          base_prim);
	build_primitive( "0<",            zero_less);
	build_primitive( "0>",            zero_greater);
	build_primitive( "0=",            zero_equal);
	build_primitive( "1+",            one_plus);
	build_primitive( "1-",            one_minus);
	build_primitive( "2+",            two_plus);
	build_primitive( "2-",            two_minus);
	build_primitive( "2/",            two_divide);
	build_primitive( "allot",         allot);
	build_primitive( "malloc",        do_malloc);
	build_primitive( "realloc",       do_realloc);
	build_primitive( "free",          do_free);
	build_primitive( "execute",       execute);
	build_primitive( "dump",          dump);
	build_primitive( "variable",      Compile_variable);
	build_primitive( "constant",      Compile_constant);
	build_primitive( "string",        Compile_string);
	build_primitive( "u>",            u_greater_than);
	build_primitive( "u<",            u_less_than);
	build_primitive( "=",             equals);
	build_primitive( ">",             greater_than);
	build_primitive( ">=",            greater_than_equal);
	build_primitive( "<",             less_than);
	build_primitive( "<=",            less_than_equal);
	build_primitive( "-",             minus);
	build_primitive( "+",             plus);
	build_primitive( "rot",           rot);
	build_primitive( "swap",          swap);
	build_primitive( "dup",           prim_dup);
	build_primitive( "2dup",          two_dup);
	build_primitive( "/",             divide);
	build_primitive( "*",             multiply);
	build_primitive( "-rot",          minus_rot);
	build_primitive( "c!",            cstore);
	build_primitive( "!",             store);
	build_primitive( "c@",            cfetch);
	build_primitive( "@",             fetch);
	build_primitive( "r@",            rfetch);
	build_primitive( ">r",            to_r);
	build_primitive( "r>",            r_from);
	build_primitive( "-find",         minus_find);
	build_primitive( "drop",          drop);
	build_primitive( "word",          word);
	build_primitive( "cold",          cold);
	build_primitive( "immediate",     immediate);
	build_primitive( "words",         words);
	build_primitive( "0",             zero);
	build_primitive( "1",             one);
	build_primitive( "2",             two);
	build_primitive( "3",             three);
	build_primitive( "abort",         abort_F);
	build_primitive( "abort_quote",   abort_quote);
	build_primitive( "bye",           bye);
	build_primitive( "dword",         display_word);
	build_primitive( "vword",         v_word);
	build_primitive( "pad",           addr_pad);
	build_primitive( ">in",           to_in);
	build_primitive( "tib",           addr_tib);
	build_primitive( "#tib",          number_tib);
	build_primitive( "pfa_list",      pfa_list_addr);
	build_primitive( "pfa_offset",    pfa_offset_value);
	build_primitive( "comma_offset",  comma_offset_value);
	build_primitive( "count",         count);
	build_primitive( ".s",            dot_s);
	build_primitive( "depth",         depth);
	build_primitive( "quote",         quote);
	build_primitive( "*/",            star_slash);
	build_primitive( "+!",            plus_store);
	build_primitive( "*/mod",         star_slash_mod);
	build_primitive( "/mod",          slash_mod);
	build_primitive( "decimal",       decimal);
	build_primitive( "hex",           hex);
	build_primitive( "binary",        binary);
	build_primitive( "?dup",          query_dup);
	build_primitive( "abs",           absolute);
	build_primitive( "cmove",         cmove);
	build_primitive( "cmove>",        cmove_up);
	build_primitive( "fill",          fill);

	build_primitive( "forth",         prim_forth);

	build_primitive( "forth-83",      forth_83);
	build_primitive( "here",          here);
	build_primitive( "load",          U_load);
	build_primitive( "fload",         fload);
	build_primitive( "max",           prim_max);
	build_primitive( "min",           prim_min);
	build_primitive( "mod",           mod);
	build_primitive( "negate",        negate);
	build_primitive( "not",           not);
	build_primitive( "or",            or);
	build_primitive( "xor",           xor);
	build_primitive( "and",           and);
	build_primitive( "over",          over);
	build_primitive( "pick",          pick);
	build_primitive( "roll",          roll);
	build_primitive( "span",          span_prim);
	build_primitive( "state",         state_prim);
	build_primitive( "u.",            u_dot);
	build_primitive( "um*",           um_star);
	build_primitive( "um/mod",        um_slash_mod);
	build_primitive( "0bran",         zero_bran);
	build_primitive( "branch",        branch);
	build_primitive( "do_of",         do_of);
	build_primitive( "do_endcase",    do_endcase);
	build_primitive( "do_case",       do_case);
	build_primitive( "do_do",         do_do);
	build_primitive( "do_loop",       do_loop);
	build_primitive( "do_for",        do_zero_do);
	build_primitive( "do_vocabulary", do_vocabulary);
	build_primitive( "definitions",   do_definitions);
	build_primitive( "plus_loop",     do_plus_loop);
	build_primitive( "smudge",        do_smudge);
	build_primitive( "i",             do_i);
	build_primitive( "j",             do_j);
	build_primitive( "k",             do_k);
	build_primitive( "-trailing",     minus_trailing);
	build_primitive( "exit",          f_exit);
	build_primitive( "exit()",        force_exit);
	build_primitive( "{",             do_cstring);
	build_primitive( "expect",        prim_expect);
#ifdef CIO_MODULE
	build_primitive( "fnsplit",       file_fnsplit);
	build_primitive( "fopen",         file_fopen);
	build_primitive( "fclose",        file_fclose);
	build_primitive( "fgetc",         file_fgetc);
	build_primitive( "fgets",         file_fgets);
	build_primitive( "fputc",         file_fputc);
	build_primitive( "fputs",         file_fputs);
	build_primitive( "fcr",           file_fcr);
	build_primitive( "fflush",        file_fflush);
	build_primitive( "ftell",         file_ftell);
	build_primitive( "fseek",         file_fseek);
	build_primitive( "fread",         file_fread);
	build_primitive( "fwrite",        file_fwrite);
	build_primitive( "fputline",      file_fputline);
	build_primitive( "STDIN",         file_stdin);
	build_primitive( "STDOUT",        file_stdout);
	build_primitive( "STDERR",        file_stderr);
	build_primitive( "READ",          ft_read);
	build_primitive( "WRITE",         ft_write);
	build_primitive( "APPEND",        ft_append);
	build_primitive( "BINARY",        ft_binary);
	build_primitive( "UPDATE",        ft_update);
	build_primitive( "NEW_UPDATE",    ft_new_update);
	build_primitive( "APPEND_UPDATE", ft_append_update);
	build_primitive( "TEXT",          ft_text);
#endif
	build_primitive( "argc",          xargc);
	build_primitive( "argv",          xargv);
	build_primitive( "time",          prim_ftime);
	build_primitive( ">counted",      to_counted);
	build_primitive( ">null",         to_null);
	build_primitive( "null->counted", n_to_count);
	build_primitive( "include",       finclude);
	build_primitive( "d+",            d_plus);
	build_primitive( "d-",            d_minus);
	build_primitive( "d<",            d_less);
	build_primitive( "dnegate",       dnegate);
	build_primitive( "?",             query);
	build_primitive( "]",             right_bracket);
	build_primitive( "show_stack",    set_show_stack);
	build_primitive( "show_copy",     set_show_copy);
	build_primitive( "errno",         return_errno);
	build_primitive( "string_returns",set_str_return);
	build_primitive( "null_strings",  set_str_type);
	build_primitive( "strret",        str_return);
	build_primitive( "getenv",        prim_getenv);
#ifdef C_STRINGS
	build_primitive( "strncmp",       C_strncmp);
	build_primitive( "strcmp",        C_strcmp);
	build_primitive( "strncat",       C_strncat);
	build_primitive( "strcat",        C_strcat);
	build_primitive( "strncpy",       C_strncpy);
	build_primitive( "strcpy",        C_strcpy);
	build_primitive( "strlen",        C_strlen);
	build_primitive( "strupr",        C_strupr);
	build_primitive( "atol",          C_atol);
#endif
#ifdef SEAL_MODULE
	build_primitive( "seal",          seal);
	build_primitive( "unseal",        unseal);
#endif
	build_primitive( "token",         prim_token);
#ifdef SEARCH_MODULE
	build_primitive( "search",        call_search);
	build_primitive( "insert",        call_insert);
	build_primitive( "replace",       call_replace);
	build_primitive( "search&insert", call_search_insert);
	build_primitive( "sub",           call_search_replace);
	build_primitive( "gsub",          call_gsearch_replace);
	build_primitive( "clear_temp",    call_clear_temp);
#endif
	build_primitive( ",",             do_comma);
	build_primitive( "c,",            do_c_comma);

	build_primitive( "do_does",       do_does);
	build_primitive( "Compile_do_does",Compile_do_does);
#ifdef VECTORED_IO
	build_primitive( ".",             exec_dot);
	build_primitive( ".quote",        exec_dot_quote);
	build_primitive( ".s",            exec_dot_s);
	build_primitive( "..s",           exec_g_dots);
	build_primitive( "type",          exec_type);
	build_primitive( "cr",            exec_cr);
	build_primitive( "emit",          exec_emit);
	build_primitive( "key",           exec_key);
	build_primitive( "space",         exec_space);
	build_primitive( "spaces",        exec_spaces);
	build_primitive( "?terminal",     exec_query_terminal);
/*
	build_primitive( "prints",        exec_prints);
	build_primitive( "\n",            exec_newline);
	build_primitive( "center",        exec_center);
	build_primitive( "verbatim",      exec_verbatim);
*/
	build_primitive( "dot_vect",      addr_dot_vect);
	build_primitive( ".\"_vect",      addr_dot_quote_vect);
	build_primitive( "dot_s_vect",    addr_dot_vect);
	build_primitive( "g_dots_vect",   addr_dot_vect);
	build_primitive( "type_vect",     addr_type_vect);
	build_primitive( "cr_vect",       addr_cr_vect);
	build_primitive( "emit_vect",     addr_emit_vect);
	build_primitive( "key_vect",      addr_key_vect);
	build_primitive( "space_vect",    addr_space_vect);
	build_primitive( "spaces_vect",   addr_spaces_vect);
	build_primitive( "?terminal_vect",addr_query_terminal_vect);
	build_primitive( "printf_vect",   addr_printf_vect);
/*
	build_primitive( "prints_vect",   addr_prints_vect);
	build_primitive( "\n_vect",       addr_newline_vect);
	build_primitive( "center_vect",   addr_center_vect);
	build_primitive( "verbatim_vect", addr_verbatim_vect);
*/
#else
	build_primitive( ".",             dot);
	build_primitive( ".quote",        dot_quote);
	build_primitive( "type",          type);
	build_primitive( "cr",            cr);
	build_primitive( "emit",          emit);
	build_primitive( "key",           key);
	build_primitive( "space",         space);
	build_primitive( "spaces",        spaces);
	build_primitive( "?terminal",     query_terminal);
/*
	build_primitive( "prints",        prints);
	build_primitive( "\n",            newline);
	build_primitive( "center",        center);
	build_primitive( "verbatim",      verbatim);
*/
#endif
	build_primitive( "cls",           cls);
#ifdef ANSI_CONSOLE
	build_primitive( "home",          scr_home);
	build_primitive( "_gotoxy",       scr_gotoxy);
	build_primitive( "erase_eol",     scr_erase_eol);
	build_primitive( "normal_video",  scr_normal_video);
	build_primitive( "bold_video",    scr_bold_video);
	build_primitive( "_cls",          scr_cls);
#endif

	build_iprim( "<builds",           builds);
/*
	build_iprim( "does>",             Compile_does);
*/
/*
	build_iprim( "recurse",           recurse);
*/
	build_iprim( "[",                 left_bracket);
	build_iprim( "\'",                tick);
	build_iprim( "[']",               bracket_tick);
	build_iprim( "[compile]",         bracket_compile);
	build_iprim( "compile",           compile);
	build_iprim( "vocabulary",        Compile_vocabulary);
/*
	build_iprim( "definitions",       Compile_definitions);
*/
	build_iprim( "begin",             Compile_begin);
	build_iprim( "until",             Compile_until);
	build_iprim( "while",             Compile_while);
	build_iprim( "repeat",            Compile_repeat);
	build_iprim( ".(",                dot_paren);
	build_iprim( "\\",                backslash);
	build_iprim( "literal",           do_literal);
	build_iprim( ":",                 Compile_colon);
	build_iprim( ";",                 semi);
	build_iprim( "(",                 parens);
	build_iprim( "ascii",             ascii);
	build_iprim( "if",                Compile_if);
	build_iprim( "else",              Compile_else);
	build_iprim( "then",              Compile_then);
	build_iprim( "endif",             Compile_then);
	build_iprim( "\"",                Compile_quote);
	build_iprim( ".\"",               Compile_dot_quote);
	build_iprim( "abort\"",           Compile_abort_quote);
	build_iprim( "do",                Compile_do);
	build_iprim( "loop",              Compile_loop);
	build_iprim( "for",               Compile_for);
	build_iprim( "next",              Compile_loop);
	build_iprim( "+loop",             Compile_plus_loop);
	build_iprim( "case",              Compile_case);
	build_iprim( "endcase",           Compile_endcase);
	build_iprim( "of",                Compile_of);
	build_iprim( "endof",             Compile_endof);

	build_primitive( "fence",         fence);

	CASE_WA            = set_WA("do_case");
	OF_WA              = set_WA("do_of");
	ENDCASE_WA         = set_WA("do_endcase");
	LIT_WA             = set_WA("lit");
	ZERO_BRAN_WA       = set_WA("0bran");
	BRANCH_WA          = set_WA("branch");
	QUOTE_WA           = set_WA("quote");
	DOT_QUOTE_WA       = set_WA(".quote");
	ABORT_QUOTE_WA     = set_WA("abort_quote");
	DO_WA              = set_WA("do_do");
	DO_FOR_WA          = set_WA("do_for");
	LOOP_WA            = set_WA("do_loop");
	PLUS_LOOP_WA       = set_WA("plus_loop");
	I_WA               = set_WA("i");
	J_WA               = set_WA("j");
	TICK_WA            = set_WA("\'");
	DO_COMPILE_WA      = set_WA("do_compile");
	DO_DOES_WA         = set_WA("do_does");
	COMPILE_DO_DOES_WA = set_WA("Compile_do_does");
	DO_VOC_WA          = set_WA("do_vocabulary");
/*
	DO_DEF_WA          = set_WA("do_definitions");
*/
/*
	NAME0_WA           = set_WA("Name0");
*/

	hide("lit");

	hide("0bran");
	hide("branch");
	hide("quote");
	hide(".quote");
	hide("abort_quote");
	hide("do_do");
	hide("do_for");
	hide("do_loop");
	hide("plus_loop");
	hide("do_compile");
	hide("do_vocabulary");
	hide("do_definitions");
	hide("<outer>");
	hide("do_does");
	hide("Compile_do_does");

#ifndef VECTORED_C_IO
	printf("...Done\n");
#else
	stdout_vect("...Done\n");
#endif
}
