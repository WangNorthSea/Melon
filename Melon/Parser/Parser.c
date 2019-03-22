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

int prelooking = 0;

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
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    Token * lookahead = token;
    
    prelooking++;
    if (term() != NULL && match(ASSIGN)) {
        prelooking--;
        token = lookahead;
        ptrs[0] = term();
        
        if (ptrs[0] == NULL) {
            if (prelooking)
                return NULL;
            else {
                printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                exit(-1);
            }
        }
        
        if (!match(ASSIGN)) {
            if (prelooking)
                return NULL;
            else {
                printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                exit(-1);
            }
        }
        
        ptrs[1] = expr();
        
        if (ptrs[1] == NULL) {
            if (prelooking)
                return NULL;
            else {
                printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                exit(-1);
            }
        }
        
        return NodeConstructor(Assign, parsingFile, token -> beginLine, NULL, ptrs);
    }
    else {
        prelooking--;
        token = lookahead;
        
        prelooking++;
        if (term() != NULL && opassign_op() != NULL) {
            prelooking--;
            token = lookahead;
            ptrs[0] = term();
            
            if (ptrs[0] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            ptrs[1] = opassign_op();
            
            if (ptrs[1] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            ptrs[2] = expr();
            
            if (ptrs[2] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            return NodeConstructor(OpAssign, parsingFile, token -> beginLine, NULL, ptrs);
        }
        else {
            prelooking--;
            token = lookahead;
            Node = expr10();
            
            if (Node == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            return Node;
        }
    }
    
    return Node;
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
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (!match(IF)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    if (!match(LEFTPARENTHESE)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    ptrs[0] = expr();
    
    if (ptrs[0] == NULL) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    if (!match(RIGHTPARENTHESE)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    ptrs[1] = stmt();
    
    if (ptrs[1] == NULL) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    if (match(ELSE)) {     //[LOOKAHEAD(1) <ELSE> stmt()]
        ptrs[2] = stmt();
        
        if (ptrs[2] == NULL) {
            if (prelooking)
                return NULL;
            else {
                printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                exit(-1);
            }
        }
    }
    
    return NodeConstructor(If, parsingFile, token -> beginLine, NULL, ptrs);
}

ASTNode * while_stmt(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (!match(WHILE)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    if (!match(LEFTPARENTHESE)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    ptrs[0] = expr();
    
    if (ptrs[0] == NULL) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    if (!match(RIGHTPARENTHESE)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    ptrs[1] = stmt();
    
    if (ptrs[1] == NULL) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    return NodeConstructor(While, parsingFile, token -> beginLine, NULL, ptrs);
}

ASTNode * dowhile_stmt(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (!match(DO)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    ptrs[0] = stmt();
    
    if (ptrs[0] == NULL) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    if (!match(WHILE)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    if (!match(LEFTPARENTHESE)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    ptrs[1] = expr();
    
    if (ptrs[1] == NULL) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    if (!match(RIGHTPARENTHESE)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    if (!match(SEMICOLON)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    return NodeConstructor(DoWhile, parsingFile, token -> beginLine, NULL, ptrs);
}

ASTNode * for_stmt(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (!match(FOR)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    if (!match(LEFTPARENTHESE)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    ptrs[0] = expr();      //可选
    
    if (!match(SEMICOLON)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    ptrs[1] = expr();      //可选
    
    if (!match(SEMICOLON)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    ptrs[2] = expr();      //可选
    
    if (!match(RIGHTPARENTHESE)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    ptrs[3] = stmt();
    
    if (ptrs[3] == NULL) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    return NodeConstructor(For, parsingFile, token -> beginLine, NULL, ptrs);
}

ASTNode * switch_stmt(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (!match(SWITCH)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    if (!match(LEFTPARENTHESE)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    ptrs[0] = expr();
    
    if (ptrs[0] == NULL) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    if (!match(RIGHTPARENTHESE)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    if (!match(LEFTBRACE)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    ptrs[1] = case_clauses();
    
    if (ptrs[1] == NULL) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    if (!match(RIGHTBRACE)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    return NodeConstructor(Switch, parsingFile, token -> beginLine, NULL, ptrs);
}

ASTNode * break_stmt(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (!match(BREAK)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    if (!match(SEMICOLON)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    return NodeConstructor(Break, parsingFile, token -> beginLine, NULL, ptrs);
}

ASTNode * continue_stmt(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (!match(CONTINUE)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    if (!match(SEMICOLON)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    return NodeConstructor(Continue, parsingFile, token -> beginLine, NULL, ptrs);
}

ASTNode * goto_stmt(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    char * label = NULL;
    
    if (!match(GOTO)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    label = token -> image;
    if (!match(IDENTIFIER)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    if (!match(SEMICOLON)) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    return NodeConstructor(Goto, parsingFile, token -> beginLine, label, ptrs);
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
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    ASTNode * Node = NULL;
    
    switch (token -> kind) {
        case INTEGER:
            Node = NodeConstructor(IntegerLiteral, parsingFile, token -> beginLine, token -> image, ptrs);
            token = token -> next;
            break;
        case CHARACTER:
            Node = NodeConstructor(CharacterLiteral, parsingFile, token -> beginLine, token -> image, ptrs);
            token = token -> next;
            break;
        case STRING:
            Node = NodeConstructor(StringLiteral, parsingFile, token -> beginLine, token -> image, ptrs);
            token = token -> next;
            break;
        case IDENTIFIER:
            Node = NodeConstructor(Identifier, parsingFile, token -> beginLine, token -> image, ptrs);
            token = token -> next;
            break;
        case LEFTPARENTHESE:
            token = token -> next;
            Node = expr();
            
            if (Node == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            if (!match(RIGHTPARENTHESE)) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            break;
        default:
            if (prelooking)
                return NULL;
            else {
                printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                exit(-1);
            }
    }
    
    return Node;
}

ASTNode * term(void) {
    ASTNode * Node = NULL;
    Token * lookahead = token;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    prelooking++;
    if (match(LEFTPARENTHESE) && type() != NULL) {
        prelooking--;
        token = lookahead;
        
        if (!match(LEFTPARENTHESE)) {
            if (prelooking)
                return NULL;
            else {
                printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                exit(-1);
            }
        }
        
        ptrs[0] = type();
        
        if (ptrs[0] == NULL) {
            if (prelooking)
                return NULL;
            else {
                printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                exit(-1);
            }
        }
        
        if (!match(RIGHTPARENTHESE)) {
            if (prelooking)
                return NULL;
            else {
                printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                exit(-1);
            }
        }
        
        ptrs[1] = term();
        
        if (ptrs[1] == NULL) {
            if (prelooking)
                return NULL;
            else {
                printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                exit(-1);
            }
        }
        
        Node = NodeConstructor(Cast, parsingFile, token -> beginLine, NULL, ptrs);
    }
    else {
        prelooking--;
        token = lookahead;
        
        Node = unary();
        
        if (Node == NULL) {
            if (prelooking)
                return NULL;
            else {
                printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                exit(-1);
            }
        }
    }
    
    return Node;
}

ASTNode * opassign_op(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (token -> kind) {
        case SUMASSIGN:
            token = token -> next;
            Node = NodeConstructor(Operator, parsingFile, token -> beginLine, "+", ptrs);
            break;
        case SUBASSIGN:
            token = token -> next;
            Node = NodeConstructor(Operator, parsingFile, token -> beginLine, "-", ptrs);
            break;
        case MULASSIGN:
            token = token -> next;
            Node = NodeConstructor(Operator, parsingFile, token -> beginLine, "*", ptrs);
            break;
        case DIVASSIGN:
            token = token -> next;
            Node = NodeConstructor(Operator, parsingFile, token -> beginLine, "/", ptrs);
            break;
        case RESASSIGN:
            token = token -> next;
            Node = NodeConstructor(Operator, parsingFile, token -> beginLine, "%", ptrs);
            break;
        case ANDASSIGN:
            token = token -> next;
            Node = NodeConstructor(Operator, parsingFile, token -> beginLine, "&", ptrs);
            break;
        case ORASSIGN:
            token = token -> next;
            Node = NodeConstructor(Operator, parsingFile, token -> beginLine, "|", ptrs);
            break;
        case XORASSIGN:
            token = token -> next;
            Node = NodeConstructor(Operator, parsingFile, token -> beginLine, "^", ptrs);
            break;
        case LSHASSIGN:
            token = token -> next;
            Node = NodeConstructor(Operator, parsingFile, token -> beginLine, "<<", ptrs);
            break;
        case RSHASSIGN:
            token = token -> next;
            Node = NodeConstructor(Operator, parsingFile, token -> beginLine, ">>", ptrs);
            break;
        default:
            if (prelooking)
                return NULL;
            else {
                printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                exit(-1);
            }
            break;
    }
    
    return Node;
}

ASTNode * expr10(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    Node = expr9();
    
    if (Node == NULL) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }

    //可选           ["?" expr() ":" expr10()]
    if (match(QUESTION)) {
        ptrs[0] = Node;
        ptrs[1] = expr();
        
        if (ptrs[1] == NULL) {
            if (prelooking)
                return NULL;
            else {
                printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                exit(-1);
            }
        }
        
        if (!match(COLON)) {
            if (prelooking)
                return NULL;
            else {
                printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                exit(-1);
            }
        }
        
        ptrs[2] = expr10();
        
        if (ptrs[2] == NULL) {
            if (prelooking)
                return NULL;
            else {
                printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                exit(-1);
            }
        }
        
        return NodeConstructor(CondExpr, parsingFile, token -> beginLine, NULL, ptrs);
    }
    
    return Node;
}

ASTNode * expr9(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    Node = expr8();
    
    if (Node == NULL) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    //("||" expr8())*
    do {
        if (match(LOGICOR)) {
            ptrs[0] = Node;
            ptrs[1] = expr8();
            
            if (ptrs[1] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            Node = NodeConstructor(LogicOr, parsingFile, token -> beginLine, NULL, ptrs);
        }
        else
            goto jumpout;
    } while (1);
jumpout:
    
    return Node;
}

ASTNode * expr8(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    Node = expr7();
    
    if (Node == NULL) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    //("&&" expr7())*
    do {
        if (match(LOGICAND)) {
            ptrs[0] = Node;
            ptrs[1] = expr7();
            
            if (ptrs[1] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            Node = NodeConstructor(LogicAnd, parsingFile, token -> beginLine, NULL, ptrs);
        }
        else
            goto jumpout;
    } while (1);
jumpout:
    
    return Node;
}

ASTNode * expr7(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    Node = expr6();
    
    if (Node == NULL) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    do {  //(">" expr6() | "<" expr6() | ">=" expr6() | "<=" expr6() | "==" expr6() | "!=" expr6())*
        switch (token -> kind) {
            case GREATERTHAN:
                token = token -> next;
                ptrs[0] = Node;
                ptrs[1] = expr6();
                
                if (ptrs[1] == NULL) {
                    if (prelooking)
                        return NULL;
                    else {
                        printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                        exit(-1);
                    }
                }
                
                Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, ">", ptrs);
                break;
            case LESSTHAN:
                token = token -> next;
                ptrs[0] = Node;
                ptrs[1] = expr6();
                
                if (ptrs[1] == NULL) {
                    if (prelooking)
                        return NULL;
                    else {
                        printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                        exit(-1);
                    }
                }
                
                Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, "<", ptrs);
                break;
            case GREATERANDEQUAL:
                token = token -> next;
                ptrs[0] = Node;
                ptrs[1] = expr6();
                
                if (ptrs[1] == NULL) {
                    if (prelooking)
                        return NULL;
                    else {
                        printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                        exit(-1);
                    }
                }
                
                Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, ">=", ptrs);
                break;
            case LESSANDEQUAL:
                token = token -> next;
                ptrs[0] = Node;
                ptrs[1] = expr6();
                
                if (ptrs[1] == NULL) {
                    if (prelooking)
                        return NULL;
                    else {
                        printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                        exit(-1);
                    }
                }
                
                Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, "<=", ptrs);
                break;
            case EQUAL:
                token = token -> next;
                ptrs[0] = Node;
                ptrs[1] = expr6();
                
                if (ptrs[1] == NULL) {
                    if (prelooking)
                        return NULL;
                    else {
                        printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                        exit(-1);
                    }
                }
                
                Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, "==", ptrs);
                break;
            case NOTEQUAL:
                token = token -> next;
                ptrs[0] = Node;
                ptrs[1] = expr6();
                
                if (ptrs[1] == NULL) {
                    if (prelooking)
                        return NULL;
                    else {
                        printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                        exit(-1);
                    }
                }
                
                Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, "!=", ptrs);
                break;
            default:
                goto jumpout;
        }
    } while (1);
jumpout:
    
    return Node;
}

ASTNode * expr6(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    Node = expr5();
    
    if (Node == NULL) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    do {        //("|" expr5())*
        if (match(OR)) {
            ptrs[0] = Node;
            ptrs[1] = expr5();
            
            if (ptrs[1] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, "|", ptrs);
        }
        else
            goto jumpout;
    } while (1);
jumpout:
    
    return Node;
}

ASTNode * expr5(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    Node = expr4();
    
    if (Node == NULL) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    do {        //("^" expr4())*
        if (match(XOR)) {
            ptrs[0] = Node;
            ptrs[1] = expr4();
            
            if (ptrs[1] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, "^", ptrs);
        }
        else
            goto jumpout;
    } while (1);
jumpout:
    
    return Node;
}

ASTNode * expr4(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    Node = expr3();
    
    if (Node == NULL) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    do {        //("&" expr3())*
        if (match(AND)) {
            ptrs[0] = Node;
            ptrs[1] = expr3();
            
            if (ptrs[1] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, "&", ptrs);
        }
        else
            goto jumpout;
    } while (1);
jumpout:
    
    return Node;
}

ASTNode * expr3(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    Node = expr2();
    
    if (Node == NULL) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    do {       //(">>" expr2() | "<<" expr2())*
        if (match(RSH)) {
            ptrs[0] = Node;
            ptrs[1] = expr2();
            
            if (ptrs[1] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, ">>", ptrs);
        }
        else if (match(LSH)) {
            ptrs[0] = Node;
            ptrs[1] = expr2();
            
            if (ptrs[1] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, "<<", ptrs);
        }
        else {
            goto jumpout;
        }
    } while (1);
jumpout:
    
    return Node;
}

ASTNode * expr2(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    Node = expr1();
    
    if (Node == NULL) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    do {       //("+" expr1() | "-" expr1())*
        if (match(SUM)) {
            ptrs[0] = Node;
            ptrs[1] = expr1();
            
            if (ptrs[1] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, "+", ptrs);
        }
        else if (match(SUB)) {
            ptrs[0] = Node;
            ptrs[1] = expr1();
            
            if (ptrs[1] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, "-", ptrs);
        }
        else
            goto jumpout;
    } while (1);
jumpout:
    
    return Node;
}

ASTNode * expr1(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    Node = term();
    
    if (Node == NULL) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    do {       //("*" term() | "/" term() | "%" term())*
        if (match(MUL)) {
            ptrs[0] = Node;
            ptrs[1] = term();
            
            if (ptrs[1] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, "*", ptrs);
        }
        else if (match(DIV)) {
            ptrs[0] = Node;
            ptrs[1] = term();
            
            if (ptrs[1] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, "/", ptrs);
        }
        else if (match(RES)){
            ptrs[0] = Node;
            ptrs[1] = term();
            
            if (ptrs[1] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, "%", ptrs);
        }
        else
            goto jumpout;
    } while (1);
jumpout:
    
    return Node;
}

ASTNode * unary(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    Token * lookahead1 = NULL;
    Token * lookahead2 = NULL;
    
    switch (token -> kind) {
        case SELFSUM:
            token = token -> next;
            ptrs[0] = unary();
            
            if (ptrs[0] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            Node = NodeConstructor(PrefixOp, parsingFile, token -> beginLine, "++", ptrs);
            break;
        case SELFSUB:
            token = token -> next;
            ptrs[0] = unary();
            
            if (ptrs[0] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            Node = NodeConstructor(PrefixOp, parsingFile, token -> beginLine, "--", ptrs);
            break;
        case SUM:
            token = token -> next;
            ptrs[0] = term();
            
            if (ptrs[0] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            Node = NodeConstructor(UnaryOp, parsingFile, token -> beginLine, "+", ptrs);
            break;
        case SUB:
            token = token -> next;
            ptrs[0] = term();
            
            if (ptrs[0] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            Node = NodeConstructor(UnaryOp, parsingFile, token -> beginLine, "-", ptrs);
            break;
        case LOGICNOT:
            token = token -> next;
            ptrs[0] = term();
            
            if (ptrs[0] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            Node = NodeConstructor(UnaryOp, parsingFile, token -> beginLine, "!", ptrs);
            break;
        case NOT:
            token = token -> next;
            ptrs[0] = term();
            
            if (ptrs[0] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            Node = NodeConstructor(UnaryOp, parsingFile, token -> beginLine, "~", ptrs);
            break;
        case MUL:
            token = token -> next;
            ptrs[0] = term();
            
            if (ptrs[0] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            Node = NodeConstructor(Dereference, parsingFile, token -> beginLine, NULL, ptrs);
            break;
        case AND:
            token = token -> next;
            ptrs[0] = term();
            
            if (ptrs[0] == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
            
            Node = NodeConstructor(Address, parsingFile, token -> beginLine, NULL, ptrs);
            break;
        case SIZEOF:
            token = token -> next;
            lookahead1 = token;
            lookahead2 = token -> next -> next;
            token -> next -> next = NULL;
            
            prelooking++;
            if (match(LEFTPARENTHESE) && type() != NULL) {
                prelooking--;
                token = lookahead1;
                token -> next -> next = lookahead2;
                
                if (!match(LEFTPARENTHESE)) {
                    if (prelooking)
                        return NULL;
                    else {
                        printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                        exit(-1);
                    }
                }
                
                ptrs[0] = type();
                
                if (ptrs[0] == NULL) {
                    if (prelooking)
                        return NULL;
                    else {
                        printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                        exit(-1);
                    }
                }
                
                if (!match(RIGHTPARENTHESE)) {
                    if (prelooking)
                        return NULL;
                    else {
                        printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                        exit(-1);
                    }
                }
                
                Node = NodeConstructor(SizeofType, parsingFile, token -> beginLine, NULL, ptrs);
            }
            else {
                prelooking--;
                token = lookahead1;
                token -> next -> next = lookahead2;
                
                ptrs[0] = unary();
                
                if (ptrs[0] == NULL) {
                    if (prelooking)
                        return NULL;
                    else {
                        printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                        exit(-1);
                    }
                }
                
                Node = NodeConstructor(SizeofExpr, parsingFile, token -> beginLine, NULL, ptrs);
            }
            break;
        default:
            Node = postfix();
            
            if (Node == NULL) {
                if (prelooking)
                    return NULL;
                else {
                    printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                    exit(-1);
                }
            }
    }
    
    return Node;
}

ASTNode * postfix(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    Node = primary();
    
    if (Node == NULL) {
        if (prelooking)
            return NULL;
        else {
            printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
            exit(-1);
        }
    }
    
    do {    //("++"|"--"|"["expr()"]"|"."name()|"->"name()|"("args()")")*
        switch (token -> kind) {
            case SELFSUM:
                token = token -> next;
                
                ptrs[0] = Node;
                ptrs[1] = NULL;
                
                Node = NodeConstructor(SuffixOp, parsingFile, token -> beginLine, "++", ptrs);
                break;
            case SELFSUB:
                token = token -> next;
                
                ptrs[0] = Node;
                ptrs[1] = NULL;
                
                Node = NodeConstructor(SuffixOp, parsingFile, token -> beginLine, "--", ptrs);
                break;
            case LEFTBRACKET:
                token = token -> next;
                
                ptrs[0] = Node;
                ptrs[1] = expr();
                
                if (ptrs[1] == NULL) {
                    if (prelooking)
                        return NULL;
                    else {
                        printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                        exit(-1);
                    }
                }
                
                Node = NodeConstructor(ArrayRef, parsingFile, token -> beginLine, NULL, ptrs);
                
                if (!match(RIGHTBRACKET)) {
                    if (prelooking)
                        return NULL;
                    else {
                        printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                        exit(-1);
                    }
                }
                break;
            case DOT:
                token = token -> next;
                
                ptrs[0] = Node;
                ptrs[1] = name();
                
                if (ptrs[1] == NULL) {
                    if (prelooking)
                        return NULL;
                    else {
                        printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                        exit(-1);
                    }
                }
                
                Node = NodeConstructor(Member, parsingFile, token -> beginLine, NULL, ptrs);
                break;
            case ARROW:
                token = token -> next;
                
                ptrs[0] = Node;
                ptrs[1] = name();
                
                if (ptrs[1] == NULL) {
                    if (prelooking)
                        return NULL;
                    else {
                        printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                        exit(-1);
                    }
                }
                
                Node = NodeConstructor(PtrMember, parsingFile, token -> beginLine, NULL, ptrs);
                break;
            case LEFTPARENTHESE:
                token = token -> next;
                
                ptrs[0] = Node;
                ptrs[1] = args();
                
                if (ptrs[1] == NULL) {
                    if (prelooking)
                        return NULL;
                    else {
                        printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                        exit(-1);
                    }
                }
                
                Node = NodeConstructor(Funcall, parsingFile, token -> beginLine, NULL, ptrs);
                
                if (!match(RIGHTPARENTHESE)) {
                    if (prelooking)
                        return NULL;
                    else {
                        printf("Melon: %s: syntax \033[31merror\033[0m in line %d\n", parsingFile, token -> beginLine);
                        exit(-1);
                    }
                }
                break;
            default:
                goto jumpout;
                break;
        }
    } while (1);
jumpout:
    
    return Node;
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
