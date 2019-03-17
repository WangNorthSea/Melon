//
//  tokenkind.h
//  Melon
//
//  Created by 王浩宇 on 2019/3/16.
//  Copyright © 2019 UCAS Developers. All rights reserved.
//

#ifndef tokenkind_h
#define tokenkind_h

//保留字
#define VOID 0
#define CHAR 1
#define SHORT 2
#define INT 3
#define LONG 4
#define STRUCT 5
#define UNION 6
#define ENUM 7
#define STATIC 8
#define EXTERN 9
#define CONST 10
#define SIGNED 11
#define UNSIGNED 12
#define IF 13
#define ELSE 14
#define SWITCH 15
#define CASE 16
#define DEFAULT 17
#define WHILE 18
#define DO 19
#define FOR 20
#define RETURN 21
#define BREAK 22
#define CONTINUE 23
#define GOTO 24
#define TYPEDEF 25
#define IMPORT 26
#define SIZEOF 27
#define FLOAT 28
#define DOUBLE 29

//标识符
#define IDENTIFIER 30

//整数字面量
#define INTEGER 31

//浮点数字面量
#define FLOAT_ 32

//字符字面量
#define CHARACTER 33

//字符串字面量
#define STRING 34

//空白符
#define SPACES 35    // ' ', '\r', '\n', '\t', '\f'

//注释
#define LINE_COMMENT 36
#define BLOCK_COMMENT 37

//算术运算符
#define SUM 38           // +
#define SUB 39           // -
#define MUL 40           // *
#define DIV 41           // /
#define RES 42           // %
#define AND 43           // &
#define OR 44            // |
#define NOT 45           // ~
#define XOR 46           // ^
#define SELFSUM 47       // ++
#define SELFSUB 48       // --
#define SUMASSIGN 49     // +=
#define SUBASSIGN 50     // -=
#define MULASSIGN 51     // *=
#define DIVASSIGN 52     // /=
#define RESASSIGN 53     // %=
#define ANDASSIGN 54     // &=
#define ORASSIGN 55      // |=
#define XORASSIGN 56     // ^=

//逻辑运算符
#define LOGICAND 57         // &&
#define LOGICOR 58          // ||
#define LOGICNOT 59         // !
#define LESSTHAN 60         // <
#define GREATERTHAN 61      // >
#define LESSANDEQUAL 62     // <=
#define GREATERANDEQUAL 63  // >=
#define EQUAL 64            // ==
#define NOTEQUAL 65         // !=

//赋值
#define ASSIGN 66   // =

//括号
#define LEFTPARENTHESE 67    // (
#define RIGHTPARENTHESE 68   // )
#define LEFTBRACKET 69       // [
#define RIGHTBRACKET 70      // ]
#define LEFTBRACE 71         // {
#define RIGHTBRACE 72        // }

//标点符号
#define COMMA 73       // ,
#define SEMICOLON 74   // ;
#define DOT 75         // .

//箭头
#define ARROW 76       // ->

//文件结尾
#define EOF_ 77

#endif /* tokenkind_h */
