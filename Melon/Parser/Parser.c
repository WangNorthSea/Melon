//
//  Parser.c
//  Melon
//
//  Created by 王浩宇 on 2019/3/18.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include "../Lexer/token.h"
#include "../Lexer/tokenkind.h"

void * importStmts(void);

void * topDefs(void);

void * importStmt(void);

void * name(void);

void * defvars(void);

void * defun(void);

void * storage(void);

void * type(void);

void * expr(void);

void * typeref(void);

void * typeref_base(void);

void * params(void);

void * block(void);

void * fixedParams(void);

void * param(void);

void * defconst(void);

void * defstruct(void);

void * defunion(void);

void * typedef_(void);

void * stmts(void);

void * member_list(void);

void * slot(void);

void * varname(void);

void * array(void);

int isType(void);

int match(int kind);

Token * token = NULL;

void compilationUnit(Token * headToken) {
    token = headToken -> next;
    
    importStmts();
    
    topDefs();
    
    match(EOF_);
}

void * importStmts(void) {
    void * returnPtr;
    do {                             //(importStmt())*
        returnPtr = importStmt();
    } while (returnPtr != NULL);
    
    return NULL;
}

void * topDefs(void) {
    void * returnPtr = NULL;
    Token * lookahead1 = token;
    
    while (1) {
        if (storage() != NULL && typeref() != NULL && match(IDENTIFIER) && match(LEFTPARENTHESE)) {
            token = lookahead1;
            returnPtr = defun();
        }
        else {
            token = lookahead1;
            Token * lookahead2 = token -> next -> next -> next;    //有隐患
            token -> next -> next -> next = NULL;
            
            if (defvars() != NULL) {
                token = lookahead1;
                token -> next -> next -> next = lookahead2;
                returnPtr = defvars();
            }
            else {
                token = lookahead1;
                token -> next -> next -> next = lookahead2;
                switch (token -> kind) {
                    case CONST:
                        returnPtr = defconst();
                        break;
                    case STRUCT:
                        returnPtr = defstruct();
                        break;
                    case UNION:
                        returnPtr = defunion();
                        break;
                    case TYPEDEF:
                        returnPtr = typedef_();
                        break;
                    default:
                        goto jumpout;
                }
            }
        }
        
        lookahead1 = token;
    }
jumpout:
    
    
    return returnPtr;
}

void * importStmt(void) {
    void * returnPtr = NULL;
    
    if (!match(IMPORT))
        return NULL;
    
    name();
    
    do {                        //("." name())*
        if (match(DOT))
            returnPtr = name();
        else
            break;
        
    } while (returnPtr != NULL);
    
    if (!match(SEMICOLON))
        return NULL;
    
    return returnPtr;
}

void * name(void) {
    void * returnPtr = NULL;
    
    if (match(IDENTIFIER))
        return returnPtr;
    else
        return NULL;
}

void * storage(void) {
    void * returnPtr = NULL;
    
    if (match(STATIC))
        return returnPtr;
    else
        return NULL;
}

void * defvars(void) {
    void * returnPtr = NULL;
    
    storage();
    
    type();
    
    if (match(LEFTPARENTHESE)) {        //函数指针
        if (!match(MUL))
            return NULL;
        
        name();
        
        if (!match(RIGHTPARENTHESE))
            return NULL;
        
        if (!match(LEFTPARENTHESE))
            return NULL;
        
        params();
        
        if (!match(RIGHTPARENTHESE))
            return NULL;
    }
    else {
        varname();
    }
    
    
    
    if (match(ASSIGN))   //["=" expr()]
        expr();
    
    do {                   //("," name() ["=" expr()])*
        if (match(COMMA)) {
            varname();
            
            if (match(ASSIGN)) {
                expr();
            }
        }
        else
            returnPtr = NULL;
    } while (returnPtr != NULL);
    
    if (!match(SEMICOLON))
        return NULL;
    
    return returnPtr;
}

void * defun(void) {
    void * returnPtr = NULL;
    
    storage();
    
    typeref();
    
    name();
    
    if (!match(LEFTPARENTHESE))
        return NULL;
    
    params();
    
    if (!match(RIGHTPARENTHESE))
        return NULL;
    
    block();
    
    
    return returnPtr;
}

void * type(void) {
    void * returnPtr = NULL;
    
    typeref();
    
    return returnPtr;
}

void * expr(void) {
    void * returnPtr = NULL;
    
    
    return returnPtr;
}

void * typeref(void) {
    void * returnPtr = NULL;
    
    typeref_base();
    
    return returnPtr;
}

