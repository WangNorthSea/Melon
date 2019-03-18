//
//  main.c
//  Melon
//
//  Created by 王浩宇 on 2019/3/16.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "./Lexer/token.h"
#include "./Lexer/lexer.h"

int main(int argc, const char * argv[]) {
    if (argc == 1) {
        printf("Melon: no input file\n");
        return 0;
    }
    
    FILE * fp = fopen(argv[1], "r");
    
    if (fp == NULL) {
        printf("Melon: %s: no such file\n", argv[1]);
        exit(-1);
    }
    
    Token * headToken = lexicalAnalyze(fp);
    while (headToken -> next != NULL) {
        printf("line: %d  value: %s  kind: %d\n", headToken -> next -> beginLine, headToken -> next -> image, headToken -> next -> kind);
        headToken = headToken -> next;
    }
    return 0;
}
