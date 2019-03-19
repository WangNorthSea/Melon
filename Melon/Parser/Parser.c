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

void * importStmts(Token * currentToken);

void * topDefs(Token * currentToken);

void * importStmt(Token * currentToken);

void * name(Token * currentToken);

void * defvars(Token * currentToken);

void * defun(Token * currentToken);

void * storage(Token * currentToken);

void * type(Token * currentToken);

void * expr(Token * currentToken);

void * typeref(Token * currentToken);

void * params(Token * currentToken);

void * block(Token * currentToken);

void * fixedParams(Token * currentToken);

void * param(Token * currentToken);

void * defconst(Token * currentToken);

void * defstruct(Token * currentToken);

void * defunion(Token * currentToken);

void * typedef_(Token * currentToken);

int match(Token * token, int kind);

Token * token = NULL;

void compilationUnit(Token * headToken) {
    token = headToken -> next;
    
    importStmts(token);
    
    topDefs(token);
    
    match(token, EOF_);
}

void * importStmts(Token * currentToken) {
    void * returnPtr;
    do {                             //(importStmt())*
        returnPtr = importStmt(currentToken);
    } while (returnPtr != NULL);
    
    return NULL;
}

void * topDefs(Token * currentToken) {
    void * returnPtr = NULL;
    Token * lookahead1 = currentToken;
    Token * lookahead2 = currentToken;
    lookahead2 -> next -> next -> next = NULL;     //有隐患
    
    while (1) {
        if (storage(lookahead1) != NULL && typeref(lookahead1) != NULL && match(lookahead1, IDENTIFIER) && match(lookahead1, LEFTPARENTHESE)) {
            returnPtr = defun(currentToken);
        }
        else if (defvars(lookahead2) != NULL) {
            returnPtr = defvars(currentToken);
        }
        else {
            switch (currentToken -> kind) {
                case CONST:
                    returnPtr = defconst(currentToken);
                    break;
                case STRUCT:
                    returnPtr = defstruct(currentToken);
                    break;
                case UNION:
                    returnPtr = defunion(currentToken);
                    break;
                case TYPEDEF:
                    returnPtr = typedef_(currentToken);
                    break;
                default:
                    goto jumpout;
            }
        }
        
        lookahead1 = currentToken;
        lookahead2 = currentToken;
        lookahead2 -> next -> next -> next = NULL;
    }
jumpout:
    
    
    return returnPtr;
}

void * importStmt(Token * currentToken) {
    void * returnPtr = NULL;
    
    if (!match(currentToken, IMPORT))
        return NULL;
    
    name(currentToken);
    
    do {                        //("." name())*
        if (match(currentToken, DOT))
            returnPtr = name(currentToken);
        else
            break;
        
    } while (returnPtr != NULL);
    
    if (!match(currentToken, SEMICOLON))
        return NULL;
    
    return returnPtr;
}

void * name(Token * currentToken) {
    void * returnPtr = NULL;
    
    if (match(currentToken, IDENTIFIER))
        return returnPtr;
    else
        return NULL;
}

void * storage(Token * currentToken) {
    void * returnPtr = NULL;
    
    if (match(currentToken, STATIC))
        return returnPtr;
    else
        return NULL;
}

void * defvars(Token * currentToken) {
    void * returnPtr = NULL;
    
    storage(currentToken);
    
    type(currentToken);
    
    name(currentToken);
    
    if (match(currentToken, ASSIGN))   //["=" expr()]
        expr(currentToken);
    
    do {                   //("," name() ["=" expr()])*
        if (match(currentToken, COMMA)) {
            name(currentToken);
            
            if (match(currentToken, ASSIGN)) {
                expr(currentToken);
            }
        }
        else
            returnPtr = NULL;
    } while (returnPtr != NULL);
    
    if (!match(currentToken, SEMICOLON))
        return NULL;
    
    return returnPtr;
}

void * defun(Token * currentToken) {
    void * returnPtr = NULL;
    
    storage(currentToken);
    
    typeref(currentToken);
    
    name(currentToken);
    
    if (!match(currentToken, LEFTPARENTHESE))
        return NULL;
    
    params(currentToken);
    
    if (!match(currentToken, RIGHTPARENTHESE))
        return NULL;
    
    block(currentToken);
    
    
    return returnPtr;
}

void * type(Token * currentToken) {
    void * returnPtr = NULL;
    
    
    return returnPtr;
}

void * expr(Token * currentToken) {
    void * returnPtr = NULL;
    
    
    return returnPtr;
}

void * typeref(Token * currentToken) {
    void * returnPtr = NULL;
    
    
    return returnPtr;
}

void * params(Token * currentToken) {
    void * returnPtr = NULL;
    Token * lookahead = currentToken;
    
    if (match(lookahead, VOID) && match(lookahead, RIGHTPARENTHESE)) {         //LOOKAHEAD(<VOID> ")")
        if (!match(currentToken, VOID))
            return NULL;
    }
    else {
        fixedParams(currentToken);
        
        if (match(currentToken, COMMA)) {  //["," "..."]
            if (match(currentToken, DOT) && match(currentToken, DOT) && match(currentToken, DOT)) {
                
            }
        }
    }
    
    return returnPtr;
}

void * block(Token * currentToken) {
    void * returnPtr = NULL;
    
    
    
    
    
    
    return returnPtr;
}

void * fixedParams(Token * currentToken) {
    void * returnPtr = NULL;
    Token * lookahead = currentToken;
    
    param(currentToken);
    
    do {                                     //(LOOKAHEAD(2) "," param())* 区分, ...
        if (match(lookahead, COMMA) && !match(lookahead, DOT)) {
            if (match(currentToken, COMMA))
                ;
            
            param(currentToken);
        }
        else
            returnPtr = NULL;
    } while (returnPtr != NULL);
    
    return returnPtr;
}

void * param(Token * currentToken) {
    void * returnPtr = NULL;
    
    type(currentToken);
    
    name(currentToken);
    
    return returnPtr;
}

void * defconst(Token * currentToken) {
    void * returnPtr = NULL;
    
    
    
    
    
    return returnPtr;
}

void * defstruct(Token * currentToken) {
    void * returnPtr = NULL;
    
    
    
    
    
    return returnPtr;
}

void * defunion(Token * currentToken) {
    void * returnPtr = NULL;
    
    
    
    
    
    return returnPtr;
}

void * typedef_(Token * currentToken) {
    void * returnPtr = NULL;
    
    
    
    
    
    return returnPtr;
}

int match(Token * token, int kind) {
    if (token -> kind == kind) {
        token = token -> next;
        return 1;
    }
    else
        return 0;
}