void * typeref_base(void) {
    void * returnPtr = NULL;
    
    switch (token -> kind) {
        case VOID:
            token = token -> next;
            break;
        case CHAR:
            token = token -> next;
            break;
        case SHORT:
            token = token -> next;
            break;
        case INT:
            token = token -> next;
            break;
        case LONG:
            token = token -> next;
            break;
        case UNSIGNED:
            token = token -> next;
            switch (token -> kind) {
                case CHAR:
                    token = token -> next;
                    break;
                case SHORT:
                    token = token -> next;
                    break;
                case INT:
                    token = token -> next;
                    break;
                case LONG:
                    token = token -> next;
                    break;
                default:
                    //语法错误
                    break;
            }
            break;
        case STRUCT:
            token = token -> next;
            if (match(IDENTIFIER)) {
                
            }
            else {
                //语法错误
            }
            break;
        case UNION:
            token = token -> next;
            if (match(IDENTIFIER)) {
                
            }
            else {
                //语法错误
            }
        default:
            if (isType()) {
                token = token -> next;
                if (match(IDENTIFIER)) {
                    
                }
                else {
                    //语法错误
                }
            }
            else {
                //语法错误
            }
            break;
    }
    
    
    
    
    return returnPtr;
}

void * params(void) {
    void * returnPtr = NULL;
    Token * lookahead = token;
    
    if (match(VOID) && match(RIGHTPARENTHESE)) {         //LOOKAHEAD(<VOID> ")")
        token = lookahead;
        if (!match(VOID))
            return NULL;
    }
    else {
        token = lookahead;
        fixedParams();
        
        if (match(COMMA)) {  //["," "..."]
            if (match(DOT) && match(DOT) && match(DOT)) {
                
            }
        }
    }
    
    return returnPtr;
}

void * block(void) {
    void * returnPtr = NULL;
    
    if (!match(LEFTBRACE))
        return NULL;
    
    stmts();
    
    if (!match(RIGHTBRACE))
        return NULL;
    
    return returnPtr;
}

void * fixedParams(void) {
    void * returnPtr = NULL;
    
    param();
    
    Token * lookahead1 = token;
    Token * lookahead2 = token -> next -> next;     //有隐患
    token -> next -> next = NULL;
    
    do {                                     //(LOOKAHEAD(2) "," param())* 区分, ...
        if (match(COMMA) && !match(DOT)) {
            token = lookahead1;
            token -> next -> next = lookahead2;
            if (match(COMMA))
                ;
            
            param();
        }
        else {
            token = lookahead1;
            token -> next -> next = lookahead2;
            returnPtr = NULL;
            break;
        }
        
        lookahead1 = token;
        lookahead2 = token -> next -> next;     //有隐患
        token -> next -> next = NULL;
    } while (1);
    
    return returnPtr;
}

void * param(void) {
    void * returnPtr = NULL;
    
    type();
    
    if (match(LEFTPARENTHESE)) {        //函数指针
        if (!match(MUL))
            return NULL;
        
        name();
        
        if (!match(RIGHTPARENTHESE))
            return NULL;
        
        if (!match(LEFTPARENTHESE))
            return NULL;
        
        params();
        
        if (!match(RIGHTPARENTHESE))
            return NULL;
    }
    else {
        varname();
    }
    
    return returnPtr;
}

void * defconst(void) {
    void * returnPtr = NULL;
    
    
    
    
    
    return returnPtr;
}

void * defstruct(void) {
    void * returnPtr = NULL;
    
    if (!match(STRUCT))
        return NULL;
    
    name();
    
    member_list();
    
    if (!match(SEMICOLON))
        return NULL;
    
    return returnPtr;
}

void * defunion(void) {
    void * returnPtr = NULL;
    
    if (!match(UNION))
        return NULL;
    
    name();
    
    member_list();
    
    if (!match(SEMICOLON))
        return NULL;
    
    return returnPtr;
}

void * typedef_(void) {
    void * returnPtr = NULL;
    
    if (!match(TYPEDEF))
        return NULL;
    
    typeref();
    
    varname();
    
    if (!match(SEMICOLON))
        return NULL;
    
    return returnPtr;
}

void * stmts(void) {
    void * returnPtr = NULL;
    
    
    
    
    
    return returnPtr;
}

void * member_list(void) {
    void * returnPtr = NULL;
    
    if (!match(LEFTBRACE))
        return NULL;
    
    do {                          //(slot() ";")*
        if (slot() != NULL) {
            if (!match(SEMICOLON))
                return NULL;
        }
        else
            break;
    } while (1);
    
    if (!match(RIGHTBRACE))
        return NULL;
    
    return returnPtr;
}

void * slot(void) {
    void * returnPtr = NULL;
    
    type();
    
    if (match(LEFTPARENTHESE)) {        //函数指针
        if (!match(MUL))
            return NULL;
        
        name();
        
        if (!match(RIGHTPARENTHESE))
            return NULL;
        
        if (!match(LEFTPARENTHESE))
            return NULL;
        
        params();
        
        if (!match(RIGHTPARENTHESE))
            return NULL;
    }
    else {
        varname();
    }
    
    return returnPtr;
}

void * varname(void) {
    void * returnPtr = NULL;
    
    do {                    //("*")*
        if (!match(MUL))
            break;
    } while (1);
    
    name();
    
    do {
        returnPtr = array();               //零个或多个
    } while (returnPtr != NULL);
    
    return returnPtr;
}

void * array(void) {
    void * returnPtr = NULL;
    
    
    
    
    
    
    return returnPtr;
}

int isType(void) {
    
    
    
    return 0;
}

int match(int kind) {
    if (token -> kind == kind) {
        token = token -> next;
        return 1;
    }
    else
        return 0;
}
