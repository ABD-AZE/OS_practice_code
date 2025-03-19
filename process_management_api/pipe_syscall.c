#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

int main() {
	int pipe_fd[2];
	pid_t child1, child2;
	int status;
	// Create a pipe
	if (pipe(pipe_fd) == -1) {
		perror("pipe");
		exit(EXIT_FAILURE);
	}

	// Create first child process
	if ((child1 = fork()) == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (child1 == 0) { // First child process
		// Close read end of pipe
		close(pipe_fd[0]);
		
		// Redirect stdout to pipe write end
		dup2(pipe_fd[1], STDOUT_FILENO);
		close(pipe_fd[1]);

		// Execute command that generates output
		execlp("ls", "ls", "-l", NULL);
		
		// If execlp fails
		perror("execlp in child1");
		exit(EXIT_FAILURE);
	}

	// Create second child process (from parent)
	if ((child2 = fork()) == -1) {
		perror("fork");
		exit(EXIT_FAILURE);
	}

	if (child2 == 0) { // Second child process
		// Close write end of pipe
		close(pipe_fd[1]);
		
		// Redirect stdin to pipe read end
		dup2(pipe_fd[0], STDIN_FILENO);
		close(pipe_fd[0]);

		// Execute command that reads from stdin
		execlp("wc", "wc", "-l", NULL);
		
		// If execlp fails
		perror("execlp in child2");
		exit(EXIT_FAILURE);
	}

	// Parent process
	// Close both ends of the pipe
	close(pipe_fd[0]);
	close(pipe_fd[1]);

	// Wait for both children to finish
	waitpid(child1, &status, 0);
	waitpid(child2, &status, 0);

	printf("Both children have completed their execution\n");

	return 0;
}
