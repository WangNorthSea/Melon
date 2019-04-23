//
//  Lexer.c
//  Melon
//
//  Created by 王浩宇 on 2019/3/16.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "token.h"
#include "tokenkind.h"

void tokenInit(Token * token) {
    token -> image = NULL;
    token -> next = NULL;
    token -> specialToken = NULL;
}

void enlargeBuffer(char * buffer, int * bufferSize) {
    buffer = (char *)realloc(buffer, sizeof(char) * (*bufferSize + 1024));
    *bufferSize += 1024;
}

//注: headToken为空Token，第一个有用的Token地址为headToken -> next
Token * lexicalAnalyze(FILE * fp) {
    char ch = fgetc(fp);
    char * buffer = (char *)malloc(sizeof(char) * 1024);
    int bufferSize = 1024;
    int bufferIndex = 0;
    int line = 1;
    int syntaxError = 1;
    Token * headToken = (Token *)malloc(sizeof(Token));
    Token * tailToken = headToken;
    Token * tailSpecialToken = headToken;
    
    tokenInit(headToken);
    
    while (1) {
        syntaxError = 1;
        
        if (ch == EOF)
            break;
        
        //识别空白符，正则表达式: ([" ", "\t", "\n", "\r", "\f"])+
        if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '\f') {
            syntaxError = 0;
            tailSpecialToken -> specialToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailSpecialToken -> specialToken);
            tailSpecialToken -> specialToken -> kind = SPACES;
            tailSpecialToken -> specialToken -> beginLine = line;
            
            do {
                if (ch == '\n')
                    line++;
                
                if (bufferIndex == bufferSize - 1)
                    enlargeBuffer(buffer, &bufferSize);
                
                buffer[bufferIndex] = ch;
                bufferIndex++;
                ch = fgetc(fp);
            } while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '\f');
            buffer[bufferIndex] = '\0';
            
            tailSpecialToken -> specialToken -> endLine = line;
            tailSpecialToken -> specialToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailSpecialToken -> specialToken -> image[i] = buffer[i];
            
            bufferIndex = 0;
            tailSpecialToken = tailSpecialToken -> specialToken;
        }
        
        //识别标识符，正则表达式: ["a" - "z", "A" - "Z", "_"](["a" - "z", "A" - "Z", "_", "0" - "9"])*
        if (ch == '_' || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            
            do {
                if (bufferIndex == bufferSize - 1)
                    enlargeBuffer(buffer, &bufferSize);
                
                buffer[bufferIndex] = ch;
                bufferIndex++;
                ch = fgetc(fp);
            } while (ch == '_' || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'));
            buffer[bufferIndex] = '\0';
            
            tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> next -> image[i] = buffer[i];
            
            //识别保留字
            if (bufferIndex < 9) {
                if (!strcmp(buffer, "void"))
                    tailToken -> next -> kind = VOID;
                else if (!strcmp(buffer, "char"))
                    tailToken -> next -> kind = CHAR;
                else if (!strcmp(buffer, "short"))
                    tailToken -> next -> kind = SHORT;
                else if (!strcmp(buffer, "int"))
                    tailToken -> next -> kind = INT;
                else if (!strcmp(buffer, "long"))
                    tailToken -> next -> kind = LONG;
                else if (!strcmp(buffer, "struct"))
                    tailToken -> next -> kind = STRUCT;
                else if (!strcmp(buffer, "union"))
                    tailToken -> next -> kind = UNION;
                else if (!strcmp(buffer, "enum"))
                    tailToken -> next -> kind = ENUM;
                else if (!strcmp(buffer, "static"))
                    tailToken -> next -> kind = STATIC;
                else if (!strcmp(buffer, "extern"))
                    tailToken -> next -> kind = EXTERN;
                else if (!strcmp(buffer, "const"))
                    tailToken -> next -> kind = CONST;
                else if (!strcmp(buffer, "signed"))
                    tailToken -> next -> kind = SIGNED;
                else if (!strcmp(buffer, "unsigned"))
                    tailToken -> next -> kind = UNSIGNED;
                else if (!strcmp(buffer, "if"))
                    tailToken -> next -> kind = IF;
                else if (!strcmp(buffer, "else"))
                    tailToken -> next -> kind = ELSE;
                else if (!strcmp(buffer, "switch"))
                    tailToken -> next -> kind = SWITCH;
                else if (!strcmp(buffer, "case"))
                    tailToken -> next -> kind = CASE;
                else if (!strcmp(buffer, "default"))
                    tailToken -> next -> kind = DEFAULT;
                else if (!strcmp(buffer, "while"))
                    tailToken -> next -> kind = WHILE;
                else if (!strcmp(buffer, "do"))
                    tailToken -> next -> kind = DO;
                else if (!strcmp(buffer, "for"))
                    tailToken -> next -> kind = FOR;
                else if (!strcmp(buffer, "return"))
                    tailToken -> next -> kind = RETURN;
                else if (!strcmp(buffer, "break"))
                    tailToken -> next -> kind = BREAK;
                else if (!strcmp(buffer, "continue"))
                    tailToken -> next -> kind = CONTINUE;
                else if (!strcmp(buffer, "goto"))
                    tailToken -> next -> kind = GOTO;
                else if (!strcmp(buffer, "typedef"))
                    tailToken -> next -> kind = TYPEDEF;
                else if (!strcmp(buffer, "import"))
                    tailToken -> next -> kind = IMPORT;
                else if (!strcmp(buffer, "sizeof"))
                    tailToken -> next -> kind = SIZEOF;
                else if (!strcmp(buffer, "float"))
                    tailToken -> next -> kind = FLOAT;
                else if (!strcmp(buffer, "double"))
                    tailToken -> next -> kind = DOUBLE;
                else
                    tailToken -> next -> kind = IDENTIFIER;
            }
            else
                tailToken -> next -> kind = IDENTIFIER;
            
            bufferIndex = 0;
            tailToken = tailToken -> next;
            tailSpecialToken = tailToken;
        }
        
        //识别字符字面量，正则表达式: "'"[~("'", "\n")]"'"
        if (ch == '\'') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            tailToken -> next -> kind = CHARACTER;
            
            do {
                if (bufferIndex == bufferSize - 1)
                    enlargeBuffer(buffer, &bufferSize);
                
                buffer[bufferIndex] = ch;
                bufferIndex++;
                
                ch = fgetc(fp);
                
                if (ch == '\\') {
                    if (bufferIndex == bufferSize - 1)
                        enlargeBuffer(buffer, &bufferSize);
                    
                    buffer[bufferIndex] = ch;
                    bufferIndex++;
                    
                    ch = fgetc(fp);
                    
                    if (bufferIndex == bufferSize - 1)
                        enlargeBuffer(buffer, &bufferSize);
                    
                    buffer[bufferIndex] = ch;
                    bufferIndex++;
                    
                    ch = fgetc(fp);
                }
            } while (ch != '\'' && ch != '\n' && ch != EOF);
            
            if (ch == '\n' || ch == EOF) {
                printf("Melon: lexical \033[31merror\033[0m in line %d\n", line);
                exit(-1);
            }
            
            if (bufferIndex == bufferSize - 1)
                enlargeBuffer(buffer, &bufferSize);
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> next -> image[i] = buffer[i];
            
            bufferIndex = 0;
            tailToken = tailToken -> next;
            tailSpecialToken = tailToken;
        }
        
        //识别字符串字面量，正则表达式: """[~(""", "\n")]"""
        if (ch == '\"') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            tailToken -> next -> kind = STRING;
            
            do {
                if (bufferIndex == bufferSize - 1)
                    enlargeBuffer(buffer, &bufferSize);
                
                buffer[bufferIndex] = ch;
                bufferIndex++;
                
                ch = fgetc(fp);
                
                if (ch == '\\') {
                    if (bufferIndex == bufferSize - 1)
                        enlargeBuffer(buffer, &bufferSize);
                    
                    buffer[bufferIndex] = ch;
                    bufferIndex++;
                    
                    ch = fgetc(fp);
                    
                    if (bufferIndex == bufferSize - 1)
                        enlargeBuffer(buffer, &bufferSize);
                    
                    buffer[bufferIndex] = ch;
                    bufferIndex++;
                    
                    ch = fgetc(fp);
                }
            } while (ch != '\"' && ch != '\n' && ch != EOF);
            
            if (ch == '\n' || ch == EOF) {
                printf("Melon: lexical \033[31merror\033[0m in line %d\n", line);
                exit(-1);
            }
            
            if (bufferIndex == bufferSize - 1)
                enlargeBuffer(buffer, &bufferSize);
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> next -> image[i] = buffer[i];
            
            bufferIndex = 0;
            tailToken = tailToken -> next;
            tailSpecialToken = tailToken;
        }
        
        //识别逗号，正则表达式: ","
        if (ch == ',') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            tailToken -> next -> kind = COMMA;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> next -> image[i] = buffer[i];
            
            bufferIndex = 0;
            tailToken = tailToken -> next;
            tailSpecialToken = tailToken;
        }
        
        //识别分号，正则表达式: ";"
        if (ch == ';') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            tailToken -> next -> kind = SEMICOLON;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> next -> image[i] = buffer[i];
            
            bufferIndex = 0;
            tailToken = tailToken -> next;
            tailSpecialToken = tailToken;
        }
        
        //识别句号，正则表达式: "."
        if (ch == '.') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            tailToken -> next -> kind = DOT;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> next -> image[i] = buffer[i];
            
            bufferIndex = 0;
            tailToken = tailToken -> next;
            tailSpecialToken = tailToken;
        }
        
        //识别冒号，正则表达式: ":"
        if (ch == ':') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            tailToken -> next -> kind = COLON;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> next -> image[i] = buffer[i];
            
            bufferIndex = 0;
            tailToken = tailToken -> next;
            tailSpecialToken = tailToken;
        }
        
        //识别问号，正则表达式: "?"
        if (ch == '?') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            tailToken -> next -> kind = QUESTION;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> next -> image[i] = buffer[i];
            
            bufferIndex = 0;
            tailToken = tailToken -> next;
            tailSpecialToken = tailToken;
        }
        
        //识别左小括号，正则表达式: "("
        if (ch == '(') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            tailToken -> next -> kind = LEFTPARENTHESE;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> next -> image[i] = buffer[i];
            
            bufferIndex = 0;
            tailToken = tailToken -> next;
            tailSpecialToken = tailToken;
        }
        
        //识别右小括号，正则表达式: ")"
        if (ch == ')') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            tailToken -> next -> kind = RIGHTPARENTHESE;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> next -> image[i] = buffer[i];
            
            bufferIndex = 0;
            tailToken = tailToken -> next;
            tailSpecialToken = tailToken;
        }
        
        //识别左中括号，正则表达式: "["
        if (ch == '[') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            tailToken -> next -> kind = LEFTBRACKET;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> next -> image[i] = buffer[i];
            
            bufferIndex = 0;
            tailToken = tailToken -> next;
            tailSpecialToken = tailToken;
        }
        
        //识别右中括号，正则表达式: "]"
        if (ch == ']') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            tailToken -> next -> kind = RIGHTBRACKET;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> next -> image[i] = buffer[i];
            
            bufferIndex = 0;
            tailToken = tailToken -> next;
            tailSpecialToken = tailToken;
        }
        
        //识别左大括号，正则表达式: "{"
        if (ch == '{') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            tailToken -> next -> kind = LEFTBRACE;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> next -> image[i] = buffer[i];
            
            bufferIndex = 0;
            tailToken = tailToken -> next;
            tailSpecialToken = tailToken;
        }
        
        //识别右大括号，正则表达式: "}"
        if (ch == '}') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            tailToken -> next -> kind = RIGHTBRACE;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> next -> image[i] = buffer[i];
            
            bufferIndex = 0;
            tailToken = tailToken -> next;
            tailSpecialToken = tailToken;
        }
        
        //识别赋值与等于，正则表达式: "="U"=="
        if (ch == '=') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            if (ch == '=') {
                tailToken -> next -> kind = EQUAL;
                buffer[bufferIndex] = ch;
                bufferIndex++;
                ch = fgetc(fp);
            }
            else
                tailToken -> next -> kind = ASSIGN;
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> next -> image[i] = buffer[i];
            
            bufferIndex = 0;
            tailToken = tailToken -> next;
            tailSpecialToken = tailToken;
        }
        
        //识别/, /=以及行注释与块注释，正则表达式: "//"~("\n")* U "/*"~("*/")*"*/" 不规范，大体是这个意思
        if (ch == '/') {
            syntaxError = 0;
            tailSpecialToken -> specialToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailSpecialToken -> specialToken);
            tailSpecialToken -> specialToken -> beginLine = line;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            if (ch == '/') {
                tailSpecialToken -> specialToken -> endLine = line;
                tailSpecialToken -> specialToken -> kind = LINE_COMMENT;
                do {
                    if (bufferIndex == bufferSize - 1)
                        enlargeBuffer(buffer, &bufferSize);
                    
                    buffer[bufferIndex] = ch;
                    bufferIndex++;
                    ch = fgetc(fp);
                } while (ch != '\n' && ch != EOF);
                buffer[bufferIndex] = '\0';
                
                tailSpecialToken -> specialToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailSpecialToken -> specialToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailSpecialToken = tailSpecialToken -> specialToken;
            }
            else if (ch == '=') {
                free(tailSpecialToken -> specialToken);
                tailSpecialToken -> specialToken = NULL;
                tailToken -> next = (Token *)malloc(sizeof(Token));
                tokenInit(tailToken -> next);
                tailToken -> next -> beginLine = line;
                tailToken -> next -> endLine = line;
                tailToken -> next -> kind = DIVASSIGN;
                
                buffer[bufferIndex] = '=';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
            else if (ch == '*') {
                tailSpecialToken -> specialToken -> kind = BLOCK_COMMENT;
            restart:
                do {
                    if (ch == '\n')
                        line++;
                    
                    if (bufferIndex == bufferSize - 1)
                        enlargeBuffer(buffer, &bufferSize);
                    
                    buffer[bufferIndex] = ch;
                    bufferIndex++;
                    ch = fgetc(fp);
                } while (ch != '*' && ch != EOF);
                
                if (ch == '*') {
                    FILE * temp = fp;
                    ch = fgetc(temp);
                    if (ch != '/') {
                        ch = '*';
                        goto restart;
                    }
                    else {
                        fp = temp;
                        if (bufferIndex == bufferSize - 1)
                            enlargeBuffer(buffer, &bufferSize);
                        
                        buffer[bufferIndex] = '*';
                        bufferIndex++;
                        
                        if (bufferIndex == bufferSize - 1)
                            enlargeBuffer(buffer, &bufferSize);
                        
                        buffer[bufferIndex] = ch;
                        bufferIndex++;
                        ch = fgetc(fp);
                        
                        buffer[bufferIndex] = '\0';
                        
                        tailSpecialToken -> specialToken -> endLine = line;
                        tailSpecialToken -> specialToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                        
                        for (int i = 0; i <= bufferIndex; i++)
                            tailSpecialToken -> specialToken -> image[i] = buffer[i];
                        
                        bufferIndex = 0;
                        tailSpecialToken = tailSpecialToken -> specialToken;
                    }
                }
                else
                    bufferIndex = 0;
            }
            else {
                free(tailSpecialToken -> specialToken);
                tailSpecialToken -> specialToken = NULL;
                tailToken -> next = (Token *)malloc(sizeof(Token));
                tokenInit(tailToken -> next);
                tailToken -> next -> beginLine = line;
                tailToken -> next -> endLine = line;
                tailToken -> next -> kind = DIV;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
        }
        
        //识别+, ++, +=以及以+开头的整数、浮点数字面量
        if (ch == '+') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            
            ch = fgetc(fp);
            
            if (ch == '+') {
                tailToken -> next -> kind = SELFSUM;
                
                buffer[bufferIndex] = '+';
                bufferIndex++;
                
                buffer[bufferIndex] = '+';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
            else if (ch == '=') {
                tailToken -> next -> kind = SUMASSIGN;
                
                buffer[bufferIndex] = '+';
                bufferIndex++;
                
                buffer[bufferIndex] = '=';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
            else if (ch >= '0' && ch <= '9') {
                buffer[bufferIndex] = '+';
                bufferIndex++;
                
                do {
                    if (bufferIndex == bufferSize - 1)
                        enlargeBuffer(buffer, &bufferSize);
                    
                    buffer[bufferIndex] = ch;
                    bufferIndex++;
                    ch = fgetc(fp);
                } while (ch >= '0' && ch <= '9');
                
                if (ch == '.') {
                    tailToken -> next -> kind = FLOAT_;
                    
                    do {
                        if (bufferIndex == bufferSize - 1)
                            enlargeBuffer(buffer, &bufferSize);
                        
                        buffer[bufferIndex] = ch;
                        bufferIndex++;
                        ch = fgetc(fp);
                    } while (ch >= '0' && ch <= '9');
                    buffer[bufferIndex] = '\0';
                    
                    tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                    
                    for (int i = 0; i <= bufferIndex; i++)
                        tailToken -> next -> image[i] = buffer[i];
                    
                    bufferIndex = 0;
                    tailToken = tailToken -> next;
                    tailSpecialToken = tailToken;
                }
                else {
                    tailToken -> next -> kind = INTEGER;
                    
                    buffer[bufferIndex] = '\0';
                    
                    tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                    
                    for (int i = 0; i <= bufferIndex; i++)
                        tailToken -> next -> image[i] = buffer[i];
                    
                    bufferIndex = 0;
                    tailToken = tailToken -> next;
                    tailSpecialToken = tailToken;
                }
            }
            else {
                tailToken -> next -> kind = SUM;
                
                buffer[bufferIndex] = '+';
                bufferIndex++;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
        }
        
        //识别-, --, -=, ->以及以-开头的整数、浮点数字面量
        if (ch == '-') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            
            ch = fgetc(fp);
            
            if (ch == '-') {
                tailToken -> next -> kind = SELFSUB;
                
                buffer[bufferIndex] = '-';
                bufferIndex++;
                
                buffer[bufferIndex] = '-';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
            else if (ch == '=') {
                tailToken -> next -> kind = SUBASSIGN;
                
                buffer[bufferIndex] = '-';
                bufferIndex++;
                
                buffer[bufferIndex] = '=';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
            else if (ch == '>') {
                tailToken -> next -> kind = ARROW;
                
                buffer[bufferIndex] = '-';
                bufferIndex++;
                
                buffer[bufferIndex] = '>';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
            else if (ch >= '0' && ch <= '9') {
                buffer[bufferIndex] = '-';
                bufferIndex++;
                
                do {
                    if (bufferIndex == bufferSize - 1)
                        enlargeBuffer(buffer, &bufferSize);
                    
                    buffer[bufferIndex] = ch;
                    bufferIndex++;
                    ch = fgetc(fp);
                } while (ch >= '0' && ch <= '9');
                
                if (ch == '.') {
                    tailToken -> next -> kind = FLOAT_;
                    
                    do {
                        if (bufferIndex == bufferSize - 1)
                            enlargeBuffer(buffer, &bufferSize);
                        
                        buffer[bufferIndex] = ch;
                        bufferIndex++;
                        ch = fgetc(fp);
                    } while (ch >= '0' && ch <= '9');
                    buffer[bufferIndex] = '\0';
                    
                    tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                    
                    for (int i = 0; i <= bufferIndex; i++)
                        tailToken -> next -> image[i] = buffer[i];
                    
                    bufferIndex = 0;
                    tailToken = tailToken -> next;
                    tailSpecialToken = tailToken;
                }
                else {
                    tailToken -> next -> kind = INTEGER;
                    
                    buffer[bufferIndex] = '\0';
                    
                    tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                    
                    for (int i = 0; i <= bufferIndex; i++)
                        tailToken -> next -> image[i] = buffer[i];
                    
                    bufferIndex = 0;
                    tailToken = tailToken -> next;
                    tailSpecialToken = tailToken;
                }
            }
            else {
                tailToken -> next -> kind = SUB;
                
                buffer[bufferIndex] = '-';
                bufferIndex++;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
        }
        
        //识别直接以数字开头的整数、浮点数字面量
        if (ch >= '0' && ch <= '9') {
            int hasX = 0;
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            
            if (ch == '0') {
                ch = fgetc(fp);
                buffer[bufferIndex] = '0';
                bufferIndex++;
                if (ch == 'x' || ch == 'X') {
                    buffer[bufferIndex] = ch;
                    bufferIndex++;
                    hasX = 1;
                    ch = fgetc(fp);
                }
            }
            
            if (hasX) {
                while ((ch >= '0' && ch <= '9') || (ch >= 'a' && ch <= 'f') || (ch >= 'A' && ch <= 'F')) {
                    if (bufferIndex == bufferSize - 1)
                        enlargeBuffer(buffer, &bufferSize);
                    
                    buffer[bufferIndex] = ch;
                    bufferIndex++;
                    ch = fgetc(fp);
                }
            }
            else {
                while (ch >= '0' && ch <= '9') {
                    if (bufferIndex == bufferSize - 1)
                        enlargeBuffer(buffer, &bufferSize);
                    
                    buffer[bufferIndex] = ch;
                    bufferIndex++;
                    ch = fgetc(fp);
                }
            }
            
            if (ch == '.' && !hasX) {
                tailToken -> next -> kind = FLOAT_;
                
                do {
                    if (bufferIndex == bufferSize - 1)
                        enlargeBuffer(buffer, &bufferSize);
                    
                    buffer[bufferIndex] = ch;
                    bufferIndex++;
                    ch = fgetc(fp);
                } while (ch >= '0' && ch <= '9');
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
            else {
                tailToken -> next -> kind = INTEGER;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
        }
        
        //识别*, *=
        if (ch == '*') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            
            ch = fgetc(fp);
            
            if (ch == '=') {
                tailToken -> next -> kind = MULASSIGN;
                
                buffer[bufferIndex] = '*';
                bufferIndex++;
                
                buffer[bufferIndex] = '=';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
            else {
                tailToken -> next -> kind = MUL;
                
                buffer[bufferIndex] = '*';
                bufferIndex++;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
        }
        
        //识别%, %=
        if (ch == '%') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            
            ch = fgetc(fp);
            
            if (ch == '=') {
                tailToken -> next -> kind = RESASSIGN;
                
                buffer[bufferIndex] = '%';
                bufferIndex++;
                
                buffer[bufferIndex] = '=';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
            else {
                tailToken -> next -> kind = RES;
                
                buffer[bufferIndex] = '%';
                bufferIndex++;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
        }
        
        //识别&, &=, &&
        if (ch == '&') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            
            ch = fgetc(fp);
            
            if (ch == '=') {
                tailToken -> next -> kind = ANDASSIGN;
                
                buffer[bufferIndex] = '&';
                bufferIndex++;
                
                buffer[bufferIndex] = '=';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
            else if (ch == '&') {
                tailToken -> next -> kind = LOGICAND;
                
                buffer[bufferIndex] = '&';
                bufferIndex++;
                
                buffer[bufferIndex] = '&';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
            else {
                tailToken -> next -> kind = AND;
                
                buffer[bufferIndex] = '&';
                bufferIndex++;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
        }
        
        //识别|, |=, ||
        if (ch == '|') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            
            ch = fgetc(fp);
            
            if (ch == '=') {
                tailToken -> next -> kind = ORASSIGN;
                
                buffer[bufferIndex] = '|';
                bufferIndex++;
                
                buffer[bufferIndex] = '=';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
            else if (ch == '|') {
                tailToken -> next -> kind = LOGICOR;
                
                buffer[bufferIndex] = '|';
                bufferIndex++;
                
                buffer[bufferIndex] = '|';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
            else {
                tailToken -> next -> kind = OR;
                
                buffer[bufferIndex] = '|';
                bufferIndex++;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
        }
        
        //识别~
        if (ch == '~') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            
            ch = fgetc(fp);
            
            tailToken -> next -> kind = NOT;
            
            buffer[bufferIndex] = '~';
            bufferIndex++;
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> next -> image[i] = buffer[i];
            
            bufferIndex = 0;
            tailToken = tailToken -> next;
            tailSpecialToken = tailToken;
        }
        
        //识别^, ^=
        if (ch == '^') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            
            ch = fgetc(fp);
            
            if (ch == '=') {
                tailToken -> next -> kind = XORASSIGN;
                
                buffer[bufferIndex] = '^';
                bufferIndex++;
                
                buffer[bufferIndex] = '=';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
            else {
                tailToken -> next -> kind = XOR;
                
                buffer[bufferIndex] = '^';
                bufferIndex++;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
        }
        
        //识别!, !=
        if (ch == '!') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            
            ch = fgetc(fp);
            
            if (ch == '=') {
                tailToken -> next -> kind = NOTEQUAL;
                
                buffer[bufferIndex] = '!';
                bufferIndex++;
                
                buffer[bufferIndex] = '=';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
            else {
                tailToken -> next -> kind = LOGICNOT;
                
                buffer[bufferIndex] = '!';
                bufferIndex++;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
        }
        
        //识别<, <=, <<, <<=
        if (ch == '<') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            
            ch = fgetc(fp);
            
            if (ch == '=') {
                tailToken -> next -> kind = LESSANDEQUAL;
                
                buffer[bufferIndex] = '<';
                bufferIndex++;
                
                buffer[bufferIndex] = '=';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
            else if (ch == '<') {
                ch = fgetc(fp);
                
                if (ch == '=') {
                    tailToken -> next -> kind = LSHASSIGN;
                    
                    buffer[bufferIndex] = '<';
                    bufferIndex++;
                    
                    buffer[bufferIndex] = '<';
                    bufferIndex++;
                    
                    buffer[bufferIndex] = '=';
                    bufferIndex++;
                    ch = fgetc(fp);
                    
                    buffer[bufferIndex] = '\0';
                    
                    tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                    
                    for (int i = 0; i <= bufferIndex; i++)
                        tailToken -> next -> image[i] = buffer[i];
                    
                    bufferIndex = 0;
                    tailToken = tailToken -> next;
                    tailSpecialToken = tailToken;
                }
                else {
                    tailToken -> next -> kind = LSH;
                    
                    buffer[bufferIndex] = '<';
                    bufferIndex++;
                    
                    buffer[bufferIndex] = '<';
                    bufferIndex++;
                    
                    buffer[bufferIndex] = '\0';
                    
                    tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                    
                    for (int i = 0; i <= bufferIndex; i++)
                        tailToken -> next -> image[i] = buffer[i];
                    
                    bufferIndex = 0;
                    tailToken = tailToken -> next;
                    tailSpecialToken = tailToken;
                }
            }
            else {
                tailToken -> next -> kind = LESSTHAN;
                
                buffer[bufferIndex] = '<';
                bufferIndex++;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
        }
        
        //识别>, >=, >>, >>=
        if (ch == '>') {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            
            ch = fgetc(fp);
            
            if (ch == '=') {
                tailToken -> next -> kind = GREATERANDEQUAL;
                
                buffer[bufferIndex] = '>';
                bufferIndex++;
                
                buffer[bufferIndex] = '=';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
            else if (ch == '>') {
                ch = fgetc(fp);
                
                if (ch == '=') {
                    tailToken -> next -> kind = RSHASSIGN;
                    
                    buffer[bufferIndex] = '>';
                    bufferIndex++;
                    
                    buffer[bufferIndex] = '>';
                    bufferIndex++;
                    
                    buffer[bufferIndex] = '=';
                    bufferIndex++;
                    ch = fgetc(fp);
                    
                    buffer[bufferIndex] = '\0';
                    
                    tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                    
                    for (int i = 0; i <= bufferIndex; i++)
                        tailToken -> next -> image[i] = buffer[i];
                    
                    bufferIndex = 0;
                    tailToken = tailToken -> next;
                    tailSpecialToken = tailToken;
                }
                else {
                    tailToken -> next -> kind = RSH;
                    
                    buffer[bufferIndex] = '>';
                    bufferIndex++;
                    
                    buffer[bufferIndex] = '>';
                    bufferIndex++;
                    
                    buffer[bufferIndex] = '\0';
                    
                    tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                    
                    for (int i = 0; i <= bufferIndex; i++)
                        tailToken -> next -> image[i] = buffer[i];
                    
                    bufferIndex = 0;
                    tailToken = tailToken -> next;
                    tailSpecialToken = tailToken;
                }
            }
            else {
                tailToken -> next -> kind = GREATERTHAN;
                
                buffer[bufferIndex] = '>';
                bufferIndex++;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> next -> image[i] = buffer[i];
                
                bufferIndex = 0;
                tailToken = tailToken -> next;
                tailSpecialToken = tailToken;
            }
        }
        
        //识别文件结尾，正则表达式: "EOF"
        if (ch == EOF) {
            syntaxError = 0;
            tailToken -> next = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken -> next);
            tailToken -> next -> beginLine = line;
            tailToken -> next -> endLine = line;
            tailToken -> next -> kind = EOF_;
            
            buffer[bufferIndex] = 'E';
            bufferIndex++;
            
            buffer[bufferIndex] = 'O';
            bufferIndex++;
            
            buffer[bufferIndex] = 'F';
            bufferIndex++;
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> next -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> next -> image[i] = buffer[i];
            
            bufferIndex = 0;
            tailToken = tailToken -> next;
            tailSpecialToken = tailToken;
        }
        
        if (syntaxError) {
            printf("Melon: lexical \033[31merror\033[0m in line %d\n", line);
            exit(-1);
        }
    }
    
    free(buffer);
    return headToken;
}
