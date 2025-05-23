#include <stdio.h>
#include<stdlib.h>

int main(){
  int* data;
  data = (int*)malloc(sizeof(int)*100);
  for(int i=0;i<100;i++){
    data[i]=i;
  }
  data[100]=0;
  free(data);
  return 0;
}
