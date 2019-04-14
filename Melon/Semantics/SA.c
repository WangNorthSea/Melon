//
//  SA.c
//  Melon
//
//  Created by 王浩宇 on 2019/3/28.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Lexer/tokenkind.h"
#include "../ASTNode/node.h"
#include "../SymbolTable/hashtable.h"
#include "../SymbolTable/scope.h"
#include "../ASTNode/constructor.h"
#include "binoptc.h"
#include "opassigntc.h"
#include "assigntc.h"

void iterator(ASTNode * node);

Scope * scope;

const char * fileChecking = NULL;

int refCount = 0;

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

void SuffixOpTypeChecker(ASTNode * type);

void ArrayRefTypeChecker(ASTNode * type);

void OpAssignTypeChecker(ASTNode * parent, ASTNode * type1, ASTNode * type2);

void AssignTypeChecker(ASTNode * parent, ASTNode * type1, ASTNode * type2);

void BinaryOpTypeChecker(ASTNode * parent, ASTNode * type1, ASTNode * type2);

void UnaryOpTypeChecker(ASTNode * type, int kind);

void newScope(void);

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
    int i;
    int type = 0;
    int varname = 2;
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (node -> ptrs[1] -> kind == Varname)
        varname = 1;
    
    if (node -> ptrs[0] == NULL)
        type = 1;
    else if (node -> ptrs[0] -> kind == Static)
        type = 1;
    
    for (i = 1; i <= node -> ptrs[varname] -> listLen; i++) {
        temp = NodeConstructor(PtrRef, fileChecking, node -> line, NULL, ptrs);
        node -> ptrs[varname] -> append(node -> ptrs[type], *temp);
        free(temp);
    }
    
    if (node -> ptrs[type] -> kind == VoidType) {
        if (node -> ptrs[type] -> listLen == 0)
            throwSemanticError(fileChecking, node -> line, "void type variable not allowed");
    }
    
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
    int i;
    int type = 0;
    int varname = 2;
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (node -> ptrs[1] -> kind == Varname)
        varname = 1;
    
    if (node -> ptrs[0] == NULL)
        type = 1;
    else if (node -> ptrs[0] -> kind == Static)
        type = 1;
    
    for (i = 1; i <= node -> ptrs[varname] -> listLen; i++) {
        temp = NodeConstructor(PtrRef, fileChecking, node -> line, NULL, ptrs);
        node -> ptrs[varname] -> append(node -> ptrs[type], *temp);
        free(temp);
    }
    
    if (node -> ptrs[type] -> kind == VoidType) {
        if (node -> ptrs[type] -> listLen == 0)
            throwSemanticError(fileChecking, node -> line, "void type const variable not allowed");
    }
    
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
    else if (node -> kind == IntegerLiteral || node -> kind == CharacterLiteral || node -> kind == StringLiteral || node -> kind == FloatLiteral || node -> kind == SizeofType || node -> kind == SizeofExpr)
        return node;
    else if (node -> kind == BinaryOp) {
        ASTNode * type1 = exprCheck(node -> ptrs[0]);
        ASTNode * type2 = exprCheck(node -> ptrs[1]);
        
        BinaryOpTypeChecker(node, type1, type2);
        
        if (type1 -> kind == Cast)
            return type2;
        else if (type2 -> kind == Cast)
            return type1;
        else if (type1 -> kind == IntegerLiteral || type1 -> kind == CharacterLiteral || type1 -> kind == StringLiteral || type1 -> kind == FloatLiteral)
            return type2;
        else
            return type1;
    }
    else if (node -> kind == SuffixOp) {
        ASTNode * type = exprCheck(node -> ptrs[0]);
        
        SuffixOpTypeChecker(type);
        
        return type;
    }
    else if (node -> kind == ArrayRef) {
        refCount++;
        ASTNode * refType = exprCheck(node -> ptrs[0]);
        
        if (refCount > refType -> listLen)
            throwSemanticError(fileChecking, node -> line, "too many times array reference");
        refCount = 0;
        
        ASTNode * exprType = exprCheck(node -> ptrs[1]);
        
        ArrayRefTypeChecker(exprType);
        
        refType -> listLen--;
        return refType;
    }
    else if (node -> kind == OpAssign) {
        ASTNode * type1 = exprCheck(node -> ptrs[0]);
        ASTNode * type2 = exprCheck(node -> ptrs[2]);
        
        OpAssignTypeChecker(node, type1, type2);
        
        return type1;
    }
    else if (node -> kind == Assign) {
        ASTNode * type1 = exprCheck(node -> ptrs[0]);
        ASTNode * type2 = exprCheck(node -> ptrs[1]);
        
        AssignTypeChecker(node, type1, type2);
        
        return type1;
    }
    else if (node -> kind == CondExpr) {
        ASTNode * type1 = exprCheck(node -> ptrs[0]);
        type1 = exprCheck(node -> ptrs[1]);
        ASTNode * type2 = exprCheck(node -> ptrs[2]);
        
        if (type1 -> kind == Cast)
            return type2;
        else if (type2 -> kind == Cast)
            return type1;
        else if (type1 -> kind == IntegerLiteral || type1 -> kind == CharacterLiteral || type1 -> kind == StringLiteral || type1 -> kind == FloatLiteral)
            return type2;
        else
            return type1;
    }
    else if (node -> kind == LogicOr) {
        ASTNode * type1 = exprCheck(node -> ptrs[0]);
        ASTNode * type2 = exprCheck(node -> ptrs[1]);
        
        if (type1 -> kind == Cast)
            return type2;
        else if (type2 -> kind == Cast)
            return type1;
        else if (type1 -> kind == IntegerLiteral || type1 -> kind == CharacterLiteral || type1 -> kind == StringLiteral || type1 -> kind == FloatLiteral)
            return type2;
        else
            return type1;
    }
    else if (node -> kind == LogicAnd) {
        ASTNode * type1 = exprCheck(node -> ptrs[0]);
        ASTNode * type2 = exprCheck(node -> ptrs[1]);
        
        if (type1 -> kind == Cast)
            return type2;
        else if (type2 -> kind == Cast)
            return type1;
        else if (type1 -> kind == IntegerLiteral || type1 -> kind == CharacterLiteral || type1 -> kind == StringLiteral || type1 -> kind == FloatLiteral)
            return type2;
        else
            return type1;
    }
    else if (node -> kind == PrefixOp) {
        ASTNode * type = exprCheck(node -> ptrs[0]);
        
        SuffixOpTypeChecker(type);           //似乎和后缀运算符没有区别
        
        return type;
    }
    else if (node -> kind == UnaryOp) {
        ASTNode * type = exprCheck(node -> ptrs[0]);
        
        if (!strcmp(node -> image, "!"))
            UnaryOpTypeChecker(type, LOGICNOT);
        else
            UnaryOpTypeChecker(type, NOT);
        
        return type;
    }
    else if (node -> kind == Dereference) {
        refCount++;
        ASTNode * type = exprCheck(node -> ptrs[0]);
        
        if (refCount > type -> listLen)
            throwSemanticError(fileChecking, node -> line, "too many times dereference");
        refCount = 0;
        
        type -> listLen--;
        return type;
    }
    else if (node -> kind == Address) {      //可能需要改动BinaryOpTypeChecker相关代码以及BinaryOp分支
        
    }
    else if (node -> kind == SizeofType) {
        
    }
    else if (node -> kind == SizeofExpr) {
        
    }
    else if (node -> kind == Member) {
        
    }
    else if (node -> kind == PtrMember) {
        
    }
    else if (node -> kind == Cast) {
        
    }
    
    return NULL;
}

