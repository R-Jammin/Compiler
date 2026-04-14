%{
// Copyright (c) 2023-2025 Dan O’Malley
// This file is licensed under the MIT License. See LICENSE for details.

#include "stdio.h"
#include <stdlib.h>
#include <string.h>
#include "backend_parsetree.c"
#include "backend_x64.c"
#include "parserstack.h"
#include "symboltable.h"

#define MAXSYMBOLS 100

int yylex();
int yyparse();

void yyerror(const char *str) {
    FILE *backend_parsing_error = fopen("./logs/backend_parsing_error.log", "wb");
    fprintf(backend_parsing_error, "%s\n", str);
}

extern FILE *yyin;
ParserStack *parserStack;
ParserStack *parserStackReversed;
ParseTree *parseTree;
struct SymbolTable symbolTable[MAXSYMBOLS];
int location = 0;

void insertSymbol(struct SymbolTable *symbolTable, char *entryType, char *symbolType, char *symbolName, int symbolLocation, int size)
{
    int entryNumber = symbolTable->totalEntries;

    strcpy(symbolTable[entryNumber].entryType, entryType);
    strcpy(symbolTable[entryNumber].symbolType, symbolType);
    strcpy(symbolTable[entryNumber].symbolName, symbolName);
    symbolTable[entryNumber].symbolLocation = symbolLocation;
    symbolTable[entryNumber].size = size;

    symbolTable->totalEntries++;
}

void printSymbolTable(struct SymbolTable *symbolTable)
{
    printf("\n\n***** Symbol Table Dump *****\n");

    for (int entryNumber = 0; entryNumber < symbolTable->totalEntries; entryNumber++)
    {
        printf("--> ID: %d ENTRY TYPE: %-5s NAME: %-15s TYPE: %-8s LOCATION: %-3d SIZE: %d\n",
            entryNumber,
            symbolTable[entryNumber].entryType,
            symbolTable[entryNumber].symbolName,
            symbolTable[entryNumber].symbolType,
            symbolTable[entryNumber].symbolLocation,
            symbolTable[entryNumber].size);
    }

    printf("\n\n");
}

int main(int argc, char *argv[])
{
    #if YYDEBUG == 1
    extern int yydebug;
    yydebug = 1;
    #endif

    yyin = fopen(argv[1], "r");

    parserStack = parserStackCreate();
    parserStackReversed = parserStackCreate();

    yyparse();

    FILE *prog = fopen("./build/prog.s", "wb");

    dataSectionHeader(prog);
    textSectionHeader(prog);

    for (int x = 0; x < symbolTable->totalEntries; ++x)
    {
        if (!strcmp(symbolTable[x].entryType, (char *)"FUNC"))
        {
            functionHeader(prog, symbolTable[x].symbolName);
            funcPrologue(prog, symbolTable);
        }
    }

    while (parserStack->depth > 0)
    {
        printf("parserstack depth: %d\n", parserStack->depth);
        parseTree = parserStackPop(parserStack);
        parserStackPush(parserStackReversed, parseTree);
    }

    while (parserStackReversed->depth > 0)
    {
        printf("parserstackReversed depth: %d\n", parserStackReversed->depth);
        parseTree = parserStackPop(parserStackReversed);
        funcCode(prog, parseTree, symbolTable);
    }

    printSymbolTable(symbolTable);
    fclose(prog);
}
%}

%token TOK_GETELEMENTPTR
%token TOK_DEFINE TOK_ALLOCA TOK_TYPE_PTR
%token TOK_SSAINDEX TOK_SEPARATOR TOK_LOAD
%token TOK_RETURN TOK_STORE TOK_GLOBAL
%token TOK_ADD TOK_SUBTRACT TOK_MULTIPLY
%token TOK_NOT_EQUAL TOK_XOR
%token TOK_ZERO_EXTEND TOK_EQUAL TOK_TRUE
%token TOK_FALSE TOK_ALIGN TOK_LPAREN TOK_RPAREN
%token TOK_LBRACE TOK_RBRACE TOK_CMP TOK_TO

%union
{
    int number;
    char *string;
}

%token <number> TOK_UINT
%token <string> TOK_TYPE
%token <string> TOK_IDENTIFIER
%token <string> TOK_IDENTIFIER_RELOAD

%%

program:
    function
    ;

function:
    function TOK_DEFINE TOK_TYPE TOK_GLOBAL TOK_IDENTIFIER TOK_LPAREN TOK_RPAREN TOK_LBRACE stmt_list TOK_RBRACE
    {
        insertSymbol(symbolTable, (char *)"FUNC", (char *)$3, (char *)$5, 0, 0);
        parserStackPush(parserStack, funcType($5));
    }
    |
    ;

