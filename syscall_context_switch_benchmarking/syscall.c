#include<stdio.h>
#include<unistd.h>
#include<fcntl.h>
#include <linux/time.h>

int main(int argc, char* argv[]){
    struct timespec start,end; 
    clock_gettime(CLOCK_MONOTONIC,&start);
    int fd = open("temp.txt",0);
    if (fd == -1) {
        perror("Error opening file");
        return 1;
    }
    int counter = 0;
    while(counter<1000000){
        char buffer[1];
        int byte_read = read(fd,buffer,1);
        if (byte_read == 0) {
            break;
        }
        counter++;
    }
    clock_gettime(CLOCK_MONOTONIC,&end);
    printf("Time for read syscall: %lfns\n", ((end.tv_sec - start.tv_sec) * 1e9 + 
    (end.tv_nsec - start.tv_nsec))/(1000000.0));
    close(fd);

    clock_gettime(CLOCK_MONOTONIC,&start);
    while(counter>0){
        pid_t p = getpid();
        counter--;
    }
    clock_gettime(CLOCK_MONOTONIC,&end);
    printf("Time for getpid syscall: %lfns\n", ((end.tv_sec - start.tv_sec) * 1e9 +
    (end.tv_nsec - start.tv_nsec))/(1000000.0));

    // write syscall
    fd = open("temp.txt", O_WRONLY | O_APPEND);
    char buffer[1500] = "Hello world Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.Hello world Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.Hello world Lorem ipsum dolor sit amet, consectetur adipiscing elit, sed do eiusmod tempor incididunt ut labore et dolore magna aliqua. Ut enim ad minim veniam, quis nostrud exercitation ullamco laboris nisi ut aliquip ex ea commodo consequat. Duis aute irure dolor in reprehenderit in voluptate velit esse cillum dolore eu fugiat nulla pariatur. Excepteur sint occaecat cupidatat non proident, sunt in culpa qui officia deserunt mollit anim id est laborum.";
    clock_gettime(CLOCK_MONOTONIC,&start);
    while(counter<1000){
        int x = write(fd,buffer+counter,1);
        counter++;
    }
    clock_gettime(CLOCK_MONOTONIC,&end);
    printf("Time for write syscall: %lfns\n", ((end.tv_sec - start.tv_sec) * 1e9 +
    (end.tv_nsec - start.tv_nsec))/(1000.0));
    close(fd);
    return 0;
}