CC=gcc.exe
CFLAGS=-c -Wall -O3
CVERSION=-std=c17
PROJECT=ChecksumInator

SRC=$(wildcard *.c) $(wildcard ./console/*.c)
OBJ=$(SRC:.c=.o)
BIN=../bin/$(PROJECT).exe

RM= rm -f

all: $(SRC) $(BIN)

clean:
	${RM} $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -s -o $@

.c.o:
	$(CC) $(CVERSION) $(CFLAGS) -s $< -o $@