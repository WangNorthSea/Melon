
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
#include "returntc.h"
#include "graphnode.h"
#include "../Report/error.h"

//待解决的问题:
//1.funcPtr(constFuncPtr)的赋值与运算类型检查
//2.const类型的赋值检查，不能在Assign，OpAssign语句出现

void iterator(ASTNode * node);

Scope * scope;

Hashtable * StruUnionScope;

GNode * lastStop = NULL;

const char * fileChecking = NULL;

int refCount = 0;

int inStructOrUnion = 0;

int inFunc = 0;

int inFor = 0;

int inWhile = 0;

int inDoWhile = 0;

int inSwitch = 0;

int returned = 0;

ASTNode * funcReturnType = NULL;

void definedFunc(ASTNode * node);

void definedStruct(ASTNode * node);

void definedUnion(ASTNode * node);

void normalParam(ASTNode * node);

void constParam(ASTNode * node);

void funcPtrParam(ASTNode * node);

void funcPtr(ASTNode * node);

void * variable(ASTNode * node);

void constFuncPtr(ASTNode * node);

void * constVariable(ASTNode * node);

void blockScope(ASTNode * node);

void funcStmt(ASTNode * node);

void externVar(ASTNode * node);

void externConst(ASTNode * node);

void label(ASTNode * node);

void return_(ASTNode * node);

ASTNode * exprCheck(ASTNode * node);

void SuffixOpTypeChecker(ASTNode * type);

void ArrayRefTypeChecker(ASTNode * type);

void OpAssignTypeChecker(ASTNode * parent, ASTNode * type1, ASTNode * type2);

void AssignTypeChecker(ASTNode * parent, ASTNode * type1, ASTNode * type2);

void BinaryOpTypeChecker(ASTNode * parent, ASTNode * type1, ASTNode * type2);

void UnaryOpTypeChecker(ASTNode * type, int kind);

void MemberTypeChecker(ASTNode * type);

void PtrMemberTypeChecker(ASTNode * type);

void ReturnTypeChecker(ASTNode * parent, ASTNode * type1, ASTNode * type2);

ASTNode * getTargetType(ASTNode * target);

void funcCompare(ASTNode * func1, ASTNode * func2);

int paramsCompare(ASTNode * paramNode1, ASTNode * paramNode2);

void newScope(void);

void throwSemanticError(ASTNode * node, char * content);

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
            inFunc = 1;
            funcReturnType = node -> ptrs[1];
            definedFunc(node);
            break;
        case NormalParam:
            normalParam(node);
            break;
        case ConstParam:
            constParam(node);
            break;
        case FuncPtrParam:
            needRollBackScope = 1;
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
            needRollBackScope = 1;
            funcPtr(node);
            break;
        case Variable:
            temp = variable(node);
            if (temp == NULL)
                return;
            break;
        case ConstFuncPtr:
            needRollBackScope = 1;
            constFuncPtr(node);
            break;
        case ConstVariable:
            temp = constVariable(node);
            if (temp == NULL)
                return;
            break;
        case Block:
            needRollBackScope = 1;
            blockScope(node);
            break;
        case FuncStmt:
            needRollBackScope = 1;
            funcStmt(node);
            break;
        case ExternFunc:
            needRollBackScope = 1;
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
        case Return:
            return_(node);
            returned = 1;
            break;
        case For:
            inFor++;
            break;
        case While:
            inWhile++;
            break;
        case DoWhile:
            inDoWhile++;
            break;
        case Switch:
            inSwitch++;
            break;
        case Break:
            if (inFor == 0 && inWhile == 0 && inDoWhile == 0 && inSwitch == 0)
                throwSemanticError(node, "'break' appeared not in a loop or switch");
            break;
        case Continue:
            if (inFor == 0 && inWhile == 0 && inDoWhile == 0)
                throwSemanticError(node, "'continue' appeared not in a loop");
            break;
        default:
            temp = exprCheck(node);
            if (temp == NULL)
                return;
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
            throwSemanticError(node, "recursively defined");
    }
    else if (node -> kind == DefinedFunc) {
        inFunc = 0;
        if (returned == 0) {
            if (funcReturnType -> kind != VoidType || funcReturnType -> listLen != 0)
                throwSemanticError(node, "control reaches end of non-void function");
        }
        returned = 0;
    }
    else if (node -> kind == For)
        inFor--;
    else if (node -> kind == While)
        inWhile--;
    else if (node -> kind == DoWhile)
        inDoWhile--;
    else if (node -> kind == Switch)
        inSwitch--;
}

void definedFunc(ASTNode * node) {      //返回值type为ptrs[1]
    ASTNode * target = scope -> localLookup(scope, node -> ptrs[2] -> image);
    if (target != NULL) {
        if (target -> kind == DefinedFunc) {
            throwSemanticError(node, "function redefined");
            return;
        }
        else
            funcCompare(target, node);
    }
    
    if (node -> ptrs[1] -> kind == StructType) {
        if (scope -> lookup(scope, node -> ptrs[1] -> image) == NULL) {
            throwSemanticError(node, "struct type undefined");
            return;
        }
    }
    
    if (node -> ptrs[1] -> kind == UnionType) {
        if (scope -> lookup(scope, node -> ptrs[1] -> image) == NULL) {
            throwSemanticError(node, "union type undefined");
            return;
        }
    }
    
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[2] -> image, node);
    newScope();
}

