//
//  DumpScope.c
//  Melon
//
//  Created by 王浩宇 on 2019/4/16.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#include <stdio.h>
#include "../ASTNode/node.h"
#include "../SymbolTable/hashtable.h"
#include "../SymbolTable/scope.h"
#include "./ASTdumper.h"

int dumpScopeLevel = 0;

void dumpASTNode(int dumplevel, ASTNode * node) {
    int j;
    for (j = 0; j < dumplevel + 1; j++)
        printf("\t");
    printf("<<");
    kindTable(node -> kind);
    printf(">>\n");
    for (j = 0; j < dumplevel + 1; j++)
        printf("\t");
    printf("line: %d\n", node -> line);
    
    if (node -> image != NULL) {
        for (j = 0; j < dumplevel + 1; j++)
            printf("\t");
        printf("image: %s\n", node -> image);
    }
    
    if (node -> kind != Block && node -> kind != DefinedFunc && node -> kind != DefinedStruct && node -> kind != DefinedUnion) {
        for (j = 0; j < 6; j++) {
            if (node -> ptrs[j] != NULL)
                dumpASTNode(dumplevel + 1, node -> ptrs[j]);
        }
    
        for (j = 0; j < node -> listLen; j++)
            dumpASTNode(dumplevel + 1, node -> list + j);
    }
    else if (node -> kind == DefinedFunc) {
        for (j = 0; j < 4; j++) {
            if (node -> ptrs[j] != NULL)
                dumpASTNode(dumplevel + 1, node -> ptrs[j]);
        }
    }
}

//仍然有问题
void dumpScope(Scope * scope) {
    int i, branch = 0;
    
    dumpScopeLevel++;
    
    for (i = 0; i < TableArraySize; i++) {
        if (scope -> symbolTable -> tableArray[i].key != NULL) {
            struct Value * next = &(scope -> symbolTable -> tableArray[i]);
            while (next != NULL) {
                int j;
                for (j = 0; j < dumpScopeLevel; j++)
                    printf("\t");
                printf("key: %s\n", next -> key);
                dumpASTNode(dumpScopeLevel, (ASTNode *)next -> target);
                if (((ASTNode *)next -> target) -> kind == DefinedFunc || ((ASTNode *)next -> target) -> kind == DefinedStruct || ((ASTNode *)next -> target) -> kind == DefinedUnion || ((ASTNode *)next -> target) -> kind == Block) {
                    dumpScope(scope -> lowerLevel[branch]);
                    branch++;
                }
                next = next -> next;
            }
        }
    }
    
    dumpScopeLevel--;
}
