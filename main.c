#include "raylib.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define CELL_SIZE 1
#define GRID_SIZE 3000
#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280
#define SPAWNS 5000

//#define IDX(i, j) ((i) * GRID_SIZE + (j))
#define IDX(i, j) ((size_t)(i) * GRID_SIZE + (j))

// assumes square matrix
int checkBounds(int i, int j, int mtxSize){
    if(i < 0 || j < 0) return 0;
    else if(i > mtxSize-1 || j > mtxSize-1) return 0;
    else return 1;
}

void drawCell(int x, int y, Color color){
    DrawRectangle(x * CELL_SIZE, y*CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
}

bool checkCell(int aliveNeighbours, bool dead){
    if(dead) return aliveNeighbours == 3;
    return aliveNeighbours == 2 || aliveNeighbours == 3;
}

void spawnPentomino(unsigned char * grid){
    int x = rand() % (GRID_SIZE-6);
    int y = rand() % (GRID_SIZE-6);
    grid[IDX(x,y+1)] = 1;
    grid[IDX(x,y+2)] = 1;
    grid[IDX(x+1,y)] = 1;
    grid[IDX(x+1,y+1)] = 1;
    grid[IDX(x+2,y+1)] = 1;
}

void spawnAcorn(unsigned char * grid){
    int x = rand() % (GRID_SIZE-6);
    int y = rand() % (GRID_SIZE-6);
    grid[IDX(x, y+1)] = 1;
    grid[IDX(x+1,y+3)] = 1;
    grid[IDX(x+2,y)] = 1;
    grid[IDX(x+2,y+1)] = 1;
    grid[IDX(x+2,y+4)] = 1;
    grid[IDX(x+2,y+5)] = 1;
    grid[IDX(x+2,y+6)] = 1;
}

// [i-5, j-5] [i, j-5] [i+5, j-5]
// [i-5,  j ] [ i, j ] [i+5,  j ]
// [i-5, j+5] [ i,j+5] [i+5, j+5]
void checkGrid(unsigned char * grid, unsigned char * auxGrid){
    bool dead = false;
    for(int i = 0; i < GRID_SIZE; i++){
        for(int j = 0; j < GRID_SIZE; j++){
            int aliveNeighbours = 0;
            dead = grid[IDX(i,j)] == 0 ? true : false;
            // i - 5
            if(checkBounds(i-1, j-1, GRID_SIZE))
                if(grid[IDX(i-1,j-1)] == 1) aliveNeighbours++;
            if(checkBounds(i-1, j, GRID_SIZE))
                if(grid[IDX(i-1,j)] == 1) aliveNeighbours++;
            if(checkBounds(i-1, j+1, GRID_SIZE))
                if(grid[IDX(i-1,j+1)] == 1) aliveNeighbours++;
            // i
            if(checkBounds(i, j-1, GRID_SIZE))
                if(grid[IDX(i,j-1)] == 1) aliveNeighbours++;
            if(checkBounds(i, j+1, GRID_SIZE))
                if(grid[IDX(i,j+1)] == 1) aliveNeighbours++;
            // i+5
            if(checkBounds(i+1, j-1, GRID_SIZE))
                if(grid[IDX(i+1,j-1)] == 1) aliveNeighbours++;
            if(checkBounds(i+1, j, GRID_SIZE))
                if(grid[IDX(i+1,j)] == 1) aliveNeighbours++;
            if(checkBounds(i+1, j+1, GRID_SIZE))
                if(grid[IDX(i+1,j+1)] == 1) aliveNeighbours++;

            if(checkCell(aliveNeighbours, dead)){
                auxGrid[IDX(i,j)] = 1;
            } else auxGrid[IDX(i,j)] = 0;
        }
    }
}

void drawGrid(unsigned char * grid){
    int visibleCols = WINDOW_WIDTH / CELL_SIZE;
    int visibleRows = WINDOW_HEIGHT / CELL_SIZE;

    for(int i = 0; i < visibleCols; i++){
        for(int j = 0; j < visibleRows; j++){
            if(grid[IDX(i,j)] == 1){
                drawCell(i, j, BLACK);
            }
        }
    }
}

// 0,0 5,5

int main() {
    srand(time(NULL));
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "raylib");

    size_t total = (size_t)GRID_SIZE * GRID_SIZE;

    unsigned char * grid = calloc(total, sizeof(unsigned char));
    unsigned char * auxGrid = calloc(total, sizeof(unsigned char));
    if(!grid || !auxGrid){
        return 1;
    }

    int spawns = rand() % SPAWNS;
    for(int i = 0; i < spawns; i++){
        spawnAcorn(grid);
        spawnPentomino(grid);
    }
    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);

        drawGrid(grid);
        checkGrid(grid, auxGrid);

        unsigned char *temp = grid;
        grid = auxGrid;
        auxGrid = temp;

        // usleep(1000);
        EndDrawing();
    }

    free(grid);
    free(auxGrid);
    CloseWindow();
    return 0;
}
