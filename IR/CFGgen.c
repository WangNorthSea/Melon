
#include <stdlib.h>
#include "../List/list.h"
#include "../ASTNode/node.h"
#include "../SymbolTable/hashtable.h"
#include "../SymbolTable/scope.h"
#include "ir.h"
#include "opcode.h"

struct list_head cfg_list;

void CFGConstruct(ASTNode * ast) {
    init_list_head(&cfg_list);
    CFGIterator(ast);
}

void CFGIterator(ASTNode * node) {
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