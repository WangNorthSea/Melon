//
//  Parser.c
//  Melon
//
//  Created by 王浩宇 on 2019/3/18.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../Lexer/token.h"
#include "../Lexer/tokenkind.h"
#include "../ASTNode/node.h"
#include "../ASTNode/constructor.h"
#include "../SymbolTable/hashtable.h"
#include "../Report/error.h"

ASTNode * importStmts(void);

ASTNode * topDefs(void);

ASTNode * importStmt(void);

ASTNode * name(void);

ASTNode * defvars(void);

ASTNode * defun(void);

ASTNode * storage(void);

ASTNode * type(void);

ASTNode * expr(void);

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

ASTNode * extern_(void);

ASTNode * getType(char * key);

int match(int kind);

void throwSyntaxError(char * file, char * expected, Token * token);

Token * token = NULL;

char * parsingFile = NULL;

int prelooking = 0;

Hashtable * typeDefList = NULL;

error_t * err_list = NULL;

ASTNode * compilationUnit(Token * headToken) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    token = list_entry(headToken -> list.next, Token, list);
    typeDefList = HashtableConstructor();
    err_list = ErrorConstructor(0, 0, NULL, NULL, NULL);

    if (token == NULL)
        return NodeConstructor(Root, parsingFile, 1, NULL, ptrs);
    
    ptrs[0] = importStmts();
    
    ptrs[1] = topDefs();
    
    match(EOF_);
    
    return NodeConstructor(Root, parsingFile, 1, NULL, ptrs);
}

ASTNode * importStmts(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    Node = NodeConstructor(Imports, parsingFile, token -> beginLine, NULL, ptrs);
    
    do {                             //(importStmt())*
        ptrs[0] = importStmt();
        
        if (ptrs[0] != NULL) {
            Node -> append(Node, *ptrs[0]);
            free(ptrs[0]);
        }
        else
            break;
    } while (1);
    
    return Node;
}

ASTNode * topDefs(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    Token * lookahead1 = token;
    
    Node = NodeConstructor(TopDefs, parsingFile, token -> beginLine, NULL, ptrs);
    
    while (1) {
        if (token -> kind == EOF_)
            break;
        
        prelooking++;
        storage();
        if (type() != NULL && match(IDENTIFIER) && match(LEFTPARENTHESE)) {
            prelooking--;
            token = lookahead1;
            ptrs[0] = defun();
            
            if (ptrs[0] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "function definition");
                //}
            }
            
            Node -> append(Node, *ptrs[0]);
            free(ptrs[0]);
        }
        else {
            prelooking--;
            token = lookahead1;
            
            if (token -> kind != TYPEDEF && token -> kind != EXTERN && token -> kind != CONST && (list_entry((list_entry(token -> list.next, Token, list)) -> list.next, Token, list)) -> kind != LEFTBRACE) {
                ptrs[0] = defvars();
                
                if (ptrs[0] == NULL) {
                    //if (prelooking)
                    return NULL;
                    //else {
                        //throwSyntaxError(parsingFile, "variable definition");
                    //}
                }
                
                Node -> append(Node, *ptrs[0]);
                free(ptrs[0]);
            }
            else {
                switch (token -> kind) {
                    case CONST:
                        ptrs[0] = defconst();
                        
                        if (ptrs[0] == NULL) {
                            //if (prelooking)
                            return NULL;
                            //else {
                                //throwSyntaxError(parsingFile, "constant definition");
                            //}
                        }
                        
                        Node -> append(Node, *ptrs[0]);
                        free(ptrs[0]);
                        break;
                    case STRUCT:
                        ptrs[0] = defstruct();
                        
                        if (ptrs[0] == NULL) {
                            //if (prelooking)
                            return NULL;
                            //else {
                                //throwSyntaxError(parsingFile, "struct definition");
                            //}
                        }
                        
                        Node -> append(Node, *ptrs[0]);
                        free(ptrs[0]);
                        break;
                    case UNION:
                        ptrs[0] = defunion();
                        
                        if (ptrs[0] == NULL) {
                            //if (prelooking)
                            return NULL;
                            //else {
                                //throwSyntaxError(parsingFile, "union definition");
                            //}
                        }
                        
                        Node -> append(Node, *ptrs[0]);
                        free(ptrs[0]);
                        break;
                    case TYPEDEF:
                        ptrs[0] = typedef_();
                        
                        if (ptrs[0] == NULL) {
                            //if (prelooking)
                            return NULL;
                            //else {
                                //throwSyntaxError(parsingFile, "typedef statement");
                            //}
                        }
                        
                        Node -> append(Node, *ptrs[0]);
                        free(ptrs[0]);
                        break;
                    case EXTERN:
                        ptrs[0] = extern_();
                        
                        if (ptrs[0] == NULL) {
                            //if (prelooking)
                            return NULL;
                            //else {
                                //throwSyntaxError(parsingFile, "extern statement");
                            //}
                        }
                        
                        Node -> append(Node, *ptrs[0]);
                        free(ptrs[0]);
                        break;
                    default:
                        goto jumpout;
                }
            }
        }
        
        lookahead1 = token;
    }
jumpout:
    
    return Node;
}

ASTNode * importStmt(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    Node = NodeConstructor(Import, parsingFile, token -> beginLine, NULL, ptrs);
    
    if (!match(IMPORT)) {
        return NULL;
    }
    
    ptrs[0] = name();
    
    if (ptrs[0] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "identifier");
        //}
    }
    
    Node -> append(Node, *ptrs[0]);
    free(ptrs[0]);
    
    do {                        //("." name())*
        if (match(DOT)) {
            ptrs[0] = name();
            
            if (ptrs[0] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "identifier");
                //}
            }
            
            Node -> append(Node, *ptrs[0]);
            free(ptrs[0]);
        }
        else
            break;
    } while (1);
    
    if (!match(SEMICOLON)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\';\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    return Node;
}

ASTNode * name(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    char * label = NULL;
    
    label = token -> image;
    if (!match(IDENTIFIER)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "identifier", list_entry(token -> list.prev, Token, list));
        }
    }
    return NodeConstructor(Name, parsingFile, token -> beginLine, label, ptrs);
}

ASTNode * storage(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (match(STATIC))
        return NodeConstructor(Static, parsingFile, token -> beginLine, NULL, ptrs);
    else
        return NULL;
}

