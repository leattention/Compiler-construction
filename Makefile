# Project: VYPa Compiler (vypcomp)
# Authors: xarmenl00, xtranke00

CC = gcc
CFLAGS = -std=c11 -Wall -Wextra -O2 -D_POSIX_C_SOURCE=200809L
FLEX = flex
BISON = bison

OBJS = main.o parser.tab.o lex.yy.o ast.o symtable.o semantic.o codegen.o

all: vypcomp

vypcomp: $(OBJS)
	$(CC) $(CFLAGS) -o $@ $(OBJS)

parser.tab.c parser.tab.h: parser.y
	$(BISON) -d -o parser.tab.c parser.y

lex.yy.c: scanner.l parser.tab.h
	$(FLEX) -o lex.yy.c scanner.l

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
 -f $(OBJS) parser.tab.c parser.tab.h lex.yy.c vypcomp vypcomp.exe out*.vc

.PHONY: all clean
