//
//  Constructor.c
//  Melon
//
//  Created by 王浩宇 on 2019/3/21.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "node.h"

ASTNode * NodeConstructor(int kind, const char * file, int line, char * image, ASTNode * ptrs[6]) {
    ASTNode * node = (ASTNode *)malloc(sizeof(ASTNode));
    node -> kind = kind;
    node -> file = (char *)malloc(sizeof(char) * strlen(file));
    strcpy(node -> file, file);
    node -> line = line;
    node -> image = (char *)malloc(sizeof(char) * strlen(image));
    strcpy(node -> image, image);
    node -> ptr1 = ptrs[0];
    node -> ptr2 = ptrs[1];
    node -> ptr3 = ptrs[2];
    node -> ptr4 = ptrs[3];
    node -> ptr5 = ptrs[4];
    node -> ptr6 = ptrs[5];
    
    return node;
}
