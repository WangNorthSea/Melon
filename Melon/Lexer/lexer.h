//
//  lexer.h
//  Melon
//
//  Created by 王浩宇 on 2019/3/16.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#ifndef lexer_h
#define lexer_h

struct Token {
    int kind;
    int beginLine;
    int endLine;
    char * image;
    struct Token * next;
    struct Token * specialToken;
};

typedef struct Token Token;

extern Token * lexicalAnalyze(FILE * fp);

#endif /* lexer_h */
