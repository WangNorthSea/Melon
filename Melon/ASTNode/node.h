//
//  node.h
//  Melon
//
//  Created by 王浩宇 on 2019/3/18.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#ifndef node_h
#define node_h

struct ASTNode {
    int kind;
    char * file;
    int line;
    char * image;
    struct ASTNode * ptrs[6];
};

typedef struct ASTNode ASTNode;

#define IntegerLiteral 100
#define CharacterLiteral 101
#define StringLiteral 102
#define Identifier 103
#define Cast 104
#define PrefixOp 105
#define UnaryOp 106
#define Dereference 107
#define Address 108
#define SizeofType 109
#define SizeofExpr 110
#define SuffixOp 111
#define ArrayRef 112
#define Member 113
#define PtrMember 114
#define Funcall 115
#define BinaryOp 116

#endif /* node_h */
