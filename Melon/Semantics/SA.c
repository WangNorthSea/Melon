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
#include "graphnode.h"

void iterator(ASTNode * node);

Scope * scope;

Hashtable * StruUnionScope;

GNode * lastStop = NULL;

const char * fileChecking = NULL;

int refCount = 0;

int inStructOrUnion = 0;

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

void blockScope(ASTNode * node);

void funcStmt(ASTNode * node);

void externVar(ASTNode * node);

void externConst(ASTNode * node);

void label(ASTNode * node);

void goto_(ASTNode * node);

ASTNode * exprCheck(ASTNode * node);

void SuffixOpTypeChecker(ASTNode * type);

void ArrayRefTypeChecker(ASTNode * type);

void OpAssignTypeChecker(ASTNode * parent, ASTNode * type1, ASTNode * type2);

void AssignTypeChecker(ASTNode * parent, ASTNode * type1, ASTNode * type2);

void BinaryOpTypeChecker(ASTNode * parent, ASTNode * type1, ASTNode * type2);

void UnaryOpTypeChecker(ASTNode * type, int kind);

void MemberTypeChecker(ASTNode * type);

void PtrMemberTypeChecker(ASTNode * type);

ASTNode * getTargetType(ASTNode * target);

void newScope(void);

void throwSemanticError(const char * file, int line, char * content);

void semanticAnalyze(ASTNode * root, const char * file) {
    fileChecking = file;
    scope = ScopeConstructor(NULL);
    StruUnionScope = HashtableConstructor();
    iterator(root);
}

void iterator(ASTNode * node) {
    int i = 0, needRollBackScope = 0;
    ASTNode * temp = NULL;
    
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
            inStructOrUnion = 1;
            definedStruct(node);
            break;
        case DefinedUnion:
            needRollBackScope = 1;
            inStructOrUnion = 1;
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
            blockScope(node);
            break;
        case FuncStmt:
            funcStmt(node);
            break;
        case ExternFunc:
            funcStmt(node -> ptrs[0]);
            break;
        case ExternVar:
            externVar(node);
            break;
        case ExternConst:
            externConst(node);
            break;
        case Label:
            label(node);
            break;
        case Goto:
            goto_(node);
            break;
        default:
            temp = exprCheck(node);
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
    
    if (node -> kind == DefinedStruct || node -> kind == DefinedUnion) {
        inStructOrUnion = 0;
        if (checkClosure() == 1)
            throwSemanticError(fileChecking, node -> line, "recursively defined");
    }
}

void definedFunc(ASTNode * node) {      //返回值type为ptrs[1]
    if (scope -> localLookup(scope, node -> ptrs[2] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "function redefined");
    
    if (node -> ptrs[1] -> kind == StructType) {
        if (scope -> lookup(scope, node -> ptrs[1] -> image) == NULL)
            throwSemanticError(fileChecking, node -> line, "struct type undefined");
    }
    
    if (node -> ptrs[1] -> kind == UnionType) {
        if (scope -> lookup(scope, node -> ptrs[1] -> image) == NULL)
            throwSemanticError(fileChecking, node -> line, "union type undefined");
    }
    
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[2] -> image, node);
    newScope();
}

void definedStruct(ASTNode * node) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (scope -> localLookup(scope, node -> ptrs[0] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "struct redefined");
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[0] -> image, node);
    newScope();
    StruUnionScope -> put(StruUnionScope, node -> ptrs[0] -> image, scope);
    lastStop = GNodeConstructor(NULL, NodeConstructor(StructType, fileChecking, node -> line, node -> ptrs[0] -> image, ptrs));
}

void definedUnion(ASTNode * node) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (scope -> localLookup(scope, node -> ptrs[0] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "union redefined");
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[0] -> image, node);
    newScope();
    StruUnionScope -> put(StruUnionScope, node -> ptrs[0] -> image, scope);
    lastStop = GNodeConstructor(NULL, NodeConstructor(UnionType, fileChecking, node -> line, node -> ptrs[0] -> image, ptrs));
}

