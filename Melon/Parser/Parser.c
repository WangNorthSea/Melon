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

void * stmt(void);

void * member_list(void);

void * slot(void);

void * varname(void);

void * array(void);

void * labeled_stmt(void);

void * if_stmt(void);

void * while_stmt(void);

void * dowhile_stmt(void);

void * for_stmt(void);

void * switch_stmt(void);

void * break_stmt(void);

void * continue_stmt(void);

void * goto_stmt(void);

void * return_stmt(void);

void * case_clauses(void);

void * case_clause(void);

void * default_clause(void);

void * cases(void);

void * case_body(void);

void * primary(void);

void * term(void);

void * opassign_op(void);

void * expr10(void);

void * expr9(void);

void * expr8(void);

void * expr7(void);

void * expr6(void);

void * expr5(void);

void * expr4(void);

void * expr3(void);

void * expr2(void);

void * expr1(void);

void * unary(void);

void * postfix(void);

void * args(void);

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
    Token * lookahead = token;
    
    if (term() != NULL && match(ASSIGN)) {
        token = lookahead;
        term();
        
        if (!match(ASSIGN))
            return NULL;
        
        expr();
    }
    else {
        token = lookahead;
        
        if (term() != NULL && opassign_op() != NULL) {
            token = lookahead;
            term();
            
            opassign_op();
            
            expr();
        }
        else {
            token = lookahead;
            expr10();
        }
    }
    
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
    
    if (!match(CONST))
        return NULL;
    
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
    
    do {                         //(stmt())*
        returnPtr = stmt();
    } while (returnPtr != NULL);
    
    return returnPtr;
}

void * stmt(void) {
    void * returnPtr = NULL;
    
    if (match(SEMICOLON)) {
        
    }
    else if (token -> kind == IDENTIFIER && token -> next -> kind == COLON) {
        labeled_stmt();
    }
    else if (token -> kind == LEFTPARENTHESE || token -> kind == SELFSUM || token -> kind == SELFSUB || token -> kind == SUM || token -> kind == SUB || token -> kind == LOGICNOT || token -> kind == NOT || token -> kind == MUL || token -> kind == AND || token -> kind == SIZEOF || token -> kind == INTEGER || token -> kind == CHARACTER || token -> kind == STRING || token -> kind == IDENTIFIER) {
        expr();
        
        if (!match(SEMICOLON))
            return NULL;
    }
    else if (token -> kind == LEFTBRACE) {
        block();
    }
    else if (token -> kind == IF) {
        if_stmt();
    }
    else if (token -> kind == WHILE) {
        while_stmt();
    }
    else if (token -> kind == DO) {
        dowhile_stmt();
    }
    else if (token -> kind == FOR) {
        for_stmt();
    }
    else if (token -> kind == SWITCH) {
        switch_stmt();
    }
    else if (token -> kind == BREAK) {
        break_stmt();
    }
    else if (token -> kind == CONTINUE) {
        continue_stmt();
    }
    else if (token -> kind == GOTO) {
        goto_stmt();
    }
    else if (token -> kind == RETURN) {
        return_stmt();
    }
    else if (token -> kind == CONST) {
        defconst();
    }
    else {
        defvars();      //匹配不成功即为语法错误
    }
    
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
    
    do {
        if (token -> kind == LEFTBRACKET && token -> next -> kind == RIGHTBRACKET) {
            match(LEFTBRACKET);
            match(RIGHTBRACKET);
        }
        else {
            if (match(LEFTBRACKET)) {
                if (match(INTEGER)) {
                    if (match(RIGHTBRACKET)) {
                        
                    }
                    else {
                        returnPtr = NULL;
                    }
                }
                else {
                    returnPtr = NULL;
                }
            }
            else {
                returnPtr = NULL;
            }
        }
    } while (returnPtr != NULL);
    
    return returnPtr;
}

void * labeled_stmt(void) {
    void * returnPtr = NULL;
    
    if (!match(IDENTIFIER))
        return NULL;
    
    if (!match(COLON))
        return NULL;
    
    stmt();                   //label其实记录的是这条语句的地址
    
    
    return returnPtr;
}

