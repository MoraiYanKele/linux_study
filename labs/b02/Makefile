CC = gcc
CPPFLAGS = -I include
CFLAGS = -std=c11 -Wall -Wextra -g -O0

all: log_demo

log_demo: main.o log.o
	$(CC) main.o log.o -o log_demo

log.o: log.c log.h 
	mkdir -p build
	$(CC) $(CPPFLAGS) $(CFLAGS) -c log.c -o log.o

main.o: main.c log.h
	mkdir -p build
	$(CC) $(CPPFLAGS) $(CFLAGS) -c main.c -o main.o

clean:
	rm -f main.o log.o log_demo

.PHONY: all clean