void definedStruct(ASTNode * node) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (scope -> localLookup(scope, node -> ptrs[0] -> image) != NULL) {
        throwSemanticError(node, "struct redefined");
        return;
    }
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[0] -> image, node);
    newScope();
    StruUnionScope -> put(StruUnionScope, node -> ptrs[0] -> image, scope);
    lastStop = GNodeConstructor(NULL, NodeConstructor(StructType, fileChecking, node -> line, node -> ptrs[0] -> image, ptrs));
}

void definedUnion(ASTNode * node) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (scope -> localLookup(scope, node -> ptrs[0] -> image) != NULL) {
        throwSemanticError(node, "union redefined");
        return;
    }
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[0] -> image, node);
    newScope();
    StruUnionScope -> put(StruUnionScope, node -> ptrs[0] -> image, scope);
    lastStop = GNodeConstructor(NULL, NodeConstructor(UnionType, fileChecking, node -> line, node -> ptrs[0] -> image, ptrs));
}

void normalParam(ASTNode * node) {
    if (scope -> localLookup(scope, node -> ptrs[1] -> ptrs[0] -> image) != NULL) {
        throwSemanticError(node, "parameter redefined");
        return;
    }
    
    if (node -> ptrs[0] -> kind == StructType) {
        if (scope -> lookup(scope, node -> ptrs[0] -> image) == NULL) {
            throwSemanticError(node, "struct type undefined");
            return;
        }
    }
    
    if (node -> ptrs[0] -> kind == UnionType) {
        if (scope -> lookup(scope, node -> ptrs[0] -> image) == NULL) {
            throwSemanticError(node, "union type undefined");
            return;
        }
    }
    
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[1] -> ptrs[0] -> image, node);
}

void constParam(ASTNode * node) {
    if (scope -> localLookup(scope, node -> ptrs[1] -> ptrs[0] -> image) != NULL) {
        throwSemanticError(node, "parameter redefined");
        return;
    }
    
    if (node -> ptrs[0] -> kind == StructType) {
        if (scope -> lookup(scope, node -> ptrs[0] -> image) == NULL) {
            throwSemanticError(node, "struct type undefined");
            return;
        }
    }
    
    if (node -> ptrs[0] -> kind == UnionType) {
        if (scope -> lookup(scope, node -> ptrs[0] -> image) == NULL) {
            throwSemanticError(node, "union type undefined");
        }
    }
    
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[1] -> ptrs[0] -> image, node);
}

void funcPtrParam(ASTNode * node) {
    if (scope -> localLookup(scope, node -> ptrs[1] -> image) != NULL) {
        throwSemanticError(node, "parameter redefined");
        return;
    }
    
    if (node -> ptrs[0] -> kind == StructType) {
        if (scope -> lookup(scope, node -> ptrs[0] -> image) == NULL) {
            throwSemanticError(node, "struct type undefined");
            return;
        }
    }
    
    if (node -> ptrs[0] -> kind == UnionType) {
        if (scope -> lookup(scope, node -> ptrs[0] -> image) == NULL) {
            throwSemanticError(node, "union type undefined");
            return;
        }
    }
    
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[1] -> image, node);
    newScope();
}

void funcPtr(ASTNode * node) {
    int name = 2;
    GNode * tempGnode = NULL;
    if (node -> ptrs[1] -> kind == Name)
        name = 1;
    
    if (node -> ptrs[name - 1] -> kind == StructType) {
        if (scope -> lookup(scope, node -> ptrs[name - 1] -> image) == NULL) {
            throwSemanticError(node, "struct type undefined");
            return;
        }
    }
    
    if (node -> ptrs[name - 1] -> kind == UnionType) {
        if (scope -> lookup(scope, node -> ptrs[name - 1] -> image) == NULL) {
            throwSemanticError(node, "union type undefined");
            return;
        }
    }
    
    if (inStructOrUnion == 1)
        tempGnode = GNodeConstructor(lastStop, node -> ptrs[name - 1]);
    
    if (scope -> localLookup(scope, node -> ptrs[name] -> image) != NULL) {
        throwSemanticError(node, "function pointer redefined");
        return;
    }
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[name] -> image, node);
    newScope();
}

