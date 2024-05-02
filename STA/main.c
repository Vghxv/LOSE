#include <complex.h>
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <semaphore.h>

#define CHAIR_NUM 4
#define STUDENT_NUM 5
sem_t cond;
// pthread_cond_t ta_finish_cond;
pthread_mutex_t mutex;
pthread_mutex_t list_mutex;
sem_t sem;
sem_t sem_ta_finish;

typedef struct student_queue{
    pthread_t student_thread;
    struct student_queue *next;
} student_queue_t;
student_queue_t *head = NULL;

void pop(){
    pthread_mutex_lock(&list_mutex);
    student_queue_t *temp = head->next;
    if (temp == NULL){
        pthread_mutex_unlock(&list_mutex);
        exit(1);
    }
    head->next = temp->next;
    pthread_t student_thread = temp->student_thread;
    free(temp);
    pthread_mutex_unlock(&list_mutex);
}

void push(pthread_t student_thread){
    pthread_mutex_lock(&list_mutex);
    student_queue_t *temp = head;
    while(temp->next != NULL){
        temp = temp->next;
    }
    student_queue_t *new_node = (student_queue_t *)malloc(sizeof(student_queue_t));
    new_node->student_thread = student_thread;
    new_node->next = NULL;
    temp->next = new_node;
    pthread_mutex_unlock(&list_mutex);
}

void *teaching_assistant(void *arg){
    while(1){
        printf("Teaching assistant is taking a nap\n");
        // pthread_cond_wait(&cond, &mutex);
        sem_wait(&cond);

        printf("Teaching assistant is helping student\n");
        sleep(1);
        printf("Teaching assistant finish helping student\n");
        sem_post(&sem_ta_finish);
    }
}

void *student(void *arg){
    int wait_time;
    while(1){
        sleep(1);
        printf("Student %ld is coming\n", pthread_self());

        sem_wait(&sem);
        push(pthread_self());
        printf("Student %ld is sitting on the chair\n", pthread_self());

        while(head->next->student_thread != pthread_self());

        pthread_mutex_lock(&mutex);
        printf("kokoko🚪🤛, Student %ld is smashing the door\n", pthread_self());
        // play sfx of knocking the door from a file named "knock.mp3"
        // system("mpg123 knock.mp3");

        
        // pthread_cond_signal(&cond);
        sem_post(&cond);
        pop();

        sem_post(&sem);
        sem_wait(&sem_ta_finish);
        printf("Student %ld is leaving\n", pthread_self());
        pthread_mutex_unlock(&mutex); 
    }
}

int main(int argc, char *argv[]){
    pthread_t ta;
    pthread_t students[STUDENT_NUM];
    sem_init(&sem, 0, CHAIR_NUM);
    sem_init(&sem_ta_finish, 0, 1);
    sem_init(&cond, 0, 1);
    pthread_mutex_init(&mutex, NULL);
    pthread_mutex_init(&list_mutex, NULL);
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