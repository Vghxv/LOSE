#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define BUFFER_SIZE 10
#define PRODUCER_NUM 5
#define CONSUMER_NUM 5

int buffer[BUFFER_SIZE];
int in = 0;
int out = 0;

sem_t empty, full;
pthread_mutex_t mutex;

void *producer(void *arg) {
    int id = *(int *)arg;
    while (1) {
        sem_wait(&empty);
        pthread_mutex_lock(&mutex);
        buffer[in] = id;
        in = (in + 1) % BUFFER_SIZE;
        printf("Producer %d produce an item\n", id);
        pthread_mutex_unlock(&mutex);
        sem_post(&full);
        sleep(1);
    }
}

void *consumer(void *arg) {
    int id = *(int *)arg;
    while (1) {
        sem_wait(&full);
        pthread_mutex_lock(&mutex);
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        printf("Consumer %d consume an item\n", id);
        pthread_mutex_unlock(&mutex);
        sem_post(&empty);
        sleep(1);
    }
}

int main() {
    pthread_t producer_tid[PRODUCER_NUM], consumer_tid[CONSUMER_NUM];
    int producer_id[PRODUCER_NUM], consumer_id[CONSUMER_NUM];
    sem_init(&empty, 0, BUFFER_SIZE);
    sem_init(&full, 0, 0);
    pthread_mutex_init(&mutex, NULL);

    for (int i = 0; i < PRODUCER_NUM; i++) {
        producer_id[i] = i;
        pthread_create(&producer_tid[i], NULL, producer, &producer_id[i]);
    }
    for (int i = 0; i < CONSUMER_NUM; i++) {
        consumer_id[i] = i;
        pthread_create(&consumer_tid[i], NULL, consumer, &consumer_id[i]);
    }

    for (int i = 0; i < PRODUCER_NUM; i++) {
        pthread_join(producer_tid[i], NULL);
    }
    for (int i = 0; i < CONSUMER_NUM; i++) {
        pthread_join(consumer_tid[i], NULL);
    }

    sem_destroy(&empty);
    sem_destroy(&full);
    pthread_mutex_destroy(&mutex);
    return 0;
}