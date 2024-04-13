#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

int num_points_circle = 0;
void *runner(void *param) {
    int n = *(int*)param;
    for (int i = 0; i < n; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1) {
            num_points_circle++;
        }
    }
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage: %s <number of points>\n", argv[0]);
        return 1;
    }
    int n = atoi(argv[1]);
    pthread_t tid;
    pthread_create(&tid, NULL, runner, &n);
    pthread_join(tid, NULL);
    double pi = 4 * (double)num_points_circle / n;
    printf("PI = %f\n", pi);
    return 0;
}