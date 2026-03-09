// Copyright (c) 2023-2025 Dan O’Malley
// This file is licensed under the MIT License. See LICENSE for details.


#include <stdlib.h>
#include <string.h>

#ifndef COMPILER_PARSETREE
#define COMPILER_PARSETREE

typedef enum { FUNC, INT, STRING, BINOP, UNOP } ParseTreeType;
typedef enum { ADDITION, SUBTRACTION, MULTIPLICATION } ParseTreeBinOp;
typedef enum { LOGICALNEGATION, DECLASSIGN } ParseTreeUnOp;

typedef struct parseTree ParseTree;

typedef struct BinOpExpr {
    ParseTreeBinOp BinOpType;
    ParseTree *lOperand;
    ParseTree *rOperand;
} BinOpExpr;

typedef struct UnOpExpr {
    ParseTreeUnOp UnOpType;
    ParseTree *rOperand;
} UnOpExpr;

struct parseTree {
    ParseTreeType type;
    union {
        int constantValue;
        char *string;
        BinOpExpr *binExpr;
        UnOpExpr *unExpr;
    };
};


ParseTree *funcType(char *string) {
    ParseTree *parseTree = malloc(sizeof(parseTree));
    parseTree->type = FUNC;
    parseTree->string = string;
    return parseTree;
}


ParseTree *intType(int constantValue) {
    ParseTree *parseTree = malloc(sizeof(parseTree));
    parseTree->type = INT;
    parseTree->constantValue = constantValue;
    return parseTree;
}

ParseTree *stringType(char *string) {
    
    // TO DO (and remove the "return 0;")
    return 0;
}

ParseTree *add(ParseTree *lOperand, ParseTree *rOperand) {
    ParseTree *parseTree = malloc(sizeof(parseTree));
    BinOpExpr *binOpExpr = malloc(sizeof(binOpExpr));
    binOpExpr->BinOpType = ADDITION;
    binOpExpr->lOperand = lOperand;
    binOpExpr->rOperand = rOperand;
    parseTree->type = BINOP;
    parseTree->binExpr = binOpExpr;
    return parseTree;
}

ParseTree *subtract(ParseTree *lint, ParseTree *rint) {
    
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

ParseTree *multiply(ParseTree *lint, ParseTree *rint) {
    
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

ParseTree *logicalNegation(ParseTree *rint) {

    // TO DO (and remove the "return 0;")
    ParseTree *parseTree = malloc(sizeof(parseTree));
    UnOpExpr *unOpExpr = malloc(sizeof(unOpExpr));
    unOpExpr->UnOpType = LOGICALNEGATION;
    unOpExpr->rOperand = rint;
    parseTree->type = UNOP;
    parseTree->unExpr = unOpExpr;
    return parseTree;
}

ParseTree *declarationWithAssign(ParseTree *rint) {

    // TO DO (and remove the "return 0;")
     ParseTree *parseTree = malloc(sizeof(parseTree));
    UnOpExpr *unOpExpr = malloc(sizeof(unOpExpr));
    unOpExpr->UnOpType = DECLASSIGN;
    unOpExpr->rOperand = rint;
    parseTree->type = UNOP;
    parseTree->unExpr = unOpExpr;
    return parseTree;
}


#endif