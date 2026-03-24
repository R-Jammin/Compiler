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
"i32"         { yylval.string=strdup(yytext); return TOK_TYPE; }
"i1"          { yylval.string=strdup(yytext); return TOK_TYPE; }


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