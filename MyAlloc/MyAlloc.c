/*
---------------------------------------------------------
MyAlloc: Reimplementation of malloc
CONTRIBUTORS: Abdullah Azeem <abdullahazeem770@gmail.com>
---------------------------------------------------------

Functionalities:
 - Coalescing
 - Allocation using sbrk for smaller requests and mmap for larger requests
 - exploring various fitting algorithms (first fit, best fit, worst fit)
 - configurable fitting algorithm
*/

#include "MyAlloc.h"
#include <unistd.h>
#include <sys/mman.h>
#include <string.h>

// Define MAP_ANONYMOUS if not available
#ifndef MAP_ANONYMOUS
#ifdef __linux__
#define MAP_ANONYMOUS 0x20
#elif defined(__APPLE__)
#define MAP_ANONYMOUS MAP_ANON
#else
#define MAP_ANONYMOUS 0x20  // Default value for most systems
#endif
#endif

// Global variable definition
node_t* head = NULL;

#define MAGIC 0x12345678
#define ALIGNMENT 8
#define ALIGN(size) (((size) + (ALIGNMENT-1)) & ~(ALIGNMENT-1))
#define HEADER_SIZE ALIGN(sizeof(header_t))

// Initialize the free list
static void init() {
    // Initial heap size (1MB)
    size_t initial_size = 1024 * 1024;
    void* heap = mmap(NULL, initial_size, PROT_READ | PROT_WRITE, 
                     MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (heap == MAP_FAILED) {
        return;
    }
    
    // Create the first free block
    head = (node_t*)heap;
    head->size = initial_size - sizeof(node_t);
    head->next = NULL;
}

// First fit algorithm: find the first block that fits
static void* first_fit(int bytes) {
    if (head == NULL) {
        init();
        if (head == NULL) return NULL; // Init failed
    }
    
    node_t* current = head;
    node_t* prev = NULL;
    
    // Find the first block that fits
    while (current != NULL) {
        if (current->size >= bytes + (int)HEADER_SIZE) {
            // Found a block
            if (current->size >= bytes + (int)HEADER_SIZE + (int)sizeof(node_t)) {
                // Split the block
                node_t* new_node = (node_t*)((char*)current + bytes + (int)HEADER_SIZE);
                new_node->size = current->size - bytes - (int)HEADER_SIZE;
                new_node->next = current->next;
                
                // Update the free list
                if (prev == NULL) {
                    head = new_node;
                } else {
                    prev->next = new_node;
                }
                
                // Prepare the allocated block
                header_t* header = (header_t*)current;
                header->size = bytes;
                header->magic = MAGIC;
                
                return (void*)((char*)header + HEADER_SIZE);
            } else {
                // Use the entire block
                if (prev == NULL) {
                    head = current->next;
                } else {
                    prev->next = current->next;
                }
                
                header_t* header = (header_t*)current;
                header->size = current->size;
                header->magic = MAGIC;
                
                return (void*)((char*)header + HEADER_SIZE);
            }
        }
        
        prev = current;
        current = current->next;
    }
    
    return NULL; // No suitable block found
}

// Best fit algorithm: find the smallest block that fits
static void* best_fit(int bytes) {
    if (head == NULL) {
        init();
        if (head == NULL) return NULL; // Init failed
    }
    
    node_t* current = head;
    node_t* prev = NULL;
    node_t* best = NULL;
    node_t* best_prev = NULL;
    int best_size = -1;
    
    // Find the smallest block that fits
    while (current != NULL) {
        if (current->size >= bytes + (int)HEADER_SIZE) {
            if (best_size == -1 || current->size < best_size) {
                best = current;
                best_prev = prev;
                best_size = current->size;
            }
        }
        
        prev = current;
        current = current->next;
    }
    
    if (best == NULL) {
        return NULL; // No suitable block found
    }
    
    // Use the best block
    if (best->size >= bytes + (int)HEADER_SIZE + (int)sizeof(node_t)) {
        // Split the block
        node_t* new_node = (node_t*)((char*)best + bytes + (int)HEADER_SIZE);
        new_node->size = best->size - bytes - (int)HEADER_SIZE;
        new_node->next = best->next;
        
        // Update the free list
        if (best_prev == NULL) {
            head = new_node;
        } else {
            best_prev->next = new_node;
        }
        
        // Prepare the allocated block
        header_t* header = (header_t*)best;
        header->size = bytes;
        header->magic = MAGIC;
        
        return (void*)((char*)header + HEADER_SIZE);
    } else {
        // Use the entire block
        if (best_prev == NULL) {
            head = best->next;
        } else {
            best_prev->next = best->next;
        }
        
        header_t* header = (header_t*)best;
        header->size = best->size;
        header->magic = MAGIC;
        
        return (void*)((char*)header + HEADER_SIZE);
    }
}

// Worst fit algorithm: find the largest block that fits
static void* worst_fit(int bytes) {
    if (head == NULL) {
        init();
        if (head == NULL) return NULL; // Init failed
    }
    
    node_t* current = head;
    node_t* prev = NULL;
    node_t* worst = NULL;
    node_t* worst_prev = NULL;
    int worst_size = -1;
    
    // Find the largest block that fits
    while (current != NULL) {
        if (current->size >= bytes + (int)HEADER_SIZE) {
            if (current->size > worst_size) {
                worst = current;
                worst_prev = prev;
                worst_size = current->size;
            }
        }
        
        prev = current;
        current = current->next;
    }
    
    if (worst == NULL) {
        return NULL; // No suitable block found
    }
    
    // Use the worst block
    if (worst->size >= bytes + (int)HEADER_SIZE + (int)sizeof(node_t)) {
        // Split the block
        node_t* new_node = (node_t*)((char*)worst + bytes + (int)HEADER_SIZE);
        new_node->size = worst->size - bytes - (int)HEADER_SIZE;
        new_node->next = worst->next;
        
        // Update the free list
        if (worst_prev == NULL) {
            head = new_node;
        } else {
            worst_prev->next = new_node;
        }
        
        // Prepare the allocated block
        header_t* header = (header_t*)worst;
        header->size = bytes;
        header->magic = MAGIC;
        
        return (void*)((char*)header + HEADER_SIZE);
    } else {
        // Use the entire block
        if (worst_prev == NULL) {
            head = worst->next;
        } else {
            worst_prev->next = worst->next;
        }
        
        header_t* header = (header_t*)worst;
        header->size = worst->size;
        header->magic = MAGIC;
        
        return (void*)((char*)header + HEADER_SIZE);
    }
}

// Allocate memory using sbrk
static void* sbrk_alloc(int bytes) {
    // Align the size
    size_t aligned_size = ALIGN(bytes + HEADER_SIZE);
    
    // Request memory from the system
    void* mem = sbrk(aligned_size);
    if (mem == (void*)-1) {
        return NULL;
    }
    
    // Set up header
    header_t* header = (header_t*)mem;
    header->size = bytes;
    header->magic = MAGIC;
    
    return (void*)((char*)header + HEADER_SIZE);
}

// Allocate memory using mmap
static void* mmap_alloc(int bytes) {
    // Align the size
    size_t aligned_size = ALIGN(bytes + HEADER_SIZE);
    
    // Request memory from the system
    void* mem = mmap(NULL, aligned_size, PROT_READ | PROT_WRITE, 
                    MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    
    if (mem == MAP_FAILED) {
        return NULL;
    }
    
    // Set up header
    header_t* header = (header_t*)mem;
    header->size = bytes;
    header->magic = MAGIC;
    
    return (void*)((char*)header + HEADER_SIZE);
}

// Main allocation function
void* alloc(int bytes) {
    // Align the size
    bytes = ALIGN(bytes);
    
    void* ptr = NULL;
    
    // Choose the appropriate fitting algorithm
    if (FIT == FIRST_FIT) {
        ptr = first_fit(bytes);
    } else if (FIT == BEST_FIT) {
        ptr = best_fit(bytes);
    } else if (FIT == WORST_FIT) {
        ptr = worst_fit(bytes);
    } else {
        return NULL;
    }
    
    // If no suitable block found in free list, allocate new memory
    if (ptr == NULL) {
        if (bytes <= THRESHOLD) {
            ptr = sbrk_alloc(bytes);
        } else {
            ptr = mmap_alloc(bytes);
        }
    }
    
    return ptr;
}

// Free allocated memory
void myfree(void* ptr) {
    if (ptr == NULL) {
        return;
    }
    
    // Get the header
    header_t* header = (header_t*)((char*)ptr - HEADER_SIZE);
    
    // Verify magic number
    if (header->magic != MAGIC) {
        return; // Not a valid block
    }
    
    // Add to free list
    node_t* block = (node_t*)header;
    block->size = header->size;
    block->next = head;
    head = block;
}