void SuffixOpTypeChecker(ASTNode * type) {
    switch (type -> kind) {
        case IntegerLiteral:
            throwSemanticError(fileChecking, type -> line, "integer literal is not assignable");
            break;
        case CharacterLiteral:
            throwSemanticError(fileChecking, type -> line, "character literal is not assignable");
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type -> line, "string literal is not assignable");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type -> line, "float literal is not assignable");
            break;
        case VoidType:
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
            if (type -> listLen == 0)
                throwSemanticError(fileChecking, type -> line, "struct type not allowed");
            break;
        case UnionType:
            if (type -> listLen == 0)
                throwSemanticError(fileChecking, type -> line, "union type not allowed");
            break;
        case FloatType:
            break;
        case DoubleType:
            break;
        default:
            break;
    }
}

void ArrayRefTypeChecker(ASTNode * type) {
    switch (type -> kind) {
        case IntegerLiteral:
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type -> line, "array subscript is not an integer");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type -> line, "array subscript is not an integer");
            break;
        case VoidType:
            throwSemanticError(fileChecking, type -> line, "array subscript is not an integer");
            break;
        case CharType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "array subscript is not an integer");
            break;
        case ShortIntType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "array subscript is not an integer");
            break;
        case IntType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "array subscript is not an integer");
            break;
        case LongIntType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "array subscript is not an integer");
            break;
        case UnsignedCharType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "array subscript is not an integer");
            break;
        case UnsignedShortIntType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "array subscript is not an integer");
            break;
        case UnsignedIntType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "array subscript is not an integer");
            break;
        case UnsignedLongIntType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "array subscript is not an integer");
            break;
        case StructType:
            throwSemanticError(fileChecking, type -> line, "array subscript is not an integer");
            break;
        case UnionType:
            throwSemanticError(fileChecking, type -> line, "array subscript is not an integer");
            break;
        case FloatType:
            throwSemanticError(fileChecking, type -> line, "array subscript is not an integer");
            break;
        case DoubleType:
            throwSemanticError(fileChecking, type -> line, "array subscript is not an integer");
            break;
        default:
            break;
    }
}

