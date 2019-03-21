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
#include "../ASTNode/node.h"
#include "../ASTNode/constructor.h"

ASTNode * importStmts(void);

ASTNode * topDefs(void);

ASTNode * importStmt(void);

ASTNode * name(void);

ASTNode * defvars(void);

ASTNode * defun(void);

ASTNode * storage(void);

ASTNode * type(void);

ASTNode * expr(void);

ASTNode * typeref(void);

ASTNode * typeref_base(void);

ASTNode * params(void);

ASTNode * block(void);

ASTNode * fixedParams(void);

ASTNode * param(void);

ASTNode * defconst(void);

ASTNode * defstruct(void);

ASTNode * defunion(void);

ASTNode * typedef_(void);

ASTNode * stmts(void);

ASTNode * stmt(void);

ASTNode * member_list(void);

ASTNode * slot(void);

ASTNode * varname(void);

ASTNode * array(void);

ASTNode * labeled_stmt(void);

ASTNode * if_stmt(void);

ASTNode * while_stmt(void);

ASTNode * dowhile_stmt(void);

ASTNode * for_stmt(void);

ASTNode * switch_stmt(void);

ASTNode * break_stmt(void);

ASTNode * continue_stmt(void);

ASTNode * goto_stmt(void);

ASTNode * return_stmt(void);

ASTNode * case_clauses(void);

ASTNode * case_clause(void);

ASTNode * default_clause(void);

ASTNode * cases(void);

ASTNode * case_body(void);

ASTNode * primary(void);

ASTNode * term(void);

ASTNode * opassign_op(void);

ASTNode * expr10(void);

ASTNode * expr9(void);

ASTNode * expr8(void);

ASTNode * expr7(void);

ASTNode * expr6(void);

ASTNode * expr5(void);

ASTNode * expr4(void);

ASTNode * expr3(void);

ASTNode * expr2(void);

ASTNode * expr1(void);

ASTNode * unary(void);

ASTNode * postfix(void);

ASTNode * args(void);

int isType(void);

int match(int kind);

Token * token = NULL;

const char * parsingFile = NULL;

void compilationUnit(Token * headToken) {
    token = headToken -> next;
    
    importStmts();
    
    topDefs();
    
    match(EOF_);
}

ASTNode * importStmts(void) {
    ASTNode * returnPtr;
    do {                             //(importStmt())*
        returnPtr = importStmt();
    } while (returnPtr != NULL);
    
    return NULL;
}

