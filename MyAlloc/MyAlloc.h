#include<stdlib.h>

#ifndef MYALLOC_H
#define MYALLOC_H

// 128KB
#define THRESHOLD 128*1024
// default is FIRST FIT
#define FIT FIRST_FIT
#define FIRST_FIT 0
#define BEST_FIT 1
#define WORST_FIT 2

typedef struct __header_t{
  size_t size; 
  int magic;
} header_t;

typedef struct __node_t{
  int size;
  node_t* next;
} node_t;

node_t* head = NULL; 
// allocates `bytes` bytes and returns void* ptr
void* alloc(int bytes);
// would free ptr if it is the starting address of an allocated block (checked using magic bytes)
void free(void* ptr);

// if out_of_memory && bytes <= THRESHOLD alloc calls sbrk_alloc internally 
static void* sbrk_alloc(bytes);
// if out_of_memory && bytes >= THRESHOLD alloc calls mmap_alloc internally 
static void* mmap_alloc(bytes);
// on the first allocation check if head==NULL and run init to acquire memory through mmap for the heap 
static void init();
static void* first_fit(int bytes);
static void* best_fit(int bytes);
static void* worst_fit(int bytes);

#endif
