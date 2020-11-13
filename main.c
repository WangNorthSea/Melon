
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

char * parsingFile = NULL;

int main(int argc, const char * argv[]) {
    if (argc == 1) {
        printf("Melon: no input file\n");
        return 0;
    }
    
    FILE * fp = fopen(argv[1], "r");
    parsingFile = (char *)argv[1];
    
    if (fp == NULL) {
        printf("Melon: %s: no such file\n", argv[1]);
        exit(-1);
    }

    err_list = ErrorConstructor(0, 0, NULL, NULL, NULL);
    
    Token * headToken = lexicalAnalyze(fp);
    Token * pos = NULL;
    list_for_each_entry(pos, &headToken -> list, list) {
        printf("line: %d\t\tvalue: %s\t\tkind: %d   \t\tfpos: %lld\n", pos -> beginLine, pos -> image, pos -> kind, pos -> fpos);
    }

    if (!list_empty(&err_list -> list)) {
        dumpErrorList(err_list, fp);
        return -1;
    }
    
    printf("\nAST:\n");
    
    
    ASTNode * rootNode = compilationUnit(headToken);

    if (!list_empty(&err_list -> list)) {
        dumpErrorList(err_list, fp);
        return -1;
    }

    dumpAST(rootNode);
    
    semanticAnalyze(rootNode, parsingFile);
    
    printf("\nAfter semantic analysis...\nScope:\n");

    if (!list_empty(&err_list -> list)) {
        dumpErrorList(err_list, fp);
        return -1;
    }
    
    dumpScope(scope);

    printf("\nAST:\n");
    dumpAST(rootNode);
    
    return 0;
}