void OpAssignTypeChecker(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    switch (type1 -> kind) {
        case IntegerLiteral:
            throwSemanticError(fileChecking, type1 -> line, "expression is not assignable");
            break;
        case CharacterLiteral:
            throwSemanticError(fileChecking, type1 -> line, "expression is not assignable");
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type1 -> line, "expression is not assignable");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type1 -> line, "expression is not assignable");
            break;
        case VoidType:
            opAssignVoidType(parent, type1, type2);
            break;
        case CharType:
            opAssignCharType(parent, type1, type2);
            break;
        case ShortIntType:
            opAssignShortIntType(parent, type1, type2);
            break;
        case IntType:
            opAssignIntType(parent, type1, type2);
            break;
        case LongIntType:
            opAssignLongIntType(parent, type1, type2);
            break;
        case UnsignedCharType:
            opAssignUnsignedCharType(parent, type1, type2);
            break;
        case UnsignedShortIntType:
            opAssignUnsignedShortIntType(parent, type1, type2);
            break;
        case UnsignedIntType:
            opAssignUnsignedIntType(parent, type1, type2);
            break;
        case UnsignedLongIntType:
            opAssignUnsignedLongIntType(parent, type1, type2);
            break;
        case StructType:
            opAssignStructType(parent, type1, type2);
            break;
        case UnionType:
            opAssignUnionType(parent, type1, type2);
            break;
        case FloatType:
            opAssignFloatType(parent, type1, type2);
            break;
        case DoubleType:
            opAssignDoubleType(parent, type1, type2);
            break;
        default:
            break;
    }
}

void AssignTypeChecker(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    switch (type1 -> kind) {
        case IntegerLiteral:
            throwSemanticError(fileChecking, type1 -> line, "expression is not assignable");
            break;
        case CharacterLiteral:
            throwSemanticError(fileChecking, type1 -> line, "expression is not assignable");
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type1 -> line, "expression is not assignable");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type1 -> line, "expression is not assignable");
            break;
        case VoidType:
            assignVoidType(parent, type1, type2);
            break;
        case CharType:
            assignCharType(parent, type1, type2);
            break;
        case ShortIntType:
            assignShortIntType(parent, type1, type2);
            break;
        case IntType:
            assignIntType(parent, type1, type2);
            break;
        case LongIntType:
            assignLongIntType(parent, type1, type2);
            break;
        case UnsignedCharType:
            assignUnsignedCharType(parent, type1, type2);
            break;
        case UnsignedShortIntType:
            assignUnsignedShortIntType(parent, type1, type2);
            break;
        case UnsignedIntType:
            assignUnsignedIntType(parent, type1, type2);
            break;
        case UnsignedLongIntType:
            assignUnsignedLongIntType(parent, type1, type2);
            break;
        case StructType:
            assignStructType(parent, type1, type2);
            break;
        case UnionType:
            assignUnionType(parent, type1, type2);
            break;
        case FloatType:
            assignFloatType(parent, type1, type2);
            break;
        case DoubleType:
            assignDoubleType(parent, type1, type2);
            break;
        default:
            break;
    }
}

