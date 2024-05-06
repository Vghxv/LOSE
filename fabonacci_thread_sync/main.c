#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <threads.h>
#include <time.h>
#include <semaphore.h>

struct ThreadData_t {
    int n1;
    int n2;
} ThreadData_t;

unsigned long shm[95] = {0};
sem_t sem;

unsigned long fast_fibonacci(int n) {
    if (n < 1){
        return 0;
    }
    if (n < 3){
        return 1;
    }
    if (shm[n] != 0){
        return shm[n];
    }
    if (n % 2 == 0){
        unsigned long k = n >> 1;
        unsigned long a = fast_fibonacci(k);
        unsigned long b = fast_fibonacci(k + 1);
        return a * ((b << 1) - a);
    }
    else{
        unsigned long k = (n - 1) >> 1;
        unsigned long a = fast_fibonacci(k + 1);
        unsigned long b = fast_fibonacci(k);
        return a * a + b * b;
    }
}

void *runner(void *param) {
    int n1 = ((struct ThreadData_t *)param)->n1;
    int n2 = ((struct ThreadData_t *)param)->n2;
    for (int i = n1; i <= n2; i++) {
        shm[i] = fast_fibonacci(i);
        sem_post(&sem);
    }
    pthread_exit(0);
}

int main(int argc, char *argv[]) {
    int n = 0;
    if (argc > 1) {
        n = atoi(argv[1]);
    }
    else{
        printf("Usage: %s <n>\n", argv[0]);
        return 1;
    }
    sem_init(&sem, 0 , 0);
    pthread_t tid;
    struct ThreadData_t param = {0, n};
    pthread_create(&tid, NULL, runner, &param);

    for (int i = 0; i <= n; i++) {
        sem_wait(&sem);
        printf("Fibonacci(%d) = %lu\n", i, shm[i]);
    }
    return 0;
}