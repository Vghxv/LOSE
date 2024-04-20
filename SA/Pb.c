#include <limits.h>
#include<stdio.h>
#include <stdlib.h>

typedef struct {
    int id;             // Process ID
    int burst_time;     // Burst time of the process
    int priority;       // Priority of the process
    int completion_time; // Completion time of the process
    int turnaround_time; // Turnaround time of the process
    int waiting_time;   // Waiting time of the process
    int executed;       // Flag to mark whether the process has been executed
} process_t;

void computeTimes(process_t *processes, int num_processes) {
    int current_time = 0;
    int total_executed = 0;
    while (total_executed < num_processes) {
        int highest_priority_index = -1;
        int highest_priority = INT_MAX;
        for (int i = 0; i < num_processes; i++) {
            if (!processes[i].executed && processes[i].priority < highest_priority) {
                highest_priority_index = i;
                highest_priority = processes[i].priority;
            }
        }
        if (highest_priority_index == -1) {
            current_time++;
        } else {
            processes[highest_priority_index].completion_time = current_time + processes[highest_priority_index].burst_time;
            processes[highest_priority_index].turnaround_time = processes[highest_priority_index].completion_time;
            processes[highest_priority_index].waiting_time = processes[highest_priority_index].turnaround_time - processes[highest_priority_index].burst_time;
            processes[highest_priority_index].executed = 1;
            current_time = processes[highest_priority_index].completion_time;
            total_executed++;
        }
    }
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
    printf("Process\t Burst Time\t Priority\t Completion Time\t Turnaround Time\t Waiting Time\n");
    for (int i = 0; i < num_processes; i++) {
        printf("%d\t\t %d\t\t %d\t\t %d\t\t %d\t\t\t %d\n", processes[i].id,
               processes[i].burst_time, processes[i].priority, processes[i].completion_time, processes[i].turnaround_time,
               processes[i].waiting_time);
    }
    printf("\nAverage Turnaround Time: %.2f\n", avg_turnaround_time);
    printf("Average Waiting Time: %.2f\n", avg_waiting_time);
}

int main() {
    int num_processes;
    
    FILE *file = fopen("Pb.txt", "r");
    if (file == NULL) {
        printf("Error opening file\n");
        return 1;
    }
    fscanf(file, "%d", &num_processes);

    process_t *processes = (process_t *) malloc(num_processes * sizeof(process_t));

    int i = 0;
    while(fscanf(file, "%d %d", &processes[i].burst_time, &processes[i].priority) != EOF) {
        processes[i].id = i + 1;
        processes[i].executed = 0;
        i++;
    }

    fclose(file);

    computeTimes(processes, num_processes);

    float avg_turnaround_time, avg_waiting_time;
    calculateAverageTimes(processes, num_processes, &avg_turnaround_time, &avg_waiting_time);

    displaySchedule(processes, num_processes, avg_turnaround_time, avg_waiting_time);

    free(processes);

    return 0;
}
