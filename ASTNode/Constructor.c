
#include <stdlib.h>
#include <string.h>
#include "node.h"

void append(ASTNode * appender, ASTNode ToAppend) {
    appender -> listLen++;
    appender -> list = (ASTNode *)realloc(appender -> list, sizeof(ASTNode) * (appender -> listLen));
    appender -> list[appender -> listLen - 1] = ToAppend;
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
