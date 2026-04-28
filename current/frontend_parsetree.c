// Copyright (c) 2023-2025 Dan O’Malley
// This file is licensed under the MIT License. See LICENSE for details.

#include <stdlib.h>
#include <string.h>

#ifndef COMPILER_PARSETREE
#define COMPILER_PARSETREE

typedef enum
{
    FUNC,
    INT,
    STRING,
    BINOP,
    UNOP, 
    DOWHILE
} ParseTreeType;
typedef enum
{
    ADDITION,
    SUBTRACTION,
    MULTIPLICATION,
    ASSIGN, 
    LESSTHAN
} ParseTreeBinOp;
typedef enum
{
    LOGICALNEGATION,
    DECLASSIGN,
    DECL
} ParseTreeUnOp;


typedef struct parseTree ParseTree;

typedef struct BinOpExpr
{
    ParseTreeBinOp BinOpType;
    ParseTree *lOperand;
    ParseTree *rOperand;
} BinOpExpr;

typedef struct UnOpExpr
{
    ParseTreeUnOp UnOpType;
    ParseTree *rOperand;
} UnOpExpr;
typedef struct DoWhileExpr
{
    ParseTree *body;
    ParseTree *condition;
} DoWhileExpr;

struct parseTree
{
    ParseTreeType type;
    union
    {
        int constantValue;
        char *string;
        BinOpExpr *binExpr;
        UnOpExpr *unExpr;
        DoWhileExpr *doWhileExpr;
    };
};

ParseTree *funcType(char *string)
{
    ParseTree *parseTree = malloc(sizeof(parseTree));
    parseTree->type = FUNC;
    parseTree->string = string;
    return parseTree;
}

ParseTree *intType(int constantValue)
{
    ParseTree *parseTree = malloc(sizeof(parseTree));
    parseTree->type = INT;
    parseTree->constantValue = constantValue;
    return parseTree;
}

ParseTree *stringType(char *string)
{

    // TO DO (and remove the "return 0;")
    ParseTree *parseTree = malloc(sizeof(parseTree));
    parseTree->type = STRING;
    parseTree->string = string;
    return parseTree;
}

ParseTree *add(ParseTree *lOperand, ParseTree *rOperand)
{
    ParseTree *parseTree = malloc(sizeof(parseTree));
    BinOpExpr *binOpExpr = malloc(sizeof(binOpExpr));
    binOpExpr->BinOpType = ADDITION;
    binOpExpr->lOperand = lOperand;
    binOpExpr->rOperand = rOperand;
    parseTree->type = BINOP;
    parseTree->binExpr = binOpExpr;
    return parseTree;
}

ParseTree *subtract(ParseTree *lint, ParseTree *rint)
{

    // TO DO (and remove the "return 0;")
    ParseTree *parseTree = malloc(sizeof(parseTree));
    BinOpExpr *binOpExpr = malloc(sizeof(binOpExpr));
    binOpExpr->BinOpType = SUBTRACTION;
    binOpExpr->lOperand = lint;
    binOpExpr->rOperand = rint;
    parseTree->type = BINOP;
    parseTree->binExpr = binOpExpr;
    return parseTree;
}
ParseTree *storeToVariable(char *identifier, ParseTree *rOperand)
{
    ParseTree *parseTree = malloc(sizeof(parseTree));
    BinOpExpr *binOpExpr = malloc(sizeof(binOpExpr));

    binOpExpr->BinOpType = ASSIGN;
    binOpExpr->lOperand = stringType(identifier);
    binOpExpr->rOperand = rOperand;

    parseTree->type = BINOP;
    parseTree->binExpr = binOpExpr;

    return parseTree;
}

ParseTree *multiply(ParseTree *lint, ParseTree *rint)
{

    // TO DO (and remove the "return 0;")
    ParseTree *parseTree = malloc(sizeof(parseTree));
    BinOpExpr *binOpExpr = malloc(sizeof(binOpExpr));
    binOpExpr->BinOpType = MULTIPLICATION;
    binOpExpr->lOperand = lint;
    binOpExpr->rOperand = rint;
    parseTree->type = BINOP;
    parseTree->binExpr = binOpExpr;
    return parseTree;
}
ParseTree *lessThan(ParseTree *lOperand, ParseTree *rOperand)
{
    ParseTree *parseTree = malloc(sizeof(ParseTree));
    BinOpExpr *binOpExpr = malloc(sizeof(BinOpExpr));

    binOpExpr->BinOpType = LESSTHAN;
    binOpExpr->lOperand = lOperand;
    binOpExpr->rOperand = rOperand;

    parseTree->type = BINOP;
    parseTree->binExpr = binOpExpr;

    return parseTree;
}

ParseTree *logicalNegation(ParseTree *rint)
{

    // TO DO (and remove the "return 0;")
    ParseTree *parseTree = malloc(sizeof(parseTree));
    UnOpExpr *unOpExpr = malloc(sizeof(unOpExpr));
    unOpExpr->UnOpType = LOGICALNEGATION;
    unOpExpr->rOperand = rint;
    parseTree->type = UNOP;
    parseTree->unExpr = unOpExpr;
    return parseTree;
}
ParseTree *declaration(char *identifier)
{
    ParseTree *parseTree = malloc(sizeof(parseTree));
    UnOpExpr *unOpExpr = malloc(sizeof(unOpExpr));

    unOpExpr->UnOpType = DECL;
    unOpExpr->rOperand = stringType(identifier);

    parseTree->type = UNOP;
    parseTree->unExpr = unOpExpr;

    return parseTree;
}
ParseTree *declarationWithAssign(char *identifier, ParseTree *rint)
{
    ParseTree *parseTree = malloc(sizeof(parseTree));
    BinOpExpr *binOpExpr = malloc(sizeof(binOpExpr));

    binOpExpr->BinOpType = ASSIGN;
    binOpExpr->lOperand = stringType(identifier);
    binOpExpr->rOperand = rint;

    parseTree->type = BINOP;
    parseTree->binExpr = binOpExpr;

    return parseTree;
}

ParseTree *doWhileLoop(ParseTree *body, ParseTree *condition)
{
    ParseTree *parseTree = malloc(sizeof(ParseTree));
    DoWhileExpr *doWhileExpr = malloc(sizeof(DoWhileExpr));

    doWhileExpr->body = body;
    doWhileExpr->condition = condition;

    parseTree->type = DOWHILE;
    parseTree->doWhileExpr = doWhileExpr;

    return parseTree;
}
#endif