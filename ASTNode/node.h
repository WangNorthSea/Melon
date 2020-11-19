
#ifndef node_h
#define node_h

#include "../List/list.h"

struct ASTNode {
    struct list_head child_list;
    struct list_head brother_list;
    int kind;
    int listLen;
    char * file;
    int line;
    char * image;
    struct ASTNode * ptrs[6];
    struct ASTNode * list;
    void (*append)(struct ASTNode *, struct ASTNode);
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
#define FuncPtr 142
#define DefinedVariables 143
#define Variable 144
#define DefinedFunc 145
#define VoidType 146
#define CharType 147
#define ShortIntType 148
#define IntType 149
#define LongIntType 150
#define UnsignedCharType 151
#define UnsignedShortIntType 152
#define UnsignedIntType 153
#define UnsignedLongIntType 154
#define StructType 155
#define UnionType 156
#define ParamsNode 158
#define FuncPtrParam 159
#define ConstParam 160
#define NormalParam 161
#define DefinedConstVariables 162
#define DefinedStruct 163
#define DefinedUnion 164
#define TypeDef 165
#define Args 166
#define MemberList 167
#define Slot 168
#define PtrRef 169
#define Varname 170
#define UnfixedArray 171
#define FixedArray 172
#define Static 173
#define Name 174
#define Import 175
#define TopDefs 176
#define Imports 177
#define Root 178
#define FloatType 179
#define DoubleType 180
#define FloatLiteral 181
#define UnlimitedParams 182
#define FuncStmt 183
#define ExternFunc 184
#define ExternVar 185
#define ExternConst 186
#define ConstFuncPtr 187
#define ConstVariable 188

//CACT
#define BoolType 189
#define BoolLiteral 190
#define ListExpr 191

#define ConstMark 192

#endif /* node_h */
