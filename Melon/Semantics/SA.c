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

void funcall(ASTNode * node);

ASTNode * exprCheck(ASTNode * node);

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
        case Funcall:
            funcall(node);
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

void definedFunc(ASTNode * node) {
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
    return NULL;
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
