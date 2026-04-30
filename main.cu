#include "raylib.h"
#include <stdio.h>
#include <unistd.h>
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

const int cellSize = 3;
#define GRID_SIZE 500
#define WINDOW_HEIGHT 720
#define WINDOW_WIDTH 1280

#define IDX(i, j) ((i) * GRID_SIZE + (j))

// assumes square matrix
__device__ int checkBounds(int i, int j, int mtxSize){
    if(i < 0 || j < 0) return 0;
    else if(i > mtxSize-1 || j > mtxSize-1) return 0;
    else return 1;
}

void drawCell(int x, int y, Color color){
    DrawRectangle(x * cellSize, y*cellSize, cellSize, cellSize, color);
}

__device__ bool checkCell(int aliveNeighbours, bool dead){
    if(dead) return aliveNeighbours == 3;
    return aliveNeighbours == 2 || aliveNeighbours == 3;
}

void spawnPentomino(int *grid){
    int x = rand() % (GRID_SIZE-6);
    int y = rand() % (GRID_SIZE-6);
    grid[IDX(x,y+1)] = 1;
    grid[IDX(x,y+2)] = 1;
    grid[IDX(x+1,y)] = 1;
    grid[IDX(x+1,y+1)] = 1;
    grid[IDX(x+2,y+1)] = 1;
}

void spawnAcorn(int *grid){
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
__global__ void checkGrid(int *grid, int *auxGrid, int gridSize){
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int total = gridSize * gridSize;

    if (idx >= total) return;

    int i = idx / gridSize;
    int j = idx % gridSize;

    bool dead = grid[idx] == 0;
    int aliveNeighbours = 0;

    if(checkBounds(i-1, j-1, GRID_SIZE) && grid[IDX(i-1,j-1)] == 1) aliveNeighbours++;
    if(checkBounds(i-1, j, GRID_SIZE) && grid[IDX(i-1,j)] == 1) aliveNeighbours++;
    if(checkBounds(i-1, j+1, GRID_SIZE) && grid[IDX(i-1,j+1)] == 1) aliveNeighbours++;
    // i
    if(checkBounds(i, j-1, GRID_SIZE) && grid[IDX(i,j-1)] == 1) aliveNeighbours++;
    if(checkBounds(i, j+1, GRID_SIZE) && grid[IDX(i,j+1)] == 1) aliveNeighbours++;
    // i+5
    if(checkBounds(i+1, j-1, GRID_SIZE) && grid[IDX(i+1,j-1)] == 1) aliveNeighbours++;
    if(checkBounds(i+1, j, GRID_SIZE) && grid[IDX(i+1,j)] == 1) aliveNeighbours++;
    if(checkBounds(i+1, j+1, GRID_SIZE) && grid[IDX(i+1,j+1)] == 1) aliveNeighbours++;

    auxGrid[idx] = checkCell(aliveNeighbours, dead) ? 1 : 0;
}

void drawGrid(int *grid){
    for(int i = 0; i < GRID_SIZE; i++){
        for(int j = 0; j < GRID_SIZE; j++){
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

    // arrays for cuda
    int *grid = (int *)calloc(GRID_SIZE * GRID_SIZE, sizeof(int));
    int *grid_cuda;
    int *auxGrid = (int *)calloc(GRID_SIZE * GRID_SIZE, sizeof(int));
    int *aux_cuda;

    int spawns = rand() % 400;
    for(int i = 0; i < spawns; i++){
        spawnAcorn(grid);
        spawnPentomino(grid);
    }

    cudaMalloc(&grid_cuda, GRID_SIZE * GRID_SIZE * sizeof(int));
    cudaMemcpy(grid_cuda, grid, GRID_SIZE * GRID_SIZE * sizeof(int), cudaMemcpyHostToDevice);

    cudaMalloc(&aux_cuda, GRID_SIZE * GRID_SIZE * sizeof(int));
    cudaMemcpy(aux_cuda, grid, GRID_SIZE * GRID_SIZE * sizeof(int), cudaMemcpyHostToDevice);


    while (!WindowShouldClose()) {
        int total = GRID_SIZE * GRID_SIZE;
        int threads = 256;
        int blocks = (total + threads - 1) / threads;

        BeginDrawing();
        ClearBackground(WHITE);
        drawGrid(grid);

        checkGrid<<<blocks, threads>>>(grid_cuda, aux_cuda, GRID_SIZE);
        cudaError_t err = cudaGetLastError();
        if (err != cudaSuccess) {
            printf("CUDA error: %s\n", cudaGetErrorString(err));
        }
        cudaDeviceSynchronize();

        int *tmp = grid_cuda;
        grid_cuda = aux_cuda;
        aux_cuda = tmp;

        cudaMemcpy(grid, grid_cuda, total * sizeof(int), cudaMemcpyDeviceToHost);

        usleep(1000);
        EndDrawing();
    }

    cudaFree(grid_cuda);
    cudaFree(aux_cuda);
    free(grid);
    free(auxGrid);
    CloseWindow();
    return 0;
}
