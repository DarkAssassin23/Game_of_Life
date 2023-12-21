#include <stdint.h>
#include <stdlib.h>

#include "raylib.h"
#include "grid.h"

void swap_grids(uint8_t ***front, uint8_t*** back)
{
    uint8_t ** temp = *front;
    *front = *back;
    *back = temp;
}

void init_grids(uint8_t ***front, uint8_t*** back, int rows, int cols)
{
    uint8_t **f = *front;
    uint8_t **b = *back;
    for(int row = 0; row < rows; row++)
    {
        for(int col = 0; col < cols; col++)
        {
            b[row][col] = 0;
            f[row][col] = rand() % 2;
        }
    }
}

void draw_grid(uint8_t **grid, int rows, int cols, int resolution)
{
    for(int row = 0; row < rows; row++)
    {
        for(int col = 0; col < cols; col++)
        {
            if(grid[row][col])
            {
                int c = col * resolution;
                int r = row * resolution;
                DrawRectangle(r, c, 
                        resolution - 1, resolution - 1, 
                        RAYWHITE);
            }
        }
    }
}

/// Gets the state of the current cell for the next generation
/// @param grid 2D-Array of the front grid
/// @param row The row of the cell to get the state of 
/// @param col The col of the cell to get the state of
/// @param rows The number of rows in the grid
/// @param cols The number of columns in the grid
/// @return The sum of the cells surrounding neighbors
int get_cell_state(uint8_t **grid, int row, int col, int rows, int cols)
{
    int sum = 0;
    for(int i = -1; i <= 1; i++)
    {
        int r = (row + i + rows ) % rows;
        for(int j = -1; j <= 1; j++)
        {
            int c = (col + j + cols) % cols;
            sum += grid[r][c];
        }
    }
    // Don't include the current cell
    sum -= grid[row][col];

    return sum;
}

void update_grid(uint8_t ***front, uint8_t*** back, int rows, int cols)
{
    uint8_t **f = *front;
    uint8_t **b = *back;
    for(int row = 0; row < rows; row++)
    {
        for(int col = 0; col < cols; col++)
        {
            int state = get_cell_state(f, row, col, rows, cols);
            // ref: https://en.wikipedia.org/wiki/Conway%27s_Game_of_Life#Rules
            if(f[row][col] && (state < 2 || state > 3))
                b[row][col] = 0;
            else if(!f[row][col] && state == 3)
                b[row][col] = 1;
            else
                b[row][col] = f[row][col];
        }
    }
}