#include<stdio.h>
#include<stdlib.h>
#include <signal.h>
        
char *arr; // Make arr accessible to the signal handler

void sigint_handler(int sig) {
    printf("\nCaught signal %d. Freeing memory and exiting...\n", sig);
    free(arr);
    exit(0);
}
// inference: for relatively smaller allocations (around 2GB), the result is as expected, but for larger allocations (around 6GB) the expected memory is not allocated (paging occurs)
int main(int argc, char *argv[]) {
    signal(SIGINT, sigint_handler);
    if (argc > 1){
        int memory = atoi(argv[1]);
        printf("Memory: %d\n", memory);
        arr = (char *)malloc(memory*(1024*1024));
        if (arr == NULL){
            printf("Memory allocation failed\n");
            return 1;
        }
        printf("Memory allocated\n");
        while(1){
            for(int i = 0; i < memory*(1024*1024); i++){
                arr[i] = (i-1)%128;
            }
            for(int i = 0; i < memory*(1024*1024); i++){
                arr[i] = (i+1)%128;
            }
        }
        
        // Continue with your original code
        free(arr);
    }
    return 0;
}