void * variable(ASTNode * node) {
    int type = 0;
    int varname = 2;
    GNode * tempGnode = NULL;
    ASTNode * assignType = NULL;
    ASTNode * temp = NULL;
    ASTNode * varType = NULL;
    
    if (node -> ptrs[1] -> kind == Varname)
        varname = 1;
    
    if (node -> ptrs[0] == NULL)
        type = 1;
    else if (node -> ptrs[0] -> kind == Static)
        type = 1;
    
    if (node -> ptrs[type] -> kind == VoidType) {
        if (node -> ptrs[type] -> listLen == 0) {
            throwSemanticError(node, "void type variable not allowed");
            return NULL;
        }
    }
    
    if (node -> ptrs[type] -> kind == StructType) {
        if (scope -> lookup(scope, node -> ptrs[type] -> image) == NULL) {
            throwSemanticError(node, "struct type undefined");
            return NULL;
        }
    }
    
    if (node -> ptrs[type] -> kind == UnionType) {
        if (scope -> lookup(scope, node -> ptrs[type] -> image) == NULL) {
            throwSemanticError(node, "union type undefined");
            return NULL;
        }
    }
    
    if (inStructOrUnion == 1)
        tempGnode = GNodeConstructor(lastStop, node -> ptrs[type]);
    
    if (scope -> localLookup(scope, node -> ptrs[varname] -> ptrs[0] -> image) != NULL) {
        throwSemanticError(node, "variable redefined");
        return NULL;
    }
    
    if (node -> ptrs[varname + 1] != NULL) {
        assignType = exprCheck(node -> ptrs[varname + 1]);
        if (assignType == NULL)
            return NULL;
        temp = node -> ptrs[1];
        varType = node -> ptrs[type];
        node -> ptrs[1] = node -> ptrs[varname + 1];
        AssignTypeChecker(node, varType, assignType);
        node -> ptrs[varname + 1] = node -> ptrs[1];
        node -> ptrs[1] = temp;
    }

    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[varname] -> ptrs[0] -> image, node);

    return (void *)1;
}

void constFuncPtr(ASTNode * node) {
    int name = 2;
    GNode * tempGnode = NULL;
    if (node -> ptrs[1] -> kind == Name)
        name = 1;
    
    if (node -> ptrs[name - 1] -> kind == StructType) {
        if (scope -> lookup(scope, node -> ptrs[name - 1] -> image) == NULL) {
            throwSemanticError(node, "struct type undefined");
            return;
        }
    }
    
    if (node -> ptrs[name - 1] -> kind == UnionType) {
        if (scope -> lookup(scope, node -> ptrs[name - 1] -> image) == NULL) {
            throwSemanticError(node, "union type undefined");
            return;
        }
    }
    
    if (inStructOrUnion == 1)
        tempGnode = GNodeConstructor(lastStop, node -> ptrs[name - 1]);
    
    if (scope -> localLookup(scope, node -> ptrs[name] -> image) != NULL) {
        throwSemanticError(node, "function pointer redefined");
        return;
    }
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[name] -> image, node);
    newScope();
}

void * constVariable(ASTNode * node) {
    int type = 0;
    int varname = 2;
    GNode * tempGnode = NULL;
    ASTNode * assignType = NULL;
    ASTNode * temp = NULL;
    ASTNode * varType = NULL;
    
    if (node -> ptrs[1] -> kind == Varname)
        varname = 1;
    
    if (node -> ptrs[0] == NULL)
        type = 1;
    else if (node -> ptrs[0] -> kind == Static)
        type = 1;
    
    if (node -> ptrs[type] -> kind == VoidType) {
        if (node -> ptrs[type] -> listLen == 0) {
            throwSemanticError(node, "void type const variable not allowed");
            return NULL;
        }
    }
    
    if (node -> ptrs[type] -> kind == StructType) {
        if (scope -> lookup(scope, node -> ptrs[type] -> image) == NULL) {
            throwSemanticError(node, "struct type undefined");
            return NULL;
        }
    }
    
    if (node -> ptrs[type] -> kind == UnionType) {
        if (scope -> lookup(scope, node -> ptrs[type] -> image) == NULL) {
            throwSemanticError(node, "union type undefined");
            return NULL;
        }
    }
    
    if (inStructOrUnion == 1)
        tempGnode = GNodeConstructor(lastStop, node -> ptrs[type]);
    
    if (scope -> localLookup(scope, node -> ptrs[varname] -> ptrs[0] -> image) != NULL) {
        throwSemanticError(node, "const variable redefined");
        return NULL;
    }
    
    if (node -> ptrs[varname + 1] != NULL) {
        assignType = exprCheck(node -> ptrs[varname + 1]);
        if (assignType == NULL)
            return NULL;
        temp = node -> ptrs[1];
        varType = node -> ptrs[type];
        node -> ptrs[1] = node -> ptrs[varname + 1];
        AssignTypeChecker(node, varType, assignType);
        node -> ptrs[varname + 1] = node -> ptrs[1];
        node -> ptrs[1] = temp;
    }

    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[varname] -> ptrs[0] -> image, node);

    return (void *)1;
}

void blockScope(ASTNode * node) {
    scope -> symbolTable -> put(scope -> symbolTable, "<<Block>>", node);
    newScope();
}

