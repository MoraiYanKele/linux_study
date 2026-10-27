CC = gcc
CPPFLAGS = -I inc
CFLAGS = -std=c11 -Wall -Wextra -g -O0

all: minisv

minisv: build/main.o build/command.o
	$(CC) build/main.o build/command.o -o minisv

build/main.o: src/main.c inc/command.h
	mkdir -p build
	$(CC) $(CPPFLAGS) $(CFLAGS) -c src/main.c -o build/main.o

build/command.o: src/command.c inc/command.h
	mkdir -p build
	$(CC) $(CPPFLAGS) $(CFLAGS) -c src/command.c -o build/command.o

clean:
	rm -f build/main.o build/command.o minisv

.PHONY: all clean