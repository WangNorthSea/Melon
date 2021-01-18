
#ifndef __CODEGEN_H__
#define __CODEGEN_H__

#include "../ASTNode/node.h"
#include "../SymbolTable/scope.h"


extern void riscv64_codegen(ASTNode * root, Scope * scope);

#endif