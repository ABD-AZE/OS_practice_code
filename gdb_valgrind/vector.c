// vector implementation using arrays in if c 
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct{
  void* data;
  int length;
  int datasize;
  int capacity;
} myvector;

void init_vector(myvector* v,int datasize){
  v->length=0;
  v->capacity = 4;
  v-> datasize = datasize;
  v->data = malloc(datasize*(v->capacity));
}

void push(myvector* v,void* element){
  if((v->length + 1) > v->capacity ){
    v->data = realloc((v->data), 2*(v->length)*(v->datasize));
    v->capacity *= 2;
  }
  else{
    memcpy((char*)v->data + (v->length)*(v->datasize),element,v->datasize); 
  }
  v->length++;
}

void* get(myvector* v, int pos){
  if(v->length<pos){
    return NULL;
  } 
  return ((char*)v->data + pos*(v->datasize));
}

// return values:
// -1 -> length of vector is 0
// 0 -> successful allocation
int pop(myvector* v){
  if(v->length == 0){
    return -1;
  }
  (v->length)--;
  return 0;
}


int main(int argc, char* argv[]){
  myvector vec;
  myvector* v = &vec;
  // int vector
  init_vector(v,4);
  int* x = (int *)malloc(4);
  *x = 10;
  push(v,x);
  *x = 1342;
  push(v,x);
  for(int i=0;i<v->length;i++){
    printf("v[%d]: %d",i,*(int*)get(v,i));
  }
  int p = pop(v);
  if(p==-1){
    printf("cannot pop length of vector is 0");
  }
  printf("v[0]: %d",*(int*)get(v,0));
  free(x);
  free(v->data);
  return 0;
}
