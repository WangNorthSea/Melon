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

void append(int listLen, ASTNode * list, ASTNode ToAppend) {
    listLen++;
    list = (ASTNode *)realloc(list, sizeof(ASTNode) * (listLen));
    list[listLen - 1] = ToAppend;
}

ASTNode * NodeConstructor(int kind, const char * file, int line, char * image, ASTNode * ptrs[6]) {
    int i;
    
    ASTNode * node = (ASTNode *)malloc(sizeof(ASTNode));
    node -> kind = kind;
    node -> listLen = 0;
    node -> file = (char *)malloc(sizeof(char) * strlen(file));
    strcpy(node -> file, file);
    node -> line = line;
    
    if (image != NULL) {
        node -> image = (char *)malloc(sizeof(char) * strlen(image));
        strcpy(node -> image, image);
    }
    else
        node -> image = NULL;
    
    for (i = 0; i < 6; i++)
        node -> ptrs[i] = ptrs[i];
    
    node -> list = NULL;
    node -> append = append;
    
    return node;
}