void normalParam(ASTNode * node) {
    if (scope -> localLookup(scope, node -> ptrs[1] -> ptrs[0] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "parameter redefined");
    
    if (node -> ptrs[0] -> kind == StructType) {
        if (scope -> lookup(scope, node -> ptrs[0] -> image) == NULL)
            throwSemanticError(fileChecking, node -> line, "struct type undefined");
    }
    
    if (node -> ptrs[0] -> kind == UnionType) {
        if (scope -> lookup(scope, node -> ptrs[0] -> image) == NULL)
            throwSemanticError(fileChecking, node -> line, "union type undefined");
    }
    
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[1] -> ptrs[0] -> image, node);
}

void constParam(ASTNode * node) {
    if (scope -> localLookup(scope, node -> ptrs[1] -> ptrs[0] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "parameter redefined");
    
    if (node -> ptrs[0] -> kind == StructType) {
        if (scope -> lookup(scope, node -> ptrs[0] -> image) == NULL)
            throwSemanticError(fileChecking, node -> line, "struct type undefined");
    }
    
    if (node -> ptrs[0] -> kind == UnionType) {
        if (scope -> lookup(scope, node -> ptrs[0] -> image) == NULL)
            throwSemanticError(fileChecking, node -> line, "union type undefined");
    }
    
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[1] -> ptrs[0] -> image, node);
}

void funcPtrParam(ASTNode * node) {
    if (scope -> localLookup(scope, node -> ptrs[1] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "parameter redefined");
    
    if (node -> ptrs[0] -> kind == StructType) {
        if (scope -> lookup(scope, node -> ptrs[0] -> image) == NULL)
            throwSemanticError(fileChecking, node -> line, "struct type undefined");
    }
    
    if (node -> ptrs[0] -> kind == UnionType) {
        if (scope -> lookup(scope, node -> ptrs[0] -> image) == NULL)
            throwSemanticError(fileChecking, node -> line, "union type undefined");
    }
    
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[1] -> image, node);
}

void funcPtr(ASTNode * node) {
    int name = 2;
    GNode * tempGnode = NULL;
    if (node -> ptrs[1] -> kind == Name)
        name = 1;
    
    if (node -> ptrs[name - 1] -> kind == StructType) {
        if (scope -> lookup(scope, node -> ptrs[name - 1] -> image) == NULL)
            throwSemanticError(fileChecking, node -> line, "struct type undefined");
    }
    
    if (node -> ptrs[name - 1] -> kind == UnionType) {
        if (scope -> lookup(scope, node -> ptrs[name - 1] -> image) == NULL)
            throwSemanticError(fileChecking, node -> line, "union type undefined");
    }
    
    if (inStructOrUnion == 1)
        tempGnode = GNodeConstructor(lastStop, node -> ptrs[name - 1]);
    
    if (scope -> localLookup(scope, node -> ptrs[name] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "function pointer redefined");
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[name] -> image, node);
}

void variable(ASTNode * node) {
    int type = 0;
    int varname = 2;
    GNode * tempGnode = NULL;
    
    if (node -> ptrs[1] -> kind == Varname)
        varname = 1;
    
    if (node -> ptrs[0] == NULL)
        type = 1;
    else if (node -> ptrs[0] -> kind == Static)
        type = 1;
    
    if (node -> ptrs[type] -> kind == VoidType) {
        if (node -> ptrs[type] -> listLen == 0)
            throwSemanticError(fileChecking, node -> line, "void type variable not allowed");
    }
    
    if (node -> ptrs[type] -> kind == StructType) {
        if (scope -> lookup(scope, node -> ptrs[type] -> image) == NULL)
            throwSemanticError(fileChecking, node -> line, "struct type undefined");
    }
    
    if (node -> ptrs[type] -> kind == UnionType) {
        if (scope -> lookup(scope, node -> ptrs[type] -> image) == NULL)
            throwSemanticError(fileChecking, node -> line, "union type undefined");
    }
    
    if (inStructOrUnion == 1)
        tempGnode = GNodeConstructor(lastStop, node -> ptrs[type]);
    
    if (scope -> localLookup(scope, node -> ptrs[varname] -> ptrs[0] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "variable redefined");
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[varname] -> ptrs[0] -> image, node);
}