ASTNode * defvars(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    ASTNode * Node = NULL;
    ASTNode * temp = NULL;
    int isFuncPtr = 0;
    
    Node = NodeConstructor(DefinedVariables, parsingFile, token -> beginLine, NULL, ptrs);
    
    ptrs[0] = storage();           //可选
    
    ptrs[1] = type();
    
    if (ptrs[1] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "variable type");
        //}
    }
    
    if (match(LEFTPARENTHESE)) {        //函数指针
        isFuncPtr = 1;
        
        if (!match(MUL)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\'*\'", list_entry(token -> list.prev, Token, list));
            }
        }
        
        ptrs[2] = name();
        
        if (ptrs[2] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "identifier");
            //}
        }
        
        if (!match(RIGHTPARENTHESE)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\')\'", list_entry(token -> list.prev, Token, list));
            }
        }
        
        if (!match(LEFTPARENTHESE)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\'(\'", list_entry(token -> list.prev, Token, list));
            }
        }
        
        ptrs[3] = params();
        
        if (ptrs[3] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "parameters");
            //}
        }
        
        if (!match(RIGHTPARENTHESE)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\')\'", list_entry(token -> list.prev, Token, list));
            }
        }
    }
    else {
        ptrs[2] = varname();
        
        if (ptrs[2] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "variable name");
            //}
        }
    }
    
    if (match(ASSIGN)) {  //["=" expr()]
        if (isFuncPtr) {
            ptrs[4] = expr();
            
            if (ptrs[4] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "expression");
                //}
            }
        }
        else {
            ptrs[3] = expr();
            
            if (ptrs[3] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "expression");
                //}
            }
        }
    }
    
    if (isFuncPtr) {
        if (!match(SEMICOLON)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\';\'", list_entry(token -> list.prev, Token, list));
            }
        }
        
        temp = NodeConstructor(FuncPtr, parsingFile, token -> beginLine, NULL, ptrs);
        Node -> append(Node, *temp);
        free(temp);
        
        return Node;
    }
    
    temp = NodeConstructor(Variable, parsingFile, token -> beginLine, NULL, ptrs);
    Node -> append(Node, *temp);
    free(temp);
    
    do {                   //("," varname() ["=" expr()])*
        if (match(COMMA)) {
            ptrs[2] = varname();
            
            if (ptrs[2] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "variable name");
                //}
            }
            
            ptrs[3] = NULL;
            
            if (match(ASSIGN)) {
                ptrs[3] = expr();
                
                if (ptrs[3] == NULL) {
                    //if (prelooking)
                    return NULL;
                    //else {
                        //throwSyntaxError(parsingFile, "expression");
                    //}
                }
            }
            
            temp = NodeConstructor(Variable, parsingFile, token -> beginLine, NULL, ptrs);
            Node -> append(Node, *temp);
            free(temp);
        }
        else
            goto jumpout;
    } while (1);
jumpout:
    
    if (!match(SEMICOLON)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\';\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    return Node;
}

ASTNode * defun(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    ptrs[0] = storage();       //可选
    
    ptrs[1] = type();
    
    if (ptrs[1] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "return value type");
        //}
    }
    
    ptrs[2] = name();
    
    if (ptrs[2] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "identifier");
        //}
    }
    
    if (!match(LEFTPARENTHESE)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'(\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    ptrs[3] = params();
    
    if (ptrs[3] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "parameters");
        //}
    }
    
    if (!match(RIGHTPARENTHESE)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\')\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    if (match(SEMICOLON))
        return NodeConstructor(FuncStmt, parsingFile, token -> beginLine, NULL, ptrs);

    ptrs[4] = block();
    
    if (ptrs[4] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "block statements");
        //}
    }
    
    return NodeConstructor(DefinedFunc, parsingFile, token -> beginLine, NULL, ptrs);
}

ASTNode * type(void) {
    ASTNode * Node = NULL;
    ASTNode * temp = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    char * label = NULL;
    
    switch (token -> kind) {
        case VOID:
            Node = NodeConstructor(VoidType, parsingFile, token -> beginLine, NULL, ptrs);
            token = list_entry(token -> list.next, Token, list);
            break;
        case CHAR:
            Node = NodeConstructor(CharType, parsingFile, token -> beginLine, NULL, ptrs);
            token = list_entry(token -> list.next, Token, list);
            break;
        case SHORT:
            Node = NodeConstructor(ShortIntType, parsingFile, token -> beginLine, NULL, ptrs);
            token = list_entry(token -> list.next, Token, list);
            break;
        case INT:
            Node = NodeConstructor(IntType, parsingFile, token -> beginLine, NULL, ptrs);
            token = list_entry(token -> list.next, Token, list);
            break;
        case LONG:
            Node = NodeConstructor(LongIntType, parsingFile, token -> beginLine, NULL, ptrs);
            token = list_entry(token -> list.next, Token, list);
            break;
        case FLOAT:
            Node = NodeConstructor(FloatType, parsingFile, token -> beginLine, NULL, ptrs);
            token = list_entry(token -> list.next, Token, list);
            break;
        case DOUBLE:
            Node = NodeConstructor(DoubleType, parsingFile, token -> beginLine, NULL, ptrs);
            token = list_entry(token -> list.next, Token, list);
            break;
        case UNSIGNED:
            token = list_entry(token -> list.next, Token, list);
            switch (token -> kind) {
                case CHAR:
                    Node = NodeConstructor(UnsignedCharType, parsingFile, token -> beginLine, NULL, ptrs);
                    token = list_entry(token -> list.next, Token, list);
                    break;
                case SHORT:
                    Node = NodeConstructor(UnsignedShortIntType, parsingFile, token -> beginLine, NULL, ptrs);
                    token = list_entry(token -> list.next, Token, list);
                    break;
                case INT:
                    Node = NodeConstructor(UnsignedIntType, parsingFile, token -> beginLine, NULL, ptrs);
                    token = list_entry(token -> list.next, Token, list);
                    break;
                case LONG:
                    Node = NodeConstructor(UnsignedLongIntType, parsingFile, token -> beginLine, NULL, ptrs);
                    token = list_entry(token -> list.next, Token, list);
                    break;
                default:
                    if (prelooking)
                        return NULL;
                    else {
                        throwSyntaxError(parsingFile, "legitimate type", list_entry(token -> list.prev, Token, list));
                    }
                    break;
            }
            break;
        case STRUCT:
            token = list_entry(token -> list.next, Token, list);
            label = token -> image;
            if (!match(IDENTIFIER)) {
                if (prelooking)
                    return NULL;
                else {
                    throwSyntaxError(parsingFile, "identifier", list_entry(token -> list.prev, Token, list));
                }
            }
            
            Node = NodeConstructor(StructType, parsingFile, token -> beginLine, label, ptrs);
            break;
        case UNION:
            token = list_entry(token -> list.next, Token, list);
            label = token -> image;
            if (!match(IDENTIFIER)) {
                if (prelooking)
                    return NULL;
                else {
                    throwSyntaxError(parsingFile, "identifier", list_entry(token -> list.prev, Token, list));
                }
            }
            
            Node = NodeConstructor(UnionType, parsingFile, token -> beginLine, label, ptrs);
            break;
        default:
            Node = getType(token -> image);
            if (Node != NULL)
                token = list_entry(token -> list.next, Token, list);
            else
                return Node;
            break;
    }
    
    do {                    //("*")*
        if (match(MUL)) {
            temp = NodeConstructor(PtrRef, parsingFile, token -> beginLine, NULL, ptrs);
            Node -> append(Node, *temp);
            free(temp);
        }
        else
            break;
    } while (1);
    
    return Node;
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
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "term");
            //}
        }
        
        if (!match(ASSIGN)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\'=\'", list_entry(token -> list.prev, Token, list));
            }
        }
        
        ptrs[1] = expr();
        
        if (ptrs[1] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "expression");
            //}
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
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "term");
                //}
            }
            
            ptrs[1] = opassign_op();
            
            if (ptrs[1] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "assign operator");
                //}
            }
            
            ptrs[2] = expr();
            
            if (ptrs[2] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "expression");
                //}
            }
            
            return NodeConstructor(OpAssign, parsingFile, token -> beginLine, NULL, ptrs);
        }
        else {
            prelooking--;
            token = lookahead;
            Node = expr10();
            
            if (Node == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "expression");
                //}
            }
            
            return Node;
        }
    }
    
    return Node;
}

