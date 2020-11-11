
#ifndef __IR_H__
#define __IR_H__

#include "../List/list.h"
#include "../ASTNode/node.h"

struct ThreeAddrCode {
    struct list_head list;
    char op;
    char dest_basic_type;
    char src1_basic_type;
    char src2_basic_type;
    char * dest;
    char * src1;
    char * src2;
    ASTNode * dest_node;
    ASTNode * src1_node;
    ASTNode * src2_node;
};

struct BasicBlock {
    struct list_head ir_code;
};

struct CFGNode {
    struct list_head predecessors;
    struct list_head successors;
    struct BasicBlock basic_block;
};

struct CtrlFlowGraph {
    struct list_head list;
    char * function;
    ASTNode * func_node;
    struct CFGNode * entry;
    struct CFGNode * exit;
};

typedef struct ThreeAddrCode ThreeAddrCode;
typedef struct BasicBlock BasicBlock;
typedef struct CFGNode CFGNode;
typedef struct CtrlFlowGraph CtrlFlowGraph;

#endif /* ir_h */
