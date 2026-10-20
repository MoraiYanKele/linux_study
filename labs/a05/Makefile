CC = gcc
CPPFLAGS = -I include
CFLAGS = -std=c11 -Wall -Wextra -g -O0

all: demo_worker

demo_worker: build/demo_worker.o build/worker_output.o
	$(CC) build/demo_worker.o build/worker_output.o -o demo_worker

build/demo_worker.o: workers/demo_worker.c include/worker_output.h
	mkdir -p build
	$(CC) $(CPPFLAGS) $(CFLAGS) -c workers/demo_worker.c -o build/demo_worker.o

build/worker_output.o: workers/worker_output.c include/worker_output.h
	mkdir -p build
	$(CC) $(CPPFLAGS) $(CFLAGS) -c workers/worker_output.c -o build/worker_output.o

clean:
	rm -f build/demo_worker.o build/worker_output.o demo_worker

.PHONY: all clean