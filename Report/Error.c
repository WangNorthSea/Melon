#include <stdio.h>
#include <stdlib.h>
#include "error.h"

error_t * ErrorConstructor(int line, int type, char * file, char * expected, Token * token) {
    error_t * err = (error_t *)malloc(sizeof(error_t));
    init_list_head(&err -> list);
    err -> line = line;
    err -> type = type;
    err -> file = file;
    err -> expected = expected;
    err -> lastToken = token;
    return err;
}

void dumpErrorList(error_t * err_list, FILE * fp) {
    return;
}