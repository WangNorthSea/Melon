//
//  node.h
//  Melon
//
//  Created by 王浩宇 on 2019/3/18.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#ifndef node_h
#define node_h

//抽象语法书根节点
typedef struct {
    int line;
} AST;

//表达式节点
typedef struct {
    int line;
} ExprNode;

//结构体成员节点
typedef struct {
    int line;
} Slot;

//语句节点
typedef struct {
    int line;
} StmtNode;

//类型定义节点
typedef struct {
    int line;
} TypeDefinition;

//存储类型节点
typedef struct {
    int line;
} TypeNode;

#endif /* node_h */
