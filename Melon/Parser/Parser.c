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

void * params(void);

void * block(void);

int match(int kind);

Token * currentToken = NULL;

void compilationUnit(Token * headToken) {
    currentToken = headToken -> next;
    
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
    
    
    return NULL;
}

void * importStmt(void) {
    void * returnPtr = NULL;
    
    if (match(IMPORT))
        currentToken = currentToken -> next;
    else
        return NULL;
    
    name();
    
    do {                        //("." name())*
        if (match(DOT)) {
            currentToken = currentToken -> next;
            returnPtr = name();
        }
        else
            break;
        
    } while (returnPtr != NULL);
    
    if (match(SEMICOLON))
        currentToken = currentToken -> next;
    else
        return NULL;
    
    return returnPtr;
}

void * name(void) {
    void * returnPtr = NULL;
    
    if (match(IDENTIFIER)) {
        currentToken = currentToken -> next;
        return returnPtr;
    }
    else
        return NULL;
}

void * storage(void) {
    void * returnPtr = NULL;
    
    if (match(STATIC)) {
        currentToken = currentToken -> next;
        return returnPtr;
    }
    else
        return NULL;
}

void * defvars(void) {
    void * returnPtr = NULL;
    
    storage();
    
    type();
    
    name();
    
    if (match(ASSIGN)) {   //["=" expr()]
        currentToken = currentToken -> next;
        expr();
    }
    
    do {                   //("," name() ["=" expr()])*
        if (match(COMMA)) {
            currentToken = currentToken -> next;
            name();
            
            if (match(ASSIGN)) {
                currentToken = currentToken -> next;
                expr();
            }
        }
        else
            returnPtr = NULL;
    } while (returnPtr != NULL);
    
    if (match(SEMICOLON))
        currentToken = currentToken -> next;
    else
        return NULL;
    
    return returnPtr;
}

void * defun(void) {
    void * returnPtr = NULL;
    
    storage();
    
    typeref();
    
    name();
    
    if (match(LEFTPARENTHESE))
        currentToken = currentToken -> next;
    else
        return NULL;
    
    params();
    
    if (match(RIGHTPARENTHESE))
        currentToken = currentToken -> next;
    else
        return NULL;
    
    block();
    
    
    return returnPtr;
}

void * type(void) {
    void * returnPtr = NULL;
    
    
    return returnPtr;
}

void * expr(void) {
    void * returnPtr = NULL;
    
    
    return returnPtr;
}

void * typeref(void) {
    void * returnPtr = NULL;
    
    
    return returnPtr;
}

void * params(void) {
    void * returnPtr = NULL;
    
    if (match(VOID)) {         //LOOKAHEAD(<VOID> ")")
        
    }
    else {
        
    }
    
    return returnPtr;
}

int match(int kind) {
    if (currentToken -> kind == kind)
        return 1;
    else
        return 0;
}