stmt:
    TOK_SSAINDEX TOK_UINT TOK_EQUAL TOK_ALLOCA TOK_TYPE TOK_SEPARATOR TOK_ALIGN TOK_UINT
    {
        char *ssaName = malloc(20);
        sprintf(ssaName, "%d", $2);

        location++;
        insertSymbol(symbolTable, (char *)"VAR", (char *)$5, ssaName, location * 4 + 4, 4);
    }
    |
    TOK_STORE TOK_TYPE TOK_UINT TOK_SEPARATOR TOK_TYPE_PTR TOK_SSAINDEX TOK_UINT TOK_SEPARATOR TOK_ALIGN TOK_UINT
    {
        ParseTree *rOperand = intType($3);
        parserStackPush(parserStack, storeToStack(rOperand));
    }
    |
    TOK_STORE TOK_TYPE TOK_SSAINDEX TOK_UINT TOK_SEPARATOR TOK_TYPE_PTR TOK_SSAINDEX TOK_UINT TOK_SEPARATOR TOK_ALIGN TOK_UINT
    {
        ParseTree *rOperand = parserStackPop(parserStack);
        parserStackPush(parserStack, storeToStack(rOperand));
    }
    |
    TOK_SSAINDEX TOK_UINT TOK_EQUAL TOK_GETELEMENTPTR TOK_TYPE TOK_SEPARATOR TOK_TYPE_PTR TOK_SSAINDEX TOK_UINT TOK_SEPARATOR TOK_TYPE TOK_UINT
    {
        char *ssaName = malloc(20);
        sprintf(ssaName, "%d", $9);
        parserStackPush(parserStack, stringType(ssaName));
    }
    |
    TOK_SSAINDEX TOK_UINT TOK_EQUAL TOK_LOAD TOK_TYPE TOK_SEPARATOR TOK_TYPE_PTR TOK_SSAINDEX TOK_UINT TOK_SEPARATOR TOK_ALIGN TOK_UINT
    {
        char *ssaName = malloc(20);
        sprintf(ssaName, "%d", $9);
        parserStackPush(parserStack, reload(ssaName));
    }
    |
    TOK_SSAINDEX TOK_UINT TOK_EQUAL TOK_ADD TOK_TYPE TOK_SSAINDEX TOK_UINT TOK_SEPARATOR TOK_SSAINDEX TOK_UINT
    {
        parserStackPush(parserStack, add());
    }
    |
    TOK_SSAINDEX TOK_UINT TOK_EQUAL TOK_SUBTRACT TOK_TYPE TOK_SSAINDEX TOK_UINT TOK_SEPARATOR TOK_SSAINDEX TOK_UINT
    {
        parserStackPush(parserStack, subtract());
    }
    |
    TOK_SSAINDEX TOK_UINT TOK_EQUAL TOK_MULTIPLY TOK_TYPE TOK_SSAINDEX TOK_UINT TOK_SEPARATOR TOK_SSAINDEX TOK_UINT
    {
        parserStackPush(parserStack, multiply());
    }
    |
    TOK_SSAINDEX TOK_UINT TOK_EQUAL TOK_CMP TOK_IDENTIFIER TOK_TYPE TOK_SSAINDEX TOK_UINT TOK_SEPARATOR TOK_UINT
    {
        char *ssaName = malloc(20);
        sprintf(ssaName, "%d", $8);
        ParseTree *rOperand = reload(ssaName);
        parserStackPush(parserStack, logicalNegation(rOperand));
    }
    |
    TOK_SSAINDEX TOK_UINT TOK_EQUAL TOK_ZERO_EXTEND TOK_TYPE TOK_SSAINDEX TOK_UINT TOK_TO TOK_TYPE
    {
        char *ssaName = malloc(20);
        sprintf(ssaName, "%d", $7);
        ParseTree *rOperand = reload(ssaName);
        parserStackPush(parserStack, declarationWithAssign(rOperand));
    }
    |
    TOK_RETURN TOK_TYPE TOK_SSAINDEX TOK_UINT
    {
        ParseTree *rOperand = parserStackPop(parserStack);
        parserStackPush(parserStack, ret(rOperand));
    }
    |
    TOK_RETURN TOK_TYPE TOK_UINT
    {
        ParseTree *rOperand = intType($3);
        parserStackPush(parserStack, ret(rOperand));
    }
    ;

stmt_list:
    stmt
    |
    stmt_list stmt
    ;

expr:
    value
    ;

value:
    TOK_IDENTIFIER
    {
        parserStackPush(parserStack, stringType($1));
    }
    |
    number
    ;

number:
    TOK_UINT
    {
        printf("*******parser.y uint push value: %d\n", $1);
        parserStackPush(parserStack, intType($1));
    }
    ;