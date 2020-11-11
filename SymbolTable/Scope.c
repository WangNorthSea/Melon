
#include <stdlib.h>
#include "../ASTNode/node.h"
#include "hashtable.h"
#include "scope.h"

void appendScope(Scope * appender, Scope * toAppend) {
    appender -> listLen++;
    appender -> lowerLevel = (Scope **)realloc(appender -> lowerLevel, sizeof(Scope *) * appender -> listLen);
    appender -> lowerLevel[appender -> listLen - 1] = toAppend;
}

ASTNode * lookup(Scope * scope, char * key) {
    ASTNode * target = NULL;
    while (scope != NULL) {
        target = scope -> symbolTable -> get(scope -> symbolTable, key);
        if (target != NULL)
            return target;
        else
            scope = scope -> upperLevel;
    }
    return target;
}

ASTNode * localLookup(Scope * scope, char * key) {
    ASTNode * target = scope -> symbolTable -> get(scope -> symbolTable, key);
    return target;
}


Scope * ScopeConstructor(Scope * upperLevel) {
    Scope * scope = (Scope *)malloc(sizeof(Scope));
    scope -> listLen = 0;
    scope -> upperLevel = upperLevel;
    scope -> symbolTable = HashtableConstructor();
    scope -> lowerLevel = NULL;
    scope -> appendScope = appendScope;
    scope -> lookup = lookup;
    scope -> localLookup = localLookup;
    return scope;
}
