#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define SHM_SIZE  sizeof(struct timeval)

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <command>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Shared memory variables
    key_t key = IPC_PRIVATE;
    int shmid;
    void *shmaddr;

    // Create shared memory segment
    if ((shmid = shmget(key, SHM_SIZE, IPC_CREAT | 0666)) < 0) {
        perror("shmget");
        exit(EXIT_FAILURE);
    }

    // Attach shared memory segment
    if ((shmaddr = shmat(shmid, NULL, 0)) == (void *) -1) {
        perror("shmat");
        exit(EXIT_FAILURE);
    }

    struct timeval *start_time = (struct timeval *)shmaddr;

    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } 
    else if (pid == 0) {  // Child process
        gettimeofday(start_time, NULL);
        execvp(argv[1], &argv[1]);
        perror("exec");
        exit(EXIT_FAILURE);
    } 
    else {  // Parent process
        int status;
        waitpid(pid, &status, 0);
        if (!WIFEXITED(status)) {
            fprintf(stderr, "Child process terminated abnormally.\n");
            exit(EXIT_FAILURE);
        }

        struct timeval end_time;
        gettimeofday(&end_time, NULL);

        double elapsed_time = 
            (end_time.tv_sec - start_time->tv_sec) + 
            (end_time.tv_usec - start_time->tv_usec) / 1000000.0;
        printf("Elapsed time: %.6f seconds\n", elapsed_time);

        // Detach and remove shared memory segment
        shmdt(shmaddr);
        shmctl(shmid, IPC_RMID, NULL);
    }

    return 0;
}
