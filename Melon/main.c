//
//  main.c
//  Melon
//
//  Created by 王浩宇 on 2019/3/16.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "Lexer/token.h"
#include "Lexer/lexer.h"
#include "ASTNode/node.h"
#include "Parser/parser.h"
#include "SymbolTable/hashtable.h"
#include "SymbolTable/scope.h"
#include "Semantics/semantics.h"
#include "Dumper/ASTdumper.h"
#include "Dumper/scopedumper.h"

typedef struct {
    int line;
} Node;

typedef struct {
    Node line;
} ExprNode;

int main(int argc, const char * argv[]) {
    if (argc == 1) {
        printf("Melon: no input file\n");
        return 0;
    }
    
    FILE * fp = fopen(argv[1], "r");
    
    if (fp == NULL) {
        printf("Melon: %s: no such file\n", argv[1]);
        exit(-1);
    }
    
    Token * headToken = lexicalAnalyze(fp);
    Token * headToken2 = headToken;
    while (headToken -> next != NULL) {
        printf("line: %d\tvalue: %s\tkind: %d\n", headToken -> next -> beginLine, headToken -> next -> image, headToken -> next -> kind);
        headToken = headToken -> next;
    }
    
    printf("\nAST:\n");
    
    parsingFile = argv[1];
    ASTNode * rootNode = compilationUnit(headToken2);
    dumpAST(rootNode);
    
    semanticAnalyze(rootNode, parsingFile);
    
    printf("\nAfter semantic analysis...\nScope:\n");
    
    dumpScope(scope);
    
    printf("\nAST:\n");
    
    dumpAST(rootNode);
    
    return 0;
}
