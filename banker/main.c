#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdbool.h>
#define RESOURCES 4
#define MAX_CUSTOMER 5
int available[RESOURCES];
pthread_mutex_t bank_lock = PTHREAD_MUTEX_INITIALIZER;
typedef struct customer {
    int max[RESOURCES];
    int allocation[RESOURCES];
    int need[RESOURCES]; // not necessary needed
} customer_t;
customer_t customers[MAX_CUSTOMER];
int safe_algorithm(){
    int work[RESOURCES];
    for (int i = 0; i < RESOURCES; i++) {
        work[i] = available[i];
    }
    bool finish[MAX_CUSTOMER];
    for (int i = 0; i < MAX_CUSTOMER; i++) {
        finish[i] = false;
    }
    int count = 0;
    while (count < MAX_CUSTOMER) {
        bool found = false;
        for (int i = 0; i < MAX_CUSTOMER; i++) {
            if (finish[i] == false) {
                bool need = true;
                for (int j = 0; j < RESOURCES; j++) {
                    if (customers[i].need[j] > work[j]) {
                        need = false;
                        break;
                    }
                }
                if (need) {
                    for (int j = 0; j < RESOURCES; j++) {
                        work[j] += customers[i].allocation[j];
                    }
                    count++;
                    finish[i] = true;
                    found = true;
                }
            }
        }

        if (!found) {
            return -1;
        }
    }
    return 0;
}
int request_resources(int customer_id, int request[]) { 
    pthread_mutex_lock(&bank_lock);
    for (int i = 0; i < RESOURCES; i++) {
        if (request[i] > customers[customer_id].need[i]|| request[i] > available[i]) {
            pthread_mutex_unlock(&bank_lock);
            return -1;
        }
    }
    for (int i = 0; i < RESOURCES; i++) {
        available[i] -= request[i];
        customers[customer_id].allocation[i] += request[i];
        customers[customer_id].need[i] -= request[i];
    }
    printf("Available: ");
    for (int i = 0; i < RESOURCES; i++) {
        printf("%d ", available[i]);
    }
    puts(
       "" 
    );
    for (int i = 0; i < MAX_CUSTOMER; i++) {
        printf("Customer %d:\n", i);
        for (int j = 0; j < RESOURCES; j++) {
            printf("resource %d, allocation %d need %d\n",j , customers[i].allocation[j], customers[i].need[j]);
        }
        printf("\n");
    }
    if(safe_algorithm() == -1){
        for (int i = 0; i < RESOURCES; i++) {
            available[i] += request[i];
            customers[customer_id].allocation[i] -= request[i];
            customers[customer_id].need[i] += request[i];
        }
        pthread_mutex_unlock(&bank_lock);
        return -1;
    }
    // print log
    pthread_mutex_unlock(&bank_lock);
    return 0;
}
void release_resources(int customer_id) { 
    pthread_mutex_lock(&bank_lock);
    for (int i = 0; i < RESOURCES; i++) {
        available[i] += customers[customer_id].allocation[i];
        customers[customer_id].allocation[i] = 0;
    }
    pthread_mutex_unlock(&bank_lock);
}
void* customer_thread(void* arg) {
    int customer_id = *(int*)arg;
    int request[RESOURCES];
    while (true) {
        for (int i = 0; i < RESOURCES; i++) {
            request[i] = rand() % (customers[customer_id].need[i] + 1);
        }
        // request resources
        if (request_resources(customer_id, request) == 0) {
            // do something
            bool flag = true;
            for (int i = 0; i < RESOURCES; i++) {
                if (customers[customer_id].need[i] != 0) {
                    flag = false;
                    break;
                }
            }
            if (flag) {
                // release resources
                release_resources(customer_id);
                pthread_exit(NULL);
            }
            sleep(1);
        }
        // release resources
    }
}
int main (int argc, char* argv[]) {
    if (argc != RESOURCES + 1) {
        fprintf(stderr, "Usage: %s <resource 1> <resource 2> <resource 3> <resource 4>\n", argv[0]);
        return 1;
    }
    for (int i = 0; i < RESOURCES; i++) {
        available[i] = atoi(argv[i + 1]);
    } 
    for (int i = 0; i < MAX_CUSTOMER; i++) {
        for (int j = 0; j < RESOURCES; j++) {
            customers[i].max[j] = rand() % (available[j] - 3);
            customers[i].allocation[j] = 0;
            customers[i].need[j] = customers[i].max[j];
        }
    }
    pthread_t threads[MAX_CUSTOMER];        
    for (int i = 0; i < MAX_CUSTOMER; i++) {
        int *arg = malloc(sizeof(*arg));
        *arg = i;
        pthread_create(&threads[i], NULL, customer_thread, arg);
    }
    for (int i = 0; i < MAX_CUSTOMER; i++) {
        pthread_join(threads[i], NULL);
    }
}