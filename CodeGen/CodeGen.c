
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

void riscv64__put_globl_int(FILE * fp, ASTNode * var, struct varinfo * info_ptr) {
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

void riscv64__put_globl_char(FILE * fp, ASTNode * var, struct varinfo * info_ptr) {
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

void riscv64__put_globl_float(FILE * fp, ASTNode * var, struct varinfo * info_ptr) {
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

void riscv64__put_globl_double(FILE * fp, ASTNode * var, struct varinfo * info_ptr) {
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

void riscv64__put_globl_var(FILE * fp, Scope * scope) {
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
                            riscv64__put_globl_int(fp, var, info_ptr);
                            break;
                        case CharType:
                            riscv64__put_globl_char(fp, var, info_ptr);
                            break;
                        case BoolType:
                            riscv64__put_globl_char(fp, var, info_ptr);
                            break;
                        case FloatType:
                            riscv64__put_globl_float(fp, var, info_ptr);
                            break;
                        case DoubleType:
                            riscv64__put_globl_double(fp, var, info_ptr);
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

void riscv64__set_var_offset(Scope * scope, int * frame_offset) {
    int branch = 0;
    for (int i = 0; i < TableArraySize; i++) {
        if (scope -> symbolTable -> tableArray[i].key != NULL) {
            struct Value * next = &(scope -> symbolTable -> tableArray[i]);
            while (next != NULL) {
                ASTNode * var = (ASTNode *)next -> target;
                struct varinfo * info_ptr = &(next -> info);
                if (var -> kind == Variable || var -> kind == ConstVariable) {
                    switch (var -> ptrs[1] -> kind) {
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
                    riscv64__set_var_offset(scope -> lowerLevel[branch], frame_offset);
                    branch++;
                }
                next = next -> next;
            }
        }
    }
}

void riscv64__put_func(FILE * fp, ASTNode * node, Scope * scope) {
    int frame_offset = 0;
    riscv64__set_var_offset(scope, &frame_offset);
    
}

void riscv64__codegen(ASTNode * root, Scope * scope) {
    char * codefile_path = riscv64__new_filepath();
    FILE * codefile = fopen(codefile_path, "w");
    
    riscv64__put_header(codefile);
    riscv64__put_globl_var(codefile, scope);
    
    int branch = 0;
    for (int i = 0; i < TableArraySize; i++) {
        if (scope -> symbolTable -> tableArray[i].key != NULL) {
            struct Value * next = &(scope -> symbolTable -> tableArray[i]);
            while (next != NULL) {
                ASTNode * var = (ASTNode *)next -> target;
                struct varinfo * info_ptr = &(next -> info);
                if (var -> kind == DefinedFunc) {
                    riscv64__put_func(codefile, var, scope -> lowerLevel[branch]);
                    branch++;
                }
                next = next -> next;
            }
        }
    }

    fclose(codefile);
}
