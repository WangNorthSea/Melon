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
#define LSH 57           // <<
#define RSH 58           // >>
#define LSHASSIGN 59     // <<=
#define RSHASSIGN 60     // >>=

//逻辑运算符
#define LOGICAND 61         // &&
#define LOGICOR 62          // ||
#define LOGICNOT 63         // !
#define LESSTHAN 64         // <
#define GREATERTHAN 65      // >
#define LESSANDEQUAL 66     // <=
#define GREATERANDEQUAL 67  // >=
#define EQUAL 68            // ==
#define NOTEQUAL 69         // !=

//赋值
#define ASSIGN 70   // =

//括号
#define LEFTPARENTHESE 71    // (
#define RIGHTPARENTHESE 72   // )
#define LEFTBRACKET 73       // [
#define RIGHTBRACKET 74      // ]
#define LEFTBRACE 75         // {
#define RIGHTBRACE 76        // }

//标点符号
#define COMMA 77       // ,
#define SEMICOLON 78   // ;
#define DOT 79         // .
#define COLON 80       // :
#define QUESTION 81    // ?

//箭头
#define ARROW 82       // ->

//文件结尾
#define EOF_ 83

//CACT
#define BOOL 84
#define BOOL_ 85    //bool literal value

#define DOUBLE_ 86

#endif /* tokenkind_h */
