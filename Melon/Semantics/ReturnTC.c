//
//  ReturnTC.c
//  Melon
//
//  Created by 王浩宇 on 2019/4/16.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include "../ASTNode/node.h"
#include "../ASTNode/constructor.h"
#include "../SymbolTable/hashtable.h"
#include "../SymbolTable/scope.h"
#include "semantics.h"

void returnVoidType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    switch (type2 -> kind) {
        case IntegerLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharacterLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StringLiteral:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        //还需要改
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case ShortIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case IntType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case LongIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedCharType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StructType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnionType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case DoubleType:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        default:
            break;
    }
}

void returnCharType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharacterLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StringLiteral:
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            }
            break;
        case ShortIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case IntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case LongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedCharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedCharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedCharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StructType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnionType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case DoubleType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        default:
            break;
    }
}

void returnShortIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharacterLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case IntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case LongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedCharType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StructType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnionType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case DoubleType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        default:
            break;
    }
}

void returnIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharacterLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case ShortIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case IntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case LongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedCharType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StructType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnionType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case DoubleType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        default:
            break;
    }
}

void returnLongIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharacterLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case ShortIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case IntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case LongIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedCharType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StructType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnionType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case DoubleType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        default:
            break;
    }
}

void returnUnsignedCharType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharacterLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case ShortIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case IntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case LongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedCharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedCharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedCharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StructType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnionType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case DoubleType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        default:
            break;
    }
}

void returnUnsignedShortIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharacterLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case IntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case LongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedCharType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StructType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnionType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case DoubleType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        default:
            break;
    }
}

void returnUnsignedIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharacterLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case ShortIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case IntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case LongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedCharType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StructType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnionType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case DoubleType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        default:
            break;
    }
}

void returnUnsignedLongIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharacterLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case ShortIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case IntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case LongIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedCharType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedIntType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StructType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnionType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case DoubleType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        default:
            break;
    }
}

void returnStructType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    switch (type2 -> kind) {
        case IntegerLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharacterLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case ShortIntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case IntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case LongIntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedCharType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedShortIntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedIntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedLongIntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StructType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnionType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case DoubleType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        default:
            break;
    }
}

void returnUnionType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    switch (type2 -> kind) {
        case IntegerLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharacterLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case ShortIntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case IntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case LongIntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedCharType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedShortIntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedIntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedLongIntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StructType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnionType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case DoubleType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        default:
            break;
    }
}

void returnFloatType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharacterLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case ShortIntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case IntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case LongIntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedCharType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedShortIntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedIntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedLongIntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StructType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnionType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case DoubleType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        default:
            break;
    }
}

void returnDoubleType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharacterLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case CharType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case ShortIntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case IntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case LongIntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedCharType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedShortIntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedIntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnsignedLongIntType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case StructType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case UnionType:
            throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case FloatType:
            if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        case DoubleType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            else if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(fileChecking, type2 -> line, "return value type conflicting with function definition");
            break;
        default:
            break;
    }
}
