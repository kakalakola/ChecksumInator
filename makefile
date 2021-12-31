CC=gcc.exe
CFLAGS=-c -Wall -O3
CVERSION=-std=c17
PROJECT=ChecksumInator
SOURCE=$(wildcard *.c) $(wildcard ./console/*.c)
OBJECTS=$(SOURCE:.c=.o)
EXECUTABLE=../bin/$(PROJECT).exe
RM= rm -f

all: $(SOURCE) $(EXECUTABLE)

clean:
	${RM} $(OBJECTS) $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(OBJECTS) -s -o $@

.c.o:
	$(CC) $(CVERSION) $(CFLAGS) -s $< -o $@
