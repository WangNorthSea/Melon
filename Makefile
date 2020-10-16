src = ASTNode/*.c Dumper/*.c Parser/*.c Lexer/*.c SymbolTable/*.c Semantics/*.c IR/*.c main.c
obj = ASTNode/*.o Dumper/*.o Parser/*.o Lexer/*.o SymbolTable/*.o Semantics/*.o IR/*.o *.o
CFLAGS = -O2
target = Melon
CC = clang

.PHONY: all clean
all:
	${CC} ${CFLAGS} ${src} -o ${target}

clean:
	rm -f ${obj}
