#include <stdio.h>
#include <stdlib.h>

int main(){
  int* data = (int *) malloc(sizeof(int)*100);
  int* middle = data+50;
  // cannot pass an offset pointer to free, free only accepts pointers returned by malloc, calloc etc. 
  free(middle);
  data[0]=1000;
  data[100]=123;
  data[51]=12;
  printf("data[51] = %d\n",data[51]);
  printf("data[0] = %d\n",data[0]);
}
