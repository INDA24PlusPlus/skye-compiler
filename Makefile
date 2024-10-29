CFLAGS = -Wall -Wextra -g -O0
CLIBS= -lm
CC = gcc
compiler: main.o
	$(CC) $(CFLAGS) $? -o $@ $(CLIBS)
main.o: main.c
	$(CC) $(CFLAGS) -c $? -o $@ $(CLIBS)

