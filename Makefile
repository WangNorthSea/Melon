src = $(wildcard ASTNode/*.c) $(wildcard Dumper/*.c) $(wildcard Parser/*.c) $(wildcard Lexer/*.c) $(wildcard SymbolTable/*.c) $(wildcard Semantics/*.c) $(wildcard IR/*.c) $(wildcard main.c)
obj = $(patsubst %.c, %.o, $(src))
target = Melon
CC = gcc

$(target): $(obj)
	$(CC) $(obj) -o $(target)

%.o: %.c
	$(CC) -O2 -c $< -o $@

.PHONY: clean all
clean:
	rm -f $(obj)
all:
	@echo $(src)
	@echo $(obj)