ASTNode * params(void) {
    Token * lookahead = token;
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    Node = NodeConstructor(ParamsNode, parsingFile, token -> beginLine, NULL, ptrs);
    
    if (match(VOID) && match(RIGHTPARENTHESE)) {         //LOOKAHEAD(<VOID> ")")
        token = lookahead;
        if (!match(VOID)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\'void\'", list_entry(token -> list.prev, Token, list));
            }
        }
    }
    else {
        token = lookahead;
        free(Node);
        Node = fixedParams();
        
        if (Node == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "parameters");
            //}
        }
        
        if (match(COMMA)) {  //["," "..."]
            if (!match(DOT) || !match(DOT) || !match(DOT)) {
                if (prelooking)
                    return NULL;
                else {
                    throwSyntaxError(parsingFile, "\'...\'", list_entry(token -> list.prev, Token, list));
                }
            }
            ASTNode * temp = NodeConstructor(UnlimitedParams, parsingFile, token -> beginLine, NULL, ptrs);
            Node -> append(Node, *temp);
            free(temp);
        }
    }
    
    return Node;
}

ASTNode * block(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (!match(LEFTBRACE)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'{\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    ptrs[0] = stmts();        //没有语句则返回NULL，非语法错误
    
    if (!match(RIGHTBRACE)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'}\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    return NodeConstructor(Block, parsingFile, token -> beginLine, NULL, ptrs);
}

ASTNode * fixedParams(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    Node = NodeConstructor(ParamsNode, parsingFile, token -> beginLine, NULL, ptrs);
    
    ptrs[0] = param();
    
    if (ptrs[0] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "parameter");
        //}
    }
    
    Node -> append(Node, *ptrs[0]);
    free(ptrs[0]);
    
    Token * lookahead = token;
    
    do {                                     //(LOOKAHEAD(2) "," param())* 区分, ...
        if (match(COMMA) && !match(DOT)) {
            token = lookahead;
            if (match(COMMA))
                ;
            ptrs[0] = param();
            
            if (ptrs[0] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "parameter");
                //}
            }
            
            Node -> append(Node, *ptrs[0]);
            free(ptrs[0]);
        }
        else {
            token = lookahead;
            goto jumpout;
        }
        
        lookahead = token;
    } while (1);
jumpout:
    
    return Node;
}

ASTNode * param(void) {
    int isFuncPtr = 0;
    int isConst = 0;
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (match(CONST))
        isConst = 1;
    
    ptrs[0] = type();
    
    if (ptrs[0] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "parameter type");
        //}
    }
    
    if (match(LEFTPARENTHESE)) {        //函数指针
        isFuncPtr = 1;
        
        if (!match(MUL)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\'*\'", list_entry(token -> list.prev, Token, list));
            }
        }
        
        ptrs[1] = name();
        
        if (ptrs[1] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "identifier");
            //}
        }
        
        if (!match(RIGHTPARENTHESE)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\')\'", list_entry(token -> list.prev, Token, list));
            }
        }
        
        if (!match(LEFTPARENTHESE)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\'(\'", list_entry(token -> list.prev, Token, list));
            }
        }
        
        ptrs[2] = params();
        
        if (ptrs[2] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "parameters");
            //}
        }
        
        if (!match(RIGHTPARENTHESE)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\')\'", list_entry(token -> list.prev, Token, list));
            }
        }
    }
    else {
        ptrs[1] = varname();
        
        if (ptrs[1] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "variable name");
            //}
        }
    }
    
    if (isFuncPtr)
        Node = NodeConstructor(FuncPtrParam, parsingFile, token -> beginLine, NULL, ptrs);
    else {
        if (isConst)
            Node = NodeConstructor(ConstParam, parsingFile, token -> beginLine, NULL, ptrs);
        else
            Node = NodeConstructor(NormalParam, parsingFile, token -> beginLine, NULL, ptrs);
    }
    
    return Node;
}

ASTNode * defconst(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    ASTNode * Node = NULL;
    ASTNode * temp = NULL;
    int isFuncPtr = 0;
    
    if (!match(CONST)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'const\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    Node = NodeConstructor(DefinedConstVariables, parsingFile, token -> beginLine, NULL, ptrs);
    
    ptrs[0] = storage();           //可选
    
    ptrs[1] = type();
    
    if (ptrs[1] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "data type");
        //}
    }
    
    if (match(LEFTPARENTHESE)) {        //函数指针
        isFuncPtr = 1;
        
        if (!match(MUL)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\'*\'", list_entry(token -> list.prev, Token, list));
            }
        }
        
        ptrs[2] = name();
        
        if (ptrs[2] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "identifier");
            //}
        }
        
        if (!match(RIGHTPARENTHESE)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\')\'", list_entry(token -> list.prev, Token, list));
            }
        }
        
        if (!match(LEFTPARENTHESE)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\'(\'", list_entry(token -> list.prev, Token, list));
            }
        }
        
        ptrs[3] = params();
        
        if (ptrs[3] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "parameters");
            //}
        }
        
        if (!match(RIGHTPARENTHESE)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\')\'", list_entry(token -> list.prev, Token, list));
            }
        }
    }
    else {
        ptrs[2] = varname();
        
        if (ptrs[2] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "variable name");
            //}
        }
    }
    
    if (match(ASSIGN)) {  //["=" expr()]
        if (isFuncPtr) {
            ptrs[4] = expr();
            
            if (ptrs[4] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "\'=\'");
                //}
            }
        }
        else {
            ptrs[3] = expr();
            
            if (ptrs[3] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "expression");
                //}
            }
        }
    }
    
    if (isFuncPtr) {
        if (!match(SEMICOLON)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\';\'", list_entry(token -> list.prev, Token, list));
            }
        }
        
        temp = NodeConstructor(ConstFuncPtr, parsingFile, token -> beginLine, NULL, ptrs);
        Node -> append(Node, *temp);
        free(temp);
        
        return Node;
    }
    
    temp = NodeConstructor(ConstVariable, parsingFile, token -> beginLine, NULL, ptrs);
    Node -> append(Node, *temp);
    free(temp);
    
    do {                   //("," varname() ["=" expr()])*
        if (match(COMMA)) {
            ptrs[2] = varname();
            
            if (ptrs[2] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "variable name");
                //}
            }
            
            ptrs[3] = NULL;
            
            if (match(ASSIGN)) {
                ptrs[3] = expr();
                
                if (ptrs[3] == NULL) {
                    //if (prelooking)
                    return NULL;
                    //else {
                        //throwSyntaxError(parsingFile, "expression");
                    //}
                }
            }
            
            temp = NodeConstructor(Variable, parsingFile, token -> beginLine, NULL, ptrs);
            Node -> append(Node, *temp);
            free(temp);
        }
        else
            goto jumpout;
    } while (1);
jumpout:
    
    if (!match(SEMICOLON)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\';\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    return Node;
}

ASTNode * defstruct(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (!match(STRUCT)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'struct\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    ptrs[0] = name();
    
    if (ptrs[0] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "identifier");
        //}
    }
    
    ptrs[1] = member_list();
    
    if (ptrs[1] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "members");
        //}
    }
    
    if (!match(SEMICOLON)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\';\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    return NodeConstructor(DefinedStruct, parsingFile, token -> beginLine, NULL, ptrs);
}

ASTNode * defunion(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (!match(UNION)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'union\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    ptrs[0] = name();
    
    if (ptrs[0] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "identifier");
        //}
    }
    
    ptrs[1] = member_list();
    
    if (ptrs[1] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "members");
        //}
    }
    
    if (!match(SEMICOLON)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\';\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    return NodeConstructor(DefinedUnion, parsingFile, token -> beginLine, NULL, ptrs);
}

ASTNode * typedef_(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (!match(TYPEDEF)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'typedef\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    ptrs[0] = type();
    
    if (ptrs[0] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "data type");
        //}
    }
    
    ptrs[1] = name();
    
    if (ptrs[1] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "identifier");
        //}
    }
    
    if (!match(SEMICOLON)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\';\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    typeDefList -> put(typeDefList, ptrs[1] -> image, ptrs[0]);
    
    return NodeConstructor(TypeDef, parsingFile, token -> beginLine, NULL, ptrs);
}