void funcStmt(ASTNode * node) {
    ASTNode * target = scope -> localLookup(scope, node -> ptrs[2] -> image);
    
    if (target != NULL)
        funcCompare(target, node);
    
    if (node -> ptrs[1] -> kind == StructType) {
        if (scope -> lookup(scope, node -> ptrs[1] -> image) == NULL) {
            throwSemanticError(node, "struct type undefined");
            return;
        }
    }
    
    if (node -> ptrs[1] -> kind == UnionType) {
        if (scope -> lookup(scope, node -> ptrs[1] -> image) == NULL) {
            throwSemanticError(node, "union type undefined");
            return;
        }
    }
    
    scope -> symbolTable -> put(scope -> symbolTable, node -> ptrs[2] -> image, node);
    newScope();
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
    if (scope -> localLookup(scope, node -> image) != NULL) {
        throwSemanticError(node, "label relocated");
        return;
    }
    scope -> symbolTable -> put(scope -> symbolTable, node -> image, node);
}

void return_(ASTNode * node) {
    if (inFunc == 0) {
        throwSemanticError(node, "not in the definition of a function");
        return;
    }
    
    if (node -> ptrs[0] == NULL) {
        if (funcReturnType -> kind != VoidType || funcReturnType -> listLen != 0) {
            throwSemanticError(node, "non-void function should return a value");
            return;
        }
    }
    else {
        ASTNode * returnType = exprCheck(node -> ptrs[0]);
        if (returnType == NULL)
            return;
        ReturnTypeChecker(node, funcReturnType, returnType);
    }
}

