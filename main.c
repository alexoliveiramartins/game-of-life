#include "raylib.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>

const int cellSize = 5;
#define GRID_SIZE 500

// assumes square matrix
int checkBounds(int i, int j, int mtxSize){
    if(i < 0 || j < 0) return 0;
    else if(i > mtxSize-1 || j > mtxSize-1) return 0;
    else return 1;
}

void drawCell(int x, int y, Color color){
    DrawRectangle(x, y, cellSize, cellSize, color);
}

bool checkCell(int aliveNeighbours, bool dead){
    if(dead) return aliveNeighbours == 3;
    return aliveNeighbours == 2 || aliveNeighbours == 3;
}

// [i-5, j-5] [i, j-5] [i+5, j-5]
// [i-5,  j ] [ i, j ] [i+5,  j ]
// [i-5, j+5] [ i,j+5] [i+5, j+5]
void checkGrid(int grid[GRID_SIZE][GRID_SIZE]){
    int newGrid[GRID_SIZE][GRID_SIZE];
    memcpy(newGrid, grid, GRID_SIZE * GRID_SIZE * sizeof(int));

    bool dead = false;
    for(int i = 0; i < GRID_SIZE; i+= cellSize){
        for(int j = 0; j < GRID_SIZE; j+= cellSize){
            int aliveNeighbours = 0;
            dead = grid[i][j] == 0 ? true : false;
            // i - 5
            if(checkBounds(i-cellSize, j-cellSize, GRID_SIZE))
                if(grid[i-cellSize][j-cellSize] == 1) aliveNeighbours++;
            if(checkBounds(i-cellSize, j, GRID_SIZE))
                if(grid[i-cellSize][j] == 1) aliveNeighbours++;
            if(checkBounds(i-cellSize, j+cellSize, GRID_SIZE))
                if(grid[i-cellSize][j+cellSize] == 1) aliveNeighbours++;
            // i
            if(checkBounds(i, j-cellSize, GRID_SIZE))
                if(grid[i][j-cellSize] == 1) aliveNeighbours++;
            if(checkBounds(i, j+cellSize, GRID_SIZE))
                if(grid[i][j+cellSize] == 1) aliveNeighbours++;
            // i+5
            if(checkBounds(i+cellSize, j-cellSize, GRID_SIZE))
                if(grid[i+cellSize][j-cellSize] == 1) aliveNeighbours++;
            if(checkBounds(i+cellSize, j, GRID_SIZE))
                if(grid[i+cellSize][j] == 1) aliveNeighbours++;
            if(checkBounds(i+cellSize, j+cellSize, GRID_SIZE))
                if(grid[i+cellSize][j+cellSize] == 1) aliveNeighbours++;

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
    InitWindow(GRID_SIZE, GRID_SIZE, "raylib");
    int grid[GRID_SIZE][GRID_SIZE] = { 0 };

    // blinker pattern
    // grid[250][245] = 1;
    // grid[250][250] = 1;
    // grid[250][255] = 1;

    // block pattern
    // grid[250][250] = 1;
    // grid[250][255] = 1;
    // grid[255][250] = 1;
    // grid[255][255] = 1;

    // R-pentomino
    // grid[250][255] = 1;
    // grid[250][260] = 1;
    // grid[255][250] = 1;
    // grid[255][255] = 1;
    // grid[260][255] = 1;

    // Acorn
    grid[250][250] = 1;
    grid[250][255] = 1;
    grid[255][255] = 1;
    grid[255][265] = 1;
    grid[255][270] = 1;
    grid[255][275] = 1;
    grid[255][280] = 1;

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);

        checkGrid(grid);

        usleep(300 * 1000);
        EndDrawing();
    }

    CloseWindow();
    return 0;
}
