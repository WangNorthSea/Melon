//
//  node.h
//  Melon
//
//  Created by 王浩宇 on 2019/3/18.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#ifndef node_h
#define node_h

struct ASTNode {
    int kind;
    char * file;
    int line;
    char * image;
    struct ASTNode * ptr1;
    struct ASTNode * ptr2;
    struct ASTNode * ptr3;
    struct ASTNode * ptr4;
    struct ASTNode * ptr5;
    struct ASTNode * ptr6;
};

typedef struct ASTNode ASTNode;

#endif /* node_h */
