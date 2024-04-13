#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#define MAX_RESOUCRES 5

int available_resources = MAX_RESOUCRES;

int decrease_count(int count) {
    static pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_lock(&mutex);
    if (available_resources < count){
        pthread_mutex_unlock(&mutex);
        printf("Not enough resources\n");
        return -1;
    }
    else {
        available_resources -= count;
        pthread_mutex_unlock(&mutex);
        printf("Decrease count by %d, available resources: %d\n", count, available_resources);
        return 0;
    }
}

int increase_count(int count) {
    available_resources += count;
    printf("Increase count by %d, available resources: %d\n", count, available_resources);
    return 0;
}

void *runner(void *param) {
    int count = *(int*)param;
    decrease_count(count);
    pthread_exit(0);
}

int main() {
    int count = 3;
    pthread_t tid1, tid2;
    
    pthread_create(&tid1, NULL, runner, &count);
    pthread_create(&tid2, NULL, runner, &count);

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);

    printf("Available resources: %d\n", available_resources);
    return 0;
}