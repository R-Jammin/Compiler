/* Copyright (c) 2023-2025 Dan O’Malley */
/* This file is licensed under the MIT License. See LICENSE for details. */

/* https://www.quut.com/c/ANSI-C-grammar-l-1999.html */

%{
#include <stdio.h>
#include "y.tab.h"
%}

%%
"//"[^\n]*    { /* consume //-comment */ }
[ \t\n]+      { /* consume whitespace characters */ }


"return"      { return TOK_RETURN; }
"int"         { yylval.string=strdup(yytext); return TOK_TYPE; }
"{"           { return TOK_LBRACE; }
"}"           { return TOK_RBRACE; }
"("           { return TOK_LPAREN; }
")"           { return TOK_RPAREN; }
"+"           { return TOK_ADD; }
"-"           { return TOK_SUB; }
"*"           { return TOK_MUL; }
"!"			  { return TOK_LOGICAL_NEGATION;}


";"           { return TOK_SEMI; }
"="		      { return TOK_EQUAL; }
[0-9]+        { yylval.number=atoi(yytext); return TOK_UINT; }
[a-zA-Z_]+	  { yylval.string=strdup(yytext); return TOK_IDENTIFIER; }

%%

int yywrap() {
	return 1;
}