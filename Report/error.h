
#ifndef __ERROR_H__
#define __ERROR_H__

#include "../List/list.h"
#include "report.h"
#include "../Lexer/token.h"

struct error_t {
    struct list_head list;
    int line;
    int type;
    char * file;
    char * expected;
    Token * lastToken;
};

typedef struct error_t error_t;

extern error_t * ErrorConstructor(int line, int type, char * file, char * expected, Token * token);
extern void dumpErrorList(error_t * err_list, FILE * fp);

#endif /* error.h */