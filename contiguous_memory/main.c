#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#define PROCESS_ID_SIZE 10
#define MAIN_MEMORY_SIZE 1000

typedef struct block {
    int start_address;
    int available_space;
    struct block *next;
    char process_id[PROCESS_ID_SIZE];
} block_t;

typedef enum {
    FIRST_FIT,
    BEST_FIT,
    WORST_FIT
} allocation_strategy;

block_t* find_first_fit(block_t *main_memory, int allocation_size) {
    block_t *current = main_memory;
    while(current != NULL) {
        if (current->available_space >= allocation_size && strcmp(current->process_id, "FREE") == 0) {
            return current;
        }
        current = current->next;
    }
    return NULL;
}

block_t* find_best_fit(block_t *main_memory, int allocation_size) {
    block_t *current = main_memory;
    block_t *best_fit = NULL;
    int best_fit_size = MAIN_MEMORY_SIZE;
    while(current != NULL) {
        if (current->available_space < best_fit_size && current->available_space >= allocation_size && strcmp(current->process_id, "FREE") == 0) {   
            best_fit = current;
            best_fit_size = current->available_space;
        }
        current = current->next;
    }
    return best_fit;
}

block_t* find_worst_fit(block_t *main_memory, int allocation_size) {
    block_t *current = main_memory;
    block_t *worst_fit = NULL;
    int worst_fit_size = 0;
    while(current != NULL) {
        if (current->available_space > worst_fit_size && current->available_space >= allocation_size && strcmp(current->process_id, "FREE") == 0) {
            worst_fit = current;
            worst_fit_size = current->available_space;
        }
        current = current->next;
    }
    return worst_fit;
}

void merge_unused_adjacent_blocks(block_t *main_memory) {
    block_t *current = main_memory;
    while(current != NULL) {
        if (strcmp(current->process_id, "FREE") == 0) {
            block_t *next = current->next;
            while(next != NULL && strcmp(next->process_id, "FREE") == 0) {
                current->available_space += next->available_space;
                current->next = next->next;
                free(next);
                next = current->next;
            }
        }
        current = current->next;
    }
}

void free_blocks(block_t *main_memory, char *process_id) {
    block_t *current = main_memory;
    while(current != NULL) {
        if (strcmp(current->process_id, process_id) == 0) {
            strncpy(current->process_id, "FREE", 10);
            break;
        }
        current = current->next;
    }
    merge_unused_adjacent_blocks(main_memory);
}

block_t *init_block(int start_address, int available_space, char *process_id) {
    block_t *block = (block_t *)malloc(sizeof(block_t));
    block->start_address = start_address;
    block->available_space = available_space;
    block->next = NULL;
    strcpy(block->process_id, process_id);
    return block;
}

void allocate_blocks(block_t *block, int allocation_size, char *process_id) {
    int left_over_space = block->available_space - allocation_size;
    block->available_space = allocation_size;
    strncpy(block->process_id, process_id, PROCESS_ID_SIZE);
    if (left_over_space > 0) {
        block_t *newBlock = init_block(block->start_address + allocation_size, left_over_space, "FREE");
        block_t *temp = block->next;
        block->next = newBlock;
        newBlock->next = temp;
    }
}

void request_block(block_t *main_memory, int allocation_size, char *process_id, allocation_strategy strategy) {
    block_t *block = NULL;
    switch (strategy) {
        case FIRST_FIT:
            block = find_first_fit(main_memory, allocation_size);
            break;
        case BEST_FIT:
            block = find_best_fit(main_memory, allocation_size);
            break;
        case WORST_FIT:
            block = find_worst_fit(main_memory, allocation_size);
            break;
        default:
            printf("Invalid allocation strategy\n");
            return;
    }

    if (block != NULL) {
        allocate_blocks(block, allocation_size, process_id);
    } else {
        printf("No available space for process %s\n", process_id);
    }
}

void free_list(block_t *main_memory) {
    block_t *current = main_memory;
    block_t *next;
    while(current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

block_t *setup_memory_state() {
    float segments[10] = {0.1, 0.2, 0.05, 0.1, 0.15, 0.05, 0.2, 0.05, 0.06, 0.04};
    block_t *main_memory;
    int flop = 0;
    int example_process_id = 1;
    for (int i = 0; i < 10; i++) {
        int available_space = MAIN_MEMORY_SIZE * segments[i];
        if (i == 0) {
            main_memory = init_block(0, available_space, "FREE");
        } 
        else {
            block_t *current = main_memory;
            int start_address = 0;
            while(current != NULL) {
                if (current->next == NULL) {
                    start_address = current->start_address + current->available_space;
                    break;
                }
                current = current->next;
            }
            char process_id[PROCESS_ID_SIZE];
            if (flop == 0) {
                snprintf(process_id, PROCESS_ID_SIZE, "process_%d", example_process_id++);
            } else {
                snprintf(process_id, PROCESS_ID_SIZE, "FREE");
            }
            flop = flop ^ 1;
            block_t *newBlock = init_block(start_address, available_space, process_id);
            current->next = newBlock;
        }
    }
    return main_memory;
}

void report_memory_usage(block_t *main_memory) {
    block_t *current = main_memory;
    while(current != NULL) {
        printf("Address[%d:%d] Process ID: %s\n", current->start_address, current->start_address + current->available_space, current->process_id);
        current = current->next;
    }
}

int main() {
    block_t *main_memory = setup_memory_state();

    char buffer[100];
    while (1){
        printf("allocator> ");
        fgets(buffer, 100, stdin);
        char *command = strtok(buffer, " ");
        command[strcspn(command, "\n")] = 0;

        if (strcmp(command, "exit") == 0) {
            break;
        }
        else if (strcmp(command, "request") == 0) {
            char *process_id = strtok(NULL, " ");
            int allocation_size = atoi(strtok(NULL, " "));
            char *strategy = strtok(NULL, " ");
            strategy[strcspn(strategy, "\n")] = 0;
            allocation_strategy allocation_strategy;
            
            if (strcmp(strategy, "first") == 0) {
                allocation_strategy = FIRST_FIT;
            } else if (strcmp(strategy, "best") == 0) {
                allocation_strategy = BEST_FIT;
            } else if (strcmp(strategy, "worst") == 0) {
                allocation_strategy = WORST_FIT;
            } else {
                printf("Invalid allocation strategy\n");
                continue;
            }
            request_block(main_memory, allocation_size, process_id, allocation_strategy);
        }
        else if (strcmp(command, "free") == 0) {
            char *process_id = strtok(NULL, " ");
            process_id[strcspn(process_id, "\n")] = 0;
            free_blocks(main_memory, process_id);
        }
        else if (strcmp(command, "report") == 0) {
            report_memory_usage(main_memory);
        }
        else if (strcmp(command, "help") == 0) {
            printf("Commands:\n");
            printf("request <process_id> <allocation_size> <allocation_strategy>\n");
            printf("free <process_id>\n");
            printf("report\n");
            printf("exit\n");
        }
        else {
            printf("Invalid command\n");
        }
    }
    free_list(main_memory);
}