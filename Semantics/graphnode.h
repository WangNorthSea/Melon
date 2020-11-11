
#ifndef graphnode_h
#define graphnode_h

#define Searching 0
#define Finished 1
#define New 2

struct GNode {
    ASTNode * type;
    int mark;
    int listLen;
    struct GNode ** neighbors;
    void (*appendNeighbor)(struct GNode * appender, struct GNode * toAppend);
};

typedef struct GNode GNode;

extern GNode * GNodeConstructor(GNode * lastStop, ASTNode * type);
extern int checkClosure(void);

#endif /* graphnode_h */
