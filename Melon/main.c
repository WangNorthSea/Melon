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
    FILE * fp = fopen("/Users/haoyuwang/Desktop/Melon/Melon/test.c", "r");
    Token * headToken = lexicalAnalyze(fp);
    while (headToken -> next != NULL) {
        printf("line: %d  value: %s  kind: %d\n", headToken -> next -> beginLine, headToken -> next -> image, headToken -> next -> kind);
        headToken = headToken -> next;
    }
    return 0;
}
