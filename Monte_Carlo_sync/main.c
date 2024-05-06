#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>
#define THREADS_NUM 20
int in_circle = 0;
int out_of_circle = 0;

pthread_mutex_t mutex;

void *runner(void *param) {
    int n = rand() % 1000;
    int local_in_circle = 0;
    for (int i = 0; i < n; i++) {
        double x = (double)rand() / RAND_MAX;
        double y = (double)rand() / RAND_MAX;
        if (x * x + y * y <= 1) {
            local_in_circle++;
        }
    }
    int local_out_of_circle = n - local_in_circle;
    pthread_mutex_lock(&mutex);
    in_circle += local_in_circle;
    out_of_circle += local_out_of_circle;
    printf("PI = %f, in_circle = %d, out_of_circle = %d\n", (4 * (double)in_circle) / (out_of_circle + in_circle), in_circle, out_of_circle);
    pthread_mutex_unlock(&mutex);    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {

    pthread_t tid[THREADS_NUM];
    pthread_mutex_init(&mutex, NULL);    
    for (int i = 0; i < THREADS_NUM; i++) {
        pthread_create(&tid[i], NULL, runner, NULL);
    }
    for (int i = 0; i < THREADS_NUM; i++) {
        pthread_join(tid[i], NULL);
    }

    return 0;
}