ASTNode * stmts(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    Node = NodeConstructor(Stmts, parsingFile, token -> beginLine, NULL, ptrs);
    
    do {                         //(stmt())*
        ptrs[0] = stmt();
        
        if (ptrs[0] == NULL)
            goto jumpout;
        
        Node -> append(Node, *ptrs[0]);
        free(ptrs[0]);
    } while (1);
jumpout:
    
    return Node;
}

ASTNode * stmt(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    Token * lookahead = token;
    
    if (match(SEMICOLON)) {
        Node = NodeConstructor(EmptyStmt, parsingFile, token -> beginLine, NULL, ptrs);
    }
    else if (token -> kind == IDENTIFIER && (list_entry(token -> list.next, Token, list)) -> kind == COLON) {
        ptrs[0] = labeled_stmt();
        
        if (ptrs[0] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "label statement");
            //}
        }
        
        Node = NodeConstructor(Stmt, parsingFile, token -> beginLine, NULL, ptrs);
    }
    else if (getType(token -> image) != NULL){
        prelooking++;
        storage();
        if (type() != NULL) {
            prelooking--;
            token = lookahead;
            ptrs[0] = defvars();
            
            if (ptrs[0] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "variable definition");
                //}
            }
            
            Node = NodeConstructor(Stmt, parsingFile, token -> beginLine, NULL, ptrs);
        }
        else {
            prelooking--;
            token = lookahead;
        }
    }
    else if (token -> kind == LEFTPARENTHESE || token -> kind == SELFSUM || token -> kind == SELFSUB || token -> kind == SUM || token -> kind == SUB || token -> kind == LOGICNOT || token -> kind == NOT || token -> kind == MUL || token -> kind == AND || token -> kind == SIZEOF || token -> kind == INTEGER || token -> kind == CHARACTER || token -> kind == STRING || token -> kind == IDENTIFIER) {
        ptrs[0] = expr();
        
        if (ptrs[0] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "expression");
            //}
        }
        
        if (!match(SEMICOLON)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\';\'", list_entry(token -> list.prev, Token, list));
            }
        }
        
        Node = NodeConstructor(Stmt, parsingFile, token -> beginLine, NULL, ptrs);
    }
    else if (token -> kind == LEFTBRACE) {
        ptrs[0] = block();
        
        if (ptrs[0] == NULL) {
            //if (prelooking)
                return NULL;
            //else {
                //throwSyntaxError(parsingFile, "block statements");
            //}
        }
        
        Node = NodeConstructor(Stmt, parsingFile, token -> beginLine, NULL, ptrs);
    }
    else if (token -> kind == IF) {
        ptrs[0] = if_stmt();
        
        if (ptrs[0] == NULL) {
            //if (prelooking)
                return NULL;
            //else {
                //throwSyntaxError(parsingFile, "\'if\' statement");
            //}
        }
        
        Node = NodeConstructor(Stmt, parsingFile, token -> beginLine, NULL, ptrs);
    }
    else if (token -> kind == WHILE) {
        ptrs[0] = while_stmt();
        
        if (ptrs[0] == NULL) {
            //if (prelooking)
                return NULL;
            //else {
                //throwSyntaxError(parsingFile, "\'while\' statement");
            //}
        }
        
        Node = NodeConstructor(Stmt, parsingFile, token -> beginLine, NULL, ptrs);
    }
    else if (token -> kind == DO) {
        ptrs[0] = dowhile_stmt();
        
        if (ptrs[0] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "\'do while\' statement");
            //}
        }
        
        Node = NodeConstructor(Stmt, parsingFile, token -> beginLine, NULL, ptrs);
    }
    else if (token -> kind == FOR) {
        ptrs[0] = for_stmt();
        
        if (ptrs[0] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "\'for\' statement");
            //}
        }
        
        Node = NodeConstructor(Stmt, parsingFile, token -> beginLine, NULL, ptrs);
    }
    else if (token -> kind == SWITCH) {
        ptrs[0] = switch_stmt();
        
        if (ptrs[0] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "\'switch\' statement");
            //}
        }
        
        Node = NodeConstructor(Stmt, parsingFile, token -> beginLine, NULL, ptrs);
    }
    else if (token -> kind == BREAK) {
        ptrs[0] = break_stmt();
        
        if (ptrs[0] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "\'break\' statement");
            //}
        }
        
        Node = NodeConstructor(Stmt, parsingFile, token -> beginLine, NULL, ptrs);
    }
    else if (token -> kind == CONTINUE) {
        ptrs[0] = continue_stmt();
        
        if (ptrs[0] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "\'continue\' statement");
            //}
        }
        
        Node = NodeConstructor(Stmt, parsingFile, token -> beginLine, NULL, ptrs);
    }
    else if (token -> kind == GOTO) {
        ptrs[0] = goto_stmt();
        
        if (ptrs[0] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "\'goto\' statement");
            //}
        }
        
        Node = NodeConstructor(Stmt, parsingFile, token -> beginLine, NULL, ptrs);
    }
    else if (token -> kind == RETURN) {
        ptrs[0] = return_stmt();
        
        if (ptrs[0] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "\'return\' statement");
            //}
        }
        
        Node = NodeConstructor(Stmt, parsingFile, token -> beginLine, NULL, ptrs);
    }
    else if (token -> kind == CONST) {
        ptrs[0] = defconst();
        
        if (ptrs[0] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "constant definition");
            //}
        }
        
        Node = NodeConstructor(Stmt, parsingFile, token -> beginLine, NULL, ptrs);
    }
    else {
        prelooking++;
        storage();
        if (type() != NULL) {
            prelooking--;
            token = lookahead;
            ptrs[0] = defvars();
            
            if (ptrs[0] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "variable definition");
                //}
            }
            
            Node = NodeConstructor(Stmt, parsingFile, token -> beginLine, NULL, ptrs);
        }
        else {
            prelooking--;
            token = lookahead;
        }
    }
    
    return Node;
}

ASTNode * member_list(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    Node = NodeConstructor(MemberList, parsingFile, token -> beginLine, NULL, ptrs);
    
    if (!match(LEFTBRACE)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'{\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    do {                          //(slot())*
        ptrs[0] = slot();
        
        if (ptrs[0] != NULL) {
            Node -> append(Node, *ptrs[0]);
            free(ptrs[0]);
        }
        else
            break;
    } while (1);
    
    if (!match(RIGHTBRACE)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'}\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    return Node;
}

ASTNode * slot(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    ASTNode * Node = NULL;
    ASTNode * temp = NULL;
    int isFuncPtr = 0;
    int isConst = 0;
    
    Node = NodeConstructor(Slot, parsingFile, token -> beginLine, NULL, ptrs);
    
    if (match(CONST))
        isConst = 1;
    
    ptrs[0] = type();
    
    if (ptrs[0] == NULL)
        return NULL;
    
    if (match(LEFTPARENTHESE)) {        //函数指针
        isFuncPtr = 1;
        
        if (!match(MUL)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\'*\'", list_entry(token -> list.prev, Token, list));
            }
        }
        
        ptrs[1] = name();
        
        if (ptrs[1] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "identifier");
            //}
        }
        
        if (!match(RIGHTPARENTHESE)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\')\'", list_entry(token -> list.prev, Token, list));
            }
        }
        
        if (!match(LEFTPARENTHESE)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\'(\'", list_entry(token -> list.prev, Token, list));
            }
        }
        
        ptrs[2] = params();
        
        if (ptrs[2] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "parameters");
            //}
        }
        
        if (!match(RIGHTPARENTHESE)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\')\'", list_entry(token -> list.prev, Token, list));
            }
        }
    }
    else {
        ptrs[1] = varname();
        
        if (ptrs[1] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "variable name");
            //}
        }
    }
    
    if (isFuncPtr) {
        if (!match(SEMICOLON)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\';\'", list_entry(token -> list.prev, Token, list));
            }
        }
        
        if (isConst == 1)
            temp = NodeConstructor(ConstFuncPtr, parsingFile, token -> beginLine, NULL, ptrs);
        else
            temp = NodeConstructor(FuncPtr, parsingFile, token -> beginLine, NULL, ptrs);
        Node -> append(Node, *temp);
        free(temp);
        
        return Node;
    }
    
    if (isConst == 1)
        temp = NodeConstructor(ConstVariable, parsingFile, token -> beginLine, NULL, ptrs);
    else
        temp = NodeConstructor(Variable, parsingFile, token -> beginLine, NULL, ptrs);
    Node -> append(Node, *temp);
    free(temp);
    
    do {                   //("," varname())*
        if (match(COMMA)) {
            ptrs[1] = varname();
            
            if (ptrs[1] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "variable name");
                //}
            }
            
            if (isConst == 1)
                temp = NodeConstructor(ConstVariable, parsingFile, token -> beginLine, NULL, ptrs);
            else
                temp = NodeConstructor(Variable, parsingFile, token -> beginLine, NULL, ptrs);
            Node -> append(Node, *temp);
            free(temp);
        }
        else
            goto jumpout;
    } while (1);
