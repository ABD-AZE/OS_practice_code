/*
---------------------------------------------------------
MyAlloc: Reimplementation of malloc
CONTRIBUTORS: Abdullah Azeem <abdullahazeem770@gmail.com>
---------------------------------------------------------

Functionalities:
 - Coalescing
 - Allocation using sbrk for smaller requests and mmap for larger requests
 - exploring various fitting algorithms (first fit, best fit, worst fit, binning strategies...)
 - configurable fitting algorithm
 */

#include "MyAlloc.h"

void* alloc(int bytes){
  if(FIT == FIRST_FIT ){
    void* ptr = first_fit(bytes);
  } else if(FIT == BEST_FIT){
    void* ptr = best_fit(bytes);
  } else if(FIT == WORST_FIT){
    void* ptr = worst_fit(bytes);
  } else{
    return NULL;
  }
  if (ptr == NULL){
    if(bytes <= THRESHOLD){
      void* ptr = sbrk_alloc(bytes);
    }
  }
}


