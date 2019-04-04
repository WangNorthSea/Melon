//
//  SA.c
//  Melon
//
//  Created by 王浩宇 on 2019/3/28.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "../ASTNode/node.h"
#include "../SymbolTable/hashtable.h"
#include "../SymbolTable/scope.h"
#include "../ASTNode/constructor.h"

void iterator(ASTNode * node);

Scope * scope;

const char * fileChecking = NULL;

void definedFunc(ASTNode * node);

void definedStruct(ASTNode * node);

void definedUnion(ASTNode * node);

void normalParam(ASTNode * node);

void constParam(ASTNode * node);

void funcPtrParam(ASTNode * node);

void funcPtr(ASTNode * node);

void variable(ASTNode * node);

void constFuncPtr(ASTNode * node);

void constVariable(ASTNode * node);

void funcStmt(ASTNode * node);

void externFunc(ASTNode * node);

void externVar(ASTNode * node);

void externConst(ASTNode * node);

void funcall(ASTNode * node);

ASTNode * exprCheck(ASTNode * node);

void typeChecker(ASTNode * parent, ASTNode * type1, ASTNode * type2);

void newScope(void);

void integerLiteralType(ASTNode * parent, ASTNode * type1, ASTNode * type2);

void characterLiteralType(ASTNode * parent, ASTNode * type1, ASTNode * type2);

void stringLiteralType(ASTNode * parent, ASTNode * type1, ASTNode * type2);

void floatLiteralType(ASTNode * parent, ASTNode * type1, ASTNode * type2);

void addressType(ASTNode * parent, ASTNode * type1, ASTNode * type2);

void voidType(ASTNode * parent, ASTNode * type1, ASTNode * type2);

void charType(ASTNode * parent, ASTNode * type1, ASTNode * type2);

void shortIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2);

void throwSemanticError(const char * file, int line, char * content);

void semanticAnalyze(ASTNode * root, const char * file) {
    fileChecking = file;
    scope = ScopeConstructor(NULL);
    iterator(root);
}

void iterator(ASTNode * node) {
    int i = 0, needRollBackScope = 0;
    
    switch (node -> kind) {
        case DefinedFunc:
            needRollBackScope = 1;
            definedFunc(node);
            break;
        case NormalParam:
            normalParam(node);
            break;
        case ConstParam:
            constParam(node);
            break;
        case FuncPtrParam:
            funcPtrParam(node);
            break;
        case DefinedStruct:
            needRollBackScope = 1;
            definedStruct(node);
            break;
        case DefinedUnion:
            needRollBackScope = 1;
            definedUnion(node);
            break;
        case FuncPtr:
            funcPtr(node);
            break;
        case Variable:
            variable(node);
            break;
        case ConstFuncPtr:
            constFuncPtr(node);
            break;
        case ConstVariable:
            constVariable(node);
            break;
        case Block:
            needRollBackScope = 1;
            newScope();
            break;
        case FuncStmt:
            funcStmt(node);
            break;
        case ExternFunc:
            externFunc(node);
            break;
        case ExternVar:
            externVar(node);
            break;
        case ExternConst:
            externConst(node);
            break;
        case Funcall:
            //funcall(node);
            break;
    }
    
    for (i = 0; i < 6; i++) {
        if (node -> ptrs[i] != NULL)
            iterator(node -> ptrs[i]);
    }
    
    for (i = 0; i < node -> listLen; i++)
        iterator(node -> list + i);
    
    if (needRollBackScope)
        scope = scope -> upperLevel;
}

void definedFunc(ASTNode * node) {      //返回值type为ptrs[1]
    if (scope -> lookup(scope, node -> ptrs[2] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "function redefined");
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[2] -> image, node);
    newScope();
}

void definedStruct(ASTNode * node) {
    if (scope -> lookup(scope, node -> ptrs[0] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "struct redefined");
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[0] -> image, node);
    newScope();
}

void definedUnion(ASTNode * node) {
    if (scope -> lookup(scope, node -> ptrs[0] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "union redefined");
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[0] -> image, node);
    newScope();
}

void normalParam(ASTNode * node) {
    if (scope -> lookup(scope, node -> ptrs[1] -> ptrs[0] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "parameter redefined");
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[1] -> ptrs[0] -> image, node);
}

void constParam(ASTNode * node) {
    if (scope -> lookup(scope, node -> ptrs[1] -> ptrs[0] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "parameter redefined");
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[1] -> ptrs[0] -> image, node);
}

