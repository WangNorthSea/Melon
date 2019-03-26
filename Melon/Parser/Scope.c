//
//  Scope.c
//  Melon
//
//  Created by 王浩宇 on 2019/3/26.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include "../ASTNode/node.h"
#include "hashtable.h"
#include "scope.h"

Scope * ScopeConstructor(Scope * upperLevel) {
    Scope * scope = (Scope *)malloc(sizeof(Scope));
    scope -> upperLevel = upperLevel;
    scope -> symbolTable = HashtableConstructor();
    
    return scope;
}
