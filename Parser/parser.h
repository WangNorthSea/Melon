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

extern const char * parsingFile;
extern error_t * err_list;

#endif /* parser_h */
