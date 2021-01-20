
#include <stdlib.h>
#include <string.h>
#include "../ASTNode/node.h"
#include "../ASTNode/constructor.h"
#include "../SymbolTable/hashtable.h"
#include "../SymbolTable/scope.h"
#include "semantics.h"

void integerLiteralType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(type1, "data type mismatched");
            break;
        case FloatLiteral:
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            break;
        case CharType:
            break;
        case ShortIntType:
            break;
        case IntType:
            break;
        case LongIntType:
            break;
        case UnsignedCharType:
            break;
        case UnsignedShortIntType:
            break;
        case UnsignedIntType:
            break;
        case UnsignedLongIntType:
            break;
        case StructType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "struct type operand not allowed");
            break;
        case UnionType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "union type operand not allowed");
            break;
        case FloatType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case DoubleType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        default:
            break;
    }
}

void stringLiteralType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    switch (type2 -> kind) {
        case IntegerLiteral:
            throwSemanticError(type1, "string literal operand not allowed");
            break;
        case CharacterLiteral:
            throwSemanticError(type1, "string literal operand not allowed");
            break;
        case StringLiteral:
            throwSemanticError(type1, "string literal operand not allowed");
            break;
        case FloatLiteral:
            throwSemanticError(type1, "string literal operand not allowed");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            throwSemanticError(type1, "string literal operand not allowed");
            break;
        case CharType:
            throwSemanticError(type1, "string literal operand not allowed");
            break;
        case ShortIntType:
            throwSemanticError(type1, "string literal operand not allowed");
            break;
        case IntType:
            throwSemanticError(type1, "string literal operand not allowed");
            break;
        case LongIntType:
            throwSemanticError(type1, "string literal operand not allowed");
            break;
        case UnsignedCharType:
            throwSemanticError(type1, "string literal operand not allowed");
            break;
        case UnsignedShortIntType:
            throwSemanticError(type1, "string literal operand not allowed");
            break;
        case UnsignedIntType:
            throwSemanticError(type1, "string literal operand not allowed");
            break;
        case UnsignedLongIntType:
            throwSemanticError(type1, "string literal operand not allowed");
            break;
        case StructType:
            throwSemanticError(type1, "string literal operand not allowed");
            break;
        case UnionType:
            throwSemanticError(type1, "string literal operand not allowed");
            break;
        case FloatType:
            throwSemanticError(type1, "string literal operand not allowed");
            break;
        case DoubleType:
            throwSemanticError(type1, "string literal operand not allowed");
            break;
        default:
            break;
    }
}

//浮点数字面量按double类型存储
void floatLiteralType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            throwSemanticError(type1, "data type mismatched");
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(type1, "data type mismatched");
            break;
        case FloatLiteral:
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            throwSemanticError(type1, "data type mismatched");
            break;
        case CharType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(type1, "data type mismatched");
            break;
        case ShortIntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(type1, "data type mismatched");
            break;
        case IntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(type1, "data type mismatched");
            break;
        case LongIntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(type1, "data type mismatched");
            break;
        case UnsignedCharType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(type1, "data type mismatched");
            break;
        case UnsignedShortIntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(type1, "data type mismatched");
            break;
        case UnsignedIntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(type1, "data type mismatched");
            break;
        case UnsignedLongIntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(type1, "data type mismatched");
            break;
        case StructType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "struct type operand not allowed");
            else
                throwSemanticError(type1, "data type mismatched");
            break;
        case UnionType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "union type operand not allowed");
            else
                throwSemanticError(type1, "data type mismatched");
            break;
        case FloatType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(type1, "data type mismatched");
            break;
        case DoubleType:
            if (type2 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            break;
        default:
            break;
    }
}

void voidType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(type1, "string literal operand not allowed");
            break;
        case FloatLiteral:
            throwSemanticError(type1, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            break;
        case CharType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            break;
        case UnsignedCharType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            break;
        case StructType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "struct type not allowed");
            break;
        case UnionType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "union type mismatched");
            break;
        case FloatType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "data type mismatched");
            break;
        case DoubleType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "data type mismatched");
            break;
        default:
            break;
    }
}

//无符号数扩展为无符号扩展，有符号数扩展为有符号扩展

