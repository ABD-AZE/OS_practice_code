#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<sys/wait.h>

int main(int argc, char *argv[]) {
    int pid = fork();
    if (pid == 0) {
        close(STDOUT_FILENO);
        int * st = malloc(sizeof(int));
        int x = wait(st); 
        printf("return status of wait: %d\n", x);
        printf("Status of child: %d\n", *st);
        int status = execl("/bin/ls", "ls","-a", NULL);
        printf("Child\n");
    } else {
        printf("Parent\n");
    }
    return 0;
}