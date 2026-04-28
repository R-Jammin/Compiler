// Copyright (c) 2023-2025 Dan O’Malley
// This file is licensed under the MIT License. See LICENSE for details.


#ifndef SYMBOLTABLE_HEADER
#define SYMBOLTABLE_HEADER

#define MAXSYMBOLS 100

struct SymbolTable {
    int totalEntries;
    char entryType[20];
    char symbolType[10];
    char symbolName[20];
    int symbolLocation;
    int size;
} symbolTable[MAXSYMBOLS];

#endif