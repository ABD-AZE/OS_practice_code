/*
---------------------------------------------------------
MyAlloc Test: Test cases for the custom memory allocator
---------------------------------------------------------
*/

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include "MyAlloc.h"

// Simple test to allocate and free memory
void test_basic_allocation() {
    printf("Running basic allocation test...\n");
    
    // Allocate a small block
    int* numbers = (int*)alloc(10 * sizeof(int));
    assert(numbers != NULL);
    
    // Write to the allocated memory
    for (int i = 0; i < 10; i++) {
        numbers[i] = i;
    }
    
    // Verify the values
    for (int i = 0; i < 10; i++) {
        assert(numbers[i] == i);
    }
    
    // Free the memory
    myfree(numbers);
    
    printf("Basic allocation test passed!\n");
}

// Test allocating multiple blocks of different sizes
void test_multiple_allocations() {
    printf("Running multiple allocations test...\n");
    
    // Allocate several blocks of different sizes
    char* block1 = (char*)alloc(100);
    int* block2 = (int*)alloc(25 * sizeof(int));
    double* block3 = (double*)alloc(10 * sizeof(double));
    
    assert(block1 != NULL);
    assert(block2 != NULL);
    assert(block3 != NULL);
    
    // Write to the blocks
    memset(block1, 'A', 100);
    for (int i = 0; i < 25; i++) {
        block2[i] = i * 2;
    }
    for (int i = 0; i < 10; i++) {
        block3[i] = i * 1.5;
    }
    
    // Verify the values
    for (int i = 0; i < 100; i++) {
        assert(block1[i] == 'A');
    }
    for (int i = 0; i < 25; i++) {
        assert(block2[i] == i * 2);
    }
    for (int i = 0; i < 10; i++) {
        assert(block3[i] == i * 1.5);
    }
    
    // Free the blocks
    myfree(block1);
    myfree(block2);
    myfree(block3);
    
    printf("Multiple allocations test passed!\n");
}

// Test allocating a large block (greater than THRESHOLD)
void test_large_allocation() {
    printf("Running large allocation test...\n");
    
    // Allocate a block larger than THRESHOLD
    size_t large_size = THRESHOLD * 2;
    void* large_block = alloc(large_size);
    assert(large_block != NULL);
    
    // Write to the block
    memset(large_block, 0x55, large_size);
    
    // Verify a few values
    unsigned char* bytes = (unsigned char*)large_block;
    for (int i = 0; i < 100; i++) {
        assert(bytes[i] == 0x55);
    }
    
    // Free the block
    myfree(large_block);
    
    printf("Large allocation test passed!\n");
}

int main() {
    printf("Starting MyAlloc tests...\n");
    
    test_basic_allocation();
    test_multiple_allocations();
    test_large_allocation();
    
    printf("All tests completed successfully!\n");
    return 0;
} 