void * if_stmt(void) {
    void * returnPtr = NULL;
    
    if (!match(IF))
        return NULL;
    
    if (!match(LEFTPARENTHESE))
        return NULL;
    
    expr();
    
    if (!match(RIGHTPARENTHESE))
        return NULL;
    
    stmt();
    
    if (match(ELSE)) {     //[LOOKAHEAD(1) <ELSE> stmt()]
        stmt();
    }
    
    return returnPtr;
}

void * while_stmt(void) {
    void * returnPtr = NULL;
    
    if (!match(WHILE))
        return NULL;
    
    if (!match(LEFTPARENTHESE))
        return NULL;
    
    expr();
    
    if (!match(RIGHTPARENTHESE))
        return NULL;
    
    stmt();
    
    return returnPtr;
}

void * dowhile_stmt(void) {
    void * returnPtr = NULL;
    
    if (!match(DO))
        return NULL;
    
    stmt();
    
    if (!match(WHILE))
        return NULL;
    
    if (!match(LEFTPARENTHESE))
        return NULL;
    
    expr();
    
    if (!match(RIGHTPARENTHESE))
        return NULL;
    
    if (!match(SEMICOLON))
        return NULL;
    
    return returnPtr;
}

void * for_stmt(void) {
    void * returnPtr = NULL;
    
    if (!match(FOR))
        return NULL;
    
    if (!match(LEFTPARENTHESE))
        return NULL;
    
    expr();      //可选
    
    if (!match(SEMICOLON))
        return NULL;
    
    expr();      //可选
    
    if (!match(SEMICOLON))
        return NULL;
    
    expr();      //可选
    
    if (!match(RIGHTPARENTHESE))
        return NULL;
    
    stmt();
    
    return returnPtr;
}

void * switch_stmt(void) {
    void * returnPtr = NULL;
    
    if (!match(SWITCH))
        return NULL;
    
    if (!match(LEFTPARENTHESE))
        return NULL;
    
    expr();
    
    if (!match(RIGHTPARENTHESE))
        return NULL;
    
    if (!match(LEFTBRACE))
        return NULL;
    
    case_clauses();
    
    
    if (!match(RIGHTBRACE))
        return NULL;
    
    return returnPtr;
}

void * break_stmt(void) {
    void * returnPtr = NULL;
    
    if (!match(BREAK))
        return NULL;
    
    if (!match(SEMICOLON))
        return NULL;
    
    return returnPtr;
}

void * continue_stmt(void) {
    void * returnPtr = NULL;
    
    if (!match(CONTINUE))
        return NULL;
    
    if (!match(SEMICOLON))
        return NULL;
    
    return returnPtr;
}

void * goto_stmt(void) {
    void * returnPtr = NULL;
    
    if (!match(GOTO))
        return NULL;
    
    if (!match(IDENTIFIER))
        return NULL;
    
    if (!match(SEMICOLON))
        return NULL;
    
    return returnPtr;
}

void * return_stmt(void) {
    void * returnPtr = NULL;
    
    if (!match(RETURN))
        return NULL;
    
    if (token -> kind == SEMICOLON) {
        match(SEMICOLON);
        
    }
    else {
        expr();
        
        if (!match(SEMICOLON))
            return NULL;
    }
    
    return returnPtr;
}

void * case_clauses(void) {
    void * returnPtr = NULL;
    
    do {                              //(case_clause())*
        returnPtr = case_clause();
    } while (returnPtr != NULL);
    
    default_clause();         //必需
    
    return returnPtr;
}

void * case_clause(void) {
    void * returnPtr = NULL;
    
    cases();
    
    case_body();
    
    return returnPtr;
}

void * default_clause(void) {
    void * returnPtr = NULL;
    
    if (!match(DEFAULT))
        return NULL;
    
    if (!match(COLON))
        return NULL;
    
    case_body();
    
    return returnPtr;
}

void * cases(void) {
    void * returnPtr = NULL;
    
    if (!match(CASE))
        return NULL;
    
    primary();
    
    if (!match(COLON))
        return NULL;
    
    return returnPtr;
}

