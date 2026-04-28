#include <stdio.h>
#include <stdlib.h>

#define GRID_SIZE 10

int main(){
    int **grid = (int**) calloc(0, GRID_SIZE * sizeof(int *));
    for(int i = 0; i < GRID_SIZE; i++){
        grid[i] = calloc(GRID_SIZE, sizeof(int));
    }

    for(int i = 0; i < GRID_SIZE; i++){
        for(int j =0 ; j < GRID_SIZE; j++){
            printf("%d", grid[i][j]);
        }
        printf("\n");
    }
}
