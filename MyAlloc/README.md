# MyAlloc: Custom Memory Allocator

A custom implementation of memory allocation functions similar to malloc/free.

## Features

- **Multiple Allocation Strategies**: Supports first fit, best fit, and worst fit algorithms
- **Threshold-Based Allocation**: Uses `sbrk` for small allocations and `mmap` for large ones
- **Configurable**: Can be configured to use different allocation strategies
- **Memory Visualization**: Includes a terminal-based tool to visualize memory allocation

## Implementation Details

- **Header Structure**: Each allocated block has a header containing size and magic number
- **Free List**: Uses a linked list to track free blocks
- **Alignment**: Memory is aligned to 8-byte boundaries
- **Magic Number**: Used to validate pointers during free operations

## Usage

To use the memory allocator in your program:

```c
#include "MyAlloc.h"

int main() {
    // Allocate memory
    int* array = (int*)alloc(10 * sizeof(int));
    
    // Use the memory
    for (int i = 0; i < 10; i++) {
        array[i] = i;
    }
    
    // Free the memory when done
    myfree(array);
    
    return 0;
}
```

## Configuration

You can configure the allocation strategy by modifying the `FIT` macro in `MyAlloc.h`:

- `FIRST_FIT`: Use the first block that fits (default)
- `BEST_FIT`: Use the smallest block that fits
- `WORST_FIT`: Use the largest block that fits

## Building and Testing

To build the test program and visualization tool:

```bash
make
```

To run the tests:

```bash
make test
```

To run the memory visualizer:

```bash
make run_visualize
```

To clean up compiled files:

```bash
make clean
```

## Memory Visualizer

The included memory visualizer provides an interactive way to understand how the memory allocator works:

- **Allocation**: Allocate memory blocks with custom sizes and labels
- **Freeing**: Free previously allocated blocks
- **Visualization**: See a graphical representation of memory usage
- **Statistics**: View memory usage statistics and fragmentation metrics

### Visualizer Commands

- `allocate`: Allocate a new memory block
- `free`: Free an existing memory block
- `view`: Display the current memory map
- `stats`: Show memory usage statistics
- `quit`: Exit the visualizer

## Contributors

- Abdullah Azeem <abdullahazeem770@gmail.com> 