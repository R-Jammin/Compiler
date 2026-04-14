// Copyright (c) 2023-2025 Dan O’Malley
// This file is licensed under the MIT License. See LICENSE for details.


// https://refspecs.linuxbase.org/elf/x86_64-abi-0.99.pdf
// sticking to Intel syntax and will use NASM to assemble

#include "stdio.h"
#include "backend_parsetree.c"
#include <string.h>
#include "symboltable.h"

int funcCodeSymbolIndex = 0;
int LatestRegCounter = 1;
int secondaryIndexLogicalNegation = 1;
int stackLocation = 1;
int regEBX[1] = {0}; // counter of how many times it was used
int regECX[1] = {0}; // counter of how many times it was used

char * allocateNewRegister() 
{
    if (regEBX[0] <= regECX[0])
    {
        regEBX[0] = LatestRegCounter;
        LatestRegCounter++;
        return (char*)"ebx";
    }
    else if (regEBX[0] >= regECX[0])
    {
        regECX[0]= LatestRegCounter;
        LatestRegCounter++;
        return (char*)"ecx";
    }

    return (char*)"NULL REGISTER RETURNED";
}

char * lastRegisterAllocated()
{
    if (regEBX[0] <= regECX[0])
    {
        return (char*)"ecx";
    }
    else if (regEBX[0] >= regECX[0])
    {
        return (char*)"ebx";
    }

    return (char*)"NULL REGISTER RETURNED";
}

char * secondToLastRegisterAllocated() 
{
    if (regEBX[0] <= regECX[0])
    {
        return (char*)"ebx";
    }
    else if (regEBX[0] >= regECX[0])
    {
        return (char*)"ecx";
    }

    return (char*)"NULL REGISTER RETURNED";
}

void printRegisterAllocation()
{
    printf("\n*********** -> regEBX[%d]\n", regEBX[0]);
    printf("*********** -> regECX[%d]\n\n", regECX[0]);
}

void dataSectionHeader(FILE *prog) {
    fprintf(prog, "section .data\n\n");
}

void textSectionHeader(FILE *prog) {
    fprintf(prog, "section .text\n\n");
}

void functionHeader(FILE *prog, char * funcName) {
    fprintf(prog, "    global %sFinal:\n\n", funcName); 
    fprintf(prog, "%sFinal:\n", funcName); 
}


void funcPrologue(FILE *prog, struct SymbolTable * symbolTable) {
    
    // Find the largest stack location in the symbol table
    int stackLocation = 0;

    for(int x = 0; x < symbolTable->totalEntries; ++x)
    {
        stackLocation = symbolTable[x].symbolLocation;
    }
    
    
    //prepare a dynamic stack frame
    fprintf(prog, "    push rbp\n");
    fprintf(prog, "    mov rbp, rsp\n");
    fprintf(prog, "    sub rsp, %d\n\n", (stackLocation *4) - 4);
}

void returnValue(FILE *prog) {
    fprintf(prog, "    mov eax, edi\n");
    fprintf(prog, "    leave\n"); // This moves base pointer to stack pointer for the return
    fprintf(prog, "    ret\n");
}