void funcPtrParam(ASTNode * node) {
    if (scope -> lookup(scope, node -> ptrs[1] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "parameter redefined");
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[1] -> image, node);
}

void funcPtr(ASTNode * node) {
    int name = 2;
    if (node -> ptrs[1] -> kind == Name)
        name = 1;
    
    if (scope -> lookup(scope, node -> ptrs[name] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "function pointer redefined");
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[name] -> image, node);
}

void variable(ASTNode * node) {
    int varname = 2;
    if (node -> ptrs[1] -> kind == Varname)
        varname = 1;
    
    if (scope -> lookup(scope, node -> ptrs[varname] -> ptrs[0] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "variable redefined");
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[varname] -> ptrs[0] -> image, node);
}

void constFuncPtr(ASTNode * node) {
    int name = 2;
    if (node -> ptrs[1] -> kind == Name)
        name = 1;
    
    if (scope -> lookup(scope, node -> ptrs[name] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "const function pointer redefined");
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[name] -> image, node);
}

void constVariable(ASTNode * node) {
    int varname = 2;
    if (node -> ptrs[1] -> kind == Varname)
        varname = 1;
    
    if (scope -> lookup(scope, node -> ptrs[varname] -> ptrs[0] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "const variable redefined");
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[varname] -> ptrs[0] -> image, node);
}

void funcStmt(ASTNode * node) {
    if (scope -> lookup(scope, node -> ptrs[2] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "function redefined");
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[2] -> image, node);
}

void externFunc(ASTNode * node) {
    if (scope -> lookup(scope, node -> ptrs[0] -> ptrs[2] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "function redefined");
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[0] -> ptrs[2] -> image, node);
}

void externVar(ASTNode * node) {
    if (node -> ptrs[0] -> list[0].kind == Variable)
        variable(&(node -> ptrs[0] -> list[0]));
    else
        funcPtr(&(node -> ptrs[0] -> list[0]));
}

void externConst(ASTNode * node) {
    if (node -> ptrs[0] -> list[0].kind == ConstVariable)
        constVariable(&(node -> ptrs[0] -> list[0]));
    else
        constFuncPtr(&(node -> ptrs[0] -> list[0]));
}

/*void funcall(ASTNode * node) {
    int params = 3;
    ASTNode * targetFunc = scope -> lookup(scope, node -> ptrs[0] -> image);
    if (targetFunc == NULL || (targetFunc -> kind != DefinedFunc && targetFunc -> kind != FuncStmt && targetFunc -> kind != FuncPtr && targetFunc -> kind != ConstFuncPtr && targetFunc -> kind != ExternFunc))
        throwSemanticError(fileChecking, node -> line, "function undefined");
    
    if (targetFunc -> ptrs[2] -> kind == ParamsNode)
        params = 2;
    
    if (targetFunc -> ptrs[params] -> listLen > 0) {
        if (targetFunc -> ptrs[params] -> list[targetFunc -> ptrs[params] -> listLen - 1].kind == UnlimitedParams) {
            
        }
        else {
            int targetArgs = targetFunc -> ptrs[params] -> listLen;
            int nodeArgs = node -> ptrs[1] -> listLen;
            if (targetArgs > nodeArgs) {
                printf("Melon: %s: semantic \033[31merror\033[0m in line %d too few arguments, expected %d got %d\n", fileChecking, node -> line, targetArgs, nodeArgs);
                exit(-1);
            }
            else if (targetArgs < nodeArgs) {
                printf("Melon: %s: semantic \033[31merror\033[0m in line %d too many arguments, expected %d got %d\n", fileChecking, node -> line, targetArgs, nodeArgs);
                exit(-1);
            }
            else {
                int i;
                ASTNode * 
                for (i = 0; i < targetArgs; i++) {
                    if (targetFunc -> ptrs[params] -> list[i].kind != node -> ptrs[1] -> list[i].kind)
                        throwSemanticError(fileChecking, node -> line, "argument type mismatched");
                }
            }
        }
    }
    else {
        
    }
}*/

