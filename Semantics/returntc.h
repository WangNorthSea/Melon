//
//  returntc.h
//  Melon
//
//  Created by 王浩宇 on 2019/4/16.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#ifndef returntc_h
#define returntc_h

extern void returnVoidType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void returnCharType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void returnShortIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void returnIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void returnLongIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void returnUnsignedCharType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void returnUnsignedShortIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void returnUnsignedIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void returnUnsignedLongIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void returnStructType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void returnUnionType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void returnFloatType(ASTNode * parent, ASTNode * type1, ASTNode * type2);
extern void returnDoubleType(ASTNode * parent, ASTNode * type1, ASTNode * type2);

#endif /* returntc_h */