void funcCode(FILE *prog, ParseTree *parseTree, struct SymbolTable * symbolTable) {

    if (parseTree->type == INT) {
        printf("backend_x64.c constantValue: %d\n", parseTree->constantValue);
        fprintf(prog, "    mov edi, %d\n", parseTree->constantValue);

    }  
    else if (parseTree->type == STRING) {
        printf("backend_x64.c stringValue: %s\n", parseTree->string);

        for(int x = 0; x < symbolTable->totalEntries; ++x)
        {
            if(!strcmp(symbolTable[x].symbolName, parseTree->string))
            {
                printf("backend_x64.c found variable in symbol table: %d\n", x);
                fprintf(prog, "    mov edi, [rbp-%d]\n", symbolTable[x].symbolLocation);
            }
        }

    }  
    else if (parseTree->type == RELOAD) {
        printf("backend_x64.c stringValue: %s\n", parseTree->string);

        for(int x = 0; x < symbolTable->totalEntries; ++x)
        {
            if(!strcmp(symbolTable[x].symbolName, parseTree->string))
            {
                // TO DO
                printf("backend_x64.c reloading variable from symbol table: %d\n", x);

                fprintf(prog, "    mov %s, [rbp-%d]\n", allocateNewRegister(), symbolTable[x].symbolLocation);

                break;
            }
        }

    } 
    else if (parseTree->type == BINOP) {
        BinOpExpr *binOpExpr = parseTree->binExpr;

        if (binOpExpr->BinOpType == ADDITION) {

            fprintf(prog, "    add %s, %s\n", secondToLastRegisterAllocated(), lastRegisterAllocated() );
            fprintf(prog, "    mov [rbp-%d], %s\n", stackLocation *4 + 4, secondToLastRegisterAllocated());
            fprintf(prog, "    mov edi, %s\n\n", secondToLastRegisterAllocated() );
            fprintf(prog, "    mov %s, %s \n\n", lastRegisterAllocated(), secondToLastRegisterAllocated() );
                       
        }
        else if (binOpExpr->BinOpType == SUBTRACTION) {
            
            // TO DO
            fprintf(prog, "    sub %s, %s\n", secondToLastRegisterAllocated(), lastRegisterAllocated());
            fprintf(prog, "    mov [rbp-%d], %s\n",stackLocation * 4 + 4,secondToLastRegisterAllocated());
            fprintf(prog, "    mov edi, %s\n\n",secondToLastRegisterAllocated());
            fprintf(prog, "    mov %s, %s\n\n", lastRegisterAllocated(), secondToLastRegisterAllocated());
            stackLocation++;
            
        }
        else if (binOpExpr->BinOpType == MULTIPLICATION) {
            
            // TO DO
            fprintf(prog, "    imul %s, %s\n", secondToLastRegisterAllocated(),lastRegisterAllocated());
            fprintf(prog, "    mov [rbp-%d], %s\n", stackLocation * 4 + 4, secondToLastRegisterAllocated());
            fprintf(prog, "    mov edi, %s\n\n", secondToLastRegisterAllocated());
            fprintf(prog, "    mov %s, %s\n\n", lastRegisterAllocated(), secondToLastRegisterAllocated());
            stackLocation++;
            
        }

    } else if (parseTree->type == UNOP) {
        UnOpExpr *unOpExpr = parseTree->unExpr;

        if (unOpExpr->UnOpType == LOGICALNEGATION) {
            
            // TO DO
            funcCode(prog, unOpExpr->rOperand, symbolTable);

            fprintf(prog, "    cmp edi, 0\n");
            fprintf(prog, "    sete al\n");
            fprintf(prog, "    movzx edi, al\n");
            fprintf(prog, "    mov [rbp-%d], edi\n\n", stackLocation * 4 + 4);
            stackLocation++;

        }

        else if (unOpExpr->UnOpType == DECLASSIGN) {
            
            // TO DO
            if (unOpExpr->rOperand->type == STRING) {
        for (int x = 0; x < symbolTable->totalEntries; ++x)
        {
            if (!strcmp(symbolTable[x].symbolName, unOpExpr->rOperand->string))
            {
                char *reg = allocateNewRegister();
                fprintf(prog, "    mov %s, [rbp-%d]\n", reg, symbolTable[x].symbolLocation);
                fprintf(prog, "    mov edi, %s\n", reg);
                break;
            }
        }
    }
    else {
        funcCode(prog, unOpExpr->rOperand, symbolTable);
    }

    while (funcCodeSymbolIndex < symbolTable->totalEntries &&
           strcmp(symbolTable[funcCodeSymbolIndex].entryType, "VAR"))
    {
        funcCodeSymbolIndex++;
    }

    if (funcCodeSymbolIndex < symbolTable->totalEntries) {
        symbolTable[funcCodeSymbolIndex].symbolLocation = stackLocation * 4 + 4;
        fprintf(prog, "    mov [rbp-%d], edi\n\n",
                symbolTable[funcCodeSymbolIndex].symbolLocation);
        funcCodeSymbolIndex++;
        stackLocation++;
    }
        }

        else if (unOpExpr->UnOpType == STORETOSTACK) {
            
            // TO DO
         funcCode(prog, unOpExpr->rOperand, symbolTable);

    if (unOpExpr->rOperand->type == RELOAD || unOpExpr->rOperand->type == STRING) {
        fprintf(prog, "    mov [rbp-%d], %s\n\n",
                stackLocation * 4 + 4,
                lastRegisterAllocated());
    }
    else {
        fprintf(prog, "    mov [rbp-%d], edi\n\n", stackLocation * 4 + 4);
    }

    stackLocation++;
        

   
        }

        else if (unOpExpr->UnOpType == RET) {
            
            // TO DO
             funcCode(prog, unOpExpr->rOperand, symbolTable);

    if (unOpExpr->rOperand->type == RELOAD || unOpExpr->rOperand->type == STRING) {
        fprintf(prog, "    mov edi, %s\n", lastRegisterAllocated());
    }

    returnValue(prog);
        }


    }
}