void constFuncPtr(ASTNode * node) {
    int name = 2;
    GNode * tempGnode = NULL;
    if (node -> ptrs[1] -> kind == Name)
        name = 1;
    
    if (node -> ptrs[name - 1] -> kind == StructType) {
        if (scope -> lookup(scope, node -> ptrs[name - 1] -> image) == NULL)
            throwSemanticError(fileChecking, node -> line, "struct type undefined");
    }
    
    if (node -> ptrs[name - 1] -> kind == UnionType) {
        if (scope -> lookup(scope, node -> ptrs[name - 1] -> image) == NULL)
            throwSemanticError(fileChecking, node -> line, "union type undefined");
    }
    
    if (inStructOrUnion == 1)
        tempGnode = GNodeConstructor(lastStop, node -> ptrs[name - 1]);
    
    if (scope -> localLookup(scope, node -> ptrs[name] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "function pointer redefined");
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[name] -> image, node);
}

void constVariable(ASTNode * node) {
    int type = 0;
    int varname = 2;
    GNode * tempGnode = NULL;
    
    if (node -> ptrs[1] -> kind == Varname)
        varname = 1;
    
    if (node -> ptrs[0] == NULL)
        type = 1;
    else if (node -> ptrs[0] -> kind == Static)
        type = 1;
    
    if (node -> ptrs[type] -> kind == VoidType) {
        if (node -> ptrs[type] -> listLen == 0)
            throwSemanticError(fileChecking, node -> line, "void type const variable not allowed");
    }
    
    if (node -> ptrs[type] -> kind == StructType) {
        if (scope -> lookup(scope, node -> ptrs[type] -> image) == NULL)
            throwSemanticError(fileChecking, node -> line, "struct type undefined");
    }
    
    if (node -> ptrs[type] -> kind == UnionType) {
        if (scope -> lookup(scope, node -> ptrs[type] -> image) == NULL)
            throwSemanticError(fileChecking, node -> line, "union type undefined");
    }
    
    if (inStructOrUnion == 1)
        tempGnode = GNodeConstructor(lastStop, node -> ptrs[type]);
    
    if (scope -> localLookup(scope, node -> ptrs[varname] -> ptrs[0] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "const variable redefined");
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[varname] -> ptrs[0] -> image, node);
}

void blockScope(ASTNode * node) {
    scope -> symbolTable -> put(scope -> symbolTable, "<<Block>>", node);
    newScope();
}

