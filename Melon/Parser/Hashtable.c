//
//  Hashtable.c
//  Melon
//
//  Created by 王浩宇 on 2019/3/26.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#include <stdlib.h>
#include <string.h>
#include "../ASTNode/node.h"
#include "hashtable.h"

unsigned int ELFHash(char * str) {
    unsigned int hash = 0;
    unsigned int x = 0;
    
    while (*str != '\0') {
        hash = (hash << 4) + *str;
        str++;
        if ((x = hash & 0xf0000000) != 0) {
            hash ^= x >> 24;
            hash &= ~x;
        }
    }
    
    return hash & 0x7fffffff;
}

void put(Hashtable * table, char * key, ASTNode * value) {
    unsigned int hashValue = ELFHash(key);
    int arrayLoc = hashValue % TableArraySize;
    Value * temp = NULL;
    
    if (table -> tableArray[arrayLoc].key == NULL) {
        table -> tableArray[arrayLoc].target = value;
        table -> tableArray[arrayLoc].key = key;
    }
    else {
        temp = &table -> tableArray[arrayLoc];
        while (temp -> next != NULL)
            temp = temp -> next;
        temp -> next = (Value *)malloc(sizeof(Value));
        temp = temp -> next;
        temp -> next = NULL;
        temp -> target = value;
        temp -> key = key;
    }
}

ASTNode * get(struct Hashtable * table, char * key) {
    unsigned int hashValue = ELFHash(key);
    int arrayLoc = hashValue % TableArraySize;
    Value * temp = &table -> tableArray[arrayLoc];
    
    while (temp != NULL) {
        if (temp -> key != NULL) {
            if (!strcmp(temp -> key, key))
                return temp -> target;
            else
                temp = temp -> next;
        }
        else
            return NULL;
    }
    return NULL;
}

Hashtable * HashtableConstructor(void) {
    int i;
    Hashtable * table = (Hashtable *)malloc(sizeof(Hashtable));
    table -> get = get;
    table -> put = put;
    
    for (i = 0; i < TableArraySize; i++) {
        table -> tableArray[i].key = NULL;
        table -> tableArray[i].next = NULL;
        table -> tableArray[i].target = NULL;
    }
    
    return table;
}