//从表达式节点出发，检查表达式各部分的类型，进行隐式类型转换，并返回表达式最终得到的类型，基本上是语义分析的核心
ASTNode * exprCheck(ASTNode * node) {
    if (node -> kind == Identifier) {
        ASTNode * target = scope -> lookup(scope, node -> image);       //并不是type
        if (target == NULL) {
            printf("Melon: %s: semantic \033[31merror\033[0m in line %d identifier \'%s\' undefined\n", fileChecking, node -> line, node -> image);
            exit(-1);
        }
        
        switch (target -> kind) {
            case DefinedFunc:
                return target -> ptrs[1];
                break;
            case DefinedStruct:
                return target;
                break;
            case DefinedUnion:
                return target;
                break;
            case NormalParam:
                return target -> ptrs[0];
                break;
            case ConstParam:
                return target -> ptrs[0];
                break;
            case FuncPtrParam:
                return target -> ptrs[0];
                break;
            case FuncPtr:
                if (target -> ptrs[0] == NULL || target -> ptrs[0] -> kind == Static)
                    return target -> ptrs[1];
                else
                    return target -> ptrs[0];
                break;
            case ConstFuncPtr:
                if (target -> ptrs[0] == NULL || target -> ptrs[0] -> kind == Static)
                    return target -> ptrs[1];
                else
                    return target -> ptrs[0];
                break;
            case Variable:
                if (target -> ptrs[0] == NULL || target -> ptrs[0] -> kind == Static)
                    return target -> ptrs[1];
                else
                    return target -> ptrs[0];
                break;
            case ConstVariable:
                if (target -> ptrs[0] == NULL || target -> ptrs[0] -> kind == Static)
                    return target -> ptrs[1];
                else
                    return target -> ptrs[0];
                break;
            case FuncStmt:
                return target -> ptrs[1];
                break;
            case ExternFunc:
                return target -> ptrs[0] -> ptrs[1];
                break;
            case ExternVar:
                return target -> ptrs[0] -> ptrs[1];
                break;
            case ExternConst:
                return target -> ptrs[0] -> ptrs[1];
                break;
            default:
                break;
        }
    }
    else if (node -> kind == IntegerLiteral || node -> kind == CharacterLiteral || node -> kind == StringLiteral || node -> kind == FloatLiteral)
        return node;
    else if (node -> kind == BinaryOp) {
        ASTNode * type1 = exprCheck(node -> ptrs[0]);
        ASTNode * type2 = exprCheck(node -> ptrs[1]);
        
        typeChecker(node, type1, type2);
    }
    
    return NULL;
}

void typeChecker(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    switch (type1 -> kind) {
        case IntegerLiteral:
            integerLiteralType(parent, type1, type2);
            break;
        case CharacterLiteral:
            characterLiteralType(parent, type1, type2);
            break;
        case StringLiteral:
            stringLiteralType(parent, type1, type2);
            break;
        case FloatLiteral:
            floatLiteralType(parent, type1, type2);
            break;
        case SizeofType:
            integerLiteralType(parent, type1, type2);
            break;
        case SizeofExpr:
            integerLiteralType(parent, type1, type2);
            break;
        case Address:
            addressType(parent, type1, type2);
            break;
        case VoidType:
            voidType(parent, type1, type2);
            break;
        case CharType:
            charType(parent, type1, type2);
            break;
        case ShortIntType:
            shortIntType(parent, type1, type2);
            break;
    }
}

void newScope(void) {
    Scope * temp = ScopeConstructor(scope);
    scope -> appendScope(scope, temp);
    scope = temp;
}

//字面量自身不需要进行隐式类型转换，需不需要转换的关键在于是否将其存储在了固定长度的空间中

void integerLiteralType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
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
            break;
        case SizeofType:
            break;
        case SizeofExpr:
            break;
        case Address:
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        case CharType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            break;
        case LongIntType:
            break;
        case UnsignedCharType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            break;
        case UnsignedLongIntType:
            break;
        case StructType:
            throwSemanticError(fileChecking, type1 -> line, "struct type operand not allowed");
            break;
        case UnionType:
            throwSemanticError(fileChecking, type1 -> line, "union type operand not allowed");
            break;
        case FloatType:
            break;
        case DoubleType:
            break;
        default:
            break;
    }
}

void characterLiteralType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    switch (type2 -> kind) {
        case IntegerLiteral:
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case FloatLiteral:
            break;
        case SizeofType:
            break;
        case SizeofExpr:
            break;
        case Address:
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
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
            throwSemanticError(fileChecking, type1 -> line, "struct type operand not allowed");
            break;
        case UnionType:
            throwSemanticError(fileChecking, type1 -> line, "union type operand not allowed");
            break;
        case FloatType:
            break;
        case DoubleType:
            break;
        default:
            break;
    }
}