void funcStmt(ASTNode * node) {
    if (scope -> localLookup(scope, node -> ptrs[2] -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "function redefined");
    
    if (node -> ptrs[1] -> kind == StructType) {
        if (scope -> lookup(scope, node -> ptrs[1] -> image) == NULL)
            throwSemanticError(fileChecking, node -> line, "struct type undefined");
    }
    
    if (node -> ptrs[1] -> kind == UnionType) {
        if (scope -> lookup(scope, node -> ptrs[1] -> image) == NULL)
            throwSemanticError(fileChecking, node -> line, "union type undefined");
    }
    
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[2] -> image, node);
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

void label(ASTNode * node) {
    if (scope -> localLookup(scope, node -> image) != NULL)
        throwSemanticError(fileChecking, node -> line, "label relocated");
    scope -> symbolTable -> put(scope -> symbolTable, node -> image, node);
}

void goto_(ASTNode * node) {
    if (scope -> lookup(scope, node -> image) == NULL)
        throwSemanticError(fileChecking, node -> line, "label undefined");
}

//从表达式节点出发，检查表达式各部分的类型，进行隐式类型转换，并返回表达式最终得到的类型，基本上是语义分析的核心
ASTNode * exprCheck(ASTNode * node) {
    if (node -> kind == Identifier) {
        ASTNode * target = scope -> lookup(scope, node -> image);       //并不是type
        if (target == NULL) {
            printf("Melon: %s: semantic \033[31merror\033[0m in line %d identifier \'%s\' undefined\n", fileChecking, node -> line, node -> image);
            exit(-1);
        }
        
        return getTargetType(target);
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
    else if (node -> kind == Address) {
        ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
        ASTNode * temp = NodeConstructor(PtrRef, fileChecking, node -> line, NULL, ptrs);
        
        switch (node -> ptrs[0] -> kind) {
            case Identifier:
            case Member:
            case PtrMember:
            case ArrayRef:
            case Dereference:
                break;
            default:
                throwSemanticError(fileChecking, node -> line, "can not take the address of an rvalue");
                break;
        }
        
        ASTNode * type = exprCheck(node -> ptrs[0]);
        type -> append(type, *temp);
        free(temp);
        
        return type;
    }
    else if (node -> kind == SizeofType) {
        ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
        ASTNode * type = NodeConstructor(IntegerLiteral, fileChecking, node -> line, "4", ptrs);
        
        //返回的值具体应该是多少现在先不管，这里只负责类型的检查
        return type;
    }
    else if (node -> kind == SizeofExpr) {
        ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
        ASTNode * type = exprCheck(node -> ptrs[0]);
        type = NodeConstructor(IntegerLiteral, fileChecking, node -> line, "4", ptrs);
        
        //跟SizeofType一样，这里先不管返回的值具体应该是多少
        return type;
    }
    else if (node -> kind == Member) {
        switch (node -> ptrs[0] -> kind) {
            case Identifier:
            case Member:
            case PtrMember:
            case ArrayRef:
            case Dereference:
                break;
            default:
                throwSemanticError(fileChecking, node -> line, "member reference base type is not a structure or union");
                break;
        }
        
        ASTNode * type = exprCheck(node -> ptrs[0]);
        
        MemberTypeChecker(type);
        
        Scope * localScope = StruUnionScope -> get(StruUnionScope, type -> image);
        ASTNode * member = localScope -> localLookup(localScope, node -> ptrs[1] -> image);
        
        if (member == NULL)
            throwSemanticError(fileChecking, node -> line, "no such member");
        
        return getTargetType(member);
    }
    else if (node -> kind == PtrMember) {
        ASTNode * type = exprCheck(node -> ptrs[0]);
        
        PtrMemberTypeChecker(type);
        
        Scope * localScope = StruUnionScope -> get(StruUnionScope, type -> image);
        ASTNode * member = localScope -> localLookup(localScope, node -> ptrs[1] -> image);
        
        if (member == NULL)
            throwSemanticError(fileChecking, node -> line, "no such member");
        
        return getTargetType(member);
    }
    else if (node -> kind == Cast) {
        return node -> ptrs[0];
    }
    else if (node -> kind == Funcall) {
        ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
        
        if (node -> ptrs[0] -> kind != Identifier)
            throwSemanticError(fileChecking, node -> line, "invalid function call");
        
        int params = 3;
        
        ASTNode * targetFunc = scope -> lookup(scope, node -> ptrs[0] -> image);
        if (targetFunc == NULL || (targetFunc -> kind != DefinedFunc && targetFunc -> kind != FuncStmt && targetFunc -> kind != FuncPtr && targetFunc -> kind != ConstFuncPtr))
            throwSemanticError(fileChecking, node -> line, "function undefined");
        
        if (targetFunc -> ptrs[2] -> kind == ParamsNode)
            params = 2;
        
        if (targetFunc -> ptrs[params] -> listLen > 0) {
            if (targetFunc -> ptrs[params] -> list[targetFunc -> ptrs[params] -> listLen - 1].kind == UnlimitedParams) {
                int i;
                ASTNode * targetType;
                ASTNode * inputType;
                for (i = 0; i < targetFunc -> ptrs[params] -> listLen - 1; i++) {
                    inputType = exprCheck(&(node -> ptrs[1] -> list[i]));
                    targetType = exprCheck(&(targetFunc -> ptrs[params] -> list[i]));
                    
                    if (inputType -> kind != targetType -> kind || inputType -> listLen != targetType -> listLen)
                        throwSemanticError(fileChecking, node -> line, "argument type mismatched");
                }
            }
            else {
                int targetArgs = targetFunc -> ptrs[params] -> listLen;
                int nodeArgs;
                if (node -> ptrs[1] == NULL)
                    nodeArgs = 0;
                else
                    nodeArgs = node -> ptrs[1] -> listLen;
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
                    ASTNode * temp = NULL;
                    ASTNode * targetType;
                    ASTNode * inputType;
                    for (i = 0; i < targetArgs; i++) {
                        inputType = exprCheck(&(node -> ptrs[1] -> list[i]));
                        
                        if (targetFunc -> ptrs[params] -> list[i].ptrs[1] -> kind == Name)
                            temp = NodeConstructor(Identifier, fileChecking, node -> line, targetFunc -> ptrs[params] -> list[i].ptrs[1] -> image, ptrs);
                        else
                            temp = NodeConstructor(Identifier, fileChecking, node -> line, targetFunc -> ptrs[params] -> list[i].ptrs[1] -> ptrs[0] -> image, ptrs);
                        targetType = exprCheck(temp);
                        free(temp);
                        
                        if (inputType -> kind != targetType -> kind || inputType -> listLen != targetType -> listLen)
                            throwSemanticError(fileChecking, node -> line, "argument type mismatched");
                    }
                }
            }
        }
        else {
            if (node -> ptrs[1] != NULL) {
                printf("Melon: %s: semantic \033[31merror\033[0m in line %d too many arguments, expected 0 got %d\n", fileChecking, node -> line, node -> ptrs[1] -> listLen);
                exit(-1);
            }
        }
        
        return getTargetType(targetFunc);
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

void MemberTypeChecker(ASTNode * type) {
    switch (type -> kind) {
        case IntegerLiteral:
            throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            break;
        case CharacterLiteral:
            throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            break;
        case VoidType:
            throwSemanticError(fileChecking, type -> line, "member reference base type is a pointer");
            break;
        case CharType:
            if (type -> listLen != 0)
                    throwSemanticError(fileChecking, type -> line, "member reference base type is a pointer");
            else
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            break;
        case ShortIntType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "member reference base type is a pointer");
            else
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            break;
        case IntType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "member reference base type is a pointer");
            else
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            break;
        case LongIntType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "member reference base type is a pointer");
            else
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            break;
        case UnsignedCharType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "member reference base type is a pointer");
            else
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            break;
        case UnsignedShortIntType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "member reference base type is a pointer");
            else
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            break;
        case UnsignedIntType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "member reference base type is a pointer");
            else
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            break;
        case UnsignedLongIntType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "member reference base type is a pointer");
            else
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            break;
        case StructType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "member reference base type is a pointer");
            break;
        case UnionType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "member reference base type is a pointer");
            break;
        case FloatType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "member reference base type is a pointer");
            else
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            break;
        case DoubleType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "member reference base type is a pointer");
            else
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            break;
        default:
            break;
    }
}

