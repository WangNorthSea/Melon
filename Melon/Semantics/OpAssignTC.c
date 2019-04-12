//
//  OpAssignTC.c
//  Melon
//
//  Created by 王浩宇 on 2019/4/12.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include "../ASTNode/node.h"
#include "../ASTNode/constructor.h"
#include "semantics.h"

//void类型必定是void指针，单独的void类型非法
void opAssignVoidType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type1 -> line, "string literal not allowed");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            break;
        case CharType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            break;
        case UnsignedCharType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            break;
        case StructType:
            if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct type not allowed");
            break;
        case UnionType:
            if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "union type mismatched");
            break;
        case FloatType:
            if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case DoubleType:
            if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        default:
            break;
    }
}

void opAssignCharType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedCharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedCharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedCharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedCharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedCharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedCharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedCharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case StructType:
            if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct type not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnionType:
            if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "union type not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case FloatType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type2 -> line, "data type mismatched");
            }
            break;
        case DoubleType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type2 -> line, "data type mismatched");
            }
            break;
        default:
            break;
    }
}

void opAssignShortIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case StructType:
            if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct type not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnionType:
            if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "union type not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case FloatType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type2 -> line, "data type mismatched");
            }
            break;
        case DoubleType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type2 -> line, "data type mismatched");
            }
            break;
        default:
            break;
    }
}

void opAssignIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case StructType:
            if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct type not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnionType:
            if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "union type not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case FloatType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type2 -> line, "data type mismatched");
            }
            break;
        case DoubleType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type2 -> line, "data type mismatched");
            }
            break;
        default:
            break;
    }
}

void opAssignLongIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            break;
        case StructType:
            if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct type not allowed");
            break;
        case UnionType:
            if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "union type not allowed");
            break;
        case FloatType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type2 -> line, "data type mismatched");
            }
            break;
        case DoubleType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type2 -> line, "data type mismatched");
            }
            break;
        default:
            break;
    }
}

void opAssignUnsignedCharType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedCharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedCharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedCharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedCharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedCharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedCharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedCharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case StructType:
            if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct type not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnionType:
            if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "union type not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case FloatType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type2 -> line, "data type mismatched");
            }
            break;
        case DoubleType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type2 -> line, "data type mismatched");
            }
            break;
        default:
            break;
    }
}

void opAssignUnsignedShortIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case StructType:
            if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct type not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnionType:
            if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "union type not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case FloatType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type2 -> line, "data type mismatched");
            }
            break;
        case DoubleType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type2 -> line, "data type mismatched");
            }
            break;
        default:
            break;
    }
}

void opAssignUnsignedIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case StructType:
            if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct type not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnionType:
            if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "union type not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case FloatType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type2 -> line, "data type mismatched");
            }
            break;
        case DoubleType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type2 -> line, "data type mismatched");
            }
            break;
        default:
            break;
    }
}

void opAssignUnsignedLongIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            break;
        case StructType:
            if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct type not allowed");
            break;
        case UnionType:
            if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "union type not allowed");
            break;
        case FloatType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type2 -> line, "data type mismatched");
            }
            break;
        case DoubleType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type2 -> line, "data type mismatched");
            }
            break;
        default:
            break;
    }
}

void opAssignStructType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct type not allowed");
            break;
        case CharacterLiteral:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct type not allowed");
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct type not allowed");
            break;
        case CharType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct type not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct type not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct type not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct type not allowed");
            break;
        case UnsignedCharType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct type not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct type not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct type not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct type not allowed");
            break;
        case StructType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct type not allowed");
            break;
        case UnionType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct or union type not allowed");
            break;
        case FloatType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case DoubleType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        default:
            break;
    }
}

void opAssignUnionType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "union type not allowed");
            break;
        case CharacterLiteral:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "union type not allowed");
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "union type not allowed");
            break;
        case CharType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "union type not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "union type not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "union type not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "union type not allowed");
            break;
        case UnsignedCharType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "union type operand not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "union type not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "union type not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "union type not allowed");
            break;
        case StructType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "struct or union type not allowed");
            break;
        case UnionType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "union type not allowed");
            break;
        case FloatType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case DoubleType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        default:
            break;
    }
}

void opAssignFloatType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case CharacterLiteral:
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type1 -> line, "string literal not allowed");
            break;
        case FloatLiteral:
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case StructType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case UnionType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case FloatType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            else if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case DoubleType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            else if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        default:
            break;
    }
}

void opAssignDoubleType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case CharacterLiteral:
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type1 -> line, "string literal operand not allowed");
            break;
        case FloatLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case StructType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case UnionType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case FloatType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            else if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[2] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case DoubleType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            else if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        default:
            break;
    }
}
