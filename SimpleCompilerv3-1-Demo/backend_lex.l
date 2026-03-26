/* Copyright (c) 2023-2025 Dan O’Malley */
/* This file is licensed under the MIT License. See LICENSE for details. */

%{

#include <stdio.h>
#include "y.tab.h"
%}

%%
";"[^\n]*     { /* consume ;-comment */ }
[ \t\n]+      { /* consume whitespace characters */ }


"define"      { return TOK_DEFINE; }
"alloca"                  { return TOK_ALLOCA; }
"load"                    { return TOK_LOAD; }
"store"                   { return TOK_STORE; }
"ret"                     { return TOK_RETURN; }
"add"                     { return TOK_ADD; }
"sub"                     { return TOK_SUBTRACT; }
"mul"                     { return TOK_MULTIPLY; }
"icmp"                    { return TOK_CMP; }
"zext"                    { return TOK_ZERO_EXTEND; }
"to"                      { return TOK_TO; }
"align"                   { return TOK_ALIGN; }
"getelementptr"           { return TOK_GETELEMENTPTR; }
"i32"         { yylval.string=strdup(yytext); return TOK_TYPE; }
"i1"          { yylval.string=strdup(yytext); return TOK_TYPE; }

"@"                       { return TOK_GLOBAL; }
"%"                       { return TOK_SSAINDEX; }
","                       { return TOK_SEPARATOR; }
"="                       { return TOK_EQUAL; }
"("			  { return TOK_LPAREN; }
")"			  { return TOK_RPAREN; }
"{"			  { return TOK_LBRACE; }
"}"			  { return TOK_RBRACE; }
[0-9]+        { yylval.number=atoi(yytext); return TOK_UINT; }
[a-zA-Z_]+	  { yylval.string=strdup(yytext); return TOK_IDENTIFIER; }


%%

int yywrap() {
	return 1;
}