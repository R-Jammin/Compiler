%{
// Copyright (c) 2023-2025 Dan O’Malley
// This file is licensed under the MIT License. See LICENSE for details.


// https://www.quut.com/c/ANSI-C-grammar-y-1999.html

#include "stdio.h"
#include "frontend_parsetree.c"
#include "frontend_ir.c"
#include "parserstack.h"
#include "symboltable.h"

#define MAXSYMBOLS 100

int yylex();
int yyparse();

void yyerror(const char *str) {
    FILE *frontend_parsing_error = fopen("./logs/frontend_parsing_error.log", "wb");
    fprintf (frontend_parsing_error, "%s\n", str);
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

    FILE *prog = fopen("./build/frontend_ir.ll", "wb");
    fprintf(prog, "                                     \n"); //spacer heading at the top for function

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

    returnValue(prog);
    fclose(prog);

}

%}

%token TOK_LBRACE TOK_RBRACE TOK_RETURN TOK_ADD
%token TOK_LPAREN TOK_RPAREN TOK_SEMI TOK_EQUAL
%token TOK_MUL TOK_SUB TOK_LOGICAL_NEGATION

%union
{
    int number;
    char *string;
}

%token <number> TOK_UINT
%token <string> TOK_TYPE 
%token <string> TOK_IDENTIFIER

%%

// grammar rules or productions

program: 
	function
        ;
function:
        function TOK_TYPE TOK_IDENTIFIER TOK_LPAREN TOK_RPAREN TOK_LBRACE stmt_list TOK_RBRACE
        {
            insertSymbol(symbolTable, (char *)"FUNC", (char *)$2, (char*)$3, ++location, 0);
            parserStackPush(parserStack, funcType($3));
        }
        |
        ;
stmt:
        TOK_SEMI
        |
        expr TOK_SEMI
        |
        TOK_RETURN expr TOK_SEMI
        |
        TOK_TYPE TOK_IDENTIFIER TOK_EQUAL expr TOK_SEMI
        {
            // TO DO
            ParseTree *rOperand = parserStackPop(parserStack);

            insertSymbol(symbolTable, (char *)"VAR", (char *)$1, (char *)$2, ++location, 4);

            parserStackPush(parserStack, declarationWithAssign(rOperand));
        }
        |
        TOK_LBRACE stmt_list TOK_RBRACE
        ;
stmt_list:
        stmt
        |
        stmt_list stmt
        ;
expr:
        value TOK_ADD expr
        {
            ParseTree *lOperand = parserStackPop(parserStack);
            ParseTree *rOperand = parserStackPop(parserStack);
            parserStackPush(parserStack, add(lOperand, rOperand));
        }
        |
        value TOK_SUB expr
        {
            // TO DO
            ParseTree *lOperand = parserStackPop(parserStack);
            ParseTree *rOperand = parserStackPop(parserStack);
            parserStackPush(parserStack, subtract(lOperand, rOperand));
        }
        |
        value TOK_MUL expr
        {
            // TO DO
            ParseTree *lOperand = parserStackPop(parserStack);
            ParseTree *rOperand = parserStackPop(parserStack);
            parserStackPush(parserStack, multiply(lOperand, rOperand));
        }
        |
        TOK_LOGICAL_NEGATION expr
        {
            // TO DO
            ParseTree *operand = parserStackPop(parserStack);
            parserStackPush(parserStack, logicalNegation(operand));
        }
        |
        value
        |
        TOK_LPAREN expr TOK_RPAREN
        ;
value:
        TOK_IDENTIFIER
        {
            // TO DO
            parserStackPush(parserStack, stringType($1));
        }
        |
        number
        ;
number:
        TOK_UINT
        {
            printf("*******frontend_parser.y uint push value: %d\n", $1);
            parserStackPush(parserStack, intType($1));
        }
        ;