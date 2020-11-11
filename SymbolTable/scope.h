
#ifndef scope_h
#define scope_h

struct Scope {
    struct Scope * upperLevel;
    struct Scope ** lowerLevel;
    Hashtable * symbolTable;
    int listLen;
    void (*appendScope)(struct Scope * appender, struct Scope * toAppend);
    ASTNode * (*lookup)(struct Scope * scope, char * key);
    ASTNode * (*localLookup)(struct Scope * scope, char * key);
};

typedef struct Scope Scope;

extern Scope * ScopeConstructor(Scope * upperLevel);

#endif /* scope_h */
