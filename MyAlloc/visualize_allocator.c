/*
---------------------------------------------------------
MyAlloc Advanced Visualizer: Detailed visualization of memory allocator
---------------------------------------------------------
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "MyAlloc.h"

#define MAX_BLOCKS 20
#define VISUALIZATION_WIDTH 60

// Structure to track allocated blocks
typedef struct {
    void* ptr;
    int size;
    int is_free;
    char label[20];
} block_info_t;

// Global tracking
block_info_t blocks[MAX_BLOCKS];
int block_count = 0;
int total_allocated = 0;
int total_freed = 0;
int allocation_count = 0;
int free_count = 0;

// Function to visualize the free list with addresses
void visualize_free_list() {
    if (head == NULL) {
        printf("Free list is empty\n");
        return;
    }
    
    printf("\n=== FREE LIST ===\n");
    node_t* current = head;
    int block_num = 1;
    
    while (current != NULL) {
        printf("Block %d: Address=%p, Size=%d bytes", 
               block_num++, (void*)current, current->size);
        
        if (current->next != NULL) {
            printf(" → Next=%p\n", (void*)current->next);
        } else {
            printf(" → NULL\n");
        }
        
        current = current->next;
    }
    printf("\n");
}

// Function to print a detailed memory map
void print_memory_map() {
    printf("\n=== MEMORY MAP ===\n");
    printf("┌");
    for (int i = 0; i < VISUALIZATION_WIDTH; i++) printf("─");
    printf("┐\n");
    
    // Calculate total memory for visualization
    int total_memory = 0;
    for (int i = 0; i < block_count; i++) {
        total_memory += blocks[i].size;
    }
    
    // Draw blocks
    for (int i = 0; i < block_count; i++) {
        int block_width = (blocks[i].size * VISUALIZATION_WIDTH) / total_memory;
        if (block_width < 3) block_width = 3;
        
        printf("│");
        if (blocks[i].is_free) {
            // Free block
            for (int j = 0; j < block_width; j++) {
                printf(" ");
            }
        } else {
            // Allocated block
            for (int j = 0; j < block_width; j++) {
                if (j == block_width/2) {
                    printf("%d", i+1);
                } else {
                    printf("█");
                }
            }
        }
        
        if (i == block_count - 1) {
            // Fill the rest of the line
            for (int j = block_width; j < VISUALIZATION_WIDTH; j++) {
                printf(" ");
            }
        }
    }
    printf("│\n");
    
    printf("└");
    for (int i = 0; i < VISUALIZATION_WIDTH; i++) printf("─");
    printf("┘\n");
    
    // Print block details
    printf("\n=== BLOCK DETAILS ===\n");
    for (int i = 0; i < block_count; i++) {
        printf("Block %d: %s, Address=%p, Size=%d bytes\n", 
               i+1, 
               blocks[i].is_free ? "FREE" : blocks[i].label, 
               blocks[i].ptr,
               blocks[i].size);
    }
}

// Function to print statistics
void print_statistics() {
    printf("\n=== STATISTICS ===\n");
    printf("Total blocks tracked: %d\n", block_count);
    printf("Total memory allocated: %d bytes\n", total_allocated);
    printf("Total memory freed: %d bytes\n", total_freed);
    printf("Current memory usage: %d bytes\n", total_allocated - total_freed);
    printf("Allocation operations: %d\n", allocation_count);
    printf("Free operations: %d\n", free_count);
    
    // Calculate fragmentation
    int free_blocks = 0;
    int free_memory = 0;
    for (int i = 0; i < block_count; i++) {
        if (blocks[i].is_free) {
            free_blocks++;
            free_memory += blocks[i].size;
        }
    }
    
    printf("Free blocks: %d\n", free_blocks);
    printf("Free memory: %d bytes\n", free_memory);
    if (free_blocks > 0) {
        printf("Average free block size: %d bytes\n", free_memory / free_blocks);
        printf("Fragmentation: %.2f%%\n", 
               (free_blocks > 1) ? ((free_blocks - 1) * 100.0 / free_blocks) : 0.0);
    }
}

// Function to simulate allocation
void simulate_allocation() {
    if (block_count >= MAX_BLOCKS) {
        printf("Maximum number of blocks reached\n");
        return;
    }
    
    char label[20];
    int size;
    
    printf("Enter block label: ");
    scanf("%s", label);
    
    printf("Enter size in bytes: ");
    scanf("%d", &size);
    
    void* ptr = alloc(size);
    if (ptr != NULL) {
        blocks[block_count].ptr = ptr;
        blocks[block_count].size = size;
        blocks[block_count].is_free = 0;
        strncpy(blocks[block_count].label, label, sizeof(blocks[block_count].label) - 1);
        blocks[block_count].label[sizeof(blocks[block_count].label) - 1] = '\0';
        
        block_count++;
        total_allocated += size;
        allocation_count++;
        
        printf("Allocated block of %d bytes at address %p\n", size, ptr);
    } else {
        printf("Allocation failed\n");
    }
}

// Function to simulate freeing
void simulate_free() {
    int index;
    
    printf("Enter block index to free (1-%d): ", block_count);
    scanf("%d", &index);
    
    if (index < 1 || index > block_count) {
        printf("Invalid block index\n");
        return;
    }
    
    index--; // Convert to 0-based index
    
    if (blocks[index].is_free) {
        printf("Block %d is already free\n", index + 1);
        return;
    }
    
    printf("Freeing block %d (%s) at address %p\n", 
           index + 1, blocks[index].label, blocks[index].ptr);
    
    myfree(blocks[index].ptr);
    blocks[index].is_free = 1;
    total_freed += blocks[index].size;
    free_count++;
}

// Main function with interactive simulation
int main() {
    printf("MyAlloc Advanced Memory Allocator Visualization\n");
    printf("==============================================\n\n");
    
    char command[20];
    
    while (1) {
        printf("\nCommands: allocate, free, view, stats, quit\n");
        printf("Enter command: ");
        scanf("%s", command);
        
        if (strcmp(command, "allocate") == 0) {
            simulate_allocation();
            visualize_free_list();
            
        } else if (strcmp(command, "free") == 0) {
            simulate_free();
            visualize_free_list();
            
        } else if (strcmp(command, "view") == 0) {
            print_memory_map();
            visualize_free_list();
            
        } else if (strcmp(command, "stats") == 0) {
            print_statistics();
            
        } else if (strcmp(command, "quit") == 0) {
            break;

        } else {
            printf("Unknown command\n");
        }
    }
    
    // Free any remaining blocks before exiting
    for (int i = 0; i < block_count; i++) {
        if (!blocks[i].is_free) {
            myfree(blocks[i].ptr);
        }
    }
    
    printf("Exiting...\n");
    return 0;
} 