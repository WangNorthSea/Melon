//
//  token.h
//  Melon
//
//  Created by 王浩宇 on 2019/3/17.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#ifndef token_h
#define token_h

struct Token {
    int kind;
    int beginLine;
    int endLine;
    char * image;
    struct Token * next;
    struct Token * specialToken;
};

typedef struct Token Token;

#endif /* token_h */
