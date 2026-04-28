// Copyright (c) 2023-2025 Dan O’Malley
// This file is licensed under the MIT License. See LICENSE for details.

#include "stdio.h"
#include "frontend_parsetree.c"
#include <string.h>
#include "symboltable.h"

int funcCodeSymbolIndex = 0;
int ssaIndex = 1;
int labelIndex = 0;

void returnValue(FILE *prog)
{
    fseek(prog, 0, SEEK_END); // go to the end of the file
    fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4 ;loading return value \n", ssaIndex, ssaIndex - 1);
    fprintf(prog, "    ret i32 %%%d\n", ssaIndex); // return value
    fprintf(prog, "\n}");                          // this is the end parentheses from the main function
}

void funcCode(FILE *prog, ParseTree *parseTree, struct SymbolTable *symbolTable)
{

    if (parseTree->type == FUNC)
    {
        printf("frontend_ir.c funcValue: %s\n", parseTree->string);

        for (int x = 0; x < symbolTable->totalEntries; ++x)
        {
            if (!strcmp(symbolTable[x].symbolName, parseTree->string))
            {
                printf("frontend_ir.c found variable in symbol table: %d\n", x);
                fseek(prog, 0, SEEK_SET);                                         // reset to beginning of file.
                fprintf(prog, "define i32 @%s() {\n", symbolTable[x].symbolName); // **** Need to remove the "Final" for assignment #2
            }
        }
    }

    else if (parseTree->type == INT)
    {
        printf("frontend_ir.c constantValue: %d\n", parseTree->constantValue);
        fprintf(prog, "    %%%d = alloca i32, align 4\n", ssaIndex);
        fprintf(prog, "    store i32 %d, i32* %%%d, align 4\n\n", parseTree->constantValue, ssaIndex);
        ssaIndex++;
    }
    else if (parseTree->type == STRING)
    {
        printf("frontend_ir.c stringValue: %s\n", parseTree->string);

        for (int x = 0; x < symbolTable->totalEntries; ++x)
        {
            if (!strcmp(symbolTable[x].symbolName, parseTree->string))
            {
                printf("frontend_ir.c found variable in symbol table: %d\n", x);

                fprintf(prog, "    %%%d = getelementptr i32, i32* %%%d, i32 0\n\n",
                        ssaIndex, symbolTable[x].symbolLocation);

                ssaIndex++;
                break;
            }
            else if (x == symbolTable->totalEntries - 1)
            {
                printf("frontend_ir.c not found variable in symbol table: %s\n",
                       parseTree->string);
            }
        }
    }
    else if (parseTree->type == BINOP)
    {
        BinOpExpr *binOpExpr = parseTree->binExpr;

        if (binOpExpr->BinOpType == ADDITION)
        {

            funcCode(prog, binOpExpr->rOperand, symbolTable);
            funcCode(prog, binOpExpr->lOperand, symbolTable);

            fprintf(prog, "    ; loading values for binary operation\n\n");
            fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4\n\n", ssaIndex, ssaIndex - 2);
            ssaIndex++;
            fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4\n\n", ssaIndex, ssaIndex - 2);
            ssaIndex++;

            fprintf(prog, "    %%%d = add i32 %%%d, %%%d\n", ssaIndex, ssaIndex - 2, ssaIndex - 1);
            ssaIndex++;
            fprintf(prog, "    %%%d = alloca i32, align 4\n", ssaIndex);
            fprintf(prog, "    store i32 %%%d, i32* %%%d, align 4 ; storing the answer\n\n", ssaIndex - 1, ssaIndex);
            ssaIndex++;
        }
        else if (binOpExpr->BinOpType == SUBTRACTION)
        {

            // TO DO
            funcCode(prog, binOpExpr->rOperand, symbolTable);
            funcCode(prog, binOpExpr->lOperand, symbolTable);

            fprintf(prog, "    ; loading values for binary operation\n\n");
            fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4\n\n", ssaIndex, ssaIndex - 2);
            ssaIndex++;
            fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4\n\n", ssaIndex, ssaIndex - 2);
            ssaIndex++;

            fprintf(prog, "    %%%d = sub i32 %%%d, %%%d\n", ssaIndex, ssaIndex - 2, ssaIndex - 1);
            ssaIndex++;
            fprintf(prog, "    %%%d = alloca i32, align 4\n", ssaIndex);
            fprintf(prog, "    store i32 %%%d, i32* %%%d, align 4 ; storing the answer\n\n", ssaIndex - 1, ssaIndex);
            ssaIndex++;
        }
        else if (binOpExpr->BinOpType == MULTIPLICATION)
        {

            // TO DO
            funcCode(prog, binOpExpr->rOperand, symbolTable);
            funcCode(prog, binOpExpr->lOperand, symbolTable);

            fprintf(prog, "    ; loading values for binary operation\n\n");
            fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4\n\n", ssaIndex, ssaIndex - 2);
            ssaIndex++;
            fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4\n\n", ssaIndex, ssaIndex - 2);
            ssaIndex++;

            fprintf(prog, "    %%%d = mul i32 %%%d, %%%d\n", ssaIndex, ssaIndex - 2, ssaIndex - 1);
            ssaIndex++;
            fprintf(prog, "    %%%d = alloca i32, align 4\n", ssaIndex);
            fprintf(prog, "    store i32 %%%d, i32* %%%d, align 4 ; storing the answer\n\n", ssaIndex - 1, ssaIndex);
            ssaIndex++;
        }
        else if (binOpExpr->BinOpType == LESSTHAN)
        {
            funcCode(prog, binOpExpr->rOperand, symbolTable);
            funcCode(prog, binOpExpr->lOperand, symbolTable);

            fprintf(prog, "    ; loading values for less than comparison\n\n");

            fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4\n",
                    ssaIndex, ssaIndex - 2);
            ssaIndex++;

            fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4\n",
                    ssaIndex, ssaIndex - 2);
            ssaIndex++;

            fprintf(prog, "    %%%d = icmp slt i32 %%%d, %%%d\n",
                    ssaIndex, ssaIndex - 2, ssaIndex - 1);
            ssaIndex++;

            fprintf(prog, "    %%%d = zext i1 %%%d to i32\n",
                    ssaIndex, ssaIndex - 1);
            ssaIndex++;

            fprintf(prog, "    %%%d = alloca i32, align 4\n", ssaIndex);

            fprintf(prog, "    store i32 %%%d, i32* %%%d, align 4\n\n",
                    ssaIndex - 1, ssaIndex);

            ssaIndex++;
        }
        if (binOpExpr->BinOpType == ASSIGN)
        {
            char *variableName = binOpExpr->lOperand->string;
            int targetLocation = -1;

            for (int x = 0; x < symbolTable->totalEntries; ++x)
            {
                if (!strcmp(symbolTable[x].entryType, "VAR") &&
                    !strcmp(symbolTable[x].symbolName, variableName))
                {
                    targetLocation = symbolTable[x].symbolLocation;
                    break;
                }
            }

            if (targetLocation == -1)
            {
                printf("frontend_ir.c could not find assignment target: %s\n", variableName);
                return;
            }

            if (binOpExpr->rOperand->type == INT)
            {
                fprintf(prog, "    store i32 %d, i32* %%%d, align 4\n\n",
                        binOpExpr->rOperand->constantValue,
                        targetLocation);
            }
            else if (binOpExpr->rOperand->type == STRING)
            {
                char *sourceName = binOpExpr->rOperand->string;
                int sourceLocation = -1;

                for (int x = 0; x < symbolTable->totalEntries; ++x)
                {
                    if (!strcmp(symbolTable[x].entryType, "VAR") &&
                        !strcmp(symbolTable[x].symbolName, sourceName))
                    {
                        sourceLocation = symbolTable[x].symbolLocation;
                        break;
                    }
                }

                fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4\n",
                        ssaIndex, sourceLocation);
                fprintf(prog, "    store i32 %%%d, i32* %%%d, align 4\n\n",
                        ssaIndex, targetLocation);
                ssaIndex++;
            }
            else
            {
                funcCode(prog, binOpExpr->rOperand, symbolTable);

                fprintf(prog, "    store i32 %%%d, i32* %%%d, align 4\n\n",
                        ssaIndex - 2,
                        targetLocation);
            }

            return;
        }
    }
    else if (parseTree->type == UNOP)
    {
        UnOpExpr *unOpExpr = parseTree->unExpr;

        if (unOpExpr->UnOpType == LOGICALNEGATION)
        {

            // TO DO
            funcCode(prog, unOpExpr->rOperand, symbolTable);

            fprintf(prog, "    ; logical negation\n");

            fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4\n", ssaIndex, ssaIndex - 1);
            ssaIndex++;

            fprintf(prog, "    %%%d = icmp eq i32 %%%d, 0\n", ssaIndex, ssaIndex - 1);
            ssaIndex++;

            fprintf(prog, "    %%%d = zext i1 %%%d to i32\n", ssaIndex, ssaIndex - 1);
            ssaIndex++;

            fprintf(prog, "    %%%d = alloca i32, align 4\n", ssaIndex);
            fprintf(prog, "    store i32 %%%d, i32* %%%d, align 4\n\n", ssaIndex - 1, ssaIndex);
            ssaIndex++;
        }
        else if (unOpExpr->UnOpType == DECL)
        {

            char *variableName = unOpExpr->rOperand->string;

            for (int x = 0; x < symbolTable->totalEntries; ++x)
            {
                if (!strcmp(symbolTable[x].entryType, "VAR") &&
                    !strcmp(symbolTable[x].symbolName, variableName))
                {
                    fprintf(prog, "    %%%d = alloca i32, align 4\n\n", ssaIndex);
                    symbolTable[x].symbolLocation = ssaIndex;
                    ssaIndex++;
                    break;
                }
            }
        }
        else if (unOpExpr->UnOpType == DECLASSIGN)
        {

            funcCode(prog, unOpExpr->rOperand, symbolTable);

            while (funcCodeSymbolIndex < symbolTable->totalEntries &&
                   strcmp(symbolTable[funcCodeSymbolIndex].entryType, "VAR"))
            {
                funcCodeSymbolIndex++;
            }

            fprintf(prog, "    %%%d = alloca i32, align 4\n", ssaIndex);
            fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4\n",
                    ssaIndex + 1, ssaIndex - 1);
            fprintf(prog, "    store i32 %%%d, i32* %%%d, align 4\n\n",
                    ssaIndex + 1, ssaIndex);

            symbolTable[funcCodeSymbolIndex].symbolLocation = ssaIndex;

            ssaIndex += 2;
            funcCodeSymbolIndex++;
        }
    }
    else if (parseTree->type == DOWHILE)
    {
        int doBodyLabel = labelIndex++;
        int doEndLabel = labelIndex++;

        fprintf(prog, "    br label %%do_body_%d\n\n", doBodyLabel);

        fprintf(prog, "do_body_%d:\n", doBodyLabel);

        funcCode(prog, parseTree->doWhileExpr->body, symbolTable);

        funcCode(prog, parseTree->doWhileExpr->condition, symbolTable);

        fprintf(prog, "    %%%d = load i32, i32* %%%d, align 4\n",
                ssaIndex, ssaIndex - 1);
        ssaIndex++;

        fprintf(prog, "    %%%d = icmp ne i32 %%%d, 0\n",
                ssaIndex, ssaIndex - 1);
        ssaIndex++;

        fprintf(prog, "    br i1 %%%d, label %%do_body_%d, label %%do_end_%d\n\n",
                ssaIndex - 1, doBodyLabel, doEndLabel);

        fprintf(prog, "do_end_%d:\n", doEndLabel);
    }
    
}
