#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 10

typedef struct {
    int* array;
    size_t start;
    size_t end;
} ThreadData;

void quick_sort(int* arr, int low, int high);

void *runner(void *param) {
    int n1 = ((ThreadData*)param)->start;
    int n2 = ((ThreadData*)param)->end;
    int *arr = ((ThreadData*)param)->array;
    quick_sort(arr, n1, n2);
    pthread_exit(0);
}

void swap(int* a, int* b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int partition(int* arr, int low, int high) {
    int pivot = arr[high];
    int i = low - 1;
    for (int j = low; j <= high - 1; j++) {
        if (arr[j] < pivot) {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return i + 1;
}

void quick_sort(int* arr, int low, int high) {
    pthread_t thread1, thread2;
    if (low < high) {
        int pi = partition(arr, low, high);
        ThreadData data1 = {arr, low, pi - 1};
        pthread_create(&thread1, NULL, runner, &data1);
        ThreadData data2 = {arr, pi + 1, high};
        pthread_create(&thread2, NULL, runner, &data2);
        pthread_join(thread1, NULL);
        pthread_join(thread2, NULL);
    }
}

void generateRandomArray(int *arr, int size) {
    for (int i = 0; i < size; i++) {
        arr[i] = rand() % 100;
    }
}

int main() {
    int arr[SIZE];
    int n = sizeof(arr) / sizeof(arr[0]);
    generateRandomArray(arr, n);
    
    printf("Unsorted array: ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
    
    quick_sort(arr, 0, n - 1);

    printf("Sorted array: ");
    for (int i = 0; i < n; i++)
        printf("%d ", arr[i]);
    printf("\n");
    return 0;
}
