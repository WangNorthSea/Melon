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

void semanticAnalyze(ASTNode * root) {
    scope = ScopeConstructor(NULL);
    iterator(root);
}

void iterator(ASTNode * node) {
    switch (node -> kind) {
        case DefinedFunc:
            
            break;
    }
}