//从表达式节点出发，检查表达式各部分的类型，进行隐式类型转换，并返回表达式最终得到的类型，基本上是语义分析的核心
ASTNode * exprCheck(ASTNode * node) {
    if (node -> kind == Identifier) {
        ASTNode * target = scope -> lookup(scope, node -> image);       //并不是type
        if (target == NULL) {
            throwSemanticError(node, "identifier undefined");
            return NULL;
            //printf("Melon: %s: semantic \033[31merror\033[0m in line %d identifier \'%s\' undefined\n", fileChecking, node -> line, node -> image);
            //exit(-1);
        }
        
        return getTargetType(target);
    }
    else if (node -> kind == IntegerLiteral || node -> kind == CharacterLiteral || node -> kind == StringLiteral || node -> kind == FloatLiteral || node -> kind == SizeofType || node -> kind == SizeofExpr)
        return node;
    else if (node -> kind == BinaryOp) {
        ASTNode * type1 = exprCheck(node -> ptrs[0]);
        ASTNode * type2 = exprCheck(node -> ptrs[1]);

        if (type1 == NULL || type2 == NULL)
            return NULL;
        
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

        if (type == NULL)
            return NULL;
        
        SuffixOpTypeChecker(type);
        
        return type;
    }
    else if (node -> kind == ArrayRef) {
        refCount++;
        ASTNode * refType = exprCheck(node -> ptrs[0]);

        if (refType == NULL)
            return NULL;
        
        if (refCount > refType -> listLen) {
            throwSemanticError(node, "too many times array reference");
            return NULL;
        }
        refCount = 0;
        
        ASTNode * exprType = exprCheck(node -> ptrs[1]);

        if (exprType == NULL)
            return NULL;
        
        ArrayRefTypeChecker(exprType);
        
        refType -> listLen--;
        return refType;
    }
    else if (node -> kind == OpAssign) {
        if (node -> ptrs[0] -> kind == Cast) {
            throwSemanticError(node, "assignment to cast is illegal, lvalue casts are not supported");
            return NULL;
        }
        
        switch (node -> ptrs[0] -> kind) {
            case Identifier:
            case Member:
            case PtrMember:
            case ArrayRef:
            case Dereference:
                break;
            default:
                throwSemanticError(node, "expression is not assignable");
                return NULL;
                break;
        }
        
        ASTNode * type1 = exprCheck(node -> ptrs[0]);
        ASTNode * type2 = exprCheck(node -> ptrs[2]);

        if (type1 == NULL || type2 == NULL)
            return NULL;
        
        OpAssignTypeChecker(node, type1, type2);
        
        return type1;
    }
    else if (node -> kind == Assign) {
        if (node -> ptrs[0] -> kind == Cast) {
            throwSemanticError(node, "assignment to cast is illegal, lvalue casts are not supported");
            return NULL;
        }
        
        switch (node -> ptrs[0] -> kind) {
            case Identifier:
            case Member:
            case PtrMember:
            case ArrayRef:
            case Dereference:
                break;
            default:
                throwSemanticError(node, "expression is not assignable");
                return NULL;
                break;
        }
        
        ASTNode * type1 = exprCheck(node -> ptrs[0]);
        ASTNode * type2 = exprCheck(node -> ptrs[1]);

        if (type1 == NULL || type2 == NULL)
            return NULL;
        
        AssignTypeChecker(node, type1, type2);
        
        return type1;
    }
    else if (node -> kind == CondExpr) {
        ASTNode * type1 = exprCheck(node -> ptrs[0]);
        type1 = exprCheck(node -> ptrs[1]);
        ASTNode * type2 = exprCheck(node -> ptrs[2]);

        if (type1 == NULL || type2 == NULL)
            return NULL;
        
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

        if (type1 == NULL || type2 == NULL)
            return NULL;
        
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

        if (type1 == NULL || type2 == NULL)
            return NULL;
        
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

        if (type == NULL)
            return NULL;
        
        SuffixOpTypeChecker(type);           //似乎和后缀运算符没有区别
        
        return type;
    }
    else if (node -> kind == UnaryOp) {
        ASTNode * type = exprCheck(node -> ptrs[0]);

        if (type == NULL)
            return NULL;
        
        if (!strcmp(node -> image, "!"))
            UnaryOpTypeChecker(type, LOGICNOT);
        else
            UnaryOpTypeChecker(type, NOT);
        
        return type;
    }
    else if (node -> kind == Dereference) {
        refCount++;
        ASTNode * type = exprCheck(node -> ptrs[0]);

        if (type == NULL)
            return NULL;
        
        if (refCount > type -> listLen) {
            throwSemanticError(node, "too many times dereference");
            return NULL;
        }
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
                throwSemanticError(node, "can not take the address of an rvalue");
                return NULL;
                break;
        }
        
        ASTNode * type = exprCheck(node -> ptrs[0]);

        if (type == NULL)
            return NULL;

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
                throwSemanticError(node, "member reference base type is not a structure or union");
                return NULL;
                break;
        }
        
        ASTNode * type = exprCheck(node -> ptrs[0]);

        if (type == NULL)
            return NULL;
        
        MemberTypeChecker(type);
        
        Scope * localScope = StruUnionScope -> get(StruUnionScope, type -> image);
        ASTNode * member = localScope -> localLookup(localScope, node -> ptrs[1] -> image);
        
        if (member == NULL) {
            throwSemanticError(node, "no such member");
            return NULL;
        }
        
        return getTargetType(member);
    }
    else if (node -> kind == PtrMember) {
        ASTNode * type = exprCheck(node -> ptrs[0]);

        if (type == NULL)
            return NULL;
        
        PtrMemberTypeChecker(type);
        
        Scope * localScope = StruUnionScope -> get(StruUnionScope, type -> image);
        ASTNode * member = localScope -> localLookup(localScope, node -> ptrs[1] -> image);
        
        if (member == NULL) {
            throwSemanticError(node, "no such member");
            return NULL;
        }
        
        return getTargetType(member);
    }
    else if (node -> kind == Cast) {
        return node -> ptrs[0];
    }
    else if (node -> kind == Funcall) {
        ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
        
        if (node -> ptrs[0] -> kind != Identifier) {
            throwSemanticError(node, "invalid function call");
            return NULL;
        }
        
        int params = 3;
        
        ASTNode * targetFunc = scope -> lookup(scope, node -> ptrs[0] -> image);
        if (targetFunc == NULL || (targetFunc -> kind != DefinedFunc && targetFunc -> kind != FuncStmt && targetFunc -> kind != FuncPtr && targetFunc -> kind != ConstFuncPtr)) {
            throwSemanticError(node, "function undefined");
            return NULL;
        }
        
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

                    if (inputType == NULL || targetType == NULL)
                        return NULL;
                    
                    if (inputType -> kind != targetType -> kind || inputType -> listLen != targetType -> listLen) {
                        throwSemanticError(node, "argument type mismatched");
                        return NULL;
                    }
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

                        if (inputType == NULL)
                            return NULL;
                        
                        if (targetFunc -> ptrs[params] -> list[i].ptrs[1] -> kind == Name)
                            temp = NodeConstructor(Identifier, fileChecking, node -> line, targetFunc -> ptrs[params] -> list[i].ptrs[1] -> image, ptrs);
                        else
                            temp = NodeConstructor(Identifier, fileChecking, node -> line, targetFunc -> ptrs[params] -> list[i].ptrs[1] -> ptrs[0] -> image, ptrs);
                        targetType = exprCheck(temp);
                        if (targetType == NULL)
                            return NULL;
                        free(temp);
                        
                        if (inputType -> kind != targetType -> kind || inputType -> listLen != targetType -> listLen) {
                            throwSemanticError(node, "argument type mismatched");
                            return NULL;
                        }
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
    
    return node;
}

