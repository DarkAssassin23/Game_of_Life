#pragma once

#include <stdint.h>

/// Swap the front and the back grids
/// @param front Pointer to the 2D-Array that is the front grid
/// @param back Pointer to the 2D-Array that is the back grid
void swap_grids(uint8_t ***front, uint8_t*** back);

/// Initialize the front and the back grids
/// @param front Pointer to the 2D-Array that is the front grid
/// @param back Pointer to the 2D-Array that is the back grid
/// @param rows The number of rows in the grid
/// @param cols The number of columns in the grid
void init_grids(uint8_t ***front, uint8_t*** back, int rows, int cols);

/// Draws the grid to the screen
/// @param grid 2D-Array that is the grid
/// @param rows The number of rows in the grid
/// @param cols The number of columns in the grid
/// @param resolution Size of each of the cells are
void draw_grid(uint8_t **grid, int rows, int cols, int resolution);

/// Uses the rules of The Game of Life to update the living and dead cells
/// @param front Pointer to the 2D-Array that is the front grid
/// @param back Pointer to the 2D-Array that is the back grid
/// @param rows The number of rows in the grid
/// @param cols The number of columns in the grid
void update_grid(uint8_t ***front, uint8_t*** back, int rows, int cols);