void * case_body(void) {
    void * returnPtr = NULL;
    
    do {                         //(stmt())+
        returnPtr = stmt();
    } while (returnPtr != NULL);
    
    return returnPtr;
}

void * primary(void) {
    void * returnPtr = NULL;
    
    switch (token -> kind) {
        case INTEGER:
            token = token -> next;
            break;
        case CHARACTER:
            token = token -> next;
            break;
        case STRING:
            token = token -> next;
            break;
        case IDENTIFIER:
            token = token -> next;
            break;
        case LEFTPARENTHESE:
            token = token -> next;
            expr();
            if (!match(RIGHTPARENTHESE))
                return NULL;
            break;
        default:
            //语法错误
            break;
    }
    
    return returnPtr;
}

void * term(void) {
    void * returnPtr = NULL;
    Token * lookahead = token;
    
    if (match(LEFTPARENTHESE) && type()) {
        token = lookahead;
        
        if (!match(LEFTPARENTHESE))
            return NULL;
        
        type();
        
        if (!match(RIGHTPARENTHESE))
            return NULL;
        
        term();
    }
    else {
        token = lookahead;
        
        unary();
    }
    
    return returnPtr;
}

void * opassign_op(void) {
    void * returnPtr = NULL;
    
    switch (token -> kind) {
        case SUMASSIGN:
            token = token -> next;
            break;
        case SUBASSIGN:
            token = token -> next;
            break;
        case MULASSIGN:
            token = token -> next;
            break;
        case DIVASSIGN:
            token = token -> next;
            break;
        case RESASSIGN:
            token = token -> next;
            break;
        case ANDASSIGN:
            token = token -> next;
            break;
        case ORASSIGN:
            token = token -> next;
            break;
        case XORASSIGN:
            token = token -> next;
            break;
        case LSHASSIGN:
            token = token -> next;
            break;
        case RSHASSIGN:
            token = token -> next;
            break;
        default:
            //语法错误
            break;
    }
    
    return returnPtr;
}

void * expr10(void) {
    void * returnPtr = NULL;
    
    expr9();
    
    //可选           ["?" expr() ":" expr10()]
    if (match(QUESTION)) {
        expr();
        
        if (!match(COLON))
            return NULL;
        
        expr10();
    }
    
    return returnPtr;
}

void * expr9(void) {
    void * returnPtr = NULL;
    
    expr8();
    
    //("||" expr8())*
    do {
        if (match(LOGICOR)) {
            expr8();
        }
        else
            returnPtr = NULL;
    } while (returnPtr != NULL);
    
    return returnPtr;
}

void * expr8(void) {
    void * returnPtr = NULL;
    
    expr7();
    
    //("&&" expr7())*
    do {
        if (match(LOGICAND)) {
            expr7();
        }
        else
            returnPtr = NULL;
    } while (returnPtr != NULL);
    
    return returnPtr;
}

void * expr7(void) {
    void * returnPtr = NULL;
    
    expr6();
    
    do {  //(">" expr6() | "<" expr6() | ">=" expr6() | "<=" expr6() | "==" expr6() | "!=" expr6())*
        switch (token -> kind) {
            case GREATERTHAN:
                token = token -> next;
                expr6();
                break;
            case LESSTHAN:
                token = token -> next;
                expr6();
                break;
            case GREATERANDEQUAL:
                token = token -> next;
                expr6();
                break;
            case LESSANDEQUAL:
                token = token -> next;
                expr6();
                break;
            case EQUAL:
                token = token -> next;
                expr6();
                break;
            case NOTEQUAL:
                token = token -> next;
                expr6();
                break;
            default:
                returnPtr = NULL;
                break;
        }
    } while (returnPtr != NULL);
    
    return returnPtr;
}

void * expr6(void) {
    void * returnPtr = NULL;
    
    expr5();
    
    do {        //("|" expr5())*
        if (match(OR)) {
            expr5();
        }
        else {
            returnPtr = NULL;
        }
    } while (returnPtr != NULL);
    
    return returnPtr;
}

