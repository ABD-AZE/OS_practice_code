#define _POSIX_C_SOURCE 199309L
#define _GNU_SOURCE
#include<stdio.h>
#include<sys/wait.h>
#include<unistd.h>
#include<time.h>
#include<sched.h>

// important inference: average context switch time when using sched_setaffinity is less than when not using it (stats 4.3k ns vs 6.8k ns)
int main(int argc,char *argv[]) {
    // make two child processes 
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(0,&mask);
    sched_setaffinity(0,sizeof(cpu_set_t),&mask);
    int bufferfd1[2], bufferfd2[2];
    struct timespec start,end;
    int counter = 0;
    if(pipe(bufferfd1) < 0) {
        printf("Pipe creation failed\n");
        return 1;
    }
    if(pipe(bufferfd2) < 0) {
        printf("Pipe creation failed\n");
        return 1;
    }
    int child1 = fork();
    if (child1 == 0) {
        printf("Child 1\n");
        char byte[1];
        clock_gettime(CLOCK_MONOTONIC,&start);
        while(counter<1000000){
            write(bufferfd1[1], "0",1);
            read(bufferfd2[0], byte,1);
            counter++;
        }
        clock_gettime(CLOCK_MONOTONIC,&end);
        printf("Time for context switch: %lfns\n", ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec))/(2000000.0));
    } else {
        int child2 = fork();
        if (child2 == 0) {
            printf("Child 2\n");
            char byte[1];
            clock_gettime(CLOCK_MONOTONIC,&start);
            while(counter<1000000){
                read(bufferfd1[0], byte,1);
                write(bufferfd2[1], "1",1);
                counter++;
            }
            clock_gettime(CLOCK_MONOTONIC,&end);
            printf("Time for context switch: %lfns\n", ((end.tv_sec - start.tv_sec) * 1e9 + (end.tv_nsec - start.tv_nsec))/(2000000.0));
        } else {
        printf("Parent\n");
        wait(NULL);
        wait(NULL);
        }
    return 0;
    }
}