jumpout:
    
    if (!match(SEMICOLON)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\';\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    return Node;
}

ASTNode * varname(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    ASTNode * Node = NULL;
    ASTNode * temp = NULL;
    
    Node = NodeConstructor(Varname, parsingFile, token -> beginLine, NULL, ptrs);
    
    ptrs[0] = name();
    
    if (ptrs[0] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "identifier");
        //}
    }
    
    Node -> ptrs[0] = ptrs[0];
    
    do {
        temp = array();               //零个或多个
        
        if (temp != NULL) {
            Node -> append(Node, *temp);
            free(temp);
        }
        else
            break;
    } while (1);
    
    return Node;
}

ASTNode * array(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    char * label = NULL;
    
    if (token -> kind == LEFTBRACKET && (list_entry(token -> list.next, Token, list)) -> kind == RIGHTBRACKET) {
        match(LEFTBRACKET);
        match(RIGHTBRACKET);
        
        return NodeConstructor(UnfixedArray, parsingFile, token -> beginLine, NULL, ptrs);
    }
    else {
        if (match(LEFTBRACKET)) {
            label = token -> image;
            if (match(INTEGER)) {
                if (match(RIGHTBRACKET)) {
                    return NodeConstructor(FixedArray, parsingFile, token -> beginLine, label, ptrs);
                }
                else {
                    if (prelooking)
                        return NULL;
                    else {
                        throwSyntaxError(parsingFile, "\']\'", list_entry(token -> list.prev, Token, list));
                    }
                }
            }
            else {
                if (prelooking)
                    return NULL;
                else {
                    throwSyntaxError(parsingFile, "integer literal", list_entry(token -> list.prev, Token, list));
                }
            }
        }
        else {
            return NULL;
        }
    }
    return NULL;
}

ASTNode * labeled_stmt(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    char * label = NULL;
    
    label = token -> image;
    if (!match(IDENTIFIER)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "identifier", list_entry(token -> list.prev, Token, list));
        }
    }
    
    if (!match(COLON)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\':\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    ptrs[0] = stmt();                   //label其实记录的是这条语句的地址
    
    if (ptrs[0] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "statement");
        //}
    }
    
    return NodeConstructor(Label, parsingFile, token -> beginLine, label, ptrs);
}

