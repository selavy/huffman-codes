CC = gcc
DEBUG = -g
#CFLAGS = -Wall -Werror $(DEBUG)
CFLAGS = $(DEBUG)

PROGS = huffman
LIBS = heap.o node.h compress.o return_codes.h extract.o stack.o
OBJS = main.o
TESTS = tests.o
UNIT_TESTS = unit_tests

tests: $(LIBS) $(TESTS)
	$(CC) $(CFLAGS) -o $(UNIT_TESTS) $(LIBS) $(TESTS)

huffman: $(LIBS) $(OBJS)
	$(CC) $(CFLAGS) -o huffman $(OBJS) $(LIBS)

main.o: main.c
	$(CC) $(CFLAGS) -c main.c

tests.o: tests.c
	$(CC) $(CFLAGS) -c tests.c

heap.o: heap.h heap.c
	$(CC) $(CFLAGS) -c heap.c

compress.o: compress.h compress.c
	$(CC) $(CFLAGS) -c compress.c

extract.o: extract.h extract.c
	$(CC) $(CFLAGS) -c extract.c

stack.o: stack.h stack.c
	$(CC) $(CFLAGS) -c stack.c

.PHONY: clean
clean:
	rm -rf $(PROGS) $(OBJS) $(UNIT_TESTS) $(TESTS) *~