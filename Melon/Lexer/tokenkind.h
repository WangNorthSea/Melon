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

//字符字面量
#define CHARACTER 30

//字符串字面量
#define STRING 31

//空白符
#define SPACES 32

//注释
#define LINE_COMMENT 33
#define BLOCK_COMMENT 34

//文件结尾
#define EOF_ 35

#endif /* tokenkind_h */