void stringLiteralType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    switch (type2 -> kind) {
        case IntegerLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case CharacterLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type1 -> line, "string literal operand not allowed");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case SizeofType:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case SizeofExpr:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case Address:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        case CharType:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case ShortIntType:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case IntType:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case LongIntType:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case UnsignedCharType:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case UnsignedShortIntType:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case UnsignedIntType:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case UnsignedLongIntType:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case StructType:
            throwSemanticError(fileChecking, type1 -> line, "struct type operand not allowed");
            break;
        case UnionType:
            throwSemanticError(fileChecking, type1 -> line, "union type operand not allowed");
            break;
        case FloatType:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case DoubleType:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
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
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case FloatLiteral:
            break;
        case SizeofType:
            break;
        case SizeofExpr:
            break;
        case Address:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        case CharType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case ShortIntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case IntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case LongIntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case UnsignedCharType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case UnsignedShortIntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case UnsignedIntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case UnsignedLongIntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case StructType:
            throwSemanticError(fileChecking, type1 -> line, "struct type operand not allowed");
            break;
        case UnionType:
            throwSemanticError(fileChecking, type1 -> line, "union type operand not allowed");
            break;
        case FloatType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case DoubleType:
            if (type2 -> listLen != 0)
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        default:
            break;
    }
}

void addressType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
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
            break;
        case SizeofType:
            break;
        case SizeofExpr:
            break;
        case Address:
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        case CharType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedCharType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            break;
        case StructType:
            throwSemanticError(fileChecking, type1 -> line, "struct type operand not allowed");
            break;
        case UnionType:
            throwSemanticError(fileChecking, type1 -> line, "union type operand not allowed");
            break;
        case FloatType:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case DoubleType:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        default:
            break;
    }
}

void voidType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    switch (type2 -> kind) {
        case IntegerLiteral:
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        case CharacterLiteral:
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        case SizeofType:
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        case SizeofExpr:
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        case Address:
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        case CharType:
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        case ShortIntType:
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        case IntType:
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        case LongIntType:
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        case UnsignedCharType:
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        case UnsignedShortIntType:
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        case UnsignedIntType:
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        case UnsignedLongIntType:
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        case StructType:
            throwSemanticError(fileChecking, type1 -> line, "struct type operand not allowed");
            break;
        case UnionType:
            throwSemanticError(fileChecking, type1 -> line, "union type operand not allowed");
            break;
        case FloatType:
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        case DoubleType:
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        default:
            break;
    }
}

void charType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (type2 -> kind) {
        case IntegerLiteral:
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case FloatLiteral:
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else
                throwSemanticError(fileChecking, type1 -> line, "data type mismatched");
            break;
        case SizeofType:
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case SizeofExpr:
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case Address:
            if (type1 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case VoidType:                                                                        //注意指针(带有PtrRef)的问题，字面量可以但特定类型不一定行
            throwSemanticError(fileChecking, type1 -> line, "void type operand not allowed");
            break;
        case CharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case ShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(ShortIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case IntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(IntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case LongIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(LongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnsignedCharType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(CharType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            break;
        case UnsignedShortIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedShortIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnsignedIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case UnsignedLongIntType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case StructType:
            throwSemanticError(fileChecking, type1 -> line, "struct type operand not allowed");
            break;
        case UnionType:
            throwSemanticError(fileChecking, type1 -> line, "union type operand not allowed");
            break;
        case FloatType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(FloatType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        case DoubleType:
            if (type1 -> listLen != 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type2 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type2;
                parent -> ptrs[1] = NodeConstructor(Cast, fileChecking, type2 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen != 0) {
                temp = NodeConstructor(UnsignedLongIntType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            else if (type1 -> listLen == 0 && type2 -> listLen == 0) {
                temp = NodeConstructor(DoubleType, fileChecking, type1 -> line, NULL, ptrs);           //隐式类型转换
                ptrs[0] = temp;
                ptrs[1] = type1;
                parent -> ptrs[0] = NodeConstructor(Cast, fileChecking, type1 -> line, NULL, ptrs);
            }
            break;
        default:
            break;
    }
}

void shortIntType(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    
}

void throwSemanticError(const char * file, int line, char * content) {
    printf("Melon: %s: semantic \033[31merror\033[0m in line %d %s\n", file, line, content);
    exit(-1);
}
