//
//  ir.h
//  Melon
//
//  Created by 王浩宇 on 2019/4/26.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#ifndef ir_h
#define ir_h

struct Quad {
    char op;
    struct Quad * src1;
    struct Quad * src2;
    struct Quad * prev;
    struct Quad * next;
};

struct BasicBlock {
    struct Quad * head;
    struct Quad * currentQuad;
    struct Quad ** phis;
    void (*PhiAppend)(struct BasicBlock * appender, struct Quad * phiQuad);
    int phiCount;
};

struct IR {
    struct BasicBlock * start;
    struct BasicBlock ** blocks;
    void (*BlockAppend)(struct IR * appender, struct BasicBlock * block);
    int blockCount;
};

typedef struct Quad Quad;
typedef struct BasicBlock BasicBlock;
typedef struct IR IR;

#endif /* ir_h */
