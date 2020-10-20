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
    error_t * p = NULL;
    char line_buffer[1024] = {0};
    int line_index = 0;
    list_for_each_entry(p, &err_list -> list, list) {
        switch (p -> type) {
            case LEX:
                printf("Melon: %s: lexical \033[31merror\033[0m in line %d expected %s\n", p -> file, p -> line, p -> expected);
                break;
            case SYNTAX:
                printf("Melon: %s: syntax \033[31merror\033[0m in line %d expected %s\n", p -> file, p -> line, p -> expected);
                break;
            default:
                printf("Melon: %s: semantic \033[31merror\033[0m in line %d expected %s\n", p -> file, p -> line, p -> expected);
                break;
        }

        int read_line = 1;
        char ch = 0;
        fseek(fp, 0, SEEK_SET);
        line_index = 0;
        while (read_line < p -> line) {
            do {
                ch = fgetc(fp);
            } while (ch != '\n');
            read_line++;
        }

        fpos_t file_pos = 0;
        fgetpos(fp, &file_pos);
        fpos_t pos_diff = p -> lastToken -> fpos - file_pos - 1;    //ch == '\n'

        while (line_index < 1023) {
            ch = fgetc(fp);
            line_buffer[line_index] = ch;
            line_index++;
            if (ch == '\n')
                break;
        }
        line_buffer[line_index] = '\0';

        if (line_buffer[line_index - 1] == '\n') {
            printf("%s", line_buffer);
        }
        else {
            printf("%s\n", line_buffer);
        }
        line_index = 0;

        while (pos_diff > 0) {
            if (line_buffer[line_index] == '\t') {
                printf("\t");
            }
            else {
                printf(" ");
            }
            line_index++;
            pos_diff--;
        }

        printf("\033[32m^\033[0m\n");

    }
    return;
}