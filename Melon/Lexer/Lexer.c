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
#include "tokenkind.h"
#include "lexstate.h"

struct Token {
    int kind;
    int beginLine;
    int endLine;
    char * image;
    struct Token * next;
    struct Token * specialToken;
};

typedef struct Token Token;

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
    int state = DEFAULT_STATE;
    Token * headToken = (Token *)malloc(sizeof(Token));
    Token * tailToken = headToken;
    Token * tailSpecialToken = headToken;
    
    tokenInit(headToken);
    
    while (1) {
        if (ch == EOF)
            break;
        
        //识别空白符，正则表达式: ([" ", "\t", "\n", "\r", "\f"])+
        if ((state == DEFAULT_STATE) && (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r' || ch == '\f')) {
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
        if ((state == DEFAULT_STATE) && (ch == '_' || (ch >= 'a' && ch <= 'z') || (ch >= 'A' && ch <= 'Z'))) {
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
                else
                    tailToken -> next -> kind = IDENTIFIER;
            }
            else
                tailToken -> next -> kind = IDENTIFIER;
            
            bufferIndex = 0;
            tailToken = tailToken -> next;
            tailSpecialToken = tailToken;
        }
    }
    
    free(buffer);
    return headToken;
}
