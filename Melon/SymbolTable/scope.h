//
//  scope.h
//  Melon
//
//  Created by 王浩宇 on 2019/3/26.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#ifndef scope_h
#define scope_h

struct Scope {
    struct Scope * upperLevel;
    struct Scope ** lowerLevel;
    Hashtable * symbolTable;
    int listLen;
    void (*appendScope)(struct Scope * appender, struct Scope * toAppend);
};

typedef struct Scope Scope;

extern Scope * ScopeConstructor(Scope * upperLevel);

#endif /* scope_h */
