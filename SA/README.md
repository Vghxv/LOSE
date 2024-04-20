# Scheduling Algorithms

## Problem

- This project involves implementing several process
scheduling algorithms
    - FCFS
    - SJF
    - Priority-based
    - Round-Robin
    - Priority with round-robin
- The implementation of this project may be completed in
either C or Java

## Solution

The solution is implemented in C. The program reads a list of processes from a file and simulates the execution of the processes using the following scheduling algorithms:
- First-Come, First-Served (FCFS)
- Shortest Job First (SJF)(non-preemptive)
- Priority-Based(Pb)(non-preemptive)
- Round-Robin(RR)
- Priority with Round-Robin(PwRR)

### FCFS

The FCFS algorithm is implemented by selecting the process that arrives first to execute next. The program calculates the waiting time, turnaround time, and response time for each process.

### SJF

The SJF algorithm is implemented by selecting the process with the shortest burst time to execute next. The program calculates the waiting time, turnaround time, and response time for each process.

### Priority-Based

The Priority-Based algorithm is implemented by selecting the process with the highest priority to execute next. The program calculates the waiting time, turnaround time, and response time for each process.

### Round-Robin

The Round-Robin algorithm is implemented by assigning a fixed time quantum to each process. The program calculates the waiting time, turnaround time, and response time for each process.

### Priority with Round-Robin

The Priority with Round-Robin algorithm is implemented by selecting the process with the highest priority to execute next. If two processes have the same priority, the Round-Robin algorithm is used to break the tie. The program calculates the waiting time, turnaround time, and response time for each process.

## Instructions

To compile and run the program, use the following commands:

```bash
make
```

user will be prompted to input the c file containing the specific implementation of the scheduling algorithm. The program will then read the processes from the file and simulate the execution using the selected scheduling algorithm.
