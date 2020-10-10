//
//  IrConstructor.c
//  Melon
//
//  Created by 王浩宇 on 2019/4/26.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include "opcode.h"
#include "ir.h"

void BlockAppend(struct IR * appender, struct BasicBlock * block) {
    appender -> blockCount++;
    appender -> blocks = (BasicBlock **)realloc(appender -> blocks, sizeof(BasicBlock *) * (appender -> blockCount));
    appender -> blocks[appender -> blockCount - 1] = block;
}

void PhiAppend(BasicBlock * appender, Quad * phiQuad) {
    appender -> phiCount++;
    appender -> phis = (Quad **)realloc(appender -> phis, sizeof(Quad *) * (appender -> phiCount));
    appender -> phis[appender -> phiCount - 1] = phiQuad;
}

Quad * QuadConstructor(char op, Quad * src1, Quad * src2, Quad * prev) {
    Quad * quad = (Quad *)malloc(sizeof(Quad));
    quad -> op = op;
    quad -> src1 = src1;
    quad -> src2 = src2;
    quad -> prev = prev;
    quad -> next = NULL;
    return quad;
}

BasicBlock * BlockConstructor(IR * ir) {
    BasicBlock * block = (BasicBlock *)malloc(sizeof(BasicBlock));
    block -> head = NULL;
    block -> currentQuad = NULL;
    block -> phis = NULL;
    block -> PhiAppend = PhiAppend;
    block -> phiCount = 0;
    ir -> BlockAppend(ir, block);
    return block;
}

IR * IRConstructor(void) {
    IR * ir = (IR *)malloc(sizeof(IR));
    ir -> blockCount = 0;
    ir -> blocks = NULL;
    ir -> BlockAppend = BlockAppend;
    ir -> start = BlockConstructor(ir);
    return ir;
}
