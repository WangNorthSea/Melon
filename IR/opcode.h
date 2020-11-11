
#ifndef __OPCODE_H__
#define __OPCODE_H__

/* arithmetic ops */
#define IR_ADDS     0
#define IR_ADDU     1
#define IR_SUBS     2
#define IR_SUBU     3
#define IR_MULS     4
#define IR_MULU     5
#define IR_DIVS     6
#define IR_DIVU     7
#define IR_MOD      8

/* memory ops */
#define IR_LOAD     9
#define IR_STORE    10

/* control flow ops */
#define IR_BR       11
#define IR_CALL     12
#define IR_RETURN   13

#endif /* opcode_h */
