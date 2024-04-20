#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int id;             // Process ID
    int burst_time;     // Burst time of the process
    int remaining_time; // Remaining burst time of the process
    int completion_time; // Completion time of the process
    int turnaround_time; // Turnaround time of the process
    int waiting_time;   // Waiting time of the process
} process_t;

void roundRobin(process_t *processes, int num_processes, int time_quantum) {
    int current_time = 0;
    int completed_processes = 0;
    int *burst_remaining = (int *)malloc(num_processes * sizeof(int));
    
    for (int i = 0; i < num_processes; i++) {
        burst_remaining[i] = processes[i].burst_time;
    }

    while (completed_processes < num_processes) {
        int all_processes_completed = 1;
        for (int i = 0; i < num_processes; i++) {
            if (burst_remaining[i] > 0) {
                all_processes_completed = 0;
                if (burst_remaining[i] > time_quantum) {
                    current_time += time_quantum;
                    burst_remaining[i] -= time_quantum;
                } 
                else {
                    current_time += burst_remaining[i];
                    burst_remaining[i] = 0;
                    processes[i].completion_time = current_time;
                    processes[i].turnaround_time = processes[i].completion_time;
                    processes[i].waiting_time = processes[i].turnaround_time - processes[i].burst_time;
                    completed_processes++;
                }
            }
        }
        if (all_processes_completed)
            break;
    }
    free(burst_remaining);
}

void calculateAverageTimes(process_t *processes, int num_processes, float *avg_turnaround_time, float *avg_waiting_time) {
    float total_turnaround_time = 0, total_waiting_time = 0;
    for (int i = 0; i < num_processes; i++) {
        total_turnaround_time += processes[i].turnaround_time;
        total_waiting_time += processes[i].waiting_time;
    }
    *avg_turnaround_time = total_turnaround_time / num_processes;
    *avg_waiting_time = total_waiting_time / num_processes;
}

void displaySchedule(process_t *processes, int num_processes, float avg_turnaround_time, float avg_waiting_time) {
    printf("Process\t Burst Time\t Completion Time\t Turnaround Time\t Waiting Time\n");
    for (int i = 0; i < num_processes; i++) {
        printf("%d\t\t %d\t\t %d\t\t %d\t\t\t %d\n", processes[i].id,
               processes[i].burst_time, processes[i].completion_time, processes[i].turnaround_time,
               processes[i].waiting_time);
    }
    printf("\nAverage Turnaround Time: %.2f\n", avg_turnaround_time);
    printf("Average Waiting Time: %.2f\n", avg_waiting_time);
}

int main() {
    int num_processes, time_quantum;
    // printf("Enter the number of processes: ");
    // scanf("%d", &num_processes);
    // printf("Enter the time quantum: ");
    // scanf("%d", &time_quantum);
    FILE *file = fopen("RR.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    fscanf(file, "%d %d", &num_processes, &time_quantum);
    
    process_t *processes = (process_t *)malloc(num_processes * sizeof(process_t));

    int i = 0;
    while (fscanf(file, "%d", &processes[i].burst_time) != EOF) {
        processes[i].id = i + 1;
        processes[i].remaining_time = processes[i].burst_time;
        i++;
    }

    fclose(file);

    roundRobin(processes, num_processes, time_quantum);

    float avg_turnaround_time, avg_waiting_time;
    calculateAverageTimes(processes, num_processes, &avg_turnaround_time, &avg_waiting_time);

    displaySchedule(processes, num_processes, avg_turnaround_time, avg_waiting_time);

    free(processes);

    return 0;
}
