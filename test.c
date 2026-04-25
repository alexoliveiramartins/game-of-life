#include <stdio.h>

int main(){
    int grid[3][3] = { 0 };

    grid[0][1] = 1;
    grid[0][2] = 1;
    grid[1][1] = 1;
    grid[0][1] = 1;
    grid[1][0] = 1;
    grid[2][1] = 1;
    for(int i =0;i<3;i++){
        for(int j=0;j<3;j++){
            printf("| %d |", grid[i][j]);
        }
        printf("\n");
    }

}
