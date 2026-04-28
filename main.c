#include "raylib.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

const int cellSize = 3;
#define GRID_SIZE 1050
#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280

// assumes square matrix
int checkBounds(int i, int j, int mtxSize){
    if(i < 0 || j < 0) return 0;
    else if(i > mtxSize-1 || j > mtxSize-1) return 0;
    else return 1;
}

void drawCell(int x, int y, Color color){
    DrawRectangle(x * cellSize, y*cellSize, cellSize, cellSize, color);
}

bool checkCell(int aliveNeighbours, bool dead){
    if(dead) return aliveNeighbours == 3;
    return aliveNeighbours == 2 || aliveNeighbours == 3;
}

void spawnPentomino(int grid[GRID_SIZE][GRID_SIZE]){
    int x = rand() % (GRID_SIZE-6);
    int y = rand() % (GRID_SIZE-6);
    grid[x][y+1] = 1;
    grid[x][y+2] = 1;
    grid[x+1][y] = 1;
    grid[x+1][y+1] = 1;
    grid[x+2][y+1] = 1;
}

void spawnAcorn(int grid[GRID_SIZE][GRID_SIZE]){
    int x = rand() % (GRID_SIZE-6);
    int y = rand() % (GRID_SIZE-6);
    grid[x][y+1] = 1;
    grid[x+1][y+3] = 1;
    grid[x+2][y] = 1;
    grid[x+2][y+1] = 1;
    grid[x+2][y+4] = 1;
    grid[x+2][y+5] = 1;
    grid[x+2][y+6] = 1;
}

// [i-5, j-5] [i, j-5] [i+5, j-5]
// [i-5,  j ] [ i, j ] [i+5,  j ]
// [i-5, j+5] [ i,j+5] [i+5, j+5]
void checkGrid(int grid[GRID_SIZE][GRID_SIZE]){
    int newGrid[GRID_SIZE][GRID_SIZE];
    memcpy(newGrid, grid, GRID_SIZE * GRID_SIZE * sizeof(int));

    bool dead = false;
    for(int i = 0; i < GRID_SIZE; i++){
        for(int j = 0; j < GRID_SIZE; j++){
            int aliveNeighbours = 0;
            dead = grid[i][j] == 0 ? true : false;
            // i - 5
            if(checkBounds(i-1, j-1, GRID_SIZE))
                if(grid[i-1][j-1] == 1) aliveNeighbours++;
            if(checkBounds(i-1, j, GRID_SIZE))
                if(grid[i-1][j] == 1) aliveNeighbours++;
            if(checkBounds(i-1, j+1, GRID_SIZE))
                if(grid[i-1][j+1] == 1) aliveNeighbours++;
            // i
            if(checkBounds(i, j-1, GRID_SIZE))
                if(grid[i][j-1] == 1) aliveNeighbours++;
            if(checkBounds(i, j+1, GRID_SIZE))
                if(grid[i][j+1] == 1) aliveNeighbours++;
            // i+5
            if(checkBounds(i+1, j-1, GRID_SIZE))
                if(grid[i+1][j-1] == 1) aliveNeighbours++;
            if(checkBounds(i+1, j, GRID_SIZE))
                if(grid[i+1][j] == 1) aliveNeighbours++;
            if(checkBounds(i+1, j+1, GRID_SIZE))
                if(grid[i+1][j+1] == 1) aliveNeighbours++;

            if(checkCell(aliveNeighbours, dead)){
                drawCell(i, j, BLACK);
                newGrid[i][j] = 1;
            } else newGrid[i][j] = 0;
        }
    }
    memcpy(grid, newGrid, GRID_SIZE * GRID_SIZE * sizeof(int));
}

// 0,0 5,5

int main() {
    srand(time(NULL));
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "raylib");
    int grid[GRID_SIZE][GRID_SIZE] = { 0 };

    int spawns = rand() % 200;
    for(int i = 0; i < spawns; i++){
        spawnAcorn(grid);
        spawnPentomino(grid);
    }
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);

        checkGrid(grid);

        usleep(50 * 1000);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
