#pragma once
#include <stdio.h>

typedef struct {
    int width, height, resolution, speed, threads;
} Config_Screen_Properties;

/// Parse the config file and return its properties
/// @param config The config file to read the contents of
/// @return Properties of how the screen should be configured
Config_Screen_Properties parse_config(FILE* config);