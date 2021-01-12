#include "fileop.h"

void file_write(FILE * fp, char * str) {
    fwrite(str, 1, strlen(str), fp);
}