void charType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(type1, "data type mismatched");
            break;
        case FloatLiteral:
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(type1, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case StructType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "struct type operand not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnionType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "union type operand not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case FloatType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(type1, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case DoubleType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(type1, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        default:
            break;
    }
}

void shortIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(type1, "data type mismatched");
            break;
        case FloatLiteral:
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(type1, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case StructType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "struct type operand not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnionType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "union type operand not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case FloatType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(type1, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case DoubleType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(type1, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        default:
            break;
    }
}

void intType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    int dim1 = 0, dim2 = 0;

    if (type1 -> ptrs[1] != NULL) {
        dim1 = type1 -> ptrs[1] -> listLen;
    }
    if (type2 -> ptrs[1] != NULL) {
        dim2 = type2 -> ptrs[1] -> listLen;
    }
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(type1, "data type mismatched");
            break;
        case FloatLiteral:
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(type1, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            /*if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }*/
            if (dim1 != dim2) {
                throwSemanticError(type1, "dimension mismatched");
            }
            else if (dim1 != 0) {
                for (int i = 0; i < dim1; i++) {
                    if (strcmp(type1 -> ptrs[1] -> list[i].image, type2 -> ptrs[1] -> list[i].image)) {
                        throwSemanticError(type1, "array size mismatched");
                        break;
                    }
                }
            }
            break;
        case LongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case StructType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "struct type operand not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnionType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "union type operand not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case FloatType:
            /*if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(type1, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }*/
            throwSemanticError(type1, "data type mismatched");
            break;
        case DoubleType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(type1, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        default:
            break;
    }
}

void longIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(type1, "data type mismatched");
            break;
        case FloatLiteral:
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(type1, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            break;
        case StructType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "struct type operand not allowed");
            break;
        case UnionType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "union type operand not allowed");
            break;
        case FloatType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(type1, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case DoubleType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(type1, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        default:
            break;
    }
}

void unsignedCharType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(type1, "data type mismatched");
            break;
        case FloatLiteral:
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(type1, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case StructType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "struct type operand not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnionType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "union type operand not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case FloatType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(type1, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case DoubleType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(type1, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        default:
            break;
    }
}

void unsignedShortIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(type1, "data type mismatched");
            break;
        case FloatLiteral:
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(type1, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case StructType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "struct type operand not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnionType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "union type operand not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case FloatType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(type1, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case DoubleType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(type1, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        default:
            break;
    }
}

void unsignedIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(type1, "data type mismatched");
            break;
        case FloatLiteral:
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(type1, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case StructType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "struct type operand not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnionType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "union type operand not allowed");
            else if (type1 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case FloatType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(type1, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case DoubleType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(type1, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        default:
            break;
    }
}

void unsignedLongIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(type1, "data type mismatched");
            break;
        case FloatLiteral:
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(type1, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            break;
        case StructType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "struct type operand not allowed");
            break;
        case UnionType:
            if (type2 -> listLen == 0)
                throwSemanticError(type1, "union type operand not allowed");
            break;
        case FloatType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(type1, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case DoubleType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                throwSemanticError(type1, "data type mismatched");
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        default:
            break;
    }
}

void structType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "struct type operand not allowed");
            break;
        case CharacterLiteral:
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "struct type operand not allowed");
            break;
        case StringLiteral:
            throwSemanticError(type1, "data type mismatched");
            break;
        case FloatLiteral:
            throwSemanticError(type1, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "struct type not allowed");
            break;
        case CharType:
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "struct type operand not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "struct type operand not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "struct type operand not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "struct type operand not allowed");
            break;
        case UnsignedCharType:
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "struct type operand not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "struct type operand not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "struct type operand not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "struct type operand not allowed");
            break;
        case StructType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(type1, "struct type operand not allowed");
            break;
        case UnionType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(type1, "struct or union type operand not allowed");
            break;
        case FloatType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(type1, "data type mismatched");
            break;
        case DoubleType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(type1, "data type mismatched");
            break;
        default:
            break;
    }
}

void unionType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "union type operand not allowed");
            break;
        case CharacterLiteral:
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "union type operand not allowed");
            break;
        case StringLiteral:
            throwSemanticError(type1, "data type mismatched");
            break;
        case FloatLiteral:
            throwSemanticError(type1, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "union type not allowed");
            break;
        case CharType:
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "union type operand not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "union type operand not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "union type operand not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "union type operand not allowed");
            break;
        case UnsignedCharType:
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "union type operand not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "union type operand not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "union type operand not allowed");
            else if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "union type operand not allowed");
            break;
        case StructType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(type1, "struct or union type operand not allowed");
            break;
        case UnionType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(type1, "union type operand not allowed");
            break;
        case FloatType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(type1, "data type mismatched");
            break;
        case DoubleType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(type1, "data type mismatched");
            break;
        default:
            break;
    }
}

void floatType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            /*if (type1 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }*/
            break;
        case CharacterLiteral:
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case StringLiteral:
            throwSemanticError(type1, "string literal operand not allowed");
            break;
        case FloatLiteral:
            /*if (type1 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else*/
            if (type1 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "data type mismatched");
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            /*if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }*/
            throwSemanticError(type1, "data type mismatched");
            break;
        case LongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case StructType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(type1, "data type mismatched");
            break;
        case UnionType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(type1, "data type mismatched");
            break;
        case FloatType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            else if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(type1, "data type mismatched");
            break;
        case DoubleType:
            /*if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            else if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(type1, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[0];
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }*/
            throwSemanticError(type1, "data type mismatched");
            break;
        default:
            break;
    }
}

void doubleType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case CharacterLiteral:
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case StringLiteral:
            throwSemanticError(type1, "string literal operand not allowed");
            break;
        case FloatLiteral:
            if (type1 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            if (type1 -> listLen == 0)
                throwSemanticError(type1, "data type mismatched");
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            /*if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }*/
            throwSemanticError(type1, "data type mismatched");
            break;
        case LongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case StructType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(type1, "data type mismatched");
            break;
        case UnionType:
            if (type1 -> listLen == 0 || type2 -> listLen == 0)
                throwSemanticError(type1, "data type mismatched");
            break;
        case FloatType:
            /*if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            else if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(type1, "data type mismatched");
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = parent -> ptrs[1];
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }*/
            throwSemanticError(type1, "data type mismatched");
            break;
        case DoubleType:
            if (type1 -> listLen == 0 && type2 -> listLen != 0)
                throwSemanticError(type1, "data type mismatched");
            else if (type1 -> listLen != 0 && type2 -> listLen == 0)
                throwSemanticError(type1, "data type mismatched");
            break;
        default:
            break;
    }
}
