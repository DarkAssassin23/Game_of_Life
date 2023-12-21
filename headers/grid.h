#pragma once

#include <stdint.h>
#include <stdbool.h>

/// Initialize the front and the back grids
/// @param front Pointer to the 2D-Array that is the front grid
/// @param back Pointer to the 2D-Array that is the back grid
/// @param rows The number of rows in the grid
/// @param cols The number of columns in the grid
void init_grids(uint8_t ***front, uint8_t*** back, int rows, int cols);

/// Uses the rules of The Game of Life to update the living and dead cells.
/// It also draws the grid to the screen.
/// @param front Pointer to the 2D-Array that is the front grid
/// @param back Pointer to the 2D-Array that is the back grid
/// @param paused If the simulation is paused, if true, no update is needed
/// @param rows The number of rows in the grid
/// @param cols The number of columns in the grid
/// @param resolution Size of each of the cells are
void update_grid(uint8_t ***front, uint8_t*** back, bool paused,
                int rows, int cols, int resolution);