#include "raylib.h"

const int cellSize = 5;

void desenharCelula(int x, int y){
    DrawRectangle(x * cellSize, y * cellSize, cellSize, cellSize, BLACK);
}

int main() {
    InitWindow(500, 500, "raylib");

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(WHITE);

        // primeira celula
        desenharCelula(50, 50);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
