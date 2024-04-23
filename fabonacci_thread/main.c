#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <threads.h>
#include <time.h>


struct ThreadData_t {
    int n1;
    int n2;
    // a function pointer
    unsigned long (*cal_func)(int);
} ThreadData_t;

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
    return traditional_fibonacci(n - 1) + traditional_fibonacci(n - 2);
}

void *runner(void *param) {
    int n1 = ((struct ThreadData_t *)param)->n1;
    int n2 = ((struct ThreadData_t *)param)->n2;
    unsigned long (*func)(int) = ((struct ThreadData_t *)param)->cal_func;
    for (int i = n1; i <= n2; i++) {
        shm[i] = func(i);
    }
    pthread_exit(0);
}

void single_thread(int n, unsigned long (*func)(int)) {
    pthread_t tid;
    struct ThreadData_t param = {0, n, func};
    pthread_create(&tid, NULL, runner, &param);
    pthread_join(tid, NULL);
}

void double_thread(int n, unsigned long (*func)(int)) {
    pthread_t tid1, tid2;
    struct ThreadData_t param1 = {0, n / 2, func};
    struct ThreadData_t param2 = {n / 2 + 1, n, func};
    pthread_create(&tid1, NULL, runner, &param1);
    pthread_create(&tid2, NULL, runner, &param2);
    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
}

void triple_thread(int n, unsigned long (*func)(int)) {
    pthread_t tid1, tid2, tid3;
    struct ThreadData_t param1 = {0, n / 3, func};
    struct ThreadData_t param2 = {n / 3 + 1, n / 3 * 2, func};
    struct ThreadData_t param3 = {n / 3 * 2 + 1, n, func};
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
    
    printf("Select the number of threads:\n");
    printf("1. Single thread\n");
    printf("2. Double thread\n");
    printf("3. Triple thread\n");
    int choice = 0;
    if (scanf("%d", &choice) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    printf("Select the calculation method:\n");
    printf("1. Traditional\n");
    printf("2. Fast\n");
    int method = 0;
    if (scanf("%d", &method) != 1) {
        printf("Invalid input\n");
        return 1;
    }
    switch (choice) {
        case 1:
            if (method == 1){
                single_thread(n, traditional_fibonacci);
            }
            else{
                single_thread(n, fast_fibonacci);
            }
            break;
        case 2:
            if (method == 1){
                double_thread(n, traditional_fibonacci);
            }
            else{
                double_thread(n, fast_fibonacci);
            }
            break;
        case 3:
            if (method == 1){
                triple_thread(n, traditional_fibonacci);
            }
            else{
                triple_thread(n, fast_fibonacci);
            }
            break;
        default:
            printf("Invalid choice\n");
            return 1;
    }
    

    end_time = clock();
    elapsed_time = (double)(end_time - start_time) / CLOCKS_PER_SEC;
    for (int i = 0; i <= n; i++) {
        printf("Fibonacci(%d) = %lu\n", i, shm[i]);
    }
    printf("Elapsed time: %fs\n", elapsed_time);
    return 0;
}