ASTNode * topDefs(void) {
    ASTNode * returnPtr = NULL;
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

ASTNode * importStmt(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * name(void) {
    ASTNode * returnPtr = NULL;
    
    if (match(IDENTIFIER))
        return returnPtr;
    else
        return NULL;
}

ASTNode * storage(void) {
    ASTNode * returnPtr = NULL;
    
    if (match(STATIC))
        return returnPtr;
    else
        return NULL;
}

ASTNode * defvars(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * defun(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * type(void) {
    ASTNode * returnPtr = NULL;
    
    typeref();
    
    return returnPtr;
}

ASTNode * expr(void) {
    ASTNode * returnPtr = NULL;
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

ASTNode * typeref(void) {
    ASTNode * returnPtr = NULL;
    
    typeref_base();
    
    return returnPtr;
}

ASTNode * typeref_base(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * params(void) {
    ASTNode * returnPtr = NULL;
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

ASTNode * block(void) {
    ASTNode * returnPtr = NULL;
    
    if (!match(LEFTBRACE))
        return NULL;
    
    stmts();
    
    if (!match(RIGHTBRACE))
        return NULL;
    
    return returnPtr;
}

ASTNode * fixedParams(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * param(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * defconst(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * defstruct(void) {
    ASTNode * returnPtr = NULL;
    
    if (!match(STRUCT))
        return NULL;
    
    name();
    
    member_list();
    
    if (!match(SEMICOLON))
        return NULL;
    
    return returnPtr;
}

ASTNode * defunion(void) {
    ASTNode * returnPtr = NULL;
    
    if (!match(UNION))
        return NULL;
    
    name();
    
    member_list();
    
    if (!match(SEMICOLON))
        return NULL;
    
    return returnPtr;
}

ASTNode * typedef_(void) {
    ASTNode * returnPtr = NULL;
    
    if (!match(TYPEDEF))
        return NULL;
    
    typeref();
    
    varname();
    
    if (!match(SEMICOLON))
        return NULL;
    
    return returnPtr;
}

ASTNode * stmts(void) {
    ASTNode * returnPtr = NULL;
    
    do {                         //(stmt())*
        returnPtr = stmt();
    } while (returnPtr != NULL);
    
    return returnPtr;
}

ASTNode * stmt(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * member_list(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * slot(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * varname(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * array(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * labeled_stmt(void) {
    ASTNode * returnPtr = NULL;
    
    if (!match(IDENTIFIER))
        return NULL;
    
    if (!match(COLON))
        return NULL;
    
    stmt();                   //label其实记录的是这条语句的地址
    
    
    return returnPtr;
}

ASTNode * if_stmt(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * while_stmt(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * dowhile_stmt(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * for_stmt(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * switch_stmt(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * break_stmt(void) {
    ASTNode * returnPtr = NULL;
    
    if (!match(BREAK))
        return NULL;
    
    if (!match(SEMICOLON))
        return NULL;
    
    return returnPtr;
}

ASTNode * continue_stmt(void) {
    ASTNode * returnPtr = NULL;
    
    if (!match(CONTINUE))
        return NULL;
    
    if (!match(SEMICOLON))
        return NULL;
    
    return returnPtr;
}

ASTNode * goto_stmt(void) {
    ASTNode * returnPtr = NULL;
    
    if (!match(GOTO))
        return NULL;
    
    if (!match(IDENTIFIER))
        return NULL;
    
    if (!match(SEMICOLON))
        return NULL;
    
    return returnPtr;
}

ASTNode * return_stmt(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * case_clauses(void) {
    ASTNode * returnPtr = NULL;
    
    do {                              //(case_clause())*
        returnPtr = case_clause();
    } while (returnPtr != NULL);
    
    default_clause();         //必需
    
    return returnPtr;
}

ASTNode * case_clause(void) {
    ASTNode * returnPtr = NULL;
    
    cases();
    
    case_body();
    
    return returnPtr;
}

ASTNode * default_clause(void) {
    ASTNode * returnPtr = NULL;
    
    if (!match(DEFAULT))
        return NULL;
    
    if (!match(COLON))
        return NULL;
    
    case_body();
    
    return returnPtr;
}

ASTNode * cases(void) {
    ASTNode * returnPtr = NULL;
    
    if (!match(CASE))
        return NULL;
    
    primary();
    
    if (!match(COLON))
        return NULL;
    
    return returnPtr;
}

ASTNode * case_body(void) {
    ASTNode * returnPtr = NULL;
    
    do {                         //(stmt())+
        returnPtr = stmt();
    } while (returnPtr != NULL);
    
    return returnPtr;
}

ASTNode * primary(void) {
    ASTNode * returnPtr = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    ASTNode * Node;
    
    switch (token -> kind) {
        case INTEGER:
        case CHARACTER:
        case STRING:
        case IDENTIFIER:
            Node = NodeConstructor(token -> kind, parsingFile, token -> beginLine, token -> image, ptrs);
            token = token -> next;
            return Node;
        case LEFTPARENTHESE:
            token = token -> next;
            Node = expr();
            if (!match(RIGHTPARENTHESE)) {
                printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                exit(-1);
            }
            return Node;
        default:
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
    }
    
    return returnPtr;
}

ASTNode * term(void) {
    ASTNode * returnPtr = NULL;
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

ASTNode * opassign_op(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * expr10(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * expr9(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * expr8(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * expr7(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * expr6(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * expr5(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * expr4(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * expr3(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * expr2(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * expr1(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * unary(void) {
    ASTNode * returnPtr = NULL;
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

ASTNode * postfix(void) {
    ASTNode * returnPtr = NULL;
    
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

ASTNode * args(void) {
    ASTNode * returnPtr = NULL;
    
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
