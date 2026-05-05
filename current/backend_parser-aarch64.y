%{
// Copyright (c) 2023-2025 Dan O’Malley
// This file is licensed under the MIT License. See LICENSE for details.


#include "stdio.h"
#include "backend_parsetree.c"
#include "backend_aarch64.c"
#include "parserstack.h"
#include "symboltable.h"

#define MAXSYMBOLS 100

int yylex();
int yyparse();

void yyerror(const char *str) {
    FILE *backend_parsing_error = fopen("./logs/backend_parsing_error.log", "wb");
    fprintf (backend_parsing_error, "%s\n", str);
}


extern FILE *yyin;
ParserStack *parserStack;
ParserStack *parserStackReversed;
ParseTree *parseTree;
struct SymbolTable symbolTable[MAXSYMBOLS];
int location = 0;


void insertSymbol(struct SymbolTable * symbolTable, char * entryType, char * symbolType, char * symbolName, int symbolLocation, int size) 
{
    int entryNumber = symbolTable->totalEntries;
    strcpy(symbolTable[entryNumber].entryType, entryType);
    strcpy(symbolTable[entryNumber].symbolType, symbolType);
    strcpy(symbolTable[entryNumber].symbolName, symbolName);
    symbolTable[entryNumber].symbolLocation = symbolLocation;
    symbolTable[entryNumber].size = size;

    symbolTable->totalEntries++;
}

void printSymbolTable(struct SymbolTable * symbolTable) 
{
    printf("\n\n***** Symbol Table Dump *****\n");

    for (int entryNumber = 0; entryNumber < symbolTable->totalEntries; entryNumber++) 
    {
        printf("--> ID: %d ENTRY TYPE: %-5s NAME: %-15s TYPE: %-8s LOCATION: %-3d SIZE: %d\n", entryNumber, 
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

    for (int x = 0; x < 99; x++)
    {
        fprintf(prog, "\n");   // Making room for the function header to be written later
    }

    while (parserStack->depth > 0)
    {
        printf("parserstack depth: %d\n", parserStack->depth);
        parseTree = parserStackPop(parserStack);
        parserStackPush(parserStackReversed, parseTree);

    }

    // I have to reverse the parsetree stack to get the codegen in the correct order
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

// grammar rules or productions

program: 
	function
        ;
function:
        function TOK_DEFINE TOK_TYPE TOK_GLOBAL TOK_IDENTIFIER TOK_LPAREN TOK_RPAREN TOK_LBRACE stmt_list TOK_RBRACE
        {
            insertSymbol(symbolTable, (char *)"FUNC", (char *)$3, (char*)$5, ++location, 0);
            parserStackPush(parserStack, funcType($5));
        }
        |
        ;
stmt:
        TOK_SSAINDEX TOK_UINT TOK_EQUAL TOK_ALLOCA TOK_TYPE TOK_SEPARATOR TOK_ALIGN TOK_UINT
        {
            // I am recognizing this but ignoring
            // NOTHING MORE TO DO WITH THIS RULE

            // TO DO - THERE ARE MANY RULES BELOW THIS ONE TO IMPLEMENT
            // FOR EXAMPLE, MY SOLUTION HAS 15 ADDITIONAL GRAMMAR RULES AFTER THIS ONE
            // TO PROCESS ALL OF THE TEST CASES
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
            // TO DO
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