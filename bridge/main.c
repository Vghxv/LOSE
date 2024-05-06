#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define FARMER_NUM 5
pthread_mutex_t bridge;

void cross_bridge(const char* direction, int sleep_time) {
    pthread_mutex_lock(&bridge);
    printf("Crossing the bridge %s\n", direction);
    sleep(sleep_time); 
    printf("Crossed the bridge %s\n", direction);
    pthread_mutex_unlock(&bridge);

}

void* northbound_farmer(void* arg) {
    int sleep_time = rand() % 5;
    cross_bridge("from North Tunbridge to South Tunbridge", sleep_time);
    return NULL;
}

void* southbound_farmer(void* arg) {
    int sleep_time = rand() % 5;
    cross_bridge("from South Tunbridge to North Tunbridge", sleep_time);
    return NULL;
}

int main() {
    pthread_mutex_init(&bridge, NULL);
    pthread_t northbound_farmer_thread[FARMER_NUM], southbound_farmer_thread[FARMER_NUM];
    for (int i = 0; i < FARMER_NUM; i++) {
        pthread_create(&northbound_farmer_thread[i], NULL, northbound_farmer, NULL);
        pthread_create(&southbound_farmer_thread[i], NULL, southbound_farmer, NULL);
    }
    for (int i = 0; i < FARMER_NUM; i++) {
        pthread_join(northbound_farmer_thread[i], NULL);
        pthread_join(southbound_farmer_thread[i], NULL);
    }
    return 0;
}
