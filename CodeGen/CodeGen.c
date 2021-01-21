
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

#define ROUND(a, n)     (((((int)(a))+(n)-1)) & ~((n)-1))

int branch_label = 2;

void riscv64_put_block(FILE * fp, ASTNode * block, Scope * scope);
void riscv64_put_stmt(FILE * fp, ASTNode * stmt_node, Scope * scope, int * branch);
void riscv64_put_funcall(FILE * fp, ASTNode * node, Scope * scope);
ASTNode * riscv64_calc_array_ref_addr(FILE * fp, ASTNode * node, Scope * scope);

char * riscv64_new_filepath(void) {
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

void riscv64_put_header(FILE * fp) {
    file_write(fp, "\t\t.file\t\"");
    file_write(fp, parsingFile);
    file_write(fp, "\"\n");
    file_write(fp, "\t\t.option nopic\n");
    file_write(fp, "\t\t.attribute arch, \"rv64i2p0_m2p0_a2p0_f2p0_d2p0_c2p0\"\n");
    file_write(fp, "\t\t.attribute unaligned_access, 0\n");
    file_write(fp, "\t\t.attribute stack_align, 16\n");
}

void riscv64_put_globl_int(FILE * fp, ASTNode * var, struct varinfo * info_ptr) {
    if (var -> ptrs[2] -> listLen == 0) {  //not array
        file_write(fp, "\t\t.globl ");
        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image); //name
        file_write(fp, "\n");
        if (var -> ptrs[3] == NULL) {
            file_write(fp, "\t\t.section\t\t.sbss,\"aw\",@nobits\n");
            file_write(fp, "\t\t.align\t2\n");
            file_write(fp, "\t\t.type\t");
            file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
            file_write(fp, ", @object\n");
            file_write(fp, "\t\t.size\t");
            file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
            file_write(fp, ", 4\n");
            file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
            file_write(fp, ":\n");
            file_write(fp, "\t\t.zero\t4\n\n");
        }
        else if (var -> ptrs[3] -> kind == IntegerLiteral) {
            if (!strcmp(var -> ptrs[3] -> image, "0")) {  //initialized as 0
                file_write(fp, "\t\t.section\t\t.sbss,\"aw\",@nobits\n");
                file_write(fp, "\t\t.align\t2\n");
                file_write(fp, "\t\t.type\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ", @object\n");
                file_write(fp, "\t\t.size\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ", 4\n");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ":\n");
                file_write(fp, "\t\t.zero\t4\n\n");
            }
            else {
                file_write(fp, "\t\t.section\t\t.sdata,\"aw\"\n");
                file_write(fp, "\t\t.align\t2\n");
                file_write(fp, "\t\t.type\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ", @object\n");
                file_write(fp, "\t\t.size\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ", 4\n");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ":\n");
                file_write(fp, "\t\t.word\t");
                file_write(fp, var -> ptrs[3] -> image);
                file_write(fp, "\n\n");
            }
        }
    }
    else {
        if (var -> ptrs[2] -> list[0].kind == FixedArray) {
            int elem_count = atoi(var -> ptrs[2] -> list[0].image);
            if (var -> ptrs[3] == NULL) {   //uninitialized
                file_write(fp, "\t\t.globl ");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image); //name
                file_write(fp, "\n");
                file_write(fp, "\t\t.bss\n");
                file_write(fp, "\t\t.align\t3\n");
                file_write(fp, "\t\t.type\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ", @object\n");
                file_write(fp, "\t\t.size\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                fprintf(fp, ", %d\n", elem_count * 4);
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ":\n");
                fprintf(fp, "\t\t.zero\t%d\n\n", elem_count * 4);
            }
            else {
                file_write(fp, "\t\t.globl ");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image); //name
                file_write(fp, "\n");
                if (var -> ptrs[3] -> kind == ListExpr) {
                    if (var -> ptrs[3] -> listLen == 1 && var -> ptrs[3] -> list[0].image[0] == '0') {
                        file_write(fp, "\t\t.bss\n");
                        file_write(fp, "\t\t.align\t3\n");
                        file_write(fp, "\t\t.type\t");
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        file_write(fp, ", @object\n");
                        file_write(fp, "\t\t.size\t");
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        fprintf(fp, ", %d\n", elem_count * 4);
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        file_write(fp, ":\n");
                        fprintf(fp, "\t\t.zero\t%d\n\n", elem_count * 4);
                    }
                    else {
                        file_write(fp, "\t\t.data\n");
                        file_write(fp, "\t\t.align\t3\n");
                        file_write(fp, "\t\t.type\t");
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        file_write(fp, ", @object\n");
                        file_write(fp, "\t\t.size\t");
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        fprintf(fp, ", %d\n", elem_count * 4);
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        file_write(fp, ":\n");
                        for (int i = 0; i < elem_count; i++) {
                            fprintf(fp, "\t\t.word\t%s\n", var -> ptrs[3] -> list[i].image);
                        }
                        file_write(fp, "\n");
                    }
                }
                else {
                    ;
                }
            }
        }
    }
}

void riscv64_put_globl_char(FILE * fp, ASTNode * var, struct varinfo * info_ptr) {
    if (var -> ptrs[2] -> listLen == 0) {  //not array
        file_write(fp, "\t\t.globl ");
        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image); //name
        file_write(fp, "\n");
        if (var -> ptrs[3] == NULL) {
            file_write(fp, "\t\t.section\t\t.sbss,\"aw\",@nobits\n");
            file_write(fp, "\t\t.type\t");
            file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
            file_write(fp, ", @object\n");
            file_write(fp, "\t\t.size\t");
            file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
            file_write(fp, ", 1\n");
            file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
            file_write(fp, ":\n");
            file_write(fp, "\t\t.zero\t1\n\n");
        }
        else if (var -> ptrs[3] -> kind == IntegerLiteral) {
            if (!strcmp(var -> ptrs[3] -> image, "0")) {  //initialized as 0
                file_write(fp, "\t\t.section\t\t.sbss,\"aw\",@nobits\n");
                file_write(fp, "\t\t.type\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ", @object\n");
                file_write(fp, "\t\t.size\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ", 1\n");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ":\n");
                file_write(fp, "\t\t.zero\t1\n\n");
            }
            else {
                file_write(fp, "\t\t.section\t\t.sdata,\"aw\"\n");
                file_write(fp, "\t\t.type\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ", @object\n");
                file_write(fp, "\t\t.size\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ", 1\n");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ":\n");
                file_write(fp, "\t\t.byte\t");
                file_write(fp, var -> ptrs[3] -> image);
                file_write(fp, "\n\n");
            }
        }
    }
    else {
        if (var -> ptrs[2] -> list[0].kind == FixedArray) {
            int elem_count = atoi(var -> ptrs[2] -> list[0].image);
            if (var -> ptrs[3] == NULL) {   //uninitialized
                file_write(fp, "\t\t.globl ");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image); //name
                file_write(fp, "\n");
                file_write(fp, "\t\t.section\t\t.sbss\n");
                file_write(fp, "\t\t.align\t3\n");
                file_write(fp, "\t\t.type\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ", @object\n");
                file_write(fp, "\t\t.size\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                fprintf(fp, ", %d\n", elem_count);
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ":\n");
                fprintf(fp, "\t\t.zero\t%d\n\n", elem_count);
            }
            else {
                file_write(fp, "\t\t.globl ");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image); //name
                file_write(fp, "\n");
                if (var -> ptrs[3] -> kind == ListExpr) {
                    if (var -> ptrs[3] -> listLen == 1 && var -> ptrs[3] -> list[0].image[0] == '0') {
                        file_write(fp, "\t\t.section\t\t.sbss\n");
                        file_write(fp, "\t\t.align\t3\n");
                        file_write(fp, "\t\t.type\t");
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        file_write(fp, ", @object\n");
                        file_write(fp, "\t\t.size\t");
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        fprintf(fp, ", %d\n", elem_count);
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        file_write(fp, ":\n");
                        fprintf(fp, "\t\t.zero\t%d\n\n", elem_count);
                    }
                    else {
                        file_write(fp, "\t\t.section\t\t.sdata\n");
                        file_write(fp, "\t\t.align\t3\n");
                        file_write(fp, "\t\t.type\t");
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        file_write(fp, ", @object\n");
                        file_write(fp, "\t\t.size\t");
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        fprintf(fp, ", %d\n", elem_count);
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        file_write(fp, ":\n");
                        for (int i = 0; i < elem_count; i++) {
                            fprintf(fp, "\t\t.byte\t%s\n", var -> ptrs[3] -> list[i].image);
                        }
                        file_write(fp, "\n");
                    }
                }
                else {
                    ;
                }
            }
        }
    }
}

void riscv64_put_globl_float(FILE * fp, ASTNode * var, struct varinfo * info_ptr) {
    if (var -> ptrs[2] -> listLen == 0) {  //not array
        file_write(fp, "\t\t.globl ");
        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image); //name
        file_write(fp, "\n");
        if (var -> ptrs[3] == NULL) {
            file_write(fp, "\t\t.section\t\t.sbss,\"aw\",@nobits\n");
            file_write(fp, "\t\t.align\t2\n");
            file_write(fp, "\t\t.type\t");
            file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
            file_write(fp, ", @object\n");
            file_write(fp, "\t\t.size\t");
            file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
            file_write(fp, ", 4\n");
            file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
            file_write(fp, ":\n");
            file_write(fp, "\t\t.zero\t4\n\n");
        }
        else if (var -> ptrs[3] -> kind == IntegerLiteral || var -> ptrs[3] -> kind == FloatLiteral) {
            if (!strcmp(var -> ptrs[3] -> image, "0")) {  //initialized as 0
                file_write(fp, "\t\t.section\t\t.sbss,\"aw\",@nobits\n");
                file_write(fp, "\t\t.align\t2\n");
                file_write(fp, "\t\t.type\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ", @object\n");
                file_write(fp, "\t\t.size\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ", 4\n");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ":\n");
                file_write(fp, "\t\t.zero\t4\n\n");
            }
            else {
                file_write(fp, "\t\t.section\t\t.sdata,\"aw\"\n");
                file_write(fp, "\t\t.align\t2\n");
                file_write(fp, "\t\t.type\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ", @object\n");
                file_write(fp, "\t\t.size\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ", 4\n");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ":\n");
                file_write(fp, "\t\t.word\t");
                float num = strtof(var -> ptrs[3] -> image, NULL);
                float * num_ptr = &num;
                unsigned output = *(unsigned *)num_ptr;
                fprintf(fp, "%u\n\n", output);
            }
        }
    }
    else {
        if (var -> ptrs[2] -> list[0].kind == FixedArray) {
            int elem_count = atoi(var -> ptrs[2] -> list[0].image);
            if (var -> ptrs[3] == NULL) {   //uninitialized
                file_write(fp, "\t\t.globl ");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image); //name
                file_write(fp, "\n");
                file_write(fp, "\t\t.bss\n");
                file_write(fp, "\t\t.align\t3\n");
                file_write(fp, "\t\t.type\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ", @object\n");
                file_write(fp, "\t\t.size\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                fprintf(fp, ", %d\n", elem_count * 4);
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ":\n");
                fprintf(fp, "\t\t.zero\t%d\n\n", elem_count * 4);
            }
            else {
                file_write(fp, "\t\t.globl ");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image); //name
                file_write(fp, "\n");
                if (var -> ptrs[3] -> kind == ListExpr) {
                    if (var -> ptrs[3] -> listLen == 1 && var -> ptrs[3] -> list[0].image[0] == '0') {
                        file_write(fp, "\t\t.bss\n");
                        file_write(fp, "\t\t.align\t3\n");
                        file_write(fp, "\t\t.type\t");
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        file_write(fp, ", @object\n");
                        file_write(fp, "\t\t.size\t");
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        fprintf(fp, ", %d\n", elem_count * 4);
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        file_write(fp, ":\n");
                        fprintf(fp, "\t\t.zero\t%d\n\n", elem_count * 4);
                    }
                    else {
                        file_write(fp, "\t\t.data\n");
                        file_write(fp, "\t\t.align\t3\n");
                        file_write(fp, "\t\t.type\t");
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        file_write(fp, ", @object\n");
                        file_write(fp, "\t\t.size\t");
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        fprintf(fp, ", %d\n", elem_count * 4);
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        file_write(fp, ":\n");
                        for (int i = 0; i < elem_count; i++) {
                            float num = strtof(var -> ptrs[3] -> list[i].image, NULL);
                            float * num_ptr = &num;
                            unsigned output = *(unsigned *)num_ptr;
                            fprintf(fp, "\t\t.word\t%u\n", output);
                        }
                        file_write(fp, "\n");
                    }
                }
                else {
                    ;
                }
            }
        }
    }
}

void riscv64_put_globl_double(FILE * fp, ASTNode * var, struct varinfo * info_ptr) {
    if (var -> ptrs[2] -> listLen == 0) {  //not array
        file_write(fp, "\t\t.globl ");
        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image); //name
        file_write(fp, "\n");
        if (var -> ptrs[3] == NULL) {
            file_write(fp, "\t\t.section\t\t.sbss,\"aw\",@nobits\n");
            file_write(fp, "\t\t.align\t3\n");
            file_write(fp, "\t\t.type\t");
            file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
            file_write(fp, ", @object\n");
            file_write(fp, "\t\t.size\t");
            file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
            file_write(fp, ", 8\n");
            file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
            file_write(fp, ":\n");
            file_write(fp, "\t\t.zero\t8\n\n");
        }
        else if (var -> ptrs[3] -> kind == IntegerLiteral || var -> ptrs[3] -> kind == FloatLiteral) {
            if (!strcmp(var -> ptrs[3] -> image, "0")) {  //initialized as 0
                file_write(fp, "\t\t.section\t\t.sbss,\"aw\",@nobits\n");
                file_write(fp, "\t\t.align\t3\n");
                file_write(fp, "\t\t.type\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ", @object\n");
                file_write(fp, "\t\t.size\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ", 8\n");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ":\n");
                file_write(fp, "\t\t.zero\t8\n\n");
            }
            else {
                file_write(fp, "\t\t.section\t\t.sdata,\"aw\"\n");
                file_write(fp, "\t\t.align\t3\n");
                file_write(fp, "\t\t.type\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ", @object\n");
                file_write(fp, "\t\t.size\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ", 8\n");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ":\n");
                double num = strtod(var -> ptrs[3] -> image, NULL);
                double * num_ptr = &num;
                unsigned long long output = *(unsigned long long *)num_ptr;
                unsigned upper = (unsigned)((output >> 32) & 0xffffffff);
                unsigned lower = (unsigned)(output & 0xffffffff);
                fprintf(fp, "\t\t.word\t%u\n", lower);
                fprintf(fp, "\t\t.word\t%u\n\n", upper);  //little endian
            }
        }
    }
    else {
        if (var -> ptrs[2] -> list[0].kind == FixedArray) {
            int elem_count = atoi(var -> ptrs[2] -> list[0].image);
            if (var -> ptrs[3] == NULL) {   //uninitialized
                file_write(fp, "\t\t.globl ");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image); //name
                file_write(fp, "\n");
                file_write(fp, "\t\t.bss\n");
                file_write(fp, "\t\t.align\t3\n");
                file_write(fp, "\t\t.type\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ", @object\n");
                file_write(fp, "\t\t.size\t");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                fprintf(fp, ", %d\n", elem_count * 8);
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                file_write(fp, ":\n");
                fprintf(fp, "\t\t.zero\t%d\n\n", elem_count * 8);
            }
            else {
                file_write(fp, "\t\t.globl ");
                file_write(fp, var -> ptrs[2] -> ptrs[0] -> image); //name
                file_write(fp, "\n");
                if (var -> ptrs[3] -> kind == ListExpr) {
                    if (var -> ptrs[3] -> listLen == 1 && var -> ptrs[3] -> list[0].image[0] == '0') {
                        file_write(fp, "\t\t.bss\n");
                        file_write(fp, "\t\t.align\t3\n");
                        file_write(fp, "\t\t.type\t");
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        file_write(fp, ", @object\n");
                        file_write(fp, "\t\t.size\t");
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        fprintf(fp, ", %d\n", elem_count * 8);
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        file_write(fp, ":\n");
                        fprintf(fp, "\t\t.zero\t%d\n\n", elem_count * 8);
                    }
                    else {
                        file_write(fp, "\t\t.data\n");
                        file_write(fp, "\t\t.align\t3\n");
                        file_write(fp, "\t\t.type\t");
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        file_write(fp, ", @object\n");
                        file_write(fp, "\t\t.size\t");
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        fprintf(fp, ", %d\n", elem_count * 8);
                        file_write(fp, var -> ptrs[2] -> ptrs[0] -> image);
                        file_write(fp, ":\n");
                        for (int i = 0; i < elem_count; i++) {
                            double num = strtod(var -> ptrs[3] -> list[i].image, NULL);
                            double * num_ptr = &num;
                            unsigned long long output = *(unsigned long long *)num_ptr;
                            unsigned upper = (unsigned)((output >> 32) & 0xffffffff);
                            unsigned lower = (unsigned)(output & 0xffffffff);
                            fprintf(fp, "\t\t.word\t%u\n", lower);
                            fprintf(fp, "\t\t.word\t%u\n", upper); //little endian
                        }
                        file_write(fp, "\n");
                    }
                }
                else {
                    ;
                }
            }
        }
    }
}

void riscv64_put_globl_var(FILE * fp, Scope * scope) {
    file_write(fp, "\t\t.text\n");
    for (int i = 0; i < TableArraySize; i++) {
        if (scope -> symbolTable -> tableArray[i].key != NULL) {
            struct Value * next = &(scope -> symbolTable -> tableArray[i]);
            while (next != NULL) {
                ASTNode * var = (ASTNode *)next -> target;
                struct varinfo * info_ptr = &(next -> info);
                if (var -> kind == Variable || var -> kind == ConstVariable) {
                    switch (var -> ptrs[1] -> kind) {
                        case IntType:
                            riscv64_put_globl_int(fp, var, info_ptr);
                            break;
                        case CharType:
                            riscv64_put_globl_char(fp, var, info_ptr);
                            break;
                        case BoolType:
                            riscv64_put_globl_char(fp, var, info_ptr);
                            break;
                        case FloatType:
                            riscv64_put_globl_float(fp, var, info_ptr);
                            break;
                        case DoubleType:
                            riscv64_put_globl_double(fp, var, info_ptr);
                            break;
                        default:
                            break;
                    }
                }
                /*if (((ASTNode *)next -> target) -> kind == DefinedFunc || ((ASTNode *)next -> target) -> kind == DefinedStruct || ((ASTNode *)next -> target) -> kind == DefinedUnion || ((ASTNode *)next -> target) -> kind == Block || ((ASTNode *)next -> target) -> kind == FuncPtrParam) {
                    ;
                }*/
                next = next -> next;
            }
        }
    }

}

void riscv64_set_var_offset(Scope * scope, int * frame_offset) {
    int branch = 0;
    for (int i = 0; i < TableArraySize; i++) {
        if (scope -> symbolTable -> tableArray[i].key != NULL) {
            struct Value * next = &(scope -> symbolTable -> tableArray[i]);
            while (next != NULL) {
                ASTNode * var = (ASTNode *)next -> target;
                struct varinfo * info_ptr = &(next -> info);
                int type_loc = 1;
                if (var -> kind == Variable || var -> kind == ConstVariable || var -> kind == NormalParam) {
                    if (var -> kind == NormalParam)
                        type_loc = 0;
                    switch (var -> ptrs[type_loc] -> kind) {
                        case IntType:
                            *frame_offset = ROUND(*frame_offset, 4);
                            if (info_ptr -> isArray == 0) {
                                *frame_offset += 4;
                            }
                            else {
                                *frame_offset += info_ptr -> arrlen * 4;
                            }
                            break;
                        case CharType:
                            if (info_ptr -> isArray == 0) {
                                *frame_offset += 1;
                            }
                            else {
                                *frame_offset += info_ptr -> arrlen;
                            }
                            break;
                        case BoolType:
                            if (info_ptr -> isArray == 0) {
                                *frame_offset += 1;
                            }
                            else {
                                *frame_offset += info_ptr -> arrlen;
                            }
                            break;
                        case FloatType:
                            *frame_offset = ROUND(*frame_offset, 4);
                            if (info_ptr -> isArray == 0) {
                                *frame_offset += 4;
                            }
                            else {
                                *frame_offset += info_ptr -> arrlen * 4;
                            }
                            break;
                        case DoubleType:
                            *frame_offset = ROUND(*frame_offset, 8);
                            if (info_ptr -> isArray == 0) {
                                *frame_offset += 8;
                            }
                            else {
                                *frame_offset += info_ptr -> arrlen * 8;
                            }
                            break;
                        default:
                            break;
                    }
                    info_ptr -> frame_offset = -(*frame_offset);
                }
                if (var -> kind == Block) {
                    riscv64_set_var_offset(scope -> lowerLevel[branch], frame_offset);
                    branch++;
                }
                next = next -> next;
            }
        }
    }
}

void riscv64_put_abi_head(FILE * fp, int frame_offset, int ra_offset, int s0_offset) {
    fprintf(fp, "\t\taddi\t\tsp,sp,%d\n", -frame_offset);
    fprintf(fp, "\t\tsd\t\tra,%d(sp)\n", frame_offset + ra_offset);
    fprintf(fp, "\t\tsd\t\ts0,%d(sp)\n", frame_offset + s0_offset);
    fprintf(fp, "\t\taddi\t\ts0,sp,%d\n", frame_offset);
}

void riscv64_put_abi_tail(FILE * fp, ASTNode * node, int frame_offset, int ra_offset, int s0_offset) {
    if (node -> ptrs[1] -> kind != VoidType) {
        file_write(fp, "\t\tmv\t\ta0,t0\n");
    }
    fprintf(fp, "\t\tld\t\tra,%d(sp)\n", frame_offset + ra_offset);
    fprintf(fp, "\t\tld\t\ts0,%d(sp)\n", frame_offset + s0_offset);
    fprintf(fp, "\t\taddi\t\tsp,sp,%d\n", frame_offset);
    file_write(fp, "\t\tjr\t\tra\n");
}

void riscv64_put_binary_op(FILE * fp, ASTNode * node, Scope * scope) {
    int lfloat = 0, ldouble = 0, rfloat = 0, rdouble = 0;

    if (node -> ptrs[1] -> kind == Funcall) {
        riscv64_put_funcall(fp, node -> ptrs[1], scope);
        file_write(fp, "\t\tmv\t\tt2,a0\n");
    }

    if (node -> ptrs[0] -> kind == Identifier) {
        ASTNode * val1 = scope -> lookup(scope, node -> ptrs[0] -> image);
        if (global_info_ptr -> loc_type == LOCAL_VAR) {
            switch (val1 -> ptrs[1] -> kind) {
                case IntType:
                    fprintf(fp, "\t\tlw\t\tt1,%d(s0)\n", global_info_ptr -> frame_offset);
                    break;
                case FloatType:
                    fprintf(fp, "\t\tflw\t\tft1,%d(s0)\n", global_info_ptr -> frame_offset);
                    lfloat = 1;
                    break;
                case DoubleType:
                    fprintf(fp, "\t\tfld\t\tft1,%d(s0)\n", global_info_ptr -> frame_offset);
                    ldouble = 1;
                    break;
            }
        }
        else if (global_info_ptr -> loc_type == PARAM_VAR) {
            switch (val1 -> ptrs[0] -> kind) {
                case IntType:
                    fprintf(fp, "\t\tlw\t\tt1,%d(s0)\n", global_info_ptr -> frame_offset);
                    break;
                case FloatType:
                    fprintf(fp, "\t\tflw\t\tft1,%d(s0)\n", global_info_ptr -> frame_offset);
                    lfloat = 1;
                    break;
                case DoubleType:
                    fprintf(fp, "\t\tfld\t\tft1,%d(s0)\n", global_info_ptr -> frame_offset);
                    ldouble = 1;
                    break;
            }
        }
        else {
            fprintf(fp, "\t\tla\t\ta5,%s\n", val1 -> ptrs[2] -> ptrs[0] -> image);
            switch (val1 -> ptrs[1] -> kind) {
                case IntType:
                    file_write(fp, "\t\tlw\t\tt1,(a5)\n");
                    break;
                case FloatType:
                    file_write(fp, "\t\tflw\t\tft1,(a5)\n");
                    lfloat = 1;
                    break;
                case DoubleType:
                    file_write(fp, "\t\tfld\t\tft1,(a5)\n");
                    ldouble = 1;
                    break;
            }
        }
    }
    else if (node -> ptrs[0] -> kind == ArrayRef) {
        ASTNode * var = riscv64_calc_array_ref_addr(fp, node -> ptrs[0], scope);
        switch (var -> ptrs[1] -> kind) {
            case IntType:
                file_write(fp, "\t\tlw\t\tt1,(a5)\n");
                break;
            case FloatType:
                file_write(fp, "\t\tflw\t\tft1,(a5)\n");
                lfloat = 1;
                break;
            case DoubleType:
                file_write(fp, "\t\tfld\t\tft1,(a5)\n");
                ldouble = 1;
                break;
        }
    }

    if (node -> ptrs[1] -> kind == Identifier) {
        ASTNode * val2 = scope -> lookup(scope, node -> ptrs[1] -> image);
        if (global_info_ptr -> loc_type == LOCAL_VAR) {
            switch (val2 -> ptrs[1] -> kind) {
                case IntType:
                    fprintf(fp, "\t\tlw\t\tt2,%d(s0)\n", global_info_ptr -> frame_offset);
                    break;
                case FloatType:
                    fprintf(fp, "\t\tflw\t\tft2,%d(s0)\n", global_info_ptr -> frame_offset);
                    rfloat = 1;
                    break;
                case DoubleType:
                    fprintf(fp, "\t\tfld\t\tft2,%d(s0)\n", global_info_ptr -> frame_offset);
                    rdouble = 1;
                    break;
            }
        }
        else if (global_info_ptr -> loc_type == PARAM_VAR) {
            switch (val2 -> ptrs[0] -> kind) {
                case IntType:
                    fprintf(fp, "\t\tlw\t\tt2,%d(s0)\n", global_info_ptr -> frame_offset);
                    break;
                case FloatType:
                    fprintf(fp, "\t\tflw\t\tft2,%d(s0)\n", global_info_ptr -> frame_offset);
                    rfloat = 1;
                    break;
                case DoubleType:
                    fprintf(fp, "\t\tfld\t\tft2,%d(s0)\n", global_info_ptr -> frame_offset);
                    rdouble = 1;
                    break;
            }
        }
        else {
            fprintf(fp, "\t\tla\t\ta5,%s\n", val2 -> ptrs[2] -> ptrs[0] -> image);
            switch (val2 -> ptrs[1] -> kind) {
                case IntType:
                    file_write(fp, "\t\tlw\t\tt2,(a5)\n");
                    break;
                case FloatType:
                    file_write(fp, "\t\tflw\t\tft2,(a5)\n");
                    rfloat = 1;
                    break;
                case DoubleType:
                    file_write(fp, "\t\tfld\t\tft2,(a5)\n");
                    rdouble = 1;
                    break;
            }
        }
    }
    else if (node -> ptrs[1] -> kind == ArrayRef) {
        ASTNode * var = riscv64_calc_array_ref_addr(fp, node -> ptrs[1], scope);
        switch (var -> ptrs[1] -> kind) {
            case IntType:
                file_write(fp, "\t\tlw\t\tt2,(a5)\n");
                break;
            case FloatType:
                file_write(fp, "\t\tflw\t\tft2,(a5)\n");
                rfloat = 1;
                break;
            case DoubleType:
                file_write(fp, "\t\tfld\t\tft2,(a5)\n");
                rdouble = 1;
                break;
        }
    }

    if (node -> ptrs[0] -> kind == IntegerLiteral) {
        fprintf(fp, "\t\tli\t\tt1,%s\n", node -> ptrs[0] -> image);
        if (rfloat == 1 || rdouble == 1) {
            if (rfloat == 1) {
                file_write(fp, "\t\tfcvt.s.w\t\tft1,t1\n");
            }
            else {
                file_write(fp, "\t\tfcvt.d.l\t\tft1,t1\n");
            }
        }
    }
    else if (node -> ptrs[0] -> kind == FloatLiteral) {
        if (rfloat == 1) {
            float num = strtof(node -> ptrs[0] -> image, NULL);
            float * num_ptr = &num;
            unsigned output = *(unsigned *)num_ptr;
            fprintf(fp, "\t\tli\t\tt1,%u\n", output);
            file_write(fp, "\t\tfmv.w.x\t\tft1,t1\n");
        }
        else {
            double num = strtod(node -> ptrs[0] -> image, NULL);
            double * num_ptr = &num;
            unsigned long long output = *(unsigned long long *)num_ptr;
            fprintf(fp, "\t\tli\t\tt1,%llu\n", output);
            file_write(fp, "\t\tfmv.d.x\t\tft1,t1\n");
        }
    }

    if (node -> ptrs[1] -> kind == IntegerLiteral) {
        fprintf(fp, "\t\tli\t\tt2,%s\n", node -> ptrs[1] -> image);
        if (lfloat == 1 || ldouble == 1) {
            if (lfloat == 1) {
                file_write(fp, "\t\tfcvt.s.w\t\tft2,t2\n");
            }
            else {
                file_write(fp, "\t\tfcvt.d.l\t\tft2,t2\n");
            }
        }
    }
    else if (node -> ptrs[1] -> kind == FloatLiteral) {
        if (lfloat == 1) {
            float num = strtof(node -> ptrs[1] -> image, NULL);
            float * num_ptr = &num;
            unsigned output = *(unsigned *)num_ptr;
            fprintf(fp, "\t\tli\t\tt2,%u\n", output);
            file_write(fp, "\t\tfmv.w.x\t\tft2,t2\n");
        }
        else {
            double num = strtod(node -> ptrs[1] -> image, NULL);
            double * num_ptr = &num;
            unsigned long long output = *(unsigned long long *)num_ptr;
            fprintf(fp, "\t\tli\t\tt2,%llu\n", output);
            file_write(fp, "\t\tfmv.d.x\t\tft2,t2\n");
        }
    }

    if (lfloat == 1 || ldouble == 1 || rfloat == 1 || rdouble == 1) {
        if (!strcmp(node -> image, "+")) {
            if (lfloat == 1 || rfloat == 1) {
                file_write(fp, "\t\tfadd.s\t\tft0,ft1,ft2\n");
            }
            else {
                file_write(fp, "\t\tfadd.d\t\tft0,ft1,ft2\n");
            }
        }
        else if (!strcmp(node -> image, "-")) {
            if (lfloat == 1 || rfloat == 1) {
                file_write(fp, "\t\tfsub.s\t\tft0,ft1,ft2\n");
            }
            else {
                file_write(fp, "\t\tfsub.d\t\tft0,ft1,ft2\n");
            }
        }
        else if (!strcmp(node -> image, "*")) {
            if (lfloat == 1 || rfloat == 1) {
                file_write(fp, "\t\tfmul.s\t\tft0,ft1,ft2\n");
            }
            else {
                file_write(fp, "\t\tfmul.d\t\tft0,ft1,ft2\n");
            }
        }
        else if (!strcmp(node -> image, "/")) {
            if (lfloat == 1 || rfloat == 1) {
                file_write(fp, "\t\tfdiv.s\t\tft0,ft1,ft2\n");
            }
            else {
                file_write(fp, "\t\tfdiv.d\t\tft0,ft1,ft2\n");
            }
        }
    }
    else {
        if (!strcmp(node -> image, "+")) {
            file_write(fp, "\t\tadd\t\tt0,t1,t2\n");
        }
        else if (!strcmp(node -> image, "-")) {
            file_write(fp, "\t\tsub\t\tt0,t1,t2\n");
        }
        else if (!strcmp(node -> image, "*")) {
            file_write(fp, "\t\tmul\t\tt0,t1,t2\n");
        }
        else if (!strcmp(node -> image, "/")) {
            file_write(fp, "\t\tdiv\t\tt0,t1,t2\n");
        }
        else if (!strcmp(node -> image, "==")) {
            file_write(fp, "\t\tsext.w\t\tt1,t1\n");
            file_write(fp, "\t\tsext.w\t\tt2,t2\n");
            fprintf(fp, "\t\tbne\t\tt1,t2,.L%d\n", branch_label);
        }
        else if (!strcmp(node -> image, "<")) {
            file_write(fp, "\t\tsext.w\t\tt1,t1\n");
            file_write(fp, "\t\tsext.w\t\tt2,t2\n");
            fprintf(fp, "\t\tblt\t\tt1,t2,.L%d\n", branch_label);
        }
        else if (!strcmp(node -> image, ">")) {
            file_write(fp, "\t\tsext.w\t\tt1,t1\n");
            file_write(fp, "\t\tsext.w\t\tt2,t2\n");
            fprintf(fp, "\t\tble\t\tt1,t2,.L%d\n", branch_label);
        }
    }
}

void riscv64_put_funcall(FILE * fp, ASTNode * node, Scope * scope) {
    if (node -> ptrs[1] != NULL) {
        ASTNode * args = node -> ptrs[1];
        for (int i = 0; i < args -> listLen; i++) {
            ASTNode * val = NULL;
            switch (args -> list[i].kind) {
                case Identifier:
                    val = scope -> lookup(scope, args -> list[i].image);
                    if (global_info_ptr -> loc_type == LOCAL_VAR) {
                        switch (val -> ptrs[1] -> kind) {
                            case IntType:
                                fprintf(fp, "\t\tlw\t\ta%d,%d(s0)\n", i, global_info_ptr -> frame_offset);
                                break;
                            case FloatType:
                                fprintf(fp, "\t\tflw\t\tfa%d,%d(s0)\n", i, global_info_ptr -> frame_offset);
                                break;
                            case DoubleType:
                                fprintf(fp, "\t\tfld\t\tfa%d,%d(s0)\n", i, global_info_ptr -> frame_offset);
                                break;
                            case BoolType:
                                fprintf(fp, "\t\tlb\t\ta%d,%d(s0)\n", i, global_info_ptr -> frame_offset);
                                break;
                        }
                    }
                    break;
                case IntegerLiteral:
                    fprintf(fp, "\t\tli\t\ta%d,%s\n", i, args -> list[i].image);
                    break;
                case Funcall:
                    riscv64_put_funcall(fp, &(args -> list[i]), scope);
                    val = scope -> lookup(scope, args -> list[i].ptrs[0] -> image);
                    if (val -> ptrs[1] -> kind == IntType) {
                        fprintf(fp, "\t\tmv\t\ta%d,a0\n", i);
                    }
                    else if (val -> ptrs[1] -> kind == FloatType) {
                        fprintf(fp, "\t\tfmv.s\t\tfa%d,fa0\n", i);
                    }
                    else if (val -> ptrs[1] -> kind == DoubleType) {
                        fprintf(fp, "\t\tfmv.d\t\tfa%d,fa0\n", i);
                    }
                    break;
                case BinaryOp:
                    riscv64_put_binary_op(fp, &(args -> list[i]), scope);
                    fprintf(fp, "\t\tmv\t\ta%d,t0\n", i);
                    break;
                case ArrayRef:
                    val = riscv64_calc_array_ref_addr(fp, &(args -> list[i]), scope);
                    switch (val -> ptrs[1] -> kind) {
                        case IntType:
                            fprintf(fp, "\t\tlw\t\ta%d,(a5)\n", i);
                            break;
                        case FloatType:
                            fprintf(fp, "\t\tflw\t\ta%d,(a5)\n", i);
                            break;
                        case DoubleType:
                            fprintf(fp, "\t\tfld\t\ta%d,(a5)\n", i);
                            break;
                    }
                    break;
            }
        }
    }
    fprintf(fp, "\t\tcall\t\t%s\n", node -> ptrs[0] -> image);
}

void riscv64_put_while(FILE * fp, ASTNode * node, Scope * scope) {
    ASTNode * expr = node -> ptrs[0];
    int branch = 0;
    fprintf(fp, "\t\tj\t\t.L%d\n", branch_label);
    branch_label++;
    fprintf(fp, ".L%d:\n", branch_label);
    riscv64_put_stmt(fp, node -> ptrs[1] -> ptrs[0], scope, &branch);
    fprintf(fp, ".L%d:\n", branch_label - 1);
    riscv64_put_binary_op(fp, expr, scope);
    branch_label++;
}

void riscv64_put_if(FILE * fp, ASTNode * node, Scope * scope) {
    ASTNode * expr = node -> ptrs[0];
    int branch = 0;
    riscv64_put_binary_op(fp, expr, scope);
    branch_label++;
    riscv64_put_stmt(fp, node -> ptrs[1] -> ptrs[0], scope, &branch);
    fprintf(fp, "\t\tj\t\t.L%d\n", branch_label);
    fprintf(fp, ".L%d:\n", branch_label - 1);
    riscv64_put_stmt(fp, node -> ptrs[2] -> ptrs[0], scope, &branch);
    fprintf(fp, ".L%d:\n", branch_label);
    branch_label++;
}

ASTNode * riscv64_calc_array_ref_addr(FILE * fp, ASTNode * node, Scope * scope) {
    ASTNode * var = NULL;
    if (node -> ptrs[1] -> kind == Identifier) {
        var = scope -> lookup(scope, node -> ptrs[1] -> image);
        if (global_info_ptr -> loc_type == LOCAL_VAR) {
            switch (var -> ptrs[1] -> kind) {
                case IntType:
                    fprintf(fp, "\t\tlw\t\ta4,%d(s0)\n", global_info_ptr -> frame_offset);
                    break;
            }
        }
    }
    else if (node -> ptrs[1] -> kind == IntegerLiteral) {
        fprintf(fp, "\t\tli\t\ta4,%s\n", node -> ptrs[1] -> image);
    }


    if (node -> ptrs[0] -> kind == Identifier) {
        var = scope -> lookup(scope, node -> ptrs[0] -> image);
        if (global_info_ptr -> loc_type == GLOBAL_VAR) {
            fprintf(fp, "\t\tla\t\ta5,%s\n", node -> ptrs[0] -> image);
        }
        else if (global_info_ptr -> loc_type == LOCAL_VAR) {
            fprintf(fp, "\t\taddi\t\ta5,s0,%d\n", global_info_ptr -> frame_offset);
        }
        switch (var -> ptrs[1] -> kind) {
            case IntType:
                file_write(fp, "\t\tslli\t\ta4,a4,2\n");
                break;
            case FloatType:
                file_write(fp, "\t\tslli\t\ta4,a4,2\n");
                break;
            case DoubleType:
                file_write(fp, "\t\tslli\t\ta4,a4,3\n");
                break;
        }
        file_write(fp, "\t\tadd\t\ta5,a4,a5\n");
    }

    return var;
}

void riscv64_put_stmt(FILE * fp, ASTNode * stmt_node, Scope * scope, int * branch) {
    switch (stmt_node -> kind) {
        case DefinedVariables:
            for (int i = 0; i < stmt_node -> listLen; i++) {
                if (stmt_node -> list[i].kind == Variable) {
                    ASTNode * var = scope -> lookup(scope, stmt_node -> list[i].ptrs[2] -> ptrs[0] -> image);
                    if (var -> ptrs[3] != NULL) {
                        if (global_info_ptr -> loc_type == LOCAL_VAR) {
                            switch (var -> ptrs[1] -> kind) {
                                case IntType:
                                    if (var -> ptrs[3] -> kind == IntegerLiteral) {
                                        fprintf(fp, "\t\tli\t\tt0,%s\n", var -> ptrs[3] -> image);
                                        fprintf(fp, "\t\tsw\t\tt0,%d(s0)\n", global_info_ptr -> frame_offset);
                                    }
                                    break;
                                case FloatType:
                                    if (var -> ptrs[3] -> kind == FloatLiteral) {
                                        float num = strtof(var -> ptrs[3] -> image, NULL);
                                        float * num_ptr = &num;
                                        unsigned output = *(unsigned *)num_ptr;
                                        fprintf(fp, "\t\tli\t\tt0,%u\n", output);
                                        fprintf(fp, "\t\tsw\t\tt0,%d(s0)\n", global_info_ptr -> frame_offset);
                                    }
                                    break;
                                case DoubleType:
                                    if (var -> ptrs[3] -> kind == FloatLiteral) {
                                        double num = strtod(var -> ptrs[3] -> image, NULL);
                                        double * num_ptr = &num;
                                        unsigned long long output = *(unsigned long long *)num_ptr;
                                        fprintf(fp, "\t\tli\t\tt0,%llu\n", output);
                                        fprintf(fp, "\t\tsd\t\tt0,%d(s0)\n", global_info_ptr -> frame_offset);
                                    }
                                    break;
                                case BoolType:
                                    if (var -> ptrs[3] -> kind == BoolLiteral) {
                                        if (!strcmp(var -> ptrs[3] -> image, "true")) {
                                            file_write(fp, "\t\tli\t\tt0,1\n");
                                            fprintf(fp, "\t\tsb\t\tt0,%d(s0)\n", global_info_ptr -> frame_offset);
                                        }
                                        else {
                                            file_write(fp, "\t\tli\t\tt0,0\n");
                                            fprintf(fp, "\t\tsb\t\tt0,%d(s0)\n", global_info_ptr -> frame_offset);
                                        }
                                    }
                                    break;
                            }
                        }
                    }
                }
            }
            break;
        case DefinedConstVariables:
            for (int i = 0; i < stmt_node -> listLen; i++) {
                if (stmt_node -> list[i].kind == ConstVariable) {
                    ASTNode * var = scope -> lookup(scope, stmt_node -> list[i].ptrs[2] -> ptrs[0] -> image);
                    if (var -> ptrs[3] != NULL) {
                        if (global_info_ptr -> loc_type == LOCAL_VAR) {
                            switch (var -> ptrs[1] -> kind) {
                                case IntType:
                                    if (var -> ptrs[3] -> kind == IntegerLiteral) {
                                        fprintf(fp, "\t\tli\t\tt0,%s\n", var -> ptrs[3] -> image);
                                        fprintf(fp, "\t\tsw\t\tt0,%d(s0)\n", global_info_ptr -> frame_offset);
                                    }
                                    break;
                                case FloatType:
                                    if (var -> ptrs[3] -> kind == FloatLiteral) {
                                        float num = strtof(var -> ptrs[3] -> image, NULL);
                                        float * num_ptr = &num;
                                        unsigned output = *(unsigned *)num_ptr;
                                        fprintf(fp, "\t\tli\t\tt0,%u\n", output);
                                        fprintf(fp, "\t\tsw\t\tt0,%d(s0)\n", global_info_ptr -> frame_offset);
                                    }
                                    break;
                                case DoubleType:
                                    if (var -> ptrs[3] -> kind == FloatLiteral) {
                                        double num = strtod(var -> ptrs[3] -> image, NULL);
                                        double * num_ptr = &num;
                                        unsigned long long output = *(unsigned long long *)num_ptr;
                                        fprintf(fp, "\t\tli\t\tt0,%llu\n", output);
                                        fprintf(fp, "\t\tsd\t\tt0,%d(s0)\n", global_info_ptr -> frame_offset);
                                    }
                                    break;
                                case BoolType:
                                    if (var -> ptrs[3] -> kind == BoolLiteral) {
                                        if (!strcmp(var -> ptrs[3] -> image, "true")) {
                                            file_write(fp, "\t\tli\t\tt0,1\n");
                                            fprintf(fp, "\t\tsb\t\tt0,%d(s0)\n", global_info_ptr -> frame_offset);
                                        }
                                        else {
                                            file_write(fp, "\t\tli\t\tt0,0\n");
                                            fprintf(fp, "\t\tsb\t\tt0,%d(s0)\n", global_info_ptr -> frame_offset);
                                        }
                                    }
                                    break;
                            }
                        }
                    }
                }
            }
            break;
        case Assign:
            if (stmt_node -> ptrs[1] -> kind == IntegerLiteral) {
                fprintf(fp, "\t\tli\t\tt0,%s\n", stmt_node -> ptrs[1] -> image);
            }
            else if (stmt_node -> ptrs[1] -> kind == BoolLiteral) {
                if (!strcmp(stmt_node -> ptrs[1] -> image, "true")) {
                    file_write(fp, "\t\tli\t\tt0,1\n");
                }
                else {
                    file_write(fp, "\t\tli\t\tt0,0\n");
                }
            }
            else if (stmt_node -> ptrs[1] -> kind == BinaryOp) {
                riscv64_put_binary_op(fp, stmt_node -> ptrs[1], scope);
            }
            else if (stmt_node -> ptrs[1] -> kind == Funcall) {
                riscv64_put_funcall(fp, stmt_node -> ptrs[1], scope);
                ASTNode * func_node = scope -> lookup(scope, stmt_node -> ptrs[1] -> ptrs[0] -> image);
                if (func_node -> ptrs[1] -> kind == FloatType) {
                    file_write(fp, "\t\tfmv.s\t\tft0,fa0\n");
                }
                else if (func_node -> ptrs[1] -> kind == DoubleType) {
                    file_write(fp, "\t\tfmv.d\t\tft0,fa0\n");
                }
                else {
                    file_write(fp, "\t\tmv\t\tt0,a0\n");
                }
            }
            else if (stmt_node -> ptrs[1] -> kind == Identifier) {
                ASTNode * rval = scope -> lookup(scope, stmt_node -> ptrs[1] -> image);
                if (global_info_ptr -> loc_type == LOCAL_VAR) {
                    switch (rval -> ptrs[1] -> kind) {
                        case IntType:
                            fprintf(fp, "\t\tlw\t\tt0,%d(s0)\n", global_info_ptr -> frame_offset);
                            break;
                        case FloatType:
                            fprintf(fp, "\t\tflw\t\tft0,%d(s0)\n", global_info_ptr -> frame_offset);
                            break;
                        case DoubleType:
                            fprintf(fp, "\t\tfld\t\tft0,%d(s0)\n", global_info_ptr -> frame_offset);
                            break;
                        case BoolType:
                            fprintf(fp, "\t\tlb\t\tt0,%d(s0)\n", global_info_ptr -> frame_offset);
                            break;
                    }
                }
            }

            if (stmt_node -> ptrs[0] -> kind == Identifier) {
                ASTNode * lval = scope -> lookup(scope, stmt_node -> ptrs[0] -> image);
                if (global_info_ptr -> loc_type == LOCAL_VAR) {
                    switch (lval -> ptrs[1] -> kind) {
                        case IntType:
                            fprintf(fp, "\t\tsw\t\tt0,%d(s0)\n", global_info_ptr -> frame_offset);
                            break;
                        case FloatType:
                            if (stmt_node -> ptrs[1] -> kind == FloatLiteral) {
                                float num = strtof(stmt_node -> ptrs[1] -> image, NULL);
                                float * num_ptr = &num;
                                unsigned output = *(unsigned *)num_ptr;
                                fprintf(fp, "\t\tli\t\tt0,%u\n", output);
                                fprintf(fp, "\t\tsw\t\tt0,%d(s0)\n", global_info_ptr -> frame_offset);
                            }
                            else {
                                fprintf(fp, "\t\tfsw\t\tft0,%d(s0)\n", global_info_ptr -> frame_offset);
                            }
                            break;
                        case DoubleType:
                            if (stmt_node -> ptrs[1] -> kind == FloatLiteral) {
                                double num = strtod(stmt_node -> ptrs[1] -> image, NULL);
                                double * num_ptr = &num;
                                unsigned long long output = *(unsigned long long *)num_ptr;
                                fprintf(fp, "\t\tli\t\tt0,%llu\n", output);
                                fprintf(fp, "\t\tsd\t\tt0,%d(s0)\n", global_info_ptr -> frame_offset);
                            }
                            else {
                                fprintf(fp, "\t\tfsd\t\tft0,%d(s0)\n", global_info_ptr -> frame_offset);
                            }
                            break;
                        case BoolType:
                            fprintf(fp, "\t\tsb\t\tt0,%d(s0)\n", global_info_ptr -> frame_offset);
                            break;
                    }
                }
            }
            else if (stmt_node -> ptrs[0] -> kind == ArrayRef) {
                ASTNode * var = riscv64_calc_array_ref_addr(fp, stmt_node -> ptrs[0], scope);
                switch (var -> ptrs[1] -> kind) {
                    case IntType:
                        file_write(fp, "\t\tsw\t\tt0,(a5)\n");
                        break;
                    case FloatType:
                        file_write(fp, "\t\tfsw\t\tft0,(a5)\n");
                        break;
                    case DoubleType:
                        file_write(fp, "\t\tfsd\t\tft0,(a5)\n");
                        break;
                }
            }
            break;
        case Return:
            if (stmt_node -> ptrs[0] == NULL) {
                break;
            }

            if (stmt_node -> ptrs[0] -> kind == Identifier) {
                ASTNode * val = scope -> lookup(scope, stmt_node -> ptrs[0] -> image);
                if (global_info_ptr -> loc_type == LOCAL_VAR) {
                    switch (val -> ptrs[1] -> kind) {
                        case IntType:
                            fprintf(fp, "\t\tlw\t\tt0,%d(s0)\n", global_info_ptr -> frame_offset);
                            break;
                    }
                }
                else if (global_info_ptr -> loc_type == PARAM_VAR) {
                    switch (val -> ptrs[0] -> kind) {
                        case IntType:
                            fprintf(fp, "\t\tlw\t\tt0,%d(s0)\n", global_info_ptr -> frame_offset);
                            break;
                    }
                }
            }
            else if (stmt_node -> ptrs[0] -> kind == IntegerLiteral) {
                fprintf(fp, "\t\tli\t\tt0,%s\n", stmt_node -> ptrs[0] -> image);
            }
            else if (stmt_node -> ptrs[0] -> kind == BinaryOp) {
                riscv64_put_binary_op(fp, stmt_node -> ptrs[0], scope);
            }
            break;
        case If:
            riscv64_put_if(fp, stmt_node, scope);
            break;
        case While:
            riscv64_put_while(fp, stmt_node, scope);
            break;
        case Funcall:
            riscv64_put_funcall(fp, stmt_node, scope);
            break;
        case Block:
            riscv64_put_block(fp, stmt_node, scope -> lowerLevel[*branch]);
            *branch = *branch + 1;
            break;
        default:
            break;
    }
}

void riscv64_put_block(FILE * fp, ASTNode * block, Scope * scope) {
    ASTNode * stmt = NULL;
    int branch = 0;
    for (int i = 0; i < block -> ptrs[0] -> listLen; i++) {
        stmt = &(block -> ptrs[0] -> list[i]);
        ASTNode * stmt_node = stmt -> ptrs[0];
        riscv64_put_stmt(fp, stmt_node, scope, &branch);
    }
}

void riscv64_put_param(FILE * fp, Scope * scope) {
    int num = 0;
    for (int i = 0; i < TableArraySize; i++) {
        if (scope -> symbolTable -> tableArray[i].key != NULL) {
            struct Value * next = &(scope -> symbolTable -> tableArray[i]);
            while (next != NULL) {
                ASTNode * var = (ASTNode *)next -> target;
                struct varinfo * info_ptr = &(next -> info);
                if (var -> kind == NormalParam) {
                    switch (var -> ptrs[0] -> kind) {
                        case IntType:
                            fprintf(fp, "\t\tsw\t\ta%d,%d(s0)\n", num, info_ptr -> frame_offset);
                            num++;
                            break;
                        case CharType:
                            fprintf(fp, "\t\tsb\t\ta%d,%d(s0)\n", num, info_ptr -> frame_offset);
                            num++;
                            break;
                        case BoolType:
                            fprintf(fp, "\t\tsb\t\ta%d,%d(s0)\n", num, info_ptr -> frame_offset);
                            num++;
                            break;
                        case FloatType:
                            fprintf(fp, "\t\tfsw\t\tfa%d,%d(s0)\n", num, info_ptr -> frame_offset);
                            num++;
                            break;
                        case DoubleType:
                            fprintf(fp, "\t\tfsd\t\tfa%d,%d(s0)\n", num, info_ptr -> frame_offset);
                            num++;
                            break;
                        default:
                            break;
                    }
                }
                next = next -> next;
            }
        }
    }
}

void riscv64_put_func(FILE * fp, ASTNode * node, Scope * scope) {
    int frame_offset = 16, ra_offset = -8, s0_offset = -16;
    char * funcname = node -> ptrs[2] -> image;
    riscv64_set_var_offset(scope, &frame_offset);
    frame_offset = ROUND(frame_offset, 8);
    file_write(fp, "\t\t.text\n");
    file_write(fp, "\t\t.align\t1\n");
    fprintf(fp, "\t\t.globl\t%s\n", funcname);
    fprintf(fp, "\t\t.type\t%s, @function\n", funcname);
    fprintf(fp, "%s:\n", funcname);

    riscv64_put_abi_head(fp, frame_offset, ra_offset, s0_offset);
    riscv64_put_param(fp, scope);
    riscv64_put_block(fp, node -> ptrs[4], scope -> lowerLevel[0]);




    riscv64_put_abi_tail(fp, node, frame_offset, ra_offset, s0_offset);

    fprintf(fp, "\t\t.size\t%s, .-%s\n\n", funcname, funcname);
}

void riscv64_codegen(ASTNode * root, Scope * scope) {
    char * codefile_path = riscv64_new_filepath();
    FILE * codefile = fopen(codefile_path, "w");
    
    riscv64_put_header(codefile);
    riscv64_put_globl_var(codefile, scope);
    
    int branch = 0;
    for (int i = 0; i < TableArraySize; i++) {
        if (scope -> symbolTable -> tableArray[i].key != NULL) {
            struct Value * next = &(scope -> symbolTable -> tableArray[i]);
            while (next != NULL) {
                ASTNode * var = (ASTNode *)next -> target;
                struct varinfo * info_ptr = &(next -> info);
                if (var -> kind == DefinedFunc) {
                    riscv64_put_func(codefile, var, scope -> lowerLevel[branch]);
                    branch++;
                }
                /*if (var -> kind == DefinedFunc) {
                    branch++;
                }*/
                next = next -> next;
            }
        }
    }

    fclose(codefile);
}
