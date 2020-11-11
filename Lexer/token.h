
#ifndef token_h
#define token_h

#include "../List/list.h"

struct Token {
    struct list_head list;
    int kind;
    int beginLine;
    char * image;
    fpos_t fpos;    /* the next pos of the last byte of a Token */
};

typedef struct Token Token;

#endif /* token_h */
