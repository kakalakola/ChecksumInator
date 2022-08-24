CC=gcc.exe
CFLAGS=-c -Wall -O3
CVERSION=-std=c17
PROJECT=checksum_inator

SRC=$(wildcard *.c) $(wildcard ./console/*.c)
OBJ=$(SRC:.c=.o)
ASM=$(SRC:.c=.s)
BIN=../bin/$(PROJECT).exe

RM= rm -f

all: $(SRC) $(BIN)

clean:
	${RM} $(ASM) $(OBJ) $(BIN)

$(BIN): $(OBJ)
	$(CC) $(OBJ) -s -o $@

.c.o:
	$(CC) $(CVERSION) $(CFLAGS) -s $< -o $@

full: clean all
