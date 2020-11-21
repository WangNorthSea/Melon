#!/bin/bash
gcc -O0 -g ASTNode/*.c Dumper/*.c Parser/*.c Lexer/*.c SymbolTable/*.c Semantics/*.c IR/*.c Report/*.c File/*.c main.c -o main