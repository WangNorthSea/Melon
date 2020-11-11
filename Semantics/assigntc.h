
#ifndef assigntc_h
#define assigntc_h

extern void assignVoidType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void assignCharType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void assignShortIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void assignIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void assignLongIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void assignUnsignedCharType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void assignUnsignedShortIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void assignUnsignedIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void assignUnsignedLongIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void assignStructType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void assignUnionType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void assignFloatType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void assignDoubleType(ASTNode * parent, ASTNode * type1, ASTNode * type2);

#endif /* assigntc_h */
