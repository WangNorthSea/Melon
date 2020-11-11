
#include <stdlib.h>
#include "opcode.h"
#include "ir.h"
#include "../List/list.h"
#include "../ASTNode/node.h"

ThreeAddrCode * threeAddrCodeGen(char op, char dest_basic_type, char src1_basic_type, char src2_basic_type, char * dest, char * src1, char * src2, ASTNode * dest_node, ASTNode * src1_node, ASTNode * src2_node) {
    ThreeAddrCode * new_code = (ThreeAddrCode *)malloc(sizeof(ThreeAddrCode));
    init_list_head(&new_code -> list);
    new_code -> op = op;
    new_code -> dest_basic_type = dest_basic_type;
    new_code -> src1_basic_type = src1_basic_type;
    new_code -> src2_basic_type = src2_basic_type;
    new_code -> dest = dest;
    new_code -> src1 = src1;
    new_code -> src2 = src2;
    new_code -> dest_node = dest_node;
    new_code -> src1_node = src1_node;
    new_code -> src2_node = src2_node;
    return new_code;
}

CFGNode * CFGNodeGen(void) {
    CFGNode * node = (CFGNode *)malloc(sizeof(CFGNode));
    init_list_head(&node -> predecessors);
    init_list_head(&node -> successors);
    init_list_head(&node->basic_block.ir_code);
    return node;
}

CtrlFlowGraph * CFGGen(char * function, ASTNode * func_node) {
    CtrlFlowGraph * cfg = (CtrlFlowGraph *)malloc(sizeof(CtrlFlowGraph));
    cfg -> function = function;
    cfg -> func_node = func_node;
    cfg -> entry = NULL;
    cfg -> exit = NULL;
    return cfg;
}
