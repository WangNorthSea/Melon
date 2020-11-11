
#ifndef binoptc_h
#define binoptc_h

extern void integerLiteralType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void stringLiteralType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void floatLiteralType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void voidType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void charType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void shortIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void intType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void longIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void unsignedCharType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void unsignedShortIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void unsignedIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void unsignedLongIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void structType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void unionType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void floatType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void doubleType(ASTNode * parent, ASTNode * type1, ASTNode * type2);

#endif /* binoptc_h */
