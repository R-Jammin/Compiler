// Copyright (c) 2023-2025 Dan O’Malley
// This file is licensed under the MIT License. See LICENSE for details.


#include <stdlib.h>
#include <string.h>

#ifndef COMPILER_PARSETREE
#define COMPILER_PARSETREE

typedef enum { FUNC, INT, STRING, BINOP, UNOP, RELOAD} ParseTreeType;
typedef enum { ADDITION, SUBTRACTION, MULTIPLICATION } ParseTreeBinOp;
typedef enum { LOGICALNEGATION, DECLASSIGN, RET, STORETOSTACK } ParseTreeUnOp;

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
     ParseTree *parseTree = malloc(sizeof(ParseTree));
    parseTree->type = STRING;
    parseTree->string = string;
    return parseTree;
}

ParseTree *reload(char *string) {
    
    ParseTree *parseTree = malloc(sizeof(ParseTree));
    parseTree->type = RELOAD;
    parseTree->string = string;
    return parseTree;
}

ParseTree *add() {
    ParseTree *parseTree = malloc(sizeof(parseTree));
    BinOpExpr *binOpExpr = malloc(sizeof(binOpExpr));
    binOpExpr->BinOpType = ADDITION;
    parseTree->type = BINOP;
    parseTree->binExpr = binOpExpr;
    return parseTree;
}

ParseTree *subtract() {
    
    ParseTree *parseTree = malloc(sizeof(ParseTree));
    BinOpExpr *binOpExpr = malloc(sizeof(_BITS_STDIO_LIM_H));
    binOpExpr->BinOpType = SUBTRACTION;
    parseTree->type = BINOP;
    parseTree->binExpr = binOpExpr;
    return parseTree;
}

ParseTree *multiply() {
    
    ParseTree *parseTree = malloc(sizeof(ParseTree));
    BinOpExpr *binOpExpr = malloc(sizeof(binOpExpr));
    binOpExpr->BinOpType = MULTIPLICATION;
    parseTree->type = BINOP;
    parseTree->binExpr = binOpExpr;
    return parseTree;
    
}

ParseTree *logicalNegation(ParseTree *rint) {
    
    ParseTree *parseTree = malloc(sizeof(ParseTree));
    UnOpExpr *unOpExpr = malloc(sizeof(unOpExpr));
    unOpExpr->UnOpType = LOGICALNEGATION;
    unOpExpr->rOperand = rint;
    parseTree->type = UNOP;
    parseTree->unExpr = unOpExpr;
    return parseTree;
}

ParseTree *declarationWithAssign(ParseTree *rint) {
    
    ParseTree *parseTree = malloc(sizeof(ParseTree));
    UnOpExpr *unOpExpr = malloc(sizeof(unOpExpr));
    unOpExpr->UnOpType = DECLASSIGN;
    unOpExpr->rOperand = rint;
    parseTree->type = UNOP;
    parseTree->unExpr = unOpExpr;
    return parseTree;
}

ParseTree *storeToStack(ParseTree *rint) {
    
    ParseTree *parseTree = malloc(sizeof(ParseTree));
    UnOpExpr *unOpExpr = malloc(sizeof(unOpExpr));
    unOpExpr->UnOpType = STORETOSTACK;
    unOpExpr->rOperand = rint;
    parseTree->type = UNOP;
    parseTree->unExpr = unOpExpr;
    return parseTree;
}

ParseTree *ret(ParseTree *rint) {
    
    ParseTree *parseTree = malloc(sizeof(ParseTree));
    UnOpExpr *unOpExpr = malloc(sizeof(unOpExpr));
    unOpExpr->UnOpType = RET;
    unOpExpr->rOperand = rint;
    parseTree->type = UNOP;
    parseTree->unExpr = unOpExpr;
    return parseTree;
}

#endif