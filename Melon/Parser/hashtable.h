//
//  hashtable.h
//  Melon
//
//  Created by 王浩宇 on 2019/3/26.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#ifndef hashtable_h
#define hashtable_h

#define TableArraySize 1024

struct Value {
    ASTNode * target;
    char * key;
    struct Value * next;
};

struct Hashtable {
    struct Value tableArray[TableArraySize];
    void (*put)(struct Hashtable * table, char * key, ASTNode * value);
    ASTNode * (*get)(struct Hashtable * table, char * key);
};

typedef struct Value Value;
typedef struct Hashtable Hashtable;

extern Hashtable * HashtableConstructor(void);

#endif /* hashtable_h */
