CC = gcc
CFLAGS = -Wall -Wextra -g

all: test_myalloc visualize

# Compile the memory allocator library
MyAlloc.o: MyAlloc.c MyAlloc.h
	$(CC) $(CFLAGS) -c MyAlloc.c -o MyAlloc.o

# Compile the test program
test_myalloc: test_myalloc.c MyAlloc.o
	$(CC) $(CFLAGS) test_myalloc.c MyAlloc.o -o test_myalloc

# Compile the basic visualizer
visualize: visualize_allocator.c MyAlloc.o
	$(CC) $(CFLAGS) visualize_allocator.c MyAlloc.o -o visualize

# Run the test program
test: test_myalloc
	./test_myalloc

# Run the basic visualizer
run_visualize: visualize
	./visualize


# Clean up compiled files
clean:
	rm -f *.o test_myalloc visualize

.PHONY: all test clean run_visualize