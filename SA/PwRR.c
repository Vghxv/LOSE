#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int id;             // Process ID
    int arrival_time;   // Arrival time of the process
    int burst_time;     // Burst time of the process
    int priority;       // Priority of the process
    int remaining_time; // Remaining burst time of the process
    int completion_time; // Completion time of the process
    int turnaround_time; // Turnaround time of the process
    int waiting_time;   // Waiting time of the process
} process_t;

void priorityRoundRobin(process_t *processes, int num_processes, int time_quantum) {
    int current_time = 0;
    int completed_processes = 0;
    int *burst_remaining = (int *)malloc(num_processes * sizeof(int));
    
    for (int i = 0; i < num_processes; i++) {
        burst_remaining[i] = processes[i].burst_time;
    }

    while (completed_processes < num_processes) {
        int all_processes_completed = 1;
        int highest_priority_index = -1;
        int highest_priority = INT_MAX;
        for (int i = 0; i < num_processes; i++) {
            if (processes[i].arrival_time <= current_time && burst_remaining[i] > 0) {
                all_processes_completed = 0;
                if (processes[i].priority < highest_priority) {
                    highest_priority_index = i;
                    highest_priority = processes[i].priority;
                }
            }
        }
        if (highest_priority_index == -1) {
            current_time++;
        } 
        else {
            if (burst_remaining[highest_priority_index] > time_quantum) {
                current_time += time_quantum;
                burst_remaining[highest_priority_index] -= time_quantum;
            } 
            else {
                current_time += burst_remaining[highest_priority_index];
                burst_remaining[highest_priority_index] = 0;
                processes[highest_priority_index].completion_time = current_time;
                processes[highest_priority_index].turnaround_time = processes[highest_priority_index].completion_time - processes[highest_priority_index].arrival_time;
                processes[highest_priority_index].waiting_time = processes[highest_priority_index].turnaround_time - processes[highest_priority_index].burst_time;
                completed_processes++;
            }
        }
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
    printf("Process\t Arrival Time\t Burst Time\t Priority\t Completion Time\t Turnaround Time\t Waiting Time\n");
    for (int i = 0; i < num_processes; i++) {
        printf("%d\t\t %d\t\t %d\t\t %d\t\t %d\t\t\t %d\t\t\t %d\n", processes[i].id, processes[i].arrival_time,
               processes[i].burst_time, processes[i].priority, processes[i].completion_time, processes[i].turnaround_time,
               processes[i].waiting_time);
    }
    printf("\nAverage Turnaround Time: %.2f\n", avg_turnaround_time);
    printf("Average Waiting Time: %.2f\n", avg_waiting_time);
}

int main() {
    int num_processes, time_quantum;
    FILE *file = fopen("PwRR.txt", "r");
    fscanf(file, "%d %d", &num_processes, &time_quantum);

    process_t *processes = (process_t *)malloc(num_processes * sizeof(process_t));

    int i = 0;
    while(fscanf(file, "%d %d %d", &processes[i].arrival_time, &processes[i].burst_time, &processes[i].priority) != EOF) {
        processes[i].id = i + 1;
        processes[i].remaining_time = processes[i].burst_time;
        i++;
    }

    fclose(file);
    
    priorityRoundRobin(processes, num_processes, time_quantum);

    float avg_turnaround_time, avg_waiting_time;
    calculateAverageTimes(processes, num_processes, &avg_turnaround_time, &avg_waiting_time);

    displaySchedule(processes, num_processes, avg_turnaround_time, avg_waiting_time);

    free(processes);

    return 0;
}
