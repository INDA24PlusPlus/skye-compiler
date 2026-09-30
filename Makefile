CC = clang
CFLAGS := -Wall -Wextra -g
SRC	:= src/main.c
LIBS := -lm
MAIN:=csklang
OBJECT:=$(SRC:.c=.o)
OUT:=out
MAINOUT:=$(OUT)/$(MAIN)
all: $(OUT) $(MAIN)
$(OUT):
	mkdir -p $(OUT)
$(MAIN): $(OBJECT)
	$(CC) $(CFLAGS) $? -o $(MAINOUT) $(LIBS)
$(OBJECT): $(SRC)
	$(CC) $(CFLAGS) -c $? -o $@ $(LIBS)

clean:
	rm -f $(MAINOUT)
	rm -f $(OBJECT)

run: all
	./$(MAINOUT)

