// Copyright (c) 2023-2025 Dan O’Malley
// This file is licensed under the MIT License. See LICENSE for details.


// sudo apt install gcc-aarch64-linux-gnu binutils-aarch64-linux-gnu
// sudo apt install qemu-user
//
// ARM Calling Conventions
// https://medium.com/@tunacici7/aarch64-procedure-call-standard-aapcs64-abi-calling-conventions-machine-registers-a2c762540278


#include "stdio.h"
#include "backend_parsetree.c"
#include <string.h>
#include "symboltable.h"

int funcCodeSymbolIndex = 0;
int secondaryIndexDoWhileLoop = 1;
int secondaryIndexLogicalNegation = 1;
int LatestRegCounter = 1;
int stackLocation = 1;
int regW16[1] = {0}; // counter of how many times it was used
int regW17[1] = {0}; // counter of how many times it was used

char * allocateNewRegister() 
{
    if (regW16[0] <= regW17[0])
    {
        regW16[0] = LatestRegCounter;
        LatestRegCounter++;
        return (char*)"w16";
    }
    else if (regW16[0] >= regW17[0])
    {
        regW17[0]= LatestRegCounter;
        LatestRegCounter++;
        return (char*)"w17";
    }

    return (char*)"NULL REGISTER RETURNED";
}

char * lastRegisterAllocated()
{
    if (regW16[0] <= regW17[0])
    {
        return (char*)"w17";
    }
    else if (regW16[0] >= regW17[0])
    {
        return (char*)"w16";
    }

    return (char*)"NULL REGISTER RETURNED";
}

char * secondToLastRegisterAllocated() 
{
    if (regW16[0] <= regW17[0])
    {
        return (char*)"w16";
    }
    else if (regW16[0] >= regW17[0])
    {
        return (char*)"w17";
    }

    return (char*)"NULL REGISTER RETURNED";
}

void printRegisterAllocation()
{
    printf("\n*********** -> regW16[%d]\n", regW16[0]);
    printf("*********** -> regW17[%d]\n\n", regW17[0]);
}


void funcCode(FILE *prog, ParseTree *parseTree, struct SymbolTable * symbolTable) {

    if (parseTree->type == INT) {
        printf("backend_aarch64.c constantValue: %d\n", parseTree->constantValue);
        fprintf(prog, "    mov w0, %d\n", parseTree->constantValue);

    }  
    else if (parseTree->type == STRING) {
        // TO DO

    }  
    else if (parseTree->type == RELOAD) {
        // TO DO

    } 
    else if (parseTree->type == DECL) {
        // TO DO

    } 
    else if (parseTree->type == INCREMENT) {
        // TO DO

    } 

    else if (parseTree->type == ARGDECL) {
        // TO DO

    } 
    else if (parseTree->type == BINOP) {
        BinOpExpr *binOpExpr = parseTree->binExpr;

        if (binOpExpr->BinOpType == ADDITION) {

            fprintf(prog, "    add %s, %s, %s\n", secondToLastRegisterAllocated(), secondToLastRegisterAllocated(), lastRegisterAllocated() );
            fprintf(prog, "    str %s, [sp, %d]\n", secondToLastRegisterAllocated(), stackLocation *4 );
            fprintf(prog, "    ldr %s, [sp, %d]\n\n", secondToLastRegisterAllocated(), stackLocation *4 );
            fprintf(prog, "    mov %s, %s \n\n", lastRegisterAllocated(), secondToLastRegisterAllocated() );
                       
        }
        else if (binOpExpr->BinOpType == SUBTRACTION) {
            
            // TO DO
        }
        else if (binOpExpr->BinOpType == MULTIPLICATION) {
            
            // TO DO
        }

        else if (binOpExpr->BinOpType == WHILELOOP) {
            
            // TO DO

        }

        else if (binOpExpr->BinOpType == EXITWHILELOOP) {
            
            // TO DO

        }

    } else if (parseTree->type == UNOP) {
        UnOpExpr *unOpExpr = parseTree->unExpr;

        if (unOpExpr->UnOpType == LOGICALNEGATION) {
            
            // TO DO

        }

        else if (unOpExpr->UnOpType == DECLASSIGN) {
            
            // TO DO
        }

        else if (unOpExpr->UnOpType == ASSIGN) {
            
            // TO DO
        }

        else if (unOpExpr->UnOpType == STORETOSTACK) {
            
            // TO DO

        }

        else if (unOpExpr->UnOpType == RET) {
            
            funcCode(prog, unOpExpr->rOperand, symbolTable);

            // Need to write the return here and the write the beginning of the file
            // This is needed since I have to add to sp upon return
            // Incorrect sp add values crash the binary.
            // x86 doesn't need this

            fprintf(prog, "    mov w0, %s\n\n", lastRegisterAllocated() );
            fprintf(prog, "    add sp, sp, #%d\n\n", (stackLocation *4));
            fprintf(prog, "    ret\n");
        
            // Reset to beginning of the file and write the header
            fseek(prog, 0, SEEK_SET);
            fprintf(prog, ".section .data\n\n");
        
            fprintf(prog, ".section .text\n\n");

            for(int x = 0; x < symbolTable->totalEntries; ++x)
            {
                if(!strcmp(symbolTable[x].entryType, (char *)"FUNC"))
                {
                    fprintf(prog, "    .global %sFinal\n\n", symbolTable[x].symbolName); 
                    fprintf(prog, "%sFinal:\n", symbolTable[x].symbolName); 
                }
            }
    
            //prepare a dynamic stack frame
            fprintf(prog, "    sub sp, sp, #%d\n\n", (stackLocation *4));
        }


    }
}