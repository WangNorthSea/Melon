//
//  SA.c
//  Melon
//
//  Created by 王浩宇 on 2019/3/28.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include "../ASTNode/node.h"
#include "../SymbolTable/hashtable.h"
#include "../SymbolTable/scope.h"

void iterator(ASTNode * node);

Scope * scope;

char * fileChecking;

void definedFunc(ASTNode * node);

void definedStruct(ASTNode * node);

void definedUnion(ASTNode * node);

void semanticAnalyze(ASTNode * root, char * file) {
    fileChecking = file;
    scope = ScopeConstructor(NULL);
    iterator(root);
}

void iterator(ASTNode * node) {
    switch (node -> kind) {
        case DefinedFunc:
            definedFunc(node);
            break;
        case DefinedStruct:
            definedStruct(node);
            break;
        case DefinedUnion:
            definedUnion(node);
            break;
    }
}

void definedFunc(ASTNode * node) {
    
}

void definedStruct(ASTNode * node) {
    
}

void definedUnion(ASTNode * node) {
    
}
