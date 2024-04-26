#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
    pthread_t thread_id;
} thread_t;

typedef struct {
    void (*function)(void *);
    void *arg;
} task_t;

typedef struct {
    int thread_count;
    thread_t *threads;
    task_t *task_queue; 
    int queue_size;
    int head;
    int tail;
    pthread_mutex_t lock;
    pthread_cond_t task_available;
} thread_pool_t;

void *thread_function(void *arg);

thread_pool_t *thread_pool_init(int thread_count, int queue_size) {
    thread_pool_t *pool = malloc(sizeof(thread_pool_t));
    
    pool->thread_count = thread_count;
    pool->threads = malloc(thread_count * sizeof(thread_t));
    
    pool->queue_size = queue_size;
    pool->task_queue = malloc(queue_size * sizeof(task_t));
    
    pool->head = pool->tail = 0;
    pthread_mutex_init(&pool->lock, NULL);
    pthread_cond_init(&pool->task_available, NULL);
    
    for (int i = 0; i < thread_count; i++) {
        pthread_create(&pool->threads[i].thread_id, NULL, thread_function, pool);
    }
    
    return pool;
}

void thread_pool_submit(thread_pool_t *pool, void (*function)(void *), void *arg) {
    pthread_mutex_lock(&pool->lock);
    // below line illustrates that the queue is full
    while ((pool->tail + 1) % pool->queue_size == pool->head) {
        pthread_cond_wait(&pool->task_available, &pool->lock);
    }
    // put a task in the task_queue pointer, update the tail, mod with queue_size to make it circular 
    pool->task_queue[pool->tail].function = function;
    pool->task_queue[pool->tail].arg = arg;
    pool->tail = (pool->tail + 1) % pool->queue_size;
    pthread_cond_signal(&pool->task_available);
    pthread_mutex_unlock(&pool->lock);
}

void *thread_function(void *arg) {
    thread_pool_t *pool = (thread_pool_t *)arg;
    while (1) {
        pthread_mutex_lock(&pool->lock);
        // below line illustrates that the queue is empty   
        while (pool->head == pool->tail) {
            pthread_cond_wait(&pool->task_available, &pool->lock);
        }
        // get a task from task_queue pointer, update the head, mod with queue_size to make it circular
        task_t task = pool->task_queue[pool->head];
        pool->head = (pool->head + 1) % pool->queue_size;
        pthread_cond_signal(&pool->task_available);
        pthread_mutex_unlock(&pool->lock);
        task.function(task.arg);
    }
    return NULL;
}

void thread_pool_shutdown(thread_pool_t *pool) {
    for (int i = 0; i < pool->thread_count; i++) {
        printf("Cancelling thread %ld\n", pool->threads[i].thread_id);
        pthread_cancel(pool->threads[i].thread_id);
    }
    free(pool->threads);
    free(pool->task_queue);
    pthread_mutex_destroy(&pool->lock);
    pthread_cond_destroy(&pool->task_available);
    free(pool);
}

void task_function(void *arg) {
    int *num = (int *)arg;
    printf("Task executed with argument: %d\n", *num);
}

int main() {

    thread_pool_t *pool = thread_pool_init(5, 10);
    
    for (int i = 0; i < 20; i++) {
        int *num = malloc(sizeof(int));
        *num = i;
        thread_pool_submit(pool, task_function, num);
    }

    sleep(5);

    thread_pool_shutdown(pool);
    
    return 0;
}