void SuffixOpTypeChecker(ASTNode * type) {
    switch (type -> kind) {
        case IntegerLiteral:
            throwSemanticError(type, "integer literal is not assignable");
            break;
        case CharacterLiteral:
            throwSemanticError(type, "character literal is not assignable");
            break;
        case StringLiteral:
            throwSemanticError(type, "string literal is not assignable");
            break;
        case FloatLiteral:
            throwSemanticError(type, "float literal is not assignable");
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
                throwSemanticError(type, "struct type not allowed");
            break;
        case UnionType:
            if (type -> listLen == 0)
                throwSemanticError(type, "union type not allowed");
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
            throwSemanticError(type, "array subscript is not an integer");
            break;
        case FloatLiteral:
            throwSemanticError(type, "array subscript is not an integer");
            break;
        case VoidType:
            throwSemanticError(type, "array subscript is not an integer");
            break;
        case CharType:
            if (type -> listLen != 0)
                throwSemanticError(type, "array subscript is not an integer");
            break;
        case ShortIntType:
            if (type -> listLen != 0)
                throwSemanticError(type, "array subscript is not an integer");
            break;
        case IntType:
            if (type -> listLen != 0)
                throwSemanticError(type, "array subscript is not an integer");
            break;
        case LongIntType:
            if (type -> listLen != 0)
                throwSemanticError(type, "array subscript is not an integer");
            break;
        case UnsignedCharType:
            if (type -> listLen != 0)
                throwSemanticError(type, "array subscript is not an integer");
            break;
        case UnsignedShortIntType:
            if (type -> listLen != 0)
                throwSemanticError(type, "array subscript is not an integer");
            break;
        case UnsignedIntType:
            if (type -> listLen != 0)
                throwSemanticError(type, "array subscript is not an integer");
            break;
        case UnsignedLongIntType:
            if (type -> listLen != 0)
                throwSemanticError(type, "array subscript is not an integer");
            break;
        case StructType:
            throwSemanticError(type, "array subscript is not an integer");
            break;
        case UnionType:
            throwSemanticError(type, "array subscript is not an integer");
            break;
        case FloatType:
            throwSemanticError(type, "array subscript is not an integer");
            break;
        case DoubleType:
            throwSemanticError(type, "array subscript is not an integer");
            break;
        default:
            break;
    }
}

void OpAssignTypeChecker(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    switch (type1 -> kind) {
        case IntegerLiteral:
            throwSemanticError(type1, "expression is not assignable");
            break;
        case CharacterLiteral:
            throwSemanticError(type1, "expression is not assignable");
            break;
        case StringLiteral:
            throwSemanticError(type1, "expression is not assignable");
            break;
        case FloatLiteral:
            throwSemanticError(type1, "expression is not assignable");
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
            throwSemanticError(type1, "expression is not assignable");
            break;
        case CharacterLiteral:
            throwSemanticError(type1, "expression is not assignable");
            break;
        case StringLiteral:
            throwSemanticError(type1, "expression is not assignable");
            break;
        case FloatLiteral:
            throwSemanticError(type1, "expression is not assignable");
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
            throwSemanticError(type, "string literal operand not allowed");
            break;
        case FloatLiteral:
            if (kind == NOT)
                throwSemanticError(type, "float literal operand not allowed");
            break;
        case VoidType:
            if (kind == NOT)
                throwSemanticError(type, "pointer operand not allowed");
            break;
        case CharType:
            if (type -> listLen != 0) {
                if (kind == NOT)
                    throwSemanticError(type, "pointer operand not allowed");
            }
            break;
        case ShortIntType:
            if (type -> listLen != 0) {
                if (kind == NOT)
                    throwSemanticError(type, "pointer operand not allowed");
            }
            break;
        case IntType:
            if (type -> listLen != 0) {
                if (kind == NOT)
                    throwSemanticError(type, "pointer operand not allowed");
            }
            break;
        case LongIntType:
            if (type -> listLen != 0) {
                if (kind == NOT)
                    throwSemanticError(type, "pointer operand not allowed");
            }
            break;
        case UnsignedCharType:
            if (type -> listLen != 0) {
                if (kind == NOT)
                    throwSemanticError(type, "pointer operand not allowed");
            }
            break;
        case UnsignedShortIntType:
            if (type -> listLen != 0) {
                if (kind == NOT)
                    throwSemanticError(type, "pointer operand not allowed");
            }
            break;
        case UnsignedIntType:
            if (type -> listLen != 0) {
                if (kind == NOT)
                    throwSemanticError(type, "pointer operand not allowed");
            }
            break;
        case UnsignedLongIntType:
            if (type -> listLen != 0) {
                if (kind == NOT)
                    throwSemanticError(type, "pointer operand not allowed");
            }
            break;
        case StructType:
            if (type -> listLen == 0)
                throwSemanticError(type, "struct type operand not allowed");
            else {
                if (kind == NOT)
                    throwSemanticError(type, "pointer operand not allowed");
            }
            break;
        case UnionType:
            if (type -> listLen == 0)
                throwSemanticError(type, "union type operand not allowed");
            else {
                if (kind == NOT)
                    throwSemanticError(type, "pointer operand not allowed");
            }
            break;
        case FloatType:
            if (kind == NOT)
                throwSemanticError(type, "float type operand not allowed");
            break;
        case DoubleType:
            if (kind == NOT)
                throwSemanticError(type, "double type operand not allowed");
            break;
        default:
            break;
    }
}

