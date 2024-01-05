#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <pthread.h>

typedef struct
{
    pthread_cond_t work_to_do;
    int rstart, rstop;
    bool done;
} thread_args;

extern pthread_t* threadPool;
extern thread_args* t_args;
extern int num_threads;
extern bool running;

/// Initialize the front and the back grids
/// @param front Pointer to the 2D-Array that is the front grid
/// @param back Pointer to the 2D-Array that is the back grid
/// @param rows The total number of rows in the grid
/// @param cols The total number of columns in the grid
/// @param resolution Size of each of the cells are
void init_grids(uint8_t ***front, uint8_t*** back, 
                int rows, int cols, 
                int resolution);

/// Uses the rules of The Game of Life to update the living and dead cells.
/// It also draws the grid to the screen.
/// @param front Pointer to the 2D-Array that is the front grid
/// @param back Pointer to the 2D-Array that is the back grid
/// @param paused If the simulation is paused, if true, no update is needed
void update_grid(uint8_t ***front, uint8_t*** back, bool paused);

/// Clean up the grid by joining all threads and freeing their memory
void cleanup_grid(void);