void PtrMemberTypeChecker(ASTNode * type) {
    switch (type -> kind) {
        case IntegerLiteral:
            throwSemanticError(fileChecking, type -> line, "member reference base type is not a pointer");
            break;
        case CharacterLiteral:
            throwSemanticError(fileChecking, type -> line, "member reference base type is not a pointer");
            break;
        case StringLiteral:
            throwSemanticError(fileChecking, type -> line, "member reference base type is not a pointer");
            break;
        case FloatLiteral:
            throwSemanticError(fileChecking, type -> line, "member reference base type is not a pointer");
            break;
        case VoidType:
            throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            break;
        case CharType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            else
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a pointer");
            break;
        case ShortIntType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            else
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a pointer");
            break;
        case IntType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            else
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a pointer");
            break;
        case LongIntType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            else
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a pointer");
            break;
        case UnsignedCharType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            else
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a pointer");
            break;
        case UnsignedShortIntType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            else
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a pointer");
            break;
        case UnsignedIntType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            else
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a pointer");
            break;
        case UnsignedLongIntType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            else
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a pointer");
            break;
        case StructType:
            if (type -> listLen != 0) {
                if (type -> listLen != 1)
                    throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            }
            else
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a pointer");
            break;
        case UnionType:
            if (type -> listLen != 0) {
                if (type -> listLen != 1)
                    throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            }
            else
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a pointer");
            break;
        case FloatType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            else
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a pointer");
            break;
        case DoubleType:
            if (type -> listLen != 0)
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a structure or union");
            else
                throwSemanticError(fileChecking, type -> line, "member reference base type is not a pointer");
            break;
        default:
            break;
    }
}