void MemberTypeChecker(ASTNode * type) {
    switch (type -> kind) {
        case IntegerLiteral:
            throwSemanticError(type, "member reference base type is not a structure or union");
            break;
        case CharacterLiteral:
            throwSemanticError(type, "member reference base type is not a structure or union");
            break;
        case StringLiteral:
            throwSemanticError(type, "member reference base type is not a structure or union");
            break;
        case FloatLiteral:
            throwSemanticError(type, "member reference base type is not a structure or union");
            break;
        case VoidType:
            throwSemanticError(type, "member reference base type is a pointer");
            break;
        case CharType:
            if (type -> listLen != 0)
                    throwSemanticError(type, "member reference base type is a pointer");
            else
                throwSemanticError(type, "member reference base type is not a structure or union");
            break;
        case ShortIntType:
            if (type -> listLen != 0)
                throwSemanticError(type, "member reference base type is a pointer");
            else
                throwSemanticError(type, "member reference base type is not a structure or union");
            break;
        case IntType:
            if (type -> listLen != 0)
                throwSemanticError(type, "member reference base type is a pointer");
            else
                throwSemanticError(type, "member reference base type is not a structure or union");
            break;
        case LongIntType:
            if (type -> listLen != 0)
                throwSemanticError(type, "member reference base type is a pointer");
            else
                throwSemanticError(type, "member reference base type is not a structure or union");
            break;
        case UnsignedCharType:
            if (type -> listLen != 0)
                throwSemanticError(type, "member reference base type is a pointer");
            else
                throwSemanticError(type, "member reference base type is not a structure or union");
            break;
        case UnsignedShortIntType:
            if (type -> listLen != 0)
                throwSemanticError(type, "member reference base type is a pointer");
            else
                throwSemanticError(type, "member reference base type is not a structure or union");
            break;
        case UnsignedIntType:
            if (type -> listLen != 0)
                throwSemanticError(type, "member reference base type is a pointer");
            else
                throwSemanticError(type, "member reference base type is not a structure or union");
            break;
        case UnsignedLongIntType:
            if (type -> listLen != 0)
                throwSemanticError(type, "member reference base type is a pointer");
            else
                throwSemanticError(type, "member reference base type is not a structure or union");
            break;
        case StructType:
            if (type -> listLen != 0)
                throwSemanticError(type, "member reference base type is a pointer");
            break;
        case UnionType:
            if (type -> listLen != 0)
                throwSemanticError(type, "member reference base type is a pointer");
            break;
        case FloatType:
            if (type -> listLen != 0)
                throwSemanticError(type, "member reference base type is a pointer");
            else
                throwSemanticError(type, "member reference base type is not a structure or union");
            break;
        case DoubleType:
            if (type -> listLen != 0)
                throwSemanticError(type, "member reference base type is a pointer");
            else
                throwSemanticError(type, "member reference base type is not a structure or union");
            break;
        default:
            break;
    }
}

void PtrMemberTypeChecker(ASTNode * type) {
    switch (type -> kind) {
        case IntegerLiteral:
            throwSemanticError(type, "member reference base type is not a pointer");
            break;
        case CharacterLiteral:
            throwSemanticError(type, "member reference base type is not a pointer");
            break;
        case StringLiteral:
            throwSemanticError(type, "member reference base type is not a pointer");
            break;
        case FloatLiteral:
            throwSemanticError(type, "member reference base type is not a pointer");
            break;
        case VoidType:
            throwSemanticError(type, "member reference base type is not a structure or union");
            break;
        case CharType:
            if (type -> listLen != 0)
                throwSemanticError(type, "member reference base type is not a structure or union");
            else
                throwSemanticError(type, "member reference base type is not a pointer");
            break;
        case ShortIntType:
            if (type -> listLen != 0)
                throwSemanticError(type, "member reference base type is not a structure or union");
            else
                throwSemanticError(type, "member reference base type is not a pointer");
            break;
        case IntType:
            if (type -> listLen != 0)
                throwSemanticError(type, "member reference base type is not a structure or union");
            else
                throwSemanticError(type, "member reference base type is not a pointer");
            break;
        case LongIntType:
            if (type -> listLen != 0)
                throwSemanticError(type, "member reference base type is not a structure or union");
            else
                throwSemanticError(type, "member reference base type is not a pointer");
            break;
        case UnsignedCharType:
            if (type -> listLen != 0)
                throwSemanticError(type, "member reference base type is not a structure or union");
            else
                throwSemanticError(type, "member reference base type is not a pointer");
            break;
        case UnsignedShortIntType:
            if (type -> listLen != 0)
                throwSemanticError(type, "member reference base type is not a structure or union");
            else
                throwSemanticError(type, "member reference base type is not a pointer");
            break;
        case UnsignedIntType:
            if (type -> listLen != 0)
                throwSemanticError(type, "member reference base type is not a structure or union");
            else
                throwSemanticError(type, "member reference base type is not a pointer");
            break;
        case UnsignedLongIntType:
            if (type -> listLen != 0)
                throwSemanticError(type, "member reference base type is not a structure or union");
            else
                throwSemanticError(type, "member reference base type is not a pointer");
            break;
        case StructType:
            if (type -> listLen != 0) {
                if (type -> listLen != 1)
                    throwSemanticError(type, "member reference base type is not a structure or union");
            }
            else
                throwSemanticError(type, "member reference base type is not a pointer");
            break;
        case UnionType:
            if (type -> listLen != 0) {
                if (type -> listLen != 1)
                    throwSemanticError(type, "member reference base type is not a structure or union");
            }
            else
                throwSemanticError(type, "member reference base type is not a pointer");
            break;
        case FloatType:
            if (type -> listLen != 0)
                throwSemanticError(type, "member reference base type is not a structure or union");
            else
                throwSemanticError(type, "member reference base type is not a pointer");
            break;
        case DoubleType:
            if (type -> listLen != 0)
                throwSemanticError(type, "member reference base type is not a structure or union");
            else
                throwSemanticError(type, "member reference base type is not a pointer");
            break;
        default:
            break;
    }
}