ASTNode * if_stmt(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (!match(IF)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'if\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    if (!match(LEFTPARENTHESE)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'(\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    if (token -> kind != RIGHTPARENTHESE)
        ptrs[0] = expr();
    
    if (ptrs[0] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "expression");
        //}
    }
    
    if (!match(RIGHTPARENTHESE)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\')\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    ptrs[1] = stmt();
    
    if (ptrs[1] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "statement");
        //}
    }
    
    if (match(ELSE)) {     //[LOOKAHEAD(1) <ELSE> stmt()]
        ptrs[2] = stmt();
        
        if (ptrs[2] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "statement");
            //}
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
            throwSyntaxError(parsingFile, "\'while\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    if (!match(LEFTPARENTHESE)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'(\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    if (token -> kind != RIGHTPARENTHESE)
        ptrs[0] = expr();
    
    if (ptrs[0] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "expression");
        //}
    }
    
    if (!match(RIGHTPARENTHESE)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\')\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    ptrs[1] = stmt();
    
    if (ptrs[1] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "statement");
        //}
    }
    
    return NodeConstructor(While, parsingFile, token -> beginLine, NULL, ptrs);
}

ASTNode * dowhile_stmt(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (!match(DO)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'do\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    ptrs[0] = stmt();
    
    if (ptrs[0] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "statement");
        //}
    }
    
    if (!match(WHILE)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'while\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    if (!match(LEFTPARENTHESE)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'(\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    if (token -> kind != RIGHTPARENTHESE)
        ptrs[1] = expr();
    
    if (ptrs[1] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "expression");
        //}
    }
    
    if (!match(RIGHTPARENTHESE)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\')\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    if (!match(SEMICOLON)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\';\'", list_entry(token -> list.prev, Token, list));
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
            throwSyntaxError(parsingFile, "\'for\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    if (!match(LEFTPARENTHESE)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'(\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    ptrs[0] = expr();      //可选
    
    if (!match(SEMICOLON)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\';\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    ptrs[1] = expr();      //可选
    
    if (!match(SEMICOLON)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\';\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    ptrs[2] = expr();      //可选
    
    if (!match(RIGHTPARENTHESE)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\')\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    ptrs[3] = stmt();
    
    if (ptrs[3] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "statement");
        //}
    }
    
    return NodeConstructor(For, parsingFile, token -> beginLine, NULL, ptrs);
}

ASTNode * switch_stmt(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (!match(SWITCH)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'switch\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    if (!match(LEFTPARENTHESE)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'(\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    if (token -> kind != RIGHTPARENTHESE)
        ptrs[0] = expr();
    
    if (ptrs[0] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "expression");
        //}
    }
    
    if (!match(RIGHTPARENTHESE)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\')\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    if (!match(LEFTBRACE)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'{\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    ptrs[1] = case_clauses();
    
    if (ptrs[1] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "case clauses");
        //}
    }
    
    if (!match(RIGHTBRACE)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'}\'", list_entry(token -> list.prev, Token, list));
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
            throwSyntaxError(parsingFile, "\'break\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    if (!match(SEMICOLON)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\';\'", list_entry(token -> list.prev, Token, list));
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
            throwSyntaxError(parsingFile, "\'continue\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    if (!match(SEMICOLON)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\';\'", list_entry(token -> list.prev, Token, list));
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
            throwSyntaxError(parsingFile, "\'goto\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    label = token -> image;
    if (!match(IDENTIFIER)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "identifier", list_entry(token -> list.prev, Token, list));
        }
    }
    
    if (!match(SEMICOLON)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\';\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    return NodeConstructor(Goto, parsingFile, token -> beginLine, label, ptrs);
}

ASTNode * return_stmt(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (!match(RETURN)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'return\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    if (token -> kind == SEMICOLON)
        match(SEMICOLON);
    else {
        ptrs[0] = expr();
        
        if (ptrs[0] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "expression");
            //}
        }
        
        if (!match(SEMICOLON)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\';\'", list_entry(token -> list.prev, Token, list));
            }
        }
    }
    
    return NodeConstructor(Return, parsingFile, token -> beginLine, NULL, ptrs);;
}

ASTNode * case_clauses(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    Node = NodeConstructor(CaseClauses, parsingFile, token -> beginLine, NULL, ptrs);
    
    do {                              //(case_clause())*
        ptrs[0] = case_clause();
        
        if (ptrs[0] == NULL)
            goto jumpout;
        
        Node -> append(Node, *ptrs[0]);
        free(ptrs[0]);
    } while (1);
jumpout:
    
    ptrs[0] = default_clause();         //必需
    
    if (ptrs[0] == NULL) {
        //if (prelooking)
            return NULL;
        //else {
            //throwSyntaxError(parsingFile, "default clause");
        //}
    }
    
    Node -> append(Node, *ptrs[0]);
    free(ptrs[0]);
    
    return Node;
}

ASTNode * case_clause(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    ptrs[0] = cases();
    
    if (ptrs[0] == NULL)
        return NULL;
    
    ptrs[1] = case_body();
    
    if (ptrs[1] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "case body");
        //}
    }
    
    return NodeConstructor(CaseClause, parsingFile, token -> beginLine, NULL, ptrs);
}

ASTNode * default_clause(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (!match(DEFAULT)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'default\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    if (!match(COLON)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\':\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    ptrs[0] = case_body();
    
    if (ptrs[0] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "default body");
        //}
    }
    
    return NodeConstructor(DefaultClause, parsingFile, token -> beginLine, NULL, ptrs);
}

ASTNode * cases(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    if (!match(CASE)) {
        return NULL;
    }
    
    ptrs[0] = primary();
    
    if (ptrs[0] == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "case situation");
        //}
    }
    
    if (!match(COLON)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\':\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    return NodeConstructor(Cases, parsingFile, token -> beginLine, NULL, ptrs);
}

ASTNode * case_body(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    Node = NodeConstructor(CaseBody, parsingFile, token -> beginLine, NULL, ptrs);
    
    ptrs[0] = stmt();            //至少有一个stmt()
    
    if (ptrs[0] == NULL) {
        //if (prelooking)
            return NULL;
        //else {
            //throwSyntaxError(parsingFile, "statement");
        //}
    }
    
    Node -> append(Node, *ptrs[0]);
    free(ptrs[0]);
    
    do {                         //(stmt())*
        ptrs[0] = stmt();
        
        if (ptrs[0] == NULL)
            goto jumpout;
        
        Node -> append(Node, *ptrs[0]);
        free(ptrs[0]);
    } while (1);
jumpout:
    
    return Node;
}

ASTNode * primary(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    ASTNode * Node = NULL;
    
    switch (token -> kind) {
        case INTEGER:
            Node = NodeConstructor(IntegerLiteral, parsingFile, token -> beginLine, token -> image, ptrs);
            token = list_entry(token -> list.next, Token, list);
            break;
        case CHARACTER:
            Node = NodeConstructor(CharacterLiteral, parsingFile, token -> beginLine, token -> image, ptrs);
            token = list_entry(token -> list.next, Token, list);
            break;
        case FLOAT_:
            Node = NodeConstructor(FloatLiteral, parsingFile, token -> beginLine, token -> image, ptrs);
            token = list_entry(token -> list.next, Token, list);
            break;
        case STRING:
            Node = NodeConstructor(StringLiteral, parsingFile, token -> beginLine, token -> image, ptrs);
            token = list_entry(token -> list.next, Token, list);
            break;
        case IDENTIFIER:
            Node = NodeConstructor(Identifier, parsingFile, token -> beginLine, token -> image, ptrs);
            token = list_entry(token -> list.next, Token, list);
            break;
        case LEFTPARENTHESE:
            token = list_entry(token -> list.next, Token, list);
            Node = expr();
            
            if (Node == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "expression");
                //}
            }
            
            if (!match(RIGHTPARENTHESE)) {
                if (prelooking)
                    return NULL;
                else {
                    throwSyntaxError(parsingFile, "\')\'", list_entry(token -> list.prev, Token, list));
                }
            }
            break;
        default:
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "literal, identifier or \'(\'");
            //}
            break;
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
                throwSyntaxError(parsingFile, "\'(\'", list_entry(token -> list.prev, Token, list));
            }
        }
        
        ptrs[0] = type();
        
        if (ptrs[0] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "data type");
            //}
        }
        
        if (!match(RIGHTPARENTHESE)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\')\'", list_entry(token -> list.prev, Token, list));
            }
        }
        
        ptrs[1] = term();
        
        if (ptrs[1] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "term");
            //}
        }
        
        Node = NodeConstructor(Cast, parsingFile, token -> beginLine, NULL, ptrs);
    }
    else {
        prelooking--;
        token = lookahead;
        
        Node = unary();
        
        if (Node == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "unary operation");
            //}
        }
    }
    
    return Node;
}

ASTNode * opassign_op(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    switch (token -> kind) {
        case SUMASSIGN:
            token = list_entry(token -> list.next, Token, list);
            Node = NodeConstructor(Operator, parsingFile, token -> beginLine, "+", ptrs);
            break;
        case SUBASSIGN:
            token = list_entry(token -> list.next, Token, list);
            Node = NodeConstructor(Operator, parsingFile, token -> beginLine, "-", ptrs);
            break;
        case MULASSIGN:
            token = list_entry(token -> list.next, Token, list);
            Node = NodeConstructor(Operator, parsingFile, token -> beginLine, "*", ptrs);
            break;
        case DIVASSIGN:
            token = list_entry(token -> list.next, Token, list);
            Node = NodeConstructor(Operator, parsingFile, token -> beginLine, "/", ptrs);
            break;
        case RESASSIGN:
            token = list_entry(token -> list.next, Token, list);
            Node = NodeConstructor(Operator, parsingFile, token -> beginLine, "%", ptrs);
            break;
        case ANDASSIGN:
            token = list_entry(token -> list.next, Token, list);
            Node = NodeConstructor(Operator, parsingFile, token -> beginLine, "&", ptrs);
            break;
        case ORASSIGN:
            token = list_entry(token -> list.next, Token, list);
            Node = NodeConstructor(Operator, parsingFile, token -> beginLine, "|", ptrs);
            break;
        case XORASSIGN:
            token = list_entry(token -> list.next, Token, list);
            Node = NodeConstructor(Operator, parsingFile, token -> beginLine, "^", ptrs);
            break;
        case LSHASSIGN:
            token = list_entry(token -> list.next, Token, list);
            Node = NodeConstructor(Operator, parsingFile, token -> beginLine, "<<", ptrs);
            break;
        case RSHASSIGN:
            token = list_entry(token -> list.next, Token, list);
            Node = NodeConstructor(Operator, parsingFile, token -> beginLine, ">>", ptrs);
            break;
        default:
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "assign operator");
            //}
            break;
    }
    
    return Node;
}