ASTNode * getTargetType(ASTNode * target) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (target -> kind) {
        case DefinedFunc:
            return target -> ptrs[1];
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
            if (target -> ptrs[0] == NULL || target -> ptrs[0] -> kind == Static) {
                int i;
                ASTNode * temp = NULL;
                ASTNode * new = NodeConstructor(target -> ptrs[1] -> kind, fileChecking, target -> line, target -> ptrs[1] -> image, ptrs);
                
                for (i = 0; i < target -> ptrs[1] -> listLen; i++) {
                    temp = NodeConstructor(PtrRef, fileChecking, target -> line, NULL, ptrs);
                    new -> append(new, *temp);
                    free(temp);
                }
                
                for (i = 0; i < target -> ptrs[2] -> listLen; i++) {
                    temp = NodeConstructor(PtrRef, fileChecking, target -> line, NULL, ptrs);
                    new -> append(new, *temp);
                    free(temp);
                }
                return new;
            }
            else {
                int i;
                ASTNode * temp = NULL;
                ASTNode * new = NodeConstructor(target -> ptrs[0] -> kind, fileChecking, target -> line, target -> ptrs[0] -> image, ptrs);
                
                for (i = 0; i < target -> ptrs[0] -> listLen; i++) {
                    temp = NodeConstructor(PtrRef, fileChecking, target -> line, NULL, ptrs);
                    new -> append(new, *temp);
                    free(temp);
                }
                
                for (i = 0; i < target -> ptrs[1] -> listLen; i++) {
                    temp = NodeConstructor(PtrRef, fileChecking, target -> line, NULL, ptrs);
                    new -> append(new, *temp);
                    free(temp);
                }
                return new;
            }
            break;
        case ConstVariable:
            if (target -> ptrs[0] == NULL || target -> ptrs[0] -> kind == Static) {
                int i;
                ASTNode * temp = NULL;
                ASTNode * new = NodeConstructor(target -> ptrs[1] -> kind, fileChecking, target -> line, target -> ptrs[1] -> image, ptrs);
                
                for (i = 0; i < target -> ptrs[1] -> listLen; i++) {
                    temp = NodeConstructor(PtrRef, fileChecking, target -> line, NULL, ptrs);
                    new -> append(new, *temp);
                    free(temp);
                }
                
                for (i = 0; i < target -> ptrs[2] -> listLen; i++) {
                    temp = NodeConstructor(PtrRef, fileChecking, target -> line, NULL, ptrs);
                    new -> append(new, *temp);
                    free(temp);
                }
                return new;
            }
            else {
                int i;
                ASTNode * temp = NULL;
                ASTNode * new = NodeConstructor(target -> ptrs[0] -> kind, fileChecking, target -> line, target -> ptrs[0] -> image, ptrs);
                
                for (i = 0; i < target -> ptrs[0] -> listLen; i++) {
                    temp = NodeConstructor(PtrRef, fileChecking, target -> line, NULL, ptrs);
                    new -> append(new, *temp);
                    free(temp);
                }
                
                for (i = 0; i < target -> ptrs[1] -> listLen; i++) {
                    temp = NodeConstructor(PtrRef, fileChecking, target -> line, NULL, ptrs);
                    new -> append(new, *temp);
                    free(temp);
                }
                return new;
            }
            break;
        case FuncStmt:
            return target -> ptrs[1];
            break;
        case ExternVar: {
            int i;
            ASTNode * temp = NULL;
            ASTNode * new = NodeConstructor(target -> ptrs[0] -> ptrs[1] -> kind, fileChecking, target -> line, target -> ptrs[0] -> ptrs[1] -> image, ptrs);
            
            for (i = 0; i < target -> ptrs[0] -> ptrs[1] -> listLen; i++) {
                temp = NodeConstructor(PtrRef, fileChecking, target -> line, NULL, ptrs);
                new -> append(new, *temp);
                free(temp);
            }
            
            for (i = 0; i < target -> ptrs[0] -> ptrs[2] -> listLen; i++) {
                temp = NodeConstructor(PtrRef, fileChecking, target -> line, NULL, ptrs);
                new -> append(new, *temp);
                free(temp);
            }
            return new;
        }
            break;
        case ExternConst: {
            int i;
            ASTNode * temp = NULL;
            ASTNode * new = NodeConstructor(target -> ptrs[0] -> ptrs[1] -> kind, fileChecking, target -> line, target -> ptrs[0] -> ptrs[1] -> image, ptrs);
            
            for (i = 0; i < target -> ptrs[0] -> ptrs[1] -> listLen; i++) {
                temp = NodeConstructor(PtrRef, fileChecking, target -> line, NULL, ptrs);
                new -> append(new, *temp);
                free(temp);
            }
            
            for (i = 0; i < target -> ptrs[0] -> ptrs[2] -> listLen; i++) {
                temp = NodeConstructor(PtrRef, fileChecking, target -> line, NULL, ptrs);
                new -> append(new, *temp);
                free(temp);
            }
            return new;
        }
            break;
        default:
            return NULL;
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
