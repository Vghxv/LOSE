#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/wait.h>

#define BUFSIZE 4096

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <source_file> <destination_file>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int pipefd[2];
    pid_t pid;
    
    if (pipe(pipefd) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    }

    if (pid == 0) { // Child process
        close(pipefd[1]); // Close write end of the pipe
        
        int dest_fd = open(argv[2], O_WRONLY | O_CREAT | O_TRUNC, 0644);
        if (dest_fd == -1) {
            perror("Error opening destination file");
            close(pipefd[0]);
            exit(EXIT_FAILURE);
        }

        char buffer[BUFSIZE];
        ssize_t bytes_read;
        
        while ((bytes_read = read(pipefd[0], buffer, BUFSIZE)) > 0) {
            if (write(dest_fd, buffer, bytes_read) != bytes_read) {
                perror("Error writing to destination file");
                close(pipefd[0]);
                close(dest_fd);
                exit(EXIT_FAILURE);
            }

        }
        
        close(pipefd[0]);
        close(dest_fd);
        
        if (bytes_read == -1) {
            perror("Error reading from pipe");
            exit(EXIT_FAILURE);
        }
        
        exit(EXIT_SUCCESS);
    } 
    else {
        close(pipefd[0]); // Close read end of the pipe

        int source_fd = open(argv[1], O_RDONLY);
        if (source_fd == -1) {
            perror("Error opening source file");
            close(pipefd[1]);
            exit(EXIT_FAILURE);
        }

        char buffer[BUFSIZE];
        ssize_t bytes_read;

        while ((bytes_read = read(source_fd, buffer, BUFSIZE)) > 0) {
            if (write(pipefd[1], buffer, bytes_read) != bytes_read) {
                perror("Error writing to pipe");
                close(pipefd[1]); // Close write end of the pipe
                close(source_fd);
                exit(EXIT_FAILURE);
            }
        }
        
        if (bytes_read == -1) {
            perror("Error reading from source file");
            close(pipefd[1]); // Close write end of the pipe
            close(source_fd);
            exit(EXIT_FAILURE);
        }

        close(pipefd[1]); // Close write end of the pipe
        close(source_fd);

        // Wait for the child process to finish
        wait(NULL);
        exit(EXIT_SUCCESS);
    }

    return 0;
}
