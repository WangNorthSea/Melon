
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Lexer/token.h"
#include "../Lexer/tokenkind.h"
#include "../ASTNode/node.h"
#include "../ASTNode/constructor.h"
#include "../SymbolTable/hashtable.h"
#include "../SymbolTable/scope.h"
#include "../Semantics/semantics.h"
#include "../Report/error.h"
#include "../Parser/parser.h"
#include "../File/fileop.h"

char * riscv64__new_filepath(void) {
    char * codefile_path = (char *)malloc(strlen(parsingFile));
    memcpy(codefile_path, parsingFile, strlen(parsingFile));
    for (int i = strlen(parsingFile) - 1; i >= 0; i--) {
        if (codefile_path[i] == '.') {
            codefile_path[i + 1] = 's';
            codefile_path[i + 2] = 0;
            break;
        }
    }
    return codefile_path;
}

void riscv64__put_header(FILE * fp) {
    file_write(fp, "\t\t.file\t\"");
    file_write(fp, parsingFile);
    file_write(fp, "\"\n");
    file_write(fp, "\t\t.option nopic\n");
    file_write(fp, "\t\t.attribute arch, \"rv64i2p0_m2p0_a2p0_f2p0_d2p0_c2p0\"\n");
    file_write(fp, "\t\t.attribute unaligned_access, 0\n");
    file_write(fp, "\t\t.attribute stack_align, 16\n");
}

void riscv64__put_globl_var(FILE * fp, Scope * scope) {
    file_write(fp, "\t\t.text\n");
    for (int i = 0; i < TableArraySize; i++) {
        if (scope -> symbolTable -> tableArray[i].key != NULL) {
            struct Value * next = &(scope -> symbolTable -> tableArray[i]);
            while (next != NULL) {
                ASTNode * var = (ASTNode *)next -> target;
                
                /*if (((ASTNode *)next -> target) -> kind == DefinedFunc || ((ASTNode *)next -> target) -> kind == DefinedStruct || ((ASTNode *)next -> target) -> kind == DefinedUnion || ((ASTNode *)next -> target) -> kind == Block || ((ASTNode *)next -> target) -> kind == FuncPtrParam) {
                    ;
                }*/
                next = next -> next;
            }
        }
    }

}

void riscv64__codegen(ASTNode * root, Scope * scope) {
    char * codefile_path = riscv64__new_filepath();
    FILE * codefile = fopen(codefile_path, "w");
    
    riscv64__put_header(codefile);
    //riscv64__put_globl_var(codefile, scope);












    fclose(codefile);
}
