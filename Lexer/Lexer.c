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
    init_list_head(&token -> list);
    token -> beginLine = 0;
    token -> kind = 0;
    token -> image = NULL;
    token -> fpos = 0;
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
    Token * tailToken = NULL;
    
    tokenInit(headToken);
    
    while (1) {
        syntaxError = 1;
        
        if (ch == EOF)
            break;
        
        //识别空白符，正则表达式: ([" ", "\t", "\n", "\r", "\f"])+
        if (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '\f') {
            syntaxError = 0;
            do {
                if (ch == '\n')
                    line++;
                ch = fgetc(fp);
            } while (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '\f');
        }
        
        //识别标识符，正则表达式: ["a" - "z", "A" - "Z", "_"](["a" - "z", "A" - "Z", "_", "0" - "9"])*
        if (ch == '_' || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z')) {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            
            do {
                if (bufferIndex == bufferSize - 1)
                    enlargeBuffer(buffer, &bufferSize);
                
                buffer[bufferIndex] = ch;
                bufferIndex++;
                ch = fgetc(fp);
            } while (ch == '_' || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z') || (ch >= '0' && ch <= '9'));
            buffer[bufferIndex] = '\0';
            
            tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> image[i] = buffer[i];
            
            //识别保留字
            if (bufferIndex < 9) {
                if (!strcmp(buffer, "void"))
                    tailToken -> kind = VOID;
                else if (!strcmp(buffer, "char"))
                    tailToken -> kind = CHAR;
                else if (!strcmp(buffer, "short"))
                    tailToken -> kind = SHORT;
                else if (!strcmp(buffer, "int"))
                    tailToken -> kind = INT;
                else if (!strcmp(buffer, "long"))
                    tailToken -> kind = LONG;
                else if (!strcmp(buffer, "struct"))
                    tailToken -> kind = STRUCT;
                else if (!strcmp(buffer, "union"))
                    tailToken -> kind = UNION;
                else if (!strcmp(buffer, "enum"))
                    tailToken -> kind = ENUM;
                else if (!strcmp(buffer, "static"))
                    tailToken -> kind = STATIC;
                else if (!strcmp(buffer, "extern"))
                    tailToken -> kind = EXTERN;
                else if (!strcmp(buffer, "const"))
                    tailToken -> kind = CONST;
                else if (!strcmp(buffer, "signed"))
                    tailToken -> kind = SIGNED;
                else if (!strcmp(buffer, "unsigned"))
                    tailToken -> kind = UNSIGNED;
                else if (!strcmp(buffer, "if"))
                    tailToken -> kind = IF;
                else if (!strcmp(buffer, "else"))
                    tailToken -> kind = ELSE;
                else if (!strcmp(buffer, "switch"))
                    tailToken -> kind = SWITCH;
                else if (!strcmp(buffer, "case"))
                    tailToken -> kind = CASE;
                else if (!strcmp(buffer, "default"))
                    tailToken -> kind = DEFAULT;
                else if (!strcmp(buffer, "while"))
                    tailToken -> kind = WHILE;
                else if (!strcmp(buffer, "do"))
                    tailToken -> kind = DO;
                else if (!strcmp(buffer, "for"))
                    tailToken -> kind = FOR;
                else if (!strcmp(buffer, "return"))
                    tailToken -> kind = RETURN;
                else if (!strcmp(buffer, "break"))
                    tailToken -> kind = BREAK;
                else if (!strcmp(buffer, "continue"))
                    tailToken -> kind = CONTINUE;
                else if (!strcmp(buffer, "goto"))
                    tailToken -> kind = GOTO;
                else if (!strcmp(buffer, "typedef"))
                    tailToken -> kind = TYPEDEF;
                else if (!strcmp(buffer, "import"))
                    tailToken -> kind = IMPORT;
                else if (!strcmp(buffer, "sizeof"))
                    tailToken -> kind = SIZEOF;
                else if (!strcmp(buffer, "float"))
                    tailToken -> kind = FLOAT;
                else if (!strcmp(buffer, "double"))
                    tailToken -> kind = DOUBLE;
                else if (!strcmp(buffer, "bool"))
                    tailToken -> kind = BOOL;
                else if (!strcmp(buffer, "true") || !strcmp(buffer, "false"))
                    tailToken -> kind = BOOL_;
                else
                    tailToken -> kind = IDENTIFIER;
            }
            else
                tailToken -> kind = IDENTIFIER;
            
            bufferIndex = 0;
            fgetpos(fp, &tailToken -> fpos);
            list_add_tail(&tailToken -> list, &headToken -> list);
        }
        
        //识别字符字面量，正则表达式: "'"[~("'", "\n")]"'"
        if (ch == '\'') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            tailToken -> kind = CHARACTER;
            
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
            
            tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> image[i] = buffer[i];
            
            bufferIndex = 0;
            fgetpos(fp, &tailToken -> fpos);
            list_add_tail(&tailToken -> list, &headToken -> list);
        }
        
        //识别字符串字面量，正则表达式: """[~(""", "\n")]"""
        if (ch == '\"') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            tailToken -> kind = STRING;
            
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
            
            tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> image[i] = buffer[i];
            
            bufferIndex = 0;
            fgetpos(fp, &tailToken -> fpos);
            list_add_tail(&tailToken -> list, &headToken -> list);
        }
        
        //识别逗号，正则表达式: ","
        if (ch == ',') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            tailToken -> kind = COMMA;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> image[i] = buffer[i];
            
            bufferIndex = 0;
            fgetpos(fp, &tailToken -> fpos);
            list_add_tail(&tailToken -> list, &headToken -> list);
        }
        
        //识别分号，正则表达式: ";"
        if (ch == ';') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            tailToken -> kind = SEMICOLON;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> image[i] = buffer[i];
            
            bufferIndex = 0;
            fgetpos(fp, &tailToken -> fpos);
            list_add_tail(&tailToken -> list, &headToken -> list);
        }
        
        //识别句号，正则表达式: "."
        if (ch == '.') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            tailToken -> kind = DOT;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> image[i] = buffer[i];
            
            bufferIndex = 0;
            fgetpos(fp, &tailToken -> fpos);
            list_add_tail(&tailToken -> list, &headToken -> list);
        }
        
        //识别冒号，正则表达式: ":"
        if (ch == ':') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            tailToken -> kind = COLON;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> image[i] = buffer[i];
            
            bufferIndex = 0;
            fgetpos(fp, &tailToken -> fpos);
            list_add_tail(&tailToken -> list, &headToken -> list);
        }
        
        //识别问号，正则表达式: "?"
        if (ch == '?') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            tailToken -> kind = QUESTION;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> image[i] = buffer[i];
            
            bufferIndex = 0;
            fgetpos(fp, &tailToken -> fpos);
            list_add_tail(&tailToken -> list, &headToken -> list);
        }
        
        //识别左小括号，正则表达式: "("
        if (ch == '(') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            tailToken -> kind = LEFTPARENTHESE;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> image[i] = buffer[i];
            
            bufferIndex = 0;
            fgetpos(fp, &tailToken -> fpos);
            list_add_tail(&tailToken -> list, &headToken -> list);
        }
        
        //识别右小括号，正则表达式: ")"
        if (ch == ')') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            tailToken -> kind = RIGHTPARENTHESE;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> image[i] = buffer[i];
            
            bufferIndex = 0;
            fgetpos(fp, &tailToken -> fpos);
            list_add_tail(&tailToken -> list, &headToken -> list);
        }
        
        //识别左中括号，正则表达式: "["
        if (ch == '[') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            tailToken -> kind = LEFTBRACKET;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> image[i] = buffer[i];
            
            bufferIndex = 0;
            fgetpos(fp, &tailToken -> fpos);
            list_add_tail(&tailToken -> list, &headToken -> list);
        }
        
        //识别右中括号，正则表达式: "]"
        if (ch == ']') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            tailToken -> kind = RIGHTBRACKET;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> image[i] = buffer[i];
            
            bufferIndex = 0;
            fgetpos(fp, &tailToken -> fpos);
            list_add_tail(&tailToken -> list, &headToken -> list);
        }
        
        //识别左大括号，正则表达式: "{"
        if (ch == '{') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            tailToken -> kind = LEFTBRACE;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> image[i] = buffer[i];
            
            bufferIndex = 0;
            fgetpos(fp, &tailToken -> fpos);
            list_add_tail(&tailToken -> list, &headToken -> list);
        }
        
        //识别右大括号，正则表达式: "}"
        if (ch == '}') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            tailToken -> kind = RIGHTBRACE;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> image[i] = buffer[i];
            
            bufferIndex = 0;
            fgetpos(fp, &tailToken -> fpos);
            list_add_tail(&tailToken -> list, &headToken -> list);
        }
        
        //识别赋值与等于，正则表达式: "="U"=="
        if (ch == '=') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            if (ch == '=') {
                tailToken -> kind = EQUAL;
                buffer[bufferIndex] = ch;
                bufferIndex++;
                ch = fgetc(fp);
            }
            else
                tailToken -> kind = ASSIGN;
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> image[i] = buffer[i];
            
            bufferIndex = 0;
            fgetpos(fp, &tailToken -> fpos);
            list_add_tail(&tailToken -> list, &headToken -> list);
        }
        
        //识别/, /=以及行注释与块注释，正则表达式: "//"~("\n")* U "/*"~("*/")*"*/" 不规范，大体是这个意思
        if (ch == '/') {
            syntaxError = 0;
            
            buffer[bufferIndex] = ch;
            bufferIndex++;
            ch = fgetc(fp);
            
            if (ch == '/') {
                do {
                    ch = fgetc(fp);
                } while (ch != '\n' && ch != EOF);
                bufferIndex = 0;
            }
            else if (ch == '=') {
                tailToken = (Token *)malloc(sizeof(Token));
                tokenInit(tailToken);
                tailToken -> beginLine = line;
                tailToken -> kind = DIVASSIGN;
                
                buffer[bufferIndex] = '=';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
            else if (ch == '*') {
            restart:
                do {
                    if (ch == '\n')
                        line++;
                    ch = fgetc(fp);
                } while (ch != '*' && ch != EOF);
                
                if (ch == '*') {
                    ch = fgetc(fp);
                    if (ch != '/') {
                        fpos_t fpos;
                        fgetpos(fp, &fpos);
                        fseek(fp, fpos - 1, SEEK_SET);
                        ch = '*';
                        goto restart;
                    }
                    else {
                        ch = fgetc(fp);
                        bufferIndex = 0;
                    }
                }
                else
                    bufferIndex = 0;
            }
            else {
                tailToken = (Token *)malloc(sizeof(Token));
                tokenInit(tailToken);
                tailToken -> beginLine = line;
                tailToken -> kind = DIV;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
        }
        
        //识别+, ++, +=以及以+开头的整数、浮点数字面量
        if (ch == '+') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            
            ch = fgetc(fp);
            
            if (ch == '+') {
                tailToken -> kind = SELFSUM;
                
                buffer[bufferIndex] = '+';
                bufferIndex++;
                
                buffer[bufferIndex] = '+';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
            else if (ch == '=') {
                tailToken -> kind = SUMASSIGN;
                
                buffer[bufferIndex] = '+';
                bufferIndex++;
                
                buffer[bufferIndex] = '=';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
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
                    //tailToken -> kind = FLOAT_;
                int is_sn = 0;
                
                do {
                    if (bufferIndex == bufferSize - 1)
                        enlargeBuffer(buffer, &bufferSize);
                    buffer[bufferIndex] = ch;
                    bufferIndex++;
                    ch = fgetc(fp);
                } while (ch >= '0' && ch <= '9');

                

                if (ch == 'e' || ch == 'E') {
                    is_sn = 1;
                    buffer[bufferIndex] = ch;
                    bufferIndex++;
                    ch = fgetc(fp);


                }

                if (is_sn) {
                    if (ch == '+' || ch == '-'){
                        buffer[bufferIndex] = ch;
                        bufferIndex++;
                        ch = fgetc(fp);

                    } else {
                        ;
                    }
                    while (ch >= '0' && ch <= '9') {
                        if (bufferIndex == bufferSize - 1)
                            enlargeBuffer(buffer, &bufferSize);
                    
                            buffer[bufferIndex] = ch;
                            bufferIndex++;
                            ch = fgetc(fp);
                    }
                }

                int is_float = 0;

                if (ch == 'f' || ch == 'F') {
                    is_float = 1;
                    buffer[bufferIndex] = ch;
                    bufferIndex++;
                    ch = fgetc(fp);
                }

                if (is_float) {
                    tailToken -> kind = FLOAT_;
                } else {
                    tailToken -> kind = DOUBLE_;
                }


                buffer[bufferIndex] = '\0';
                    
                    tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                    
                    for (int i = 0; i <= bufferIndex; i++)
                        tailToken -> image[i] = buffer[i];
                    
                    bufferIndex = 0;
                    fgetpos(fp, &tailToken -> fpos);
                    list_add_tail(&tailToken -> list, &headToken -> list);
                }
                else {
                    tailToken -> kind = INTEGER;
                    
                    buffer[bufferIndex] = '\0';
                    
                    tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                    
                    for (int i = 0; i <= bufferIndex; i++)
                        tailToken -> image[i] = buffer[i];
                    
                    bufferIndex = 0;
                    fgetpos(fp, &tailToken -> fpos);
                    list_add_tail(&tailToken -> list, &headToken -> list);
                }
            }
            else {
                tailToken -> kind = SUM;
                
                buffer[bufferIndex] = '+';
                bufferIndex++;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
        }
        
        //识别-, --, -=, ->以及以-开头的整数、浮点数字面量
        if (ch == '-') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            
            ch = fgetc(fp);
            
            if (ch == '-') {
                tailToken -> kind = SELFSUB;
                
                buffer[bufferIndex] = '-';
                bufferIndex++;
                
                buffer[bufferIndex] = '-';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
            else if (ch == '=') {
                tailToken -> kind = SUBASSIGN;
                
                buffer[bufferIndex] = '-';
                bufferIndex++;
                
                buffer[bufferIndex] = '=';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
            else if (ch == '>') {
                tailToken -> kind = ARROW;
                
                buffer[bufferIndex] = '-';
                bufferIndex++;
                
                buffer[bufferIndex] = '>';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
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
                    //tailToken -> kind = FLOAT_;
                int is_sn = 0;
                
                do {
                    if (bufferIndex == bufferSize - 1)
                        enlargeBuffer(buffer, &bufferSize);
                    buffer[bufferIndex] = ch;
                    bufferIndex++;
                    ch = fgetc(fp);
                } while (ch >= '0' && ch <= '9');

                

                if (ch == 'e' || ch == 'E') {
                    is_sn = 1;
                    buffer[bufferIndex] = ch;
                    bufferIndex++;
                    ch = fgetc(fp);


                }

                if (is_sn) {
                    if (ch == '+' || ch == '-'){
                        buffer[bufferIndex] = ch;
                        bufferIndex++;
                        ch = fgetc(fp);

                    } else {
                        ;
                    }
                    while (ch >= '0' && ch <= '9') {
                        if (bufferIndex == bufferSize - 1)
                            enlargeBuffer(buffer, &bufferSize);
                    
                            buffer[bufferIndex] = ch;
                            bufferIndex++;
                            ch = fgetc(fp);
                    }
                }

                int is_float = 0;

                if (ch == 'f' || ch == 'F') {
                    is_float = 1;
                    buffer[bufferIndex] = ch;
                    bufferIndex++;
                    ch = fgetc(fp);
                }

                if (is_float) {
                    tailToken -> kind = FLOAT_;
                } else {
                    tailToken -> kind = DOUBLE_;
                }


                buffer[bufferIndex] = '\0';
                    
                    tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                    
                    for (int i = 0; i <= bufferIndex; i++)
                        tailToken -> image[i] = buffer[i];
                    
                    bufferIndex = 0;
                    fgetpos(fp, &tailToken -> fpos);
                    list_add_tail(&tailToken -> list, &headToken -> list);
                }
                else {
                    tailToken -> kind = INTEGER;
                    
                    buffer[bufferIndex] = '\0';
                    
                    tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                    
                    for (int i = 0; i <= bufferIndex; i++)
                        tailToken -> image[i] = buffer[i];
                    
                    bufferIndex = 0;
                    fgetpos(fp, &tailToken -> fpos);
                    list_add_tail(&tailToken -> list, &headToken -> list);
                }
            }
            else {
                tailToken -> kind = SUB;
                
                buffer[bufferIndex] = '-';
                bufferIndex++;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
        }
        
        //识别直接以数字开头的整数、浮点数字面量
        if (ch >= '0' && ch <= '9') {
            int hasX = 0;
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            
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
                //tailToken -> kind = FLOAT_;
                int is_sn = 0;
                
                do {
                    if (bufferIndex == bufferSize - 1)
                        enlargeBuffer(buffer, &bufferSize);
                    buffer[bufferIndex] = ch;
                    bufferIndex++;
                    ch = fgetc(fp);
                } while (ch >= '0' && ch <= '9');

                

                if (ch == 'e' || ch == 'E') {
                    is_sn = 1;
                    buffer[bufferIndex] = ch;
                    bufferIndex++;
                    ch = fgetc(fp);


                }

                if (is_sn) {
                    if (ch == '+' || ch == '-'){
                        buffer[bufferIndex] = ch;
                        bufferIndex++;
                        ch = fgetc(fp);

                    } else {
                        ;
                    }
                    while (ch >= '0' && ch <= '9') {
                        if (bufferIndex == bufferSize - 1)
                            enlargeBuffer(buffer, &bufferSize);
                    
                            buffer[bufferIndex] = ch;
                            bufferIndex++;
                            ch = fgetc(fp);
                    }
                }

                int is_float = 0;

                if (ch == 'f' || ch == 'F') {
                    is_float = 1;
                    buffer[bufferIndex] = ch;
                    bufferIndex++;
                    ch = fgetc(fp);
                }

                if (is_float) {
                    tailToken -> kind = FLOAT_;
                } else {
                    tailToken -> kind = DOUBLE_;
                }

                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
            else {
                tailToken -> kind = INTEGER;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
        }
        
        //识别*, *=
        if (ch == '*') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            
            ch = fgetc(fp);
            
            if (ch == '=') {
                tailToken -> kind = MULASSIGN;
                
                buffer[bufferIndex] = '*';
                bufferIndex++;
                
                buffer[bufferIndex] = '=';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
            else {
                tailToken -> kind = MUL;
                
                buffer[bufferIndex] = '*';
                bufferIndex++;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
        }
        
        //识别%, %=
        if (ch == '%') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            
            ch = fgetc(fp);
            
            if (ch == '=') {
                tailToken -> kind = RESASSIGN;
                
                buffer[bufferIndex] = '%';
                bufferIndex++;
                
                buffer[bufferIndex] = '=';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
            else {
                tailToken -> kind = RES;
                
                buffer[bufferIndex] = '%';
                bufferIndex++;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
        }
        
        //识别&, &=, &&
        if (ch == '&') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            
            ch = fgetc(fp);
            
            if (ch == '=') {
                tailToken -> kind = ANDASSIGN;
                
                buffer[bufferIndex] = '&';
                bufferIndex++;
                
                buffer[bufferIndex] = '=';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
            else if (ch == '&') {
                tailToken -> kind = LOGICAND;
                
                buffer[bufferIndex] = '&';
                bufferIndex++;
                
                buffer[bufferIndex] = '&';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
            else {
                tailToken -> kind = AND;
                
                buffer[bufferIndex] = '&';
                bufferIndex++;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
        }
        
        //识别|, |=, ||
        if (ch == '|') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            
            ch = fgetc(fp);
            
            if (ch == '=') {
                tailToken -> kind = ORASSIGN;
                
                buffer[bufferIndex] = '|';
                bufferIndex++;
                
                buffer[bufferIndex] = '=';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
            else if (ch == '|') {
                tailToken -> kind = LOGICOR;
                
                buffer[bufferIndex] = '|';
                bufferIndex++;
                
                buffer[bufferIndex] = '|';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
            else {
                tailToken -> kind = OR;
                
                buffer[bufferIndex] = '|';
                bufferIndex++;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
        }
        
        //识别~
        if (ch == '~') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            
            ch = fgetc(fp);
            
            tailToken -> kind = NOT;
            
            buffer[bufferIndex] = '~';
            bufferIndex++;
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> image[i] = buffer[i];
            
            bufferIndex = 0;
            fgetpos(fp, &tailToken -> fpos);
            list_add_tail(&tailToken -> list, &headToken -> list);
        }
        
        //识别^, ^=
        if (ch == '^') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            
            ch = fgetc(fp);
            
            if (ch == '=') {
                tailToken -> kind = XORASSIGN;
                
                buffer[bufferIndex] = '^';
                bufferIndex++;
                
                buffer[bufferIndex] = '=';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
            else {
                tailToken -> kind = XOR;
                
                buffer[bufferIndex] = '^';
                bufferIndex++;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
        }
        
        //识别!, !=
        if (ch == '!') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            
            ch = fgetc(fp);
            
            if (ch == '=') {
                tailToken -> kind = NOTEQUAL;
                
                buffer[bufferIndex] = '!';
                bufferIndex++;
                
                buffer[bufferIndex] = '=';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
            else {
                tailToken -> kind = LOGICNOT;
                
                buffer[bufferIndex] = '!';
                bufferIndex++;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
        }
        
        //识别<, <=, <<, <<=
        if (ch == '<') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            
            ch = fgetc(fp);
            
            if (ch == '=') {
                tailToken -> kind = LESSANDEQUAL;
                
                buffer[bufferIndex] = '<';
                bufferIndex++;
                
                buffer[bufferIndex] = '=';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
            else if (ch == '<') {
                ch = fgetc(fp);
                
                if (ch == '=') {
                    tailToken -> kind = LSHASSIGN;
                    
                    buffer[bufferIndex] = '<';
                    bufferIndex++;
                    
                    buffer[bufferIndex] = '<';
                    bufferIndex++;
                    
                    buffer[bufferIndex] = '=';
                    bufferIndex++;
                    ch = fgetc(fp);
                    
                    buffer[bufferIndex] = '\0';
                    
                    tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                    
                    for (int i = 0; i <= bufferIndex; i++)
                        tailToken -> image[i] = buffer[i];
                    
                    bufferIndex = 0;
                    fgetpos(fp, &tailToken -> fpos);
                    list_add_tail(&tailToken -> list, &headToken -> list);
                }
                else {
                    tailToken -> kind = LSH;
                    
                    buffer[bufferIndex] = '<';
                    bufferIndex++;
                    
                    buffer[bufferIndex] = '<';
                    bufferIndex++;
                    
                    buffer[bufferIndex] = '\0';
                    
                    tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                    
                    for (int i = 0; i <= bufferIndex; i++)
                        tailToken -> image[i] = buffer[i];
                    
                    bufferIndex = 0;
                    fgetpos(fp, &tailToken -> fpos);
                    list_add_tail(&tailToken -> list, &headToken -> list);
                }
            }
            else {
                tailToken -> kind = LESSTHAN;
                
                buffer[bufferIndex] = '<';
                bufferIndex++;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
        }
        
        //识别>, >=, >>, >>=
        if (ch == '>') {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            
            ch = fgetc(fp);
            
            if (ch == '=') {
                tailToken -> kind = GREATERANDEQUAL;
                
                buffer[bufferIndex] = '>';
                bufferIndex++;
                
                buffer[bufferIndex] = '=';
                bufferIndex++;
                ch = fgetc(fp);
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
            else if (ch == '>') {
                ch = fgetc(fp);
                
                if (ch == '=') {
                    tailToken -> kind = RSHASSIGN;
                    
                    buffer[bufferIndex] = '>';
                    bufferIndex++;
                    
                    buffer[bufferIndex] = '>';
                    bufferIndex++;
                    
                    buffer[bufferIndex] = '=';
                    bufferIndex++;
                    ch = fgetc(fp);
                    
                    buffer[bufferIndex] = '\0';
                    
                    tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                    
                    for (int i = 0; i <= bufferIndex; i++)
                        tailToken -> image[i] = buffer[i];
                    
                    bufferIndex = 0;
                    fgetpos(fp, &tailToken -> fpos);
                    list_add_tail(&tailToken -> list, &headToken -> list);
                }
                else {
                    tailToken -> kind = RSH;
                    
                    buffer[bufferIndex] = '>';
                    bufferIndex++;
                    
                    buffer[bufferIndex] = '>';
                    bufferIndex++;
                    
                    buffer[bufferIndex] = '\0';
                    
                    tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                    
                    for (int i = 0; i <= bufferIndex; i++)
                        tailToken -> image[i] = buffer[i];
                    
                    bufferIndex = 0;
                    fgetpos(fp, &tailToken -> fpos);
                    list_add_tail(&tailToken -> list, &headToken -> list);
                }
            }
            else {
                tailToken -> kind = GREATERTHAN;
                
                buffer[bufferIndex] = '>';
                bufferIndex++;
                
                buffer[bufferIndex] = '\0';
                
                tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
                
                for (int i = 0; i <= bufferIndex; i++)
                    tailToken -> image[i] = buffer[i];
                
                bufferIndex = 0;
                fgetpos(fp, &tailToken -> fpos);
                list_add_tail(&tailToken -> list, &headToken -> list);
            }
        }
        
        //识别文件结尾，正则表达式: "EOF"
        if (ch == EOF) {
            syntaxError = 0;
            tailToken = (Token *)malloc(sizeof(Token));
            tokenInit(tailToken);
            tailToken -> beginLine = line;
            tailToken -> kind = EOF_;
            
            buffer[bufferIndex] = 'E';
            bufferIndex++;
            
            buffer[bufferIndex] = 'O';
            bufferIndex++;
            
            buffer[bufferIndex] = 'F';
            bufferIndex++;
            
            buffer[bufferIndex] = '\0';
            
            tailToken -> image = (char *)malloc(sizeof(char) * (bufferIndex + 1));
            
            for (int i = 0; i <= bufferIndex; i++)
                tailToken -> image[i] = buffer[i];
            
            bufferIndex = 0;
            fgetpos(fp, &tailToken -> fpos);
            list_add_tail(&tailToken -> list, &headToken -> list);
        }
        
        if (syntaxError) {
            printf("Melon: lexical \033[31merror\033[0m in line %d\n", line);
            exit(-1);
        }
    }
    
    free(buffer);
    return headToken;
}
