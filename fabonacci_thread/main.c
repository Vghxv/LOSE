#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <time.h>

unsigned long shm[95] = {0};
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

unsigned long traditional_fibonacci(int n) {
    if (n < 1){
        return 0;
    }
    if (n < 3){
        return 1;
    }
    return fibonacci(n - 1) + fibonacci(n - 2);
}

void *runner(void *param) {
    int n1 = *(int*)param;
    int n2 = *((int*)param + 1);
    for (int i = n1; i <= n2; i++) {
        shm[i] = fast_fibonacci(i);
    }
    pthread_exit(0);
}

void single_thread(int n) {
    pthread_t tid;
    int param[2] = {0, n};
    pthread_create(&tid, NULL, runner, &param);
    pthread_join(tid, NULL);
}

void double_thread(int n) {
    pthread_t tid1, tid2;
    int param1[2] = {0, n / 2};
    int param2[2] = {n / 2 + 1, n};
    pthread_create(&tid1, NULL, runner, &param1);
    pthread_create(&tid2, NULL, runner, &param2);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
}

void triple_thread(int n) {
    pthread_t tid1, tid2, tid3;
    int param1[2] = {0, n / 3};
    int param2[2] = {n / 3 + 1, n * 2 / 3};
    int param3[2] = {n * 2 / 3 + 1, n};
    pthread_create(&tid1, NULL, runner, &param1);
    pthread_create(&tid2, NULL, runner, &param2);
    pthread_create(&tid3, NULL, runner, &param3);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);
}

int main(int argc, char *argv[]) {
    clock_t start_time, end_time;
    double elapsed_time;
    start_time = clock();

    int n = 0;
    if (argc > 1) {
        n = atoi(argv[1]);
    }
    else{
        printf("Usage: %s <n>\n", argv[0]);
        return 1;
    }
    
    single_thread(n);
    // double_thread(n);
    // triple_thread(n);

    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    for (int i = 0; i <= n; i++) {
        printf("Fibonacci(%d) = %lu\n", i, shm[i]);
    }
    printf("Elapsed time: %fs\n", elapsed_time);
    return 0;
}