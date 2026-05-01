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

#define IDX(i, j) ((i) * GRID_SIZE + (j))

// assumes square matrix
__device__ int checkBounds(int i, int j, int mtxSize){
    if(i < 0 || j < 0) return 0;
    else if(i > mtxSize-1 || j > mtxSize-1) return 0;
    else return 1;
}

void drawCell(int x, int y, Color color){
    DrawRectangle(x * CELL_SIZE, y*CELL_SIZE, CELL_SIZE, CELL_SIZE, color);
}

__device__ bool checkCell(int aliveNeighbours, bool dead){
    if(dead) return aliveNeighbours == 3;
    return aliveNeighbours == 2 || aliveNeighbours == 3;
}

void spawnPentomino(unsigned char *grid){
    int x = rand() % (GRID_SIZE-6);
    int y = rand() % (GRID_SIZE-6);
    grid[IDX(x,y+1)] = 1;
    grid[IDX(x,y+2)] = 1;
    grid[IDX(x+1,y)] = 1;
    grid[IDX(x+1,y+1)] = 1;
    grid[IDX(x+2,y+1)] = 1;
}

void spawnAcorn(unsigned char *grid){
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

__global__ void checkGrid(unsigned char *grid, unsigned char *auxGrid, int gridSize){
    int idx = blockIdx.x * blockDim.x + threadIdx.x;
    int total = gridSize * gridSize;

    if (idx >= total) return;

    int i = idx / gridSize;
    int j = idx % gridSize;

    bool dead = grid[idx] == 0;
    int aliveNeighbours = 0;

    if(checkBounds(i-1, j-1, gridSize) && grid[IDX(i-1,j-1)] == 1) aliveNeighbours++;
    if(checkBounds(i-1, j, gridSize) && grid[IDX(i-1,j)] == 1) aliveNeighbours++;
    if(checkBounds(i-1, j+1, gridSize) && grid[IDX(i-1,j+1)] == 1) aliveNeighbours++;
    // i
    if(checkBounds(i, j-1, gridSize) && grid[IDX(i,j-1)] == 1) aliveNeighbours++;
    if(checkBounds(i, j+1, gridSize) && grid[IDX(i,j+1)] == 1) aliveNeighbours++;
    // i+5
    if(checkBounds(i+1, j-1, gridSize) && grid[IDX(i+1,j-1)] == 1) aliveNeighbours++;
    if(checkBounds(i+1, j, gridSize) && grid[IDX(i+1,j)] == 1) aliveNeighbours++;
    if(checkBounds(i+1, j+1, gridSize) && grid[IDX(i+1,j+1)] == 1) aliveNeighbours++;

    auxGrid[idx] = checkCell(aliveNeighbours, dead) ? 1 : 0;
}

void drawGrid(unsigned char *grid){
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

    // arrays for cuda
    unsigned char *grid = (unsigned char *)calloc(total, sizeof(unsigned char));
    unsigned char *grid_cuda;
    unsigned char *aux_cuda;
    if(grid == NULL)
        return 1;

    int spawns = rand() % SPAWNS;
    for(int i = 0; i < spawns; i++){
        spawnAcorn(grid);
        spawnPentomino(grid);
    }

    cudaMalloc(&grid_cuda, total * sizeof(unsigned char));
    cudaMemcpy(grid_cuda, grid, total * sizeof(unsigned char), cudaMemcpyHostToDevice);

    cudaMalloc(&aux_cuda, total * sizeof(unsigned char));
    // cudaMemcpy(aux_cuda, grid, total * sizeof(unsigned char), cudaMemcpyHostToDevice);


    while (!WindowShouldClose()) {
        int threads = 256;
        int blocks = (total + threads - 1) / threads;

        checkGrid<<<blocks, threads>>>(grid_cuda, aux_cuda, GRID_SIZE);
        cudaError_t err = cudaGetLastError();
        if (err != cudaSuccess) {
            printf("CUDA error: %s\n", cudaGetErrorString(err));
        }
        cudaDeviceSynchronize();

        unsigned char *tmp = grid_cuda;
        grid_cuda = aux_cuda;
        aux_cuda = tmp;

        cudaMemcpy(grid, grid_cuda, total * sizeof(unsigned char), cudaMemcpyDeviceToHost);

        BeginDrawing();
        ClearBackground(WHITE);
        drawGrid(grid);
        // usleep(1000);
        EndDrawing();
    }

    cudaFree(grid_cuda);
    cudaFree(aux_cuda);
    free(grid);
    CloseWindow();
    return 0;
}
