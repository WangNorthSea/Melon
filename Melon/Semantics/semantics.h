//
//  semantics.h
//  Melon
//
//  Created by 王浩宇 on 2019/3/28.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#ifndef semantics_h
#define semantics_h

extern void semanticAnalyze(ASTNode * root, const char * file);
extern void throwSemanticError(const char * file, int line, char * content);

extern const char * fileChecking;
extern Scope * scope;

#endif /* semantics_h */
