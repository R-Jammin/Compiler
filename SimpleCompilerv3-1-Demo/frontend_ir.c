// Copyright (c) 2023-2025 Dan O’Malley
// This file is licensed under the MIT License. See LICENSE for details.


#include "stdio.h"
#include "frontend_parsetree.c"
#include <string.h>
#include "symboltable.h"

int funcCodeSymbolIndex = 0;
int ssaIndex = 1;


void returnValue(FILE *prog) {
    fseek(prog, 0, SEEK_END); // go to the end of the file
    fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4 ;loading return value \n", ssaIndex, ssaIndex-1);
    fprintf(prog, "    ret i32 %%%d\n", ssaIndex); // return value
    fprintf(prog, "\n}"); //this is the end parentheses from the main function
}

void funcCode(FILE *prog, ParseTree *parseTree, struct SymbolTable * symbolTable) {

    if (parseTree->type == FUNC) {
        printf("frontend_ir.c funcValue: %s\n", parseTree->string);

        for(int x = 0; x < symbolTable->totalEntries; ++x)
        {
            if(!strcmp(symbolTable[x].symbolName, parseTree->string))
            {
                printf("frontend_ir.c found variable in symbol table: %d\n", x);
                fseek(prog, 0, SEEK_SET); //reset to beginning of file.
                fprintf(prog, "define i32 @%sFinal() {\n", symbolTable[x].symbolName);  // **** Need to remove the "Final" for assignment #2
            }
        }

    } 
    
    else if (parseTree->type == INT) {
        printf("frontend_ir.c constantValue: %d\n", parseTree->constantValue);
        fprintf(prog, "    %%%d = alloca i32, align 4\n", ssaIndex);
        fprintf(prog, "    store i32 %d, i32* %%%d, align 4\n\n", parseTree->constantValue, ssaIndex);
        ssaIndex++;

    }  
    else if (parseTree->type == STRING) {
        printf("frontend_ir.c stringValue: %s\n", parseTree->string);

        for(int x = 0; x < symbolTable->totalEntries; ++x)
        {
            if(!strcmp(symbolTable[x].symbolName, parseTree->string))
            {
                // TO DO
            printf("frontend_ir.c found variable in symbol table: %d\n", x);

            fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4\n", ssaIndex, symbolTable[x].symbolLocation);
            ssaIndex++;
            break;
            }
        }

    }  
    else if (parseTree->type == BINOP) {
        BinOpExpr *binOpExpr = parseTree->binExpr;

        if (binOpExpr->BinOpType == ADDITION) {
                       
            funcCode(prog, binOpExpr->rOperand, symbolTable);
            funcCode(prog, binOpExpr->lOperand, symbolTable);
      
            fprintf(prog, "    ; loading values for binary operation\n\n");
            fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4\n\n", ssaIndex, ssaIndex-2);
            ssaIndex++;
            fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4\n\n", ssaIndex, ssaIndex-2);
            ssaIndex++;


            fprintf(prog, "    %%%d = add i32 %%%d, %%%d\n", ssaIndex, ssaIndex-2, ssaIndex-1);
            ssaIndex++;
            fprintf(prog, "    %%%d = alloca i32, align 4\n", ssaIndex);
            fprintf(prog, "    store i32 %%%d, i32* %%%d, align 4 ; storing the answer\n\n", ssaIndex-1, ssaIndex);
            ssaIndex++;
        }
        else if (binOpExpr->BinOpType == SUBTRACTION) {
            
            // TO DO
            funcCode(prog, binOpExpr->rOperand, symbolTable);
            funcCode(prog, binOpExpr->lOperand, symbolTable);
      
            fprintf(prog, "    ; loading values for binary operation\n\n");
            fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4\n\n", ssaIndex, ssaIndex-2);
            ssaIndex++;
            fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4\n\n", ssaIndex, ssaIndex-2);
            ssaIndex++;


            fprintf(prog, "    %%%d = subtract i32 %%%d, %%%d\n", ssaIndex, ssaIndex-2, ssaIndex-1);
            ssaIndex++;
            fprintf(prog, "    %%%d = alloca i32, align 4\n", ssaIndex);
            fprintf(prog, "    store i32 %%%d, i32* %%%d, align 4 ; storing the answer\n\n", ssaIndex-1, ssaIndex);
            ssaIndex++;
        
        }
        else if (binOpExpr->BinOpType == MULTIPLICATION) {
            
            // TO DO
            funcCode(prog, binOpExpr->rOperand, symbolTable);
            funcCode(prog, binOpExpr->lOperand, symbolTable);
      
            fprintf(prog, "    ; loading values for binary operation\n\n");
            fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4\n\n", ssaIndex, ssaIndex-2);
            ssaIndex++;
            fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4\n\n", ssaIndex, ssaIndex-2);
            ssaIndex++;


            fprintf(prog, "    %%%d = multiply i32 %%%d, %%%d\n", ssaIndex, ssaIndex-2, ssaIndex-1);
            ssaIndex++;
            fprintf(prog, "    %%%d = alloca i32, align 4\n", ssaIndex);
            fprintf(prog, "    store i32 %%%d, i32* %%%d, align 4 ; storing the answer\n\n", ssaIndex-1, ssaIndex);
            ssaIndex++;
        
        }

    } else if (parseTree->type == UNOP) {
        UnOpExpr *unOpExpr = parseTree->unExpr;

        if (unOpExpr->UnOpType == LOGICALNEGATION) {
            
            // TO DO
            funcCode(prog, unOpExpr->rOperand, symbolTable);

            fprintf(prog, "    ; logical negation\n");

            fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4\n", ssaIndex, ssaIndex-1);
            ssaIndex++;

            fprintf(prog, "    %%%d = logicalnegation i32 %%%d, 0\n", ssaIndex, ssaIndex-1);
            ssaIndex++;

            fprintf(prog, "    %%%d = alloca i32, align 4\n", ssaIndex);
            fprintf(prog, "    store i32 %%%d, i32* %%%d, align 4\n\n", ssaIndex-1, ssaIndex);
            ssaIndex++;
        }

        else if (unOpExpr->UnOpType == DECLASSIGN) {
            
            // TO DO
               funcCode(prog, unOpExpr->rOperand, symbolTable);

            for (int x = 0; x < symbolTable->totalEntries; ++x)
            {
             if (!strcmp(symbolTable[x].symbolName, unOpExpr->rOperand->string))
               {
                fprintf(prog, "    %%%d = alloca i32, align 4\n", symbolTable[x].symbolLocation);

                fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4\n",ssaIndex, ssaIndex - 1);

                fprintf(prog, "    store i32 %%%d, i32* %%%d, align 4\n\n", ssaIndex, symbolTable[x].symbolLocation);

                 ssaIndex++;
                break;
            }
        }
    }
 
        }
    }