ASTNode * expr10(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    Node = expr9();
    
    if (Node == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "expression");
        //}
    }

    //可选           ["?" expr() ":" expr10()]
    if (match(QUESTION)) {
        ptrs[0] = Node;
        ptrs[1] = expr();
        
        if (ptrs[1] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "expression");
            //}
        }
        
        if (!match(COLON)) {
            if (prelooking)
                return NULL;
            else {
                throwSyntaxError(parsingFile, "\':\'", list_entry(token -> list.prev, Token, list));
            }
        }
        
        ptrs[2] = expr10();
        
        if (ptrs[2] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "expression");
            //}
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
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "expression");
        //}
    }
    
    //("||" expr8())*
    do {
        if (match(LOGICOR)) {
            ptrs[0] = Node;
            ptrs[1] = expr8();
            
            if (ptrs[1] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "expression");
                //}
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
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "expression");
        //}
    }
    
    //("&&" expr7())*
    do {
        if (match(LOGICAND)) {
            ptrs[0] = Node;
            ptrs[1] = expr7();
            
            if (ptrs[1] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "expression");
                //}
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
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "expression");
        //}
    }
    
    do {  //(">" expr6() | "<" expr6() | ">=" expr6() | "<=" expr6() | "==" expr6() | "!=" expr6())*
        switch (token -> kind) {
            case GREATERTHAN:
                token = list_entry(token -> list.next, Token, list);
                ptrs[0] = Node;
                ptrs[1] = expr6();
                
                if (ptrs[1] == NULL) {
                    //if (prelooking)
                    return NULL;
                    //else {
                        //throwSyntaxError(parsingFile, "expression");
                    //}
                }
                
                Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, ">", ptrs);
                break;
            case LESSTHAN:
                token = list_entry(token -> list.next, Token, list);
                ptrs[0] = Node;
                ptrs[1] = expr6();
                
                if (ptrs[1] == NULL) {
                    //if (prelooking)
                    return NULL;
                    //else {
                        //throwSyntaxError(parsingFile, "expression");
                    //}
                }
                
                Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, "<", ptrs);
                break;
            case GREATERANDEQUAL:
                token = list_entry(token -> list.next, Token, list);
                ptrs[0] = Node;
                ptrs[1] = expr6();
                
                if (ptrs[1] == NULL) {
                    //if (prelooking)
                    return NULL;
                    //else {
                        //throwSyntaxError(parsingFile, "expression");
                    //}
                }
                
                Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, ">=", ptrs);
                break;
            case LESSANDEQUAL:
                token = list_entry(token -> list.next, Token, list);
                ptrs[0] = Node;
                ptrs[1] = expr6();
                
                if (ptrs[1] == NULL) {
                    //if (prelooking)
                    return NULL;
                    //else {
                        //throwSyntaxError(parsingFile, "expression");
                    //}
                }
                
                Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, "<=", ptrs);
                break;
            case EQUAL:
                token = list_entry(token -> list.next, Token, list);
                ptrs[0] = Node;
                ptrs[1] = expr6();
                
                if (ptrs[1] == NULL) {
                    //if (prelooking)
                    return NULL;
                    //else {
                        //throwSyntaxError(parsingFile, "expression");
                    //}
                }
                
                Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, "==", ptrs);
                break;
            case NOTEQUAL:
                token = list_entry(token -> list.next, Token, list);
                ptrs[0] = Node;
                ptrs[1] = expr6();
                
                if (ptrs[1] == NULL) {
                    //if (prelooking)
                    return NULL;
                    //else {
                        //throwSyntaxError(parsingFile, "expression");
                    //}
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
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "expression");
        //}
    }
    
    do {        //("|" expr5())*
        if (match(OR)) {
            ptrs[0] = Node;
            ptrs[1] = expr5();
            
            if (ptrs[1] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "expression");
                //}
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
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "expression");
        //}
    }
    
    do {        //("^" expr4())*
        if (match(XOR)) {
            ptrs[0] = Node;
            ptrs[1] = expr4();
            
            if (ptrs[1] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "expression");
                //}
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
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "expression");
        //}
    }
    
    do {        //("&" expr3())*
        if (match(AND)) {
            ptrs[0] = Node;
            ptrs[1] = expr3();
            
            if (ptrs[1] == NULL) {
                //if (prelooking)
                    return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "expression");
                //}
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
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "expression");
        //}
    }
    
    do {       //(">>" expr2() | "<<" expr2())*
        if (match(RSH)) {
            ptrs[0] = Node;
            ptrs[1] = expr2();
            
            if (ptrs[1] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "expression");
                //}
            }
            
            Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, ">>", ptrs);
        }
        else if (match(LSH)) {
            ptrs[0] = Node;
            ptrs[1] = expr2();
            
            if (ptrs[1] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "expression");
                //}
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
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "expression");
        //}
    }
    
    do {       //("+" expr1() | "-" expr1())*
        if (match(SUM)) {
            ptrs[0] = Node;
            ptrs[1] = expr1();
            
            if (ptrs[1] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "expression");
                //}
            }
            
            Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, "+", ptrs);
        }
        else if (match(SUB)) {
            ptrs[0] = Node;
            ptrs[1] = expr1();
            
            if (ptrs[1] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "expression");
                //}
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
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "term");
        //}
    }
    
    do {       //("*" term() | "/" term() | "%" term())*
        if (match(MUL)) {
            ptrs[0] = Node;
            ptrs[1] = term();
            
            if (ptrs[1] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "term");
                //}
            }
            
            Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, "*", ptrs);
        }
        else if (match(DIV)) {
            ptrs[0] = Node;
            ptrs[1] = term();
            
            if (ptrs[1] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "term");
                //}
            }
            
            Node = NodeConstructor(BinaryOp, parsingFile, token -> beginLine, "/", ptrs);
        }
        else if (match(RES)){
            ptrs[0] = Node;
            ptrs[1] = term();
            
            if (ptrs[1] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "term");
                //}
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
    Token * lookahead = NULL;
    
    switch (token -> kind) {
        case SELFSUM:
            token = list_entry(token -> list.next, Token, list);
            ptrs[0] = unary();
            
            if (ptrs[0] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "unary operation");
                //}
            }
            
            Node = NodeConstructor(PrefixOp, parsingFile, token -> beginLine, "++", ptrs);
            break;
        case SELFSUB:
            token = list_entry(token -> list.next, Token, list);
            ptrs[0] = unary();
            
            if (ptrs[0] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "unary operation");
                //}
            }
            
            Node = NodeConstructor(PrefixOp, parsingFile, token -> beginLine, "--", ptrs);
            break;
        case LOGICNOT:
            token = list_entry(token -> list.next, Token, list);
            ptrs[0] = term();
            
            if (ptrs[0] == NULL) {
                //if (prelooking)
                    return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "term");
                //}
            }
            
            Node = NodeConstructor(UnaryOp, parsingFile, token -> beginLine, "!", ptrs);
            break;
        case NOT:
            token = list_entry(token -> list.next, Token, list);
            ptrs[0] = term();
            
            if (ptrs[0] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "term");
                //}
            }
            
            Node = NodeConstructor(UnaryOp, parsingFile, token -> beginLine, "~", ptrs);
            break;
        case MUL:
            token = list_entry(token -> list.next, Token, list);
            ptrs[0] = term();
            
            if (ptrs[0] == NULL) {
                //if (prelooking)
                    return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "term");
                //}
            }
            
            Node = NodeConstructor(Dereference, parsingFile, token -> beginLine, NULL, ptrs);
            break;
        case AND:
            token = list_entry(token -> list.next, Token, list);
            ptrs[0] = term();
            
            if (ptrs[0] == NULL) {
                //if (prelooking)
                    return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "term");
                //}
            }
            
            Node = NodeConstructor(Address, parsingFile, token -> beginLine, NULL, ptrs);
            break;
        case SIZEOF:
            token = list_entry(token -> list.next, Token, list);
            lookahead = token;
            
            prelooking++;
            if (match(LEFTPARENTHESE) && type() != NULL) {
                prelooking--;
                token = lookahead;
                
                if (!match(LEFTPARENTHESE)) {
                    if (prelooking)
                        return NULL;
                    else {
                        throwSyntaxError(parsingFile, "\'(\'", list_entry(token -> list.prev, Token, list));
                    }
                }
                
                ptrs[0] = type();
                
                if (ptrs[0] == NULL) {
                    //if (prelooking)
                    return NULL;
                    //else {
                        //throwSyntaxError(parsingFile, "data type");
                    //}
                }
                
                if (!match(RIGHTPARENTHESE)) {
                    if (prelooking)
                        return NULL;
                    else {
                        throwSyntaxError(parsingFile, "\')\'", list_entry(token -> list.prev, Token, list));
                    }
                }
                
                Node = NodeConstructor(SizeofType, parsingFile, token -> beginLine, NULL, ptrs);
            }
            else {
                prelooking--;
                token = lookahead;
                
                ptrs[0] = unary();
                
                if (ptrs[0] == NULL) {
                    //if (prelooking)
                    return NULL;
                    //else {
                        //throwSyntaxError(parsingFile, "unary operation");
                    //}
                }
                
                Node = NodeConstructor(SizeofExpr, parsingFile, token -> beginLine, NULL, ptrs);
            }
            break;
        default:
            Node = postfix();
            
            if (Node == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "postfix operation");
                //}
            }
    }
    
    return Node;
}