void BinaryOpTypeChecker(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    switch (type1 -> kind) {
        case IntegerLiteral:
            integerLiteralType(parent, type1, type2);
            break;
        case CharacterLiteral:
            integerLiteralType(parent, type1, type2);
            break;
        case StringLiteral:
            stringLiteralType(parent, type1, type2);
            break;
        case FloatLiteral:
            floatLiteralType(parent, type1, type2);
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
        case IntType:
            intType(parent, type1, type2);
            break;
        case LongIntType:
            longIntType(parent, type1, type2);
            break;
        case UnsignedCharType:
            unsignedCharType(parent, type1, type2);
            break;
        case UnsignedShortIntType:
            unsignedShortIntType(parent, type1, type2);
            break;
        case UnsignedIntType:
            unsignedIntType(parent, type1, type2);
            break;
        case UnsignedLongIntType:
            unsignedLongIntType(parent, type1, type2);
            break;
        case StructType:
            structType(parent, type1, type2);
            break;
        case UnionType:
            unionType(parent, type1, type2);
            break;
        case FloatType:
            floatType(parent, type1, type2);
            break;
        case DoubleType:
            doubleType(parent, type1, type2);
            break;
        default:
            break;
    }
}

void UnaryOpTypeChecker(ASTNode * type, int kind) {
    switch (type -> kind) {
        case IntegerLiteral:
            break;
        case CharacterLiteral:
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type -> line, "string literal operand not allowed");
            break;
        case FloatLiteral:
            if (kind == NOT)
                throwSemanticError(fileChecking, type -> line, "float literal operand not allowed");
            break;
        case VoidType:
            if (kind == NOT)
                throwSemanticError(fileChecking, type -> line, "pointer operand not allowed");
            break;
        case CharType:
            if (type -> listLen != 0) {
                if (kind == NOT)
                    throwSemanticError(fileChecking, type -> line, "pointer operand not allowed");
            }
            break;
        case ShortIntType:
            if (type -> listLen != 0) {
                if (kind == NOT)
                    throwSemanticError(fileChecking, type -> line, "pointer operand not allowed");
            }
            break;
        case IntType:
            if (type -> listLen != 0) {
                if (kind == NOT)
                    throwSemanticError(fileChecking, type -> line, "pointer operand not allowed");
            }
            break;
        case LongIntType:
            if (type -> listLen != 0) {
                if (kind == NOT)
                    throwSemanticError(fileChecking, type -> line, "pointer operand not allowed");
            }
            break;
        case UnsignedCharType:
            if (type -> listLen != 0) {
                if (kind == NOT)
                    throwSemanticError(fileChecking, type -> line, "pointer operand not allowed");
            }
            break;
        case UnsignedShortIntType:
            if (type -> listLen != 0) {
                if (kind == NOT)
                    throwSemanticError(fileChecking, type -> line, "pointer operand not allowed");
            }
            break;
        case UnsignedIntType:
            if (type -> listLen != 0) {
                if (kind == NOT)
                    throwSemanticError(fileChecking, type -> line, "pointer operand not allowed");
            }
            break;
        case UnsignedLongIntType:
            if (type -> listLen != 0) {
                if (kind == NOT)
                    throwSemanticError(fileChecking, type -> line, "pointer operand not allowed");
            }
            break;
        case StructType:
            if (type -> listLen == 0)
                throwSemanticError(fileChecking, type -> line, "struct type operand not allowed");
            else {
                if (kind == NOT)
                    throwSemanticError(fileChecking, type -> line, "pointer operand not allowed");
            }
            break;
        case UnionType:
            if (type -> listLen == 0)
                throwSemanticError(fileChecking, type -> line, "union type operand not allowed");
            else {
                if (kind == NOT)
                    throwSemanticError(fileChecking, type -> line, "pointer operand not allowed");
            }
            break;
        case FloatType:
            if (kind == NOT)
                throwSemanticError(fileChecking, type -> line, "float type operand not allowed");
            break;
        case DoubleType:
            if (kind == NOT)
                throwSemanticError(fileChecking, type -> line, "double type operand not allowed");
            break;
        default:
            break;
    }
}

void newScope(void) {
    Scope * temp = ScopeConstructor(scope);
    scope -> appendScope(scope, temp);
    scope = temp;
}

void throwSemanticError(const char * file, int line, char * content) {
    printf("Melon: %s: semantic \033[31merror\033[0m in line %d %s\n", file, line, content);
    exit(-1);
}
