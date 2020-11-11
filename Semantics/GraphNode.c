
#include <stdlib.h>
#include "../ASTNode/node.h"
#include "graphnode.h"

GNode ** nodes = NULL;
int nodeCount = 0;

int walkThrough(GNode * node);

void appendNeighbor(GNode * appender, GNode * toAppend) {
    appender -> listLen++;
    appender -> neighbors = (GNode **)realloc(appender -> neighbors, sizeof(GNode *) * appender -> listLen);
    appender -> neighbors[appender -> listLen - 1] = toAppend;
}

GNode * GNodeConstructor(GNode * lastStop, ASTNode * type) {
    int i;
    for (i = 0; i < nodeCount; i++) {
        if (nodes[i] -> type -> kind == type -> kind && nodes[i] -> type -> listLen == type -> listLen) {
            if (lastStop != NULL) {
                lastStop -> appendNeighbor(lastStop, nodes[i]);
                return NULL;
            }
        }
    }
    
    GNode * node = (GNode *)malloc(sizeof(GNode));
    node -> type = type;
    node -> mark = New;
    node -> listLen = 0;
    node -> neighbors = NULL;
    node -> appendNeighbor = appendNeighbor;
    
    if (lastStop != NULL)
        lastStop -> appendNeighbor(lastStop, node);
    
    nodeCount++;
    nodes = (GNode **)realloc(nodes, sizeof(GNode *) * nodeCount);
    nodes[nodeCount - 1] = node;
    
    return node;
}

int checkClosure(void) {
    int i, result = 0;
    for (i = 0; i < nodeCount; i++) {
        result = walkThrough(nodes[i]);
        if (result == 1)
            return 1;
    }
    
    return 0;
}

int walkThrough(GNode * node) {
    int i, result = 0;
    
    switch (node -> mark) {
        case New:
            node -> mark = Searching;
            break;
        case Finished:
            return 0;
        case Searching:
            return 1;
        default:
            break;
    }
    
    for (i = 0; i < node -> listLen; i++) {
        result = walkThrough(node -> neighbors[i]);
        if (result == 1)
            return 1;
    }
    
    node -> mark = Finished;
    return 0;
}
