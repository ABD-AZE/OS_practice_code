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
  struct __node_t* next;
} node_t;

extern node_t* head; 
// allocates `bytes` bytes and returns void* ptr
void* alloc(int bytes);
// would free ptr if it is the starting address ojf an allocated block (checked using magic bytes)
void myfree(void* ptr);

// No static function declarations here - they belong only in the .c file

#endif
