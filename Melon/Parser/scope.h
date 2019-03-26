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
    Hashtable * symbolTable;
};

typedef struct Scope Scope;

extern Scope * ScopeConstructor(Scope * upperLevel);

#endif /* scope_h */
