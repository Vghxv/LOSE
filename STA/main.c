#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#define CHAIR_NUM 3
#define STUDENT_NUM 5

pthread_mutex_t queue_mutex;
sem_t ta_sleeping;
sem_t chairs;
sem_t ta_finish;

typedef struct student_queue{
    pthread_t student_thread;
    struct student_queue *next;
} student_queue_t;
student_queue_t *head = NULL;

void pop(){
    pthread_mutex_lock(&queue_mutex);
    student_queue_t *temp = head->next;
    if (temp == NULL){
        pthread_mutex_unlock(&queue_mutex);
        exit(1);
    }
    head->next = temp->next;
    pthread_t student_thread = temp->student_thread;
    free(temp);
    pthread_mutex_unlock(&queue_mutex);
}

void push(pthread_t student_thread){
    pthread_mutex_lock(&queue_mutex);
    student_queue_t *temp = head;
    while(temp->next != NULL){
        temp = temp->next;
    }
    student_queue_t *new_node = (student_queue_t *)malloc(sizeof(student_queue_t));
    new_node->student_thread = student_thread;
    new_node->next = NULL;
    temp->next = new_node;
    pthread_mutex_unlock(&queue_mutex);
}

void *teaching_assistant(void *arg){
    while(1){
        pthread_mutex_lock(&queue_mutex);
        if (head->next == NULL){
            pthread_mutex_unlock(&queue_mutex);
            printf("TA is sleeping\n");
            sem_wait(&ta_sleeping);
        }
        else{
            pthread_t student_thread = head->next->student_thread;
            pthread_mutex_unlock(&queue_mutex);
            printf("TA calls student %ld\n", student_thread);
            sem_post(&ta_finish);
        }
        printf("TA is helping student\n");
        sleep(1);
        printf("TA finish helping student\n");
    }
}

void *student(void *arg){
    int wait_time;
    while(1){
        wait_time = rand() % 5 + 5;
        sleep(wait_time);
        printf("Student %ld is coming\n", pthread_self());
        sem_wait(&chairs);
        push(pthread_self());
        int chairs_n;
        sem_getvalue(&chairs, &chairs_n);
        printf("Student %ld is sitting on the chair, chair left %d\n", pthread_self(), chairs_n);
        while(head->next->student_thread != pthread_self());
        printf("kokoko🚪🤛, Student %ld is smashing the door\n", pthread_self());
        system("play -q knock.mp3 -t alsa");
 
        sem_post(&ta_sleeping);
        sem_wait(&ta_finish);
        pop();
        sem_post(&chairs);
        printf("Student %ld stand up and leave\n", pthread_self());
    }
}

int main(int argc, char *argv[]){
    pthread_t ta;
    pthread_t students[STUDENT_NUM];
    sem_init(&chairs, 0, CHAIR_NUM);
    sem_init(&ta_finish, 0, 1);
    sem_init(&ta_sleeping, 0, 1);
    pthread_mutex_init(&queue_mutex, NULL);
    head = (student_queue_t *)malloc(sizeof(student_queue_t));
    pthread_create(&ta, NULL, teaching_assistant, NULL);
    for(int i = 0; i < STUDENT_NUM; i++){
        pthread_create(&students[i], NULL, student, NULL);
    }
    pthread_join(ta, NULL);
    for(int i = 0; i < STUDENT_NUM; i++){
        pthread_join(students[i], NULL);
    }

    return 0;
}