void * expr5(void) {
    void * returnPtr = NULL;
    
    expr4();
    
    do {        //("^" expr4())*
        if (match(XOR)) {
            expr4();
        }
        else {
            returnPtr = NULL;
        }
    } while (returnPtr != NULL);
    
    return returnPtr;
}

void * expr4(void) {
    void * returnPtr = NULL;
    
    expr3();
    
    do {        //("&" expr3())*
        if (match(AND)) {
            expr3();
        }
        else {
            returnPtr = NULL;
        }
    } while (returnPtr != NULL);
    
    return returnPtr;
}

void * expr3(void) {
    void * returnPtr = NULL;
    
    expr2();
    
    do {       //(">>" expr2() | "<<" expr2())*
        if (match(RSH)) {
            expr2();
        }
        else if (match(LSH)) {
            expr2();
        }
        else {
            returnPtr = NULL;
        }
    } while (returnPtr != NULL);
    
    return returnPtr;
}

void * expr2(void) {
    void * returnPtr = NULL;
    
    expr1();
    
    do {       //("+" expr1() | "-" expr1())*
        if (match(SUM)) {
            expr1();
        }
        else if (match(SUB)) {
            expr1();
        }
        else {
            returnPtr = NULL;
        }
    } while (returnPtr != NULL);
    
    return returnPtr;
}

void * expr1(void) {
    void * returnPtr = NULL;
    
    term();
    
    do {       //("*" term() | "/" term() | "%" term())*
        if (match(MUL)) {
            term();
        }
        else if (match(DIV)) {
            term();
        }
        else if (match(RES)){
            term();
        }
        else {
            returnPtr = NULL;
        }
    } while (returnPtr != NULL);
    
    return returnPtr;
}

void * unary(void) {
    void * returnPtr = NULL;
    Token * lookahead1 = NULL;
    Token * lookahead2 = NULL;
    
    switch (token -> kind) {
        case SELFSUM:
            token = token -> next;
            unary();
            break;
        case SELFSUB:
            token = token -> next;
            unary();
            break;
        case SUM:
            token = token -> next;
            term();
            break;
        case SUB:
            token = token -> next;
            term();
            break;
        case LOGICNOT:
            token = token -> next;
            term();
            break;
        case NOT:
            token = token -> next;
            term();
            break;
        case MUL:
            token = token -> next;
            term();
            break;
        case AND:
            token = token -> next;
            term();
            break;
        case SIZEOF:
            token = token -> next;
            lookahead1 = token;
            lookahead2 = token -> next -> next;
            token -> next -> next = NULL;
            
            if (match(LEFTPARENTHESE) && type() != NULL) {
                token = lookahead1;
                token -> next -> next = lookahead2;
                
                if (!match(LEFTPARENTHESE))
                    return NULL;
                
                type();
                
                if (!match(RIGHTPARENTHESE))
                    return NULL;
            }
            else {
                token = lookahead1;
                token -> next -> next = lookahead2;
                
                unary();
            }
            break;
        default:
            postfix();
    }
    
    return returnPtr;
    
}

void * postfix(void) {
    void * returnPtr = NULL;
    
    primary();
    
    do {    //("++"|"--"|"["expr()"]"|"."name()|"->"name()|"("args()")")*
        switch (token -> kind) {
            case SELFSUM:
                token = token -> next;
                break;
            case SELFSUB:
                token = token -> next;
                break;
            case LEFTBRACKET:
                token = token -> next;
                expr();
                if (!match(RIGHTBRACKET))
                    return NULL;
                break;
            case DOT:
                token = token -> next;
                name();
                break;
            case ARROW:
                token = token -> next;
                name();
                break;
            case LEFTPARENTHESE:
                token = token -> next;
                args();
                if (!match(RIGHTPARENTHESE))
                    return NULL;
                break;
            default:
                returnPtr = NULL;
                break;
        }
    } while (returnPtr != NULL);
    
    return returnPtr;
}

void * args(void) {
    void * returnPtr = NULL;
    
    //可选  [expr() ("," expr())*]
    if (expr() != NULL) {
        do {
            if (match(COMMA)) {
                expr();
            }
            else {
                returnPtr = NULL;
            }
        } while (returnPtr != NULL);
    }
    
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
