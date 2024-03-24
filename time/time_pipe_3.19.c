#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        return 1;
    }

    int pipefd[2];
    if (pipe(pipefd) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid;
    pid = fork();

    if (pid == -1) {
        perror("fork");
        return 1;
    } 

    else if (pid == 0) { // Child process
        close(pipefd[0]); // Close read end of pipe
        struct timeval start;
        gettimeofday(&start, NULL); // Record start time
        if (write(pipefd[1], &start, sizeof(struct timeval)) == -1) {
            perror("write");
            return 1;
        }
        if (execvp(argv[1], &argv[1]) == -1) {
            perror("execvp");
            return 1;
        }
    } 
    else { // Parent process
        close(pipefd[1]); // Close write end of pipe
        wait(NULL);
        struct timeval start, end;
        gettimeofday(&end, NULL); // Record end time

        // Read start time from pipe
        if (read(pipefd[0], &start, sizeof(struct timeval)) == -1) {
            perror("read");
            return 1;
        }

        // Calculate elapsed time
        double elapsed = 
            (end.tv_sec - start.tv_sec) + 
            (end.tv_usec - start.tv_usec) / 1000000.0;
        printf("Elapsed time: %.6f second\n", elapsed);

        close(pipefd[0]); // Close read end of pipe
    }

    return 0;
}
