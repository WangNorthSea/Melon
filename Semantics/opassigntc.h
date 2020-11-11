
#ifndef opassigntc_h
#define opassigntc_h

extern void opAssignVoidType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void opAssignCharType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void opAssignShortIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void opAssignIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void opAssignLongIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void opAssignUnsignedCharType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void opAssignUnsignedShortIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void opAssignUnsignedIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void opAssignUnsignedLongIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void opAssignStructType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void opAssignUnionType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void opAssignFloatType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void opAssignDoubleType(ASTNode * parent, ASTNode * type1, ASTNode * type2);

#endif /* opassigntc_h */
