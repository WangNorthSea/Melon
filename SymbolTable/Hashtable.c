
#include <stdlib.h>
#include <string.h>
#include "../ASTNode/node.h"
#include "hashtable.h"

struct varinfo * global_info_ptr = NULL;

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

void put(Hashtable * table, char * key, void * value, int loc_type, int isArray, int arrlen) {
    unsigned int hashValue = ELFHash(key);
    int arrayLoc = hashValue % TableArraySize;
    Value * temp = NULL;
    
    if (table -> tableArray[arrayLoc].key == NULL) {
        table -> tableArray[arrayLoc].target = value;
        table -> tableArray[arrayLoc].key = key;
        table -> tableArray[arrayLoc].info.loc_type = loc_type;
        table -> tableArray[arrayLoc].info.isArray = isArray;
        table -> tableArray[arrayLoc].info.arrlen = arrlen;
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
        temp -> info.loc_type = loc_type;
        temp -> info.isArray = isArray;
        temp -> info.arrlen = arrlen;
    }
}

void * get(struct Hashtable * table, char * key) {
    unsigned int hashValue = ELFHash(key);
    int arrayLoc = hashValue % TableArraySize;
    Value * temp = &table -> tableArray[arrayLoc];
    
    while (temp != NULL) {
        if (temp -> key != NULL) {
            if (!strcmp(temp -> key, key)) {
                global_info_ptr = &(temp -> info);
                return temp -> target;
            }
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
        table -> tableArray[i].info.arrlen = 0;
        table -> tableArray[i].info.frame_offset = 0;
        table -> tableArray[i].info.isArray = 0;
        table -> tableArray[i].info.loc_type = 0;
    }
    
    return table;
}
