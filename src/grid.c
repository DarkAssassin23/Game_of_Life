#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#include "defaults.h"
#include "raylib.h"
#include "grid.h"

static uint8_t** f;  // Pointer to the front grid
static uint8_t** b;  // Pointer to the back grid
static int g_rows;   // The total number of rows in the grid
static int g_cols;   // The total number of columns in the grid
static int res;      // The resolution of the grid (how big the squares are)
static bool pause;   // Is the simulation paused
static bool started; // If the thread pool has been created

/// Swap the front and the back grids
/// @param front Pointer to the 2D-Array that is the front grid
/// @param back Pointer to the 2D-Array that is the back grid
void swap_grids(uint8_t ***front, uint8_t*** back)
{
    uint8_t ** temp = *front;
    *front = *back;
    *back = temp;
}

/// Gets the state of the current cell for the next generation
/// @param grid 2D-Array of the front grid
/// @param row The row of the cell to get the state of 
/// @param col The col of the cell to get the state of
/// @return The sum of the cells surrounding neighbors
int get_cell_state(uint8_t **grid, int row, int col)
{
    int sum = 0;
    for(int i = -1; i <= 1; i++)
    {
        int r = (row + i + g_rows ) % g_rows;
        for(int j = -1; j <= 1; j++)
        {
            int c = (col + j + g_cols) % g_cols;
            sum += grid[r][c];
        }
    }
    // Don't include the current cell
    sum -= grid[row][col];

    return sum;
}

/// Uses the rules of The Game of Life to update the living and dead cells.
/// It also draws the grid to the screen.
/// @param rstart The row index to start at
/// @param rstop The row index to end at
void update(int rstart, int rstop)
{
    for(int row = rstart; row < rstop; row++)
    {
        for(int col = 0; col < g_cols; col++)
        {
            if(f[row][col] && num_threads == DEFAULT_THREADS)
            {
                int c = col * res;
                int r = row * res;
                DrawRectangle(r, c, 
                        res - 1, res - 1, 
                        RAYWHITE);
            }
            if(!pause)
            {
                int state = get_cell_state(f, row, col);
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
}

/// Update the grid using multithreading
/// @param args Pointer to a thread_args struct which contains the data 
/// needed for the given thread to update its portion of the grid
void* updated_grid_threads(void* args)
{
    thread_args* a = (thread_args*)args;
    pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
    while(running)
    {
        pthread_mutex_lock(&mutex);
        pthread_cond_wait(&a->work_to_do, &mutex);
        pthread_mutex_unlock(&mutex);
        update(a->rstart, a->rstop);
        a->done = true;
    }
    return NULL;
}

/// Initialize the threads
/// @note Assumes the threads and thread arguments arrays have been 
/// already initialized to have a size
void init_threads(void)
{
    int rows_per_thread = g_rows / num_threads;
    int leftover_rows = g_rows % num_threads;

    int rstart = 0;

    for(int x = 0; x < num_threads; x++)
    {
        int rstop = rstart + rows_per_thread;

        if(leftover_rows > 0)
        {
            rstop++;
            leftover_rows--;
        }
        thread_args arg = { 
            PTHREAD_COND_INITIALIZER, 
            rstart, rstop,  
            false
        };

        t_args[x] = arg;
        pthread_create(&threadPool[x], NULL, updated_grid_threads, &t_args[x]);
        rstart = rstop;
    }
    started = true;
}

/// Send signal to the threads letting them know there is work to do
void signal_threads(void)
{
    for(int x = 0; x < num_threads; x++)
        pthread_cond_signal(&t_args[x].work_to_do);
}

/// Join all the threads back togeather
void join_threads(void)
{
    for(int x = 0; x < num_threads; x++)
        pthread_join(threadPool[x], NULL);
}

void init_grids(uint8_t ***front, uint8_t*** back, 
                int rows, int cols,
                int resolution)
{
    uint8_t **f = *front;
    uint8_t **b = *back;
    g_rows = rows;
    g_cols = cols;
    res = resolution;

    for(int row = 0; row < rows; row++)
    {
        for(int col = 0; col < cols; col++)
        {
            b[row][col] = 0;
            f[row][col] = rand() % 2;
        }
    }
    if(num_threads > 1 && !started)
        init_threads();
}

void update_grid(uint8_t ***front, uint8_t*** back, bool paused)
{
    f = *front;
    b = *back;
    pause = paused;
    if(num_threads > 1)
    {
        // Raylib doesn't like when all the threads try to draw at the same time
        for(int row = 0; row < g_rows; row++)
        {
            for(int col = 0; col < g_cols; col++)
            {
                if(f[row][col])
                {
                    int c = col * res;
                    int r = row * res;
                    DrawRectangle(r, c, 
                            res - 1, res - 1, 
                            RAYWHITE);
                }
            }
        }
        signal_threads();
    }
    else
        update(0, g_rows);

    // Wait until all threads are done before swaping the grids
    if(num_threads > 1)
    {
        bool finished = false;
        while(!finished)
        {
            bool unfinished = false;
            for(int x = 0; x < num_threads; x++)
            {
                if(!t_args[x].done)
                {
                    unfinished = true;
                    break;
                }
            }
            finished = !unfinished;
        }
    }

    if(!paused)
        swap_grids(front, back);

    if(num_threads > 1)
    {
        for(int x = 0; x < num_threads; x++)
            t_args[x].done = false;
    }
}

void cleanup_grid(void)
{
    if(!started)
        return;
    signal_threads(); 
    join_threads();
    free(threadPool);
    free(t_args);
}