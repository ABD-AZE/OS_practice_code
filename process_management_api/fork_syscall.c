#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<fcntl.h>
#include<sys/wait.h>
int y = 200;

int main(int argc, char *argv[]) {
    int x = 100;
    int fd;
    fd = open("test.txt", 1);

    int pid = fork();
    if (pid == 0) {
        y=10;
        printf("Child: x = %d\n", x);
        printf("Child: y = %d\n", y);
        printf("Child: fd = %d\n", fd);
        write(fd, "World", 5);
        // kill(getpid(), SIGKILL); // uncomment this line to kill the child process and see the returned status in the parent process 
        printf("Hello");
    } else {
        y=20;
        printf("Parent: x = %d\n", x);
        printf("Parent: y = %d\n", y);
        printf("Parent: fd = %d\n", fd);
        write(fd, "Hello", 5);
        void *status = malloc(sizeof(int));
        wait(status); // if there are multiple processes running then the parent will wait for any of the child to finish
        printf("Status of child: %d\n", *(int *)status);
        printf("World");
    }
    close(fd);
    return 0;
}