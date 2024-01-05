#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <pthread.h>

#include "raylib.h"
#include "grid.h"
#include "defaults.h"
#include "config_utils.h"

int MAX_WIDTH = 0;
int MAX_HEIGHT = 0;
int MIN_WIDTH = 0;
int MIN_HEIGHT = 0;

pthread_t* threadPool = NULL;
thread_args* t_args = NULL;
int num_threads = DEFAULT_THREADS;
bool running = true;

int main(int argc, char** argv) 
{
    // Initialize global screen resolution
    // InitWindow needed to start the GLFW context in order to get monitor dims
    InitWindow(DEFAULT_WIDTH, DEFAULT_HEIGHT, "The Game of Life"); 
    MAX_WIDTH = fmax(DEFAULT_WIDTH, GetMonitorWidth(0));
    MAX_HEIGHT = fmax(DEFAULT_HEIGHT, GetMonitorHeight(0));
    MIN_WIDTH = fmin(DEFAULT_WIDTH, GetMonitorWidth(0));
    MIN_HEIGHT = fmin(DEFAULT_HEIGHT, GetMonitorHeight(0));
    CloseWindow();
    
    int screenWidth = MIN_WIDTH;
    int screenHeight = MIN_HEIGHT;
    int resolution = DEFAULT_RES;
    int fps = DEFAULT_FPS;

    FILE* cfg = fopen("config.txt", "r");
    if(cfg != NULL)
    {
        Config_Screen_Properties csp = parse_config(cfg);
        screenHeight = csp.height;
        screenWidth = csp.width;
        resolution = csp.resolution;
        fps = csp.speed;
        num_threads = csp.threads;
    }
    fclose(cfg);

    if(num_threads > 1)
    {
        threadPool = malloc(sizeof(pthread_t) * num_threads);
        t_args = malloc(sizeof(thread_args) * num_threads);
    }

    InitWindow(screenWidth, screenHeight, "The Game of Life"); 
    if(screenWidth == MAX_WIDTH && screenHeight == MAX_HEIGHT)
        ToggleFullscreen(); 

    int rows = screenWidth / resolution;
    int cols = screenHeight / resolution;
    bool paused = false;
    
    uint8_t **front_grid = malloc(rows * sizeof(uint8_t*));
    uint8_t **back_grid = malloc(rows * sizeof(uint8_t*));

    for(int row = 0; row < rows; row++)
    {
        front_grid[row] = malloc(cols * sizeof(uint8_t));
        back_grid[row] = malloc(cols * sizeof(uint8_t));
    }

    srand(time(NULL));

    init_grids(&front_grid, &back_grid, rows, cols, resolution);

    SetTargetFPS(fps);

    while (!WindowShouldClose()) 
    {
        if(IsKeyPressed(KEY_ENTER))
            init_grids(&front_grid, &back_grid, rows, cols, resolution);

        if(IsKeyPressed(KEY_SPACE))
            paused = !paused;

        if(IsMouseButtonDown(MOUSE_LEFT_BUTTON))
        {
            Vector2 mousePos = GetMousePosition();
            int x = mousePos.x / resolution;
            int y = mousePos.y / resolution;
            front_grid[x][y] = 1;
        }

        BeginDrawing();

        ClearBackground(BLACK);

        update_grid(&front_grid, &back_grid, paused);

        EndDrawing();
    }
    running = false;
    
    // Free our memeory
    cleanup_grid();
    for(int row = 0; row < rows; row++)
    {
        free(front_grid[row]);
        free(back_grid[row]);
    }
    free(front_grid);
    free(back_grid);

    CloseWindow();

    return 0;
}

