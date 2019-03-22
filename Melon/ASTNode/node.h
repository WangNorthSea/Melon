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
#define LogicAnd 117
#define LogicOr 118
#define CondExpr 119
#define Assign 120
#define OpAssign 121
#define Operator 122
#define If 123
#define While 124
#define DoWhile 125
#define For 126
#define Switch 127
#define Break 128
#define Continue 129
#define Goto 130
#define Return 131
#define CaseClauses 132
#define CaseClause 133
#define DefaultClause 134
#define Cases 135
#define CaseBody 136
#define Block 137
#define Stmts 138
#define EmptyStmt 139
#define Label 140
#define Stmt 141

#endif /* node_h */