ASTNode * postfix(void) {
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    Node = primary();
    
    if (Node == NULL) {
        //if (prelooking)
        return NULL;
        //else {
            //throwSyntaxError(parsingFile, "literal, identifier or \'(\'");
        //}
    }
    
    do {    //("++"|"--"|"["expr()"]"|"."name()|"->"name()|"("args()")")*
        switch (token -> kind) {
            case SELFSUM:
                token = list_entry(token -> list.next, Token, list);
                
                ptrs[0] = Node;
                ptrs[1] = NULL;
                
                Node = NodeConstructor(SuffixOp, parsingFile, token -> beginLine, "++", ptrs);
                break;
            case SELFSUB:
                token = list_entry(token -> list.next, Token, list);
                
                ptrs[0] = Node;
                ptrs[1] = NULL;
                
                Node = NodeConstructor(SuffixOp, parsingFile, token -> beginLine, "--", ptrs);
                break;
            case LEFTBRACKET:
                token = list_entry(token -> list.next, Token, list);
                
                ptrs[0] = Node;
                ptrs[1] = expr();
                
                if (ptrs[1] == NULL) {
                    //if (prelooking)
                    return NULL;
                    //else {
                        //throwSyntaxError(parsingFile, "expression");
                    //}
                }
                
                Node = NodeConstructor(ArrayRef, parsingFile, token -> beginLine, NULL, ptrs);
                
                if (!match(RIGHTBRACKET)) {
                    if (prelooking)
                        return NULL;
                    else {
                        throwSyntaxError(parsingFile, "\']\'", list_entry(token -> list.prev, Token, list));
                    }
                }
                break;
            case DOT:
                token = list_entry(token -> list.next, Token, list);
                
                ptrs[0] = Node;
                ptrs[1] = name();
                
                if (ptrs[1] == NULL) {
                    //if (prelooking)
                    return NULL;
                    //else {
                        //throwSyntaxError(parsingFile, "identifier");
                    //}
                }
                
                Node = NodeConstructor(Member, parsingFile, token -> beginLine, NULL, ptrs);
                break;
            case ARROW:
                token = list_entry(token -> list.next, Token, list);
                
                ptrs[0] = Node;
                ptrs[1] = name();
                
                if (ptrs[1] == NULL) {
                    //if (prelooking)
                    return NULL;
                    //else {
                        //throwSyntaxError(parsingFile, "identifier");
                    //}
                }
                
                Node = NodeConstructor(PtrMember, parsingFile, token -> beginLine, NULL, ptrs);
                break;
            case LEFTPARENTHESE:
                token = list_entry(token -> list.next, Token, list);
                
                ptrs[0] = Node;
                
                if (token -> kind != RIGHTPARENTHESE)
                    ptrs[1] = args();
                
                Node = NodeConstructor(Funcall, parsingFile, token -> beginLine, NULL, ptrs);
                
                if (!match(RIGHTPARENTHESE)) {
                    if (prelooking)
                        return NULL;
                    else {
                        throwSyntaxError(parsingFile, "\')\'", list_entry(token -> list.prev, Token, list));
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
    ASTNode * Node = NULL;
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    
    Node = NodeConstructor(Args, parsingFile, token -> beginLine, NULL, ptrs);
    
    ptrs[0] = expr();         //可选  [expr() ("," expr())*]
    
    if (ptrs[0] != NULL) {
        Node -> append(Node, *ptrs[0]);
    }
    
    if (ptrs[0] != NULL) {
        do {
            free(ptrs[0]);
            if (match(COMMA)) {
                ptrs[0] = expr();
                
                if (ptrs[0] == NULL) {
                    //if (prelooking)
                    return NULL;
                    //else {
                        //throwSyntaxError(parsingFile, "expression");
                    //}
                }
                
                Node -> append(Node, *ptrs[0]);
            }
            else
                goto jumpout;
        } while (1);
    }
    else
        return NULL;
jumpout:
    
    return Node;
}

ASTNode * extern_(void) {
    ASTNode * ptrs[6] = {NULL, NULL, NULL, NULL, NULL, NULL};
    Token * lookahead = NULL;
    
    if (!match(EXTERN)) {
        if (prelooking)
            return NULL;
        else {
            throwSyntaxError(parsingFile, "\'extern\'", list_entry(token -> list.prev, Token, list));
        }
    }
    
    lookahead = token;
    
    prelooking++;
    storage();
    if (type() != NULL && match(IDENTIFIER) && match(LEFTPARENTHESE)) {
        prelooking--;
        token = lookahead;
        ptrs[0] = defun();
        
        if (ptrs[0] == NULL) {
            //if (prelooking)
            return NULL;
            //else {
                //throwSyntaxError(parsingFile, "function definition");
            //}
        }
        
        return NodeConstructor(ExternFunc, parsingFile, token -> beginLine, NULL, ptrs);
    }
    else {
        prelooking--;
        token = lookahead;
        
        if (token -> kind != CONST && (list_entry((list_entry(token -> list.next, Token, list)) -> list.next, Token, list)) -> kind != LEFTBRACE) {
            ptrs[0] = defvars();
            
            if (ptrs[0] == NULL) {
                //if (prelooking)
                return NULL;
                //else {
                    //throwSyntaxError(parsingFile, "variable definition");
                //}
            }
            
            return NodeConstructor(ExternVar, parsingFile, token -> beginLine, NULL, ptrs);
        }
        else {
            switch (token -> kind) {
                case CONST:
                    ptrs[0] = defconst();
                    
                    if (ptrs[0] == NULL) {
                        //if (prelooking)
                        return NULL;
                        //else {
                            //throwSyntaxError(parsingFile, "constant definition");
                        //}
                    }
                    
                    return NodeConstructor(ExternConst, parsingFile, token -> beginLine, NULL, ptrs);
                    break;
                default:
                    return NULL;
            }
        }
    }
}

ASTNode * getType(char * key) {
    return typeDefList -> get(typeDefList, key);
}

int match(int kind) {
    if (token == NULL)
        return 0;
    
    if (token -> kind == kind) {
        token = list_entry(token -> list.next, Token, list);
        return 1;
    }
    else
        return 0;
}

void throwSyntaxError(char * file, char * expected, Token * token) {
    error_t * new_err = ErrorConstructor(token -> beginLine, SYNTAX, file, expected, token);
    list_add_tail(&new_err -> list, &err_list -> list);
    //printf("Melon: %s: syntax \033[31merror\033[0m in line %d expected %s\n", file, line, expected);
    //exit(-1);
}
