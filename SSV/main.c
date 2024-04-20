#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

#define SIZE 9

typedef struct {
    int grid[SIZE][SIZE];
} Sudoku_t;

void createRandomPuzzle(Sudoku_t* sudoku) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            sudoku->grid[i][j] = (rand()) % SIZE + 1;
        }
    }
}

void createValidSudoku(Sudoku_t* sudoku) {
    for (int i = 0; i < SIZE; i++) {
        for (int j = 0; j < SIZE; j++) {
            sudoku->grid[i][j] = (i * 3 + i / 3 + j) % 9 + 1;
        }
    }
}

void displaySudoku(Sudoku_t* sudoku) {
    for (int i = 0; i < SIZE; i++) {
        if (i % 3 == 0 && i != 0) {
            printf("------+-------+------\n");
        }
        for (int j = 0; j < SIZE; j++) {
            if (j % 3 == 0 && j != 0) {
                printf("| ");
            }
            printf("%d ", sudoku->grid[i][j]);
        }
        printf("\n");
    }
}

void *checkRow(void* sudoku){
    for (int i=0; i<9; i++){
        int total = 0;
        for (int j=0; j<9; j++){
            total += ((Sudoku_t*)sudoku)->grid[i][j];
        }
        if (total != 45){
            printf("Row %d is invalid\n", i);
            *(volatile int *)NULL;
        }
    }
    pthread_exit(NULL);
}

void *checkCol(void* sudoku){
    for (int i=0; i<9; i++){
        int total = 0;
        for (int j=0; j<9; j++){
            total += ((Sudoku_t*)sudoku)->grid[j][i];
        }
        if (total != 45){
            printf("Col %d is invalid\n", i);
            *(volatile int *)NULL;
        }
    }
    pthread_exit(NULL);
}

void *checkBox(void* sudoku){
    for (int i=0; i<9; i++){
        int total = 0;
        for (int j=0; j<9; j++){
            total += ((Sudoku_t*)sudoku)->grid[i / 3 * 3 + j / 3][i % 3 * 3 + j % 3];
        }
        if (total != 45){
            printf("Box %d is invalid\n", i);
            *(volatile int *)NULL;
        }
    }
    pthread_exit(NULL);
}

void checkSudoku(Sudoku_t* sudoku) {
    pthread_t threadRow;
    pthread_create(&threadRow, NULL, checkRow, sudoku);
    pthread_join(threadRow, NULL);

    pthread_t threadCol;
    pthread_create(&threadCol, NULL, checkCol, sudoku);
    pthread_join(threadCol, NULL);

    pthread_t threadBox;
    pthread_create(&threadBox, NULL, checkBox, sudoku);
    pthread_join(threadBox, NULL);

    printf("Sudoku is valid\n");
}

int main(int argc, char* argv[]) {
    Sudoku_t sudoku;
    if (argc != 2) {
        printf("Usage: %s <type>\n", argv[0]);
        return 1;
    }
    // if argv[1] is 'T', create a valid sudoku, else if argv[1] is 'F', create a random puzzle 
    if (argv[1][0] == 'T')
        createValidSudoku(&sudoku);
    else if (argv[1][0] == 'F')
        createRandomPuzzle(&sudoku);
    else {
        printf("Invalid type\n");
        return 1;
    }

    printf("Sudoku Grid:\n");
    displaySudoku(&sudoku);
    checkSudoku(&sudoku);

    return 0;
}
