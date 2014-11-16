CC = gcc
DEBUG = -g -DDEBUG
#CFLAGS = -Wall -Werror $(DEBUG) -Wint-to-pointer-cast
CFLAGS = $(DEBUG) -Wint-to-pointer-cast

PROGS = huffman
INC = node.h return_codes.h
LIBS = heap.o compress.o extract.o stack.o binary_utility.o
OBJS = main.o
TESTS = tests.o
UNIT_TESTS = unit_tests

tests: $(LIBS) $(INC) $(TESTS)
	$(CC) $(LIBS) $(TESTS) $(CFLAGS) -o $(UNIT_TESTS)

huffman: $(LIBS) $(INC) $(OBJS)
	$(CC) $(OBJS) $(LIBS) $(CFLAGS) -o $(PROGS) 

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

binary_utility.o: binary_utility.h binary_utility.c
	$(CC) $(CFLAGS) -c binary_utility.c

.PHONY: clean
clean:
	rm -rf $(PROGS) $(OBJS) $(UNIT_TESTS) $(TESTS) $(LIBS) *~
