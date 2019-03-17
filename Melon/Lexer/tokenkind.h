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

//标识符
#define IDENTIFIER 28

//整数字面量
#define INTEGER 29

//浮点数字面量
#define FLOAT 30

//字符字面量
#define CHARACTER 31

//字符串字面量
#define STRING 32

//空白符
#define SPACES 33    // ' ', '\r', '\n', '\t', '\f'

//注释
#define LINE_COMMENT 34
#define BLOCK_COMMENT 35

//算术运算符
#define SUM 36           // +
#define SUB 37           // -
#define MUL 38           // *
#define DIV 39           // /
#define RES 40           // %
#define AND 41           // &
#define OR 42            // |
#define NOT 43           // ~
#define XOR 44           // ^
#define SELFSUM 45       // ++
#define SELFSUB 46       // --
#define SUMASSIGN 47     // +=
#define SUBASSIGN 48     // -=
#define MULASSIGN 49     // *=
#define DIVASSIGN 50     // /=
#define RESASSIGN 51     // %=
#define ANDASSIGN 52     // &=
#define ORASSIGN 53      // |=
#define XORASSIGN 54     // ^=

//逻辑运算符
#define LOGICAND 55         // &&
#define LOGICOR 56          // ||
#define LOGICNOT 57         // !
#define LESSTHAN 58         // <
#define GREATERTHAN 59      // >
#define LESSANDEQUAL 60     // <=
#define GREATERANDEQUAL 61  // >=
#define EQUAL 62            // ==
#define NOTEQUAL 63         // !=

//赋值
#define ASSIGN 64   // =

//括号
#define LEFTPARENTHESE 65    // (
#define RIGHTPARENTHESE 66   // )
#define LEFTBRACKET 67       // [
#define RIGHTBRACKET 68      // ]
#define LEFTBRACE 69         // {
#define RIGHTBRACE 70        // }

//标点符号
#define COMMA 71       // ,
#define SEMICOLON 72   // ;
#define DOT 73         // .

//箭头
#define ARROW 74       // ->

//文件结尾
#define EOF_ 75

#endif /* tokenkind_h */
