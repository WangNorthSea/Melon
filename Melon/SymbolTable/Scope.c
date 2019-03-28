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

void append(Scope * appender, Scope * toAppend) {
    appender -> listLen++;
    appender -> lowerLevel = (Scope **)realloc(appender -> lowerLevel, sizeof(Scope *) * appender -> listLen);
    appender -> lowerLevel[appender -> listLen - 1] = toAppend;
}

Scope * ScopeConstructor(Scope * upperLevel) {
    Scope * scope = (Scope *)malloc(sizeof(Scope));
    scope -> listLen = 0;
    scope -> upperLevel = upperLevel;
    scope -> symbolTable = HashtableConstructor();
    scope -> lowerLevel = NULL;
    scope -> append = append;
    return scope;
}
