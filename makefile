BIN:=fcompiler
SRC:=src
OBJ:=build
ARGS:=grammar.grm

CC:=gcc
CFLAGS:=-std=c18

SRCS:=$(wildcard $(SRC)/*.c)
OBJS:=$(patsubst $(SRC)/%.c,$(OBJ)/%.o,$(SRCS))

.PHONY: run clean

run: $(BIN)
	./$(BIN) $(ARGS)

$(BIN): $(OBJS)
	$(CC) $(CFLAGS) $^ -o $@

$(OBJ)/%.o: $(SRC)/%.c $(OBJ)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJ):
	mkdir -p $(OBJ)

clean:
	$(RM) -rvf $(OBJS) $(OBJ) $(BIN)
