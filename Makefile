src = ASTNode/*.c Dumper/*.c Parser/*.c Lexer/*.c SymbolTable/*.c Semantics/*.c IR/*.c Report/*.c File/*.c main.c
obj = ASTNode/*.o Dumper/*.o Parser/*.o Lexer/*.o SymbolTable/*.o Semantics/*.o IR/*.o Report/*.o File/*.o *.o
CFLAGS = -O0 -g
target = Melon
CC = clang

.PHONY: all clean
all:
	${CC} ${CFLAGS} ${src} -o ${target}

clean:
	rm -f ${target}

clean_obj:
	rm -f ${obj}
