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
    Token * pos = NULL;
    list_for_each_entry(pos, &headToken -> list, list) {
        printf("line: %d\t\tvalue: %s\t\tkind: %d   \t\tfpos: %lld\n", pos -> beginLine, pos -> image, pos -> kind, pos -> fpos);
    }
    
    printf("\nAST:\n");
    
    parsingFile = argv[1];
    ASTNode * rootNode = compilationUnit(headToken);

    if (!list_empty(&err_list -> list)) {
        error_t * p = NULL;
        list_for_each_entry(p, &err_list -> list, list) {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d expected %s\n", p -> file, p -> line, p -> expected);
        }
    }
    else
        dumpAST(rootNode);
    
    //semanticAnalyze(rootNode, parsingFile);
    
    //printf("\nAfter semantic analysis...\nScope:\n");
    
    //dumpScope(scope);
    
    //printf("\nAST:\n");
    
    //dumpAST(rootNode);
    
    return 0;
}
