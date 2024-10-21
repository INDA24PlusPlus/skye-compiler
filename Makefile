CFLAGS = -Wall -Wextra -g -O0
CLIBS= -lm
CC = gcc
builder: tester compiler
tester: test.s
	cc $? -o $@
test.s: test.ssa
	qbe $? -o $@
compiler: main.o
	$(CC) $(CFLAGS) $? -o $@ $(CLIBS)
main.o: main.c
	$(CC) $(CFLAGS) -c $? -o $@ $(CLIBS)