void ReturnTypeChecker(ASTNode * parent, ASTNode * type1, ASTNode * type2) {
    switch (type1 -> kind) {
        case VoidType:
            returnVoidType(parent, type1, type2);
            break;
        case CharType:
            returnCharType(parent, type1, type2);
            break;
        case ShortIntType:
            returnShortIntType(parent, type1, type2);
            break;
        case IntType:
            returnIntType(parent, type1, type2);
            break;
        case LongIntType:
            returnLongIntType(parent, type1, type2);
            break;
        case UnsignedCharType:
            returnUnsignedCharType(parent, type1, type2);
            break;
        case UnsignedShortIntType:
            returnUnsignedShortIntType(parent, type1, type2);
            break;
        case UnsignedIntType:
            returnUnsignedIntType(parent, type1, type2);
            break;
        case UnsignedLongIntType:
            returnUnsignedLongIntType(parent, type1, type2);
            break;
        case StructType:
            returnStructType(parent, type1, type2);
            break;
        case UnionType:
            returnUnionType(parent, type1, type2);
            break;
        case FloatType:
            returnFloatType(parent, type1, type2);
            break;
        case DoubleType:
            returnDoubleType(parent, type1, type2);
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

void funcCompare(ASTNode * func1, ASTNode * func2) {
    //check static
    if ((func1 -> ptrs[0] != NULL && func2 -> ptrs[0] == NULL) || (func1 -> ptrs[0] == NULL && func2 -> ptrs[0] != NULL)) {
        throwSemanticError(func2, "conflicting function types");
        return;
    }
    
    //check return value type
    if (func1 -> ptrs[1] -> kind != func2 -> ptrs[1] -> kind || func1 -> ptrs[1] -> listLen != func2 -> ptrs[1] -> listLen) {
        throwSemanticError(func2, "conflicting function types");
        return;
    }
    
    //check parameters
    if (paramsCompare(func1 -> ptrs[3], func2 -> ptrs[3])) {
        throwSemanticError(func2, "conflicting function types");
        return;
    }
}

int paramsCompare(ASTNode * paramNode1, ASTNode * paramNode2) {
    if ((paramNode1 != NULL && paramNode2 == NULL) || (paramNode1 == NULL && paramNode2 != NULL))
        return 1;
    else if (paramNode1 != NULL) {
        if (paramNode1 -> listLen != paramNode2 -> listLen)
            return 1;
        
        int i;
        for (i = 0; i < paramNode1 -> listLen; i++) {
            //check NormalParam, ConstParam or FuncPtrParam
            if (paramNode1 -> list[i].kind != paramNode2 -> list[i].kind)
                return 1;
            
            if (paramNode1 -> list[i].kind == UnlimitedParams)
                continue;
            
            //check parameter type
            if (paramNode1 -> list[i].ptrs[0] -> kind != paramNode2 -> list[i].ptrs[0] -> kind || paramNode1 -> list[i].ptrs[0] -> listLen != paramNode2 -> list[i].ptrs[0] -> listLen)
                return 1;
            
            if (paramNode1 -> list[i].kind == FuncPtrParam) {
                //check function pointer name
                if (strcmp(paramNode1 -> list[i].ptrs[1] -> image, paramNode2 -> list[i].ptrs[1] -> image))
                    return 1;
                
                //check function pointer parameters
                if (paramsCompare(paramNode1 -> list[i].ptrs[2], paramNode2 -> list[i].ptrs[2]))
                    return 1;
            }
            else {
                if (strcmp(paramNode1 -> list[i].ptrs[1] -> ptrs[0] -> image, paramNode2 -> list[i].ptrs[1] -> ptrs[0] -> image) || paramNode1 -> list[i].ptrs[1] -> listLen != paramNode2 -> list[i].ptrs[1] -> listLen)
                    return 1;
            }
        }
    }
    
    return 0;
}

void newScope(void) {
    Scope * temp = ScopeConstructor(scope);
    scope -> appendScope(scope, temp);
    scope = temp;
}

void throwSemanticError(ASTNode * node, char * content) {
    error_t * new_err = ErrorConstructor(node -> line, SEMANTICS, node -> file, content, NULL);
    list_add_tail(&new_err -> list, &err_list -> list);
}
