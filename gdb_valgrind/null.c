#include <stdio.h>
#include<stdlib.h>

// 4 bytes allocated on heap
int main(int argc,char* argv[]){
  int *ptr = NULL;
  ptr  = malloc(sizeof(int));
  *ptr = 10;
  // with printf extra allocation takes place
  // printf("dereferencing the pointer: %d",*ptr);
  free(ptr);
  return 0;
}
