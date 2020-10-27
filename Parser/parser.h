//
//  parser.h
//  Melon
//
//  Created by 王浩宇 on 2019/3/18.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#ifndef parser_h
#define parser_h

#include "../Report/error.h"

extern ASTNode * compilationUnit(Token * headToken);

extern char * parsingFile;

#endif /* parser_h */
